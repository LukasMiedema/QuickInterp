package jdk.internal.vm.si.impl.bytecode;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.function.Function;
import java.util.stream.Collectors;

import jdk.internal.vm.si.impl.bytecode.BytecodePrimitive.BytecodeFlag;

public class InstructionSetDefinition {
	
	public static final boolean DEBUG = false;
	private int nextOpcode = 0;
	
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

		this.nextOpcode = instructions.stream()
				.max(Comparator.comparingInt(InstructionDefinition::getOpcode))
				.get()
				.getOpcode() + 1;
	}
	
	public Map<BytecodePrimitive, InstructionDefinition> addProfiling() {
		return Arrays.stream(BytecodePrimitive.values())
				.filter(p -> p.hasFlag(BytecodeFlag.JUMP_PROFILE))
				.collect(Collectors.toMap(Function.identity(), p -> this.addProfilingInstruction("profile_" + p.name(), p)));
	}

	/**
	 * Create a profiling variant for the given instruction. The primitive must have the JUMP flag set.
	 * @param name the name of the instruction.
	 * @param primitive the primitive.
	 * @return the instruction definition.
	 */
	public InstructionDefinition addProfilingInstruction(String name, BytecodePrimitive primitive) {
		if (!primitive.hasFlag(BytecodeFlag.JUMP))
			throw new IllegalArgumentException("Cannot make profiling variant for primitive " + primitive);
		int opcode = this.nextOpcode++;
		InstructionDefinition def = new InstructionDefinition(opcode, name, true, List.of(primitive));
		this.instructions.add(def);
		return def;
	}

	/**
	 * Fluent adder for a new instruction.
	 * @param opcode
	 * @param name
	 * @param primitives
	 * @return
	 */
	public InstructionSetDefinition instr(int opcode, String name, BytecodePrimitive... primitives) {
		this.nextOpcode = opcode < nextOpcode ? nextOpcode : opcode+1;
		InstructionDefinition def = new InstructionDefinition(opcode, name, List.of(primitives));
		this.instructions.add(def);
		return this;
	}
	
	/**
	 * Fluent adder for a new instruction.
	 * @param name
	 * @param primitives
	 * @return
	 */
	public InstructionSetDefinition instr(String name, BytecodePrimitive... primitives) {
		InstructionDefinition def = new InstructionDefinition(this.nextOpcode++, name, List.of(primitives));
		this.instructions.add(def);
		return this;
	}

	/**
	 * Fluent adder for a new instruction.
	 * @param name
	 * @param primitives
	 * @return 
	 * @return
	 */
	public InstructionSetDefinition instr(String name, List<BytecodePrimitive> primitives) {
		InstructionDefinition def = new InstructionDefinition(this.nextOpcode++, name, primitives);
		this.instructions.add(def);
		return this;
	}
	
	public List<InstructionDefinition> getInstructions() {
		return instructions;
	}

	public int getNextOpcode() {
		return nextOpcode;
	}

	public void setNextOpcode(int nextOpcode) {
		this.nextOpcode = nextOpcode;
	}
}
