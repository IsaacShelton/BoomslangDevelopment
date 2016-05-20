#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.147                       #
#------------------------------------------------------------------------------#


WORKDIR = %cd%

CC = gcc.exe
CXX = g++.exe
AR = ar.exe
LD = g++.exe
WINDRES = windres.exe

INC = -IC:\\Program\ Files\ (x86)\\CodeBlocks\\MinGW\\include\\freetype\\include
CFLAGS = -std=c++11 -Wall -DBOOST_SYSTEM_NO_DEPRECATED
RESINC = 
LIBDIR = 
LIB = C:\\Users\\isaac\\Downloads\\glfw-3.1.2.bin.WIN32\\glfw-3.1.2.bin.WIN32\\lib-mingw\\libglfw3.a C:\\Program\ Files\ (x86)\\CodeBlocks\\MinGW\\lib\\libopengl32.a C:\\Program\ Files\ (x86)\\CodeBlocks\\MinGW\\lib\\libgdi32.a C:\\Program\ Files\ (x86)\\CodeBlocks\\MinGW\\lib\\libglu32.a C:\\Program\ Files\ (x86)\\CodeBlocks\\MinGW\\lib\\freetype.a C:\\Program\ Files\ (x86)\\CodeBlocks\\MinGW\\lib\\boost\\libboost_system-mgw47-mt-1_60.a C:\\Program\ Files\ (x86)\\CodeBlocks\\MinGW\\lib\\boost\\libboost_filesystem-mgw47-mt-1_60.a C:\\Program\ Files\ (x86)\\CodeBlocks\\MinGW\\lib\\boost\\libboost_system-mgw47-mt-d-1_60.a
LDFLAGS = 

INC_DEBUG = $(INC)
CFLAGS_DEBUG = $(CFLAGS) -g
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS)
OBJDIR_DEBUG = obj\\Debug
DEP_DEBUG = 
OUT_DEBUG = bin\\Debug\\BoomslangDevelopment.exe

INC_RELEASE = $(INC)
CFLAGS_RELEASE = $(CFLAGS) -O3
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE = obj\\Release
DEP_RELEASE = 
OUT_RELEASE = bin\\Release\\BoomslangDevelopment.exe

OBJ_DEBUG = $(OBJDIR_DEBUG)\\source\\surface.o $(OBJDIR_DEBUG)\\source\\strings.o $(OBJDIR_DEBUG)\\source\\storage.o $(OBJDIR_DEBUG)\\source\\platform.o $(OBJDIR_DEBUG)\\source\\main.o $(OBJDIR_DEBUG)\\source\\image.o $(OBJDIR_DEBUG)\\source\\globals.o $(OBJDIR_DEBUG)\\source\\font.o $(OBJDIR_DEBUG)\\source\\files.o $(OBJDIR_DEBUG)\\source\\conversion.o $(OBJDIR_DEBUG)\\source\\callbacks.o $(OBJDIR_DEBUG)\\resource\\icon.o

OBJ_RELEASE = $(OBJDIR_RELEASE)\\source\\surface.o $(OBJDIR_RELEASE)\\source\\strings.o $(OBJDIR_RELEASE)\\source\\storage.o $(OBJDIR_RELEASE)\\source\\platform.o $(OBJDIR_RELEASE)\\source\\main.o $(OBJDIR_RELEASE)\\source\\image.o $(OBJDIR_RELEASE)\\source\\globals.o $(OBJDIR_RELEASE)\\source\\font.o $(OBJDIR_RELEASE)\\source\\files.o $(OBJDIR_RELEASE)\\source\\conversion.o $(OBJDIR_RELEASE)\\source\\callbacks.o $(OBJDIR_RELEASE)\\resource\\icon.o

all: debug release

clean: clean_debug clean_release

before_debug: 
	cmd /c if not exist bin\\Debug md bin\\Debug
	cmd /c if not exist $(OBJDIR_DEBUG)\\source md $(OBJDIR_DEBUG)\\source
	cmd /c if not exist $(OBJDIR_DEBUG)\\resource md $(OBJDIR_DEBUG)\\resource

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) $(LIBDIR_DEBUG) -o $(OUT_DEBUG) $(OBJ_DEBUG)  $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)\\source\\surface.o: source\\surface.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c source\\surface.cpp -o $(OBJDIR_DEBUG)\\source\\surface.o

$(OBJDIR_DEBUG)\\source\\strings.o: source\\strings.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c source\\strings.cpp -o $(OBJDIR_DEBUG)\\source\\strings.o

$(OBJDIR_DEBUG)\\source\\storage.o: source\\storage.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c source\\storage.cpp -o $(OBJDIR_DEBUG)\\source\\storage.o

$(OBJDIR_DEBUG)\\source\\platform.o: source\\platform.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c source\\platform.cpp -o $(OBJDIR_DEBUG)\\source\\platform.o

