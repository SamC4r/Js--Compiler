# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

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
CMAKE_SOURCE_DIR = /home/sam/Documents/compiler/Js--Compiler

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sam/Documents/compiler/Js--Compiler/build

# Include any dependencies generated for this target.
include CMakeFiles/JsCompiler.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/JsCompiler.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/JsCompiler.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/JsCompiler.dir/flags.make

CMakeFiles/JsCompiler.dir/include/principal.cpp.o: CMakeFiles/JsCompiler.dir/flags.make
CMakeFiles/JsCompiler.dir/include/principal.cpp.o: /home/sam/Documents/compiler/Js--Compiler/include/principal.cpp
CMakeFiles/JsCompiler.dir/include/principal.cpp.o: CMakeFiles/JsCompiler.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/sam/Documents/compiler/Js--Compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/JsCompiler.dir/include/principal.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/JsCompiler.dir/include/principal.cpp.o -MF CMakeFiles/JsCompiler.dir/include/principal.cpp.o.d -o CMakeFiles/JsCompiler.dir/include/principal.cpp.o -c /home/sam/Documents/compiler/Js--Compiler/include/principal.cpp

CMakeFiles/JsCompiler.dir/include/principal.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/JsCompiler.dir/include/principal.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sam/Documents/compiler/Js--Compiler/include/principal.cpp > CMakeFiles/JsCompiler.dir/include/principal.cpp.i

CMakeFiles/JsCompiler.dir/include/principal.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/JsCompiler.dir/include/principal.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sam/Documents/compiler/Js--Compiler/include/principal.cpp -o CMakeFiles/JsCompiler.dir/include/principal.cpp.s

CMakeFiles/JsCompiler.dir/include/analizador_lexico.cpp.o: CMakeFiles/JsCompiler.dir/flags.make
CMakeFiles/JsCompiler.dir/include/analizador_lexico.cpp.o: /home/sam/Documents/compiler/Js--Compiler/include/analizador_lexico.cpp
CMakeFiles/JsCompiler.dir/include/analizador_lexico.cpp.o: CMakeFiles/JsCompiler.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/sam/Documents/compiler/Js--Compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/JsCompiler.dir/include/analizador_lexico.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/JsCompiler.dir/include/analizador_lexico.cpp.o -MF CMakeFiles/JsCompiler.dir/include/analizador_lexico.cpp.o.d -o CMakeFiles/JsCompiler.dir/include/analizador_lexico.cpp.o -c /home/sam/Documents/compiler/Js--Compiler/include/analizador_lexico.cpp

CMakeFiles/JsCompiler.dir/include/analizador_lexico.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/JsCompiler.dir/include/analizador_lexico.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sam/Documents/compiler/Js--Compiler/include/analizador_lexico.cpp > CMakeFiles/JsCompiler.dir/include/analizador_lexico.cpp.i

CMakeFiles/JsCompiler.dir/include/analizador_lexico.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/JsCompiler.dir/include/analizador_lexico.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sam/Documents/compiler/Js--Compiler/include/analizador_lexico.cpp -o CMakeFiles/JsCompiler.dir/include/analizador_lexico.cpp.s

CMakeFiles/JsCompiler.dir/include/TablaSimbolos.cpp.o: CMakeFiles/JsCompiler.dir/flags.make
CMakeFiles/JsCompiler.dir/include/TablaSimbolos.cpp.o: /home/sam/Documents/compiler/Js--Compiler/include/TablaSimbolos.cpp
CMakeFiles/JsCompiler.dir/include/TablaSimbolos.cpp.o: CMakeFiles/JsCompiler.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/sam/Documents/compiler/Js--Compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/JsCompiler.dir/include/TablaSimbolos.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/JsCompiler.dir/include/TablaSimbolos.cpp.o -MF CMakeFiles/JsCompiler.dir/include/TablaSimbolos.cpp.o.d -o CMakeFiles/JsCompiler.dir/include/TablaSimbolos.cpp.o -c /home/sam/Documents/compiler/Js--Compiler/include/TablaSimbolos.cpp

