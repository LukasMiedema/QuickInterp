package jdk.internal.vm.si;

import java.util.ServiceLoader;

/**
 * Simple proxy to convert between static and {@link ServiceLoader} calls.
 * @author Lukas Miedema
 */
public class ClassBytecodeFormatConverterProxy {
	
	private static final ClassBytecodeFormatConverter IMPL = ServiceLoader
			.load(ClassBytecodeFormatConverter.class)
			.findFirst()
			.orElseThrow();
	
	public static byte[] convertClass(byte[] b) {
		return IMPL.convertClass(b);
	}
}
