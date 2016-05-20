
#include <fstream>
#include <iostream>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "../include/image.h"

using namespace std;

Image::Image(){}
Image::~Image(){
    glDeleteTextures(1 ,&texture);
}
Image::Image(const string& filename, const int& has_alpha, const int& blend){
    int w, h, comp;
    unsigned char* image;

    if(has_alpha){
        image = stbi_load(filename.c_str(), &w, &h, &comp, STBI_rgb_alpha);
    } else {
        image = stbi_load(filename.c_str(), &w, &h, &comp, STBI_rgb);
    }

    if(image == NULL){
        ifstream access;
        access.open(filename.c_str());

        if(!access.is_open()){
            access.close();
            cerr << "File does not exist" << endl;
        } else {
            access.close();
            cerr << "Invalid image format" << endl;
        }
    }

    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );

    if(!(bool)blend){
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    } else {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    }

    if(has_alpha){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    }

    stbi_image_free(image);
}
Image::Image(const unsigned char* data, unsigned int len_h, unsigned int len_w, const int& has_alpha, const int& blend){
    int w, h, comp;
    unsigned char* image;

    if(has_alpha){
        image = stbi_load_from_memory(data, 4*len_w*len_h, &w, &h, &comp, STBI_rgb_alpha);
    } else {
        image = stbi_load_from_memory(data, 3*len_w*len_h, &w, &h, &comp, STBI_rgb);
    }

    if(image == NULL){
        cerr << "Failed to load image" << endl;
    }

    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );

    if(!(bool)blend){
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    } else {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    }

    if(has_alpha){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    }

    stbi_image_free(image);
}
void Image::load(const unsigned char* data, unsigned int len_h, unsigned int len_w, const int& has_alpha, const int& blend){
    int w, h, comp;
    unsigned char* image;

    if(has_alpha){
        image = stbi_load_from_memory(data, 4*len_w*len_h, &w, &h, &comp, STBI_rgb_alpha);
    } else {
        image = stbi_load_from_memory(data, 3*len_w*len_h, &w, &h, &comp, STBI_rgb);
    }

    if(image == NULL){
        cerr << "Failed to load image" << endl;
    }

    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );

    if(!(bool)blend){
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    } else {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    }

    if(has_alpha){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    }

    stbi_image_free(image);
}
void Image::load(const string& filename, const int& has_alpha, const int& blend){
    int w, h, comp;
    unsigned char* image;

    if(has_alpha){
        image = stbi_load(filename.c_str(), &w, &h, &comp, STBI_rgb_alpha);
    } else {
        image = stbi_load(filename.c_str(), &w, &h, &comp, STBI_rgb);
    }

    if(image == NULL){
        ifstream access;
        access.open(filename.c_str());

        if(!access.is_open()){
            access.close();
            //throw boomslang_NotFound(boomslang_String("File does not exist"));
        } else {
            access.close();
            //throw boomslang_Invalid(boomslang_String("Invalid image format"));
        }
    }

    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );

    if(!(bool)blend){
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    } else {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    }

    if(has_alpha){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    }

    stbi_image_free(image);
}
void Image::draw(const int& x, const int& y){
    int width;
    int height;

    glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, texture );

    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH,&width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT,&height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3ub( 255, 255, 255 );
    glEnable( GL_TEXTURE_2D );
    glBegin(GL_QUADS);
    glTexCoord2i( 0, 0 );
    glVertex2i( x, y );
    glTexCoord2i( 1, 0 );
    glVertex2i( x + width, y );
    glTexCoord2i( 1, 1 );
    glVertex2i( x + width, y + height );
    glTexCoord2i( 0, 1 );
    glVertex2i( x, y + height );
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
