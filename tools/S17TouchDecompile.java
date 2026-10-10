// Headless, targeted stock-kernel AArch64 decompilation. No device access.
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Parameter;
import ghidra.program.model.listing.ParameterImpl;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.model.data.VoidDataType;
import ghidra.program.model.data.IntegerDataType;
import ghidra.program.model.data.DataType;
import ghidra.program.model.data.PointerDataType;
import ghidra.program.model.data.ByteDataType;
import ghidra.program.model.data.UnsignedIntegerDataType;

import java.io.File;
import java.io.PrintWriter;

public class S17TouchDecompile extends GhidraScript {
    private void parameters(Function f, DataType... types) throws Exception {
        Parameter[] params = new Parameter[types.length];
        for (int i = 0; i < types.length; i++)
            params[i] = new ParameterImpl("arg" + i, types[i], currentProgram);
        f.replaceParameters(Function.FunctionUpdateType.DYNAMIC_STORAGE_ALL_PARAMS,
            true, SourceType.USER_DEFINED, params);
    }
    private static final long BASE = Long.parseUnsignedLong("ffffff8008080000", 16);
    private static final long[] OFFSETS = {
        0x9f84a8, 0x9f8558, 0x9f8598, 0x9f886c,
        0x9f9b30, 0x9f9c9c, 0x9f9dcc,
        0x9fb4d4, 0x9fb5a0, 0x9fb644, 0x9fb968,
        0x9fba7c, 0x9fbcb8, 0x9fbdf8, 0x9fc00c, 0x9fc1e8,
        0x9fc2ec, 0x9fc40c, 0x9fc82c, 0x9fc8fc, 0x9fca64, 0x9fcbf8, 0x9fcc8c
    };
    private static final String[] NAMES = {
        "axs_read_version", "axs_reset_output", "axs_reset_proc", "axs_probe",
        "axs_write_read", "axs_raw_read", "axs_raw_write",
        "axs_select_flash", "axs_flash_read_init", "axs_upgrade_gate", "axs_flash_ready",
        "axs_flash_init", "axs_flash_erase", "axs_flash_write", "axs_flash_verify", "axs_upgrade",
        "axs_upgrade_init", "axs_upgrade_work", "axs_select_pram", "axs_download_app",
        "axs_pram_write", "axs_download_finish", "axs_pram_download"
    };
    private static final long[] ENDS = {
        0x9f8558, 0x9f8598, 0x9f865c, 0x9f926c,
        0x9f9c9c, 0x9f9dcc, 0x9f9efc,
        0x9fb5a0, 0x9fb644, 0x9fb968, 0x9fba7c,
        0x9fbcb8, 0x9fbdf8, 0x9fc00c, 0x9fc1e8, 0x9fc2ec,
        0x9fc40c, 0x9fc580, 0x9fc8fc, 0x9fca64, 0x9fcbf8, 0x9fcc8c, 0x9fcdcc
    };

    @Override
    protected void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 1) throw new IllegalArgumentException("output directory required");
        File out = new File(args[0]);
        out.mkdirs();
        Address failAddr = toAddr(BASE + 0x35798);
        Function fail = getFunctionAt(failAddr);
        if (fail == null) fail = currentProgram.getFunctionManager().createFunction(
            "__stack_chk_fail", failAddr, new AddressSet(failAddr), SourceType.USER_DEFINED);
        fail.setNoReturn(true);
        fail.setReturnType(VoidDataType.dataType, SourceType.USER_DEFINED);
        for (int i = 0; i < OFFSETS.length; i++) {
            monitor.checkCancelled();
            Address entry = toAddr(BASE + OFFSETS[i]);
            disassemble(entry);
            Function f = getFunctionAt(entry);
            if (f == null) f = createFunction(entry, NAMES[i]);
            if (f == null) throw new IllegalStateException("Cannot create " + NAMES[i]);
            f.setName(NAMES[i], ghidra.program.model.symbol.SourceType.USER_DEFINED);
            // Prevent unknown noreturn calls from merging adjacent routines.
            // Boundaries are independently verified AArch64 prologues/returns.
            f.setBody(new AddressSet(entry, toAddr(BASE + ENDS[i] - 1)));
            boolean voidResult = NAMES[i].equals("axs_reset_output") ||
                NAMES[i].equals("axs_reset_proc") || NAMES[i].equals("axs_upgrade_work");
            f.setReturnType(voidResult ? VoidDataType.dataType : IntegerDataType.dataType,
                SourceType.USER_DEFINED);
            DataType bytes = new PointerDataType(ByteDataType.dataType);
            DataType uint = UnsignedIntegerDataType.dataType;
            if (NAMES[i].equals("axs_raw_write") || NAMES[i].equals("axs_raw_read") ||
                NAMES[i].equals("axs_flash_write") || NAMES[i].equals("axs_pram_write") ||
                NAMES[i].equals("axs_pram_download")) parameters(f, bytes, uint);
            if (NAMES[i].equals("axs_write_read")) parameters(f, bytes, uint, bytes, uint);
            if (NAMES[i].equals("axs_flash_ready") || NAMES[i].equals("axs_flash_verify") ||
                NAMES[i].equals("axs_reset_output") || NAMES[i].equals("axs_reset_proc"))
                parameters(f, uint);
        }
        DecompInterface decomp = new DecompInterface();
        try {
            if (!decomp.openProgram(currentProgram)) throw new IllegalStateException("Decompiler open failed");
            for (int i = 0; i < OFFSETS.length; i++) {
                monitor.checkCancelled();
                Function f = getFunctionAt(toAddr(BASE + OFFSETS[i]));
                DecompileResults r = decomp.decompileFunction(f, 60, monitor);
                try (PrintWriter w = new PrintWriter(new File(out, NAMES[i] + ".c"))) {
                    w.println("/* Stock Image offset 0x" + Long.toHexString(OFFSETS[i]) + " */");
                    if (!r.decompileCompleted()) throw new IllegalStateException(r.getErrorMessage());
                    w.print(r.getDecompiledFunction().getC());
                }
                println("S17 decompiled " + NAMES[i]);
            }
        } finally {
            decomp.dispose();
        }
    }
}