CMakeFiles/JsCompiler.dir/include/TablaSimbolos.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/JsCompiler.dir/include/TablaSimbolos.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sam/Documents/compiler/Js--Compiler/include/TablaSimbolos.cpp > CMakeFiles/JsCompiler.dir/include/TablaSimbolos.cpp.i

CMakeFiles/JsCompiler.dir/include/TablaSimbolos.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/JsCompiler.dir/include/TablaSimbolos.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sam/Documents/compiler/Js--Compiler/include/TablaSimbolos.cpp -o CMakeFiles/JsCompiler.dir/include/TablaSimbolos.cpp.s

CMakeFiles/JsCompiler.dir/include/ColaTablaSimbolos.cpp.o: CMakeFiles/JsCompiler.dir/flags.make
CMakeFiles/JsCompiler.dir/include/ColaTablaSimbolos.cpp.o: /home/sam/Documents/compiler/Js--Compiler/include/ColaTablaSimbolos.cpp
CMakeFiles/JsCompiler.dir/include/ColaTablaSimbolos.cpp.o: CMakeFiles/JsCompiler.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/sam/Documents/compiler/Js--Compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/JsCompiler.dir/include/ColaTablaSimbolos.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/JsCompiler.dir/include/ColaTablaSimbolos.cpp.o -MF CMakeFiles/JsCompiler.dir/include/ColaTablaSimbolos.cpp.o.d -o CMakeFiles/JsCompiler.dir/include/ColaTablaSimbolos.cpp.o -c /home/sam/Documents/compiler/Js--Compiler/include/ColaTablaSimbolos.cpp

CMakeFiles/JsCompiler.dir/include/ColaTablaSimbolos.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/JsCompiler.dir/include/ColaTablaSimbolos.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sam/Documents/compiler/Js--Compiler/include/ColaTablaSimbolos.cpp > CMakeFiles/JsCompiler.dir/include/ColaTablaSimbolos.cpp.i

CMakeFiles/JsCompiler.dir/include/ColaTablaSimbolos.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/JsCompiler.dir/include/ColaTablaSimbolos.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sam/Documents/compiler/Js--Compiler/include/ColaTablaSimbolos.cpp -o CMakeFiles/JsCompiler.dir/include/ColaTablaSimbolos.cpp.s

# Object files for target JsCompiler
JsCompiler_OBJECTS = \
"CMakeFiles/JsCompiler.dir/include/principal.cpp.o" \
"CMakeFiles/JsCompiler.dir/include/analizador_lexico.cpp.o" \
"CMakeFiles/JsCompiler.dir/include/TablaSimbolos.cpp.o" \
"CMakeFiles/JsCompiler.dir/include/ColaTablaSimbolos.cpp.o"

# External object files for target JsCompiler
JsCompiler_EXTERNAL_OBJECTS =

JsCompiler: CMakeFiles/JsCompiler.dir/include/principal.cpp.o
JsCompiler: CMakeFiles/JsCompiler.dir/include/analizador_lexico.cpp.o
JsCompiler: CMakeFiles/JsCompiler.dir/include/TablaSimbolos.cpp.o
JsCompiler: CMakeFiles/JsCompiler.dir/include/ColaTablaSimbolos.cpp.o
JsCompiler: CMakeFiles/JsCompiler.dir/build.make
JsCompiler: CMakeFiles/JsCompiler.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/sam/Documents/compiler/Js--Compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable JsCompiler"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/JsCompiler.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/JsCompiler.dir/build: JsCompiler
.PHONY : CMakeFiles/JsCompiler.dir/build

CMakeFiles/JsCompiler.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/JsCompiler.dir/cmake_clean.cmake
.PHONY : CMakeFiles/JsCompiler.dir/clean

CMakeFiles/JsCompiler.dir/depend:
	cd /home/sam/Documents/compiler/Js--Compiler/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sam/Documents/compiler/Js--Compiler /home/sam/Documents/compiler/Js--Compiler /home/sam/Documents/compiler/Js--Compiler/build /home/sam/Documents/compiler/Js--Compiler/build /home/sam/Documents/compiler/Js--Compiler/build/CMakeFiles/JsCompiler.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/JsCompiler.dir/depend

