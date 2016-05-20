
#ifndef STORAGE_H_INCLUDED
#define STORAGE_H_INCLUDED

#include <string>
#include <vector>
#include <fstream>

class BasicStorageReader{
    public:
    std::ifstream file;

    BasicStorageReader(std::string filename);
    ~BasicStorageReader();
    bool isOpen();
    std::vector<std::string> get();
    void close();
};

class BasicStorageWriter{
    public:
    std::ofstream file;

    BasicStorageWriter(std::string filename, bool append = true);
    ~BasicStorageWriter();
    bool isOpen();
    void add(std::string);
    void remove(std::string);
    void close();
};

class StorageReader{
    public:
    StorageReader(std::string filename);
    std::string get(std::string tag);
    bool exists(std::string tag);
};

class StorageWriter{
    public:
    StorageWriter(std::string filename);
    void add(std::string tag, std::string value);
    void set(std::string tag, std::string value);
    void remove(std::string tag, std::string value);
};

class PngReader{
    public:
    std::ifstream file;

    PngReader(std::string filename);
    ~PngReader();
    bool isOpen();
    unsigned char* get();
    void close();
};

#endif // STORAGE_H_INCLUDED
