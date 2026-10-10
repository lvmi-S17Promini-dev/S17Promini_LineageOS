// Targeted, read-only analysis of the S17 stock LK crash-log restore path.
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.lang.Register;
import ghidra.program.model.symbol.SourceType;
import java.math.BigInteger;
import java.io.File;
import java.io.PrintWriter;

public class S17PstoreDecompile extends GhidraScript {
    protected void run() throws Exception {
        File out = new File(getScriptArgs()[0]);
        out.mkdirs();
        Register thumb = currentProgram.getRegister("TMode");
        if (getInstructionAt(toAddr(0x480205f4L)) == null)
            currentProgram.getProgramContext().setValue(thumb, toAddr(0x48000000L),
                toAddr(0x480adefbL), BigInteger.ONE);
        long[] entries = {0x480204d0L, 0x480205bcL, 0x480205f4L,
            0x4802174cL, 0x48023a60L, 0x48023b40L, 0x48023e14L,
            0x48024270L, 0x48024590L};
        String[] names = {"kedump_open_expdb", "kedump_crc", "kedump_restore_expdb",
            "kedump_entry", "ram_console_pmic_reset_flag", "ram_console_fault_check",
            "ram_console_should_restore", "get_pstore_region", "ram_console_set_restore"};
        DecompInterface dec = new DecompInterface();
        dec.openProgram(currentProgram);
        for (int i = 0; i < entries.length; i++) {
            monitor.checkCancelled();
            Address addr = toAddr(entries[i]);
            disassemble(addr);
            Function f = getFunctionAt(addr);
            if (f == null) f = createFunction(addr, names[i]);
            if (f == null) throw new IllegalStateException("Cannot create " + names[i]);
            f.setName(names[i], SourceType.USER_DEFINED);
            DecompileResults result = dec.decompileFunction(f, 60, monitor);
            try (PrintWriter writer = new PrintWriter(new File(out, names[i] + ".c"))) {
                writer.println("// Analyst-assigned name; validate meaning against ARM instructions.");
                if (result.decompileCompleted())
                    writer.println(result.getDecompiledFunction().getC());
                else writer.println(result.getErrorMessage());
            }
        }
        dec.dispose();
    }
}
