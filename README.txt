This is the source code of Bugale N-Body Simulator version 0.2.0 (Beta 2.0).

This project simulates the behavior, dynamics, and motion of a group of celestial objects.

Given N point masses in a three-dimensional space, it's present positions and velocities, and
supposing that the force of attraction experienced between each pair of particles is Newtonian, the project approximately predicts it's positions and velocities at a future moment of time.

The project can visualize the simulation, and create a graphic real-time output using FreeGLUT,
and also have an option for textual output and archiving.

Bugale N-Body Data Editor provides a graphic user interface for creating and editing the
settings and body data files of Bugale N-Body Simulator. It can be found on SourceForge.

What's new in this release:
*Fixed a bug with the logging option
*Fixed a bug with bodies overlapping the text
*Fixed some bugs with not cleaning the screen in 2D
*Added vertex/face count
*Run-time loading of OpenGL to check for existance
*Safer memory allocation
*Increased graphic performance
*Added a warning about large far/near plane distance
*Source Code: Better string management