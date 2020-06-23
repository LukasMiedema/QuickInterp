package jdk.internal.vm.si.impl.bytecode;

import static jdk.internal.vm.si.impl.bytecode.BytecodePrimitive.BytecodeFlag.*;

import java.util.Arrays;
import java.util.EnumSet;
import java.util.stream.Stream;

public enum BytecodePrimitive {
	nop				(0x00,	 2,	 0),
	aconst_null		(0x01,	 2,	 1),
	iconst_m1		(0x02,	 2,	 1),
	iconst_0		(0x03,	 2,	 1),
	iconst_1		(0x04,	 2,	 1),
	iconst_2		(0x05,	 2,	 1),
	iconst_3		(0x06,	 2,	 1),
	iconst_4		(0x07,	 2,	 1),
	iconst_5		(0x08,	 2,	 1),
	lconst_0		(0x09,	 2,	 2),
	lconst_1		(0x0a,	 2,	 2),
	fconst_0		(0x0b,	 2,	 1),
	fconst_1		(0x0c,	 2,	 1),
	fconst_2		(0x0d,	 2,	 1),
	dconst_0		(0x0e,	 2,	 2),
	dconst_1		(0x0f,	 2,	 2),
	bipush			(0x10,	 3,	 1),
	sipush			(0x11,	 4,	 1),
	ldc				(0x12,	 3,	 1),
	ldc_w			(0x13,	 4,	 1),
	ldc2_w			(0x14,	 4,	 2),
	iload			(0x15,	 3,	 1),
	lload			(0x16,	 3,	 2),
	fload			(0x17,	 3,	 1),
	dload			(0x18,	 3,	 2),
	aload			(0x19,	 3,	 1),
	iload_0			(0x1a,	 2,	 1),
	iload_1			(0x1b,	 2,	 1),
	iload_2			(0x1c,	 2,	 1),
	iload_3			(0x1d,	 2,	 1),
	lload_0			(0x1e,	 2,	 2),
	lload_1			(0x1f,	 2,	 2),
	lload_2			(0x20,	 2,	 2),
	lload_3			(0x21,	 2,	 2),
	fload_0			(0x22,	 2,	 1),
	fload_1			(0x23,	 2,	 1),
	fload_2			(0x24,	 2,	 1),
	fload_3			(0x25,	 2,	 1),
	dload_0			(0x26,	 2,	 2),
	dload_1			(0x27,	 2,	 2),
	dload_2			(0x28,	 2,	 2),
	dload_3			(0x29,	 2,	 2),
	aload_0			(0x2a,	 2,	 1),
	aload_1			(0x2b,	 2,	 1),
	aload_2			(0x2c,	 2,	 1),
	aload_3			(0x2d,	 2,	 1),
	iaload			(0x2e,	 2,	-1),
	laload			(0x2f,	 2,	 0),
	faload			(0x30,	 2,	-1),
	daload			(0x31,	 2,	 0),
	aaload			(0x32,	 2,	-1),
	baload			(0x33,	 2,	-1),
	caload			(0x34,	 2,	-1),
	saload			(0x35,	 2,	-1),
	istore			(0x36,	 3,	-1),
	lstore			(0x37,	 3,	-2),
	fstore			(0x38,	 3,	-1),
	dstore			(0x39,	 3,	-2),
	astore			(0x3a,	 3,	-1),
	istore_0		(0x3b,	 2,	-1),
	istore_1		(0x3c,	 2,	-1),
	istore_2		(0x3d,	 2,	-1),
	istore_3		(0x3e,	 2,	-1),
	lstore_0		(0x3f,	 2,	-2),
	lstore_1		(0x40,	 2,	-2),
	lstore_2		(0x41,	 2,	-2),
	lstore_3		(0x42,	 2,	-2),
	fstore_0		(0x43,	 2,	-1),
	fstore_1		(0x44,	 2,	-1),
	fstore_2		(0x45,	 2,	-1),
	fstore_3		(0x46,	 2,	-1),
	dstore_0		(0x47,	 2,	-2),
	dstore_1		(0x48,	 2,	-2),
	dstore_2		(0x49,	 2,	-2),
	dstore_3		(0x4a,	 2,	-2),
	astore_0		(0x4b,	 2,	-1),
	astore_1		(0x4c,	 2,	-1),
	astore_2		(0x4d,	 2,	-1),
	astore_3		(0x4e,	 2,	-1),
	iastore			(0x4f,	 2, -3),
	lastore			(0x50,	 2,	-4),
	fastore			(0x51,	 2,	-3),
	dastore			(0x52,	 2,	-4),
	aastore			(0x53,	 2,	-3),
	bastore			(0x54,	 2,	-3),
	castore			(0x55,	 2,	-3),
	sastore			(0x56,	 2,	-3),
	pop				(0x57,	 2,	-1),
	pop2			(0x58,	 2,	-2),
	dup				(0x59,	 2,	 1),
	dup_x1			(0x5a,	 2,	 1),
	dup_x2			(0x5b,	 2,	 1),
	dup2			(0x5c,	 2,	 2),
	dup2_x1			(0x5d,	 2,	 2),
	dup2_x2			(0x5e,	 2,	 2),
	swap			(0x5f,	 2,	 0),
	iadd			(0x60,	 2,	-1),
	ladd			(0x61,	 2,	-2),
	fadd			(0x62,	 2,	-1),
	dadd			(0x63,	 2,	-2),
	isub			(0x64,	 2,	-1),
	lsub			(0x65,	 2,	-2),
	fsub			(0x66,	 2,	-1),
	dsub			(0x67,	 2,	-2),
	imul			(0x68,	 2,	-1),
	lmul			(0x69,	 2,	-2),
	fmul			(0x6a,	 2,	-1),
	dmul			(0x6b,	 2,	-2),
	idiv			(0x6c,	 2,	-1),
	ldiv			(0x6d,	 2,	-2),
	fdiv			(0x6e,	 2,	-1),
	ddiv			(0x6f,	 2,	-2),
	irem			(0x70,	 2,	-1),
	lrem			(0x71,	 2,	-2),
	frem			(0x72,	 2,	-1),
	drem			(0x73,	 2,	-2),
	ineg			(0x74,	 2,	 0),
	lneg			(0x75,	 2,	 0),
	fneg			(0x76,	 2,	 0),
	dneg			(0x77,	 2,	 0),
	ishl			(0x78,	 2,	-1),
	lshl			(0x79,	 2,	-1),
	ishr			(0x7a,	 2,	-1),
	lshr			(0x7b,	 2,	-1),
	iushr			(0x7c,	 2,	-1),
	lushr			(0x7d,	 2,	-1),
	iand			(0x7e,	 2,	-1),
	land			(0x7f,	 2,	-2),
	ior				(0x80,	 2,	-1),
	lor				(0x81,	 2,	-2),
	ixor			(0x82,	 2,	-1),
	lxor			(0x83,	 2,	-2),
	iinc			(0x84,	 4,	 0),
	i2l				(0x85,	 2,	 1),
	i2f				(0x86,	 2,	 0),
	i2d				(0x87,	 2,	 1),
	l2i				(0x88,	 2,	-1),
	l2f				(0x89,	 2,	-1),
	l2d				(0x8a,	 2,	 0),
	f2i				(0x8b,	 2,	 0),
	f2l				(0x8c,	 2,	 1),
	f2d				(0x8d,	 2,	 1),
	d2i				(0x8e,	 2,	-1),
	d2l				(0x8f,	 2,	 0),
	d2f				(0x90,	 2,	-1),
	i2b				(0x91,	 2,	 0),
	i2c				(0x92,	 2,	 0),
	i2s				(0x93,	 2,  0),
	lcmp			(0x94,	 2, -3),
	fcmpl			(0x95,	 2, -1),
	fcmpg			(0x96,	 2, -1),
	dcmpl			(0x97,	 2,	-3),
	dcmpg			(0x98,	 2,	-3),
	ifeq			(0x99,	 4,	-1, JUMP),
	ifne			(0x9a,	 4,	-1, JUMP),
	iflt			(0x9b,	 4,	-1, JUMP),
	ifge			(0x9c,	 4,	-1, JUMP),
	ifgt			(0x9d,	 4,	-1, JUMP),
	ifle			(0x9e,	 4,	-1, JUMP),
	if_icmpeq		(0x9f,	 4,	-2, JUMP),
	if_icmpne		(0xa0,	 4,	-2, JUMP),
	if_icmplt		(0xa1,	 4,	-2, JUMP),
	if_icmpge		(0xa2,	 4,	-2, JUMP),
	if_icmpgt		(0xa3,	 4,	-2, JUMP),
	if_icmple		(0xa4,	 4,	-2, JUMP),
	if_acmpeq		(0xa5,	 4,	-2, JUMP),
	if_acmpne		(0xa6,	 4,	-2, JUMP),
	_goto			(0xa7,	 4,	 U, TERMINAL, JUMP),
	jsr				(0xa8,	 4,	 U, TERMINAL, JUMP),
	ret				(0xa9,	 4,	 U, TERMINAL, JUMP),
	tableswitch		(0xaa,	TERMINAL, JUMP),
	lookupswitch	(0xab,	TERMINAL, JUMP),
	ireturn			(0xac,	TERMINAL),
	lreturn			(0xad,	TERMINAL),
	freturn			(0xae,	TERMINAL),
	dreturn			(0xaf,	TERMINAL),
	areturn			(0xb0,	TERMINAL),
	_return			(0xb1,	TERMINAL),
	getstatic		(0xb2,	 4,	 U, UPDATES_STACK_OFFSET),
	putstatic		(0xb3,	 4,	 U, UPDATES_STACK_OFFSET),
	getfield		(0xb4,	 4,	 U, UPDATES_STACK_OFFSET),
	putfield		(0xb5,	 4,	 U, UPDATES_STACK_OFFSET),
	invokevirtual	(0xb6,	NO_SUPERINSTRUCTION),
	invokespecial	(0xb7,	NO_SUPERINSTRUCTION),
	invokestatic	(0xb8,	NO_SUPERINSTRUCTION),
	invokeinterface	(0xb9,	NO_SUPERINSTRUCTION),
	invokedynamic	(0xba,	NO_SUPERINSTRUCTION),
	_new			(0xbb,	 4,	 1),
	newarray		(0xbc,	 3,	 0),
	anewarray		(0xbd,	 4,	 0),
	arraylength		(0xbe,	 2,	 0),
	athrow			(0xbf,	TERMINAL),
	checkcast		(0xc0,	 4,	 0),
	_instanceof		(0xc1,	 4,  0),
	monitorenter	(0xc2,	NO_SUPERINSTRUCTION),
	monitorexit		(0xc3,	 2, -1),
	wide			(0xc4,	UPDATES_STACK_OFFSET, UPDATES_PC_OFFSET),
	multianewarray	(0xc5,	 5,  U, UPDATES_STACK_OFFSET),
	ifnull			(0xc6,	 4,	-1),
	ifnonnull		(0xc7,	 4,	-1),
	goto_w			(0xc8,	TERMINAL, JUMP),
	jsr_w			(0xc9,	TERMINAL, JUMP),
	breakpoint		(0xca,	NO_SUPERINSTRUCTION),
	fast_aldc		(230,	 3,  1),
	fast_aldc_w		(231,	 4,  1),
	
