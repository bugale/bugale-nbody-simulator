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
#define WIN32_LEAN_AND_MEAN
#define _USE_MATH_DEFINES
#define settings_size 199 //Size of the settings file
#define body_data_header_size 24 //Size of the header in the body data file
#define body_size 80 //Size in Bytes of One Body on the Disk
#define data_files_version 1
#define BODIES_FILENAME "bodies.bdf"
#define SETTINGS_FILENAME "settings.bdf"
#define BINARYOUTPUT_FILENAME "binary_output.bin"
#define LOG_FILENAME "log.txt"
#define CONSOLE_TITLE "Bugale N-Body Simulator Beta 0.4"

#ifndef internal_includeds

	#ifndef Threads_included
		#include "Header Files\Threads.h"
	#endif

	#ifndef BinaryOutputManager_included
		#include "Header Files\BinaryOutputManager.h"
	#endif

	#ifndef Graphic3D_included
		#include "Header Files\Graphic3D.h"
	#endif

	#ifndef Graphic2D_included
		#include "Header Files\Graphic2D.h"
	#endif

	#ifndef Engine_included
		#include "Header Files\Engine.h"
	#endif

	#ifndef SharedFunctions_included
		#include "Header Files\SharedFunctions.h"
	#endif

	#ifndef Data_included
		#include "Header Files\Data.h"
	#endif

	#ifndef Body3D_included
		#include "Header Files\Body3D.h"
	#endif

	#ifndef Errors_included
		#include "Header Files\Errors.h"
	#endif

	#ifndef SharedData_included
		#include "Header Files\SharedData.h"
	#endif

	#include <math.h>
	#include <malloc.h>
	#include <iostream>
	#include <string>
	#include <stdio.h>
	#include <time.h>
	#include <float.h>
	#include <Windows.h>
	#include <direct.h>
	#include <glut.h>
	#include <boost\thread.hpp>

	#define internal_includeds
#endif
