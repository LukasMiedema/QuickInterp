// ASM: a very small and fast Java bytecode manipulation framework
// Copyright (c) 2000-2011 INRIA, France Telecom
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of the copyright holders nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.
package jdk.internal.vm.si.impl.asm.tree;

import java.util.Map;
import jdk.internal.vm.si.impl.asm.MethodVisitor;
import jdk.internal.vm.si.impl.asm.Opcodes;

/**
 * A node that represents a wide instruction and wraps around that instruction.
 * This is needed to have an accurate representation of the bytecodes in the ASM object graph
 * on which sequences of opcodes can be replaced.
 *
 * @author Eric Bruneton
 */
public class WideInsnNode extends AbstractInsnNode {

	private final AbstractInsnNode node;

	/**
	 * Constructs a new {@link WideInsnNode}.
	 *
	 * @param opcode the opcode of the instruction to be constructed.
	 * 		The opcode is the opcode of the "wide" instruction, e.g. IINC or 
	 */
	public WideInsnNode(AbstractInsnNode node) {
		super(Opcodes.WIDE);
		this.node = node;
	}

	@Override
	public int getType() {
		return WIDE_INSN;
	}

	@Override
	public void accept(final MethodVisitor methodVisitor) {
		if (this.node instanceof IincInsnNode) {
			var iinc = (IincInsnNode) this.node;
			methodVisitor.visitWideIincInsn(iinc.var, iinc.incr);
		} else if (this.node instanceof VarInsnNode) {
			var varInsn = (IincInsnNode) this.node;
			methodVisitor.visitWideVarInsn(varInsn.opcode, varInsn.var);
		} else {
			throw new AssertionError("Unrecognized wide type " + node.getClass());
		}
		acceptAnnotations(methodVisitor);
	}

	@Override
	public AbstractInsnNode clone(final Map<LabelNode, LabelNode> clonedLabels) {
		return new WideInsnNode(this.node).cloneAnnotations(this);
	}
}
