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
CMAKE_SOURCE_DIR = /home/jcrs/Escritorio/Bloques/gr-cv2x

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jcrs/Escritorio/Bloques/gr-cv2x/build

# Include any dependencies generated for this target.
include lib/CMakeFiles/test-cv2x.dir/depend.make

# Include the progress variables for this target.
include lib/CMakeFiles/test-cv2x.dir/progress.make

# Include the compile flags for this target's objects.
include lib/CMakeFiles/test-cv2x.dir/flags.make

lib/CMakeFiles/test-cv2x.dir/test_cv2x.cc.o: lib/CMakeFiles/test-cv2x.dir/flags.make
lib/CMakeFiles/test-cv2x.dir/test_cv2x.cc.o: ../lib/test_cv2x.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jcrs/Escritorio/Bloques/gr-cv2x/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object lib/CMakeFiles/test-cv2x.dir/test_cv2x.cc.o"
	cd /home/jcrs/Escritorio/Bloques/gr-cv2x/build/lib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test-cv2x.dir/test_cv2x.cc.o -c /home/jcrs/Escritorio/Bloques/gr-cv2x/lib/test_cv2x.cc

lib/CMakeFiles/test-cv2x.dir/test_cv2x.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test-cv2x.dir/test_cv2x.cc.i"
	cd /home/jcrs/Escritorio/Bloques/gr-cv2x/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jcrs/Escritorio/Bloques/gr-cv2x/lib/test_cv2x.cc > CMakeFiles/test-cv2x.dir/test_cv2x.cc.i

lib/CMakeFiles/test-cv2x.dir/test_cv2x.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test-cv2x.dir/test_cv2x.cc.s"
	cd /home/jcrs/Escritorio/Bloques/gr-cv2x/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jcrs/Escritorio/Bloques/gr-cv2x/lib/test_cv2x.cc -o CMakeFiles/test-cv2x.dir/test_cv2x.cc.s

lib/CMakeFiles/test-cv2x.dir/test_cv2x.cc.o.requires:

.PHONY : lib/CMakeFiles/test-cv2x.dir/test_cv2x.cc.o.requires

lib/CMakeFiles/test-cv2x.dir/test_cv2x.cc.o.provides: lib/CMakeFiles/test-cv2x.dir/test_cv2x.cc.o.requires
	$(MAKE) -f lib/CMakeFiles/test-cv2x.dir/build.make lib/CMakeFiles/test-cv2x.dir/test_cv2x.cc.o.provides.build
.PHONY : lib/CMakeFiles/test-cv2x.dir/test_cv2x.cc.o.provides

lib/CMakeFiles/test-cv2x.dir/test_cv2x.cc.o.provides.build: lib/CMakeFiles/test-cv2x.dir/test_cv2x.cc.o


lib/CMakeFiles/test-cv2x.dir/qa_cv2x.cc.o: lib/CMakeFiles/test-cv2x.dir/flags.make
lib/CMakeFiles/test-cv2x.dir/qa_cv2x.cc.o: ../lib/qa_cv2x.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jcrs/Escritorio/Bloques/gr-cv2x/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object lib/CMakeFiles/test-cv2x.dir/qa_cv2x.cc.o"
	cd /home/jcrs/Escritorio/Bloques/gr-cv2x/build/lib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test-cv2x.dir/qa_cv2x.cc.o -c /home/jcrs/Escritorio/Bloques/gr-cv2x/lib/qa_cv2x.cc

lib/CMakeFiles/test-cv2x.dir/qa_cv2x.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test-cv2x.dir/qa_cv2x.cc.i"
	cd /home/jcrs/Escritorio/Bloques/gr-cv2x/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jcrs/Escritorio/Bloques/gr-cv2x/lib/qa_cv2x.cc > CMakeFiles/test-cv2x.dir/qa_cv2x.cc.i

lib/CMakeFiles/test-cv2x.dir/qa_cv2x.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test-cv2x.dir/qa_cv2x.cc.s"
	cd /home/jcrs/Escritorio/Bloques/gr-cv2x/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jcrs/Escritorio/Bloques/gr-cv2x/lib/qa_cv2x.cc -o CMakeFiles/test-cv2x.dir/qa_cv2x.cc.s

lib/CMakeFiles/test-cv2x.dir/qa_cv2x.cc.o.requires:

