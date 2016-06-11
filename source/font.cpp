
#include <iostream>
#include <GLFW/glfw3.h>
#include "../include/font.h"
#include "../include/globals.h"
#include "../include/strings.h"
#include "../include/surface.h"

#define CLASS_COLOR    glColor3ub(228,100,255);
#define KEYWORD_COLOR  glColor3ub(43,150,255);
#define OPERATOR_COLOR glColor3ub(255,100,100);
#define STRING_COLOR   glColor3ub(255,103,204);
#define NUMBER_COLOR   glColor3ub(255,103,204);
#define COMMENT_COLOR  glColor3ub(160,160,160);

using namespace std;

bool Font::initialized = false;
FT_Library Font::library = NULL;

void Font::initialize(){
    if(initialized) return;

    int error = FT_Init_FreeType(&library);
    initialized = true;
}
void Font::load(string filename){
    initialize();
    int error = FT_New_Face(library, filename.c_str(), 0, &face);

    if (error == FT_Err_Unknown_File_Format){
      cerr << "Unknown Font Format" << endl;
    }
    else if(error){
      cerr << "Couldn't Access Font File" << endl;
    }
}

Font::~Font(){
    if(initialized) FT_Done_FreeType(library);
    initialized = false;
}
void Font::draw(Surface* surface, string text, string prev_text, int scale, const int& originx, const int& originy){
    initialize();

    int x = originx;
    int y = originy;
    int error = FT_Set_Pixel_Sizes(face, scale, scale);

    if(surface != NULL){
        surface->update_texture = true;

        glColor3ub(255,255,255);
        glEnable(GL_TEXTURE_2D);

        if(cursor_position == 0){
            blink_x = x - originx;
            blink_y = y;
        }

        // Skip over prev drawn text
        for(unsigned int i = 0; i < prev_text.length(); i++){
            if(prev_text[i] == '\n'){
                x = originx;
                y += face->height/64;

                if(i == cursor_position-1){
                    blink_x = x;
                    blink_y = y;
                }
                continue;
            }

            int glyph_index = FT_Get_Char_Index( face, prev_text.c_str()[i] );
            error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );

            if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
                error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
            }

            x += face->glyph->advance.x >> 6;
            if(i == cursor_position-1){
                if(blink){
                    blink_x = x - originx;
                    blink_y = y;
                }
            }
        }

        // Draw text
        for(unsigned int i = prev_text.length(); i < text.length(); i++){
            if(text.c_str()[i] == '\n'){
                x = originx;
                y += face->height/64;

                if(i == cursor_position-1){
                    blink_x = x - originx;
                    blink_y = y;
                }
                continue;
            }

            // Syntax Highlighting
            if(text.c_str()[i] == '0' or text.c_str()[i] == '1' or text.c_str()[i] == '2' or text.c_str()[i] == '3' or text.c_str()[i] == '4' or text.c_str()[i] == '5' or text.c_str()[i] == '6' or text.c_str()[i] == '7' or text.c_str()[i] == '8' or text.c_str()[i] == '9'){
                // Numbers
                NUMBER_COLOR;
            }
            for(unsigned int class_id = 0; class_id < classes.size(); class_id++){ // Classes
                if( text.length() >= i + classes[class_id].length() and DELIMITERS_CLASS ){
                    if(text.substr(i,classes[class_id].length())==classes[class_id]){
                        CLASS_COLOR;

                        for(unsigned int a = 0; a < classes[class_id].length()-1; a++){
                            int glyph_index = FT_Get_Char_Index( face, text.c_str()[i] );
                            error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );

                            if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
                                error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
                            }

                            float current_color[4];
                            glGetFloatv(GL_CURRENT_COLOR,current_color);

                            for(unsigned int j = 0; j < face->glyph->bitmap.rows; j++){ // y
                                for(unsigned int k = 0; k < face->glyph->bitmap.width; k++){ // x
                                    if(( (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3 < surface->width*surface->height*4)
                                    and (x+k < surface->width)){
                                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 ] = current_color[0]*255;
                                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 1] = current_color[1]*255;
                                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 2] = current_color[2]*255;
                                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3] = face->glyph->bitmap.buffer[j*face->glyph->bitmap.width + k];
                                    }
                                }
                            }
                            x += face->glyph->advance.x >> 6;
                            i++;

                            if(i == cursor_position-1){
                                blink_x = x - originx;
                                blink_y = y;
                            }
                        }
                    }
                }
            }
            for(unsigned int keyword_id = 0; keyword_id < keywords.size(); keyword_id++){ // Keywords
                if( text.length() >= i + keywords[keyword_id].length() and DELIMITERS_KEYWORD ){
                    if(text.substr(i,keywords[keyword_id].length())==keywords[keyword_id]){
                        KEYWORD_COLOR;

                        for(unsigned int a = 0; a < keywords[keyword_id].length()-1; a++){
                            int glyph_index = FT_Get_Char_Index( face, text.c_str()[i] );
                            error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );

                            if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
                                error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
                            }

                            float current_color[4];
                            glGetFloatv(GL_CURRENT_COLOR,current_color);

                            for(unsigned int j = 0; j < face->glyph->bitmap.rows; j++){ // y
                                for(unsigned int k = 0; k < face->glyph->bitmap.width; k++){ // x
                                    if(( (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3 < surface->width*surface->height*4)
                                    and (x+k < surface->width)){
                                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 ] = current_color[0]*255;
                                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 1] = current_color[1]*255;
                                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 2] = current_color[2]*255;
                                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3] = face->glyph->bitmap.buffer[j*face->glyph->bitmap.width + k];
                                    }
                                }
                            }
                            x += face->glyph->advance.x >> 6;
                            i++;

                            if(i == cursor_position-1){
                                blink_x = x - originx;
                                blink_y = y;
                            }
                        }
                    }
                }
            }
            for(unsigned int operator_id = 0; operator_id < operators.size(); operator_id++){ // Operators
                if( text.length() >= i + operators[operator_id].length() ){
                    if(text.substr(i,operators[operator_id].length())==operators[operator_id]){
                        OPERATOR_COLOR;

                        for(unsigned int a = 0; a < operators[operator_id].length()-1; a++){
                            int glyph_index = FT_Get_Char_Index( face, text.c_str()[i] );
                            error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );

                            if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
                                error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
                            }

                            float current_color[4];
                            glGetFloatv(GL_CURRENT_COLOR,current_color);

                            for(unsigned int j = 0; j < face->glyph->bitmap.rows; j++){ // y
                                for(unsigned int k = 0; k < face->glyph->bitmap.width; k++){ // x
                                    if(( (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3 < surface->width*surface->height*4)
                                    and (x+k < surface->width)){
                                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 ] = current_color[0]*255;
                                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 1] = current_color[1]*255;
                                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 2] = current_color[2]*255;
                                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3] = face->glyph->bitmap.buffer[j*face->glyph->bitmap.width + k];
                                    }
                                }
                            }
                            x += face->glyph->advance.x >> 6;
                            i++;

                            if(i == cursor_position-1){
                                blink_x = x - originx;
                                blink_y = y;
                            }
                        }
                    }
                }
            }
            if(text.substr(i,1)=="\""){ // Strings
                STRING_COLOR;

                for(unsigned int a = 0; (text.c_str()[i] != '"' and i < text.length()) or (a == 0); a++){
                    if(text.c_str()[i] == '\n'){
                        x = originx;
                        y += face->height/64;

                        if(i == cursor_position-1){
                            blink_x = x - originx;
                            blink_y = y;
                        }
                    } else {
                        int glyph_index = FT_Get_Char_Index( face, text.c_str()[i] );
                        error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );

                        if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
                            error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
                        }

                        float current_color[4];
                        glGetFloatv(GL_CURRENT_COLOR,current_color);

                        for(unsigned int j = 0; j < face->glyph->bitmap.rows; j++){ // y
                            for(unsigned int k = 0; k < face->glyph->bitmap.width; k++){ // x
                                if(( (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3 < surface->width*surface->height*4)
                                and (x+k < surface->width)){
                                    surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 ] = current_color[0]*255;
                                    surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 1] = current_color[1]*255;
                                    surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 2] = current_color[2]*255;
                                    surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3] = face->glyph->bitmap.buffer[j*face->glyph->bitmap.width + k];
                                }
                            }
                        }
                        x += face->glyph->advance.x >> 6;
                    }
                    i++;

                    if(i == cursor_position){
                        blink_x = x - originx;
                        blink_y = y;
                    }
                }
            }
            if(text.substr(i,1)=="#"){ // Line Comments
                COMMENT_COLOR;

                for(unsigned int a = 0; (text.c_str()[i] != '\n' and i < text.length()) or (a == 0); a++){
                    int glyph_index = FT_Get_Char_Index( face, text.c_str()[i] );
                    error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );

                    if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
                        error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
                    }

                    float current_color[4];
                    glGetFloatv(GL_CURRENT_COLOR,current_color);

                    for(unsigned int j = 0; j < face->glyph->bitmap.rows; j++){ // y
                        for(unsigned int k = 0; k < face->glyph->bitmap.width; k++){ // x
                            if(( (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3 < surface->width*surface->height*4)
                            and (x+k < surface->width)){
                                surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 ] = current_color[0]*255;
                                surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 1] = current_color[1]*255;
                                surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 2] = current_color[2]*255;
                                surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3] = face->glyph->bitmap.buffer[j*face->glyph->bitmap.width + k];
                            }
                        }
                    }
                    x += face->glyph->advance.x >> 6;
                    i++;

                    if(i == cursor_position){
                        blink_x = x - originx;
                        blink_y = y;
                    }
                }
            }
            if(text.substr(i,2)=="/#"){ // Multi-Line Comments
                COMMENT_COLOR;

                for(unsigned int a = 0; (text.c_str()[i] != '#' and i < text.length()) or (a == 1); a++){
                    if(text.c_str()[i] == '\n'){
                        x = originx;
                        y += face->height/64;

                        if(i == cursor_position-1){
                            blink_x = x - originx;
                            blink_y = y;
                        }
                    }
                    else {
                        int glyph_index = FT_Get_Char_Index( face, text.c_str()[i] );
                        error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );

                        if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
                            error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
                        }

                        float current_color[4];
                        glGetFloatv(GL_CURRENT_COLOR,current_color);

                        for(unsigned int j = 0; j < face->glyph->bitmap.rows; j++){ // y
                            for(unsigned int k = 0; k < face->glyph->bitmap.width; k++){ // x
                                if(( (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3 < surface->width*surface->height*4)
                                and (x+k < surface->width)){
                                    surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 ] = current_color[0]*255;
                                    surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 1] = current_color[1]*255;
                                    surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 2] = current_color[2]*255;
                                    surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3] = face->glyph->bitmap.buffer[j*face->glyph->bitmap.width + k];
                                }
                            }
                        }
                        x += face->glyph->advance.x >> 6;
                    }

                    i++;

                    if(i == cursor_position){
                        blink_x = x - originx;
                        blink_y = y;
                    }
                }
            }

            if(text.c_str()[i] == '\n'){
                x = originx;
                y += face->height/64;

                if(i == cursor_position-1){
                    blink_x = x - originx;
                    blink_y = y;
                }
            }
            else {
                int glyph_index = FT_Get_Char_Index( face, text.c_str()[i] );
                error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );

                if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
                    error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
                }

                float current_color[4];
                glGetFloatv(GL_CURRENT_COLOR,current_color);

                for(unsigned int j = 0; j < face->glyph->bitmap.rows; j++){ // y
                    for(unsigned int k = 0; k < face->glyph->bitmap.width; k++){ // x
                        if(( (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3 < surface->width*surface->height*4)
                        and (x+k < surface->width)){
                            surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 ] = current_color[0]*255;
                            surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 1] = current_color[1]*255;
                            surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 2] = current_color[2]*255;
                            surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3] = face->glyph->bitmap.buffer[j*face->glyph->bitmap.width + k];
                        }
                    }
                }

                x += face->glyph->advance.x >> 6;

                if(i == cursor_position-1){
                    blink_x = x - originx;
                    blink_y = y;
                }
            }

            glColor3ub(255,255,255);
            glDisable(GL_TEXTURE_2D);
        }
    }
}
void Font::updateBlinkLocation(Surface* surface, string text, string prev_text, int scale, const int& originx, const int& originy){
    initialize();

    int x = originx;
    int y = originy;
    int error = FT_Set_Pixel_Sizes(face, scale, scale);

    if(surface == NULL){
        glColor3ub(255,255,255);
        glEnable(GL_TEXTURE_2D);

        if(cursor_position == 0){
            if(blink){
                glDisable(GL_TEXTURE_2D);
                glBegin(GL_QUADS);
                glVertex2i(x + 1,y + 3);
                glVertex2i(x + 3,y + 3);
                glVertex2i(x + 3,y + 21);
                glVertex2i(x + 1,y + 21);
                glEnd();
                glEnable(GL_TEXTURE_2D);
            }
        }

        for(unsigned int i = 0; i < text.length(); i++){
            if(text.c_str()[i] == '\n'){
                x = originx;
                y += face->height/64;

                if(i == cursor_position-1){
                    if(blink){
                        glDisable(GL_TEXTURE_2D);
                        glBegin(GL_QUADS);
                        glVertex2i(x + 1,y + 3);
                        glVertex2i(x + 3,y + 3);
                        glVertex2i(x + 3,y + 21);
                        glVertex2i(x + 1,y + 21);
                        glEnd();
                        glEnable(GL_TEXTURE_2D);
                    }
                }
                continue;
            }

            int glyph_index = FT_Get_Char_Index( face, text.c_str()[i] );
            error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );

            if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
                error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
            }

            unsigned int texture;
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glGenTextures( 1, &texture );
            glBindTexture( GL_TEXTURE_2D, texture );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

            int width;
            int height;
            glBindTexture( GL_TEXTURE_2D, texture );
            glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH,&width);
            glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT,&height);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glBegin(GL_QUADS);
            glTexCoord2i( 0, 0 );
            glVertex2i( x + face->glyph->bitmap_left, y - face->glyph->bitmap_top + face->height/64);
            glTexCoord2i( 1, 0 );
            glVertex2i( x + width + face->glyph->bitmap_left, y - face->glyph->bitmap_top + face->height/64);
            glTexCoord2i( 1, 1 );
            glVertex2i( x + width + face->glyph->bitmap_left, y + height - face->glyph->bitmap_top + face->height/64);
            glTexCoord2i( 0, 1 );
            glVertex2i( x + face->glyph->bitmap_left , y + height - face->glyph->bitmap_top + face->height/64);
            glEnd();

            glDeleteTextures(1 ,&texture);

            x += face->glyph->advance.x >> 6;
            glColor3ub(255,255,255);

            if(i == cursor_position-1){
                if(blink){
                    glBegin(GL_QUADS);
                    glVertex2i(x + 1,y + 3);
                    glVertex2i(x + 3,y + 3);
                    glVertex2i(x + 3,y + 21);
                    glVertex2i(x + 1,y + 21);
                    glEnd();
                }
            }
        }
        glDisable(GL_TEXTURE_2D);
    }
    else {
        if(cursor_position == 0){
            blink_x = x - originx;
            blink_y = y;
        }

        for(unsigned int i = 0; i < text.length(); i++){
            if(text.c_str()[i] == '\n'){
                x = originx;
                y += face->height/64;

                if(i == cursor_position-1){
                    if(blink){
                        blink_x = x - originx;
                        blink_y = y;
                    }
                }
                continue;
            }

            int glyph_index = FT_Get_Char_Index( face, text.c_str()[i] );
            error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );

            if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
                error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
            }

            x += face->glyph->advance.x >> 6;

            if(i == cursor_position-1){
                blink_x = x - originx;
                blink_y = y;
            }
        }
    }
}
void Font::drawNew(Surface* surface, string text, string prev_text, int scale, const int& originx, const int& originy){
    initialize();

    int x = originx;
    int y = originy;
    int error = FT_Set_Pixel_Sizes(face, scale, scale);

    if(surface != NULL){
        surface->update_texture = true;

        glColor3ub(255,255,255);
        glEnable(GL_TEXTURE_2D);

        if(cursor_position == 0){
            blink_x = x - originx;
            blink_y = y;
        }

        bool string_available = true;
        bool multiline_comment_available = true;
        bool in_multiline_comment = false;

        if(string_count(text,"/#") > (string_count(text," #") + string_count(text,"\n#"))){
            in_multiline_comment = true;
        }

        for(unsigned int i = 0; i < prev_text.length(); i++){
            //Skip over predrawn lines
            if(string_contains(prev_text.substr(i, prev_text.length()-i),"\n")){
                i += string_get_until(prev_text.substr(i, prev_text.length()-i),"\n").length();
                x = originx;
                y += face->height/64;
            }
            else { // Jump over characters in the same line

                // Syntax Highlighting
                if(!in_multiline_comment){
                    for(unsigned int class_id = 0; class_id < classes.size(); class_id++){ // Classes
                        if( text.length() >= i + classes[class_id].length() and DELIMITERS_CLASS ){
                            if(text.substr(i,classes[class_id].length())==classes[class_id]){
                                CLASS_COLOR;
                                int xx = x;
                                int yy = y;

                                for(unsigned int a = 0; a < classes[class_id].length(); a++){
                                    int glyph_index = FT_Get_Char_Index( face, text.c_str()[i + a] );
                                    error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );

                                    if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
                                        error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
                                    }

                                    float current_color[4];
                                    glGetFloatv(GL_CURRENT_COLOR,current_color);

                                    for(unsigned int j = 0; j < face->glyph->bitmap.rows; j++){ // y
                                        for(unsigned int k = 0; k < face->glyph->bitmap.width; k++){ // x
                                            if(( (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 3 < surface->width*surface->height*4)
                                            and (xx+k < surface->width)){
                                                surface->pixels[ (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 ] = current_color[0]*255;
                                                surface->pixels[ (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 1] = current_color[1]*255;
                                                surface->pixels[ (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 2] = current_color[2]*255;
                                                surface->pixels[ (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 3] = face->glyph->bitmap.buffer[j*face->glyph->bitmap.width + k];
                                            }
                                        }
                                    }
                                    xx += face->glyph->advance.x >> 6;

                                    if(i == cursor_position-1){
                                        blink_x = xx - originx;
                                        blink_y = xx;
                                    }
                                }

                                if(i + classes[class_id].length() == text.length()){
                                    prev_text += text.c_str()[i + classes[class_id].length()];
                                }
                            }
                        }
                    }
                    for(unsigned int keyword_id = 0; keyword_id < keywords.size(); keyword_id++){ // Keywords
                        if( text.length() >= i + keywords[keyword_id].length()  and DELIMITERS_KEYWORD ){
                            if(text.substr(i,keywords[keyword_id].length())==keywords[keyword_id]){
                                KEYWORD_COLOR;
                                int xx = x;
                                int yy = y;

                                for(unsigned int a = 0; a < keywords[keyword_id].length(); a++){
                                    int glyph_index = FT_Get_Char_Index( face, text.c_str()[i + a] );
                                    error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );

                                    if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
                                        error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
                                    }

                                    float current_color[4];
                                    glGetFloatv(GL_CURRENT_COLOR,current_color);

                                    for(unsigned int j = 0; j < face->glyph->bitmap.rows; j++){ // y
                                        for(unsigned int k = 0; k < face->glyph->bitmap.width; k++){ // x
                                            if(( (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 3 < surface->width*surface->height*4)
                                            and (xx+k < surface->width)){
                                                surface->pixels[ (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 ] = current_color[0]*255;
                                                surface->pixels[ (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 1] = current_color[1]*255;
                                                surface->pixels[ (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 2] = current_color[2]*255;
                                                surface->pixels[ (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 3] = face->glyph->bitmap.buffer[j*face->glyph->bitmap.width + k];
                                            }
                                        }
                                    }
                                    xx += face->glyph->advance.x >> 6;

                                    if(i == cursor_position-1){
                                        blink_x = xx - originx;
                                        blink_y = xx;
                                    }
                                }

                                if(i + keywords[keyword_id].length() == text.length()){
                                    prev_text += text.c_str()[i + keywords[keyword_id].length()];
                                }
                            }
                        }
                    }
                    for(unsigned int operator_id = 0; operator_id < operators.size(); operator_id++){ // Keywords
                        if( text.length() >= i + operators[operator_id].length() ){
                            if(text.substr(i,operators[operator_id].length())==operators[operator_id]){
                                OPERATOR_COLOR;
                                int xx = x;
                                int yy = y;

                                for(unsigned int a = 0; a < operators[operator_id].length(); a++){
                                    int glyph_index = FT_Get_Char_Index( face, text.c_str()[i + a] );
                                    error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );

                                    if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
                                        error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
                                    }

                                    float current_color[4];
                                    glGetFloatv(GL_CURRENT_COLOR,current_color);

                                    for(unsigned int j = 0; j < face->glyph->bitmap.rows; j++){ // y
                                        for(unsigned int k = 0; k < face->glyph->bitmap.width; k++){ // x
                                            if(( (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 3 < surface->width*surface->height*4)
                                            and (xx+k < surface->width)){
                                                surface->pixels[ (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 ] = current_color[0]*255;
                                                surface->pixels[ (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 1] = current_color[1]*255;
                                                surface->pixels[ (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 2] = current_color[2]*255;
                                                surface->pixels[ (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 3] = face->glyph->bitmap.buffer[j*face->glyph->bitmap.width + k];
                                            }
                                        }
                                    }
                                    xx += face->glyph->advance.x >> 6;

                                    if(i == cursor_position-1){
                                        blink_x = xx - originx;
                                        blink_y = xx;
                                    }
                                }

                                if(i + operators[operator_id].length() == text.length()){
                                    prev_text += text.c_str()[i + operators[operator_id].length()];
                                }
                            }
                        }
                    }
                    if(text.substr(i,1) == "\""){ // Strings
                        if(string_available){
                            STRING_COLOR;
                            int xx = x;
                            int yy = y;

                            unsigned int a;
                            for(a = 0; (text.c_str()[i+a] != '"' and i+a < text.length()) or (a == 0); a++){
                                if(text.c_str()[i+a] == '\n'){
                                    xx = originx;
                                    yy += face->height/64;

                                    if(i == cursor_position-1){
                                        blink_x = xx - originx;
                                        blink_y = yy;
                                    }
                                }
                                else {
                                    int glyph_index = FT_Get_Char_Index( face, text.c_str()[i + a] );
                                    error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );

                                    if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
                                        error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
                                    }

                                    float current_color[4];
                                    glGetFloatv(GL_CURRENT_COLOR,current_color);

                                    for(unsigned int j = 0; j < face->glyph->bitmap.rows; j++){ // y
                                        for(unsigned int k = 0; k < face->glyph->bitmap.width; k++){ // x
                                            if(( (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 3 < surface->width*surface->height*4)
                                            and (xx+k < surface->width)){
                                                surface->pixels[ (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 ] = current_color[0]*255;
                                                surface->pixels[ (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 1] = current_color[1]*255;
                                                surface->pixels[ (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 2] = current_color[2]*255;
                                                surface->pixels[ (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 3] = face->glyph->bitmap.buffer[j*face->glyph->bitmap.width + k];
                                            }
                                        }
                                    }
                                    xx += face->glyph->advance.x >> 6;
                                }

                                if(i == cursor_position-1){
                                    blink_x = xx - originx;
                                    blink_y = yy;
                                }

                                string_available = false;

                                if(i+a == prev_text.length()){
                                    prev_text += text.substr(i + a, 1);
                                }
                            }
                        } else {
                            string_available = true;
                        }
                    }
                    if(text.substr(i,1) == "#"){ // Line Comment
                            COMMENT_COLOR;
                            int xx = x;

                            unsigned int a;
                            for(a = 0; (text.c_str()[i+a] != '\n' and i+a < text.length()) or (a == 0); a++){
                                int glyph_index = FT_Get_Char_Index( face, text.c_str()[i + a] );
                                error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );

                                if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
                                    error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
                                }

                                float current_color[4];
                                glGetFloatv(GL_CURRENT_COLOR,current_color);

                                for(unsigned int j = 0; j < face->glyph->bitmap.rows; j++){ // y
                                    for(unsigned int k = 0; k < face->glyph->bitmap.width; k++){ // x
                                        if(( (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 3 < surface->width*surface->height*4)
                                        and (xx+k < surface->width)){
                                            surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 ] = current_color[0]*255;
                                            surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 1] = current_color[1]*255;
                                            surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 2] = current_color[2]*255;
                                            surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 3] = face->glyph->bitmap.buffer[j*face->glyph->bitmap.width + k];
                                        }
                                    }
                                }
                                xx += face->glyph->advance.x >> 6;

                                if(i == cursor_position-1){
                                    blink_x = xx - originx;
                                    blink_y = xx;
                                }

                                if(i+a == prev_text.length()){
                                    prev_text += text.substr(i + a, 1);
                                }
                            }
                    }
                    if(text.substr(i,2) == "/#"){ // Multiline Comment
                    if(multiline_comment_available){
                        glColor3ub(160,160,160);
                        int xx = x;
                        int yy = y;

                        unsigned int a;
                        for(a = 0; (text.c_str()[i+a] != '\n' and i+a < text.length()) or (a == 1); a++){
                            int glyph_index = FT_Get_Char_Index( face, text.c_str()[i + a] );
                            error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );

                            if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
                                error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
                            }

                            float current_color[4];
                            glGetFloatv(GL_CURRENT_COLOR,current_color);

                            for(unsigned int j = 0; j < face->glyph->bitmap.rows; j++){ // y
                                for(unsigned int k = 0; k < face->glyph->bitmap.width; k++){ // x
                                    if(( (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 3 < surface->width*surface->height*4)
                                    and (x+k < surface->width)){
                                        surface->pixels[ (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 ] = current_color[0]*255;
                                        surface->pixels[ (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 1] = current_color[1]*255;
                                        surface->pixels[ (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 2] = current_color[2]*255;
                                        surface->pixels[ (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 3] = face->glyph->bitmap.buffer[j*face->glyph->bitmap.width + k];
                                    }
                                }
                            }
                            xx += face->glyph->advance.x >> 6;

                            if(i == cursor_position-1){
                                blink_x = xx - originx;
                                blink_y = xx;
                            }

                            multiline_comment_available = false;

                            if(i+a == prev_text.length()){
                                prev_text += text.substr(i + a, 1);
                            }
                        }
                    } else {
                        multiline_comment_available = true;
                    }
                }
                }
                else {
                    COMMENT_COLOR;
                    int xx = x;
                    int yy = y;

                    unsigned int a;
                    for(a = 0; (text.c_str()[i+a] != '#' and i+a < text.length()) or (a == 1); a++){
                        if(text.c_str()[i+a] == '\n'){
                            xx = originx;
                            yy += face->height/64;

                            if(i == cursor_position-1){
                                blink_x = xx;
                                blink_y = yy;
                            }
                        }
                        else {
                            int glyph_index = FT_Get_Char_Index( face, text.c_str()[i + a] );
                            error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );

                            if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
                                error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
                            }

                            float current_color[4];
                            glGetFloatv(GL_CURRENT_COLOR,current_color);

                            for(unsigned int j = 0; j < face->glyph->bitmap.rows; j++){ // y
                                for(unsigned int k = 0; k < face->glyph->bitmap.width; k++){ // x
                                    if(( (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 3 < surface->width*surface->height*4)
                                    and (x+k < surface->width)){
                                        surface->pixels[ (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 ] = current_color[0]*255;
                                        surface->pixels[ (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 1] = current_color[1]*255;
                                        surface->pixels[ (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 2] = current_color[2]*255;
                                        surface->pixels[ (surface->width*( (yy - face->glyph->bitmap_top + face->height/64) +j) + (xx+k))*4 + 3] = face->glyph->bitmap.buffer[j*face->glyph->bitmap.width + k];
                                    }
                                }
                            }
                            xx += face->glyph->advance.x >> 6;
                        }

                        if(i == cursor_position-1){
                            blink_x = xx - originx;
                            blink_y = yy;
                        }

                        if(i+a == prev_text.length()){
                            prev_text += text.substr(i + a, 1);
                        }
                    }
                }

                int glyph_index = FT_Get_Char_Index( face, text.c_str()[i] );
                error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );

                if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
                    error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
                }

                x += face->glyph->advance.x >> 6;

                if(i == cursor_position-1){
                    blink_x = x - originx;
                    blink_y = y;
                }
            }
        }

        for(unsigned int i = prev_text.length(); i < text.length(); i++){
            glColor3ub(255,255,255);

            // Newline Handling
            if(text.c_str()[i] == '\n'){
                x = originx;
                y += face->height/64;

                if(i == cursor_position-1){
                    blink_x = x - originx;
                    blink_y = y;
                }
                continue;
            }

            // Syntax highlighting
            if(text.c_str()[i] == '0' or text.c_str()[i] == '1' or text.c_str()[i] == '2' or text.c_str()[i] == '3' or text.c_str()[i] == '4' or text.c_str()[i] == '5' or text.c_str()[i] == '6' or text.c_str()[i] == '7' or text.c_str()[i] == '8' or text.c_str()[i] == '9'){
                NUMBER_COLOR;
            }
            for(unsigned int class_id = 0; class_id < classes.size(); class_id++){ // Classes
                if( text.length() >= i + classes[class_id].length() and DELIMITERS_CLASS ){
                    if(text.substr(i,classes[class_id].length())==classes[class_id]){
                        CLASS_COLOR;

                        for(unsigned int a = 0; a < classes[class_id].length()-1; a++){
                            int glyph_index = FT_Get_Char_Index( face, text.c_str()[i] );
                            error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );

                            if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
                                error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
                            }

                            float current_color[4];
                            glGetFloatv(GL_CURRENT_COLOR,current_color);

                            for(unsigned int j = 0; j < face->glyph->bitmap.rows; j++){ // y
                                for(unsigned int k = 0; k < face->glyph->bitmap.width; k++){ // x
                                    if(( (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3 < surface->width*surface->height*4)
                                    and (x+k < surface->width)){
                                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 ] = current_color[0]*255;
                                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 1] = current_color[1]*255;
                                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 2] = current_color[2]*255;
                                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3] = face->glyph->bitmap.buffer[j*face->glyph->bitmap.width + k];
                                    }
                                }
                            }
                            x += face->glyph->advance.x >> 6;
                            i++;

                            if(i == cursor_position-1){
                                blink_x = x - originx;
                                blink_y = y;
                            }
                        }
                    }
                }
            }
            for(unsigned int operator_id = 0; operator_id < operators.size(); operator_id++){ // Operators
                if( text.length() >= i + operators[operator_id].length() ){
                    if(text.substr(i,operators[operator_id].length())==operators[operator_id]){
                        OPERATOR_COLOR;

                        for(unsigned int a = 0; a < operators[operator_id].length()-1; a++){
                            int glyph_index = FT_Get_Char_Index( face, text.c_str()[i] );
                            error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );

                            if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
                                error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
                            }

                            float current_color[4];
                            glGetFloatv(GL_CURRENT_COLOR,current_color);

                            for(unsigned int j = 0; j < face->glyph->bitmap.rows; j++){ // y
                                for(unsigned int k = 0; k < face->glyph->bitmap.width; k++){ // x
                                    if(( (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3 < surface->width*surface->height*4)
                                    and (x+k < surface->width)){
                                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 ] = current_color[0]*255;
                                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 1] = current_color[1]*255;
                                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 2] = current_color[2]*255;
                                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3] = face->glyph->bitmap.buffer[j*face->glyph->bitmap.width + k];
                                    }
                                }
                            }
                            x += face->glyph->advance.x >> 6;
                            i++;

                            if(i == cursor_position-1){
                                blink_x = x - originx;
                                blink_y = y;
                            }
                        }
                    }
                }
            }
            for(unsigned int keyword_id = 0; keyword_id < keywords.size(); keyword_id++){ // Keywords
                if( text.length() >= i + keywords[keyword_id].length() ){
                    if(text.substr(i,keywords[keyword_id].length())==keywords[keyword_id] and DELIMITERS_KEYWORD ){
                        KEYWORD_COLOR;

                        for(unsigned int a = 0; a < keywords[keyword_id].length()-1; a++){
                            int glyph_index = FT_Get_Char_Index( face, text.c_str()[i] );
                            error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );

                            if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
                                error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
                            }

                            float current_color[4];
                            glGetFloatv(GL_CURRENT_COLOR,current_color);

                            for(unsigned int j = 0; j < face->glyph->bitmap.rows; j++){ // y
                                for(unsigned int k = 0; k < face->glyph->bitmap.width; k++){ // x
                                    if(( (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3 < surface->width*surface->height*4)
                                    and (x+k < surface->width)){
                                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 ] = current_color[0]*255;
                                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 1] = current_color[1]*255;
                                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 2] = current_color[2]*255;
                                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3] = face->glyph->bitmap.buffer[j*face->glyph->bitmap.width + k];
                                    }
                                }
                            }
                            x += face->glyph->advance.x >> 6;
                            i++;

                            if(i == cursor_position-1){
                                blink_x = x - originx;
                                blink_y = y;
                            }
                        }
                    }
                }
            }
            if(text.substr(i,1)=="\""){ // Strings
                STRING_COLOR;

                for(unsigned int a = 0; (text.c_str()[i] != '"' and i < text.length()) or (a == 0); a++){
                    int glyph_index = FT_Get_Char_Index( face, text.c_str()[i] );
                    error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );

                    if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
                        error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
                    }

                    float current_color[4];
                    glGetFloatv(GL_CURRENT_COLOR,current_color);

                    for(unsigned int j = 0; j < face->glyph->bitmap.rows; j++){ // y
                        for(unsigned int k = 0; k < face->glyph->bitmap.width; k++){ // x
                            if(( (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3 < surface->width*surface->height*4)
                            and (x+k < surface->width)){
                                surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 ] = current_color[0]*255;
                                surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 1] = current_color[1]*255;
                                surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 2] = current_color[2]*255;
                                surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3] = face->glyph->bitmap.buffer[j*face->glyph->bitmap.width + k];
                            }
                        }
                    }
                    x += face->glyph->advance.x >> 6;
                    i++;

                    if(i == cursor_position){
                        blink_x = x - originx;
                        blink_y = y;
                    }
                }
            }
            if(text.substr(i,1)=="#"){ // Line Comment
                COMMENT_COLOR;

                for(unsigned int a = 0; (text.c_str()[i] != '#' and i < text.length()) or (a == 0); a++){
                    int glyph_index = FT_Get_Char_Index( face, text.c_str()[i] );
                    error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );

                    if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
                        error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
                    }

                    float current_color[4];
                    glGetFloatv(GL_CURRENT_COLOR,current_color);

                    for(unsigned int j = 0; j < face->glyph->bitmap.rows; j++){ // y
                        for(unsigned int k = 0; k < face->glyph->bitmap.width; k++){ // x
                            if(( (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3 < surface->width*surface->height*4)
                            and (x+k < surface->width)){
                                surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 ] = current_color[0]*255;
                                surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 1] = current_color[1]*255;
                                surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 2] = current_color[2]*255;
                                surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3] = face->glyph->bitmap.buffer[j*face->glyph->bitmap.width + k];
                            }
                        }
                    }
                    x += face->glyph->advance.x >> 6;
                    i++;

                    if(i == cursor_position){
                        blink_x = x - originx;
                        blink_y = y;
                    }
                }
            }

            int glyph_index = FT_Get_Char_Index( face, text.c_str()[i] );
            error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );

            if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
                error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
            }

            float current_color[4];
            glGetFloatv(GL_CURRENT_COLOR,current_color);

            // Draw the character
            for(unsigned int j = 0; j < face->glyph->bitmap.rows; j++){ // y
                for(unsigned int k = 0; k < face->glyph->bitmap.width; k++){ // x
                    if(( (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3 < surface->width*surface->height*4)
                    and (x+k < surface->width)){
                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 ] = current_color[0]*255;
                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 1] = current_color[1]*255;
                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 2] = current_color[2]*255;
                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3] = face->glyph->bitmap.buffer[j*face->glyph->bitmap.width + k];
                    }
                }
            }

            // Advance x position
            x += face->glyph->advance.x >> 6;
            glColor3ub(255,255,255);

            if(i == cursor_position-1){
                blink_x = x - originx;
                blink_y = y;
            }
        }
        glDisable(GL_TEXTURE_2D);
    }
}
void Font::drawPlain(Surface* surface, string text, int scale, const int& originx, const int& originy){
    initialize();

    int x = originx;
    int y = originy;
    int error = FT_Set_Pixel_Sizes(face, scale, scale);

    if(surface == NULL){
        glEnable(GL_TEXTURE_2D);

        for(unsigned int i = 0; i < text.length(); i++){
            if(text.c_str()[i] == '\n'){
                x = originx;
                y += face->height/64;
                continue;
            }

            int glyph_index = FT_Get_Char_Index( face, text.c_str()[i] );
            error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );

            if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
                error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
            }

            unsigned int texture;
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glGenTextures( 1, &texture );
            glBindTexture( GL_TEXTURE_2D, texture );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

            int width;
            int height;
            glBindTexture( GL_TEXTURE_2D, texture );
            glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH,&width);
            glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT,&height);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glBegin(GL_QUADS);
            glTexCoord2i( 0, 0 );
            glVertex2i( x + face->glyph->bitmap_left, y - face->glyph->bitmap_top + face->height/64);
            glTexCoord2i( 1, 0 );
            glVertex2i( x + width + face->glyph->bitmap_left, y - face->glyph->bitmap_top + face->height/64);
            glTexCoord2i( 1, 1 );
            glVertex2i( x + width + face->glyph->bitmap_left, y + height - face->glyph->bitmap_top + face->height/64);
            glTexCoord2i( 0, 1 );
            glVertex2i( x + face->glyph->bitmap_left , y + height - face->glyph->bitmap_top + face->height/64);
            glEnd();

            glDeleteTextures(1 ,&texture);

            x += face->glyph->advance.x >> 6;
        }
        glDisable(GL_TEXTURE_2D);
    } else {
        surface->update_texture = true;

        glColor3ub(255,255,255);
        glEnable(GL_TEXTURE_2D);

        for(unsigned int i = 0; i < text.length(); i++){
            if(text.c_str()[i] == '\n'){
                x = originx;
                y += face->height/64;
                continue;
            }

            int glyph_index = FT_Get_Char_Index( face, text.c_str()[i] );
            error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );

            if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
                error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
            }

            float current_color[4];
            glGetFloatv(GL_CURRENT_COLOR,current_color);

            for(unsigned int j = 0; j < face->glyph->bitmap.rows; j++){ // y
                for(unsigned int k = 0; k < face->glyph->bitmap.width; k++){ // x
                    if(( (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3 < surface->width*surface->height*4)
                    and (x+k < surface->width)){
                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 ] = current_color[0]*255;
                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 1] = current_color[1]*255;
                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 2] = current_color[2]*255;
                        surface->pixels[ (surface->width*( (y - face->glyph->bitmap_top + face->height/64) +j) + (x+k))*4 + 3] = face->glyph->bitmap.buffer[j*face->glyph->bitmap.width + k];
                    }
                }
            }

            x += face->glyph->advance.x >> 6;
        }

        glDisable(GL_TEXTURE_2D);
    }
}
