
#ifndef SURFACE_H_INCLUDED
#define SURFACE_H_INCLUDED

#include <GLFW/glfw3.h>

#include "image.h"

class Surface{
    public:
    unsigned int width;
    unsigned int height;
    unsigned char* pixels;
    unsigned int cached_texture;
    bool cached_texture_exists;
    bool update_texture;

    Surface();
    Surface(unsigned int, unsigned int);
    Surface(unsigned char*, unsigned int, unsigned int);
    ~Surface();
    void draw(int x, int y);
    void draw(int x, int y, int a);
    void resize(unsigned int, unsigned int);
    void clear();
};

#endif // SURFACE_H_INCLUDED
