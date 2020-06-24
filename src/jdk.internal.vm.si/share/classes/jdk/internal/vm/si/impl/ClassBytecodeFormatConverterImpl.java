package jdk.internal.vm.si.impl;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintWriter;

import jdk.internal.vm.si.ClassBytecodeFormatConverter;
import jdk.internal.vm.si.impl.asm.ClassReader;
import jdk.internal.vm.si.impl.asm.ClassWriter;
import jdk.internal.vm.si.impl.asm.util.TraceClassVisitor;
import jdk.internal.vm.si.impl.naive.TableSiConverter;

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
		var writer = new ClassWriter(0);

		var converter = new TableSiConverter();
		converter.convert(reader, writer);
		
		return writer.toByteArray();
	}
}
