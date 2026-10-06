#!/usr/bin/env python3
"""Patch this exact S17Promini LK and re-sign its changed cert2 offline.

Dependencies: cryptography. Assemble lk_key_entry.S with the supplied linker
script before running. This program never accesses a device.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct

from cryptography.hazmat.primitives import hashes, serialization
from cryptography.hazmat.primitives.asymmetric import padding

ROOT = Path(__file__).resolve().parents[1]
STOCK_SHA256 = 'a745cfc9f513aba14bf461f90253e0824a778172621868f53b4d4ac01b6d7864'
BASE = 0x48000000
PATCHES = (
    (0x3a6a, '01f085f9', 'hook.bin', 4),
    (0x3af0, '40f004fa04462148784424f07dfb204640f0fcf93128fad9', 'slot.bin', 24),
)
CERT1_START, CERT1_SIZE = 0xae300, 0x6ad
CERT2_START, CERT2_SIZE = 0xaebb0, 0x3d6
DATA_OID = bytes.fromhex('060760867693160201')
HEADER_OID = bytes.fromhex('060760867693160204')


def require(condition, message):
    if not condition:
        raise ValueError(message)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def tlv(data, pos):
    """Return tag, content start, end for one definite-length DER item."""
    tag = data[pos]
    first = data[pos + 1]
    if first & 0x80:
        count = first & 0x7f
        require(0 < count <= 4, 'Invalid DER length')
        size = int.from_bytes(data[pos + 2:pos + 2 + count], 'big')
        start = pos + 2 + count
    else:
        size, start = first, pos + 2
    end = start + size
    require(end <= len(data), 'Truncated DER')
    return tag, start, end


def certificate_fields(cert):
    tag, start, end = tlv(cert, 0)
    require(tag == 0x30 and end == len(cert), 'Invalid certificate container')
    tbs_tag, _, tbs_end = tlv(cert, start)
    alg_tag, _, alg_end = tlv(cert, tbs_end)
    sig_tag, sig_start, sig_end = tlv(cert, alg_end)
    require(tbs_tag == alg_tag == 0x30 and sig_tag == 3 and sig_end == end,
            'Unexpected MTK certificate layout')
    require(cert[sig_start] == 0, 'Signature has unused bits')
    return (start, tbs_end), (sig_start + 1, sig_end)


def certificate_public_key(cert):
    rsa_oid = bytes.fromhex('06092a864886f70d010101')
    index = cert.index(rsa_oid)
    for pos in range(max(0, index - 20), index):
        if cert[pos] != 0x30:
            continue
        try:
            _, _, end = tlv(cert, pos)
            return serialization.load_der_public_key(cert[pos:end])
        except ValueError:
            continue
    raise ValueError('RSA SubjectPublicKeyInfo not found')


def hash_slot(cert, oid):
    require(cert.count(oid) == 1, 'Hash OID missing or ambiguous')
    pos = cert.index(oid) + len(oid)
    tag, start, end = tlv(cert, pos)
    if tag == 3:
        require(cert[start] == 0, 'Hash has unused bits')
        start += 1
    else:
        require(tag == 4, 'Unexpected hash encoding')
    require(end - start == 32, 'Expected SHA256 hash')
    return start, end


def verify_cert(cert, pub):
    (ts, te), (ss, se) = certificate_fields(cert)
    pub.verify(cert[ss:se], cert[ts:te],
               padding.PSS(mgf=padding.MGF1(hashes.SHA256()), salt_length=padding.PSS.AUTO),
               hashes.SHA256())


def payload_hash(image):
    size = struct.unpack_from('<I', image, 4)[0]
    alignment = struct.unpack_from('<I', image, 0x44)[0]
    require(size == 0xadefc and alignment == 16, 'Unexpected LK header')
    return hashlib.sha256(image[512:512 + size] + bytes((-size) % alignment)).digest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--revision', choices=('r1', 'r2'), default='r1')
    parser.add_argument('--stock', type=Path, default=ROOT / 'stock_imgs/lk.bin')
    parser.add_argument('--assembled', type=Path)
    parser.add_argument('--keys', type=Path, default=ROOT / 'ref/mtk-lk-tools/keys')
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    if args.assembled is None:
        args.assembled = ROOT / ('extracted/lk-key-entry' if args.revision == 'r1'
                                 else 'extracted/lk-key-entry-r2')
    if args.output is None:
        args.output = ROOT / ('artifacts/lk-volup-fastboot-r1.img' if args.revision == 'r1'
                              else 'artifacts/lk-keys-fastboot-r2.img')
    stock = args.stock.read_bytes()
    require(sha(stock) == STOCK_SHA256, 'Refusing to patch a different stock LK')
    require(not args.output.exists(), 'Output exists; choose a new output path')
    private = serialization.load_pem_private_key((args.keys / 'img_prvk.pem').read_bytes(), None)
    root_public = serialization.load_pem_public_key((args.keys / 'root_pubk.pem').read_bytes())
    cert1 = stock[CERT1_START:CERT1_START + CERT1_SIZE]
    cert2 = stock[CERT2_START:CERT2_START + CERT2_SIZE]
    image_public = certificate_public_key(cert2)
    require(private.public_key().public_numbers() == image_public.public_numbers(),
            'Signing key does not match the stock image certificate')
    root_der = root_public.public_bytes(serialization.Encoding.DER,
                                        serialization.PublicFormat.SubjectPublicKeyInfo)
    image_der = image_public.public_bytes(serialization.Encoding.DER,
                                          serialization.PublicFormat.SubjectPublicKeyInfo)
    require(root_der in cert1 and image_der in cert1, 'Stock certificate key chain mismatch')
    verify_cert(cert1, root_public)
    verify_cert(cert2, image_public)
    ds, de = hash_slot(cert2, DATA_OID)
    hs, he = hash_slot(cert2, HEADER_OID)
    require(cert2[ds:de] == payload_hash(stock), 'Stock payload hash mismatch')
    require(cert2[hs:he] == hashlib.sha256(stock[:512]).digest(), 'Stock header hash mismatch')

    patched = bytearray(stock)
    ranges = []
    patches = []
    if args.revision == 'r1':
        for offset, original_hex, filename, size in PATCHES:
            patches.append((offset, bytes.fromhex(original_hex),
                            (args.assembled / filename).read_bytes(), size))
    else:
        compiled = (args.assembled / 'boot-mode.bin').read_bytes()
        require(len(compiled) <= 408 and len(compiled) % 2 == 0,
                'Replacement does not fit the stock function')
        replacement = compiled + bytes.fromhex('00bf') * ((408 - len(compiled)) // 2)
        patches.append((0x39fc, stock[0x39fc:0x3b94], replacement, 408))
        marker_original = b'Your device will boot in 5 seconds\n\0'
        marker = b'S17-LK-r2: boot in 5 seconds\n\0'
        marker = marker.ljust(len(marker_original), b'\0')
        patches.append((0x7da94, marker_original, marker, len(marker_original)))
    for offset, original, code, size in patches:
        require(len(code) == len(original) == size, 'Patch size mismatch')
        require(stock[offset:offset + size] == original, 'Original instruction mismatch')
        patched[offset:offset + size] = code
        ranges.append({'file_offset': hex(offset), 'virtual_address': hex(BASE + offset - 512),
                       'size': size, 'before': original.hex(), 'after': code.hex()})

    new_cert = bytearray(cert2)
    new_cert[ds:de] = payload_hash(patched)
    (ts, te), (ss, se) = certificate_fields(new_cert)
    signature = private.sign(bytes(new_cert[ts:te]),
                             padding.PSS(mgf=padding.MGF1(hashes.SHA256()), salt_length=32),
                             hashes.SHA256())
    require(len(signature) == se - ss == 256, 'Unexpected signature length')
    new_cert[ss:se] = signature
    verify_cert(bytes(new_cert), image_public)
    patched[CERT2_START:CERT2_START + CERT2_SIZE] = new_cert
    ranges.extend([
        {'file_offset': hex(CERT2_START + ds), 'size': 32, 'purpose': 'LK cert2 payload SHA256'},
        {'file_offset': hex(CERT2_START + ss), 'size': 256, 'purpose': 'LK cert2 RSA-PSS signature'},
    ])
    allowed = {i for r in ranges for i in range(int(r['file_offset'], 16),
                                               int(r['file_offset'], 16) + r['size'])}
    changes = {i for i, (old, new) in enumerate(zip(stock, patched)) if old != new}
    require(len(patched) == len(stock) and changes <= allowed, 'Unexpected image changes')
    require(patched[:512] == stock[:512], 'Header changed')
    require(patched[0xaef90:] == stock[0xaef90:], 'DTB or its certificates changed')
    require(patched[CERT1_START:CERT1_START + CERT1_SIZE] == cert1, 'Root certificate changed')
    require(new_cert[ds:de] == payload_hash(patched), 'Signed payload hash mismatch')
    require(new_cert[hs:he] == hashlib.sha256(patched[:512]).digest(), 'Signed header hash mismatch')
    report = {
        'stock_sha256': sha(stock), 'output_sha256': sha(patched), 'image_size': len(patched),
        'revision': args.revision,
        'code_patch_bytes': 28 if args.revision == 'r1' else 408,
        'actual_changed_bytes': len(changes), 'ranges': ranges,
        'key_hw_codes': [17] if args.revision == 'r1' else [0, 1, 17, 19],
        'key_evidence': 'stock LK unlock affirmative branch and stock dtbo keypad map',
        'poll_window_ms': 0 if args.revision == 'r1' else 3000,
        'poll_max_iterations': 1 if args.revision == 'r1' else 300,
        'watchdog_restart_during_poll': args.revision == 'r2',
        'orange_state_marker': None if args.revision == 'r1' else 'S17-LK-r2',
        'root_spki_sha256': sha(root_der), 'image_spki_sha256': sha(image_der),
        'signature_scheme': 'RSA-PSS/SHA256/MGF1-SHA256, 32-byte salt',
        'cert1_original_signature_verified': True, 'stock_cert2_signature_verified': True,
        'modified_cert2_signature_verified': True, 'header_hash_verified': True,
        'payload_hash_verified': True, 'dtb_and_dtb_certificates_unchanged': True,
        'hardware_validated': False,
        'remaining': 'Active LK selection and physical cold-boot key tests not performed',
    }
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(patched)
    args.output.with_suffix('.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report, indent=2))


if __name__ == '__main__':
    main()
