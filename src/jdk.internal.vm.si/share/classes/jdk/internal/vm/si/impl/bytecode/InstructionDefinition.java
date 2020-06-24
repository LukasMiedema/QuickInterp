package jdk.internal.vm.si.impl.bytecode;

import java.io.Serializable;
import java.util.List;
import java.util.stream.Collectors;

public class InstructionDefinition implements Serializable {
	private static final long serialVersionUID = 1L;
	
	private final String name;
	private final List<BytecodePrimitive> primitives;
	private final int opcode;
	private final boolean profiling;
	private final OperandCompactionMode compactionMode;
	
	public InstructionDefinition(int opcode, String name, List<BytecodePrimitive> primitives) {
		this(opcode, name, false, primitives);
	}
	
	public InstructionDefinition(int opcode, String name, boolean profiling, List<BytecodePrimitive> primitives) {
		this.opcode = opcode;
		if ((opcode & (-1 << 16)) != 0)
			throw new IllegalArgumentException("Opcode out of range: " + opcode);
		this.name = name;
		this.profiling = profiling;
		this.primitives = primitives;
		this.compactionMode = OperandCompactionMode.COMPATIBLE;
	}

	public String getName() {
		return name;
	}
	
	public int getOpcode() {
		return opcode;
	}
	
	public boolean hasFormatString() {
		return this.primitives.stream().allMatch(i -> i.getFormat(this.profiling) != null);
	}
	
	public boolean isSuperinstruction() {
		return this.primitives.size() > 1 || profiling;
	}
	
	public boolean isProfiling() {
		return profiling;
	}
	
	public OperandCompactionMode getCompactionMode() {
		return compactionMode;
	}
	
	@Override
	public String toString() {
		var out = new StringBuilder();
		out.append("opcode " + opcode + " (0x" + Integer.toString(opcode, 16) + ", size=" + primitives.size() + ", operand compaction=" + this.compactionMode + ", profiling=" + this.profiling + ")\n");
		out.append(" - name: " + this.name).append('\n');
		for (var primitive : primitives) {
			out.append("   ").append(primitive).append(" (" + primitive.getInstr() + ")").append('\n');
		}
		return out.toString();
	}


	/**
	 * Returns the format string describing these instructions. If no such string can be defined
	 * (due to far-length instructions), the empty string is returned.
	 * @return the format string.
	 */
	public String getFormatString() {
		if (!hasFormatString())
			return "";
		
		String separator;
		switch (this.compactionMode) {
		case COMPACTED:
			separator = "";
			break;
		case COMPATIBLE:
			separator = "bb";
			break;
		default:
			throw new IllegalStateException("Unsupported compaction mode: " + this.compactionMode);
		}
		
		// Concatenate all the format strings to build one format string encompassing all if possible
		// This string is prefixed with "b" for the instruction itself
		return "b" + this.primitives.stream()
				.map(bcp -> bcp.getFormat(this.profiling))
				.collect(Collectors.joining(separator));
	}

	public List<BytecodePrimitive> getPrimitives() {
		return primitives;
	}

	public boolean withProfiling() {
		return profiling;
	}
	
	public static enum OperandCompactionMode {
		/**
		 * Operands are precided with their opcode, so if the superinstruction is (e.g.)
		 * - iload_0, iload 10
		 * The bytes will look like this
		 * superinstr
		 * superinstr
		 * iload_10
		 * iload_10
		 * 10
		 */
		COMPATIBLE,
		
		/**
		 * Operands for superinstructions follow directly, without their opcode, so if the superinstruction is (e.g.)
		 * iload_0, iload 10, the bytes will look like this
		 * superinstr
		 * superinstr
		 * 10
		 */
		COMPACTED;
	}
}
