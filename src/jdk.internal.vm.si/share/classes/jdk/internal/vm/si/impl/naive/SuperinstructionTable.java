package jdk.internal.vm.si.impl.naive;

import java.lang.System.Logger.Level;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.stream.Collectors;

import jdk.internal.vm.si.impl.bytecode.InstructionDefinition;

public class SuperinstructionTable {
	
	private final Map<OpcodeTuple, InstructionDefinition> table;
	private static final System.Logger LOG = System.getLogger(SuperinstructionTable.class.getName()); 

	private SuperinstructionTable(Map<OpcodeTuple, InstructionDefinition> table) {
		this.table = table;
	}
	
	public static SuperinstructionTable buildTable(List<InstructionDefinition> instructions) {
		// Group all instructions by length, shortest first
		Map<Integer, List<InstructionDefinition>> byLength = instructions.stream()
			.collect(Collectors.groupingBy(i -> i.getPrimitives().size()));
		
		if (byLength.isEmpty()) {
			LOG.log(Level.WARNING, "Loaded empty superinstruction tables: no substitution can be performed");
			return new SuperinstructionTable(Collections.emptyMap());
		}
		
		int maxLength = instructions.stream()
				.mapToInt(i -> i.getPrimitives().size())
				.max().getAsInt();
		
		// Table holds all gathered instruction definitions
		Map<OpcodeTuple, InstructionDefinition> table = new HashMap<OpcodeTuple, InstructionDefinition>();
		
		for (int length = 2; length <= maxLength; length++) {
			// First superinstruction has a length of two, so start there to construct the table
			var lastGeneration = byLength.getOrDefault(length - 1, Collections.emptyList());
			var currentGeneration = byLength.getOrDefault(length, Collections.emptyList());
			
			// For all current generation instructions, find the last generation instruction
			// which is a prefix of the current instruction
			// For example, if the current generation instruction is (iload_1, iload_2, iload_3) -> x
			// find the (iload_1, iload_2) -> y instruction from the previous generation
			// Then construct a new OpcodeTuple for (y, iload_3) -> x
			
			for (var currentInstr : currentGeneration) {
				var primitives = currentInstr.getPrimitives();
				var prefix = primitives.subList(0, length - 1);
				var lastOpcode = primitives.get(primitives.size() - 1).getInstr();
				
				var match = lastGeneration.stream()
					.filter(instr -> instr.getPrimitives().equals(prefix))
					.findAny();
				
				if (match.isPresent()) {
					var opcodeTuple = new OpcodeTuple(match.get().getOpcode(), lastOpcode);
					table.put(opcodeTuple, currentInstr);
				} else {
					LOG.log(Level.ERROR, "No prefix superinstruction for instruction " + currentInstr.getOpcode() + ": instruction cannot be placed");
				}
			}
			
		}
		
		return new SuperinstructionTable(table);
	}
	

	public InstructionDefinition lookup(int opcode1, int opcode2) {
		return this.table.get(new OpcodeTuple(opcode1, opcode2));
	}
	
	static final class OpcodeTuple {
		private final int opcode1;
		private final int opcode2;
		
		public OpcodeTuple(int opcode1, int opcode2) {
			this.opcode1 = opcode1;
			this.opcode2 = opcode2;
		}
		
		public int getOpcode1() {
			return opcode1;
		}
		public int getOpcode2() {
			return opcode2;
		}

		@Override
		public int hashCode() {
			final int prime = 31;
			int result = 1;
			result = prime * result + opcode1;
			result = prime * result + opcode2;
			return result;
		}

		@Override
		public boolean equals(Object obj) {
			if (this == obj)
				return true;
			if (obj == null)
				return false;
			if (getClass() != obj.getClass())
				return false;
			OpcodeTuple other = (OpcodeTuple) obj;
			if (opcode1 != other.opcode1)
				return false;
			if (opcode2 != other.opcode2)
				return false;
			return true;
		}
	}

	public String dump() {
		var out = new StringBuilder("--- Table dump ---\n");
		Map<Integer, List<Map.Entry<OpcodeTuple, InstructionDefinition>>> byLength = table.entrySet().stream()
				.collect(Collectors.groupingBy(i -> i.getValue().getPrimitives().size()));
		int maxLength = table.values().stream()
				.mapToInt(i -> i.getPrimitives().size())
				.max().orElse(1);
		
		out.append("--- Size: " + this.table.size()).append(", max instruction length: ").append(maxLength).append('\n');
		
		for (int i = 2; i <= maxLength; i++) {
			out.append("--- Length: ").append(i).append('\n');
			List<Entry<OpcodeTuple, InstructionDefinition>> instructionPairs = byLength.getOrDefault(i, Collections.emptyList());
			var sortedEntries = instructionPairs.stream()
				.sorted(Comparator.<Entry<OpcodeTuple, InstructionDefinition>>comparingInt(e -> e.getKey().opcode1).thenComparingInt(e -> e.getKey().opcode2))
				.collect(Collectors.toList());
			
			for (var entry : sortedEntries) {
				OpcodeTuple tuple = entry.getKey();
				InstructionDefinition definition = entry.getValue();
				
				out.append("    (0x")
					.append(Integer.toString(tuple.opcode1, 16))
					.append(", 0x")
					.append(Integer.toString(tuple.opcode2, 16))
					.append(") -> 0x")
					.append(Integer.toString(definition.getOpcode(), 16))
					.append('\n');
			}
		}
		
		out.append('\n');
		return out.toString();
	}
}
