package jdk.internal.vm.si.impl.bytecode;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

public class InterpreterGenerator {
	
	public static void main(String[] args) throws IOException {
		var definition = InstructionSetDefinition.defaultDefinition();
		File directory = new File("/home/lukas/build/jdk11/src/hotspot/share/interpreter");
		var generator = new InterpreterGenerator(directory, definition);
		generator.generate();
	}
	
	private final File directory;
	private final InstructionSetDefinition definition;
	
	public InterpreterGenerator(File directory, InstructionSetDefinition definition) {
		this.directory = directory;
		this.definition = definition;
	}

	public void generate() throws IOException {
		File bytecodes = new File(this.directory, "bytecodes.generated.hpp");
		File handlers = new File(this.directory, "bytecodeInterpreter.handlers.hpp");
		File table = new File(this.directory, "bytecodeInterpreter.jumptable.hpp");
		
		writeString(bytecodes, this.definition.generateBytecodes());
		writeString(handlers, this.definition.generateHandlers());
		writeString(table, this.definition.generateJumpTable());
	}
	
	private static void writeString(File file, String str) throws IOException {
		file.createNewFile();
		try (BufferedWriter writer = new BufferedWriter(new FileWriter(file))) {
			writer.write(str);
		}
	}
}