	return_register_finalizer(232, NO_SUPERINSTRUCTION),
	invokehandle	(233,	NO_SUPERINSTRUCTION),
	
	end_sequence	(-1,	 0,	 0, TERMINAL);
	
	/*
	 * TODO:
	 * - [x] wide instruction in base.hpp
	 * - [x] fast instructions here
	 * - [x] 'flush' instruction which sets PC and bytecode, and does a CONTINUE in base.hpp
	 * - [x] make code generator
	 * - [x] edit bytecodeInterpreter.cpp to use the generated file
	 * - [x] make an interpreter with the default instruction set (no changes)
	 * - get that to work
	 * - see what rewriter.cpp does to get fast instructions
	 * - port that to Java / the codestretcher agent
	 */
	
	private final int instr;
	private final int pc;
	private final int stack;
	private final EnumSet<BytecodeFlag> flags = EnumSet.noneOf(BytecodeFlag.class);
	
	/**
	 * Regular BytecodePrimitive with a fixed pc offset and stack offset.
	 * @param instr the instruction byte.
	 * @param pc the pc offset.
	 * @param stack the stack offset.
	 * @param flags additional flags.
	 */
	private BytecodePrimitive(int instr, int pc, int stack, BytecodeFlag... flags) {
		this.instr = instr;
		this.pc = pc;
		this.stack = stack;
		
		if (instr == U || stack == U) {
			Stream<BytecodeFlag> extraFlags;
			if (instr == U && stack == U)
				extraFlags = Stream.of(UNKNOWN_PC_OFFSET, UNKNOWN_STACK_OFFSET);
			else if (instr == U)
				extraFlags = Stream.of(UNKNOWN_PC_OFFSET);
			else
				extraFlags = Stream.of(UNKNOWN_STACK_OFFSET);
			
			Stream.concat(
					Arrays.stream(flags), extraFlags)
					.distinct()
					.forEach(this.flags::add);
		} else {
			this.flags.addAll(Arrays.asList(flags));
		}
	}
	
