package jdk.internal.vm.si.impl;

import java.util.List;

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
		var it = instructions.iterator();
		var previousInstr = it.next();
		
		while (it.hasNext()) {
			var instr = it.next();
			InstructionDefinition foundInstr = superinstructionTable
					.lookup(previousInstr.getOpcode(), instr.getOpcode());
			//System.out.println("Testing (0x" + Integer.toString(previousInstr.getOpcode(), 16) + ",0x" + Integer.toString(instr.getOpcode(), 16) + ")");
			if (foundInstr != null) {
				
				// Replace the previous instruction with a superinstruction
				var newInstruction = new SuperInstrInsnNode(foundInstr, previousInstr, List.of(instr));
				it.previous();
				it.previous();
				it.set(newInstruction);
				it.next();
				it.next();
				
				// Remove the second instruction, it will be wrapped and part of the super instruction node
				it.remove();

				// Report the replaced instruction as the previous one, as it spans more than one instruction
				// Leave the 'current' instruction in place. It will be skipped at runtime unless it's a branch target
				previousInstr = newInstruction;
			} else {
				previousInstr = instr;
			}
		}
	}
}
