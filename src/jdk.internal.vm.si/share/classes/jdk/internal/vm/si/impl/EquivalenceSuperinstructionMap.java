package jdk.internal.vm.si.impl;

import static jdk.internal.vm.si.impl.bytecode.BytecodePrimitive.*;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.lang.System.Logger;
import java.lang.System.Logger.Level;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.EnumSet;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

import jdk.internal.vm.si.impl.bytecode.BytecodePrimitive;
import jdk.internal.vm.si.impl.bytecode.BytecodePrimitive.BytecodeFlag;
import jdk.internal.vm.si.impl.bytecode.InstructionDefinition;

/**
 * Can find superinstructions even when equivalence is involved.
 * This implementation is not optimized for speed, and does not make use of arithmetic properties for equivalence (e.g. it will not detect a+b = b+a)
 * @author Lukas Miedema
 *
 */
public class EquivalenceSuperinstructionMap {
	
	private static final Logger log = System.getLogger(EquivalenceSuperinstructionMap.class.getName());
	private static final LookupResult EMPTY_RESULT = new LookupResult(null, null);
	
	public static EquivalenceSuperinstructionMap buildMap(List<InstructionDefinition> instructions) {
		var map = new HashMap<List<List<BytecodePrimitive>>, List<SuperinstructionGraph>>();
		
		// Convert each instruction to a SuperinstructionGraph
		var graphs = instructions.stream()
				.filter(InstructionDefinition::isSuperinstruction)
				.map(id -> SuperinstructionGraph.build(id.getPrimitives(), id)).collect(Collectors.toList());
		
		Set<List<BytecodePrimitive>> preselectionSet = instructions.stream()
				.filter(InstructionDefinition::isSuperinstruction)
				.map(InstructionDefinition::getPrimitives)
				.map(ArrayList::new)
				.peek(Collections::sort)
				.collect(Collectors.toSet());
		
		for (var graph : graphs) {
			// Create a key that helps narrow down matches
			// The key is the set of the instructions in each block
			var key = graph.makeKey();
			var graphList = map.computeIfAbsent(key, k -> new ArrayList<>());
			
			// Deduplicate
			boolean isDuplicate = false;
			for (var otherGraph : graphList) {
				if (testEquality(color(graph, otherGraph), graph, otherGraph)) {
					log.log(Level.INFO, "Duplicate instruction " + graph.getInstructionDefinition() + ", which is equivalent with " + otherGraph.getInstructionDefinition());
					isDuplicate = true;
				}
			}
			
			if (!isDuplicate) {
				graphList.add(graph);
			}
		}
		return new EquivalenceSuperinstructionMap(preselectionSet, map);
	}
	
	// key is to help lookups: sorted list of block primitives
	private final Map<List<List<BytecodePrimitive>>, List<SuperinstructionGraph>> lookupMap;
	
	/**
	 * Hashset of SORTED bytecode sequences. Equivalence will never change instructions -- all the same instructions must be present
	 * If a sorted lookup is not in this preselection set, it will not be equivalent with any superinstruction graph.
	 */
	private final Set<List<BytecodePrimitive>> preselectionSet;
	
	private EquivalenceSuperinstructionMap(
			Set<List<BytecodePrimitive>> preselectionSet,
			Map<List<List<BytecodePrimitive>>, List<SuperinstructionGraph>> lookupMap) {
		this.preselectionSet = preselectionSet;
		this.lookupMap = lookupMap;
	}
	
	/**
	 * Conduct a lookup
	 * @param primitives the list of primitives to find a superinstruction for.
	 * @return
	 */
	public LookupResult lookup(List<BytecodePrimitive> primitives) {
		
		// Early lookup to help filter out a lot of expensive searches
		var sortedPrimitives = new ArrayList<>(primitives);
		Collections.sort(sortedPrimitives);
		if (!preselectionSet.contains(sortedPrimitives))
			return EMPTY_RESULT;
		
		var graph = SuperinstructionGraph.build(primitives, null);
		var key = graph.makeKey();
		List<SuperinstructionGraph> results = lookupMap.get(key);
		if (results == null)
			return EMPTY_RESULT;
		
		// Go through the list
		for (var candidate : results) {
			var colorResult = color(graph, candidate); // graph -> candidate mapping
			if (testEquality(colorResult, graph, candidate)) {
				// Found a match
				int[] bijection = computeBijection(colorResult, graph, candidate);
				return new LookupResult(candidate.getInstructionDefinition(), bijection);
			}
		}
		
		return EMPTY_RESULT;
	}
	
