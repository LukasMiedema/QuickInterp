package jdk.internal.vm.si;

public interface ClassBytecodeFormatConverter {
	
	public byte[] convertClass(byte[] b, int off, int len);
	public byte[] convertClass(java.nio.ByteBuffer b, int off, int len);
}
