// Copyright (c) Gratian Lup. All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following
// disclaimer in the documentation and/or other materials provided
// with the distribution.
//
// * The name "CompressionAlgorithms" must not be used to endorse or promote
// products derived from this software without prior written permission.
//
// * Products derived from this software may not be called "CompressionAlgorithms" nor
// may "CompressionAlgorithms" appear in their names without prior written
// permission of the author.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef LZW_HPP
#define LZW_HPP

#include "BinaryReader.hpp"
#include "BinaryWriter.hpp"
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <fstream>

struct TrieNode;

struct TrieList {
    TrieNode* Node;
    TrieList* Next;
    unsigned char Value;

    TrieList() {}

    TrieList(TrieNode* node, unsigned char value, TrieList* next = nullptr) :
            Node(node), Value(value), Next(next) {}
};

struct TrieNode {
    static const int MAX_LIST_ELEMENTS = 16;

    union {
        TrieList* ChildrenListHead;
        TrieNode** ChildrenArray;
    };
    
    TrieNode* Parent;
    unsigned Index;
    unsigned short ChildCount;
    unsigned char Value;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    TrieNode** AllocateArray() {
        TrieNode** array = new TrieNode*[256];
        std::memset(array, 0, 256 * sizeof(TrieNode*));
        return array;
    }

    void DeallocateArray() {
        for(int i = 0; i < 256; i++) {
            if(ChildrenArray[i]) {
                delete ChildrenArray[i];
            }
        }

        delete[] ChildrenArray;
    }

    void DeallocateList(TrieList* node, bool deallocateNodes) {
        if(node->Next) {
            DeallocateList(node->Next, deallocateNodes);
        }

        if(deallocateNodes) {
            delete node->Node;
        }

        delete node;
    }

    void ConvertListToArray() {
        assert(ChildrenListHead != nullptr);

        TrieNode** array = AllocateArray();
        TrieList* listNode = ChildrenListHead;

        while(listNode) {
            array[listNode->Value] = listNode->Node;
            listNode = listNode->Next;
        }

        DeallocateList(ChildrenListHead, false);
        ChildrenArray = array;
    }

public:
    TrieNode(TrieNode* parent, unsigned char value = 0, unsigned index = -1) : 
            Parent(parent), Value(value), ChildrenListHead(nullptr), 
            ChildCount(0), Index(index) {}

    ~TrieNode() {
        if(ChildCount == 0) return;
        else if(ChildCount > MAX_LIST_ELEMENTS) {
            DeallocateArray();
        }
        else DeallocateList(ChildrenListHead, true);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    TrieNode* GetNodeForValue(unsigned char value) {
        if(ChildCount > MAX_LIST_ELEMENTS) {
            return ChildrenArray[value];
        }
        else {
            TrieList* listNode = ChildrenListHead;

            while(listNode) {
                if(listNode->Value == value) {
                    return listNode->Node;
                }
                else listNode = listNode->Next;
            }
        }

        return nullptr;
    }

    TrieNode* CreateNodeForValue(unsigned char value, int index) {
        assert(GetNodeForValue(value) == nullptr);

        TrieNode* node = new TrieNode(this, value, index);

        if(ChildCount == MAX_LIST_ELEMENTS) {
            ConvertListToArray();
        }

        if(ChildCount >= MAX_LIST_ELEMENTS) {
            ChildrenArray[value] = node;
        }
        else {
            TrieList* listNode = ChildrenListHead;
            TrieList* lastNode = nullptr;

            while(listNode) {
                lastNode = listNode;
                listNode = listNode->Next;
            }

            TrieList* newNode = new TrieList(node, value);
            if(lastNode) lastNode->Next = newNode;
            else ChildrenListHead = newNode;
        }

        ChildCount++;
        return node;
    }
};


class LZW {
private:
    static const int MIN_DICT_SIZE = 9;
    static const int MIN_DICT_SIZE_BITS = 3;

    TrieNode* rootNode_;
    TrieNode** nodes_;
    int dictSize_;
    int maxDictSize_;
    int indexBits_;
    bool freeze_;

