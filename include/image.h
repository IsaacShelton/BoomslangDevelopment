
#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include <string>

class Image{
    public:
    unsigned int texture;

    Image();
    ~Image();
    Image(const std::string&, const int& has_alpha = 1, const int& blend = 1);
    Image(const unsigned char* data, unsigned int len_w, unsigned int len_h, const int& has_alpha = 1, const int& blend = 1);
    void load(const unsigned char* data, unsigned int len_w, unsigned int len_h, const int& has_alpha = 1, const int& blend = 1);
    void load(const std::string& filename, const int& has_alpha = 1, const int& blend = 1);
    void draw(const int& x, const int& y);
    void drawScaled(const int& x, const int& y, const int& xscale, const int& yscale);
};

#endif // IMAGE_H_INCLUDED
