
#include <string>
#include <vector>
#include "../include/font.h"
#include "../include/surface.h"
#include "../include/globals.h"

#ifdef __WIN32__
std::string USERNAME = getenv("USERNAME");
#endif // __WIN32__

GLFWwindow* window;
unsigned int room = ROOM_EDIT;
Font arial;
Surface code_surface(640 - 32,480 - 24);
std::string text;
std::vector<std::string> text_cache;
std::string open_text;
std::vector<std::string> open_files;
unsigned int current_file = 0;
unsigned int cursor_position;
bool blink = true;
unsigned int blink_timer = 30;
unsigned int blink_x, blink_y;
std::string message;
unsigned int message_timer;
std::string folder_location;
std::vector<std::string> folders;
bool update_file_list;
int file_list_yscroll;
std::string prev_text;
std::string prev_open_text;
Surface files_surface(160,480 - 24);
Surface line_number_surface(32,480 - 24);
Surface message_surface(160,24);
Surface open_surface(640,24);
Surface file_list_surface(640,480);

// Syntax

std::vector<std::string> keywords = {"on ","class ","import ","cast ","new ","create ","delete ","as ","catch","try","for ","in ",
    "true","false","if","unless","while","until","else","register ","method ","pure","self","none","return ","var","native ",
    "void","any","function ", "global ", "final "};
std::vector<std::string> operators = {"+","-","*","/","!","=","(",")",",","^","&",">","<"};
std::vector<std::string> classes = {"Rectangle","Window","String","Number","Positive","Boolean","List","Sprite","Image",
    "Error","Exception",
    "MemoryFailure","OutOfIndex","NotFound","Invalid","ZeroDivision", "Keystroke", "Object", "Numeric", "Byte", "Map",
    "Integer","UnsignedInteger"};
