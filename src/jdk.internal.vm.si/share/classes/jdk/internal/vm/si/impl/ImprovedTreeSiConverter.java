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
	

	public void convert(InsnList instructions) {
		var start = instructions.getFirst();
		var end = instructions.getFirst();
		InstructionDefinition lastValidSuperinstruction = null;
		
		// Start and tail are the two pointers, where the distance they span is considered as a superinstruction
		// lastValidSuperinstruction is exactly that
		
		var cursor = this.superinstructionTree.cursor();
		do {
			if (cursor.hasDefinition())
				lastValidSuperinstruction = cursor.getDefinition();
			
			if (end == null || !cursor.lookup(end.getOpcode())) {
				// Lookup failed --> no superinstruction possible anymore
				if (lastValidSuperinstruction != null) {
					// Place this superinstruction
					int length = lastValidSuperinstruction.getPrimitives().size();
					var tail = new ArrayList<AbstractInsnNode>(length - 1);
					var it = instructions.iterator(start);
					it.next();
					
					for (int i = 1; i < length; i++) {
						var next = it.next();
						it.remove();
						tail.add(next);
					}
					// System.out.println("Creating " + start.getOpcode() + " [" + tail.stream().map(n -> Integer.toString(n.getOpcode())).collect(Collectors.joining(", ")) + "]");
					var superInstruction = new SuperInstrInsnNode(lastValidSuperinstruction, start, tail);
					instructions.set(start, superInstruction);
					start = superInstruction; // "start" itself is no longer valid
				}
				
				// Restart the search for the next instruction
				cursor = this.superinstructionTree.cursor();
				start = start.getNext();
				end = start;
				lastValidSuperinstruction = null;
			} else {
				end = end.getNext();
			}
		} while (start != null);
	}
}
