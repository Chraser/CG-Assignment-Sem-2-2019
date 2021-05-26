Source files in the assignment/src/4.assignment/assignment/ directory:
main.cpp - nearly the entire program is in this file except for the functionality of the camera that is in camera.h which is in the assignment/include/learnopengl directory.
lighting.fs - used for all objects rendered except for the tip of the torch
lighting.vs - used for all objects rendered except for the tip of the torch
lamp.fs - used for the tip of the torch that is the light source
lamp.vs - used for the tip of the torch that is the light source


How to run:
To run, first empty the contents of the assignment/build directory if stuff exists in the build folder.
Then run "cmake .." while inside the build folder and then run "make".
Proceed to the "bin/4.assignment/" folder from the build folder directory.
Run the program with the line "./4.assignment__assignment"