	private static int[] computeBijection(HashMap<InstrBlock, Integer> colorData, SuperinstructionGraph from, SuperinstructionGraph to) {
		var fromBlocks = from.blocks;
		var toBlocks = to.blocks;
		
		if (fromBlocks.size() != toBlocks.size())
			throw new AssertionError("Block sizes do not match: these graphs cannot be isomorphic");
		
		// There's >= 1 bijection between the blocks, find one first
		var blockBijection = new int[fromBlocks.size()]; // mapping from fromblocks -> toblocks
		var visitedToBlocks = new boolean[toBlocks.size()];
		
		for (int i = 0; i < fromBlocks.size(); i++) {
			var fromBlock = fromBlocks.get(i);
			int fromColor = colorData.get(fromBlock);
			
			// Find a block that 'fromBlock' maps to in 'toBlocks'
			for (int j = 0; j < toBlocks.size(); j++) {
				if (visitedToBlocks[j])
					continue;
				var toBlock = toBlocks.get(j);
				int toColor = colorData.get(toBlock);
				
				if (fromColor == toColor) {
					blockBijection[i] = j;
					visitedToBlocks[j] = true;
				}
			}
		}
		
		// compute for each 'to' block at what offset it sits within the program
		int instrListSize = 0;
		int[] toBlockOffset = new int[toBlocks.size()];
		for (int i = 0; i < toBlockOffset.length; i++) {
			toBlockOffset[i] = instrListSize;
			instrListSize += toBlocks.get(i).getInstructions().size();
		}
		
		// compute instruction bijection from the block bijection
		int[] instrBijection = new int[instrListSize];
		int instrIndex = 0;
		for (int i = 0; i < blockBijection.length; i++) {
			var fromBlock = fromBlocks.get(i).getInstructions();
			var toBlock = toBlocks.get(blockBijection[i]).getInstructions();
			
			if (fromBlock.size() != toBlock.size())
				throw new AssertionError("Block size do not match, this is not a bijection");
			
			// write for each instruction their new index at the original location of that instruction
			for (int j = 0; j < fromBlock.size(); j++) {
				instrBijection[instrIndex++] = toBlockOffset[blockBijection[i]] + j;
			}
		}
		
		return instrBijection;
	}

	public static boolean testEquality(HashMap<InstrBlock, Integer> colorData, SuperinstructionGraph g1, SuperinstructionGraph g2) {
		int g1Color = colorData.get(g1.getLastBlock());
		int g2Color = colorData.get(g2.getLastBlock());
		return g1Color == g2Color;
	}
	
	public static HashMap<InstrBlock, Integer> color(SuperinstructionGraph g1, SuperinstructionGraph g2) {
		if (g1.blocks.size() != g2.blocks.size())
			throw new IllegalArgumentException("Can only color graphs of identical size");
		
		var colorAssignment = new HashMap<List<BytecodePrimitive>, Integer>();
		var allBlocks = new InstrBlock[g1.blocks.size() * 2];
		var currentColors = new HashMap<InstrBlock, Integer>(allBlocks.length);
		var nextColors = new HashMap<InstrBlock, Integer>(allBlocks.length);
		int nextColor = 0;
		
		for (int i = 0; i < g1.blocks.size(); i++) {
			allBlocks[i] = g1.blocks.get(i);
			allBlocks[i + g1.blocks.size()] = g2.blocks.get(i);
		}
		
		// first do initial colors based on instructions
		for (int i = 0; i < allBlocks.length; i++) {
			var color = colorAssignment.get(allBlocks[i].instructions);
			if (color == null) {
				color = nextColor++;
				colorAssignment.put(allBlocks[i].instructions, color);
			}
			currentColors.put(allBlocks[i], color);
		}
		
		// key = sort([own color, neighbor1.color, neighbor2.color, ...])
		var neighborColors = new HashMap<List<Integer>, Integer>();
		var previousPartitions = new HashSet<>(currentColors.entrySet().stream()
				.collect(Collectors.groupingBy(kv -> kv.getValue(),
						Collectors.mapping(kv -> kv.getKey(), Collectors.toSet()))).values());
		
		// iterate coloring
		// coloring will read from 'current colors' and write to 'next colors', swapping them at the end
		while (true) {
			for (int i = 0; i < allBlocks.length; i++) {
				// key for the neighborColors map
				var colorData = new ArrayList<Integer>(allBlocks[i].mustBeAfter.size() + 1);
				colorData.add(currentColors.get(allBlocks[i]));
				for (var neighbor : allBlocks[i].mustBeAfter) {
					colorData.add(currentColors.get(neighbor));
				}
				Collections.sort(colorData);
				
				
				// key is constructed, get/create the next color
				var color = neighborColors.get(colorData);
				if (color == null) {
					color = nextColor++; // TODO: deal with overflow for yuge graphs
					neighborColors.put(colorData, color);
				}
				// System.out.println("block " + i + " w/ colorData" + colorData + " -> " + color + (isNew ? "(new)" : ""));
				nextColors.put(allBlocks[i], color);
			}
			
			// test if the partitions have changed
			var newPartitions = new HashSet<>(nextColors.entrySet().stream()
					.collect(Collectors.groupingBy(kv -> kv.getValue(),
							Collectors.mapping(kv -> kv.getKey(), Collectors.toSet())))
					.values());
			
			if (previousPartitions.equals(newPartitions)) {
				break; // no change performed -> done
			} else {
				previousPartitions = newPartitions;
			}
			
			// writing to next colors is complete -> swap nextcolors and current colors
			var tmp = nextColors;
			nextColors = currentColors;
			currentColors = tmp;
		}
		
		return currentColors;
	}
	
