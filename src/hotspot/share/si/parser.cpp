
#include "jnif.hpp"

namespace jnif {

    namespace parser {

/**
 * Implements a memory buffer reader in big-endian encoding.
 */
        class BufferReader {
        public:

            /**
             * Constructs a BufferReader from a memory buffer and its size.
             * The buffer must be an accessible and readable memory location
             * at least of the specified size.
             *
             * @param buffer The memory buffer to read from.
             * @param size The size of the buffer in bytes.
             *
             */
            BufferReader(const u1 *buffer, u4 size) : buffer(buffer), _size(size), off(0) {
            }

            /**
             * When this buffer reader finishes, it will check whether the end has
             * been reached, i.e., all bytes from buffer were read or skipped.
             * In other words, when the buffer reader br is destroyed, the condition
             *
             * @f[ br.offset() = br.size() @f]
             *
             * must hold.
             */
            ~BufferReader() {
//            JnifError::check(off == _size, "Expected end of buffer");
            }

            int size() const {
                return _size;
            }

            u1 readu1() {
                JnifError::check(off + 1 <= _size, "Invalid read");

                u1 result = buffer[off];

                off += 1;

                return result;
            }

            u2 readu2() {
                JnifError::check(off + 2 <= _size, "Invalid read 2");

                u1 r0 = buffer[off + 0];
                u1 r1 = buffer[off + 1];

                u2 result = r0 << 8 | r1;

                off += 2;

                return result;
            }

            u4 readu4() {
                JnifError::check(off + 4 <= _size, "Invalid read 4");
                //if (off >= 256 ) Error::raise()

                u1 r0 = buffer[off + 0];
                u1 r1 = buffer[off + 1];
                u1 r2 = buffer[off + 2];
                u1 r3 = buffer[off + 3];

                u4 result = r0 << 24 | r1 << 16 | r2 << 8 | r3;

                off += 4;

                return result;
            }

            void skip(int count) {
                const char *const m = "Invalid read: %d (offset: %d)";
                JnifError::check(off + count <= _size, m, count, off);

                off += count;
            }

            int offset() const {
                return off;
            }

            const u1 *pos() const {
                return buffer + off;
            }

            bool eor() const {
                return off == _size;
            }

        private:

            const u1 *const buffer;
            const int _size;
            int off;

        };

        struct ConstPoolParser {

            void parse(BufferReader *br, ConstPool *cp) {
                u2 count = br->readu2();

                for (int i = 1; i < count; i++) {
                    u1 tag = br->readu1();

                    switch (tag) {
                        case ConstPool::CLASS: {
                            u2 classNameIndex = br->readu2();
                            cp->addClass(classNameIndex);
                            break;
                        }
                        case ConstPool::FIELDREF: {
                            u2 classIndex = br->readu2();
                            u2 nameAndTypeIndex = br->readu2();
                            cp->addFieldRef(classIndex, nameAndTypeIndex);
                            break;
                        }
                        case ConstPool::METHODREF: {
                            u2 classIndex = br->readu2();
                            u2 nameAndTypeIndex = br->readu2();
                            cp->addMethodRef(classIndex, nameAndTypeIndex);
                            break;
                        }
                        case ConstPool::INTERMETHODREF: {
                            u2 classIndex = br->readu2();
                            u2 nameAndTypeIndex = br->readu2();
                            cp->addInterMethodRef(classIndex, nameAndTypeIndex);
                            break;
                        }
                        case ConstPool::STRING: {
                            u2 utf8Index = br->readu2();
                            cp->addString(utf8Index);
                            break;
                        }
                        case ConstPool::INTEGER: {
                            u4 value = br->readu4();
                            cp->addInteger(value);
                            break;
                        }
                        case ConstPool::FLOAT: {
                            u4 value = br->readu4();
                            float fvalue = *(float *) &value;
                            cp->addFloat(fvalue);
                            break;
                        }
                        case ConstPool::LONG: {
                            u4 high = br->readu4();
                            u4 low = br->readu4();
                            long value = ((long) high << 32) + low;
                            cp->addLong(value);
                            i++;
                            break;
                        }
                        case ConstPool::DOUBLE: {
                            u4 high = br->readu4();
                            u4 low = br->readu4();
                            long lvalue = ((long) high << 32) + low;
                            double dvalue = *(double *) &lvalue;
                            cp->addDouble(dvalue);
                            i++;
                            break;
                        }
                        case ConstPool::NAMEANDTYPE: {
                            u2 nameIndex = br->readu2();
                            u2 descIndex = br->readu2();
                            cp->addNameAndType(nameIndex, descIndex);
                            break;
                        }
                        case ConstPool::UTF8: {
                            u2 len = br->readu2();
                            cp->addUtf8((const char *) br->pos(), len);
                            br->skip(len);
                            break;
                        }
                        case ConstPool::METHODHANDLE: {
                            u1 refKind = br->readu1();
                            u2 refIndex = br->readu2();
                            cp->addMethodHandle(refKind, refIndex);
                            break;
                        }
                        case ConstPool::METHODTYPE: {
                            u2 descIndex = br->readu2();
                            cp->addMethodType(descIndex);
                            break;
                        }
                        case ConstPool::INVOKEDYNAMIC: {
                            u2 bootMethodAttrIndex = br->readu2();
                            u2 nameAndTypeIndex = br->readu2();
                            cp->addInvokeDynamic(bootMethodAttrIndex, nameAndTypeIndex);
                            break;
                        }
                        default:
                            throw Exception("Error while reading tag: ", tag);
                    }
                }
            }
        };

