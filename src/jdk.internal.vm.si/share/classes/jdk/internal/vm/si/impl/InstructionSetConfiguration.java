package jdk.internal.vm.si.impl;

import java.io.Serializable;
import java.util.List;

import jdk.internal.vm.si.impl.bytecode.InstructionDefinition;

public final class InstructionSetConfiguration implements Serializable {
	private static final long serialVersionUID = 1L;
	
	private final Class<? extends AsmSiConverter> converterClass;
	private final List<InstructionDefinition> instructions;
	
	public InstructionSetConfiguration(Class<? extends AsmSiConverter> converterClass,
			List<InstructionDefinition> instructions) {
		this.converterClass = converterClass;
		this.instructions = instructions;
	}

	public Class<? extends AsmSiConverter> getConverterClass() {
		return converterClass;
	}

	public List<InstructionDefinition> getInstructions() {
		return instructions;
	}
}