$(OBJDIR_DEBUG)\\source\\main.o: source\\main.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c source\\main.cpp -o $(OBJDIR_DEBUG)\\source\\main.o

$(OBJDIR_DEBUG)\\source\\image.o: source\\image.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c source\\image.cpp -o $(OBJDIR_DEBUG)\\source\\image.o

$(OBJDIR_DEBUG)\\source\\globals.o: source\\globals.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c source\\globals.cpp -o $(OBJDIR_DEBUG)\\source\\globals.o

$(OBJDIR_DEBUG)\\source\\font.o: source\\font.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c source\\font.cpp -o $(OBJDIR_DEBUG)\\source\\font.o

$(OBJDIR_DEBUG)\\source\\files.o: source\\files.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c source\\files.cpp -o $(OBJDIR_DEBUG)\\source\\files.o

$(OBJDIR_DEBUG)\\source\\conversion.o: source\\conversion.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c source\\conversion.cpp -o $(OBJDIR_DEBUG)\\source\\conversion.o

$(OBJDIR_DEBUG)\\source\\callbacks.o: source\\callbacks.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c source\\callbacks.cpp -o $(OBJDIR_DEBUG)\\source\\callbacks.o

$(OBJDIR_DEBUG)\\resource\\icon.o: resource\\icon.rc
	$(WINDRES) -i resource\\icon.rc -J rc -o $(OBJDIR_DEBUG)\\resource\\icon.o -O coff $(INC_DEBUG)

clean_debug: 
	cmd /c del /f $(OBJ_DEBUG) $(OUT_DEBUG)
	cmd /c rd bin\\Debug
	cmd /c rd $(OBJDIR_DEBUG)\\source
	cmd /c rd $(OBJDIR_DEBUG)\\resource

before_release: 
	cmd /c if not exist bin\\Release md bin\\Release
	cmd /c if not exist $(OBJDIR_RELEASE)\\source md $(OBJDIR_RELEASE)\\source
	cmd /c if not exist $(OBJDIR_RELEASE)\\resource md $(OBJDIR_RELEASE)\\resource

after_release: 

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(LIBDIR_RELEASE) -o $(OUT_RELEASE) $(OBJ_RELEASE)  $(LDFLAGS_RELEASE) -mwindows $(LIB_RELEASE)

$(OBJDIR_RELEASE)\\source\\surface.o: source\\surface.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c source\\surface.cpp -o $(OBJDIR_RELEASE)\\source\\surface.o

$(OBJDIR_RELEASE)\\source\\strings.o: source\\strings.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c source\\strings.cpp -o $(OBJDIR_RELEASE)\\source\\strings.o

$(OBJDIR_RELEASE)\\source\\storage.o: source\\storage.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c source\\storage.cpp -o $(OBJDIR_RELEASE)\\source\\storage.o

$(OBJDIR_RELEASE)\\source\\platform.o: source\\platform.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c source\\platform.cpp -o $(OBJDIR_RELEASE)\\source\\platform.o

$(OBJDIR_RELEASE)\\source\\main.o: source\\main.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c source\\main.cpp -o $(OBJDIR_RELEASE)\\source\\main.o

$(OBJDIR_RELEASE)\\source\\image.o: source\\image.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c source\\image.cpp -o $(OBJDIR_RELEASE)\\source\\image.o

$(OBJDIR_RELEASE)\\source\\globals.o: source\\globals.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c source\\globals.cpp -o $(OBJDIR_RELEASE)\\source\\globals.o

$(OBJDIR_RELEASE)\\source\\font.o: source\\font.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c source\\font.cpp -o $(OBJDIR_RELEASE)\\source\\font.o

$(OBJDIR_RELEASE)\\source\\files.o: source\\files.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c source\\files.cpp -o $(OBJDIR_RELEASE)\\source\\files.o

$(OBJDIR_RELEASE)\\source\\conversion.o: source\\conversion.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c source\\conversion.cpp -o $(OBJDIR_RELEASE)\\source\\conversion.o

$(OBJDIR_RELEASE)\\source\\callbacks.o: source\\callbacks.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c source\\callbacks.cpp -o $(OBJDIR_RELEASE)\\source\\callbacks.o

$(OBJDIR_RELEASE)\\resource\\icon.o: resource\\icon.rc
	$(WINDRES) -i resource\\icon.rc -J rc -o $(OBJDIR_RELEASE)\\resource\\icon.o -O coff $(INC_RELEASE)

clean_release: 
	cmd /c del /f $(OBJ_RELEASE) $(OUT_RELEASE)
	cmd /c rd bin\\Release
	cmd /c rd $(OBJDIR_RELEASE)\\source
	cmd /c rd $(OBJDIR_RELEASE)\\resource

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