	public static class SuperinstructionGraph {
		
		// A table mapping for each instruction what the lowest relative stack access they did
		// E.g., an "iadd" will read from tos-1 and tos-2, and write to -2. So its lowest access is -2
		// A bipush will write to tos+0, so its lowest access is 0
		private static int[] LOWEST_STACK_ACCESS_TABLE;
		
		static {
			int[] table = new int[0xc9];
			LOWEST_STACK_ACCESS_TABLE = table;
			
			// Copy the defaults from the 'stack' offset column of the bytecode primitive
			// This is not reflective of operations which both pop and push from the op stack
			for (int i = 0; i < table.length; i++) {
				var stackOffset = BytecodePrimitive.byOpcode(i).getStack();
				table[i] = Math.min(0, stackOffset); // positive stack offset never happens
			}
			
			// Overwrite the exceptions
			// These are always instructions that pop and push from the operand stack
			table[iaload.getInstr()] = -2;
			table[laload.getInstr()] = -2;
			table[faload.getInstr()] = -2;
			table[daload.getInstr()] = -2;
			table[aaload.getInstr()] = -2;
			table[baload.getInstr()] = -2;
			table[caload.getInstr()] = -2;
			table[saload.getInstr()] = -2;
			
			table[dup.getInstr()] = -1;
			table[dup_x1.getInstr()] = -2;
			table[dup_x2.getInstr()] = -3;
			table[dup2.getInstr()] = -2;
			table[dup2_x1.getInstr()] = -3;
			table[dup2_x2.getInstr()] = -4;
			table[swap.getInstr()] = -2;
			
			table[iadd.getInstr()] = -2;
			table[ladd.getInstr()] = -4;
			table[fadd.getInstr()] = -2;
			table[dadd.getInstr()] = -4;
			table[isub.getInstr()] = -2;
			table[lsub.getInstr()] = -4;
			table[fsub.getInstr()] = -2;
			table[dsub.getInstr()] = -4;
			table[imul.getInstr()] = -2;
			table[lmul.getInstr()] = -4;
			table[fmul.getInstr()] = -2;
			table[dmul.getInstr()] = -4;
			table[idiv.getInstr()] = -2;
			table[ldiv.getInstr()] = -4;
			table[fdiv.getInstr()] = -2;
			table[ddiv.getInstr()] = -4;
			table[irem.getInstr()] = -2;
			table[lrem.getInstr()] = -4;
			table[frem.getInstr()] = -2;
			table[drem.getInstr()] = -4;
			table[ineg.getInstr()] = -1;
			table[lneg.getInstr()] = -2;
			table[fneg.getInstr()] = -1;
			table[dneg.getInstr()] = -2;
			
			table[ishl.getInstr()] = -2;
			table[lshl.getInstr()] = -3;
			table[ishr.getInstr()] = -2;
			table[lshr.getInstr()] = -3;
			table[iushr.getInstr()] = -2;
			table[lushr.getInstr()] = -3;
			table[iand.getInstr()] = -2;
			table[land.getInstr()] = -4;
			table[ior.getInstr()] = -2;
			table[lor.getInstr()] = -4;
			table[ixor.getInstr()] = -2;
			table[lxor.getInstr()] = -4;
			table[iinc.getInstr()] = 0;
			table[i2l.getInstr()] = -1;
			table[i2f.getInstr()] = -1;
			table[i2d.getInstr()] = -1;
			table[l2i.getInstr()] = -2;
			table[l2f.getInstr()] = -2;
			table[l2d.getInstr()] = -2;
			table[f2i.getInstr()] = -1;
			table[f2l.getInstr()] = -1;
			table[f2d.getInstr()] = -1;
			table[d2i.getInstr()] = -2;
			table[d2l.getInstr()] = -2;
			table[d2f.getInstr()] = -2;
			table[i2b.getInstr()] = -1;
			table[i2c.getInstr()] = -1;
			table[i2s.getInstr()] = -1;
			table[lcmp.getInstr()] = -4;
			table[fcmpl.getInstr()] = -2;
			table[fcmpg.getInstr()] = -2;
			table[dcmpl.getInstr()] = -4;
			table[dcmpg.getInstr()] = -4;
			
			table[newarray.getInstr()] = -1;
			table[anewarray.getInstr()] = -1;
			table[arraylength.getInstr()] = -1;
			table[checkcast.getInstr()] = -1;
			table[_instanceof.getInstr()] = -1;
		}
		
