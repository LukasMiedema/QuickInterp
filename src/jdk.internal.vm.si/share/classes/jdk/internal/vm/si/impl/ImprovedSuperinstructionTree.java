package jdk.internal.vm.si.impl;

import java.lang.System.Logger.Level;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import jdk.internal.vm.si.impl.bytecode.InstructionDefinition;

/**
 * Superinstruction table (tree) with arbitrary-length matching.
 * It's an improvement of {@link ImprovedSuperinstructionTree} and works like a tree.
 * @author Lukas Miedema
 *
 */
public class ImprovedSuperinstructionTree {
	
	private static final System.Logger LOG = System.getLogger(ImprovedSuperinstructionTree.class.getName()); 

	private final Map<Integer, Node> root;

	private ImprovedSuperinstructionTree(Map<Integer, Node> root) {
		this.root = root;
	}

	public static ImprovedSuperinstructionTree build(List<InstructionDefinition> instructions) {
//		System.out.println("=== Loaded superinstructions (" + instructions.size() + ") ===");
//		instructions.stream().filter(InstructionDefinition::isSuperinstruction).forEach(System.out::println);
//		System.out.println();
		
		// Add all instructions into the tree, mutating / populating where needed
		Map<Integer, Node> root = new HashMap<>();
		for (var instr : instructions) {
			if (!instr.isSuperinstruction())
				continue;
			var current = root;
			for (int i = 0; i < instr.getPrimitives().size(); i++) {
				var primitive = instr.getPrimitives().get(i);
				var node = current.computeIfAbsent(primitive.getInstr(), Node::new);
				if (i == instr.getPrimitives().size() - 1) {
					// Last element -> store definition
					if (node.hasDefinition())
						LOG.log(Level.ERROR, "Duplicate instruction definition: " + instr);
					node.setDefinition(instr);
				} else {
					current = node.getChildren();
				}
			}
		}
		
		return new ImprovedSuperinstructionTree(root);
	}
	
	public Cursor cursor() {
		return new Cursor();
	}
	
	static final class Node {
		private final int opcode;
		
		private final Map<Integer, Node> children = new HashMap<>();
		private InstructionDefinition definition; // or null
		
		public Node(int opcode) {
			this.opcode = opcode;
		}
		
		public boolean hasDefinition() {
			return this.definition != null;
		}

		public InstructionDefinition getDefinition() {
			return definition;
		}

		public void setDefinition(InstructionDefinition definition) {
			this.definition = definition;
		}

		public Map<Integer, Node> getChildren() {
			return children;
		}

		public int getOpcode() {
			return opcode;
		}

		public void dump(int depth, StringBuilder out) {
			String prefix = Stream.generate(() -> "  ").limit(depth + 1).collect(Collectors.joining());
			for (var entry : this.children.entrySet()) {
				out.append(prefix).append(entry.getKey());
				if (entry.getValue().hasDefinition()) {
					out.append(" --> " + entry.getValue().getDefinition().getOpcode());
				}
				out.append('\n');
				entry.getValue().dump(depth + 1, out);
			}
			
		}
	}
	
	public class Cursor {
		private Node currentValue;
		private boolean dead = false;
		
		/**
		 * Go deeper into the tree. This call will return false when the end of the tree is reached.
		 * Note that even when the end hasn't been reached, a definition may be available {@see #hasDefinition()}.
		 * Furthermore, when a definition is available but the tree isn't ended yet there is no guarantee there will be a new definition.
		 * Callers should always retrieve the definition as there is no way of getting "back up" the tree.
		 * @param opcode
		 * @return
		 */
		public boolean lookup(int opcode) {
			if (dead)
				throw new IllegalStateException("Cursor already dead");
			
			Node nextValue;
			if (currentValue == null)
				nextValue = root.get(opcode);
			else
				nextValue = currentValue.children.get(opcode);
			
			if (nextValue == null) {
				this.dead = true;
				return false;
			}
			currentValue = nextValue;
			return true;
		}
		
		public boolean hasDefinition() {
			return currentValue != null && currentValue.hasDefinition();
		}
		
		public InstructionDefinition getDefinition() {
			return currentValue == null ? null : currentValue.definition;
		}
	}

	public String dump() {
		var out = new StringBuilder("=== Tree dump ===\n");
		for (var entry : this.root.entrySet()) {
			out.append("  " + entry.getKey());
			if (entry.getValue().hasDefinition()) {
				out.append(" --> " + entry.getValue().getDefinition().getOpcode());
			}
			out.append('\n');
			entry.getValue().dump(1, out);
		}
		out.append('\n');
		return out.toString();
	}
}