.PHONY : lib/CMakeFiles/test-cv2x.dir/qa_cv2x.cc.o.requires

lib/CMakeFiles/test-cv2x.dir/qa_cv2x.cc.o.provides: lib/CMakeFiles/test-cv2x.dir/qa_cv2x.cc.o.requires
	$(MAKE) -f lib/CMakeFiles/test-cv2x.dir/build.make lib/CMakeFiles/test-cv2x.dir/qa_cv2x.cc.o.provides.build
.PHONY : lib/CMakeFiles/test-cv2x.dir/qa_cv2x.cc.o.provides

lib/CMakeFiles/test-cv2x.dir/qa_cv2x.cc.o.provides.build: lib/CMakeFiles/test-cv2x.dir/qa_cv2x.cc.o


# Object files for target test-cv2x
test__cv2x_OBJECTS = \
"CMakeFiles/test-cv2x.dir/test_cv2x.cc.o" \
"CMakeFiles/test-cv2x.dir/qa_cv2x.cc.o"

# External object files for target test-cv2x
test__cv2x_EXTERNAL_OBJECTS =

lib/test-cv2x: lib/CMakeFiles/test-cv2x.dir/test_cv2x.cc.o
lib/test-cv2x: lib/CMakeFiles/test-cv2x.dir/qa_cv2x.cc.o
lib/test-cv2x: lib/CMakeFiles/test-cv2x.dir/build.make
lib/test-cv2x: /usr/lib/x86_64-linux-gnu/libgnuradio-runtime.so
lib/test-cv2x: /usr/lib/x86_64-linux-gnu/libgnuradio-pmt.so
lib/test-cv2x: /usr/lib/x86_64-linux-gnu/liblog4cpp.so
lib/test-cv2x: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
lib/test-cv2x: /usr/lib/x86_64-linux-gnu/libboost_system.so
lib/test-cv2x: /usr/lib/x86_64-linux-gnu/libcppunit.so
lib/test-cv2x: /usr/lib/x86_64-linux-gnu/libfftw3f.so
lib/test-cv2x: lib/libgnuradio-cv2x-1.0.0git.so.0.0.0
lib/test-cv2x: /usr/lib/x86_64-linux-gnu/libgnuradio-runtime.so
lib/test-cv2x: /usr/lib/x86_64-linux-gnu/libgnuradio-pmt.so
lib/test-cv2x: /usr/lib/x86_64-linux-gnu/liblog4cpp.so
lib/test-cv2x: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
lib/test-cv2x: /usr/lib/x86_64-linux-gnu/libboost_system.so
lib/test-cv2x: /usr/lib/x86_64-linux-gnu/libfftw3f.so
lib/test-cv2x: lib/CMakeFiles/test-cv2x.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jcrs/Escritorio/Bloques/gr-cv2x/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable test-cv2x"
	cd /home/jcrs/Escritorio/Bloques/gr-cv2x/build/lib && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test-cv2x.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/CMakeFiles/test-cv2x.dir/build: lib/test-cv2x

.PHONY : lib/CMakeFiles/test-cv2x.dir/build

lib/CMakeFiles/test-cv2x.dir/requires: lib/CMakeFiles/test-cv2x.dir/test_cv2x.cc.o.requires
lib/CMakeFiles/test-cv2x.dir/requires: lib/CMakeFiles/test-cv2x.dir/qa_cv2x.cc.o.requires

.PHONY : lib/CMakeFiles/test-cv2x.dir/requires

lib/CMakeFiles/test-cv2x.dir/clean:
	cd /home/jcrs/Escritorio/Bloques/gr-cv2x/build/lib && $(CMAKE_COMMAND) -P CMakeFiles/test-cv2x.dir/cmake_clean.cmake
.PHONY : lib/CMakeFiles/test-cv2x.dir/clean

lib/CMakeFiles/test-cv2x.dir/depend:
	cd /home/jcrs/Escritorio/Bloques/gr-cv2x/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jcrs/Escritorio/Bloques/gr-cv2x /home/jcrs/Escritorio/Bloques/gr-cv2x/lib /home/jcrs/Escritorio/Bloques/gr-cv2x/build /home/jcrs/Escritorio/Bloques/gr-cv2x/build/lib /home/jcrs/Escritorio/Bloques/gr-cv2x/build/lib/CMakeFiles/test-cv2x.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/CMakeFiles/test-cv2x.dir/depend