        template<class... TAttrParsers>
        struct AttrParser {
            template<class... TArgs>
            Attr *parse(
                    u2 nameIndex, u4 len, const u1 *data, const string &,
                    ClassFile *cp, TArgs...
            ) {
                return cp->_arena.create<UnknownAttr>(nameIndex, len, data, cp);
            }
        };

        template<class TAttrParser, class... TAttrParsers>
        struct AttrParser<TAttrParser, TAttrParsers...> : AttrParser<TAttrParsers...> {

            template<class... TArgs>
            Attr *parse(u2 nameIndex, u4 len, const u1 *data, const string &attrName,
                        ClassFile *cp, TArgs... args) {
                if (attrName == TAttrParser::AttrName) {
                    BufferReader br(data, len);
                    return TAttrParser().parse(&br, cp, nameIndex, args...);
                } else {
                    return AttrParser<TAttrParsers...>::parse(
                            nameIndex, len, data, attrName, cp, args...);
                }
            }

        };

        template<class... TAttrParsers>
        struct AttrsParser {

            template<class... TArgs>
            void parse(BufferReader *br, ClassFile *cp, Attrs *as, TArgs... args) {
                u2 attrCount = br->readu2();

                for (int i = 0; i < attrCount; i++) {
                    u2 nameIndex = br->readu2();
                    u4 len = br->readu4();
                    const u1 *data = br->pos();

                    br->skip(len);

                    string attrName = cp->getUtf8(nameIndex);

                    Attr *a = AttrParser<TAttrParsers...>().parse(
                            nameIndex, len, data, attrName, cp, args...);
                    as->add(a);
                }
            }

        };

        OpKind OPKIND[256] = {KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO,
                              KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO,
                              KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_BIPUSH,
                              KIND_SIPUSH, KIND_LDC, KIND_LDC, KIND_LDC, KIND_VAR, KIND_VAR, KIND_VAR,
                              KIND_VAR, KIND_VAR, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO,
                              KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO,
                              KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO,
                              KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO,
                              KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO,
                              KIND_VAR, KIND_VAR, KIND_VAR, KIND_VAR, KIND_VAR, KIND_ZERO, KIND_ZERO,
                              KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO,
                              KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO,
                              KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO,
                              KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO,
                              KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO,
                              KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO,
                              KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO,
                              KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO,
                              KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO,
                              KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO,
                              KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO,
                              KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_IINC,
                              KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO,
                              KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO,
                              KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO,
                              KIND_ZERO, KIND_ZERO, KIND_JUMP, KIND_JUMP, KIND_JUMP, KIND_JUMP,
                              KIND_JUMP, KIND_JUMP, KIND_JUMP, KIND_JUMP, KIND_JUMP, KIND_JUMP,
                              KIND_JUMP, KIND_JUMP, KIND_JUMP, KIND_JUMP, KIND_JUMP, KIND_JUMP,
                              KIND_VAR, KIND_TABLESWITCH, KIND_LOOKUPSWITCH, KIND_ZERO, KIND_ZERO,
                              KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_ZERO, KIND_FIELD, KIND_FIELD,
                              KIND_FIELD, KIND_FIELD, KIND_INVOKE, KIND_INVOKE, KIND_INVOKE,
                              KIND_INVOKEINTERFACE, KIND_INVOKEDYNAMIC, KIND_TYPE, KIND_NEWARRAY,
                              KIND_TYPE, KIND_ZERO, KIND_ZERO, KIND_TYPE, KIND_TYPE, KIND_ZERO,
                              KIND_ZERO, KIND_ZERO, KIND_MULTIARRAY, KIND_JUMP, KIND_JUMP,
                              KIND_JUMP_W, KIND_JUMP_W, KIND_RESERVED, KIND_RESERVED,
                              KIND_RESERVED, KIND_RESERVED, KIND_RESERVED, KIND_RESERVED,
                              KIND_RESERVED, KIND_RESERVED, KIND_RESERVED, KIND_RESERVED,
                              KIND_RESERVED, KIND_RESERVED, KIND_RESERVED, KIND_RESERVED,
                              KIND_RESERVED, KIND_RESERVED, KIND_RESERVED, KIND_RESERVED,
                              KIND_RESERVED, KIND_RESERVED, KIND_RESERVED, KIND_RESERVED,
                              KIND_RESERVED, KIND_RESERVED, KIND_RESERVED, KIND_RESERVED,
                              KIND_RESERVED, KIND_RESERVED, KIND_RESERVED, KIND_RESERVED,
                              KIND_RESERVED, KIND_RESERVED, KIND_RESERVED, KIND_RESERVED,
                              KIND_RESERVED, KIND_RESERVED, KIND_RESERVED, KIND_RESERVED,
                              KIND_RESERVED, KIND_RESERVED, KIND_RESERVED, KIND_RESERVED,
                              KIND_RESERVED, KIND_RESERVED, KIND_RESERVED, KIND_RESERVED,
                              KIND_RESERVED, KIND_RESERVED, KIND_RESERVED, KIND_RESERVED,
                              KIND_RESERVED, KIND_RESERVED, KIND_RESERVED, KIND_RESERVED,};

