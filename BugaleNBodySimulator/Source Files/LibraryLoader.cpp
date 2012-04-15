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
#include "../SharedHeader.h"

bool LoadOpenGL(SharedData* shared)
{
	#ifdef _SYSTEM_WIN //Load dll's
		SetErrorMode(1); //Suppress error messages
		if (!LoadLibrary("glu32.dll"))
		{
			int err = GetLastError();
			switch (err)
			{
				case 1157:
				case 126: shared->error = Errors::MissingGlu32; return false;
				case 1114:
				case 193: shared->error = Errors::CorruptedGlu32; return false;
				default:  shared->error = Errors::OtherGlu32; shared->error_data_int = err; return false;
			}
		}
		if (!LoadLibrary("opengl32.dll"))
		{
			int err = GetLastError();
			switch (err)
			{
				case 1157:
				case 126: shared->error = Errors::MissingOpengl32; return false;
				case 1114:
				case 193: shared->error = Errors::CorruptedOpengl32; return false;
				default:  shared->error = Errors::OtherOpengl32; shared->error_data_int = err; return false;
			}
		}
		SetErrorMode(0); //Return error messages back
	#elif _SYSTEM_MAC
		if (!dlopen("libGLU.dylib", RTLD_NOW))
		{
			shared->error_data_charptr = dlerror();
			shared->error = Errors::MissinglibGLUdylib;
			return false;
		}
		if (!dlopen("libOPENGL.dylib", RTLD_NOW))
		{
			shared->error_data_charptr = dlerror();
			shared->error = Errors::MissinglibOPENGLdylib;
			return false;
		}
	#elif _SYSTEM_POSIX
		if (!dlopen("libGLU.so", RTLD_NOW))
		{
			shared->error_data_charptr = dlerror();
			shared->error = Errors::MissinglibGLUso;
			return false;
		}
		if (!dlopen("libOPENGL.so", RTLD_NOW))
		{
			shared->error_data_charptr = dlerror();
			shared->error = Errors::MissinglibOPENGLso;
			return false;
		}
	#endif
	return true;
}
bool LoadCUDA(SharedData* shared)
{
	return false;
}