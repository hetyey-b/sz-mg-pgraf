# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/craken/Documents/Egyetem/számgépgraf/bead1/01_Transforms

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/craken/Documents/Egyetem/számgépgraf/bead1/01_Transforms/build

# Include any dependencies generated for this target.
include CMakeFiles/CG_PROJ.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/CG_PROJ.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/CG_PROJ.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CG_PROJ.dir/flags.make

CMakeFiles/CG_PROJ.dir/01_Transforms/main.cpp.o: CMakeFiles/CG_PROJ.dir/flags.make
CMakeFiles/CG_PROJ.dir/01_Transforms/main.cpp.o: ../01_Transforms/main.cpp
CMakeFiles/CG_PROJ.dir/01_Transforms/main.cpp.o: CMakeFiles/CG_PROJ.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/craken/Documents/Egyetem/számgépgraf/bead1/01_Transforms/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CG_PROJ.dir/01_Transforms/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CG_PROJ.dir/01_Transforms/main.cpp.o -MF CMakeFiles/CG_PROJ.dir/01_Transforms/main.cpp.o.d -o CMakeFiles/CG_PROJ.dir/01_Transforms/main.cpp.o -c /home/craken/Documents/Egyetem/számgépgraf/bead1/01_Transforms/01_Transforms/main.cpp

CMakeFiles/CG_PROJ.dir/01_Transforms/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CG_PROJ.dir/01_Transforms/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/craken/Documents/Egyetem/számgépgraf/bead1/01_Transforms/01_Transforms/main.cpp > CMakeFiles/CG_PROJ.dir/01_Transforms/main.cpp.i

CMakeFiles/CG_PROJ.dir/01_Transforms/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CG_PROJ.dir/01_Transforms/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/craken/Documents/Egyetem/számgépgraf/bead1/01_Transforms/01_Transforms/main.cpp -o CMakeFiles/CG_PROJ.dir/01_Transforms/main.cpp.s

# Object files for target CG_PROJ
CG_PROJ_OBJECTS = \
"CMakeFiles/CG_PROJ.dir/01_Transforms/main.cpp.o"

# External object files for target CG_PROJ
CG_PROJ_EXTERNAL_OBJECTS =

CG_PROJ: CMakeFiles/CG_PROJ.dir/01_Transforms/main.cpp.o
CG_PROJ: CMakeFiles/CG_PROJ.dir/build.make
CG_PROJ: libmyapp.a
CG_PROJ: libimgui.a
CG_PROJ: libcamera.a
CG_PROJ: libcallback.a
CG_PROJ: libinlobjects.a
CG_PROJ: /usr/lib/x86_64-linux-gnu/libSDL2_image.so
CG_PROJ: /usr/lib/x86_64-linux-gnu/libGLEW.so
CG_PROJ: /usr/lib/x86_64-linux-gnu/libGL.so
CG_PROJ: /usr/lib/x86_64-linux-gnu/libGLU.so
CG_PROJ: CMakeFiles/CG_PROJ.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/craken/Documents/Egyetem/számgépgraf/bead1/01_Transforms/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable CG_PROJ"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CG_PROJ.dir/link.txt --verbose=$(VERBOSE)
	/usr/bin/cmake -E create_symlink /home/craken/Documents/Egyetem/számgépgraf/bead1/01_Transforms/01_Transforms/myFrag.frag /home/craken/Documents/Egyetem/számgépgraf/bead1/01_Transforms/build/myFrag.frag
	/usr/bin/cmake -E create_symlink /home/craken/Documents/Egyetem/számgépgraf/bead1/01_Transforms/01_Transforms/myVert.vert /home/craken/Documents/Egyetem/számgépgraf/bead1/01_Transforms/build/myVert.vert

# Rule to build all files generated by this target.
CMakeFiles/CG_PROJ.dir/build: CG_PROJ
.PHONY : CMakeFiles/CG_PROJ.dir/build

CMakeFiles/CG_PROJ.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CG_PROJ.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CG_PROJ.dir/clean

CMakeFiles/CG_PROJ.dir/depend:
	cd /home/craken/Documents/Egyetem/számgépgraf/bead1/01_Transforms/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/craken/Documents/Egyetem/számgépgraf/bead1/01_Transforms /home/craken/Documents/Egyetem/számgépgraf/bead1/01_Transforms /home/craken/Documents/Egyetem/számgépgraf/bead1/01_Transforms/build /home/craken/Documents/Egyetem/számgépgraf/bead1/01_Transforms/build /home/craken/Documents/Egyetem/számgépgraf/bead1/01_Transforms/build/CMakeFiles/CG_PROJ.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CG_PROJ.dir/depend

