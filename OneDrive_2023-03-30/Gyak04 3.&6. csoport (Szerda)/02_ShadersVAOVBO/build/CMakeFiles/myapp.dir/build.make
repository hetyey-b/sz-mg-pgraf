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
CMAKE_SOURCE_DIR = "/home/craken/Documents/Egyetem/számgépgraf/OneDrive_2023-03-30/Gyak04 3.&6. csoport (Szerda)/02_ShadersVAOVBO"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/craken/Documents/Egyetem/számgépgraf/OneDrive_2023-03-30/Gyak04 3.&6. csoport (Szerda)/02_ShadersVAOVBO/build"

# Include any dependencies generated for this target.
include CMakeFiles/myapp.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/myapp.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/myapp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/myapp.dir/flags.make

CMakeFiles/myapp.dir/02_ShadersVAOVBO/MyApp.cpp.o: CMakeFiles/myapp.dir/flags.make
CMakeFiles/myapp.dir/02_ShadersVAOVBO/MyApp.cpp.o: ../02_ShadersVAOVBO/MyApp.cpp
CMakeFiles/myapp.dir/02_ShadersVAOVBO/MyApp.cpp.o: CMakeFiles/myapp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/craken/Documents/Egyetem/számgépgraf/OneDrive_2023-03-30/Gyak04 3.&6. csoport (Szerda)/02_ShadersVAOVBO/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/myapp.dir/02_ShadersVAOVBO/MyApp.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/myapp.dir/02_ShadersVAOVBO/MyApp.cpp.o -MF CMakeFiles/myapp.dir/02_ShadersVAOVBO/MyApp.cpp.o.d -o CMakeFiles/myapp.dir/02_ShadersVAOVBO/MyApp.cpp.o -c "/home/craken/Documents/Egyetem/számgépgraf/OneDrive_2023-03-30/Gyak04 3.&6. csoport (Szerda)/02_ShadersVAOVBO/02_ShadersVAOVBO/MyApp.cpp"

CMakeFiles/myapp.dir/02_ShadersVAOVBO/MyApp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myapp.dir/02_ShadersVAOVBO/MyApp.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/craken/Documents/Egyetem/számgépgraf/OneDrive_2023-03-30/Gyak04 3.&6. csoport (Szerda)/02_ShadersVAOVBO/02_ShadersVAOVBO/MyApp.cpp" > CMakeFiles/myapp.dir/02_ShadersVAOVBO/MyApp.cpp.i

CMakeFiles/myapp.dir/02_ShadersVAOVBO/MyApp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myapp.dir/02_ShadersVAOVBO/MyApp.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/craken/Documents/Egyetem/számgépgraf/OneDrive_2023-03-30/Gyak04 3.&6. csoport (Szerda)/02_ShadersVAOVBO/02_ShadersVAOVBO/MyApp.cpp" -o CMakeFiles/myapp.dir/02_ShadersVAOVBO/MyApp.cpp.s

# Object files for target myapp
myapp_OBJECTS = \
"CMakeFiles/myapp.dir/02_ShadersVAOVBO/MyApp.cpp.o"

# External object files for target myapp
myapp_EXTERNAL_OBJECTS =

libmyapp.a: CMakeFiles/myapp.dir/02_ShadersVAOVBO/MyApp.cpp.o
libmyapp.a: CMakeFiles/myapp.dir/build.make
libmyapp.a: CMakeFiles/myapp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/craken/Documents/Egyetem/számgépgraf/OneDrive_2023-03-30/Gyak04 3.&6. csoport (Szerda)/02_ShadersVAOVBO/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libmyapp.a"
	$(CMAKE_COMMAND) -P CMakeFiles/myapp.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/myapp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/myapp.dir/build: libmyapp.a
.PHONY : CMakeFiles/myapp.dir/build

CMakeFiles/myapp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/myapp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/myapp.dir/clean

CMakeFiles/myapp.dir/depend:
	cd "/home/craken/Documents/Egyetem/számgépgraf/OneDrive_2023-03-30/Gyak04 3.&6. csoport (Szerda)/02_ShadersVAOVBO/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/craken/Documents/Egyetem/számgépgraf/OneDrive_2023-03-30/Gyak04 3.&6. csoport (Szerda)/02_ShadersVAOVBO" "/home/craken/Documents/Egyetem/számgépgraf/OneDrive_2023-03-30/Gyak04 3.&6. csoport (Szerda)/02_ShadersVAOVBO" "/home/craken/Documents/Egyetem/számgépgraf/OneDrive_2023-03-30/Gyak04 3.&6. csoport (Szerda)/02_ShadersVAOVBO/build" "/home/craken/Documents/Egyetem/számgépgraf/OneDrive_2023-03-30/Gyak04 3.&6. csoport (Szerda)/02_ShadersVAOVBO/build" "/home/craken/Documents/Egyetem/számgépgraf/OneDrive_2023-03-30/Gyak04 3.&6. csoport (Szerda)/02_ShadersVAOVBO/build/CMakeFiles/myapp.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/myapp.dir/depend

