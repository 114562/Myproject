# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_SOURCE_DIR = /home/yjx/Mine/Myproject

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yjx/Mine/Myproject/build

# Include any dependencies generated for this target.
include CMakeFiles/client.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/client.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/client.dir/flags.make

CMakeFiles/client.dir/src/conc_tcp_cli_echo.c.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/conc_tcp_cli_echo.c.o: /home/yjx/Mine/Myproject/src/conc_tcp_cli_echo.c
CMakeFiles/client.dir/src/conc_tcp_cli_echo.c.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yjx/Mine/Myproject/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/client.dir/src/conc_tcp_cli_echo.c.o"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/client.dir/src/conc_tcp_cli_echo.c.o -MF CMakeFiles/client.dir/src/conc_tcp_cli_echo.c.o.d -o CMakeFiles/client.dir/src/conc_tcp_cli_echo.c.o -c /home/yjx/Mine/Myproject/src/conc_tcp_cli_echo.c

CMakeFiles/client.dir/src/conc_tcp_cli_echo.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/src/conc_tcp_cli_echo.c.i"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/yjx/Mine/Myproject/src/conc_tcp_cli_echo.c > CMakeFiles/client.dir/src/conc_tcp_cli_echo.c.i

CMakeFiles/client.dir/src/conc_tcp_cli_echo.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/src/conc_tcp_cli_echo.c.s"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/yjx/Mine/Myproject/src/conc_tcp_cli_echo.c -o CMakeFiles/client.dir/src/conc_tcp_cli_echo.c.s

# Object files for target client
client_OBJECTS = \
"CMakeFiles/client.dir/src/conc_tcp_cli_echo.c.o"

# External object files for target client
client_EXTERNAL_OBJECTS =

client: CMakeFiles/client.dir/src/conc_tcp_cli_echo.c.o
client: CMakeFiles/client.dir/build.make
client: CMakeFiles/client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yjx/Mine/Myproject/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/client.dir/build: client
.PHONY : CMakeFiles/client.dir/build

CMakeFiles/client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/client.dir/clean

CMakeFiles/client.dir/depend:
	cd /home/yjx/Mine/Myproject/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yjx/Mine/Myproject /home/yjx/Mine/Myproject /home/yjx/Mine/Myproject/build /home/yjx/Mine/Myproject/build /home/yjx/Mine/Myproject/build/CMakeFiles/client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/client.dir/depend
