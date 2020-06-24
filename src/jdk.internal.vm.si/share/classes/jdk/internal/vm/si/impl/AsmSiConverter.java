package jdk.internal.vm.si.impl;

import java.util.List;

import jdk.internal.vm.si.impl.asm.tree.AbstractInsnNode;
import jdk.internal.vm.si.impl.asm.tree.InsnList;
import jdk.internal.vm.si.impl.bytecode.InstructionDefinition;

/**
 * Base interface for ASM superinstruction placement algorithms.
 * @author lukas
 *
 */
public interface AsmSiConverter {

	/**
	 * Set the instruction set. This method may be called multiple times.
	 * @param instructionSet
	 */
	public void setInstructions(List<InstructionDefinition> instructionSet);
	
	/**
	 * Convert an ASM list of instructions.
	 * Note that during instruction set optimization this method may be called with
	 * 'synthetic' instructions w/ no operands, and no {@link AbstractInsnNode#accept(jdk.internal.vm.si.impl.asm.MethodVisitor)} method.
	 * During IS optimization the InsnList also need not be a full method, but rather a CodeSegment or derivative. 
	 * @param instructions
	 */
	public void convert(InsnList instructions);

	/**
	 * The human-readable name of this converter.
	 * @return
	 */
	public default String getName() {
		return this.getClass().getName();
	}
	
	// TODO: implement capabilities
	static enum Capabilities {
		/**
		 * This converter supports overlapping code segments.
		 * Code segments with incoming jumps in the middle can still be
		 * converted to a superinstruction by writing two superinstructions:
		 * code:
		 *   instrA
		 *   <incoming jump>
		 *   instrB
		 *   instrC
		 * code:
		 *   instrABC
		 *   <incoming jump>
		 *   instrBC
		 * With the knowledge that the generated superinstruction ABC jumps over instrBC.
		 * This flag indicates to the ProfileReader that segments need to be generated for this case.    
		 */
		OVERLAPPING_SEGMENTS,
		
		/**
		 * This converter supports non-identity equivalence.
		 * Two superinstructions may be equivalent but not identical. As such,
		 * there's no use adding the two variants to the superinstructionset if
		 * one will suffice.
		 * This flag indicates to the InstructionSetOptimizer that equivalent superinstructions
		 * can be pruned prior to testing their performance.
		 */
		NON_IDENTITY_EQUIVALENCE,
		
		
	}
}
