# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /projects/neolm

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /projects/neolm

# Include any dependencies generated for this target.
include CMakeFiles/NeoLM.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/NeoLM.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/NeoLM.dir/flags.make

CMakeFiles/NeoLM.dir/main.cpp.o: CMakeFiles/NeoLM.dir/flags.make
CMakeFiles/NeoLM.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/projects/neolm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/NeoLM.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/NeoLM.dir/main.cpp.o -c /projects/neolm/main.cpp

CMakeFiles/NeoLM.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/NeoLM.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /projects/neolm/main.cpp > CMakeFiles/NeoLM.dir/main.cpp.i

CMakeFiles/NeoLM.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/NeoLM.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /projects/neolm/main.cpp -o CMakeFiles/NeoLM.dir/main.cpp.s

CMakeFiles/NeoLM.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/NeoLM.dir/main.cpp.o.requires

CMakeFiles/NeoLM.dir/main.cpp.o.provides: CMakeFiles/NeoLM.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/NeoLM.dir/build.make CMakeFiles/NeoLM.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/NeoLM.dir/main.cpp.o.provides

CMakeFiles/NeoLM.dir/main.cpp.o.provides.build: CMakeFiles/NeoLM.dir/main.cpp.o


# Object files for target NeoLM
NeoLM_OBJECTS = \
"CMakeFiles/NeoLM.dir/main.cpp.o"

# External object files for target NeoLM
NeoLM_EXTERNAL_OBJECTS =

NeoLM: CMakeFiles/NeoLM.dir/main.cpp.o
NeoLM: CMakeFiles/NeoLM.dir/build.make
NeoLM: /home/ndjonge/vcpkg/installed/x64-linux/debug/lib/libssl.a
NeoLM: /home/ndjonge/vcpkg/installed/x64-linux/debug/lib/libcrypto.a
NeoLM: /home/ndjonge/vcpkg/installed/x64-linux/debug/lib/libz.a
NeoLM: CMakeFiles/NeoLM.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/projects/neolm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable NeoLM"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/NeoLM.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/NeoLM.dir/build: NeoLM

.PHONY : CMakeFiles/NeoLM.dir/build

CMakeFiles/NeoLM.dir/requires: CMakeFiles/NeoLM.dir/main.cpp.o.requires

.PHONY : CMakeFiles/NeoLM.dir/requires

CMakeFiles/NeoLM.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/NeoLM.dir/cmake_clean.cmake
.PHONY : CMakeFiles/NeoLM.dir/clean

CMakeFiles/NeoLM.dir/depend:
	cd /projects/neolm && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /projects/neolm /projects/neolm /projects/neolm /projects/neolm /projects/neolm/CMakeFiles/NeoLM.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/NeoLM.dir/depend

