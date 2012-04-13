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
#ifndef __ERRORS_INCLUDED__
#define __ERRORS_INCLUDED__
#include "../SharedHeader.h"

namespace Errors
{
	enum Error
	{
		NoError,
		MissingSettingsFile,
		MissingBodyDataFile,
		WrongSettingsFileHeader,
		WrongBodyDataFileHeader,
		WrongSettingsFileVersion,
		WrongBodyDataFileVersion,
		WrongSettingsFileSize,
		WrongBodyDataFileSize,
		WrongBodyDataFileSize2,
		NegativeDT,
		NonPositiveFieldOfView,
		NegativeMoveSpeed0,
		NonPositiveMoveSpeed1,
		NegativeZoomSpeed0,
		NonPositiveZoomSpeed1,
		NegativeBodyMass,
		NegativeBodyRadius,
		NegativeBodyTrailWidth,
		TooManyBodies,

		CannotOpenLogFile,

		Other
	};
	void returnError(Error error, char* working_directory, char* ret);
}

#endif
