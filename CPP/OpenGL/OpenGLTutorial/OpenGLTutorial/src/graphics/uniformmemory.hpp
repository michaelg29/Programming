#ifndef UNIFORMMEMORY_HPP
#define UNIFORMMEMORY_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "vertexmemory.hpp"

#define N 4

namespace UBO {
    enum class Type : unsigned char {
        BOOL = 0,
        SCALAR = 1,
        VEC2 = 2,
        VEC3 = 3,
        VEC4 = 4,
        ARRAY = 5,
        STRUCT = 6,
        INVALID = 7
    };

    // type consts
    const Type BOOL = Type::BOOL;
    const Type SCALAR = Type::SCALAR;
    const Type VEC2 = Type::VEC2;
    const Type VEC3 = Type::VEC3;
    const Type VEC4 = Type::VEC4;
    const Type ARRAY = Type::ARRAY;
    const Type STRUCT = Type::STRUCT;
    const Type INVALID = Type::INVALID;

    // round val up to next multiple of 2^n
    /*unsigned int roundUpPow2(unsigned int val, unsigned char n) {
        unsigned int pow2n = 0b1 << n;
        unsigned int divisor = pow2n - 1; // = 0b0111...111 (n 1s)

        // last n bits = remainder of val / 2^n
        // add (2^n - rem) to get to the next multiple of 2^n
        unsigned int rem = val & divisor;
        if (rem) {
            val += pow2n - rem;
        }

        return val;
    }*/

    typedef struct Element {
    public:
        Type type;
        unsigned char length;
        unsigned int baseAlign;
        std::vector<Element> list; // for struct (list of elements), or array (first element is type)

        /*unsigned int alignPow2() {
            switch (baseAlign) {
            case 2: return 1;
            case 4: return 2;
            case 8: return 3;
            case 16: return 4;
            default: return 0;
            };
        }

        GLuint calcSize() {
            switch (type) {
            case Type::ARRAY:
            case Type::STRUCT:
                return calcPaddedSize();
            case Type::BOOL:
                return 1;
            case Type::SCALAR:
                return N;
            case Type::VEC2:
                return 2 * N;
            case Type::VEC3:
                return 3 * N;
            case Type::VEC4:
                return 4 * N;
            default:
                return 0;
            };
        }

        GLuint calcPaddedSize() {
            unsigned int offset = 0;

            switch (type) {
            case Type::ARRAY:
                return length * roundUpPow2(list[0].calcPaddedSize(), alignPow2());
            case Type::STRUCT:
                for (Element e : list) {
                    offset = roundUpPow2(offset, e.alignPow2());
                    offset += e.calcSize();
                }
                return offset;
            case Type::BOOL:
            case Type::SCALAR:
                return N;
            case Type::VEC2:
                return 2 * N;
            case Type::VEC3:
                return 3 * N;
            case Type::VEC4:
                return 4 * N;
            default:
                return baseAlign;
            };
        }*/

        std::string typeStr() {
            switch (type) {
            case BOOL: return "bool";
            case SCALAR: return "scalar";
            case VEC2: return "vec2";
            case VEC3: return "vec3";
            case VEC4: return "vec4";
            case ARRAY: return "array<" + list[0].typeStr() + ">";
            case STRUCT: return "struct";
            default: return "invalid";
            }
        }

        Element(Type type = Type::SCALAR)
            : type(type), length(0), list(0), baseAlign(0) {

            /*switch (type) {
            case Type::SCALAR:
                baseAlign = N; break;
            case Type::VEC2:
                baseAlign = 2 * N; break;
            case Type::VEC3:
            case Type::VEC4:
                baseAlign = 4 * N; break;
            default:
                baseAlign = 0; break;
            };*/
        }
    } Element;

    Element newScalar() {
        return Element(Type::SCALAR);
    }

    Element newVec(unsigned char dim) {
        switch (dim) {
        case 2: return VEC2;
        case 3: return VEC3;
        case 4:
        default:
            return VEC4;
        };
    }

    Element newArray(unsigned char length, Element arrElement) {
        Element ret(Type::ARRAY);
        ret.length = length;
        ret.list = std::vector<Element>(1);
        ret.list[0] = arrElement;
        if (arrElement.type == Type::STRUCT) {
            //ret.baseAlign = arrElement.baseAlign;
        }
        else {
            //ret.baseAlign = roundUpPow2(arrElement.baseAlign, 4);
        }

        return ret;
    }

    Element newColMat(unsigned char cols, unsigned char rows) {
        return newArray(cols, newVec(rows));
    }