	/**
	 * BytecodePrimitive with an unknown pc and stack offset.
	 * @param instr the instruction byte.
	 * @param pc the pc offset.
	 * @param stack the stack offset.
	 * @param flags additional flags.
	 */
	private BytecodePrimitive(int instr, BytecodeFlag... flags) {
		this(instr, U, U, flags);
	}
	
	public boolean hasFlag(BytecodeFlag flag) {
		return this.flags.contains(flag);
	}
	
	public String getHandlerName() {
		String name = this.name();
		if (name.startsWith("_"))
			return name.substring(1);
		return name;
	}
	
	public int getInstr() {
		return instr;
	}

	public int getPc() {
		return pc;
	}

	public int getStack() {
		return stack;
	}

	enum BytecodeFlag {
		/**
		 * This bytecode primitive has an unknown PC offset, and as such the PC
		 * cannot be incremented symbolically past this primitive.
		 */
		UNKNOWN_PC_OFFSET,
		
		/**
		 * This bytecode primitive has an unknown stack requirement, and as such
		 * the stack offset cannot be modified symbolically past this primitive.
		 */
		UNKNOWN_STACK_OFFSET,
		
		/**
		 * This bytecode cannot be part of a superinstruction.
		 * Typically due to it jumping to itself via the frame manager (monitorenter, all invokes).
		 * requiring it to both be the last and the first instruction.
		 */
		NO_SUPERINSTRUCTION,
		
		/**
		 * Control flow never continues normally after this instruction (e.g. unconditional jump,
		 * return), so there's no use in continuing the superinstruction after it.
		 */
		TERMINAL,
		
		/**
		 * This instruction may perform a jump. Use together with {@link #TERMINAL} to signal
		 * an unconditional jump.
		 */
		JUMP,
		
		/**
		 * This bytecode primitive modifies the "pc" variable by writing its own size +
		 * whatever the value of the symbolic PC expression is. When creating a superinstruction
		 * with such an instruction in the pipeline, the "pc" expression is cleared again
		 * for succesors.
		 * Typically combined with {@link #UNKNOWN_PC_OFFSET}.
		 */
		UPDATES_PC_OFFSET,
		
		/**
		 * This bytecode primitives modifies the "topOfStack" variable by writing its own
		 * stack demand + the value of the symbolic "offset" expression.
		 * Typically combined with {@link #UNKNOWN_STACK_OFFSET}.
		 */
		UPDATES_STACK_OFFSET;
		
		public static final int U = Integer.MIN_VALUE;
	}
}
