This is the source code of Bugale N-Body Simulator version 0.2.0 (Beta 2.0).

This project simulates the behavior, dynamics, and motion of a group of celestial objects.

Given N point masses in a three-dimensional space, it's present positions and velocities, and
supposing that the force of attraction experienced between each pair of particles is Newtonian, the project approximately predicts it's positions and velocities at a future moment of time.

The project can visualize the simulation, and create a graphic real-time output using FreeGLUT,
and also have an option for textual output and archiving.

Bugale N-Body Data Editor provides a graphic user interface for creating and editing the
settings and body data files of Bugale N-Body Simulator. It can be found on SourceForge.

What's new in this release:
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