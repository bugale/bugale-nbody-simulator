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
	#define _USE_MATH_DEFINES
	#define FREEGLUT_STATIC

	//C Standard Library
	#include <math.h>
	#include <stdarg.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <time.h>
	#include <float.h>
	#include <signal.h>
	#include <string.h>

	//OS Dependent
	#ifdef WIN32
		#define WIN32_LEAN_AND_MEAN
		#ifdef _LEGACY
			#define _WIN32_WINNT 0x400
		#endif
		#include <direct.h>
		#include <windows.h>
		#define getcwd _getcwd
		#define _SYSTEM_WIN
	#else //POSIX
		#include <unistd.h>
		#define _SYSTEM_POSIX
	#endif
	
	//Third-party
	#include <freeglut.h>
	#include <tinythread.h> //CPP is included in Main.cpp for convenience

	//Internal
	#include "Info.h"
	#include "resource.h"
	#include "Strings/StringController.h"
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

#endif