        class LabelManager {
        public:

            LabelManager(u4 codeLen, InstList &instList) :
                    codeLen(codeLen),
                    instList(instList),
                    labels(instList.constPool->_arena.newArray<LabelInst *>(codeLen + 1)) {
                for (u4 i = 0; i < codeLen + 1; i++) {
                    labels[i] = nullptr;
                }
            }

            LabelInst *createLabel(int labelPos) {
                JnifError::check(0 <= labelPos, "Invalid position for label: ", labelPos);
                JnifError::check((u4) labelPos < codeLen + 1,
                                 "Invalid position for label: ", labelPos, ", : ", codeLen);

                LabelInst *&lab = labels[labelPos];
                if (lab == nullptr) {
                    lab = instList.createLabel();
                }

                return lab;
            }

            LabelInst *createExceptionLabel(u2 labelPos, bool isTryStart, bool isTryEnd,
                                            bool isCatchHandler) {
                JnifError::check(labelPos != codeLen || isTryEnd,
                                 "Only tryEnd can have a labelPos equal to codeLen.");

                LabelInst *label = createLabel(labelPos);
                label->isTryStart = label->isTryStart || isTryStart;
                label->isCatchHandler = label->isCatchHandler || isCatchHandler;

                return label;
            }

            bool hasLabel(u2 labelPos) const {
                JnifError::jnifAssert(labelPos < codeLen + 1, "Invalid position for label: ",
                                  labelPos);

                return labels[labelPos] != nullptr;
            }

            void putLabelIfExists(u2 labelPos) const {
                if (hasLabel(labelPos)) {
                    LabelInst *label = (*this)[labelPos];
                    label->_offset = labelPos;
                    instList.addLabel(label);
                }
            }

            LabelInst *operator[](u2 labelPos) const {
                JnifError::jnifAssert(hasLabel(labelPos), "No label in position: ", labelPos);

                return labels[labelPos];
            }

            u4 codeLen;

            InstList &instList;
        private:

            LabelInst **labels;
        };

        struct LineNumberTableAttrParser {

            static constexpr const char *AttrName = "LineNumberTable";

            Attr *parse(BufferReader *br, ClassFile *cp, ConstPool::Index nameIndex,
                        LabelManager *labelManager) {

                u2 lntlen = br->readu2();

                LntAttr *lnt = cp->_arena.create<LntAttr>(nameIndex, cp);

                for (int i = 0; i < lntlen; i++) {
                    LntAttr::LnEntry e;
                    u2 startpc = br->readu2();
                    u2 lineno = br->readu2();

                    e.startPcLabel = labelManager->createLabel(startpc);

                    e.startpc = startpc;
                    e.lineno = lineno;

                    lnt->lnt.push_back(e);
                }

                return lnt;
            }

        };

        struct SourceFileAttrParser {

            static constexpr const char *AttrName = "SourceFile";

            SourceFileAttr *parse(BufferReader *br, ClassFile *cp, ConstPool::Index nameIndex) {
                u2 sourceFileIndex = br->readu2();
                SourceFileAttr *attr = cp->_arena.create<SourceFileAttr>(nameIndex, sourceFileIndex, cp);
                return attr;
            }

        };

        struct SignatureAttrParser {

            static constexpr const char *AttrName = "Signature";

