/*
    This is the Bugale N-Body Simulator. A simple N-Body simulator.
    Copyright (C) 2012  bugale
	
	You may contact me by email: bugalit@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 3 as published by
    the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//Note: strings are not sorted by ID here!

#pragma region Critical Errors
#define LNG00_STRING0000 "Critical Error: getcwd failed!\n"
#define LNG00_STRING00BF "Critical Error: Unable to allocate memory for Binary Output mem_buffer! Size is: %d."
#define LNG00_STRING00C0 "Critical Error: Unable to allocate memory for Binary Output disk_buffer! Size is: %d."
#define LNG00_STRING00C1 "Critical Error: Unable to allocate memory for the array of bodies! Size is: %d."
#define LNG00_STRING00C2 "Critical Error: Unable to allocate memory for reading body data or settings file! Size is: %d."
#define LNG00_STRING00C3 "Critical Error: Unable to allocate memory for the array dt_div_mass! Size is: %d."
#define LNG00_STRING00C4 "Critical Error: Unable to allocate memory for the array dt_div_2mass! Size is: %d."
#define LNG00_STRING00C5 "Critical Error: Unable to allocate memory for the array mass_mul_g! Size is: %d."
#define LNG00_STRING00C6 "Critical Error: Unable to allocate memory for the array dt_squared_div_2mass! Size is: %d."
#define LNG00_STRING00C7 "Critical Error: Unable to allocate memory for the array dt_squared_div_12mass! Size is: %d."
#define LNG00_STRING00C8 "Critical Error: Unable to allocate memory for the array dt_pow_3_div_6mass! Size is: %d."
#define LNG00_STRING00CB "Critical Error: Unable to allocate memory for error string! Size is: %d."
#define LNG00_STRING00CC "Critical Error: Unable to allocate memory for 2D trail array (X)! Size is: %d."
#define LNG00_STRING00CD "Critical Error: Unable to allocate memory for 2D trail array (Y)! Size is: %d."
#define LNG00_STRING00CE "Critical Error: Unable to allocate memory for 2D keyboard array (start_time)! Size is: %d."
#define LNG00_STRING00CF "Critical Error: Unable to allocate memory for 2D keyboard array (started)! Size is: %d."
#define LNG00_STRING00D0 "Critical Error: Unable to allocate memory for 2D keyboard array (start_value)! Size is: %d."
#define LNG00_STRING00D1 "Critical Error: Unable to allocate memory for 2D temporary string! Size is: %d."
#define LNG00_STRING00D2 "Critical Error: Unable to allocate memory for 3D trail array (X)! Size is: %d."
#define LNG00_STRING00D3 "Critical Error: Unable to allocate memory for 3D trail array (Y)! Size is: %d."
#define LNG00_STRING00D4 "Critical Error: Unable to allocate memory for 3D trail array (Z)! Size is: %d."
#define LNG00_STRING00D5 "Critical Error: Unable to allocate memory for 3D temporary string! Size is: %d."
#define LNG00_STRING00D6 "Critical Error: Unable to allocate memory for working directory buffer! Size is: %d."
#define LNG00_STRING00D7 "Critical Error: Unable to allocate memory for settings file path buffer! Size is: %d."
#define LNG00_STRING00D8 "Critical Error: Unable to allocate memory for bodies file path buffer! Size is: %d."
#define LNG00_STRING00D9 "Critical Error: Unable to allocate memory for binary output file path buffer! Size is: %d."
#define LNG00_STRING00DA "Critical Error: Unable to allocate memory for log file path buffer! Size is: %d."
#define LNG00_STRING00EA "Critical Error: Unable to allocate memory for CUDA data buffer! Size is: %d."
#pragma endregion

#pragma region General Messages
#define LNG00_STRING0001 "Working Directory: %s\n\n"
#define LNG00_STRING0003 "No data errors.\n"
#define LNG00_STRING0004 "Welcome to %s version %s!\n"\
						 "You may interrupt the console to close the program (usually Ctrl+C).\n"\
						 "Please Wait...\n\n"
#define LNG00_STRING0002 "Logging started.\n\n"
#define LNG00_STRING000C "Maximum Calculations Reached!\n"
#define LNG00_STRING00BD "\nWarning: The ratio FarPlaneDistance/NearPlaneDistance is extremely large. If not in wireframe mode(w), you might get artifacts.\n"
#pragma endregion

#pragma region Algorithm Names
#define LNG00_STRING001A "Euler"
#define LNG00_STRING001B "Leapfrog"
#define LNG00_STRING001C "ModifiedEuler"
#define LNG00_STRING001D "Hermite"
#pragma endregion

#pragma region Log Text
#define LNG00_STRING0007 "Entered exit_signal with shared at 0x%08X."
#define LNG00_STRING0008 "Ended exit_signal."
#define LNG00_STRING0009 "Entered GraphicThread with data at 0x%08X and shared at 0x%08X."
#define LNG00_STRING000A "Ended GraphicThread."
#define LNG00_STRING000B "Entered CalculationThread with engine at 0x%08X data at 0x%08X and shared at 0x%08X."
#define LNG00_STRING000D "Ended CalculationThread."
#define LNG00_STRING000E "Entered SharedCalculationsThread with engine at 0x%08X data at 0x%08X and shared at 0x%08X."
#define LNG00_STRING000F "Ended SharedCalculationsThread."
#define LNG00_STRING0010 "Entered BinaryOutputThread with bom at 0x%08X data at 0x%08X and shared at 0x%08X."
#define LNG00_STRING0011 "Ended BinaryOutputThread."
#define LNG00_STRING0012 "Entered ExitThreads with thread_shared_calc at 0x%08X thread_calculation at 0x%08X thread_binary at 0x%08X and data at 0x%08X."
#define LNG00_STRING0013 "Ended ExitThreads."
#define LNG00_STRING0014 "Entered getpath(1/4) with arg at 0x%08X file at 0x%08X and out at 0x%08X."
#define LNG00_STRING0015 "Entered getpath(2/4) with arg as \"%s\"."
#define LNG00_STRING0016 "Entered getpath(3/4) with file as \"%s\"."
#define LNG00_STRING0017 "Entered getpath(4/4) with out as \"%s\"."
#define LNG00_STRING0018 "Ended getpath(1/2) with result at 0x%08X."
#define LNG00_STRING0019 "Ended getpath(2/2) with result as \"%s\"."
#define LNG00_STRING001E "--------------------------------\nStart of logging:\n"
#define LNG00_STRING001F "Start Time: %s\n"
#define LNG00_STRING0020 "Fetched Settings:\n"
#define LNG00_STRING0021 " - width: %d  height: %d\n"
#define LNG00_STRING0022 " - two_dimensional_calculation: %d  two_dimensional_graphic: %d  two_dimensional_binary: %d\n"
#define LNG00_STRING0023 " - fullscreen: %d  clear_screen: %d  show_trails: %d\n"
#define LNG00_STRING0024 " - min_text: %d  crosshair: %d  paused: %d  log: %d\n"
#define LNG00_STRING0025 " - algorithm: %d  graphic_max_rate: %G  binary_max_rate: %G\n"
#define LNG00_STRING0026 " - max_trails: %d  stick_to_body: %d  sphere_slices: %d  sphere_stacks: %d\n"
#define LNG00_STRING0027 " - field_of_view: %G  near_plane_distance: %G  far_plane_distance: %G\n"
#define LNG00_STRING0028 " - camera_position: (%G,%G,%G)\n"
#define LNG00_STRING0029 " - camera_target: (%G,%G,%G)\n"
#define LNG00_STRING002A " - camera_up: (%G,%G,%G)\n"
#define LNG00_STRING002B " - keyboard_move_speed0: %G  keyboard_move_speed1: %G\n"
#define LNG00_STRING002C " - keyboard_zoom_speed0: %G  keyboard_zoom_speed1: %G\n\n\n"
#define LNG00_STRING002D "\nEnd Time: %s\n"
#define LNG00_STRING002E "Running Time(seconds): %G\n"
#define LNG00_STRING002F "Simulation Running Time(seconds): %G\n"
#define LNG00_STRING0030 "Number of Calculations Done: %d\n"
#define LNG00_STRING0031 "Average Calculations Per Second: %G\n"
#define LNG00_STRING0032 "\n\nEnd of Log!\n"
#define LNG00_STRING0033 "--------------------------------\n\n\n"
#define LNG00_STRING0034 "Entered NewGraphic3D with data at 0x%08X and shared at 0x%08X."
#define LNG00_STRING0037 "Ended 3D Glut Event Loop."
#define LNG00_STRING0038 "Ended NewGraphic3D."
#define LNG00_STRING0039 "Entered Graphic3DInitialize."
#define LNG00_STRING003A "Graphic3DInitialize - single buffer window set at 0x%08X."
#define LNG00_STRING003B "Graphic3DInitialize - double buffer window set at 0x%08X."
#define LNG00_STRING003C "Ended Graphic3DInitialize."
#define LNG00_STRING003D "Entered Graphic3DRatioHandler with width as %d and height as %d."
#define LNG00_STRING003E "Ended Graphic3DRatioHandler."
#define LNG00_STRING0053 "Entered Graphic3DProcessCameraMove(1/3) with angleVertical as %G angleHorizontal as %G zoom_duration_in as %G zoom_duration_out as %G forwardX as %G forwardY as %G."
#define LNG00_STRING0054 "Entered Graphic3DProcessCameraMove(2/3) with forwardZ as %G upX as %G upY as %G upZ as %G."
#define LNG00_STRING0055 "Entered Graphic3DProcessCameraMove(3/3) with leftX as %G leftY as %G and leftZ as %G."
#define LNG00_STRING0056 "Graphic3DProcessCameraMove: Vertical rotate back about Z result is upX as %G upY as %G forwardX as %G and forwardY as %G."
#define LNG00_STRING0057 "Graphic3DProcessCameraMove: Horizontal rotate back about Z result is upX as %G upY as %G forwardX as %G forwardY as %G."
#define LNG00_STRING0058 "Graphic3DProcessCameraMove: Zoom result is zoom_acceleration as %G zoom_distance_ratio as %G and forward_start_length as %G."
#define LNG00_STRING0059 "Ended Graphic3DProcessCameraMove with result positionX as %G positionY as %G and positionZ as %G."
#define LNG00_STRING005A "Entered Graphic3DProcessTargetMove(1/3) with angleVertical as %G angleHorizontal as %G zoom_duration_in as %G zoom_duration_out as %G forwardX as %G forwardY as %G."
#define LNG00_STRING005B "Entered Graphic3DProcessTargetMove(2/3) with forwardZ as %G upX as %G upY as %G upZ as %G."
#define LNG00_STRING005C "Entered Graphic3DProcessTargetMove(3/3) with leftX as %G leftY as %G and leftZ as %G."
#define LNG00_STRING005D "Graphic3DProcessTargetMove: Vertical rotate back about Z result is upX as %G upY as %G forwardX as %G and forwardY as %G."
#define LNG00_STRING005E "Graphic3DProcessTargetMove: Horizontal rotate back about Z result is upX as %G upY as %G forwardX as %G forwardY as %G."
#define LNG00_STRING005F "Graphic3DProcessTargetMove: Zoom result is zoom_acceleration as %G zoom_distance_ratio as %G and forward_start_length as %G."
#define LNG00_STRING0060 "Ended Graphic3DProcessTargetMove with result targetX as %G targetY as %G and targetZ as %G."
#define LNG00_STRING0061 "Entered NewGraphic2D with data at 0x%08X and shared at 0x%08X."
#define LNG00_STRING0062 "Bugale N-Body Simulator 2D Graphical Output Window"
#define LNG00_STRING0086 "Entered Engine constructor with data at 0x%08X."
#define LNG00_STRING0087 "Ended Engine constructor with this at 0x%08X."
#define LNG00_STRING0088 "Entered Engine::Precalculations."
#define LNG00_STRING0089 "Ended Engine::Precalculations(1/3) with dt_div_2 as %d dt_div_12 as %d dt_squared_mul_g_div_12 as %d and dt_pow_3_div_6 as %d."
#define LNG00_STRING008A "Ended Engine::Precalculations(2/3) with dt_squared_div_2 as %d dt_squared_div_12 as %d dt_div_mass at 0x%08X and dt_div_2mass at 0x%08X."
#define LNG00_STRING008B "Ended Engine::Precalculations(3/3) with mass_mul_g at 0x%08X dt_squared_div_2mass at 0x%08X dt_squared_div_12mass at 0x%08X and dt_pow_3_div_6mass at 0x%08X."
#define LNG00_STRING008D "Entered Engine::GetEnergySum2D."
#define LNG00_STRING008E "Ended Engine::GetEnergySum2D with result as %G."
#define LNG00_STRING008F "Entered Engine::GetEnergySum3D."
#define LNG00_STRING0090 "Ended Engine::GetEnergySum3D with result as %G."
#define LNG00_STRING0091 "Entered Engine::GetMomentumSum2D."
#define LNG00_STRING0092 "Ended Engine::GetMomentumSum2D with result as %G."
#define LNG00_STRING0093 "Entered Engine::GetMomentumSum3D."
#define LNG00_STRING0094 "Ended Engine::GetMomentumSum3D with result as %G."
#define LNG00_STRING0095 "Entered Engine::InitializeHermite2D."
#define LNG00_STRING0096 "Ended Engine::InitializeHermite2D."
#define LNG00_STRING0097 "Entered Engine::InitializeHermite3D."
#define LNG00_STRING0098 "Ended Engine::InitializeHermite3D."
#define LNG00_STRING0099 "Entered BinaryOutputManager constructor(1/2) with data at 0x%08X buffer_frames as %d filename at 0x%08X."
#define LNG00_STRING009A "Entered BinaryOutputManager constructor(2/2) with filename as \"%s\"."
#define LNG00_STRING009B "Ended BinaryOutputManager constructor with this at 0x%08X."
#define LNG00_STRING009C "Entered BinaryOutputManager::Capture with time as %d."
#define LNG00_STRING009D "Ended BinaryOutputManager::Capture with result as true."
#define LNG00_STRING009E "Ended BinaryOutputManager::Capture with result as false."
#define LNG00_STRING009F "Entered BinaryOutputManager::Save."
#define LNG00_STRING00A0 "Ended BinaryOutputManager::Save."
#define LNG00_STRING00A1 "Entered BinaryOutputManager::Finalize."
#define LNG00_STRING00A2 "Ended BinaryOutputManager::Finalize."
#define LNG00_STRING00A3 "Graphic3DProcessCameraMove: Vertical rotate about Z result is forwardX as %G and forwardY as %G."
#define LNG00_STRING00A4 "Graphic3DProcessCameraMove: Vertical rotate about Y result is forwardX as %G and forwardZ as %G."
#define LNG00_STRING00A5 "Graphic3DProcessCameraMove: Vertical rotate about X result is upY as %G upZ as %G forwardY as %G and forwardZ as %G."
#define LNG00_STRING00A6 "Graphic3DProcessCameraMove: Vertical rotate back about Y result is upX as %G upZ as %G forwardX as %G and forwardZ as %G."
#define LNG00_STRING00A7 "Graphic3DProcessCameraMove: Horizontal rotate about Z result is upX as %G upY as %G forwardX as %G and forwardY as %G."
#define LNG00_STRING00A8 "Graphic3DProcessCameraMove: Horizontal rotate about Y result is upX as %G upZ as %G forwardX as %G and forwardZ as %G."
#define LNG00_STRING00A9 "Graphic3DProcessCameraMove: Horizontal rotate about X result is upY as %G upZ as %G forwardY as %G forwardZ as %G."
#define LNG00_STRING00AA "Graphic3DProcessCameraMove: Horizontal rotate back about Y result is upX as %G upZ as %G forwardX as %G forwardZ as %G."
#define LNG00_STRING00AB "Graphic3DProcessTargetMove: Vertical rotate about Z result is forwardX as %G and forwardY as %G."
#define LNG00_STRING00AC "Graphic3DProcessTargetMove: Vertical rotate about Y result is forwardX as %G and forwardZ as %G."
#define LNG00_STRING00AD "Graphic3DProcessTargetMove: Vertical rotate about X result is upY as %G upZ as %G forwardY as %G and forwardZ as %G."
#define LNG00_STRING00AE "Graphic3DProcessTargetMove: Vertical rotate back about Y result is upX as %G upZ as %G forwardX as %G and forwardZ as %G."
#define LNG00_STRING00AF "Graphic3DProcessTargetMove: Horizontal rotate about Z result is upX as %G upY as %G forwardX as %G and forwardY as %G."
#define LNG00_STRING00B0 "Graphic3DProcessTargetMove: Horizontal rotate about Y result is upX as %G upZ as %G forwardX as %G and forwardZ as %G."
#define LNG00_STRING00B1 "Graphic3DProcessTargetMove: Horizontal rotate about X result is upY as %G upZ as %G forwardY as %G and forwardZ as %G."
#define LNG00_STRING00B2 "Graphic3DProcessTargetMove: Horizontal rotate back about Y result is upX as %G upZ as %G forwardX as %G forwardZ as %G."
#define LNG00_STRING0005 "Entered Graphic3DProcessUpMove with angle as %G forwardX as %G forwardY as %G forwardZ as %G upX as %G upY as %G upZ as %G leftX as %G."
#define LNG00_STRING0006 "Graphic3DProcessUpMove: Rotate about Z result is upX as %G upY as %G."
#define LNG00_STRING008C "Graphic3DProcessUpMove: Rotate about Y result is upX as %G upZ as %G."
#define LNG00_STRING00BC "Graphic3DProcessUpMove: Rotate about X result is upY as %G upZ as %G."
#define LNG00_STRING00C9 "Graphic3DProcessUpMove: Rotate back about Y result is upX as %G upZ as %G."
#define LNG00_STRING00CA "Graphic3DProcessUpMove: Rotate back about Z result is upX as %G upZ as %G."
#define LNG00_STRING00DB "Ended Graphic3DProcessUpMove with result upX as %G upY as %G and upZ as %G."
#define LNG00_STRING00E0 "Enetered CUDAHandler constructor with shared at 0x%08X and sata at 0x%08X."
#define LNG00_STRING00E1 "Ended CUDAHandler constructor with this at 0x%08X."
#define LNG00_STRING00E2 "Entered CUDAHandler::GetEnergySum2D."
#define LNG00_STRING00E3 "Ended CUDAHandler::GetEnergySum2D with result as %G."
#define LNG00_STRING00E4 "Entered CUDAHandler::GetEnergySum3D."
#define LNG00_STRING00E5 "Ended CUDAHandler::GetEnergySum3D with result as %G."
#define LNG00_STRING00E6 "Entered CUDAHandler::GetMomentumSum2D."
#define LNG00_STRING00E7 "Ended CUDAHandler::GetMomentumSum2D with result as %G."
#define LNG00_STRING00E8 "Entered CUDAHandler::GetMomentumSum3D."
#define LNG00_STRING00E9 "Ended CUDAHandler::GetMomentumSum3D with result as %G."
#pragma endregion

#pragma region Graphical Output Text
#define LNG00_STRING0035 "Bugale N-Body Simulator 3D Graphical Output Window"
#define LNG00_STRING0036 "\n\n\n"\
						 "3D Graphical Output Instructions:\n\n"\
						 "            ESC          : Close the Simulator\n"\
						 "     Left  Mouse Key     : Move Your Camera Around the Target\n"\
						 " Left  Mouse Key and +/- : Move Your Target Against the Camera\n"\
						 "     Right Mouse Key     : Move Your Target Around the Camera\n"\
						 "       Mouse wheel       : Rotate Your Up Vector\n"\
						 "           + or -        : Zoom In or Zoom Out\n"\
						 "             r           : Reset Your Camera Position\n"\
						 "             m           : Toggle Minimal Text Mode = Much Faster Rendering\n"\
						 "             t           : Toggle Trail Visibility\n"\
						 "             c           : Toggle Screen Clearing After Every Frame\n"\
						 "             h           : Toggle Crosshair Visibility\n"\
						 "             w           : Toggle Wireframe = faster rendering\n"\
						 "             p           : Toggle Pause\n"\
						 "   After a Number Has Been Assigned, Press Enter to Show the Body with the\n"\
						 "   Given Index on the Center of the Screen, or Press Space to Always show the\n"\
						 "   Body with the Given Index on the Center of the Screen and press again to\n"\
						 "   disable it.\n\n\n"
#define LNG00_STRING003F "Frames Per Second: %G"
#define LNG00_STRING0040 "Calculations Per Second:   %+E"
#define LNG00_STRING0041 "Total Calculations Done:   %+E"
#define LNG00_STRING0042 "DeltaTime:                 %+E"
#define LNG00_STRING0043 "Simulated Seconds Past:    %+E"
#define LNG00_STRING0044 "Simulated Years Past:      %+E"
#define LNG00_STRING0045 "Real Seconds Past:         %+E"
#define LNG00_STRING0046 "Simulated-Real Time Ratio: %+E"
#define LNG00_STRING0047 "Gravitational Constant:    %+E"
#define LNG00_STRING0048 "Number of Bodies: %d"
#define LNG00_STRING0049 "Integration Algorithm: %s %s"
#define LNG00_STRING004A "Energy Error: %+E"
#define LNG00_STRING004B "Energy Error: Pause to Calculate..."
#define LNG00_STRING004C "Momentum Error: %+E"
#define LNG00_STRING004D "Momentum Error: Pause to Calculate..."
#define LNG00_STRING004E "Camera Left:     (%+E,%+E,%+E)"
#define LNG00_STRING004F "Camera Forward:  (%+E,%+E,%+E)"
#define LNG00_STRING0050 "Camera Up:       (%+E,%+E,%+E)"
#define LNG00_STRING0051 "Camera Target:   (%+E,%+E,%+E)"
#define LNG00_STRING0052 "Camera Position: (%+E,%+E,%+E)"
#define LNG00_STRING0063 "\n\n\n"\
						 "2D Graphical Output Instructions:\n\n"\
						 "    ESC  : Close the Simulator\n"\
						 "   Arrows: Move Your Camera\n"\
						 "   + or -: Zoom In or Zoom Out\n"\
						 "     r   : Reset Your Camera Position\n"\
						 "     m   : Toggle Minimal Text Mode = Much More Frames Per Second\n"\
						 "     t   : Toggle Trail Visibility\n"\
						 "     c   : Toggle Screen Clearing After Every Frame\n"\
						 "     h   : Toggle Crosshair Visibility\n"\
						 "     p   : Toggle Pause\n"\
						 "   After a Number Has Been Assigned, Press Enter to Show the Body with the\n"\
						 "   Given Index on the Center of the Screen, or Press Space to Always show the\n"\
						 "   Body with the Given Index on the Center of the Screen and press again to\n"\
						 "   disable it.\n\n\n"\
						 "\nThank you for using Bugale N-Body Simulator, and have a pleasant day!\n\n\n\n"
#define LNG00_STRING0064 "Ended 2D Glut Event Loop."
#define LNG00_STRING0065 "Ended NewGraphic2D."
#define LNG00_STRING0066 "Entered Graphic2DInitialize."
#define LNG00_STRING0067 "Graphic2DInitialize - single buffer window set at 0x%08X."
#define LNG00_STRING0068 "Graphic2DInitialize - double buffer window set at 0x%08X."
#define LNG00_STRING0069 "Ended Graphic2DInitialize."
#define LNG00_STRING006A "Entered Graphic2DRatioHandler with width as %d and height as %d."
#define LNG00_STRING006B "Ended Graphic2DRatioHandler."
#define LNG00_STRING006C "Camera Position Z: %+E"
#define LNG00_STRING006D "Camera Position Y: %+E"
#define LNG00_STRING006E "Camera Position X: %+E"
#define LNG00_STRING006F "Screen Height in Meters: %E"
#define LNG00_STRING00BE "Vertex/Face Count: %9d,%9d"
#pragma endregion

#pragma region Errors
#define LNG00_STRING0070 "Unable to open settings file(expected path: %s)!"
#define LNG00_STRING0071 "Unable to open data file(expected path: %s)!"
#define LNG00_STRING0072 "Settings file is corrupted(wrong header)!"
#define LNG00_STRING0073 "Body data file is corrupted(wrong header)!"
#define LNG00_STRING0074 "Settings file is corrupted(wrong format version)!"
#define LNG00_STRING0075 "Body data file is corrupted(wrong format version)!"
#define LNG00_STRING0076 "Settings file is corrupted(wrong file size)!"
#define LNG00_STRING0077 "Body data file is corrupted(wrong header size)!"
#define LNG00_STRING0078 "Body data file is corrupted(wrong file size)!"
#define LNG00_STRING0079 "Settings file is corrupted(negative DT)!"
#define LNG00_STRING007A "Settings file is corrupted(non-positive field of view)!"
#define LNG00_STRING007B "Settings file is corrupted(negative MoveSpeed0)!"
#define LNG00_STRING007C "Settings file is corrupted(non-positive MoveSpeed1)!"
#define LNG00_STRING007D "Settings file is corrupted(negative ZoomSpeed0)!"
#define LNG00_STRING007E "Settings file is corrupted(non-positive ZoomSpeed1)!"
#define LNG00_STRING007F "Body data file is corrupted(negative body mass)!"
#define LNG00_STRING0080 "Body data file is corrupted(negative body radius)!"
#define LNG00_STRING0081 "Body data file is corrupted(negative body trail width)!"
#define LNG00_STRING0082 "Body data file is probably corrupted(too many bodies)!"
#define LNG00_STRING0083 "Unidentified Error!"
#define LNG00_STRING0084 "Unknown Error!"
#define LNG00_STRING00B3 "Unable to open log file(expected path: %s)! It may be in use or read-only."
#define LNG00_STRING00B5 "The file glu32.dll is corrupted!"
#define LNG00_STRING00B6 "The file opengl32.dll is corrupted!"
#define LNG00_STRING00B7 "Unable to find glu32.dll!"
#define LNG00_STRING00B8 "Unable to find opengl32.dll!"
#define LNG00_STRING00B9 "Unidentified error opening glu32.dll! Error code: %d."
#define LNG00_STRING00BA "Unidentified error opening opengl32.dll! Error code: %d."
#define LNG00_STRING00DC "Unable to open libGLU.dylib! Error is: %s."
#define LNG00_STRING00DD "Unable to open libOPENGL.dylib! Error is: %s."
#define LNG00_STRING00DE "Unable to open libGLU.so! Error is: %s."
#define LNG00_STRING00DF "Unable to open libOPENGL.so! Error is: %s."
#pragma endregion

#pragma region Errors Help
#define LNG00_STRING0085 "\nYou can use Bugale N-Body Data Editor to create and edit this file."\
						 "\nBugale N-Body Data Editor can be found on SourceForge."\
						 "\nAlternatively, you can download samples of this file for various simulations from this project's page in SourceForge."
#define LNG00_STRING00B4 "\nConsider disabling the logging in the settings file.\n"
#define LNG00_STRING00BB "\nPlease note that this application is using OpenGL and it must be present on your computer.\n"
#pragma endregion

#pragma region Unused
#define LNG00_STRING00EB ""
#define LNG00_STRING00EC ""
#define LNG00_STRING00ED ""
#define LNG00_STRING00EE ""
#define LNG00_STRING00EF ""
#define LNG00_STRING00F0 ""
#define LNG00_STRING00F1 ""
#define LNG00_STRING00F2 ""
#define LNG00_STRING00F3 ""
#define LNG00_STRING00F4 ""
#define LNG00_STRING00F5 ""
#define LNG00_STRING00F6 ""
#define LNG00_STRING00F7 ""
#define LNG00_STRING00F8 ""
#define LNG00_STRING00F9 ""
#define LNG00_STRING00FA ""
#define LNG00_STRING00FB ""
#define LNG00_STRING00FC ""
#define LNG00_STRING00FD ""
#define LNG00_STRING00FE ""
#define LNG00_STRING00FF ""
#pragma endregion
