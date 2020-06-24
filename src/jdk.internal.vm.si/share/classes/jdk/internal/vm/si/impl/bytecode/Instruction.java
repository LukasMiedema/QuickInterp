package jdk.internal.vm.si.impl.bytecode;

import java.util.List;
import java.util.Objects;

/**
 * Represents a full instruction, including the operands for that instruction. 
 * @author lukas
 *
 */
public class Instruction {
	
	private final InstructionDefinition definition;
	private final List<byte[]> operands;
	
	public Instruction(InstructionDefinition definition, List<byte[]> operands) {
		this.definition = Objects.requireNonNull(definition);
		this.operands = Objects.requireNonNull(operands);
		
		if (definition.getPrimitives().size() != operands.size())
			throw new IllegalArgumentException("Arguments not suited for this definition");
	}

	public List<byte[]> getOperands() {
		return operands;
	}

	public InstructionDefinition getDefinition() {
		return definition;
	}

	public int getOpcode() {
		return definition.getOpcode();
	}

	public List<BytecodePrimitive> getPrimitives() {
		return definition.getPrimitives();
	}
}