		public static SuperinstructionGraph build(List<BytecodePrimitive> primitives, InstructionDefinition id) {
			// Step 1+2: cut the code into blocks and compute the attributes (done in one step)
			var blocks = makeBlocks(primitives);
			
			// Step 3: add edges based on computed attributes
			makeDataDependencyGraph(blocks);
			
			return new SuperinstructionGraph(blocks, id);
		}

		/**
		 * Merge into one graph for .dot printing. There's nothing else useful you can do with this.
		 * @param g1
		 * @param g2
		 * @return
		 */
		public static SuperinstructionGraph merge(SuperinstructionGraph g1, SuperinstructionGraph g2) {
			List<InstrBlock> allBlocks = new ArrayList<>(g1.blocks.size() + g2.blocks.size());
			allBlocks.addAll(g1.blocks);
			allBlocks.addAll(g2.blocks);
			return new SuperinstructionGraph(allBlocks, null);
		}
		
		private static List<InstrBlock> makeBlocks(List<BytecodePrimitive> primitives) {
			var blocks = new ArrayList<InstrBlock>();
			
			// Step 1: find the maximum stack depth
			int stackOffset = 0;
			int lowestStackOffset = 0;
			for (var primitive : primitives) {
				lowestStackOffset = Math.min(lowestStackOffset, stackOffset + getLowestStackAccess(primitive));
				stackOffset += primitive.getStack();
			}
			
			// Step 2: cut
			stackOffset = 0;
			List<BytecodePrimitive> currentBlock = new ArrayList<>();
			for (var primitive : primitives) {
				
				// Creates an empty block if lowestStackOffset = 0
				if (stackOffset == lowestStackOffset) {
					// Found a cut point: perform cut
					blocks.add(new InstrBlock(currentBlock, blocks.isEmpty()));
					currentBlock = new ArrayList<>();
				}
				
				stackOffset += primitive.getStack();
				currentBlock.add(primitive);
			}
			
			// Create last block(s). This one can also be empty if stackOffset is back at 0
			boolean hasImmovableTail = stackOffset != lowestStackOffset; // true if there's a natural immovable last block
			if (hasImmovableTail) {
				blocks.add(new InstrBlock(currentBlock, true));
			} else {
				blocks.add(new InstrBlock(currentBlock, false));
				blocks.add(new InstrBlock(Collections.emptyList(), true)); // no natural immovable tail, so create a synthetic one
			}			
			return blocks;
		}
		
		private static void makeDataDependencyGraph(List<InstrBlock> blocks) {
			// Each block has outgoing edges going to blocks earlier in the list
			// Do a nested iteration (complexity O(n2)) to hook everything up
			for (int i = 1; i < blocks.size(); i++) {
				var currentBlock = blocks.get(i);
				for (int j = 0; j < i; j++) {
					var precedingBlock = blocks.get(j);
					
					// Current block may require that precedingBlock happens first
					if (!currentBlock.canBeMovedAheadOf(precedingBlock)) {
						currentBlock.addMustBeAfter(precedingBlock);
					}
				}
			}
		}
		