            SignatureAttr *parse(BufferReader *br, ClassFile *cp, ConstPool::Index nameIndex) {
                ConstPool::Index sigIndex = br->readu2();
                SignatureAttr *attr = cp->_arena.create<SignatureAttr>(nameIndex, sigIndex, cp);
                return attr;
            }

        };

        struct LocalVariableTypeTableAttrParser {

            static constexpr const char *AttrName = "LocalVariableTypeTable";

            Attr *parse(BufferReader *br, ClassFile *cp, ConstPool::Index nameIndex,
                        LabelManager *labelManager) {

                u2 count = br->readu2();

                LvtAttr *lvt = cp->_arena.create<LvtAttr>(ATTR_LVTT, nameIndex, cp);

                for (u2 i = 0; i < count; i++) {
                    LvtAttr::LvEntry e;

                    u2 startPc = br->readu2();

                    e.startPcLabel = labelManager->createLabel(startPc);

                    e.startPc = startPc;
                    e.len = br->readu2();
                    e.varNameIndex = br->readu2();

                    // Signature instead of descriptor.
                    e.varDescIndex = br->readu2();
                    e.index = br->readu2();

                    lvt->lvt.push_back(e);
                }

                return lvt;
            }

        };

        struct StackMapTableAttrParser {

            static constexpr const char *AttrName = "StackMapTable";

            Type parseType(BufferReader *br, const ConstPool *cp, LabelManager *labelManager) {
                u1 tag = br->readu1();

                switch (tag) {
                    case TYPE_TOP:
                        return TypeFactory::topType();
                    case TYPE_INTEGER:
                        return TypeFactory::intType();
                    case TYPE_FLOAT:
                        return TypeFactory::floatType();
                    case TYPE_LONG:
                        return TypeFactory::longType();
                    case TYPE_DOUBLE:
                        return TypeFactory::doubleType();
                    case TYPE_NULL:
                        return TypeFactory::nullType();
                    case TYPE_UNINITTHIS:
                        return TypeFactory::uninitThisType();
                    case TYPE_OBJECT: {
                        u2 cpIndex = br->readu2();
                        JnifError::check(cp->isClass(cpIndex), "Bad cpindex: ", cpIndex);
                        string className = cp->getClassName(cpIndex);
                        return TypeFactory::objectType(className, cpIndex);
                    }
                    case TYPE_UNINIT: {
                        u2 offset = br->readu2();
                        LabelInst *label = labelManager->createLabel(offset);
                        return TypeFactory::uninitType(offset, label);
                    }
                    default:
                        throw Exception("Error on parse smt");
                }
            }

            void parseTs(BufferReader *br, int count, std::vector<Type> &locs,
                         const ConstPool *cp, LabelManager *labelManager) {
                for (u1 i = 0; i < count; i++) {
                    Type t = parseType(br, cp, labelManager);
                    locs.push_back(t);
                }
            }

            Attr *parse(BufferReader *br, ClassFile *cp, u2 nameIndex, LabelManager *labelManager) {
                SmtAttr *smt = cp->_arena.create<SmtAttr>(nameIndex, cp);

                u2 numberOfEntries = br->readu2();

                int toff = -1;

                for (u2 i = 0; i < numberOfEntries; i++) {
                    u1 frameType = br->readu1();

                    SmtAttr::Entry e;
                    e.frameType = frameType;

                    if (0 <= frameType && frameType <= 63) {
                        toff += frameType;
                    } else if (64 <= frameType && frameType <= 127) {
                        parseTs(br, 1, e.sameLocals_1_stack_item_frame.stack, cp, labelManager);

                        toff += frameType - 64;
                    } else if (frameType == 247) {
                        u2 offsetDelta = br->readu2();
                        e.same_locals_1_stack_item_frame_extended.offset_delta = offsetDelta;

                        toff += e.same_locals_1_stack_item_frame_extended.offset_delta;
                        parseTs(br, 1, e.same_locals_1_stack_item_frame_extended.stack, cp, labelManager);
                    } else if (248 <= frameType && frameType <= 250) {
                        u2 offsetDelta = br->readu2();
                        e.chop_frame.offset_delta = offsetDelta;

                        toff += e.chop_frame.offset_delta;
                    } else if (frameType == 251) {
                        u2 offsetDelta = br->readu2();
                        e.same_frame_extended.offset_delta = offsetDelta;

                        toff += e.same_frame_extended.offset_delta;
                    } else if (252 <= frameType && frameType <= 254) {
                        u2 offsetDelta = br->readu2();
                        e.append_frame.offset_delta = offsetDelta;
                        parseTs(br, frameType - 251, e.append_frame.locals, cp, labelManager);

                        toff += e.append_frame.offset_delta;
                    } else if (frameType == 255) {
                        u2 offsetDelta = br->readu2();
                        e.full_frame.offset_delta = offsetDelta;

                        u2 numberOfLocals = br->readu2();
                        parseTs(br, numberOfLocals, e.full_frame.locals, cp, labelManager);

                        u2 numberOfStackItems = br->readu2();
                        parseTs(br, numberOfStackItems, e.full_frame.stack, cp, labelManager);

                        toff += e.full_frame.offset_delta;
                    }

                    toff += 1;

                    LabelInst *label = labelManager->createLabel(toff);

                    e.label = label;

                    smt->entries.push_back(e);
                }

                return smt;
            }

        };

/**
 * The ExceptionsAttrParser parses the Exceptions attribute of a method.
 */
        struct ExceptionsAttrParser {

