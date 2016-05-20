
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include "../include/storage.h"

using namespace std;

BasicStorageReader::BasicStorageReader(std::string filename){
    file.open(filename.c_str(), ios::in | ios::binary);
    file.seekg(0, file.beg);
}
BasicStorageReader::~BasicStorageReader(){
    if(file.is_open()) file.close();
}
bool BasicStorageReader::isOpen(){
    return file.is_open();
}
vector<string> BasicStorageReader::get(){
    string line;
    vector<string> data;
    char buffer;

    if(file.is_open()){
        while(!file.eof()){
            file.read(&buffer, 1);

            if(file.eof()){}
            else if(buffer == ('\n'+27)){
                if(line!=""){
                    data.push_back(line);
                    line = "";
                }
            } else {
                line += char(buffer-27);
            }
        }

        if(line!=""){
            data.push_back(line);
        }
    }
    return data;
}
void BasicStorageReader::close(){
    file.close();
}

BasicStorageWriter::BasicStorageWriter(std::string filename, bool append){
    if(append){
        file.open(filename.c_str(), ios::binary | ios::out | ios::app);
    }
    else {
        file.open(filename.c_str(), ios::binary | ios::out);
    }

    file.seekp(0);
}
BasicStorageWriter::~BasicStorageWriter(){
    if(file.is_open()) file.close();
}
bool BasicStorageWriter::isOpen(){
    return file.is_open();
}
void BasicStorageWriter::add(std::string data){
    char buffer;

    for(unsigned int i = 0; i < data.length(); i++){
        buffer = data[i]+27;
        file.write(&buffer,1);
    }

    buffer = ('\n'+27);
    file.write(&buffer,1);
}
void BasicStorageWriter::remove(std::string){

}
void BasicStorageWriter::close(){
    file.close();
}

StorageReader::StorageReader(string filename){}
string StorageReader::get(string tag){}
bool StorageReader::exists(string tag){}

StorageWriter::StorageWriter(string filename){}
void StorageWriter::add(string tag, string value){}
void StorageWriter::set(string tag, string value){}
void StorageWriter::remove(string tag, string value){}

PngReader::PngReader(std::string filename){
    file.open(filename.c_str(), ios::in | ios::binary);
}
PngReader::~PngReader(){
    if(file.is_open()) file.close();
}
bool PngReader::isOpen(){
    return file.is_open();
}
unsigned char* PngReader::get(){
    file.seekg(0, ios::end);
    unsigned int length = file.tellg();
    file.seekg(0, ios::beg);
    unsigned char* data = new unsigned char[length + 6];
    char buffer;

    if(file.is_open()){
        unsigned int i = 0;

        while(!file.eof()){
            file.read(&buffer, 1);

            data[0] = char(137);
            data[1] = char(80);
            data[2] = char(78);
            data[3] = char(71);
            data[4] = char(13);
            data[5] = char(10);

            if(!file.eof()){
                data[i + 6] = buffer;
                i++;
            }
        }
    } else {
        exit(EXIT_FAILURE);
    }

    return data;
}
void PngReader::close(){
    file.close();
}
