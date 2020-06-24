package jdk.internal.vm.si.impl;

import java.io.File;
import java.io.FileInputStream;
import java.io.ObjectInputStream;
import java.lang.System.Logger.Level;
import java.util.stream.Collectors;

import jdk.internal.vm.si.ClassBytecodeFormatConverter;
import jdk.internal.vm.si.impl.asm.ClassReader;
import jdk.internal.vm.si.impl.asm.ClassWriter;
import jdk.internal.vm.si.impl.asm.tree.ClassNode;
import jdk.internal.vm.si.impl.bytecode.InstructionDefinition;

public class ClassBytecodeFormatConverterImpl implements ClassBytecodeFormatConverter {
	
	static final String SUPERINSTRUCTION_FILE = "/home/lukas/Documents/School/Q8/code/eclipse-cdt-workspace/jdk.internal.vm.si.generator/superinstructions.list";
	static final AsmSiConverter CONVERTER;
	
	static {
		File file = new File(SUPERINSTRUCTION_FILE);
		AsmSiConverter converter = null;
		try (var ois = new ObjectInputStream(new FileInputStream(file))) {
			var configuration = (InstructionSetConfiguration) ois.readObject();
			converter = configuration.getConverterClass().getConstructor().newInstance();
			
			var instrList = configuration.getInstructions();
//			// Temp debugging
			var superinstructions = instrList.stream().filter(InstructionDefinition::isSuperinstruction).collect(Collectors.toList());
//			var regularInstructions = instrList.stream().filter(id -> !id.isSuperinstruction()).collect(Collectors.toList());
//			
//			superinstructions = superinstructions.subList(58, 59);
			
			if (System.getProperty("printSuperinstructions") != null) {
				System.out.println("=== Using SI Converter: " + converter.getName() + " ===");
				System.out.println("=== Loaded superinstructions (" + superinstructions.size() + ") ===");
				superinstructions.forEach(System.out::println);
				System.out.println();
			}
//			
//			var out = new ArrayList<InstructionDefinition>();
//			out.addAll(regularInstructions);
//			out.addAll(superinstructions);
//			
			converter.setInstructions(instrList);
		} catch (Exception e) {
			System.Logger log = System.getLogger(ClassBytecodeFormatConverterImpl.class.getName()); 
			log.log(Level.ERROR, "Could not initialize superinstruction table. SI have been disabled.", e);
		}
		CONVERTER = converter;
	}
	
	public byte[] convertClass(byte[] input) {
		if (CONVERTER == null)
			return input;
		return convertClassInternal(new ClassReader(input));
	}
	
	private static byte[] convertClassInternal(ClassReader reader) {
		var writer = new ClassWriter(0);
		
		ClassNode node = new ClassNode();
		reader.accept(node, ClassReader.SKIP_FRAMES | ClassReader.SKIP_DEBUG);
		
		// Now we have a fully populated node --> modify it and write it again
		for (var method : node.methods) {
			if (method.instructions.size() > 0) {
				CONVERTER.convert(method.instructions);
			}
		}
		
		// And write the changes back
		node.accept(writer);
		return writer.toByteArray();
	}
}
