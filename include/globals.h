
#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include <string>
#include <vector>
#include "font.h"
#include "surface.h"

#ifdef __WIN32__
    extern std::string USERNAME;
    #define HOME ("C:/Users/" + USERNAME + "/AppData/Roaming/BoomslangDevelopment/")
#endif // __WIN32__

#define RECENT_OPEN_FILENAME     "data/890214716401982610581702658037.dat"
#define RECENT_LOCATION_FILENAME "data/382059382729771463503168420291.dat"
#define HOME_NEW_IMAGE           "data/452905832916726114837291053892.dat"
#define HOME_OPEN_IMAGE          "data/021573264703314893701296046072.dat"
#define HOME_COMPILE_IMAGE       "data/630193629631037193628561814250.dat"
#define HOME_SAVE_IMAGE          "data/758291739127451974319411846384.dat"
#define HOME_CMD_IMAGE           "data/182630164091289648901447219962.dat"
#define HOME_RUN_IMAGE           "data/039277478128916319276493229102.dat"
#define HOME_BACK_IMAGE          "data/963025801677128412708918491347.dat"
#define HOME_UP_IMAGE            "data/318452890167045608568143906756.dat"
#define HOME_REFRESH_IMAGE       "data/619301648218631017382854021705.dat"
#define HOME_FONT                "InconsolataGo-Regular.ttf"

#define ROOM_EDIT 0
#define ROOM_OPEN 1
#define ROOM_SAVE 2

#define DELIMITERS_KEYWORD \
    ( i == 0 ? true : \
    text.substr(i-1,1) == "\n" \
    or text.substr(i-1,1) == " " \
    or text.substr(i-1,1) == "=" \
    or text.substr(i-1,1) == "+" \
    or text.substr(i-1,1) == "-" \
    or text.substr(i-1,1) == "*" \
    or text.substr(i-1,1) == "/" \
    or text.substr(i-1,1) == "," \
    or text.substr(i-1,1) == "!" \
    or text.substr(i-1,1) == "(" \
    or text.substr(i-1,1) == ")" \
    or text.substr(i-1,1) == "<" \
    or text.substr(i-1,1) == ">" \
    or text.substr(i-1,1) == ")" \
    or text.substr(i-1,1) == "^" \
    or text.substr(i-1,1) == "&" )

#define DELIMITERS_CLASS \
    ( i == 0 ? true : \
    text.substr(i-1,1) == "\n" \
    or text.substr(i-1,1) == " " \
    or text.substr(i-1,1) == "=" \
    or text.substr(i-1,1) == "+" \
    or text.substr(i-1,1) == "-" \
    or text.substr(i-1,1) == "*" \
    or text.substr(i-1,1) == "/" \
    or text.substr(i-1,1) == "," \
    or text.substr(i-1,1) == "!" \
    or text.substr(i-1,1) == "(" \
    or text.substr(i-1,1) == ")" \
    or text.substr(i-1,1) == "^" \
    or text.substr(i-1,1) == "&" \
    or text.substr(i-1,1) == "<" \
    or text.substr(i-1,1) == ">" \
    or text.substr(i-1,1) == "{" \
    or text.substr(i-1,1) == "}" )

extern GLFWwindow* window;
extern unsigned int room;
extern Font arial;
extern Surface code_surface;
extern std::string text;
extern std::vector<std::string> text_cache;
extern std::string open_text;
extern std::vector<std::string> open_files;
extern unsigned int current_file;
extern unsigned int cursor_position;
extern bool blink;
extern unsigned int blink_timer;
extern unsigned int blink_x, blink_y;
extern std::string message;
extern unsigned int message_timer;
extern std::string folder_location;
extern std::vector<std::string> folders;
extern bool update_file_list;
extern int file_list_yscroll;
extern std::string prev_text;
extern std::string prev_open_text;
extern Surface files_surface;
extern Surface line_number_surface;
extern Surface message_surface;
extern Surface open_surface;
extern Surface file_list_surface;

// Syntax

extern std::vector<std::string> keywords;
extern std::vector<std::string> operators;
extern std::vector<std::string> classes;

#endif // GLOBALS_H_INCLUDED
