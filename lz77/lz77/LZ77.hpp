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

#ifndef LZ77_HPP
#define LZ77_HPP

#include "BinaryReader.hpp"
#include "BinaryWriter.hpp"
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <string>

class SlidingWindow {
private:
    BinaryReader* reader_;
    unsigned char* buffer_;
    unsigned bufferSize_;
    unsigned windowSize_;
    unsigned position_;
    unsigned available_;
    unsigned processed_;
    unsigned maxLength_;
    unsigned maxOffset_;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    void AllocateBuffer() {
        buffer_ = new unsigned char[bufferSize_];
    }

    void DeallocateBuffer() {
        delete[] buffer_;
        buffer_ = nullptr;
    }

    unsigned BufferOffset(unsigned value) {
        return value % bufferSize_;
    }

    void ReadData() {
        if(reader_) {
            while(((available_ - position_) < windowSize_) &&
                  (reader_->IsEOF() == false)) {
                buffer_[BufferOffset(available_)] = reader_->ReadByte();
                available_++;
            }
        }
    }

public:
    SlidingWindow(BinaryReader* reader, unsigned bufferSize,
                  unsigned maxOffset, unsigned maxLength) : 
            reader_(reader), bufferSize_(bufferSize), windowSize_(2 * maxOffset),
            maxLength_(maxLength), maxOffset_(maxOffset) {
        AllocateBuffer();
    }

    ~SlidingWindow() {
        DeallocateBuffer();
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    void Initialize() {
        position_ = 0;
        processed_ = 0;
        available_ = 0;
        ReadData();
    }

    bool FindLongestMatch(unsigned& offset, unsigned& length) {
        unsigned maxOffset = std::min(maxOffset_, processed_);
        unsigned maxLength = std::min(maxLength_, available_ - position_);
        unsigned maxMatchLength = 0;
        unsigned maxMatchOffset = 0;

        for(int currentOffset = 0; currentOffset < maxOffset; currentOffset++) {
            int length = 0;

            for(; length < maxLength; length++) {
                if(buffer_[BufferOffset(position_ - currentOffset + length - 1)] !=
                   buffer_[BufferOffset(position_ + length)]) {
                    break;
                }
            }

            if(length > maxMatchLength) {
                maxMatchLength = length;
                maxMatchOffset = currentOffset;
            }
        }

        offset = maxMatchOffset;
        length = maxMatchLength;
        return maxMatchLength != 0;
    }

    unsigned char CurrentByte(int offset = 0) {
        return buffer_[BufferOffset(position_ + offset)];
    }

    void AdvanceWindow(unsigned offset) {
        position_ += offset;
        processed_ += offset;
        ReadData();
    }

    bool IsEOF() {
        return position_ >= available_;
    }

    bool IsOutside() {
        return position_ > available_;
    }

    void WriteByte(int value) {
        buffer_[BufferOffset(position_)] = value;
    }
};


class LZ77 {
private:
    static const int MIN_OFFSET_BITS = 3;
    static const int MIN_LENGTH_BITS = 2;
    static const int HEADER_OFFSET_BITS = 4;
    static const int HEADER_LENGTH_BITS = 2;

    unsigned offsetBits_;
    unsigned lengthBits_;
    unsigned bufferSize_;
    bool logTokens_;
    std::string tokenLog_;

    int MaxOffset() {
        return (1 << offsetBits_) - 1;
    }

    int MaxLength() {
        return (1 << lengthBits_) - 1;
    }

    void WriteHeader(BinaryWriter& writer) {
        writer.WriteBits(offsetBits_ - MIN_OFFSET_BITS, HEADER_OFFSET_BITS);
        writer.WriteBits(lengthBits_ - MIN_LENGTH_BITS, HEADER_LENGTH_BITS);
    }

    void ReadHeader(BinaryReader& reader) {
        offsetBits_ = reader.ReadBits(HEADER_OFFSET_BITS) + MIN_OFFSET_BITS;
        lengthBits_ = reader.ReadBits(HEADER_LENGTH_BITS) + MIN_LENGTH_BITS;
    }

public:
    LZ77(unsigned bufferSize, bool logTokens) : 
            bufferSize_(bufferSize), logTokens_(logTokens) {}

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    void Compress(char* source, char* dest, unsigned offsetBits, unsigned lengthBits) {
        assert(source != nullptr);
        assert(dest != nullptr);
        offsetBits_ = offsetBits;
        lengthBits_ = lengthBits;

        BinaryReader reader(source);
        SlidingWindow window(&reader, bufferSize_, MaxOffset(), MaxLength() - 1);
        window.Initialize();

        BinaryWriter writer(dest);
        WriteHeader(writer);

        while(window.IsEOF() == false) {
            unsigned length;
            unsigned offset;

            if(window.FindLongestMatch(offset, length)) {
                unsigned char current;
                window.AdvanceWindow(length + 1);

                if(window.IsOutside()) {
                    current = window.CurrentByte(-2);
                    length--;
                }
                else current = window.CurrentByte(-1);

                if(length > 0) {
                    writer.WriteBits(length, lengthBits_);
                    writer.WriteBits(offset, offsetBits_);
                    writer.WriteByte(current);

                    if(logTokens_) {
                        char buffer[1024];
                        std::sprintf(buffer, "o: %d, l: %d, c: %d          ", offset, length, current);
                        tokenLog_ += buffer;
                    }
                }
                else {
                    writer.WriteBits(0, lengthBits_);
                    writer.WriteByte(current);

                    if(logTokens_) {
                        char buffer[1024];
                        std::sprintf(buffer, "o: %d, l: %d, c: %d          ", 0, 0, current);
                        tokenLog_ += buffer;
                    }
                }
            }
            else {
                writer.WriteBits(0, lengthBits_);
                writer.WriteByte(window.CurrentByte());

                if(logTokens_) {
                    char buffer[1024];
                    std::sprintf(buffer, "o: %d, l: %d, c: %d         ", 0, 0, window.CurrentByte());
                    tokenLog_ += buffer;
                }

                window.AdvanceWindow(1);
            }
        }

        writer.WriteBits(MaxLength(), lengthBits_);
    }

    void Decompress(char* source, char* dest) {
        BinaryReader reader(source);
        ReadHeader(reader);

        BinaryWriter writer(dest);
        SlidingWindow window(nullptr, bufferSize_, MaxOffset(), MaxLength() - 1);
        window.Initialize();

        while(reader.IsEOF() == false) {
            int length = reader.ReadBits(lengthBits_);
            int value;

            if(length == MaxLength()) {
                break;
            }
            if(length == 0) {
                value = reader.ReadByte();
            }
            else {
                int offset = reader.ReadBits(offsetBits_);
                value = reader.ReadByte();

                for(int i = 0; i < length; i++) {
                    int temp = window.CurrentByte(-offset - 1);
                    writer.WriteByte(temp);
                    window.WriteByte(temp);
                    window.AdvanceWindow(1);
                }
            }

            writer.WriteByte(value);
            window.WriteByte(value);
            window.AdvanceWindow(1);
        }
    }

    std::string& TokenLog() {
        return tokenLog_;
    }
};

#endif
