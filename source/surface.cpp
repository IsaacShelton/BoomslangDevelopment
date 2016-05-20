
#include <iostream>
#include <GLFW/glfw3.h>
#include "../include/surface.h"

using namespace std;

Surface::Surface(){
    pixels = NULL;
    width = 0;
    height = 0;
    cached_texture_exists = false;
    update_texture = true;
}
Surface::Surface(unsigned char* data, unsigned int w, unsigned int h){
    pixels = new unsigned char[4*w*h];

    if(data != NULL){
        for(unsigned int i = 0; i < 4*w*h; i++){
            pixels[i] = data[i];
        }
    }
    else {
        for(unsigned int i = 0; i < 4*w*h; i++){
            pixels[i] = 0;
        }
    }

    width = w;
    height = h;
    cached_texture_exists = false;
    update_texture = true;
}
Surface::Surface(unsigned int w, unsigned int h){
    pixels = new unsigned char[4*w*h];

    for(unsigned int i = 0; i < 4*w*h; i++){
        pixels[i] = 0;
    }

    width = w;
    height = h;
    cached_texture_exists = false;
    update_texture = true;
}

Surface::~Surface(){
    if(pixels != NULL){
        delete[] pixels;
    }

    if(cached_texture_exists){
        glDeleteTextures(1, &cached_texture);
    }
}

void Surface::draw(int x, int y){
    if(pixels != NULL){
        int w, h;

        if(update_texture){
            if(cached_texture_exists){
                glDeleteTextures(1, &cached_texture);
            }

            glGenTextures( 1, &cached_texture );
            glBindTexture( GL_TEXTURE_2D, cached_texture );
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
            glTexSubImage2D(GL_TEXTURE_2D,0,0,0,width,height,GL_RGBA,GL_UNSIGNED_BYTE,pixels);

            cached_texture_exists = true;
            update_texture = false;
        }

        glEnable(GL_TEXTURE_2D);
        glBindTexture( GL_TEXTURE_2D, cached_texture );

        glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH,&w);
        glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT,&h);

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
}
void Surface::draw(int x, int y, int a){
    if(pixels != NULL){
        int w, h;

        if(update_texture){
            if(cached_texture_exists){
                glDeleteTextures(1, &cached_texture);
            }

            glGenTextures( 1, &cached_texture );
            glBindTexture( GL_TEXTURE_2D, cached_texture );
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
            glTexSubImage2D(GL_TEXTURE_2D,0,0,0,width,height,GL_RGBA,GL_UNSIGNED_BYTE,pixels);

            cached_texture_exists = true;
            update_texture = false;
        }

        glEnable(GL_TEXTURE_2D);
        glBindTexture( GL_TEXTURE_2D, cached_texture );

        glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH,&w);
        glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT,&h);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glColor4ub( 255, 255, 255, a );
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
}
void Surface::resize(unsigned int w, unsigned int h){
    if(w < 1 or h < 1){
        w = 1;
        h = 1;
    }

    unsigned char* new_pixels = new unsigned char[w*h*4];

    width = w;
    height = h;

    if(pixels != NULL){
        delete[] pixels;
    }

    pixels = new_pixels;
    update_texture = true;
}
void Surface::clear(){
    for(int i = 0; i < int(width*height*4); i++){
        pixels[i] = 0;
    }
}
