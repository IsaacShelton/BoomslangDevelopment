
#include <sstream>
#include "../include/conversion.h"

using namespace std;

double to_double(string str){
    double dec;
    if( ! (std::istringstream(str) >> dec) ) dec = 0;
    return dec;
}
string to_string(double dec){
    std::ostringstream convert;
    convert << dec;
    return convert.str();
}
