package jdk.internal.vm.si.impl;

import jdk.internal.org.objectweb.asm.ClassReader;
import jdk.internal.org.objectweb.asm.ClassVisitor;
import jdk.internal.org.objectweb.asm.ClassWriter;
import jdk.internal.org.objectweb.asm.Opcodes;
import jdk.internal.vm.si.ClassBytecodeFormatConverter;

public class ClassBytecodeFormatConverterImpl implements ClassBytecodeFormatConverter {
	
	public byte[] convertClass(byte[] b, int off, int len) {
		return b;
		//return convertClassInternal(new ClassReader(b, off, len));
	}

	public byte[] convertClass(java.nio.ByteBuffer b, int off, int len) {
		byte[] output = new byte[len];
		b.get(output, off, len);
		return output;
		//return convertClassInternal(new ClassReader(output));
	}
	
	
	private static byte[] convertClassInternal(ClassReader reader) {
		var classVisitor = new ConverterClassVisitor();
		var classWriter = new ClassWriter(0);
		
		System.out.println("Reading " + reader.getClassName());
		
		//reader.accept(classVisitor, ClassReader.SKIP_DEBUG);
		reader.accept(classWriter, ClassReader.SKIP_DEBUG);
		
		return classWriter.toByteArray();
	}
	
	static class ConverterClassVisitor extends ClassVisitor {

		public ConverterClassVisitor() {
			super(Opcodes.ASM6);
		}
		
	}
}
