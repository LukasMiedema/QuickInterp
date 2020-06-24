package jdk.internal.vm.si.impl.asm.tree;

import java.util.Map;

import jdk.internal.vm.si.impl.asm.MethodVisitor;
import jdk.internal.vm.si.impl.bytecode.BytecodePrimitive;

/**
 * A very stripped-down label with profiling information.
 * Since it represents incoming jumps, it carries an additional "inbound" frequency counter for the number
 * of jumps towards this target.
 * The value of "weight" already includes this "inbound" number.
 * @author lukas
 *
 */
public class SyntheticLabelNode extends AbstractInsnNode implements ProfileableInsnNode {

	private final long weight;
	private final long inbound;

	public SyntheticLabelNode(BytecodePrimitive primitive, long weight, long inbound) {
		super(primitive.getInstr());
		this.weight = weight;
		this.inbound = inbound;
	}
	
	public SyntheticLabelNode(int opcode, long weight, long inbound) {
		super(opcode);
		this.weight = weight;
		this.inbound = inbound;
	}

	@Override
	public int getType() {
		return LABEL;
	}

	@Override
	public void accept(MethodVisitor methodVisitor) {
		throw new UnsupportedOperationException();
	}

	@Override
	public AbstractInsnNode clone(Map<LabelNode, LabelNode> clonedLabels) {
		return new SyntheticLabelNode(opcode, weight, inbound).cloneAnnotations(this);
	}

	public long getWeight() {
		return weight;
	}
	
	public long getInbound() {
		return inbound;
	}

	@Override
	public boolean hasProfileData() {
		return true;
	}
}
