# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/clion/138/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/138/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/lhx/CLionProjects/myblockchain

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lhx/CLionProjects/myblockchain/cmake-build-debug

# Include any dependencies generated for this target.
include src/block_server/CMakeFiles/test.dir/depend.make

# Include the progress variables for this target.
include src/block_server/CMakeFiles/test.dir/progress.make

# Include the compile flags for this target's objects.
include src/block_server/CMakeFiles/test.dir/flags.make

src/block_server/CMakeFiles/test.dir/test.cpp.o: src/block_server/CMakeFiles/test.dir/flags.make
src/block_server/CMakeFiles/test.dir/test.cpp.o: ../src/block_server/test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lhx/CLionProjects/myblockchain/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/block_server/CMakeFiles/test.dir/test.cpp.o"
	cd /home/lhx/CLionProjects/myblockchain/cmake-build-debug/src/block_server && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test.dir/test.cpp.o -c /home/lhx/CLionProjects/myblockchain/src/block_server/test.cpp

src/block_server/CMakeFiles/test.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/test.cpp.i"
	cd /home/lhx/CLionProjects/myblockchain/cmake-build-debug/src/block_server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lhx/CLionProjects/myblockchain/src/block_server/test.cpp > CMakeFiles/test.dir/test.cpp.i

src/block_server/CMakeFiles/test.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/test.cpp.s"
	cd /home/lhx/CLionProjects/myblockchain/cmake-build-debug/src/block_server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lhx/CLionProjects/myblockchain/src/block_server/test.cpp -o CMakeFiles/test.dir/test.cpp.s

# Object files for target test
test_OBJECTS = \
"CMakeFiles/test.dir/test.cpp.o"

# External object files for target test
test_EXTERNAL_OBJECTS =

src/block_server/test: src/block_server/CMakeFiles/test.dir/test.cpp.o
src/block_server/test: src/block_server/CMakeFiles/test.dir/build.make
src/block_server/test: src/block_server/CMakeFiles/test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lhx/CLionProjects/myblockchain/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test"
	cd /home/lhx/CLionProjects/myblockchain/cmake-build-debug/src/block_server && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/block_server/CMakeFiles/test.dir/build: src/block_server/test

.PHONY : src/block_server/CMakeFiles/test.dir/build

src/block_server/CMakeFiles/test.dir/clean:
	cd /home/lhx/CLionProjects/myblockchain/cmake-build-debug/src/block_server && $(CMAKE_COMMAND) -P CMakeFiles/test.dir/cmake_clean.cmake
.PHONY : src/block_server/CMakeFiles/test.dir/clean

src/block_server/CMakeFiles/test.dir/depend:
	cd /home/lhx/CLionProjects/myblockchain/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lhx/CLionProjects/myblockchain /home/lhx/CLionProjects/myblockchain/src/block_server /home/lhx/CLionProjects/myblockchain/cmake-build-debug /home/lhx/CLionProjects/myblockchain/cmake-build-debug/src/block_server /home/lhx/CLionProjects/myblockchain/cmake-build-debug/src/block_server/CMakeFiles/test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/block_server/CMakeFiles/test.dir/depend

