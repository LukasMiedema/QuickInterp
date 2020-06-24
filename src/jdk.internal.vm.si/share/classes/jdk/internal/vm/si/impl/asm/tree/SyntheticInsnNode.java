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
public class SyntheticInsnNode extends AbstractInsnNode {

	public SyntheticInsnNode(BytecodePrimitive primitive) {
		super(primitive.getInstr());
	}
	
	public SyntheticInsnNode(int opcode) {
		super(opcode);
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
		return new SyntheticInsnNode(opcode).cloneAnnotations(this);
	}

}
