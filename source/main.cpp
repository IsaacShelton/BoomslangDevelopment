
#include <string>
#include <stdlib.h>
#include <iostream>
#include <GLFW/glfw3.h>

#include "../include/font.h"
#include "../include/image.h"
#include "../include/surface.h"
#include "../include/globals.h"
#include "../include/callbacks.h"
#include "../include/conversion.h"
#include "../include/strings.h"
#include "../include/storage.h"
#include "../include/files.h"

using namespace std;

int main(int argc, char** argv){
    { // Open Recent Files
        BasicStorageReader file(HOME + RECENT_OPEN_FILENAME);
        open_files = file.get();
        current_file = open_files.size()-1;
        file.close();
    }

    { // Open Recent Location
        vector<string> locations;

        BasicStorageReader file(HOME + RECENT_LOCATION_FILENAME);
        locations = file.get();
        file.close();


        if(locations.size() > 0){
            folder_location = locations[0];
            open_text = locations[0];

            folders = folder_contents(folder_location);
        } else {
            open_text = "C:/Users/" + USERNAME + "/";
            folder_location = "C:/Users/" + USERNAME + "/";
            folders = folder_contents(folder_location);
        }
    }

    { // Open files specified by arguments
        for(int i = 1; i < argc; i++){
            bool already_exists = false;

            for(unsigned int j = 0; j < open_files.size(); j++){
                if(open_files[j] == argv[i]){
                    already_exists = true;
                    current_file = j;
                    cout << j << endl;
                    break;
                }
            }

            if(!already_exists){
                open_files.push_back(argv[i]);
                current_file = open_files.size()-1;
            }
        }
    }

    if(open_files.size() == 0){
        open_files.push_back("untitled");
        current_file = 0;
    }

    // Load Opened Files
    for(unsigned int i = 0; i < open_files.size(); i++){
        if(open_files[i] != "untitled"){
            ifstream read(open_files[i]);
            string data;
            string line;

            if(read.is_open()){
                while( getline(read,line) ){
                    data += string_replace_all(line,"\t","    ") + "\n";
                }

                read.close();
            } else {
                message = "Failed to Open File";
                message_timer = 120;
            }

            text_cache.push_back(data);

            if(current_file == i){
                text = data;
            }
        } else {
            text_cache.push_back("");
            if(current_file == i){
                text = "";
            }
        }
    }

    glfwSetErrorCallback(error_callback);

    //Init glfw
    if (!glfwInit()) exit(EXIT_FAILURE);

    window = glfwCreateWindow(640, 480, "Boomslang Development", NULL, NULL);

    //Ensure window exists
    if (!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCharCallback(window, character_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);

    arial.load(HOME + HOME_FONT);
    cursor_position = 0;

    glClearColor(0.4,0.4,0.4,1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    Image new_image;
    Image open_image;
    Image compile_image;
    Image save_image;
    Image run_image;
    Image cmd_image;
    Image back_image;
    Image up_image;
    Image refresh_image;

    {
        PngReader reader(HOME + HOME_NEW_IMAGE);
        new_image.load( reader.get(), 24, 24);
        reader.close();
    }

    {
        PngReader reader(HOME + HOME_OPEN_IMAGE);
        open_image.load( reader.get(), 24, 24);
        reader.close();
    }

    {
        PngReader reader(HOME + HOME_COMPILE_IMAGE);
        compile_image.load( reader.get(), 24, 24);
        reader.close();
    }

    {
        PngReader reader(HOME + HOME_SAVE_IMAGE);
        save_image.load( reader.get(), 24, 24);
        reader.close();
    }

    {
        PngReader reader(HOME + HOME_RUN_IMAGE);
        run_image.load( reader.get(), 24, 24);
        reader.close();
    }

    {
        PngReader reader(HOME + HOME_CMD_IMAGE);
        cmd_image.load( reader.get(), 24, 24);
        reader.close();
    }

    {
        PngReader reader(HOME + HOME_BACK_IMAGE);
        back_image.load( reader.get(), 24, 24);
        reader.close();
    }

    {
        PngReader reader(HOME + HOME_UP_IMAGE);
        up_image.load( reader.get(), 24, 24);
        reader.close();
    }

    {
        PngReader reader(HOME + HOME_REFRESH_IMAGE);
        refresh_image.load( reader.get(), 24, 24);
        reader.close();
    }

    for(unsigned int i = 0; i < open_files.size(); i++){
        arial.drawPlain(&files_surface,filename_name(open_files[i]),16,2, i*24);
    }

    { // Draw line numbers
        line_number_surface.clear();
        unsigned int line_number = 1;
        arial.drawPlain(&line_number_surface,to_string(line_number),16,2, (line_number - 1)*16);
        for(unsigned int i = 0; i < text.length(); i++){
            if(text[i] == '\n'){
                line_number++;
                arial.drawPlain(&line_number_surface,to_string(line_number),16,2, (line_number - 1)*16);
            }
        }
    }

    // Draw folders in location
    for(int i = 0; i < (int)folders.size(); i++){
        arial.drawPlain(&file_list_surface, filename_name(folders[i]), 16, 0, i*24);
    }

    int prev_width = 640, prev_height = 480;

    while (!glfwWindowShouldClose(window)){
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        if(width != prev_width or height!=prev_height){
            if(width - 32 > 0 and height - 24 > 0){
                code_surface.resize(width - 32, height - 24);
                files_surface.resize(160, height - 24);
                line_number_surface.resize(32, height-24);
                open_surface.resize(width, 24);
                file_list_surface.resize(width, height - 24);
            } else {
                code_surface.resize(1, 1);
                files_surface.resize(160, 1);
                line_number_surface.resize(32, 1);
                open_surface.resize(1, 1);
                file_list_surface.resize(1, 1);
            }
            code_surface.clear();
            files_surface.clear();
            file_list_surface.clear();
            open_surface.clear();
            prev_open_text = "";

            // Draw file names
            for(unsigned int i = 0; i < open_files.size(); i++){
                arial.drawPlain(&files_surface,filename_name(open_files[i]),16,2, i*24);
            }

            arial.draw(&code_surface, text, "", 16, 2, 0);

            vector<string> folders = folder_contents(folder_location);
            for(int i = 0; i < (int)folders.size(); i++){
                arial.drawPlain( &file_list_surface, filename_name(folders[i]), 16, 0, i*24 + (file_list_yscroll*48) );
            }

            { // Draw Line Numbers
                line_number_surface.clear();
                unsigned int line_number = 1;
                arial.drawPlain(&line_number_surface,to_string(line_number),16,2, (line_number - 1)*16);
                for(unsigned int i = 0; i < text.length(); i++){
                    if(text[i] == '\n'){
                        line_number++;
                        arial.drawPlain(&line_number_surface,to_string(line_number),16,2, (line_number - 1)*16);
                    }
                }
            }
        }

        // Look at area
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glOrtho(0, width, height, 0, 1, -1); //lrbtnf

        // Draw room
        if(room == ROOM_EDIT){
            if(blink_timer==0){
                blink_timer = 30;
                if(blink) blink = false; else blink = true;
            }

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            if(text!=prev_text){
                if(prev_text.length() < text.length() and text.substr(0,prev_text.length())==prev_text and prev_text!=""){
                    arial.drawNew(&code_surface, text, prev_text, 16, 2, 0);
                } else {
                    code_surface.clear();
                    arial.draw(&code_surface, text, "", 16, 2, 0);
                }

                line_number_surface.clear();
                unsigned int line_number = 1;
                arial.drawPlain(&line_number_surface,to_string(line_number),16,2, (line_number - 1)*16);
                for(unsigned int i = 0; i < text.length(); i++){
                    if(text[i] == '\n'){
                        line_number++;
                        arial.drawPlain(&line_number_surface,to_string(line_number),16,2, (line_number - 1)*16);
                    }
                }
            }

            if(message != ""){
                message_surface.clear();
                arial.drawPlain(&message_surface,message,16,0,0);
                message = "";
            }

            if(open_files.size() > 0){
                code_surface.draw(32 + 160,24);
            }

            if(blink and open_files.size() > 0){
                glDisable(GL_TEXTURE_2D);
                glBegin(GL_QUADS);
                glVertex2i(blink_x + 1 + 32 + 160, blink_y + 3 + 24);
                glVertex2i(blink_x + 3 + 32 + 160, blink_y + 3 + 24);
                glVertex2i(blink_x + 3 + 32 + 160, blink_y + 21 + 24);
                glVertex2i(blink_x + 1 + 32 + 160, blink_y + 21 + 24);
                glEnd();
            }

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glColor3ub(144,144,144);
            glBegin(GL_QUADS);
                glVertex2i(0 + 160,24);
                glVertex2i(32 + 160,24);
                glVertex2i(32 + 160,height);
                glVertex2i(0 + 160,height);
            glEnd();

            glColor3ub(0,234,0);
            glBegin(GL_QUADS);
                glVertex2i(0,0);
                glVertex2i(width,0);
                glVertex2i(width,24);
                glVertex2i(0,24);
            glEnd();

            glColor3ub(160,160,160);
            glBegin(GL_QUADS);
                glVertex2i(0,24);
                glVertex2i(160,24);
                glVertex2i(160,height);
                glVertex2i(0,height);
            glEnd();

            if(current_file < open_files.size()){
                glColor4ub(0,255,0,128);
                glBegin(GL_QUADS);
                    glVertex2i(0,24 + 24*current_file);
                    glVertex2i(160,24 + 24*current_file);
                    glVertex2i(160,24 + 24 + 24*current_file);
                    glVertex2i(0,24 + 24 + 24*current_file);
                glEnd();
            }

            glColor3ub(255,255,255);
            glBlendFunc(GL_ONE, GL_ONE);

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            new_image.draw(0,0);
            open_image.draw(24,0);
            save_image.draw(48,0);
            compile_image.draw(96,0);
            run_image.draw(120,0);
            cmd_image.draw(144,0);

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            files_surface.draw(0,24);

            if(open_files.size() > 0){
                line_number_surface.draw(160,24);
            }

            if(message_timer > 0){
                message_surface.draw(184,0, (message_timer > 30 ? 255 : message_timer*8) );
                message_timer--;
            }

            blink_timer--;
        }
        else if(room == ROOM_OPEN){
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            if(open_text != prev_open_text){
                open_surface.clear();
                arial.drawPlain(&open_surface,open_text,16,0,0);
            }

            if(update_file_list){
                try {
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

            glColor3ub(0,234,0);
            glBegin(GL_QUADS);
                glVertex2i(0,0);
                glVertex2i(width,0);
                glVertex2i(width,24);
                glVertex2i(0,24);
            glEnd();

            back_image.draw(0,0);
            up_image.draw(48,0);
            refresh_image.draw(72,0);

            open_surface.draw(2,2 + 24);
            file_list_surface.draw(2 + 16,2 + 48);
            prev_open_text = open_text;
        }
        else if(room == ROOM_SAVE){
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            if(open_text != prev_open_text){
                open_surface.clear();
                arial.drawPlain(&open_surface,open_text,16,0,0);
            }

            if(update_file_list){
                try {
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

            glColor3ub(0,234,0);
            glBegin(GL_QUADS);
                glVertex2i(0,0);
                glVertex2i(width,0);
                glVertex2i(width,24);
                glVertex2i(0,24);
            glEnd();

            back_image.draw(0,0);
            up_image.draw(48,0);
            refresh_image.draw(72,0);

            open_surface.draw(2,2 + 24);
            file_list_surface.draw(2 + 16,2 + 48);
            prev_open_text = open_text;
        }

        // Record data
        prev_width = width;
        prev_height = height;
        prev_text = text;

        // Update window
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    { // Store Open Files
        BasicStorageWriter write(HOME + RECENT_OPEN_FILENAME, false);
        for(string item : open_files){
            if(item != "untitled"){
                write.add(item);
            }
        }
        write.close();
    }

    { // Store Location
        BasicStorageWriter write(HOME + RECENT_LOCATION_FILENAME, false);
        write.add(folder_location);
        write.close();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
