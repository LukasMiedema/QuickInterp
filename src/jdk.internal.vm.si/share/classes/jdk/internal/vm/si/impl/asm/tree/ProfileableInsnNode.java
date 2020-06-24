package jdk.internal.vm.si.impl.asm.tree;

public interface ProfileableInsnNode {
	
	public boolean hasProfileData();
	
	public long getWeight();

}
