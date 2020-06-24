package jdk.internal.vm.si.impl;

import java.util.ArrayList;
import java.util.List;
import java.util.ListIterator;

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
 * @author Lukas Miedema
 *
 */
public class ClassicTableSiConverter implements AsmSiConverter {
	
	private ClassicSuperinstructionTable superinstructionTable = null;
	
	@Override
	public void setInstructions(List<InstructionDefinition> instructionSet) {
		this.superinstructionTable = ClassicSuperinstructionTable.buildTable(instructionSet);
	}
	

	public void convert(InsnList instructions) {
		var windowStart = instructions.getFirst();
		
		do {
			if (windowStart.getType() == AbstractInsnNode.LABEL) continue;
			var windowEnd = windowStart;
			while ((windowEnd = windowEnd.getNext()) != null) {
				if (windowEnd.getType() == AbstractInsnNode.LABEL) continue;
				InstructionDefinition foundInstr = superinstructionTable
						.lookup(windowStart.getOpcode(), windowEnd.getOpcode());
				if (foundInstr == null) {
					// Go to the next start window
					break;
				} else {
					// Superinstruction is placed at window start
					// This may wrap one node multiple times but that's okay
					windowStart = new SuperInstrInsnNode(foundInstr, windowStart, instructions);
				}
			}
			
		} while ((windowStart = windowStart.getNext()) != null);
	}
}
