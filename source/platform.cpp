
#include <string>
#include "../include/platform.h"

#ifdef __WIN32__
#include <windows.h>
#endif // __WIN32__

using namespace std;

void execute(string exe, string params){
    #ifdef __WIN32__
        string program = "\"" + exe + "\" " + params;

        STARTUPINFO startupInfo;
        PROCESS_INFORMATION processInfo;

        ZeroMemory( &startupInfo, sizeof(startupInfo) );
        startupInfo.cb = sizeof(startupInfo);
        ZeroMemory( &processInfo, sizeof(processInfo) );

        CreateProcessA( NULL,
        &program[0], // safe for CreateProcessA
        NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL,
        &startupInfo, &processInfo);
    #endif // __WIN32__
}
