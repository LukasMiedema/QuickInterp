package jdk.internal.vm.si.impl;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import jdk.internal.vm.si.impl.asm.tree.AbstractInsnNode;
import jdk.internal.vm.si.impl.asm.tree.InsnList;
import jdk.internal.vm.si.impl.asm.tree.SuperInstrInsnNode;
import jdk.internal.vm.si.impl.bytecode.InstructionDefinition;

/**
 * Aware of superinstructions and able to perform basic substitution.
 * 
 * The substitution algorithm works as follows:
 * - a map of instruction tuples is constructed, mapping to a superinstruction
 * 	 e.g. (iload_1, iload_2) -> iload_1,2, or (iload_1,2, iload_3) -> iload_1,2,3
 * - when writing an instruction to the instruction stream, the previous instruction and the current
 *   instruction are checked in the instruction tuple map
 * - substitution is always eager and for longer instructions can only happen if all prefix instructions also exist.
 * @author lukas
 *
 */
public class ImprovedTreeSiConverter implements AsmSiConverter {
	
	private ImprovedSuperinstructionTree superinstructionTree = null;
	
	@Override
	public void setInstructions(List<InstructionDefinition> instructionSet) {
		this.superinstructionTree = ImprovedSuperinstructionTree.build(instructionSet);
	}
	
	private static AbstractInsnNode getNextNonLabelNode(AbstractInsnNode node) {
		while (node != null && (node = node.getNext()) != null && node.getType() == AbstractInsnNode.LABEL);
		return node;
	}
	

	public void convert(InsnList instructions) {
		var windowStart = instructions.getFirst();
		var windowEnd = instructions.getFirst();
		InstructionDefinition lastValidSuperinstruction = null;
		
		// Start and tail are the two pointers, where the distance they span is considered as a superinstruction
		// lastValidSuperinstruction is exactly that
		var cursor = this.superinstructionTree.cursor();
		do {			
			if (windowEnd == null || !cursor.lookup(windowEnd.getOpcode())) {
				// Lookup failed --> no superinstruction possible anymore
				if (lastValidSuperinstruction != null) {
					// The superinstruction replaces the windowStart node
					windowStart = new SuperInstrInsnNode(lastValidSuperinstruction, windowStart, instructions);
				}
				
				// Restart the search for the next instruction
				cursor = this.superinstructionTree.cursor();
				windowStart = getNextNonLabelNode(windowStart);
				windowEnd = windowStart;
				lastValidSuperinstruction = null;
			} else {
				if (cursor.hasDefinition())
					lastValidSuperinstruction = cursor.getDefinition();
				windowEnd = getNextNonLabelNode(windowEnd);
			}
		} while (windowStart != null);
	}
}
