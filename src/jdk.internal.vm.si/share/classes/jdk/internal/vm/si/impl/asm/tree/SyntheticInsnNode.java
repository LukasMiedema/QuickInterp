package jdk.internal.vm.si.impl.asm.tree;

import java.util.Map;

import jdk.internal.vm.si.impl.asm.MethodVisitor;
import jdk.internal.vm.si.impl.bytecode.BytecodePrimitive;

/**
 * A very stripped-down node which makes it possible to create an InsnList from
 * a CodeSegment. It can represent any instruction, but will never contain
 * any operands so reconstructing the bytecode when this is in your InsnList
 * is not possible.
 * @author lukas
 *
 */
public class SyntheticInsnNode extends AbstractInsnNode implements ProfileableInsnNode {

	private final long weight;

	public SyntheticInsnNode(BytecodePrimitive primitive, long weight) {
		super(primitive.getInstr());
		this.weight = weight;
	}
	
	public SyntheticInsnNode(int opcode, long weight) {
		super(opcode);
		this.weight = weight;
	}

	@Override
	public int getType() {
		return SYNTHETIC_INSN;
	}

	@Override
	public void accept(MethodVisitor methodVisitor) {
		throw new UnsupportedOperationException();
	}

	@Override
	public AbstractInsnNode clone(Map<LabelNode, LabelNode> clonedLabels) {
		return new SyntheticInsnNode(opcode, weight).cloneAnnotations(this);
	}

	public long getWeight() {
		return weight;
	}

	@Override
	public boolean hasProfileData() {
		return true;
	}
}
