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
CMAKE_SOURCE_DIR = /home/a12345/Desktop/DatosyRedes/Tarea1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/a12345/Desktop/DatosyRedes/Tarea1/build

# Include any dependencies generated for this target.
include CMakeFiles/Servidor.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Servidor.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Servidor.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Servidor.dir/flags.make

CMakeFiles/Servidor.dir/server.cpp.o: CMakeFiles/Servidor.dir/flags.make
CMakeFiles/Servidor.dir/server.cpp.o: ../server.cpp
CMakeFiles/Servidor.dir/server.cpp.o: CMakeFiles/Servidor.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/a12345/Desktop/DatosyRedes/Tarea1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Servidor.dir/server.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Servidor.dir/server.cpp.o -MF CMakeFiles/Servidor.dir/server.cpp.o.d -o CMakeFiles/Servidor.dir/server.cpp.o -c /home/a12345/Desktop/DatosyRedes/Tarea1/server.cpp

CMakeFiles/Servidor.dir/server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Servidor.dir/server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/a12345/Desktop/DatosyRedes/Tarea1/server.cpp > CMakeFiles/Servidor.dir/server.cpp.i

CMakeFiles/Servidor.dir/server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Servidor.dir/server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/a12345/Desktop/DatosyRedes/Tarea1/server.cpp -o CMakeFiles/Servidor.dir/server.cpp.s

# Object files for target Servidor
Servidor_OBJECTS = \
"CMakeFiles/Servidor.dir/server.cpp.o"

# External object files for target Servidor
Servidor_EXTERNAL_OBJECTS =

Servidor: CMakeFiles/Servidor.dir/server.cpp.o
Servidor: CMakeFiles/Servidor.dir/build.make
Servidor: CMakeFiles/Servidor.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/a12345/Desktop/DatosyRedes/Tarea1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Servidor"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Servidor.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Servidor.dir/build: Servidor
.PHONY : CMakeFiles/Servidor.dir/build

CMakeFiles/Servidor.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Servidor.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Servidor.dir/clean

CMakeFiles/Servidor.dir/depend:
	cd /home/a12345/Desktop/DatosyRedes/Tarea1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/a12345/Desktop/DatosyRedes/Tarea1 /home/a12345/Desktop/DatosyRedes/Tarea1 /home/a12345/Desktop/DatosyRedes/Tarea1/build /home/a12345/Desktop/DatosyRedes/Tarea1/build /home/a12345/Desktop/DatosyRedes/Tarea1/build/CMakeFiles/Servidor.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Servidor.dir/depend

