# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise"

# Include any dependencies generated for this target.
include CMakeFiles/Schungus.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Schungus.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Schungus.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Schungus.dir/flags.make

CMakeFiles/Schungus.dir/codegen:
.PHONY : CMakeFiles/Schungus.dir/codegen

CMakeFiles/Schungus.dir/libs/sdw/CanvasPoint.cpp.o: CMakeFiles/Schungus.dir/flags.make
CMakeFiles/Schungus.dir/libs/sdw/CanvasPoint.cpp.o: libs/sdw/CanvasPoint.cpp
CMakeFiles/Schungus.dir/libs/sdw/CanvasPoint.cpp.o: CMakeFiles/Schungus.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Schungus.dir/libs/sdw/CanvasPoint.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Schungus.dir/libs/sdw/CanvasPoint.cpp.o -MF CMakeFiles/Schungus.dir/libs/sdw/CanvasPoint.cpp.o.d -o CMakeFiles/Schungus.dir/libs/sdw/CanvasPoint.cpp.o -c "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/CanvasPoint.cpp"

CMakeFiles/Schungus.dir/libs/sdw/CanvasPoint.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Schungus.dir/libs/sdw/CanvasPoint.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/CanvasPoint.cpp" > CMakeFiles/Schungus.dir/libs/sdw/CanvasPoint.cpp.i

CMakeFiles/Schungus.dir/libs/sdw/CanvasPoint.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Schungus.dir/libs/sdw/CanvasPoint.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/CanvasPoint.cpp" -o CMakeFiles/Schungus.dir/libs/sdw/CanvasPoint.cpp.s

CMakeFiles/Schungus.dir/libs/sdw/CanvasTriangle.cpp.o: CMakeFiles/Schungus.dir/flags.make
CMakeFiles/Schungus.dir/libs/sdw/CanvasTriangle.cpp.o: libs/sdw/CanvasTriangle.cpp
CMakeFiles/Schungus.dir/libs/sdw/CanvasTriangle.cpp.o: CMakeFiles/Schungus.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Schungus.dir/libs/sdw/CanvasTriangle.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Schungus.dir/libs/sdw/CanvasTriangle.cpp.o -MF CMakeFiles/Schungus.dir/libs/sdw/CanvasTriangle.cpp.o.d -o CMakeFiles/Schungus.dir/libs/sdw/CanvasTriangle.cpp.o -c "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/CanvasTriangle.cpp"

CMakeFiles/Schungus.dir/libs/sdw/CanvasTriangle.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Schungus.dir/libs/sdw/CanvasTriangle.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/CanvasTriangle.cpp" > CMakeFiles/Schungus.dir/libs/sdw/CanvasTriangle.cpp.i

CMakeFiles/Schungus.dir/libs/sdw/CanvasTriangle.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Schungus.dir/libs/sdw/CanvasTriangle.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/CanvasTriangle.cpp" -o CMakeFiles/Schungus.dir/libs/sdw/CanvasTriangle.cpp.s

CMakeFiles/Schungus.dir/libs/sdw/Colour.cpp.o: CMakeFiles/Schungus.dir/flags.make
CMakeFiles/Schungus.dir/libs/sdw/Colour.cpp.o: libs/sdw/Colour.cpp
CMakeFiles/Schungus.dir/libs/sdw/Colour.cpp.o: CMakeFiles/Schungus.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Schungus.dir/libs/sdw/Colour.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Schungus.dir/libs/sdw/Colour.cpp.o -MF CMakeFiles/Schungus.dir/libs/sdw/Colour.cpp.o.d -o CMakeFiles/Schungus.dir/libs/sdw/Colour.cpp.o -c "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/Colour.cpp"

CMakeFiles/Schungus.dir/libs/sdw/Colour.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Schungus.dir/libs/sdw/Colour.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/Colour.cpp" > CMakeFiles/Schungus.dir/libs/sdw/Colour.cpp.i

CMakeFiles/Schungus.dir/libs/sdw/Colour.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Schungus.dir/libs/sdw/Colour.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/Colour.cpp" -o CMakeFiles/Schungus.dir/libs/sdw/Colour.cpp.s

CMakeFiles/Schungus.dir/libs/sdw/DrawingWindow.cpp.o: CMakeFiles/Schungus.dir/flags.make
CMakeFiles/Schungus.dir/libs/sdw/DrawingWindow.cpp.o: libs/sdw/DrawingWindow.cpp
CMakeFiles/Schungus.dir/libs/sdw/DrawingWindow.cpp.o: CMakeFiles/Schungus.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Schungus.dir/libs/sdw/DrawingWindow.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Schungus.dir/libs/sdw/DrawingWindow.cpp.o -MF CMakeFiles/Schungus.dir/libs/sdw/DrawingWindow.cpp.o.d -o CMakeFiles/Schungus.dir/libs/sdw/DrawingWindow.cpp.o -c "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/DrawingWindow.cpp"