            static constexpr const char *AttrName = "Exceptions";

            Attr *parse(BufferReader *br, ClassFile *cp, ConstPool::Index nameIndex) {
                u2 len = br->readu2();

                vector<ConstPool::Index> es;
                for (int i = 0; i < len; i++) {
                    ConstPool::Index exceptionIndex = br->readu2();

                    es.push_back(exceptionIndex);
                }

                Attr *attr = cp->_arena.create<ExceptionsAttr>(nameIndex, cp, es);

                return attr;
            }

        };

        struct LocalVariableTableAttrParser {

            static constexpr const char *AttrName = "LocalVariableTable";

            Attr *parse(BufferReader *br, ClassFile *cp, ConstPool::Index nameIndex,
                        LabelManager *labelManager) {

                u2 count = br->readu2();

                LvtAttr *lvt = cp->_arena.create<LvtAttr>(ATTR_LVT, nameIndex, cp);

                for (u2 i = 0; i < count; i++) {
                    LvtAttr::LvEntry e;

                    u2 startPc = br->readu2();

                    e.startPcLabel = labelManager->createLabel(startPc);

                    e.startPc = startPc;
                    e.len = br->readu2();
                    //u2 len = e.len;
                    //u2 endPc = startPc + len;
                    //labelManager
                    //Error::check(endPc <= labelManager.codeLen, "inv endPc");
                    //Error::check(endPc <= labelManager.codeLen, "inv endPc");

                    //e.endPcLabel = labelManager.createLabel(endPc);
                    //Error::jnifAssert(e.endPcLabel != NULL, "asdf");
                    //Error::jnifAssert(e.endPcLabel->label()->id >= 1, "1");
                    //Error::jnifAssert(e.endPcLabel->label()->id <= 65536, "65536");

                    e.varNameIndex = br->readu2();
                    e.varDescIndex = br->readu2();
                    e.index = br->readu2();

                    lvt->lvt.push_back(e);
                }

                return lvt;
            }

        };

        template<typename ... TAttrParserList>
        struct CodeAttrParser {

            static constexpr const char *AttrName = "Code";

            void parseInstTargets(BufferReader &br, LabelManager &labelManager) {
                while (!br.eor()) {
                    int offset = br.offset();

                    Opcode opcode = (Opcode) br.readu1();
                    OpKind kind = OPKIND[(int) opcode];

                    switch (kind) {
                        case KIND_ZERO:
                            if (opcode == Opcode::wide) {
                                Opcode opcodew = (Opcode) br.readu1();
                                if (opcodew == Opcode::iinc) {
                                    br.skip(4);
                                } else {
                                    br.skip(2);
                                }
                            }
                            break;
                        case KIND_BIPUSH:
                        case KIND_VAR:
                        case KIND_NEWARRAY:
                            br.skip(1);
                            break;
                        case KIND_SIPUSH:
                        case KIND_IINC:
                        case KIND_FIELD:
                        case KIND_INVOKE:
                        case KIND_TYPE:
                            br.skip(2);
                            break;
                        case KIND_MULTIARRAY:
                            br.skip(3);
                            break;
                        case KIND_INVOKEINTERFACE:
                            br.skip(4);
                            break;
                        case KIND_INVOKEDYNAMIC:
                            br.skip(4);
                            break;
                        case KIND_LDC:
                            if (opcode == Opcode::ldc) {
                                br.readu1();
                            } else {
                                br.readu2();
                            }
                            break;
                        case KIND_JUMP:
                        case KIND_JUMP_W: {
                            bool wide = kind == KIND_JUMP_W;

                            // cast readu2 to signed short first to get proper sign extension
                            int targetOffset = wide ? br.readu4() : ((short) br.readu2());
                            int labelpos = offset + targetOffset;

                            if (labelpos >= br.size())
                                printf("error, target offset: %i, labelpos: %i, wide: %i\n", targetOffset, labelpos, wide);

                            JnifError::jnifAssert(labelpos >= 0, "invalid target for jump: must be >= 0");
                            JnifError::jnifAssert(labelpos < br.size(), "invalid target for jump");

                            labelManager.createLabel(labelpos);
                            break;
                        }
                        case KIND_TABLESWITCH: {
                            for (int i = 0; i < (((-offset - 1) % 4) + 4) % 4; i++) {
                                u1 pad = br.readu1();
                                JnifError::jnifAssert(pad == 0, "Padding must be zero");
                            }

                            {
                                bool check2 = br.offset() % 4 == 0;
                                JnifError::jnifAssert(check2, "%d", br.offset());
                            }

                            int defOffset = br.readu4();
                            labelManager.createLabel(offset + defOffset);

                            int low = br.readu4();
                            int high = br.readu4();

                            JnifError::jnifAssert(low <= high, "low (%d) must be less or equal than high (%d)", low, high);

                            for (int i = 0; i < high - low + 1; i++) {
                                int targetOffset = br.readu4();
                                labelManager.createLabel(offset + targetOffset);
                            }
                            break;
                        }
                        case KIND_LOOKUPSWITCH: {
                            for (int i = 0; i < (((-offset - 1) % 4) + 4) % 4; i++) {
                                u1 pad = br.readu1();
                                JnifError::jnifAssert(pad == 0, "Padding must be zero");
                            }

                            int defOffset = br.readu4();
                            labelManager.createLabel(offset + defOffset);

                            u4 npairs = br.readu4();

                            for (u4 i = 0; i < npairs; i++) {
                                br.readu4(); // Key

                                int targetOffset = br.readu4();
                                labelManager.createLabel(offset + targetOffset);
                            }
                            break;
                        }
//			case KIND_RESERVED:
                            //			break;
                        default:
                            throw Exception("default kind in parseInstTargets: "
                                                    "opcode: ", opcode, ", kind: ", kind);
                    }
                }
            }

