// Refine known callback signatures before repeating the targeted decompilation.
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Parameter;
import ghidra.program.model.listing.ParameterImpl;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.data.IntegerDataType;
import ghidra.program.model.data.VoidDataType;
import ghidra.program.model.symbol.SourceType;

public class S17SensorSignatures extends GhidraScript {
    protected void run() throws Exception {
        long base = Long.parseUnsignedLong("ffffff8008080000", 16);
        Function report = getFunctionAt(toAddr(base + 0x62c590));
        report.setReturnType(IntegerDataType.dataType, SourceType.USER_DEFINED);
        report.replaceParameters(Function.FunctionUpdateType.DYNAMIC_STORAGE_ALL_PARAMS,
            true, SourceType.USER_DEFINED,
            new ParameterImpl("value", IntegerDataType.dataType, currentProgram));
        Function count = getFunctionAt(toAddr(base + 0x1f01c));
        if (count == null) count = currentProgram.getFunctionManager().createFunction(
            "_mcount", toAddr(base + 0x1f01c), new AddressSet(toAddr(base + 0x1f01c)),
            SourceType.USER_DEFINED);
        count.setReturnType(VoidDataType.dataType, SourceType.USER_DEFINED);
        count.replaceParameters(Function.FunctionUpdateType.DYNAMIC_STORAGE_ALL_PARAMS,
            true, SourceType.USER_DEFINED, new Parameter[0]);
    }
}
