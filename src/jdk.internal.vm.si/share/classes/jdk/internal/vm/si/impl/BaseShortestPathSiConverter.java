package jdk.internal.vm.si.impl;

import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.PriorityQueue;

import jdk.internal.vm.si.impl.asm.tree.AbstractInsnNode;
import jdk.internal.vm.si.impl.asm.tree.InsnList;
import jdk.internal.vm.si.impl.asm.tree.SuperInstrInsnNode;
import jdk.internal.vm.si.impl.bytecode.InstructionDefinition;

/**
 * Shortest path SI converter determines all the possible edges that can be added by superinstructions
 * Then, it runs Dijkstras on this graph, starting at the beginning of the program. The shortest path
 * from the beginning to the end takes the optimal superinstructions.
 * This is repeated for all jump targets.
 * 
 * @author Lukas Miedema
 *
 */
public abstract class BaseShortestPathSiConverter implements AsmSiConverter {

	/**
	 * Create a graph as map, including the superinstructions
	 * Maps [target node] to superinstr data
	 * Target node is the node _after_ the superinstruction, so where it would end up if the superinstruction was taken
	 * null is used for the node after the last node
	 */
	protected abstract Map<AbstractInsnNode, List<SuperinstructionData>> makeBackedges(InsnList instructions);

	@Override
	public void convert(InsnList instructions) {		
		var backedges = makeBackedges(instructions);
		
		// Do Dijkstras "in reverse", starting at the end of the program trying to reach all instructions from there
		var queue = new PriorityQueue<NodeDistance>();
		var visited = new HashSet<AbstractInsnNode>(); // node distance is the optimal result for that node
		var target = instructions.getFirst(); // first instruction, cache it because it might get replaced

		// A null node is used to indicate the end of the program, and since we begin there start at null
		queue.add(new NodeDistance(null, 0, null, null));
		while (!queue.isEmpty()) {
			var next = queue.remove();
			if (visited.contains(next.getCurrent())) // can be null
				continue;
			visited.add(next.getCurrent());
			
			// Create superinstruction if possible
			// There is no harm in placing a superinstruction if it's never executed (not reachable from the start)
			// And Dijkstra is greedy, so this means that when we get here, we found the optimal superinstruction to place here
			if (next.getDefinition() != null) {
				new SuperInstrInsnNode(next.getDefinition(), next.getCurrent(), instructions);
			}
			
			if (next.getCurrent() == target) // found our target, do not expand neighbors
				continue;
			
			var neighbors = backedges.get(next.getCurrent());
			var nextDistance = next.getDistance() + 1;
			for (var neighbor : neighbors) {
				if (visited.contains(neighbor.getFrom())) // target can be null
					continue;
				var nodeDistance = new NodeDistance(neighbor.getFrom(), nextDistance, next.getCurrent(), neighbor.getDefinition());
				queue.add(nodeDistance);
			}
		}
	}
	
	/**
	 * Simple data class which holds information about possible superinstructions that can be placed at a given instruction.
	 * @author Lukas Miedema
	 */
	public static class SuperinstructionData {
		private final InstructionDefinition definition; // the superinstruction that can be placed here
		private final AbstractInsnNode from; // where definition can be placed
		
		public InstructionDefinition getDefinition() {
			return definition;
		}
		
		public AbstractInsnNode getFrom() {
			return from;
		}
		
		public SuperinstructionData(InstructionDefinition id, AbstractInsnNode from) {
			this.definition = id;
			this.from = from;
		}
	}
	
	/**
	 * Simple data class containing the node and the distance to the node from the current start. Also comparable for sorting in the priority queue.
	 * Dijkstras is done in reverse order, starting at the end of the program.
	 * This traces back the backwards dijkstra, so this ends up pointing into the program control flow direction again, showing the shortest
	 * path.
	 * @author Lukas Miedema
	 */
	static class NodeDistance implements Comparable<NodeDistance> {
		private final AbstractInsnNode current;
		private final AbstractInsnNode next;
		private final InstructionDefinition definition; // the id to get from current to next. Null if a standard instruction (no superinstruction)
		private final int distance;
		
		public NodeDistance(AbstractInsnNode node, int distance, AbstractInsnNode next, InstructionDefinition definition) {
			this.current = node;
			this.next = next;
			this.definition = definition;
			this.distance = distance;
		}

		public AbstractInsnNode getCurrent() {
			return current;
		}

		public AbstractInsnNode getNext() {
			return next;
		}

		public InstructionDefinition getDefinition() {
			return definition;
		}

		public int getDistance() {
			return distance;
		}

		@Override
		public int compareTo(NodeDistance o) {
			int res = Integer.compare(this.distance, o.distance);
			return res == 0 ?
					Integer.compare(definition == null ? -1 : definition.getOpcode(), o.definition == null ? -1 : o.definition.getOpcode()) : res;
		}

		@Override
		public String toString() {
			return "NodeDistance [next=" + next + ", via=" + (definition == null ? "none" : definition.getName()) + ", distance=" + distance + "]";
		}
	}
}
