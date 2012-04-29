This is the source code of Bugale N-Body Simulator version 0.2.4 (Beta 2.4).

This project simulates the behavior, dynamics, and motion of a group of celestial objects.

Given N point masses in a three-dimensional space, it's present positions and velocities, and
supposing that the force of attraction experienced between each pair of particles is Newtonian, the project approximately predicts it's positions and velocities at a future moment of time.

The project can visualize the simulation, and create a graphic real-time output using FreeGLUT,
and also have an option for textual output and archiving.

Bugale N-Body Data Editor provides a graphic user interface for creating and editing the
settings and body data files of Bugale N-Body Simulator. It can be found on SourceForge.

What's new in v0.2.4:
*Switched to BDF 3
*Added support for OpenCL calculation
*Fixed a bug with resizing the double-buffered window
*Fixed a bug with disabling screen-clean at application startup

What's new in v0.2.3:
*Fixed many bugs
*Enlarged the console to see all of the information
*Source Code: Migrated to VS11 Beta (still using v100 compiler and linker)
*Source Code: Added the structures for CUDA calculation - will be usable soon

What's new in v0.2.2:
*Added Up vector rotation using the mouse wheel
*Better memory allocation fail messages
*Less memory usage
*Fixed a bug with the wireframe mode in 3D
*Slightly improved graphics performance
*Better error management

What's new in v0.2.1:
*Fixed a bug with the logging option
*Fixed a bug with bodies overlapping the text
*Fixed some bugs with not cleaning the screen in 2D
*Added vertex/face count
*Run-time loading of OpenGL to check for existance
*Safer memory allocation
*Increased graphic performance
*Added a warning about large far/near plane distance
*Source Code: Better string management

What's new in v0.2.0:
*Fixed some bugs with the logging option
*Fixed some typing errors
*Fixed a bug with closing the application
*Added the Euler and Leapfrog integration algorithms
*Released an executable for older versions of windows
 (95/98/ME/NT/2000/XP SP0-1) and older processors (without SSE2)
*Reduced executable size by removing large icon resolutions
*Source Code: Created the program structure that will later be used to add
 translations
*Source Code: Fixed the resources file to work with Visual Studio

What's new in v0.1.1:
*Better memory management
*Improved performance
*Fixed many bugs
*Enabled semi-transparent colors to be used - buggy

What's new in v0.1.0:
*Compiled with static-link libraries, the DLLs are not needed anymore.
*Fixed major bugs with camera/target moving in 3D.
*Reduced memory use
*Source Code - Switched to freeglut instead of glut
*Source Code - Switched to TinyThread++ instead of boost
*Source Code - The source code is now portable and should compile on most POSIX compatible compilers
*Source Code - Changed headers organization to avoid Intellisense errors and to reduce compilation time

What's new in v0.0.4:
*Now using the improved Bugale Data Format version 1.
*Switched graphical output to double-buffered when possible.
*Fixed a bug with clearing bottom text when the screen is not being cleaned.
*Completed the logging option.
*Fixed the bug with calculating the linear momentum error.
*Changed to a better working directory recognition technique.
*Minor code improvements.

What's new in v0.0.3:
*Fixed some unexpected crashes.
*Added a linear momentum preservation error check.
*Minor code and naming improvement.

What's new in v0.0.2:
*Informative error messages instead of crushes, when there are problems with the data files.