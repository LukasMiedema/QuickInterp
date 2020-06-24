package jdk.internal.vm.si.impl.asm.tree;

import java.util.List;
import java.util.Map;
import java.util.Objects;

import jdk.internal.vm.si.impl.asm.Label;
import jdk.internal.vm.si.impl.asm.MethodVisitor;
import jdk.internal.vm.si.impl.bytecode.InstructionDefinition;

public class SuperInstrInsnNode extends AbstractInsnNode {

	private final AbstractInsnNode head;
	private final List<AbstractInsnNode> tail;
	private final InstructionDefinition definition;
	
	/**
	 * Construct a new superinstruction node.
	 * @param opcode the opcode of this superinstruction.
	 * @param head the first instruction this opcode replaces -- operands from that instruction will be taken.
	 * @param tail the rest of the instructions covered by this superinstruction, which will be visited immediately
	 * following this instruction. May also be superinstructions in of themselves.
	 */
	public SuperInstrInsnNode(InstructionDefinition definition, AbstractInsnNode head, List<AbstractInsnNode> tail) {
		super(definition.getOpcode());
		this.definition = definition;
		this.head = Objects.requireNonNull(head);
		this.tail = Objects.requireNonNull(tail);
	}

	@Override
	public int getType() {
		return AbstractInsnNode.SUPERINSTR_INSN;
	}

	@Override
	public void accept(MethodVisitor methodVisitor) {
		// Write the opcode
		methodVisitor.visitOpcode(opcode);
		acceptOperands(methodVisitor);
	}

	private void acceptOperands(MethodVisitor methodVisitor) {
		switch (head.getType()) {
		case FIELD_INSN:
			var fieldInsn = (FieldInsnNode) head;
			methodVisitor.visitFieldInsnOperands(fieldInsn.opcode, fieldInsn.owner, fieldInsn.name, fieldInsn.desc);
			break;
		case IINC_INSN:
			var iincInsn = (IincInsnNode) head;
			methodVisitor.visitIincInsnOperands(iincInsn.var, iincInsn.incr);
			break;
		case INSN:
			methodVisitor.visitInsnOperands(opcode); // probably does nothing
			break;
		case INT_INSN:
			var intInsn = (IntInsnNode) head;
			methodVisitor.visitIntInsnOperands(intInsn.opcode, intInsn.operand);
			break;
		case INVOKE_DYNAMIC_INSN:
			var invokeDynamicInsn = (InvokeDynamicInsnNode) head;
			methodVisitor.visitInvokeDynamicInsnOperands(invokeDynamicInsn.name, invokeDynamicInsn.desc, invokeDynamicInsn.bsm, invokeDynamicInsn.bsmArgs);
			break;
		case JUMP_INSN:
			var jumpInsn = (JumpInsnNode) head;
			methodVisitor.visitJumpInsnOperands(jumpInsn.opcode, jumpInsn.label.getLabel());
			break;
		case LDC_INSN:
			var ldcInsn = (LdcInsnNode) head;
			methodVisitor.visitLdcInsnOperands(ldcInsn.opcode, ldcInsn.cst);
			break;
		case LOOKUPSWITCH_INSN:
			var lookupSwitchInsn = (LookupSwitchInsnNode) head;
			int[] keysArray = new int[lookupSwitchInsn.keys.size()];
			for (int i = 0, n = keysArray.length; i < n; ++i) {
				keysArray[i] = lookupSwitchInsn.keys.get(i).intValue();
			}
			Label[] labelsArray = new Label[lookupSwitchInsn.labels.size()];
			for (int i = 0, n = labelsArray.length; i < n; ++i) {
				labelsArray[i] = lookupSwitchInsn.labels.get(i).getLabel();
			}
			methodVisitor.visitLookupSwitchInsnOperands(lookupSwitchInsn.dflt.getLabel(), keysArray, labelsArray);
			break;
		case METHOD_INSN:
			var methodInsn = (MethodInsnNode) head;
			methodVisitor.visitMethodInsnOperands(methodInsn.opcode, methodInsn.owner, methodInsn.name, methodInsn.desc, methodInsn.itf);
			break;
		case MULTIANEWARRAY_INSN:
			var multiANewArrayInsn = (MultiANewArrayInsnNode) head;
			methodVisitor.visitMultiANewArrayInsnOperands(multiANewArrayInsn.desc, multiANewArrayInsn.dims);
			break;
		case TABLESWITCH_INSN:
			var tableSwitchInsn = (TableSwitchInsnNode) head;
			labelsArray = new Label[tableSwitchInsn.labels.size()];
			for (int i = 0, n = labelsArray.length; i < n; ++i) {
				labelsArray[i] = tableSwitchInsn.labels.get(i).getLabel();
			}
			methodVisitor.visitTableSwitchInsnOperands(tableSwitchInsn.min, tableSwitchInsn.max, tableSwitchInsn.dflt.getLabel(), labelsArray);
			break;
		case TYPE_INSN:
			var typeInsnNode = (TypeInsnNode) head;
			methodVisitor.visitTypeInsnOperands(typeInsnNode.opcode, typeInsnNode.desc);
			break;
		case VAR_INSN:
			var varInsnNode = (VarInsnNode) head;
			methodVisitor.visitVarInsnOperands(varInsnNode.opcode, varInsnNode.var);
			break;
		case SUPERINSTR_INSN:
			var superInsnNode = (SuperInstrInsnNode) head;
			superInsnNode.acceptOperands(methodVisitor);
			break;
		default:
			throw new IllegalStateException("Unsupported node type in superinstruction: " + head.getClass().getName());
		}
		
		head.acceptAnnotations(methodVisitor);
		
		for (var node : tail) {
			node.accept(methodVisitor);
		}
	}

	@Override
	public AbstractInsnNode clone(Map<LabelNode, LabelNode> clonedLabels) {
		return null;
	}

	public InstructionDefinition getDefinition() {
		return definition;
	}

}