            Inst *parseInst(BufferReader &br, InstList &instList,
                            const LabelManager &labelManager, bool wide) {
                int offset = br.offset();

//		if (labelManager.hasLabel(offset)) {
//			LabelInst* label = labelManager[offset];
//			label->_offset = offset;
//			instList.addLabel(label);
//		}

                labelManager.putLabelIfExists(offset);

                Opcode opcode = (Opcode) br.readu1();
                u1 kind = OPKIND[(int) opcode];

                //Inst* instp = new Inst(opcode, kind);
                //instp->_offset = offset;
                //Inst& inst = *instp;

                //inst.opcode = (Opcode) br.readu1();
                //inst.kind = OPKIND[inst.opcode];

                if (kind == KIND_ZERO) {
                    if (opcode == Opcode::wide) {
                        if (wide)
                            throw new Exception("Cannot widen a wide instruction");
                    }
                    return instList.addZero(opcode);
                } else if (kind == KIND_BIPUSH) {
                    u1 value = br.readu1();
                    return instList.addBiPush(value);
                } else if (kind == KIND_SIPUSH) {
                    u2 value = br.readu2();
                    return instList.addSiPush(value);
                } else if (kind == KIND_LDC) {
                    u2 valueIndex;
                    if (opcode == Opcode::ldc) {
                        valueIndex = br.readu1();
                    } else {
                        valueIndex = br.readu2();
                    }
                    return instList.addLdc(opcode, valueIndex);
                } else if (kind == KIND_VAR) {
                    u2 lvindex = wide ? br.readu2() : br.readu1();
                    return instList.addVar(opcode, lvindex);
                } else if (kind == KIND_IINC) {
                    if (wide) {
                        u2 index = br.readu2();
                        u2 value = br.readu2();
                        return instList.addIinc(index, value);
                    } else {
                        u1 index = br.readu1();
                        u1 value = br.readu1();
                        return instList.addIinc(index, value);
                    }
                } else if (kind == KIND_JUMP || kind == KIND_JUMP_W) {
                    bool wide = kind == KIND_JUMP_W;

                    // cast readu2 to signed short first to get proper sign extension
                    int targetOffset = wide ? br.readu4() : ((short) br.readu2());
                    int labelpos = offset + targetOffset;

                    JnifError::check(labelpos >= 0,
                                     "invalid target for jump: must be >= 0");
                    JnifError::check(labelpos < br.size(), "invalid target for jump");

                    LabelInst *targetLabel = labelManager[offset + targetOffset];
                    JnifError::check(targetLabel != NULL, "invalid label");

                    return instList.addJump(opcode, (OpKind) kind, targetLabel);
                } else if (kind == KIND_TABLESWITCH) {
                    for (int i = 0; i < (((-offset - 1) % 4) + 4) % 4; i++) {
                        u1 pad = br.readu1();
                        JnifError::jnifAssert(pad == 0, "Padding must be zero");
                    }

                    {
                        bool check2 = br.offset() % 4 == 0;
                        JnifError::jnifAssert(check2, "%d", br.offset());
                    }

                    int defOffset = br.readu4();
                    LabelInst *def = labelManager[offset + defOffset];
                    int low = br.readu4();
                    int high = br.readu4();

                    JnifError::jnifAssert(low <= high,
                                      "low (%d) must be less or equal than high (%d)", low, high);

                    TableSwitchInst *ts = instList.addTableSwitch(def, low, high);
                    for (int i = 0; i < high - low + 1; i++) {
                        u4 targetOffset = br.readu4();
                        //ts->targets.push_back(labelManager[offset + targetOffset]);
                        ts->addTarget(labelManager[offset + targetOffset]);
                    }

                    //		fprintf(stderr, "parser ts: offset: %d\n", br.offset());

                    return ts;
                } else if (kind == KIND_LOOKUPSWITCH) {
                    for (int i = 0; i < (((-offset - 1) % 4) + 4) % 4; i++) {
                        u1 pad = br.readu1();
                        JnifError::jnifAssert(pad == 0, "Padding must be zero");
                    }

                    int defOffset = br.readu4();
                    LabelInst *defbyte = labelManager[offset + defOffset];
                    u4 npairs = br.readu4();

                    LookupSwitchInst *ls = instList.addLookupSwitch(defbyte, npairs);
                    for (u4 i = 0; i < npairs; i++) {
                        u4 key = br.readu4();
                        u4 offsetTarget = br.readu4();

                        ls->keys.push_back(key);
                        //ls->targets.push_back(labelManager[offset + offsetTarget]);
                        ls->addTarget(labelManager[offset + offsetTarget]);
                    }

                    return ls;
                } else if (kind == KIND_FIELD) {
                    u2 fieldRefIndex = br.readu2();

                    return instList.addField(opcode, fieldRefIndex);
                } else if (kind == KIND_INVOKE) {
                    u2 methodRefIndex = br.readu2();

                    return instList.addInvoke(opcode, methodRefIndex);
                } else if (kind == KIND_INVOKEINTERFACE) {
                    JnifError::jnifAssert(opcode == Opcode::invokeinterface, "invalid opcode");

                    u2 interMethodRefIndex = br.readu2();
                    u1 count = br.readu1();

                    JnifError::jnifAssert(count != 0, "Count is zero!");

                    u1 zero = br.readu1();
                    JnifError::jnifAssert(zero == 0, "Fourth operand must be zero");

                    return instList.addInvokeInterface(interMethodRefIndex, count);
                } else if (kind == KIND_INVOKEDYNAMIC) {
                    u2 callSite = br.readu2();
                    u2 zero = br.readu2();
                    JnifError::check(zero == 0, "Zero is not zero: ", zero);

                    return instList.addInvokeDynamic(callSite);
                } else if (kind == KIND_TYPE) {
                    ConstPool::Index classIndex = br.readu2();

                    return instList.addType(opcode, classIndex);
                } else if (kind == KIND_NEWARRAY) {
                    u1 atype = br.readu1();

                    return instList.addNewArray(atype);
                } else if (kind == KIND_MULTIARRAY) {
                    ConstPool::Index classIndex = br.readu2();
                    u1 dims = br.readu1();

                    return instList.addMultiArray(classIndex, dims);
                } else if (kind == KIND_RESERVED) {
                    throw Exception("FrParseReservedInstr not implemented");
                } else {
                    throw Exception("default kind in parseInstList");
                }
            }