		private static int getLowestStackAccess(BytecodePrimitive primitive) {
			if (primitive.hasFlag(BytecodeFlag.NO_SUPERINSTRUCTION) || primitive.hasFlag(BytecodeFlag.UNKNOWN_STACK_OFFSET))
				throw new IllegalArgumentException("the instruction " + primitive + " does not support static stack depth analysis");
			return LOWEST_STACK_ACCESS_TABLE[primitive.getInstr()];
		}
		
		private final List<InstrBlock> blocks;
		private final InstructionDefinition instructionDefinition;
		
		/**
		 * Create the graph. The blocks must already have their DDG computed.
		 * @param allBlocks
		 */
		private SuperinstructionGraph(List<InstrBlock> allBlocks, InstructionDefinition instructionDefinition) {
			this.blocks = allBlocks;
			this.instructionDefinition = instructionDefinition;
			
			InstrBlock lastBlock = blocks.get(blocks.size() - 1);
			if (!lastBlock.hasDataDependency(DataDependency.FULL_BARRIER))
				throw new IllegalStateException("Last block must always have FULL_BARRIER. Is the DDG built?");
		}
		
		/**
		 * Get the last block. This blcok always has FULL_BARRIER, and the color of this block
		 * can be used for equivalence.
		 * @return
		 */
		public InstrBlock getLastBlock() {
			return this.blocks.get(this.blocks.size() - 1);
		}
		

		public InstructionDefinition getInstructionDefinition() {
			return instructionDefinition;
		}

		public List<List<BytecodePrimitive>> makeKey() {
			var key = blocks.stream()
				.map(InstrBlock::getInstructions)
				.collect(Collectors.toList());
			
			// To canonicalize, the list needs to be sorted
			Collections.sort(key,
					Comparator.<List<BytecodePrimitive>>comparingInt(List::size)
						.thenComparing(Comparator.comparingInt(Object::hashCode)));
			
			return key;
		}
		
