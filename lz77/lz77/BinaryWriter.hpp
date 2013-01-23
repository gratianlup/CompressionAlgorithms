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

#ifndef BINARY_WRITER_HPP
#define BINARY_WRITER_HPP

#include <fstream>
#include <assert.h>

class BinaryWriter {
private:
    std::ofstream stream_;
    char data_;
    unsigned count_;
    unsigned written_;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    void WriteData(bool force = false) {
        bool write = (count_ % 8) == 0;

        if(force) {
             write = written_ != count_;
        }

        if(write) {
            stream_.write(&data_, 1);
            data_ = 0;
            written_ = count_;
        }
    }

public:
    BinaryWriter(char* file) : 
            stream_(file, std::ios::out | std::ios::binary), 
            count_(0), data_(0) {
        assert(file != nullptr);
    }

    ~BinaryWriter() {
        Close();
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    void WriteBit(unsigned value) {
        unsigned position = 7 - (count_ % 8);
        data_ = data_ | ((value & 1) << position);
        count_++;
        WriteData();
    }

    void WriteBits(unsigned value, unsigned bits) {
        assert(bits > 0);

        for(unsigned i = bits - 1; i < bits; i--) {
            WriteBit((value & (1 << i)) >> i);
        }
    }

    void WriteByte(unsigned value) {
        if((count_ % 8 == 0)) {
            data_ = (char)value;
            count_ += 8;
            WriteData();
        }
        else WriteBits(value, 8);
    }

    unsigned Count() const {
        return count_;
    }

    void Close() {
        WriteData(true);
        stream_.close();
    }
};

#endif
