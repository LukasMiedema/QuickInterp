package jdk.internal.vm.si.impl;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintWriter;

import jdk.internal.vm.si.ClassBytecodeFormatConverter;
import jdk.internal.vm.si.impl.asm.ClassReader;
import jdk.internal.vm.si.impl.asm.ClassVisitor;
import jdk.internal.vm.si.impl.asm.ClassWriter;
import jdk.internal.vm.si.impl.asm.MethodVisitor;
import jdk.internal.vm.si.impl.asm.Opcodes;
import jdk.internal.vm.si.impl.asm.commons.GeneratorAdapter;
import jdk.internal.vm.si.impl.asm.util.TraceClassVisitor;

public class ClassBytecodeFormatConverterImpl implements ClassBytecodeFormatConverter {
	
	public byte[] convertClass(byte[] input) {
		writeToFile(input, new File("beforejava.class"));
		
		byte[] result = convertClassInternal(new ClassReader(input));

		writeToFile(result, new File("afterjava.class"));
		return result;
	}
	
	private static void writeToFile(byte[] bytes, File file) {
		try (FileOutputStream stream = new FileOutputStream(file)) {
		    stream.write(bytes);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	private static byte[] convertClassInternal(ClassReader reader) {
		var writer = new PrintWriter(System.out);
		var classWriter = new ClassWriter(0);
		var tracer = new TraceClassVisitor(classWriter, writer);
		
		var classTransformer = new NaiveSiClassTransformer(tracer);
		
		reader.accept(classTransformer, 0);
		return classWriter.toByteArray();
	}
	
	static class NaiveSiClassTransformer extends ClassVisitor {

		public NaiveSiClassTransformer(ClassVisitor delegate) {
			super(Opcodes.ASM7, delegate);
		}

		@Override
		public MethodVisitor visitMethod(int access, String name, String descriptor, String signature,
				String[] exceptions) {
			MethodVisitor delegate = super.visitMethod(access, name, descriptor, signature, exceptions);
			return new NaiveSiMethodTransformer(delegate, access, name, descriptor);
		}		
	}
	
	static class NaiveSiMethodTransformer extends GeneratorAdapter {

		public NaiveSiMethodTransformer(MethodVisitor delegate, int access, String name, String descriptor) {
			super(Opcodes.ASM7, delegate, access, name, descriptor);
		}
	}
}
