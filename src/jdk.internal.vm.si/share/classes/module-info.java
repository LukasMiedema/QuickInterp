module jdk.internal.vm.si {
	requires java.base;
	
    provides jdk.internal.vm.si.ClassBytecodeFormatConverter with
    jdk.internal.vm.si.impl.ClassBytecodeFormatConverterImpl;
}