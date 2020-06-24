package jdk.internal.vm.si.impl;

import java.lang.System.Logger;
import java.lang.System.Logger.Level;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import jdk.internal.vm.si.impl.EquivalenceSuperinstructionMap.LookupResult;
import jdk.internal.vm.si.impl.asm.tree.AbstractInsnNode;
import jdk.internal.vm.si.impl.asm.tree.InsnList;
import jdk.internal.vm.si.impl.bytecode.BytecodePrimitive;
import jdk.internal.vm.si.impl.bytecode.InstructionDefinition;
import jdk.internal.vm.si.impl.bytecode.BytecodePrimitive.BytecodeFlag;

public class EquivalenceSiConverter extends ShortestPathSiConverter {
	
	private EquivalenceSuperinstructionMap superinstructionMap = null;
	private final Logger log = System.getLogger(getClass().getName());

	@Override
	public void setInstructions(List<InstructionDefinition> instructionSet) {
		super.setInstructions(instructionSet);
		this.superinstructionMap = EquivalenceSuperinstructionMap.buildMap(instructionSet);
	}
	
	@Override
	public void convert(InsnList instructions) {
		super.convert(instructions);
	}



	@Override
	protected Map<AbstractInsnNode, List<SuperinstructionData>> makeBackedges(InsnList instructions) {
		// Two-step process
		// 1. Eagerly find all (longest) superinstructions from start to finish w/ equivalence
		//    "Inner" superinstructions are not searched for, and instructions are reordered to make the substitutions POSSIBLE
		//    No actual substitutions are made
		// 2. Run regular ShortestPathEquivalence

		
		// Step 1. Find
		// window is inclusive
		var currentPrimitives = new ArrayList<BytecodePrimitive>();
		var currentWindow = new ArrayList<AbstractInsnNode>();
		
		// windowStart is modified when a superinstruction is found
		for (var windowStart = instructions.getFirst(); windowStart != null; windowStart = windowStart.getNext()) {
			if (windowStart.getType() == AbstractInsnNode.LABEL)
				continue;
			
			currentPrimitives.clear();
			currentWindow.clear();

			LookupResult bestResult = null;

			for (var windowEnd = windowStart; windowEnd != null; windowEnd = windowEnd.getNext()) {
				// Lookup window may never contain labels, so exit when there's a label
				if (windowEnd.getType() == AbstractInsnNode.LABEL)
					break;
				
				// Lookup only needs to be concerned with superinstructionable instructions
				var primitive = BytecodePrimitive.byOpcode(windowEnd.getOpcode());
				if (primitive.hasFlag(BytecodeFlag.NO_SUPERINSTRUCTION))
					break;
				
				currentPrimitives.add(primitive);
				currentWindow.add(windowEnd);
				
				// Lookup
				if (currentPrimitives.size() >= 2) {
					LookupResult result = superinstructionMap.lookup(currentPrimitives);
					if (result.hasResult()) {
						bestResult = result; // it is always better as it's longer
					}
				}
			}
			
			// Reorder instruction, but do not place the opcode (it will be rediscovered later on)
			if (bestResult != null && !bestResult.hasTrivialBijection()) {
				var bijection = bestResult.getInstructionBijection();
				var window = currentWindow.subList(0, bijection.length);
				
				log.log(Level.INFO, "Reordering " + window.size() + " instructions to place superinstruction " + bestResult.getDefinition().getOpcode());
				
				// Connect to the first node
				var oldFirst = window.get(0);
				var newFirst = window.get(bijection[0]);
				if (instructions.getFirst() == oldFirst) {
					 // First node of the window is the first node in the instrlist -> update instrlist start
					instructions.setFirst(newFirst);
					newFirst.setPrevious(null);
				} else {
					// First node of the window has a previous
					var previous = oldFirst.getPrevious();
					previous.setNext(newFirst);
					newFirst.setPrevious(previous);
				}
				
				// Connect to the last node
				var oldLast = window.get(window.size() - 1);
				var newLast = window.get(bijection[window.size() - 1]);
				if (oldLast.getNext() == null) { // last node in the program
					newLast.setNext(null);
					break; // terminate search as the program ends here
				} else {
					var afterLast = oldLast.getNext();
					newLast.setNext(afterLast);
					afterLast.setPrevious(newLast);
					windowStart = afterLast;
				}
				
				// Reconnect all window nodes within the window
				for (int i = 0; i < bijection.length - 1; i++) {
					var node = window.get(bijection[i]);
					var nextNode = window.get(bijection[i + 1]);
					
					node.setNext(nextNode);
					nextNode.setPrevious(node);
				}
			}
		}
		
		
		// Step 2. This will find all instructions we found by reordering instructions
		return super.makeBackedges(instructions);
	}

}