		/**
		 * Print a dot, optionally w/ colors.
		 * @param filename
		 * @param colorData
		 */
		public void writeDot(String filename, HashMap<InstrBlock,Integer> colorData) {
			var outFile = new File(filename + ".dot").getAbsoluteFile();
			var blockNames = new HashMap<InstrBlock, String>();
			for (int i = 0; i < this.blocks.size(); i++) {
				blockNames.put(this.blocks.get(i), "b" + i);
			}
			
			System.out.println("=== Writing " + outFile + " ===");
			
			try (var out = new PrintWriter(new FileWriter(outFile))) {
				out.println("digraph " + filename + " {");
				
				// labels
				for (var block : blocks) {
					var blockName = blockNames.get(block);
					var label = new StringBuilder("Block " + blockName).append("\\n");
					
					if (colorData != null && colorData.containsKey(block)) {
						var color = colorData.get(block);
						label.append("Color: " + color + "\\n");
					}
					
					label.append("\\nAttrs: ");
					
					for (var attr : block.attributes) {
						label.append(attr).append("\\n");
					}
					label.append("\\nInstructions: ");
					for (var instr : block.instructions) {
						label.append(instr).append("\\n");
					}
					
					out.println(blockName + "[shape=box, label=\"" + label.toString() + "\"];");
				}
				
				// edges
				out.println();
				for (var from : blocks) {
					for (var to : from.mustBeAfter) {
						out.println(blockNames.get(from) + " -> " + blockNames.get(to) + ";");
					}
				}
				
				out.println("}");
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	/**
	 * An InstrBlock is a sequence of instructions that leaves the stack balanced (well, one that can be reordered anyways).
	 * Since it does this, InstrBlocks within a superinstruction can be reordered if their other data dependencies allow.
	 * Note that field access / method calls can never be part of a superinstruction, so it is not regarded for equivalence.
	 * @author Lukas Miedema
	 *
	 */
	public static class InstrBlock {
		private final EnumSet<DataDependency> attributes;
		private final List<BytecodePrimitive> instructions;
		private final Set<InstrBlock> mustBeAfter = new HashSet<>(); // these are the 'out' edges from this block
		
		public InstrBlock(List<BytecodePrimitive> instructions, boolean isImmovable) { // first and last block are always immovable
			this.instructions = instructions;
			this.attributes = computeAttributes(instructions, isImmovable);
		}

		public void addMustBeAfter(InstrBlock precedingBlock) {
			mustBeAfter.add(precedingBlock);
		}
		
		public boolean hasDataDependency(DataDependency dd) {
			return attributes.contains(dd);
		}

		private static EnumSet<DataDependency> computeAttributes(List<BytecodePrimitive> primitives, boolean isImmovable) {
			var attributes = EnumSet.noneOf(DataDependency.class);
			if (isImmovable)
				attributes.add(DataDependency.FULL_BARRIER);
			
			for (var p : primitives) {
				if (p.hasFlag(BytecodeFlag.JUMP))
					attributes.add(DataDependency.FULL_BARRIER);
				
				else if (p == istore || p == lstore || p == fstore || p == dstore || p == astore || p == iinc)
					attributes.add(DataDependency.STORE_BARRIER_ANY);
				else if (p == istore_0 || p == lstore_0 || p == fstore_0 || p == dstore_0 || p == astore_0)
					attributes.add(DataDependency.STORE_BARRIER_0);
				else if (p == istore_1 || p == lstore_1 || p == fstore_1 || p == dstore_1 || p == astore_1)
					attributes.add(DataDependency.STORE_BARRIER_1);
				else if (p == istore_2 || p == lstore_2 || p == fstore_2 || p == dstore_2 || p == astore_2)
					attributes.add(DataDependency.STORE_BARRIER_2);
				else if (p == istore_3 || p == lstore_3 || p == fstore_3 || p == dstore_3 || p == astore_3)
					attributes.add(DataDependency.STORE_BARRIER_3);
				
				else if (p == iload || p == lload || p == fload || p == dload || p == aload || p == ret)
					attributes.add(DataDependency.LOAD_BARRIER_ANY);
				else if (p == iload_1 || p == lload_1 || p == fload_1 || p == dload_1 || p == aload_1)
					attributes.add(DataDependency.LOAD_BARRIER_1);
				else if (p == iload_2 || p == lload_2 || p == fload_2 || p == dload_2 || p == aload_2)
					attributes.add(DataDependency.LOAD_BARRIER_2);
				else if (p == iload_3 || p == lload_3 || p == fload_3 || p == dload_3 || p == aload_3)
					attributes.add(DataDependency.LOAD_BARRIER_3);
			}
			
			return attributes;
		}
		
		/**
		 * If true, no edge. Otherwise edge.
		 */
		public boolean canBeMovedAheadOf(InstrBlock other) {
			return this.attributes.stream().allMatch(attr -> attr.canBeMovedAheadOf(other.attributes));
		}

		public EnumSet<DataDependency> getAttributes() {
			return attributes;
		}

		public List<BytecodePrimitive> getInstructions() {
			return instructions;
		}
	}
	
	public static class LookupResult {
		private final InstructionDefinition definition;
		private final int[] instructionBijection;
		
		public LookupResult(InstructionDefinition definition, int[] instructionBijection) {
			this.definition = definition;
			this.instructionBijection = instructionBijection;
		}

		public InstructionDefinition getDefinition() {
			return definition;
		}
		
		public int[] getInstructionBijection() {
			return instructionBijection;
		}
		
		public boolean hasResult() {
			return definition != null;
		}
		
		/**
		 * True if the bijection is trivial (does not change the order).
		 * @return
		 */
		public boolean hasTrivialBijection() {
			return IntStream.range(0, instructionBijection.length)
					.allMatch(i -> i == instructionBijection[i]);
		}
		
		/**
		 * Create a dot graph file of the instruction bijection
		 */
		public void exportAsDot(String filename, List<BytecodePrimitive> fromInstrs) {
			var outFile = new File(filename + ".dot").getAbsoluteFile();
			var toInstrs = definition.getPrimitives();
			
			System.out.println("=== Writing " + outFile + " ===");
			
			try (var out = new PrintWriter(new FileWriter(outFile))) {
				out.println("digraph " + filename + " {");
				
				// labels
				for (int i = 0; i < fromInstrs.size(); i++)
					out.println("f" + i + "[shape=box, label=\"" + fromInstrs.get(i) + " f" + i + "\"];");
				for (int i = 0; i < fromInstrs.size(); i++)
					out.println("t" + i + "[shape=box, label=\"" + toInstrs.get(i) + " t" + i + "\"];");
				
				// edges
				out.println();
				for (int i = 0; i < instructionBijection.length; i++) {
					out.println("f" + i + " -> t" + instructionBijection[i] + " [color=blue];");
					if (i > 0) {
						out.println("f" + (i-1) + " -> f" + i + ";");
						out.println("t" + (i-1) + " -> t" + i + ";");
					}
				}
				out.println("}");
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	public static enum DataDependency {
		FULL_BARRIER,
		
		STORE_BARRIER_ANY,
		STORE_BARRIER_0,
		STORE_BARRIER_1,
		STORE_BARRIER_2,
		STORE_BARRIER_3,
		
		LOAD_BARRIER_ANY,
		LOAD_BARRIER_0,
		LOAD_BARRIER_1,
		LOAD_BARRIER_2,
		LOAD_BARRIER_3;

		/**
		 * Returns true if the other data dependencies can be moved from before to after this
		 * one. In other words, true if this one can be moved ahead of the other.
		 * @return
		 */
		public boolean canBeMovedAheadOf(Set<DataDependency> others) {
			return others.stream().allMatch(this::canBeMovedAheadOf);
		}
		
		/**
		 * Returns true if the other data dependency can be moved from before to after this
		 * one. In other words, true if this one can be moved ahead of the other.
		 * @return
		 */
		public boolean canBeMovedAheadOf(DataDependency other) {
			switch (this) {
			case FULL_BARRIER:
				return false;

			case STORE_BARRIER_ANY:
				return 	other != FULL_BARRIER &&
						other != STORE_BARRIER_ANY &&
						other != STORE_BARRIER_0 &&
						other != STORE_BARRIER_1 &&
						other != STORE_BARRIER_2 &&
						other != STORE_BARRIER_3 &&
						other != LOAD_BARRIER_ANY &&
						other != LOAD_BARRIER_0 &&
						other != LOAD_BARRIER_1 &&
						other != LOAD_BARRIER_2 &&
						other != LOAD_BARRIER_3;
			case STORE_BARRIER_0:
				return 	other != FULL_BARRIER &&
						other != STORE_BARRIER_ANY &&
						other != STORE_BARRIER_0 &&
						other != LOAD_BARRIER_ANY &&
						other != LOAD_BARRIER_0;
			case STORE_BARRIER_1:
				return 	other != FULL_BARRIER &&
						other != STORE_BARRIER_ANY &&
						other != STORE_BARRIER_1 &&
						other != LOAD_BARRIER_ANY &&
						other != LOAD_BARRIER_1;
			case STORE_BARRIER_2:
				return 	other != FULL_BARRIER &&
						other != STORE_BARRIER_ANY &&
						other != STORE_BARRIER_2 &&
						other != LOAD_BARRIER_ANY &&
						other != LOAD_BARRIER_2;
			case STORE_BARRIER_3:
				return 	other != FULL_BARRIER &&
						other != STORE_BARRIER_ANY &&
						other != STORE_BARRIER_3 &&
						other != LOAD_BARRIER_ANY &&
						other != LOAD_BARRIER_3;

			case LOAD_BARRIER_ANY:
				return 	other != FULL_BARRIER &&
						other != STORE_BARRIER_ANY &&
						other != STORE_BARRIER_0 &&
						other != STORE_BARRIER_1 &&
						other != STORE_BARRIER_2 &&
						other != STORE_BARRIER_3;
			case LOAD_BARRIER_0:
				return 	other != FULL_BARRIER &&
						other != STORE_BARRIER_ANY &&
						other != STORE_BARRIER_0;
			case LOAD_BARRIER_1:
				return 	other != FULL_BARRIER &&
						other != STORE_BARRIER_ANY &&
						other != STORE_BARRIER_1;
			case LOAD_BARRIER_2:
				return 	other != FULL_BARRIER &&
						other != STORE_BARRIER_ANY &&
						other != STORE_BARRIER_2;
			case LOAD_BARRIER_3:
				return 	other != FULL_BARRIER &&
						other != STORE_BARRIER_ANY &&
						other != STORE_BARRIER_3;
			}
			
			throw new IllegalArgumentException("Unknown data dependency this==" + this);
		}
	}

}
