package jdk.internal.vm.si.impl.naive;

import java.io.File;
import java.io.FileInputStream;
import java.io.ObjectInputStream;
import java.lang.System.Logger.Level;
import java.util.Collections;
import java.util.List;

import jdk.internal.vm.si.impl.AsmSiConverter;
import jdk.internal.vm.si.impl.asm.ClassReader;
import jdk.internal.vm.si.impl.asm.ClassVisitor;
import jdk.internal.vm.si.impl.asm.tree.ClassNode;
import jdk.internal.vm.si.impl.asm.tree.MethodNode;
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
@SuppressWarnings("unchecked")
public class TableSiConverter implements AsmSiConverter {
	
	private static final String SUPERINSTRUCTION_FILE = "/home/lukas/Documents/School/Q8/code/eclipse-cdt-workspace/jdk.internal.vm.si.generator/superinstructions.list";
	private static final SuperinstructionTable SUPERINSTRUCTION_TABLE;
	private static final System.Logger LOG = System.getLogger(TableSiConverter.class.getName()); 
	
	static {
		File file = new File(SUPERINSTRUCTION_FILE);
		List<InstructionDefinition> instrList;
		try (var ois = new ObjectInputStream(new FileInputStream(file))) {
			instrList = (List<InstructionDefinition>) ois.readObject();
		} catch (Exception e) {
			LOG.log(Level.ERROR, "Could not initialize superinstruction table", e);
			instrList = Collections.emptyList();
		}
		SUPERINSTRUCTION_TABLE = SuperinstructionTable.buildTable(instrList);
		//LOG.log(Level.INFO, "Instruction table initialized with " + instrList.size() + " instructions (total)");
		LOG.log(Level.INFO, SUPERINSTRUCTION_TABLE.dump());
	}
	

	public void convert(ClassReader reader, ClassVisitor writer) {
		ClassNode node = new ClassNode();
		reader.accept(node, ClassReader.SKIP_FRAMES | ClassReader.SKIP_DEBUG);
		
		// Now we have a fully populated node --> modify it and write it again
		this.convert(node);
		
		// And write the changes back
		node.accept(writer);
	}

	private void convert(ClassNode node) {
		for (MethodNode method : node.methods) {
			if (method.instructions.size() > 0) {
				var it = method.instructions.iterator();
				var previousInstr = it.next();
				
				while (it.hasNext()) {
					var instr = it.next();
					InstructionDefinition foundInstr = SUPERINSTRUCTION_TABLE
							.lookup(previousInstr.getOpcode(),
									instr.getOpcode());
					//System.out.println("Testing (0x" + Integer.toString(previousInstr.getOpcode(), 16) + ",0x" + Integer.toString(instr.getOpcode(), 16) + ")");
					if (foundInstr != null) {
						int opcode = foundInstr.getOpcode();
						
						
						// Replace the previous instruction with a superinstruction
						var newInstruction = new SuperInstrInsnNode(opcode, previousInstr, List.of(instr));
						it.previous();
						it.previous();
						it.set(newInstruction);
						it.next();
						it.next();
						
						// Remove the second instruction, it will be wrapped and part of the super instruction node
						it.remove();
						
//						System.out.println("> Substitution: (0x" + Integer.toString(previousInstr.getOpcode(), 16) + ",0x"
//								+ Integer.toString(instr.getOpcode(), 16) + ") -> 0x"
//								+ Integer.toString(newInstruction.getOpcode(), 16));
						
						// Report the replaced instruction as the previous one, as it spans more than one instruction
						// Leave the 'current' instruction in place. It will be skipped at runtime unless it's a branch target
						previousInstr = newInstruction;
					} else {
						previousInstr = instr;
					}
				}
			}
		}
	}
}
