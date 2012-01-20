#define WIN32_LEAN_AND_MEAN
#define _USE_MATH_DEFINES
#define body_size 80 //Size in Bytes of One Body on the Disk
#define BODIES_FILENAME "bodies.dat"
#define SETTINGS_FILENAME "settings.dat"
#define BINARYOUTPUT_FILENAME "binary_output.bin"
#define LOG_FILENAME "log.txt"

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

#ifndef Data_included
#include "Header Files\Data.h"
#endif

#ifndef Body3D_included
#include "Header Files\Body3D.h"
#endif

#ifndef SharedFunctions_included
#include "Header Files\SharedFunctions.h"
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
#include <glut.h>
#include <boost\thread.hpp>

#define internal_includeds
#endif
