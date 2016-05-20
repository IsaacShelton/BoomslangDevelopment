
#include <string>
#include <vector>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include "../include/globals.h"

using namespace std;

vector<string> folder_contents(string directory) {
    boost::filesystem::path path(directory.c_str());
    vector<string> entries;

    if(is_directory(path)) {
        for(boost::filesystem::directory_entry& entry : boost::make_iterator_range(boost::filesystem::directory_iterator(path), {})){
            entries.push_back(entry.path().string());
        }
    }

    return entries;
}
