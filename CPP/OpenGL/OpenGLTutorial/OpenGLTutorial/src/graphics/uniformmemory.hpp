#ifndef UNIFORMMEMORY_HPP
#define UNIFORMMEMORY_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "vertexmemory.hpp"
#include "shader.h"

#define N 4

namespace UBO
{
    enum class Type : unsigned char
    {
        SCALAR = 0,
        VEC2 = 1,
        VEC3 = 2,
        VEC4 = 3,
        ARRAY = 4,
        STRUCT = 5,
        INVALID = 6
    };

    // round val up to next multiple of 2^n
    unsigned int roundUpPow2(unsigned int val, unsigned char n)
    {
        unsigned int pow2n = 0b1 << n;
        unsigned int divisor = pow2n - 1; // = 0b0111...111 (n 1s)

        // last n bits = remainder of val / 2^n
        // add (2^n - rem) to get to the next multiple of 2^n
        unsigned int rem = val & divisor;
        if (rem)
        {
            val += pow2n - rem;
        }

        return val;
    }

    typedef struct Element
    {
    public:
        Type type;
        unsigned char length;
        unsigned int baseAlign;
        std::vector<Element> list; // for struct (list of elements), or array (first element is type)

        unsigned int alignPow2()
        {
            switch (baseAlign)
            {
            case 2:
                return 1;
            case 4:
                return 2;
            case 8:
                return 3;
            case 16:
                return 4;
            default:
                return 0;
            };
        }

        GLuint calcSize()
        {
            switch (type)
            {
            case Type::ARRAY:
            case Type::STRUCT:
                return calcPaddedSize();
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

        GLuint calcPaddedSize()
        {
            unsigned int offset = 0;

            switch (type)
            {
            case Type::ARRAY:
                return length * roundUpPow2(list[0].calcPaddedSize(), alignPow2());
            case Type::STRUCT:
                for (Element e : list)
                {
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
        }

        std::string typeStr()
        {
            switch (type)
            {
            case Type::SCALAR:
                return "scalar";
            case Type::VEC2:
                return "vec2";
            case Type::VEC3:
                return "vec3";
            case Type::VEC4:
                return "vec4";
            case Type::ARRAY:
                return "array<" + list[0].typeStr() + ">";
            case Type::STRUCT:
                return "struct";
            default:
                return "invalid";
            }
        }

        Element(Type type = Type::SCALAR)
            : type(type), length(0), list(0), baseAlign(0)
        {

            switch (type)
            {
            case Type::SCALAR:
                baseAlign = N;
                break;
            case Type::VEC2:
                baseAlign = 2 * N;
                break;
            case Type::VEC3:
            case Type::VEC4:
                baseAlign = 4 * N;
                break;
            default:
                baseAlign = 0;
                break;
            }
        }
    } Element;

    Element newScalar()
    {
        return Element(Type::SCALAR);
    }

    Element newVec(unsigned char dim)
    {
        switch (dim)
        {
        case 2:
            return Type::VEC2;
        case 3:
            return Type::VEC3;
        case 4:
        default:
            return Type::VEC4;
        };
    }

    Element newArray(unsigned char length, Element arrElement)
    {
        Element ret(Type::ARRAY);
        ret.length = length;
        ret.list = std::vector<Element>(1);
        ret.list[0] = arrElement;
        if (arrElement.type == Type::STRUCT)
        {
            ret.baseAlign = arrElement.baseAlign;
        }
        else
        {
            ret.baseAlign = roundUpPow2(arrElement.baseAlign, 4);
        }

        return ret;
    }

    Element newColMat(unsigned char cols, unsigned char rows)
    {
        return newArray(cols, newVec(rows));
    }

    Element newColMatArray(unsigned char noMatrices, unsigned char cols, unsigned char rows)
    {
        return newArray(noMatrices * cols, newVec(rows));
    }

    Element newRowMat(unsigned char rows, unsigned char cols)
    {
        return newArray(rows, newVec(cols));
    }

    Element newRowMatArray(unsigned char noMatrices, unsigned char rows, unsigned char cols)
    {
        return newArray(noMatrices * rows, newVec(cols));
    }

    Element newStruct(std::vector<Element> elements)
    {
        Element ret(Type::STRUCT);
        ret.list.insert(ret.list.end(), elements.begin(), elements.end());
        ret.length = ret.list.size();

        // base alignment is the largest base alignment
        if (elements.size())
        {
            for (Element e : elements)
            {
                if (e.baseAlign > ret.baseAlign)
                {
                    ret.baseAlign = e.baseAlign;
                }
            }
        }

        ret.baseAlign = roundUpPow2(ret.baseAlign, 4);

        return ret;
    }

    class UBO : public BufferObject
    {
    public:
        Element block;
        GLuint bindingPos;
        unsigned int calculatedSize;

        UBO(GLuint bindingPos)
            : BufferObject(GL_UNIFORM_BUFFER), block(newStruct({})), calculatedSize(0), bindingPos(bindingPos) {}

        void attachToShader(Shader shader, std::string name)
        {
            GLuint blockIdx = glGetUniformBlockIndex(shader.id, name.c_str());
            glUniformBlockBinding(shader.id, blockIdx, bindingPos);
        }

        void initNullData(GLenum usage)
        {
            if (!calculatedSize)
            {
                calculatedSize = calcSize();
            }
            glBufferData(type, calculatedSize, NULL, usage);
        }

        void bindRange(GLuint index, GLuint offset = 0)
        {
            if (!calculatedSize)
            {
                calculatedSize = calcSize();
            }
            glBindBufferRange(type, index, val, offset, calculatedSize);
        }

        GLuint calcSize()
        {
            return block.calcPaddedSize();
        }

        void addElement(Element element)
        {
            block.list.push_back(element);
            if (element.baseAlign > block.baseAlign)
            {
                block.baseAlign = element.baseAlign;
            }
            block.length++;
        }

        GLuint offset;
        GLuint poppedOffset;
        std::vector<std::pair<unsigned short, Element *>> indexStack; // stack to keep track of nested indices
        int currentDepth;

        void startWrite()
        {
            offset = 0;
            poppedOffset = 0;
            currentDepth = 0;
            indexStack.clear();
            indexStack.push_back({0, &block});
        }

        Element getNextElement()
        {
            // highest level struct popped
            if (currentDepth < 0)
            {
                return Type::INVALID;
            }

            // get current deepest array/struct (last element in the stack)
            Element *currentElement = indexStack[currentDepth].second;

            // get the element at the specified index within that iterable
            if (currentElement->type == Type::STRUCT)
            {
                currentElement = &currentElement->list[indexStack[currentDepth].first];
            }
            else
            { // array
                currentElement = &currentElement->list[0];
            }

            // traverse down to deepest array/struct
            while (currentElement->type == Type::STRUCT || currentElement->type == Type::ARRAY)
            {
                currentDepth++;
                indexStack.push_back({0, currentElement});
                currentElement = &currentElement->list[0];
            }

            // pop from stack if necessary
            poppedOffset = roundUpPow2(offset, currentElement->alignPow2()) + currentElement->calcSize();
            if (!pop())
            {
                // no items popped
                poppedOffset = 0;
            }

            return *currentElement;
        }

        bool pop()
        {
            bool popped = false;

            for (int i = currentDepth; i >= 0; i--)
            {
                int advancedIdx = ++indexStack[i].first;
                if (advancedIdx >= indexStack[i].second->length)
                {
                    // iterated through entire list or stack
                    // pop
                    poppedOffset = roundUpPow2(poppedOffset, indexStack[i].second->alignPow2());
                    indexStack.erase(indexStack.begin() + i);
                    currentDepth--;
                    popped = true;
                }
                else
                {
                    break;
                }
            }

            return popped;
        }

        template <typename T>
        void writeElement(T *data)
        {
            Element element = getNextElement();
            offset = roundUpPow2(offset, element.alignPow2());

            glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(T), data);

            if (poppedOffset)
            {
                offset = poppedOffset;
            }
            else
            {
                offset += element.calcSize();
            }
        }

        template <typename T>
        void writeArray(T *first, unsigned int n)
        {
            for (int i = 0; i < n; i++)
            {
                writeElement<T>(&first[i]);
            }
        }

        template <typename T, typename V>
        void writeArrayContainer(T *container, unsigned int n)
        {
            for (int i = 0; i < n; i++)
            {
                writeElement<V>(&container->operator[](i));
            }
        }

        void advanceCursor(unsigned int n)
        {
            // write number of elements
            for (int i = 0; i < n; i++)
            {
                Element element = getNextElement();
                offset = roundUpPow2(offset, element.alignPow2());
                if (poppedOffset)
                {
                    offset = poppedOffset;
                }
                else
                {
                    offset += element.calcSize();
                }
            }
        }

        void advanceArray(unsigned int noElements)
        {
            if (currentDepth < 0)
            {
                return;
            }

            Element *currentElement = indexStack[currentDepth].second;

            // get the element at the specified index within that iterable
            if (currentElement->type == Type::STRUCT)
            {
                currentElement = &currentElement->list[indexStack[currentDepth].first];

                unsigned int depthAddition = 0;
                std::vector<std::pair<unsigned short, Element *>> stackAddition;

                // go to next array
                while (currentElement->type == Type::STRUCT)
                {
                    depthAddition++;
                    stackAddition.push_back({0, currentElement});
                    currentElement = &currentElement->list[0];
                }

                if (currentElement->type != Type::ARRAY)
                {
                    // did not find an array
                    return;
                }

                // found array, apply changes to global stack
                currentDepth += depthAddition + 1; // + 1 for next array
                indexStack.insert(indexStack.end(), stackAddition.begin(), stackAddition.end());
                indexStack.push_back({0, currentElement}); // add array
            }

            // at an array, advance number of elements
            unsigned int finalIdx = indexStack[currentDepth].first + noElements;
            unsigned int advanceCount = noElements;
            if (finalIdx >= indexStack[currentDepth].second->length)
            {
                // advance to end of array
                advanceCount = indexStack[currentDepth].second->length - indexStack[currentDepth].first;
            }

            offset += advanceCount * roundUpPow2(currentElement->list[0].calcPaddedSize(), currentElement->alignPow2());
            // pop from stack if necessary
            poppedOffset = offset;
            if (!pop())
            {
                // no items popped
                offset = poppedOffset;
            }
        }
    };
}; // namespace UBO

#endif