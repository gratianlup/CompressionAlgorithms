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

#ifndef BINARY_READER_HPP
#define BINARY_READER_HPP

#include <fstream>
#include <assert.h>

class BinaryReader {
private:
    std::ifstream stream_;
    char data_;
    unsigned available_;
    unsigned count_;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    void FetchData() {
        if(count_ == available_) {
            stream_.read(&data_, 1);
            available_ += 8;
        }
    }

public:
    BinaryReader(char* file) : 
            stream_(file, std::ios::in | std::ios::binary), count_(0), 
            available_(0), data_(0) {
        assert(file != nullptr);
    }

    ~BinaryReader() {
        stream_.close();
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    bool IsEOF() {
        return (count_ >= available_) && (stream_.peek(), stream_.eof());
    }

    unsigned ReadBit() {
        assert(IsEOF() == false);

        FetchData();
        unsigned position = 7 - (count_ % 8);
        unsigned bit = (data_ & (1 << position)) >> position;
        count_++;
        return bit;
    }

    unsigned ReadBits(int bits) {
        assert(bits > 0);
        unsigned data = 0;
        FetchData();
       
        if((bits > 1) && ((available_ - count_) > 1)) {
            int remaining = available_ - count_;
            int together = std::min(bits, remaining);
            int position = 8 - (count_ % 8) - together;
            data = (data_ & (((1 << together) - 1) << position)) >> position;
            count_ += together;
            bits -= together;
        }

        for(int i = 0; i < bits; i++) {
            data = (data << 1) | ReadBit();
        }

        return data;
    }

    unsigned ReadByte() {
        return ReadBits(8);
    }

    unsigned Count() const {
        return count_;
    }

    void Close() {
        stream_.close();
    }
};

#endif