CMakeFiles/Schungus.dir/libs/sdw/DrawingWindow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Schungus.dir/libs/sdw/DrawingWindow.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/DrawingWindow.cpp" > CMakeFiles/Schungus.dir/libs/sdw/DrawingWindow.cpp.i

CMakeFiles/Schungus.dir/libs/sdw/DrawingWindow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Schungus.dir/libs/sdw/DrawingWindow.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/DrawingWindow.cpp" -o CMakeFiles/Schungus.dir/libs/sdw/DrawingWindow.cpp.s

CMakeFiles/Schungus.dir/libs/sdw/ModelTriangle.cpp.o: CMakeFiles/Schungus.dir/flags.make
CMakeFiles/Schungus.dir/libs/sdw/ModelTriangle.cpp.o: libs/sdw/ModelTriangle.cpp
CMakeFiles/Schungus.dir/libs/sdw/ModelTriangle.cpp.o: CMakeFiles/Schungus.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Schungus.dir/libs/sdw/ModelTriangle.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Schungus.dir/libs/sdw/ModelTriangle.cpp.o -MF CMakeFiles/Schungus.dir/libs/sdw/ModelTriangle.cpp.o.d -o CMakeFiles/Schungus.dir/libs/sdw/ModelTriangle.cpp.o -c "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/ModelTriangle.cpp"

CMakeFiles/Schungus.dir/libs/sdw/ModelTriangle.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Schungus.dir/libs/sdw/ModelTriangle.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/ModelTriangle.cpp" > CMakeFiles/Schungus.dir/libs/sdw/ModelTriangle.cpp.i

CMakeFiles/Schungus.dir/libs/sdw/ModelTriangle.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Schungus.dir/libs/sdw/ModelTriangle.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/ModelTriangle.cpp" -o CMakeFiles/Schungus.dir/libs/sdw/ModelTriangle.cpp.s

CMakeFiles/Schungus.dir/libs/sdw/RayTriangleIntersection.cpp.o: CMakeFiles/Schungus.dir/flags.make
CMakeFiles/Schungus.dir/libs/sdw/RayTriangleIntersection.cpp.o: libs/sdw/RayTriangleIntersection.cpp
CMakeFiles/Schungus.dir/libs/sdw/RayTriangleIntersection.cpp.o: CMakeFiles/Schungus.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/Schungus.dir/libs/sdw/RayTriangleIntersection.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Schungus.dir/libs/sdw/RayTriangleIntersection.cpp.o -MF CMakeFiles/Schungus.dir/libs/sdw/RayTriangleIntersection.cpp.o.d -o CMakeFiles/Schungus.dir/libs/sdw/RayTriangleIntersection.cpp.o -c "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/RayTriangleIntersection.cpp"

CMakeFiles/Schungus.dir/libs/sdw/RayTriangleIntersection.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Schungus.dir/libs/sdw/RayTriangleIntersection.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/RayTriangleIntersection.cpp" > CMakeFiles/Schungus.dir/libs/sdw/RayTriangleIntersection.cpp.i

CMakeFiles/Schungus.dir/libs/sdw/RayTriangleIntersection.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Schungus.dir/libs/sdw/RayTriangleIntersection.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/RayTriangleIntersection.cpp" -o CMakeFiles/Schungus.dir/libs/sdw/RayTriangleIntersection.cpp.s

CMakeFiles/Schungus.dir/libs/sdw/TextureMap.cpp.o: CMakeFiles/Schungus.dir/flags.make
CMakeFiles/Schungus.dir/libs/sdw/TextureMap.cpp.o: libs/sdw/TextureMap.cpp
CMakeFiles/Schungus.dir/libs/sdw/TextureMap.cpp.o: CMakeFiles/Schungus.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/Schungus.dir/libs/sdw/TextureMap.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Schungus.dir/libs/sdw/TextureMap.cpp.o -MF CMakeFiles/Schungus.dir/libs/sdw/TextureMap.cpp.o.d -o CMakeFiles/Schungus.dir/libs/sdw/TextureMap.cpp.o -c "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/TextureMap.cpp"

CMakeFiles/Schungus.dir/libs/sdw/TextureMap.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Schungus.dir/libs/sdw/TextureMap.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/TextureMap.cpp" > CMakeFiles/Schungus.dir/libs/sdw/TextureMap.cpp.i

