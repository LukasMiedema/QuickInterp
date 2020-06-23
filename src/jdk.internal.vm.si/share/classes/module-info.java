import jdk.internal.vm.si.ClassBytecodeFormatConverter;
import jdk.internal.vm.si.impl.ClassBytecodeFormatConverterImpl;

module jdk.internal.vm.si {
	requires java.base;

	provides ClassBytecodeFormatConverter with ClassBytecodeFormatConverterImpl;
	
	exports jdk.internal.vm.si.impl.bytecode;
}