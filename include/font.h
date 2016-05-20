
#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

#include <string>
#include <freetype/include/ft2build.h>
#include FT_FREETYPE_H

#include "surface.h"

class Font{
    static FT_Library library;
    static bool initialized;
    static void initialize();

    public:
    ~Font();
    FT_Face face;
    void load(std::string filename);
    void draw(Surface*, std::string text, std::string prev_text, int scale, const int& x, const int& y);
    void drawNew(Surface*, std::string text, std::string prev_text, int scale, const int& x, const int& y);
    void drawPlain(Surface*, std::string text, int scale, const int& x, const int& y);
    void updateBlinkLocation(Surface*, std::string text, std::string prev_text, int scale, const int& x, const int& y);
};

#endif // FONT_H_INCLUDED
