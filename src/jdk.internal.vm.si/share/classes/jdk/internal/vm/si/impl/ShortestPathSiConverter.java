package jdk.internal.vm.si.impl;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import jdk.internal.vm.si.impl.asm.tree.AbstractInsnNode;
import jdk.internal.vm.si.impl.asm.tree.InsnList;
import jdk.internal.vm.si.impl.bytecode.InstructionDefinition;

/**
 * This converter uses the {@link ImprovedSuperinstructionTree} to index and find superinstructions.
 * @author Lukas Miedema
 *
 */
public class ShortestPathSiConverter extends BaseShortestPathSiConverter {

	private ImprovedSuperinstructionTree superinstructionTree = null;
	
	@Override
	public void setInstructions(List<InstructionDefinition> instructionSet) {
		this.superinstructionTree = ImprovedSuperinstructionTree.build(instructionSet);
	}
	
	private static AbstractInsnNode getNextNonLabelNode(AbstractInsnNode node) {
		while (node != null && (node = node.getNext()) != null && node.getType() == AbstractInsnNode.LABEL);
		return node;
	}
	
	@Override
	protected Map<AbstractInsnNode, List<SuperinstructionData>> makeBackedges(InsnList instructions) {
		var backedges = new HashMap<AbstractInsnNode, List<SuperinstructionData>>();
		
		// Basically run the tree algorithm, but place in the map instead of the IsnList
		// Also, instead of always trying to find the largest superinstruction, find all and add them to the list
		var windowStart = instructions.getFirst();
		var windowEnd = instructions.getFirst();
		backedges.computeIfAbsent(getNextNonLabelNode(windowStart), n -> new ArrayList<>())
			.add(new SuperinstructionData(null, windowStart));
		
		var cursor = this.superinstructionTree.cursor();
		do {			
			if (windowEnd == null || !cursor.lookup(windowEnd.getOpcode())) {
				// Lookup failed --> no superinstruction possible anymore
				// Restart the search for the next instruction
				cursor = this.superinstructionTree.cursor();
				windowStart = getNextNonLabelNode(windowStart);
				windowEnd = windowStart;
				
				if (windowStart != null) {
					// New window -> make sure the backedge following normal non-superinstruction flow is included
					backedges.computeIfAbsent(getNextNonLabelNode(windowStart), n -> new ArrayList<>())
						.add(new SuperinstructionData(null, windowStart));
				}
			} else {
				if (cursor.hasDefinition()) {
					backedges.computeIfAbsent(getNextNonLabelNode(windowEnd), n -> new ArrayList<>())
						.add(new SuperinstructionData(cursor.getDefinition(), windowStart));
				}
				windowEnd = getNextNonLabelNode(windowEnd);
			}
		} while (windowStart != null);
		return backedges;
	}
}
