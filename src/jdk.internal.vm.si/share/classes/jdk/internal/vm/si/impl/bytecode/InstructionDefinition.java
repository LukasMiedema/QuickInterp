package jdk.internal.vm.si.impl.bytecode;

import java.util.List;
import java.util.stream.Collectors;

public class InstructionDefinition {
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

	public String getName() {
		return name;
	}
	
	public int getOpcode() {
		return opcode;
	}
	
	public boolean hasFormatString() {
		return this.primitives.stream().allMatch(i -> i.getFormat() != null);
	}
	
	public boolean isSuperinstruction() {
		return this.primitives.size() > 1;
	}
	
	/**
	 * Returns the format string describing these instructions. If no such string can be defined
	 * (due to far-length instructions), the empty string is returned.
	 * @return the format string.
	 */
	public String getFormatString() {
		if (!hasFormatString())
			return "";

		// Concatenate all the format strings to build one format string encompassing all if possible
		// This string is prefixed with "b" for the instruction itself
		return "b" + this.primitives.stream()
				.map(BytecodePrimitive::getFormat)
				.collect(Collectors.joining());
	}

	public List<BytecodePrimitive> getPrimitives() {
		return primitives;
	}
}
