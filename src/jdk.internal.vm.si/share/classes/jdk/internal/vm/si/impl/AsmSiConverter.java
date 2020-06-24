package jdk.internal.vm.si.impl;

import jdk.internal.vm.si.impl.asm.ClassReader;
import jdk.internal.vm.si.impl.asm.ClassVisitor;

public interface AsmSiConverter {
	public void convert(ClassReader reader, ClassVisitor writer);
}
