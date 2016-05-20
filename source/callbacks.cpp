
#include <math.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include "../include/files.h"
#include "../include/strings.h"
#include "../include/strings.h"
#include "../include/globals.h"
#include "../include/platform.h"
#include "../include/conversion.h"
#include "../include/callbacks.h"

using namespace std;

void error_callback(int error, const char* description){
    fputs(description, stderr);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(room == ROOM_EDIT){
        if(key == GLFW_KEY_ESCAPE and action == GLFW_PRESS){
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        else if(key == GLFW_KEY_ENTER and action == GLFW_PRESS){
            text.insert(cursor_position, "\n");
            cursor_position++;
            blink = true;
            blink_timer = 30;
            arial.updateBlinkLocation(&code_surface, text, "", 16, 2, 0);
        }
        else if(key == GLFW_KEY_BACKSPACE and (action == GLFW_PRESS or action == GLFW_REPEAT) and cursor_position > 0){
            string::iterator it = text.begin() + (cursor_position-1);
            if( int(cursor_position)-4 > -1){
                if(text.substr(cursor_position-4,4) == "    "){
                    text.erase(it-3, it + 1);
                    cursor_position-=4;
                } else {
                    text.erase(it);
                    cursor_position--;
                }
            } else {
                text.erase(it);
                cursor_position--;
            }

            blink = true;
            blink_timer = 30;
        }
        else if(key == GLFW_KEY_LEFT and (action == GLFW_PRESS or action == GLFW_REPEAT) and cursor_position > 0){
            cursor_position--;
            blink = true;
            blink_timer = 30;
            arial.updateBlinkLocation(&code_surface, text, "", 16, 2, 0);
        }
        else if(key == GLFW_KEY_RIGHT and (action == GLFW_PRESS or action == GLFW_REPEAT) and cursor_position < text.length()){
            cursor_position++;
            blink = true;
            blink_timer = 30;
            arial.updateBlinkLocation(&code_surface, text, "", 16, 2, 0);
        }
        else if(key == GLFW_KEY_TAB and action == GLFW_PRESS){
            text.insert(cursor_position, "    ");
            cursor_position += 4;
            blink = true;
            blink_timer = 30;
        }
        else if( key == GLFW_KEY_S and glfwGetKey(window,GLFW_KEY_LEFT_CONTROL)==GLFW_PRESS and action == GLFW_PRESS ){
            if(current_file < open_files.size() and current_file >= 0){
                if(open_files[current_file] != "untitled"){
                    ofstream write(open_files[current_file]);

                    if(write.is_open()){
                        write << text;
                        write.close();
                        text_cache[current_file] = text;

                        message = "Successfully Saved";
                        message_timer = 120;
                    } else {
                        text_cache[current_file] = text;
                        message = "Failed to Saved";
                        message_timer = 120;
                    }
                } else {
                    // Get save location
                    room = ROOM_SAVE;
                }
            }
        }
        else if( key == GLFW_KEY_W and glfwGetKey(window,GLFW_KEY_LEFT_CONTROL)==GLFW_PRESS and action == GLFW_PRESS ){
            if(current_file < open_files.size() and current_file >= 0){
                vector<string>::iterator death_lazer = open_files.begin() + current_file;
                vector<string>::iterator death_lazer_cache = text_cache.begin() + current_file;

                if(open_files.size() > 0){
                    open_files.erase(death_lazer);
                }
                if(text_cache.size() > 0){
                    text_cache.erase(death_lazer_cache);
                }

                files_surface.clear();
                for(unsigned int i = 0; i < open_files.size(); i++){
                    arial.drawPlain(&files_surface,filename_name(open_files[i]),16,2, i*24);
                }
                files_surface.update_texture = true;

                if(current_file > 0) current_file--;

                if(current_file < open_files.size() and current_file >= 0){
                    text = text_cache[current_file];
                } else {
                    text = "";
                    code_surface.clear();
                    code_surface.update_texture = true;
                }

                cursor_position = 0;
            }
        }
    }
    else if(room == ROOM_OPEN){
        if(key == GLFW_KEY_BACKSPACE and (action == GLFW_PRESS or action == GLFW_REPEAT) and open_text.length()>0){
            string::iterator it = open_text.end()-1;
            open_text.erase(it);
        }
        else if(key == GLFW_KEY_ENTER and action == GLFW_PRESS){
            try{
                boost::filesystem::path folder(open_text);

                if(boost::filesystem::is_directory(folder)){
                    folder_location = open_text;
                    update_file_list = true;
                    file_list_surface.update_texture = true;
                    file_list_yscroll = 0;
                }
            } catch(...){
                message = "Failed to Open File";
                message_timer = 120;
                room = ROOM_EDIT;
            }
        }
    }
    else if(room == ROOM_SAVE){
        if(key == GLFW_KEY_BACKSPACE and (action == GLFW_PRESS or action == GLFW_REPEAT) and open_text.length()>0){
            string::iterator it = open_text.end()-1;
            open_text.erase(it);
        }
        else if(key == GLFW_KEY_ENTER and action == GLFW_PRESS){
            try{
                boost::filesystem::path folder(open_text);

                if(boost::filesystem::is_directory(folder)){
                    folder_location = open_text;
                    update_file_list = true;
                    file_list_surface.update_texture = true;
                    file_list_yscroll = 0;
                } else {
                    ofstream write(open_text);

                    if(write.is_open()){
                        cout << "good" << endl;
                        write << text;
                        write.close();

                        open_files[current_file] = open_text;

                        files_surface.clear();
                        for(unsigned int i = 0; i < open_files.size(); i++){
                            arial.drawPlain(&files_surface,filename_name(open_files[i]),16,2, i*24);
                        }

                        code_surface.clear();
                        arial.draw(&code_surface, text, "", 16, 2, 0);

                        line_number_surface.clear();
                        unsigned int line_number = 1;
                        arial.drawPlain(&line_number_surface,to_string(line_number),16,2, (line_number - 1)*16);
                        for(unsigned int i = 0; i < text.length(); i++){
                            if(text[i] == '\n'){
                                line_number++;
                                arial.drawPlain(&line_number_surface,to_string(line_number),16,2, (line_number - 1)*16);
                            }
                        }

                        message = "Successfully Saved";
                        message_timer = 120;
                    } else {
                        message = "Failed to Saved";
                        message_timer = 120;
                    }

                    room = ROOM_EDIT;
                }
            } catch(...){
                message = "Failed to Open File";
                message_timer = 120;
                room = ROOM_EDIT;
            }
        }
    }
}

void character_callback(GLFWwindow* window, unsigned int codepoint){
    if(room == ROOM_EDIT){
        char character[2] = {(char)codepoint, '\0'};
        text.insert(cursor_position, character);
        cursor_position++;
        blink = true;
        blink_timer = 30;
    } else if(room == ROOM_OPEN){
        open_text += (char)codepoint;
    } else if(room == ROOM_SAVE){
        open_text += (char)codepoint;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if(room == ROOM_EDIT){
        if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
            int x, y;
            double xprec, yprec;

            glfwGetCursorPos(window,&xprec,&yprec);
            x = floor(xprec); y = floor(yprec);

            if(x < 24 and y <= 24){ // NEW
                if(text_cache.size() > 0){
                    text_cache[current_file] = text;
                }

                text = "";
                cursor_position = 0;
                open_files.push_back("untitled");

                files_surface.clear();
                for(unsigned int i = 0; i < open_files.size(); i++){
                    arial.drawPlain(&files_surface,filename_name(open_files[i]),16,2, i*24);
                }

                code_surface.clear();
                arial.draw(&code_surface, text, "", 16, 2, 0);

                line_number_surface.clear();
                unsigned int line_number = 1;
                arial.drawPlain(&line_number_surface,to_string(line_number),16,2, (line_number - 1)*16);
                for(unsigned int i = 0; i < text.length(); i++){
                    if(text[i] == '\n'){
                        line_number++;
                        arial.drawPlain(&line_number_surface,to_string(line_number),16,2, (line_number - 1)*16);
                    }
                }

                text_cache.push_back(text);
                current_file = text_cache.size()-1;
            }
            else if(x < 48 and x >= 24 and y <= 24){ // OPEN
                room = ROOM_OPEN;
            }
            else if(x < 72 and x >= 48 and y <= 24){ // SAVE
                if(current_file < open_files.size() and current_file >= 0){
                    if(open_files[current_file] != "untitled"){
                        ofstream write(open_files[current_file]);

                        if(write.is_open()){
                            write << text;
                            write.close();
                            text_cache[current_file] = text;

                            message = "Successfully Saved";
                            message_timer = 120;
                        } else {
                            text_cache[current_file] = text;
                            message = "Failed to Saved";
                            message_timer = 120;
                        }
                    } else {
                        // Get save location
                        room = ROOM_SAVE;
                    }
                }
            }
            else if(x < 120 and x >= 96 and y <= 24){ // COMPILE
                if(open_files.size() > 0 and current_file < open_files.size() and current_file >= 0){
                    if(open_files[current_file] != "untitled"){
                        execute("C:/Users/isaac/Google Drive/Boomslang/Two/BoomslangCompiler/bin/Release/BoomslangCompiler.exe","-wait \"" + open_files[current_file] + "\"");
                    }
                }
            }
            else if(x < 144 and x >= 120 and y <= 24){ // RUN
                if(open_files.size() > 0 and current_file < open_files.size() and current_file >= 0){
                    if(open_files[current_file] != "untitled"){
                        execute(filename_change_ext(open_files[current_file],"exe"),"");
                    }
                }
            }
            else if(x < 168 and x >= 144 and y <= 24){ // CMD
                if(open_files.size() > 0 and current_file < open_files.size() and current_file >= 0){
                    if(open_files[current_file] != "untitled"){
                        // ...
                    }
                }
            }
            else if(x <= 160 and y > 24){ // SELECT TAB
                if( (y - 24)/24 < (int)open_files.size() ){
                    if(current_file < open_files.size()){
                        text_cache[current_file] = text;
                        current_file = (y - 24)/24;
                        text = text_cache[current_file];
                    } else {
                        text_cache[current_file] = text;
                        current_file = (y - 24)/24;
                        text = "";
                        code_surface.clear();
                        code_surface.update_texture = true;
                    }

                    cursor_position = 0;
                }
            }
        }
        else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
            int x, y;
            double xprec, yprec;

            glfwGetCursorPos(window,&xprec,&yprec);
            x = floor(xprec); y = floor(yprec);

            if(x < 160 and y > 24){ // CLOSE
                int item = (y - 24)/24;

                if(item < (int)open_files.size() and item >= 0){
                    vector<string>::iterator death_lazer = open_files.begin() + item;
                    vector<string>::iterator death_lazer_cache = text_cache.begin() + item;

                    if(open_files.size() > 0){
                        open_files.erase(death_lazer);
                    }
                    if(text_cache.size() > 0){
                        text_cache.erase(death_lazer_cache);
                    }

                    files_surface.clear();
                    for(unsigned int i = 0; i < open_files.size(); i++){
                        arial.drawPlain(&files_surface,filename_name(open_files[i]),16,2, i*24);
                    }
                    files_surface.update_texture = true;

                    if(current_file > 0) current_file--;

                    if(current_file < open_files.size() and current_file >= 0){
                        text = text_cache[current_file];
                    } else {
                        text = "";
                        code_surface.clear();
                        code_surface.update_texture = true;
                    }

                    cursor_position = 0;
                }
            }
        }
    }
    else if(room == ROOM_OPEN){
        if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
            int x, y;
            double xprec, yprec;

            glfwGetCursorPos(window,&xprec,&yprec);
            x = floor(xprec); y = floor(yprec);

            if(x < 24 and y <= 24){ // BACK
                room = ROOM_EDIT;
            }
            else if(x >= 48 and x < 72 and y <= 24){ // UP
                string location = filename_path(folder_location.substr(0,folder_location.length()-1));
                boost::filesystem::path folder(location);

                if(boost::filesystem::is_directory(folder) and location != "C:"){
                    location = string_replace_all(location,"\\","/");

                    file_list_yscroll = 0;
                    open_text = location;
                    folder_location = location;

                    update_file_list = true;
                }
            }
            else if(x >= 72 and x < 96 and y <= 24){ // REFRESH
                try {
                    file_list_yscroll = 0;
                    update_file_list = false;
                    file_list_surface.clear();

                    folders = folder_contents(folder_location);
                    for(int i = 0; i < (int)folders.size(); i++){
                        arial.drawPlain( &file_list_surface, filename_name(folders[i]), 16, 0, i*24 + (file_list_yscroll*48) );
                    }
                } catch(...){
                    update_file_list = true;
                    file_list_surface.clear();
                }
            }
            else if(y > 48){ // SELECT ITEM
                int item = (y - 48 - 2)/24 - file_list_yscroll*2;

                try {
                    if(item < (int)folders.size()){
                        boost::filesystem::path folder(folders[item]);

                        if(boost::filesystem::is_directory(folder)){
                            string location = string_replace_all(folders[item],"\\","/");

                            file_list_yscroll = 0;
                            folder_location = location;
                            open_text = location;

                            update_file_list = true;
                            file_list_surface.update_texture = true;
                        }
                        else {
                            ifstream read(folders[item]);

                            if(read.is_open()){
                                file_list_yscroll = 0;

                                if(text_cache.size() > 0){
                                    text_cache[current_file] = text;
                                }

                                string line;
                                text = "";
                                prev_text = "";

                                while( getline(read,line) ){
                                    text += string_replace_all(line,"\t","    ") + "\n";
                                }

                                read.close();
                                open_files.push_back(folders[item]);

                                files_surface.clear();
                                for(unsigned int i = 0; i < open_files.size(); i++){
                                    arial.drawPlain(&files_surface,filename_name(open_files[i]),16,2, i*24);
                                }

                                code_surface.clear();
                                arial.draw(&code_surface, text, "", 16, 2, 0);

                                line_number_surface.clear();
                                unsigned int line_number = 1;
                                arial.drawPlain(&line_number_surface,to_string(line_number),16,2, (line_number - 1)*16);
                                for(unsigned int i = 0; i < text.length(); i++){
                                    if(text[i] == '\n'){
                                        line_number++;
                                        arial.drawPlain(&line_number_surface,to_string(line_number),16,2, (line_number - 1)*16);
                                    }
                                }

                                text_cache.push_back(text);
                                current_file = text_cache.size()-1;
                            } else {
                                message = "Failed to Open File";
                                message_timer = 120;
                                room = ROOM_EDIT;
                            }

                            room = ROOM_EDIT;
                        }

                        cursor_position = 0;
                    }
                } catch(...){
                    message = "Failed to Open File";
                    message_timer = 120;
                    room = ROOM_EDIT;
                }
            }
        }
    }
    else if(room == ROOM_SAVE){
        if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
            int x, y;
            double xprec, yprec;

            glfwGetCursorPos(window,&xprec,&yprec);
            x = floor(xprec); y = floor(yprec);

            if(x < 24 and y <= 24){ // BACK
                room = ROOM_EDIT;
            }
            else if(x >= 48 and x < 72 and y <= 24){ // UP
                string location = filename_path(folder_location.substr(0,folder_location.length()-1));
                boost::filesystem::path folder(location);

                if(boost::filesystem::is_directory(folder) and location != "C:"){
                    location = string_replace_all(location,"\\","/");

                    file_list_yscroll = 0;
                    open_text = location;
                    folder_location = location;

                    update_file_list = true;
                }
            }
            else if(x >= 72 and x < 96 and y <= 24){ // REFRESH
                try {
                    file_list_yscroll = 0;
                    update_file_list = false;
                    file_list_surface.clear();

                    folders = folder_contents(folder_location);
                    for(int i = 0; i < (int)folders.size(); i++){
                        arial.drawPlain( &file_list_surface, filename_name(folders[i]), 16, 0, i*24 + (file_list_yscroll*48) );
                    }
                } catch(...){
                    update_file_list = true;
                    file_list_surface.clear();
                }
            }
            else if(y > 48){ // SELECT ITEM
                int item = (y - 48 - 2)/24 - file_list_yscroll*2;

                try {
                    if(item < (int)folders.size()){
                        boost::filesystem::path folder(folders[item]);

                        if(boost::filesystem::is_directory(folder)){
                            string location = string_replace_all(folders[item],"\\","/");

                            file_list_yscroll = 0;
                            folder_location = location;
                            open_text = location;

                            update_file_list = true;
                            file_list_surface.update_texture = true;
                        }
                        else {
                            ofstream write(folders[item]);

                            if(write.is_open()){
                                write << text;
                                write.close();

                                open_files[current_file] = folders[item];

                                files_surface.clear();
                                for(unsigned int i = 0; i < open_files.size(); i++){
                                    arial.drawPlain(&files_surface,filename_name(open_files[i]),16,2, i*24);
                                }

                                code_surface.clear();
                                arial.draw(&code_surface, text, "", 16, 2, 0);

                                line_number_surface.clear();
                                unsigned int line_number = 1;
                                arial.drawPlain(&line_number_surface,to_string(line_number),16,2, (line_number - 1)*16);
                                for(unsigned int i = 0; i < text.length(); i++){
                                    if(text[i] == '\n'){
                                        line_number++;
                                        arial.drawPlain(&line_number_surface,to_string(line_number),16,2, (line_number - 1)*16);
                                    }
                                }

                                message = "Successfully Saved";
                                message_timer = 120;
                            } else {
                                message = "Failed to Saved";
                                message_timer = 120;
                            }

                            room = ROOM_EDIT;
                        }

                        cursor_position = 0;
                    }
                } catch(...){
                    message = "Failed to Open File";
                    message_timer = 120;
                    room = ROOM_EDIT;
                }
            }
        }
    }
}

void mouse_scroll_callback(GLFWwindow* window, double x, double y){
    if(room == ROOM_EDIT){
    }
    else if(room == ROOM_OPEN){
        if( file_list_yscroll + y <= 0 and (file_list_yscroll + y)*2 > -(int)(folders.size() - 16) ){
            file_list_yscroll += y;
            update_file_list = true;
        }
    }
    else if(room == ROOM_SAVE){
        if( file_list_yscroll + y <= 0 and (file_list_yscroll + y)*2 > -(int)(folders.size() - 16) ){
            file_list_yscroll += y;
            update_file_list = true;
        }
    }
}
