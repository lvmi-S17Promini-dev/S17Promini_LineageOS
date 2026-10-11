// Targeted stock-camera analysis; import the verified raw Image at its VA base.
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.SourceType;
import java.io.File;
import java.io.PrintWriter;
import java.nio.file.Files;
import java.nio.file.Path;

public class S17CameraDecompile extends GhidraScript {
    protected void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 2) throw new IllegalArgumentException("bounds TSV and output directory required");
        File out = new File(args[1]);
        out.mkdirs();
        long base = Long.parseUnsignedLong("ffffff8008080000", 16);
        Address failAddress = toAddr(base + 0x35798);
        Function fail = getFunctionAt(failAddress);
        if (fail == null) fail = currentProgram.getFunctionManager().createFunction(
            "__stack_chk_fail", failAddress, new AddressSet(failAddress), SourceType.USER_DEFINED);
        fail.setNoReturn(true);
        DecompInterface decomp = new DecompInterface();
        try {
            if (!decomp.openProgram(currentProgram)) throw new IllegalStateException("Cannot open program");
            for (String line : Files.readAllLines(Path.of(args[0]))) {
                String[] cols = line.split("\\t");
                Address start = toAddr(base + Long.parseLong(cols[0], 16));
                Address end = toAddr(base + Long.parseLong(cols[1], 16) - 1);
                disassemble(start);
                Function f = getFunctionAt(start);
                if (f == null) f = createFunction(start, cols[2]);
                if (f == null) throw new IllegalStateException("Cannot create " + cols[2]);
                f.setName(cols[2], SourceType.USER_DEFINED);
                f.setBody(new AddressSet(start, end));
            }
            for (String line : Files.readAllLines(Path.of(args[0]))) {
                String[] cols = line.split("\\t");
                Function f = getFunctionAt(toAddr(base + Long.parseLong(cols[0], 16)));
                DecompileResults result = decomp.decompileFunction(f, 60, monitor);
                if (!result.decompileCompleted()) throw new IllegalStateException(result.getErrorMessage());
                try (PrintWriter writer = new PrintWriter(new File(out, cols[2] + ".c"))) {
                    writer.println("/* Stock Image offsets 0x" + cols[0] + "..0x" + cols[1] + " */");
                    writer.println(result.getDecompiledFunction().getC()
                        .replaceAll("(?m)[ \\t]+$", "").stripTrailing());
                }
                println("S17 camera decompiled " + cols[2]);
            }
        } finally { decomp.dispose(); }
    }
}