CMakeFiles/Schungus.dir/libs/sdw/TextureMap.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Schungus.dir/libs/sdw/TextureMap.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/TextureMap.cpp" -o CMakeFiles/Schungus.dir/libs/sdw/TextureMap.cpp.s

CMakeFiles/Schungus.dir/libs/sdw/TexturePoint.cpp.o: CMakeFiles/Schungus.dir/flags.make
CMakeFiles/Schungus.dir/libs/sdw/TexturePoint.cpp.o: libs/sdw/TexturePoint.cpp
CMakeFiles/Schungus.dir/libs/sdw/TexturePoint.cpp.o: CMakeFiles/Schungus.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/Schungus.dir/libs/sdw/TexturePoint.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Schungus.dir/libs/sdw/TexturePoint.cpp.o -MF CMakeFiles/Schungus.dir/libs/sdw/TexturePoint.cpp.o.d -o CMakeFiles/Schungus.dir/libs/sdw/TexturePoint.cpp.o -c "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/TexturePoint.cpp"

CMakeFiles/Schungus.dir/libs/sdw/TexturePoint.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Schungus.dir/libs/sdw/TexturePoint.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/TexturePoint.cpp" > CMakeFiles/Schungus.dir/libs/sdw/TexturePoint.cpp.i

CMakeFiles/Schungus.dir/libs/sdw/TexturePoint.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Schungus.dir/libs/sdw/TexturePoint.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/TexturePoint.cpp" -o CMakeFiles/Schungus.dir/libs/sdw/TexturePoint.cpp.s

CMakeFiles/Schungus.dir/libs/sdw/Utils.cpp.o: CMakeFiles/Schungus.dir/flags.make
CMakeFiles/Schungus.dir/libs/sdw/Utils.cpp.o: libs/sdw/Utils.cpp
CMakeFiles/Schungus.dir/libs/sdw/Utils.cpp.o: CMakeFiles/Schungus.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/Schungus.dir/libs/sdw/Utils.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Schungus.dir/libs/sdw/Utils.cpp.o -MF CMakeFiles/Schungus.dir/libs/sdw/Utils.cpp.o.d -o CMakeFiles/Schungus.dir/libs/sdw/Utils.cpp.o -c "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/Utils.cpp"

CMakeFiles/Schungus.dir/libs/sdw/Utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Schungus.dir/libs/sdw/Utils.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/Utils.cpp" > CMakeFiles/Schungus.dir/libs/sdw/Utils.cpp.i

CMakeFiles/Schungus.dir/libs/sdw/Utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Schungus.dir/libs/sdw/Utils.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/sdw/Utils.cpp" -o CMakeFiles/Schungus.dir/libs/sdw/Utils.cpp.s

CMakeFiles/Schungus.dir/libs/boople/Camera.cpp.o: CMakeFiles/Schungus.dir/flags.make
CMakeFiles/Schungus.dir/libs/boople/Camera.cpp.o: libs/boople/Camera.cpp
CMakeFiles/Schungus.dir/libs/boople/Camera.cpp.o: CMakeFiles/Schungus.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/Schungus.dir/libs/boople/Camera.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Schungus.dir/libs/boople/Camera.cpp.o -MF CMakeFiles/Schungus.dir/libs/boople/Camera.cpp.o.d -o CMakeFiles/Schungus.dir/libs/boople/Camera.cpp.o -c "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/boople/Camera.cpp"

CMakeFiles/Schungus.dir/libs/boople/Camera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Schungus.dir/libs/boople/Camera.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/boople/Camera.cpp" > CMakeFiles/Schungus.dir/libs/boople/Camera.cpp.i

CMakeFiles/Schungus.dir/libs/boople/Camera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Schungus.dir/libs/boople/Camera.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/boople/Camera.cpp" -o CMakeFiles/Schungus.dir/libs/boople/Camera.cpp.s

CMakeFiles/Schungus.dir/src/Schungus.cpp.o: CMakeFiles/Schungus.dir/flags.make
CMakeFiles/Schungus.dir/src/Schungus.cpp.o: src/Schungus.cpp
CMakeFiles/Schungus.dir/src/Schungus.cpp.o: CMakeFiles/Schungus.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/Schungus.dir/src/Schungus.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Schungus.dir/src/Schungus.cpp.o -MF CMakeFiles/Schungus.dir/src/Schungus.cpp.o.d -o CMakeFiles/Schungus.dir/src/Schungus.cpp.o -c "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/src/Schungus.cpp"

