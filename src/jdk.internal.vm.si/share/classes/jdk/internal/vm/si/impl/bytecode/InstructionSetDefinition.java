package jdk.internal.vm.si.impl.bytecode;

import java.time.Instant;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.stream.Collectors;

import jdk.internal.vm.si.impl.bytecode.BytecodePrimitive.BytecodeFlag;

public class InstructionSetDefinition {
	
	private final boolean debug = false;
	
	public static InstructionSetDefinition defaultDefinition() {
		InstructionSetDefinition definition = new InstructionSetDefinition();
		definition.addDefault();
		return definition;
	}
	
	private final List<InstructionDefinition> instructions = new ArrayList<>();
	
	/**
	 * Add all default instructions.
	 */
	public void addDefault() {
		Arrays.stream(BytecodePrimitive.values())
				.filter(p -> p.getInstr() != -1)
				.map(p -> new InstructionDefinition(p.getInstr(), p.getHandlerName(), Collections.singletonList(p)))
				.forEach(instructions::add);
	}
	
	public String generateHandlers() {
		StringBuilder builder = new StringBuilder();
		builder.append("/* GENERATED HANDLER FILE -- DO NOT EDIT (" + Instant.now() + ") */\n");
		builder.append("#include \"bytecodeInterpreter.base.hpp\"\n");
		builder.append('\n');
		
		for (var instruction : instructions) {
			instruction.generateHandler(builder);
		}
		return builder.toString();
	}
	
	public String generateJumpTable() {
		StringBuilder builder = new StringBuilder();
		InstructionDefinition[] table = new InstructionDefinition[1 << 16];
		for (InstructionDefinition definition : this.instructions) {
			table[definition.getOpcode()] = definition;
		}
		
		builder.append("/* GENERATED JUMP TABLE FILE -- DO NOT EDIT (" + Instant.now() + ") */\n");
		
		for (int i = 0; i < (1 << 16); i += 4) {
			builder.append("/* ").append(Integer.toHexString(i)).append(" */");
			for (int j = i; j < i + 4; j++) {
				String opc = (table[j] == null) ?"&&opc_default" : ("&&opc_" + table[j].name);
				builder.append(opc).append(',');
				for (int k = opc.length(); k < 40; k++)
					builder.append(' ');
			}
			builder.append('\n');
		}
		return builder.toString();
	}
	
	public String generateBytecodes() {
		StringBuilder builder = new StringBuilder();
		builder.append("/* GENERATED BYTECODE FILE -- DO NOT EDIT (" + Instant.now() + ") */\n");

		this.instructions.stream()
			.sorted(Comparator.comparingInt(InstructionDefinition::getOpcode))
			.forEach(instr -> {
				builder.append("     _").append(instr.getName());
				for (int k = instr.getName().length(); k < 40; k++)
					builder.append(' ');
				builder.append(" = ").append(instr.opcode)
					.append(',').append(" // 0x").append(Integer.toString(instr.opcode, 16))
					.append('\n');
			});
		return builder.toString();
	}
	
	class InstructionDefinition {
		private final String name;
		private final List<BytecodePrimitive> primitives;
		private final int opcode;
		
		public InstructionDefinition(int opcode, String name, List<BytecodePrimitive> primitives) {
			this.opcode = opcode;
			if ((opcode & (-1 << 16)) != 0)
				throw new IllegalArgumentException("Opcode out of range: " + opcode);
			this.name = name;
			this.primitives = primitives;
		}

		public void generateHandler(StringBuilder builder) {
			int pcOffset = 0;
			int stackOffset = 0;
			boolean terminated = false;
			
			builder.append("CASE(_" + name + "): { // " + name).append('\n');
			for (int i = 0; i < primitives.size(); i++) {
				
				if (terminated)
					throw new IllegalStateException("Cannot process instruction after terminal instruction");
				
				if (debug)
					builder.append("  puts(\"" + name + "\");\n");
				
				BytecodePrimitive primitive = primitives.get(i);
				writeInstruction(primitive, pcOffset, stackOffset, builder);
					
				// Compute offsets for next instruction
				if (primitive.hasFlag(BytecodeFlag.UPDATES_PC_OFFSET))
					pcOffset = 0;
				else if (primitive.hasFlag(BytecodeFlag.UNKNOWN_PC_OFFSET))
					pcOffset = BytecodeFlag.U;
				else
					pcOffset += primitive.getPc();
				
				if (primitive.hasFlag(BytecodeFlag.UPDATES_STACK_OFFSET))
					stackOffset = 0;
				else if (primitive.hasFlag(BytecodeFlag.UNKNOWN_STACK_OFFSET))
					stackOffset = BytecodeFlag.U;
				else
					stackOffset += primitive.getStack();

				if (primitive.hasFlag(BytecodeFlag.TERMINAL) || primitive.hasFlag(BytecodeFlag.NO_SUPERINSTRUCTION))
					terminated = true;
			}
			if (!terminated) {
				// Terminate by writing the end_sequence instruction
				writeInstruction(BytecodePrimitive.end_sequence, pcOffset, stackOffset, builder);
			}
			
			builder.append("  puts(\"Overrun of " + name + " handler\");\n");
			
			builder.append("}\n");
		}

		private void writeInstruction(BytecodePrimitive primitive, int pcOffset, int stackOffset, StringBuilder builder) {
			if (pcOffset == BytecodeFlag.U || stackOffset == BytecodeFlag.U)
				throw new IllegalStateException("Cannot write instruction with unknown offsets: " + primitive + " in " + this.primitives);
			builder.append("  { INSTR_" + primitive.getHandlerName() + "(")
					.append("pc+").append(pcOffset).append(",").append(stackOffset)
					.append("); }\n");
		}
		
		public String getName() {
			return name;
		}
		
		public int getOpcode() {
			return opcode;
		}
	}
}
