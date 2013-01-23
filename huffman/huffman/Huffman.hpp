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

#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include "BinaryReader.hpp"
#include "BinaryWriter.hpp"
#include <cstdlib>
#include <algorithm>
#include <iostream>

struct HuffmanTreeNode {
    HuffmanTreeNode* Left;
    HuffmanTreeNode* Right;
    int Count;
    unsigned char Value;

    HuffmanTreeNode(int count, unsigned char value) :
            Count(count), Value(value), Left(nullptr), Right(nullptr) {}

    HuffmanTreeNode(int count, HuffmanTreeNode* left, HuffmanTreeNode* right) :
            Count(count), Left(left), Right(right) {}
};

struct NodeList {
    HuffmanTreeNode* Node;
    NodeList* Next;

    NodeList(HuffmanTreeNode* node, NodeList* next = nullptr) :
            Node(node), Next(next) {}
};

class Huffman {
private:
    BinaryReader* reader_;
    BinaryWriter* writer_;
    int counts_[256];
    std::pair<unsigned char, int> charToCode_[256];

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    HuffmanTreeNode* BuildTree() {
        NodeList* head = CreateNodeList();
        if(head == nullptr) return nullptr;

        HuffmanTreeNode* treeRoot = nullptr;
        
        while(head->Next) {
            NodeList* left = head;
            NodeList* right = head->Next;
            head = head->Next->Next;

            HuffmanTreeNode* newNode = new HuffmanTreeNode(left->Node->Count +
                                                           right->Node->Count,
                                                           left->Node, right->Node);
            delete left;
            delete right;
            head = InsertNode(new NodeList(newNode), head);
        }

        treeRoot = head->Node;
        delete head;
        return treeRoot;
    }

    NodeList* CreateNodeList() {
        int nodeCount = 0;
        HuffmanTreeNode* nodes[256];

        for(int i = 0; i < 256; i++) {
            if(counts_[i] == 0) continue;

            nodes[nodeCount] = new HuffmanTreeNode(counts_[i], i);
            nodeCount++;
        }

        if(nodeCount == 0) return nullptr;

        std::sort(nodes, nodes + nodeCount, [](HuffmanTreeNode* a, 
                                               HuffmanTreeNode* b) -> bool {
            return a->Count < b->Count;
        });


        NodeList* head = new NodeList(nodes[0]);
        NodeList* current = head;

        for(int i = 1; i < nodeCount; i++) {
            NodeList* node = new NodeList(nodes[i]);
            current->Next = node;
            current = node;
        }

        return head;
    }

    NodeList* InsertNode(NodeList* node, NodeList* head) {
        if(head == nullptr) {
            return node;
        }
        if(node->Node->Count < head->Node->Count) {
            node->Next = head;
            return node;
        }

        NodeList* previous = head;
        NodeList* current = head->Next;

        while(current) {
            if(node->Node->Count < current->Node->Count) {
                break;
            }

            previous = current;
            current = current->Next;
        }

        node->Next = previous->Next;
        previous->Next = node;
        return head;
    }

    void ComputeCharCodes(HuffmanTreeNode* node, int code, int bits) {
        if(node->Left) {
            ComputeCharCodes(node->Left, code << 1, bits + 1);
            ComputeCharCodes(node->Right, (code << 1) | 1, bits + 1);
        }
        else {
            charToCode_[node->Value] = std::make_pair(bits, code);
        }
    }

    void CompressData() {
        while(reader_->IsEOF() == false) {
            int data = reader_->ReadByte();
            writer_->WriteBits(charToCode_[data].second,
                               charToCode_[data].first);
        }
    }

    void ComputeCounts() {
        std::memset(counts_, 0, 256 * sizeof(int));
        
        while(reader_->IsEOF() == false) {
            int ch = reader_->ReadByte();
            counts_[ch]++;
        }
    }

    void DestoyTree(HuffmanTreeNode* node) {
        if (node == nullptr) return;
        DestoyTree(node->Left);
        DestoyTree(node->Right);
        delete node;
    }

    int ReadCounts() {
        int bits[256];

        for(int i = 0; i < 256; i++) {
            int value = reader_->ReadBits(2);

            if(value == 3) bits[i] = 32;
            else bits[i] = value * 8;
        }

        int totalChars = 0;
        std::memset(counts_, 0, 256 * sizeof(int));

        for(int i = 0; i < 256; i++) {
            if(bits[i] > 0) {
                counts_[i] = reader_->ReadBits(bits[i]);
                totalChars += counts_[i];
            }
        }

        return totalChars;
    }

    void DecompressData(HuffmanTreeNode* root, int total) {
        int read = 0;
        HuffmanTreeNode* node = root;

        while(read < total) {
            int bit = reader_->ReadBit();

            if(bit == 0) node = node->Left;
            else node = node->Right;

            if(node->Left == nullptr) {
                writer_->WriteByte(node->Value);
                node = root;
                read++;
            }
        }
    }

    void WriteCounts() {
        int bits[256];

        for(int i = 0; i < 256; i++) {
            int value;

            if(counts_[i] == 0)         { bits[i] = 0;  value = 0; }
            else if(counts_[i] < 256)   { bits[i] = 8;  value = 1; }
            else if(counts_[i] < 65536) { bits[i] = 16; value = 2; }
            else                        { bits[i] = 32; value = 3; }

            writer_->WriteBits(value, 2);
        }

        for(int i = 0; i < 256; i++) {
            if(bits[i] > 0) {
                writer_->WriteBits(counts_[i], bits[i]);
            }
        }
    }

    void WriteHeader() {
        WriteCounts();
    }

    void PrintCodes() {
        for(int i = 0; i < 256; i++) {
            if(counts_[i] > 0) {
                std::cout<<i<<" bits: "<<(int)charToCode_[i].first<<
                              " code: "<<charToCode_[i].second<<"\n";
            }
        }
    }

    void PrintCounts() {
        for(int i = 0; i < 256; i++) {
            std::cout<<i<<": "<<counts_[i]<<"\n";
        }
    }

public:
    void Compress(char* source, char* dest) {
        assert(source != nullptr);
        assert(dest != nullptr);

        reader_ = new BinaryReader(source);
        ComputeCounts();
        //PrintCounts();
        delete reader_;

        HuffmanTreeNode* root = BuildTree();
        if(root == nullptr) return;

        ComputeCharCodes(root, 0, 0);
        //PrintCodes();

        reader_ = new BinaryReader(source);
        writer_ = new BinaryWriter(dest);
        WriteHeader();
        CompressData();

        DestoyTree(root);
        delete reader_;
        delete writer_;      
    }

    void Decompress(char* source, char* dest) {
        assert(source != nullptr);
        assert(dest != nullptr);

        reader_ = new BinaryReader(source);
        int totalChars = ReadCounts();
        //PrintCounts();

        HuffmanTreeNode* root = BuildTree();
        if(root == nullptr) return;

        writer_ = new BinaryWriter(dest);
        DecompressData(root, totalChars);

        DestoyTree(root);
        delete reader_;
        delete writer_;
    }
};

#endif