    Element newColMatArray(unsigned char noMatrices, unsigned char cols, unsigned char rows) {
        return newArray(noMatrices * cols, newVec(rows));
    }

    Element newRowMat(unsigned char rows, unsigned char cols) {
        return newArray(rows, newVec(cols));
    }

    Element newRowMatArray(unsigned char noMatrices, unsigned char rows, unsigned char cols) {
        return newArray(noMatrices * rows, newVec(cols));
    }

    Element newStruct(std::vector<Element> elements) {
        Element ret(Type::STRUCT);
        ret.list.insert(ret.list.end(), elements.begin(), elements.end());
        ret.length = ret.list.size();

        // base alignment is the largest base alignment
        /*if (elements.size()) {
            for (Element e : elements) {
                if (e.baseAlign > ret.baseAlign) {
                    ret.baseAlign = e.baseAlign;
                }
            }
        }

        ret.baseAlign = roundUpPow2(ret.baseAlign, 4);*/

        return ret;
    }

    class UBO : public BufferObject {
    public:
        Element block;
        //unsigned int calculatedSize;

        UBO()
            : BufferObject(GL_UNIFORM_BUFFER), block(newStruct({}))/*, calculatedSize(0)*/ {}

        void addElement(Element element) {
            block.list.push_back(element);
            if (element.baseAlign > block.baseAlign) {
                block.baseAlign = element.baseAlign;
            }
            block.length++;
        }

        /*void initNullData() {
            if (!calculatedSize) {
                calculatedSize = calcSize();
            }
            glBufferData(type, calculatedSize, NULL, GL_DYNAMIC_DRAW);
        }

        void bindRange(GLuint index, GLuint offset = 0) {
            if (!calculatedSize) {
                calculatedSize = calcSize();
            }
            glBindBufferRange(type, index, val, offset, calculatedSize);
        }

        GLuint calcSize() {
            return block.calcPaddedSize();
        }*/

        //GLuint offset;
        std::vector<std::pair<unsigned short, Element*>> indexStack; // stack to keep track of nested indices
        int currentDepth;

        void startWrite() {
            //offset = 0;
            currentDepth = 0;
            indexStack.clear();
            indexStack.push_back({ 0, &block });
        }

        Element getCurrentElement() {
            // highest level struct popped
            if (currentDepth < 0) {
                return INVALID;
            }

            // get current deepest array/struct
            Element *currentElement = indexStack[currentDepth].second;
            if (currentElement->type == STRUCT) {
                currentElement = &currentElement->list[indexStack[currentDepth].first];
            }
            else {
                currentElement = &currentElement->list[0];
            }

            // traverse down to deepest array/struct
            if (currentElement->type == STRUCT || currentElement->type == ARRAY) {
                do {
                    currentDepth++;
                    indexStack.push_back({ 0, currentElement });
                    currentElement = &currentElement->list[0];
                } while (currentElement->type == STRUCT || currentElement->type == ARRAY);
            }
            
            // pop from stack if necessary
            for (int i = currentDepth; i >= 0; i--) {
                int advancedIdx = ++indexStack[i].first;
                if (advancedIdx >= indexStack[i].second->length) {
                    // iterated through entire list or stack
                    // pop
                    indexStack.erase(indexStack.begin() + i);
                    currentDepth--;
                }
                else {
                    break;
                }
            }

            return *currentElement;
        }

        /*template<typename T>
        void writeArray(T* first, unsigned int n) {
            //if (type == ARRAY) {
                for (int i = 0; i < n; i++) {
                    //offset = roundUpPow2(offset, )
                }
            //}
        }

        template<typename T>
        void writeElement(T* element) {
            Element currentElement = block.list[indexStack[0]];

            unsigned int nestedDepth = indexStack.size() - 1;
            unsigned int listSize = block.list.size();
            for (int i = 1; i <= nestedDepth; i++) {
                listSize = currentElement.list.size();
                currentElement = currentElement.list[indexStack[i]];
            }

            //indexStack[nestedDepth]++;
        }*/

        /*
            switch (type) {
            case Type::ARRAY:
                return length * roundUpPow2(list[0].calcSize(), alignPow2());
            case Type::STRUCT:
                for (Element e : list) {
                    offset = roundUpPow2(offset, e.alignPow2());
                    offset += e.calcSize();
                }
                return offset;
            case Type::SCALAR:
                return N;
            case Type::VEC2:
                return 2 * N;
            case Type::VEC3:
                return 3 * N;
            case Type::VEC4:
                return 4 * N;
            default:
                return baseAlign;
            };
        */
    };
};

#endif