#pragma once

#include <cassert>
#include <fstream>
#include <istream>
#include <vector>

class MDBIstream {
public:
    virtual ~MDBIstream() = default;

    // try to read n bytes into buf, returns how many bytes were read
    virtual size_t read(char* buf, size_t n) = 0;

    virtual bool error() = 0;
};

class MDBIstreamWrapper : public MDBIstream {
public:
    MDBIstreamWrapper(std::istream& in) :
        in(in)
    { }

    // try to read n bytes into buf, returns how many bytes were read
    virtual size_t read(char* buf, size_t n) override
    {
        in.read(buf, n);

        if (in)
            return n;
        else
            return in.gcount();
    }

    bool error() override {
        return in.bad();
    }

    std::istream& in;
};

class MDBIstreamFiles : public MDBIstream {
public:
    MDBIstreamFiles(std::vector<std::string>& filenames) :
        filenames(filenames),
        filename_index(0)
    {
        assert(filenames.size() > 0);
        current_in.open(filenames[0], std::ios_base::in | std::ios_base::binary);
    }

    bool next_file()
    {
        current_in.close();
        filename_index++;
        if (filename_index < filenames.size()) {
            current_in.open(filenames[filename_index], std::ios_base::in | std::ios_base::binary);
            return true;
        }
        return false;
    }

    // try to read n bytes into buf, returns how many bytes were read
    virtual size_t read(char* buf, size_t n) override
    {
        auto remaining = n;

        do {
            current_in.read(buf, remaining);
            if (current_in) {
                return n;
            } else {
                auto bytes_read = current_in.gcount();
                buf += bytes_read;
                remaining -= bytes_read;
                if (!next_file()) {
                    return n - remaining;
                }
            }
        } while (true);
    }

    bool error() override {
        return current_in.bad();
    }

    std::vector<std::string> filenames;

    std::ifstream current_in;

    size_t filename_index;
};