CMakeFiles/Schungus.dir/src/Schungus.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Schungus.dir/src/Schungus.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/src/Schungus.cpp" > CMakeFiles/Schungus.dir/src/Schungus.cpp.i

CMakeFiles/Schungus.dir/src/Schungus.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Schungus.dir/src/Schungus.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/src/Schungus.cpp" -o CMakeFiles/Schungus.dir/src/Schungus.cpp.s

CMakeFiles/Schungus.dir/libs/boople/Light.cpp.o: CMakeFiles/Schungus.dir/flags.make
CMakeFiles/Schungus.dir/libs/boople/Light.cpp.o: libs/boople/Light.cpp
CMakeFiles/Schungus.dir/libs/boople/Light.cpp.o: CMakeFiles/Schungus.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/Schungus.dir/libs/boople/Light.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Schungus.dir/libs/boople/Light.cpp.o -MF CMakeFiles/Schungus.dir/libs/boople/Light.cpp.o.d -o CMakeFiles/Schungus.dir/libs/boople/Light.cpp.o -c "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/boople/Light.cpp"

CMakeFiles/Schungus.dir/libs/boople/Light.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Schungus.dir/libs/boople/Light.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/boople/Light.cpp" > CMakeFiles/Schungus.dir/libs/boople/Light.cpp.i

CMakeFiles/Schungus.dir/libs/boople/Light.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Schungus.dir/libs/boople/Light.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/libs/boople/Light.cpp" -o CMakeFiles/Schungus.dir/libs/boople/Light.cpp.s

# Object files for target Schungus
Schungus_OBJECTS = \
"CMakeFiles/Schungus.dir/libs/sdw/CanvasPoint.cpp.o" \
"CMakeFiles/Schungus.dir/libs/sdw/CanvasTriangle.cpp.o" \
"CMakeFiles/Schungus.dir/libs/sdw/Colour.cpp.o" \
"CMakeFiles/Schungus.dir/libs/sdw/DrawingWindow.cpp.o" \
"CMakeFiles/Schungus.dir/libs/sdw/ModelTriangle.cpp.o" \
"CMakeFiles/Schungus.dir/libs/sdw/RayTriangleIntersection.cpp.o" \
"CMakeFiles/Schungus.dir/libs/sdw/TextureMap.cpp.o" \
"CMakeFiles/Schungus.dir/libs/sdw/TexturePoint.cpp.o" \
"CMakeFiles/Schungus.dir/libs/sdw/Utils.cpp.o" \
"CMakeFiles/Schungus.dir/libs/boople/Camera.cpp.o" \
"CMakeFiles/Schungus.dir/src/Schungus.cpp.o" \
"CMakeFiles/Schungus.dir/libs/boople/Light.cpp.o"

# External object files for target Schungus
Schungus_EXTERNAL_OBJECTS =

Schungus: CMakeFiles/Schungus.dir/libs/sdw/CanvasPoint.cpp.o
Schungus: CMakeFiles/Schungus.dir/libs/sdw/CanvasTriangle.cpp.o
Schungus: CMakeFiles/Schungus.dir/libs/sdw/Colour.cpp.o
Schungus: CMakeFiles/Schungus.dir/libs/sdw/DrawingWindow.cpp.o
Schungus: CMakeFiles/Schungus.dir/libs/sdw/ModelTriangle.cpp.o
Schungus: CMakeFiles/Schungus.dir/libs/sdw/RayTriangleIntersection.cpp.o
Schungus: CMakeFiles/Schungus.dir/libs/sdw/TextureMap.cpp.o
Schungus: CMakeFiles/Schungus.dir/libs/sdw/TexturePoint.cpp.o
Schungus: CMakeFiles/Schungus.dir/libs/sdw/Utils.cpp.o
Schungus: CMakeFiles/Schungus.dir/libs/boople/Camera.cpp.o
Schungus: CMakeFiles/Schungus.dir/src/Schungus.cpp.o
Schungus: CMakeFiles/Schungus.dir/libs/boople/Light.cpp.o
Schungus: CMakeFiles/Schungus.dir/build.make
Schungus: /usr/local/Cellar/sdl2/2.30.7/lib/libSDL2.dylib
Schungus: CMakeFiles/Schungus.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_13) "Linking CXX executable Schungus"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Schungus.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Schungus.dir/build: Schungus
.PHONY : CMakeFiles/Schungus.dir/build

CMakeFiles/Schungus.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Schungus.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Schungus.dir/clean

CMakeFiles/Schungus.dir/depend:
	cd "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise" "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise" "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise" "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise" "/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/CMakeFiles/Schungus.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/Schungus.dir/depend

