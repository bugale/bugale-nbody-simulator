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
#ifndef __INCLUDES__
	#define __INCLUDES__
	#define WIN32_LEAN_AND_MEAN
	#define _USE_MATH_DEFINES
	#define FREEGLUT_STATIC
	#define settings_size 159 //Size of the settings file
	#define body_data_header_size 24 //Size of the header in the body data file
	#define body_size 80 //Size in Bytes of One Body on the Disk
	#define data_files_version 2
	#define BODIES_FILENAME "bodies.bdf"
	#define SETTINGS_FILENAME "settings.bdf"
	#define BINARYOUTPUT_FILENAME "binary_output.bin"
	#define LOG_FILENAME "log.txt"
	#define PROGRAM_NAME "Bugale N-Body Simulator"
	#define PROGRAM_VERSION "0.1.1 (Beta 1.1)"

	//C Standard Library
	#include <math.h>
	#include <stdarg.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <time.h>
	#include <float.h>
	#include <signal.h>

	//Third-party
	#include <freeglut.h>
	#include <tinythread.h> //CPP is included in Main.cpp for convenience

	//OS Dependant
	#ifdef WIN32
		#include <direct.h>
		#include <windows.h>
		#define getcwd _getcwd
		#define _SYSTEM_WIN
	#else //POSIX
		#include <unistd.h>
		#define _SYSTEM_POSIX
	#endif

	//Internal
	#include "Header Files/Errors.h"
	#include "Header Files/SharedData.h"
	#include "Header Files/Body3D.h"
	#include "Header Files/Data.h"
	#include "Header Files/SharedFunctions.h"
	#include "Header Files/Engine.h"
	#include "Header Files/Graphic2D.h"
	#include "Header Files/Graphic3D.h"
	#include "Header Files/BinaryOutputManager.h"
	#include "Header Files/Threads.h"
	#include "resource.h"

#endif