    void AllocateDictionary() {
        rootNode_ = new TrieNode(nullptr);
        nodes_ = new TrieNode*[maxDictSize_];
        dictSize_ = 256;

        for(int i = 0; i < 256; i++) {
            TrieNode* node = rootNode_->CreateNodeForValue(i, i);
            nodes_[i] = node;
        }
    }

    void DeallocateDictionary() {
        delete rootNode_;
        delete[] nodes_;
        dictSize_ = 0;
    }

    unsigned IndexBits() {
        return indexBits_;
    }

    void InsertValue(unsigned char value, TrieNode* parentNode) {
        if(dictSize_ == (maxDictSize_ - 1)) {
            if(freeze_ == false) {
                DeallocateDictionary();
                AllocateDictionary();
            }
        }
        else {
            TrieNode* node = parentNode->CreateNodeForValue(value, dictSize_);
            nodes_[dictSize_] = node;
            dictSize_++;
        }
    }

    void WriteHeader(BinaryWriter& writer) {
        writer.WriteBits(indexBits_ - MIN_DICT_SIZE, MIN_DICT_SIZE_BITS);
        writer.WriteBit(freeze_);
    }

    void ReadHeader(BinaryReader& reader) {
        indexBits_ = MIN_DICT_SIZE + reader.ReadBits(MIN_DICT_SIZE_BITS);
        freeze_ = reader.ReadBit();
    }

    unsigned char WriteString(TrieNode* node, BinaryWriter& writer) {
        if(node->Parent == rootNode_) {
            writer.WriteByte(node->Value);
            return node->Value;
        }
        else {
            unsigned char first = WriteString(node->Parent, writer);
            writer.WriteByte(node->Value);
            return first;
        }
    }

public:
    LZW() : rootNode_(nullptr), nodes_(nullptr) {}

    ~LZW() {
        DeallocateDictionary();
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    void Compress(char* source, char* dest, unsigned indexBits, bool freeze) {
        assert(source != nullptr);
        assert(dest != nullptr);

        indexBits_ = indexBits;
        freeze_ = freeze;
        maxDictSize_ = 1 << indexBits_;
        AllocateDictionary();
        
        BinaryReader reader(source);
        BinaryWriter writer(dest);

        WriteHeader(writer);
        TrieNode* previousNode = rootNode_;

        while(true) {
            bool stop = false;
            unsigned char value;
            TrieNode* currentNode = nullptr;

            if(reader.IsEOF() == false) {
                value = reader.ReadByte();
                currentNode = previousNode->GetNodeForValue(value);
            }
            else stop = true;

            if((currentNode == nullptr) || 
               (stop && (previousNode != rootNode_))) {
                TrieNode* lastNode = currentNode ? currentNode : previousNode;
                writer.WriteBits(lastNode->Index, IndexBits());
                
                if(stop == false) {
                    InsertValue(value, previousNode);
                    previousNode = rootNode_->GetNodeForValue(value);
                }
            }
            else previousNode = currentNode;

            if(stop) break;
        }

        writer.WriteBits(maxDictSize_ - 1, IndexBits());
    }

    void Decompress(char* source, char* dest) {
        assert(source != nullptr);
        assert(dest != nullptr);

        BinaryReader reader(source);
        BinaryWriter writer(dest);
        
        ReadHeader(reader);
        maxDictSize_ = 1 << indexBits_;
        AllocateDictionary();
        
        TrieNode* previousNode = nullptr;
        unsigned char firstChar;
        unsigned char lastChar;

        while(reader.IsEOF() == false) {
            int index = reader.ReadBits(IndexBits());
            if(index == maxDictSize_ -1) break;
            
            if(index >= dictSize_) {
                firstChar = WriteString(previousNode, writer);
                writer.WriteByte(lastChar);
            }
            else {
                firstChar = WriteString(nodes_[index], writer);
            }
            
            if(previousNode) {
                InsertValue(firstChar, previousNode);
            }

            lastChar = firstChar;
            previousNode = nodes_[index];
        }
    }
};

#endif