            void parseInstList(BufferReader &br, InstList &instList,
                               const LabelManager &labelManager) {
                bool wide = false;
                while (!br.eor()) {
                    int offset = br.offset();
                    Inst *inst = parseInst(br, instList, labelManager, wide);
                    inst->_offset = offset;
                    wide = inst->isWide();
                }
            }

            Attr *parse(BufferReader *br, ClassFile *cp, u2 nameIndex) {

                CodeAttr *ca = cp->_arena.create<CodeAttr>(nameIndex, cp);

                ca->maxStack = br->readu2();
                ca->maxLocals = br->readu2();

                u4 codeLen = br->readu4();

                JnifError::check(codeLen > 0, "");
                JnifError::check(codeLen < (2 << 16), "");

                ca->codeLen = codeLen;

                const u1 *codeBuf = br->pos();
                br->skip(ca->codeLen);

                LabelManager labelManager(codeLen, ca->instList);

                {
                    BufferReader br(codeBuf, codeLen);
                    parseInstTargets(br, labelManager);
                }

                u2 exceptionTableCount = br->readu2();
                for (int i = 0; i < exceptionTableCount; i++) {
                    u2 startPc = br->readu2();
                    u2 endPc = br->readu2();
                    u2 handlerPc = br->readu2();
                    ConstPool::Index catchType = br->readu2();

                    JnifError::check(startPc < endPc, "");
                    JnifError::check(endPc <= ca->codeLen, "");
                    JnifError::check(handlerPc < ca->codeLen, "");
                    JnifError::check(catchType == ConstPool::NULLENTRY || cp->isClass(catchType), "");

                    ca->exceptions.push_back({
                                                     labelManager.createExceptionLabel(startPc, true, false, false),
                                                     labelManager.createExceptionLabel(endPc, false, true, false),
                                                     labelManager.createExceptionLabel(handlerPc, false, false, true),
                                                     catchType
                                             });
                }

                AttrsParser<TAttrParserList ...>().parse(br, cp, &ca->attrs, &labelManager);

                {
                    BufferReader br(codeBuf, codeLen);
                    parseInstList(br, ca->instList, labelManager);
                }

                labelManager.putLabelIfExists(codeLen);

                return ca;
            }

        };

/**
 * Represents an abstract java class file parser.
 *
 * Only suitable when TClassAttrsParser, TMethodAttrsParser and
 * TFieldAttrsParser are AttrsParser classes.
 */
        template<
                typename TConstPoolParser,
                typename TClassAttrsParser,
                typename TMethodAttrsParser,
                typename TFieldAttrsParser>
        class ClassParser;

/**
 * Represents an abstract java class file parser.
 *
 * Instantiation of the parser implementation.
 *
 * The template instantiation is the only one accepted, since it receives
 * the members and class attributes parsers as templates lists.
 */
        template<
                typename TConstPoolParser,
                typename ... TClassAttrParserList,
                typename ... TMethodAttrParserList,
                typename ... TFieldAttrParserList>
        class ClassParser<
                TConstPoolParser,
                AttrsParser<TClassAttrParserList...>,
                AttrsParser<TMethodAttrParserList...>,
                AttrsParser<TFieldAttrParserList...>
        > {
        public:

            typedef AttrsParser<TClassAttrParserList...> ClassAttrsParser;

            typedef AttrsParser<TMethodAttrParserList...> MethodAttrsParser;

            typedef AttrsParser<TFieldAttrParserList...> FieldAttrsParser;

            /**
             *
             */
            void parse(BufferReader *br, ClassFile *cf) {
                u4 magic = br->readu4();

                JnifError::check(
                        magic == ClassFile::MAGIC,
                        "Invalid magic number. Expected 0xcafebabe, found: ",
                        magic);

                u2 minorVersion = br->readu2();
                u2 majorVersion = br->readu2();

                cf->version = Version(majorVersion, minorVersion);

                TConstPoolParser().parse(br, cf);

                cf->accessFlags = br->readu2();
                cf->thisClassIndex = br->readu2();
                cf->superClassIndex = br->readu2();

                u2 interCount = br->readu2();
                for (int i = 0; i < interCount; i++) {
                    u2 interIndex = br->readu2();
                    cf->interfaces.push_back(interIndex);
                }

                u2 fieldCount = br->readu2();
                for (int i = 0; i < fieldCount; i++) {
                    u2 accessFlags = br->readu2();
                    u2 nameIndex = br->readu2();
                    u2 descIndex = br->readu2();

                    Field &f = cf->addField(nameIndex, descIndex, accessFlags);
                    FieldAttrsParser().parse(br, cf, &f.attrs);
                }

                u2 methodCount = br->readu2();
                for (int i = 0; i < methodCount; i++) {
                    u2 accessFlags = br->readu2();
                    u2 nameIndex = br->readu2();
                    u2 descIndex = br->readu2();

                    Method &m = cf->addMethod(nameIndex, descIndex, accessFlags);
                    MethodAttrsParser().parse(br, cf, &m.attrs);
                }

                ClassAttrsParser().parse(br, cf, &cf->attrs);
            }

        };

        ClassFileParser::ClassFileParser(const u1 *data, u4 len) {
            parse(data, len, this);
        }

        void ClassFileParser::parse(const u1 *data, u4 len, ClassFile *classFile) {
            BufferReader br(data, len);
            ClassParser<
                    ConstPoolParser,
                    AttrsParser<
                            SourceFileAttrParser,
                            SignatureAttrParser>,
                    AttrsParser<
                            CodeAttrParser<
                                    LineNumberTableAttrParser,
                                    LocalVariableTableAttrParser,
                                    LocalVariableTypeTableAttrParser,
                                    StackMapTableAttrParser>,
                            ExceptionsAttrParser,
                            SignatureAttrParser>,
                    AttrsParser<
                            SignatureAttrParser>
            > parser;
            parser.parse(&br, classFile);
        }

    }
}
