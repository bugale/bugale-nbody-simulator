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
#include "..\SharedHeader.h"

void Errors::returnError(Error error, char* working_directory, char* ret)
{
	ret[0] = 0;
	char* buffer1 = (char*)malloc(4096); buffer1[0] = 0;
	char* buffer2 = (char*)malloc(4096); buffer2[0] = 0;
	switch (error)
	{
	case Errors::NoError:
			break;
		case Errors::MissingSettingsFile:
			getpath(working_directory, INFO_SETTINGS_FILENAME, buffer1);
			StringController::getString(0x0070, buffer2);
			sprintf(ret, buffer2, buffer1);
			break;
		case Errors::MissingBodyDataFile:
			getpath(working_directory, INFO_SETTINGS_FILENAME, buffer1);
			StringController::getString(0x0071, buffer2);
			sprintf(ret, buffer2, buffer1);
			break;
		case Errors::WrongSettingsFileHeader:  StringController::getString(0x0072, ret); break;
		case Errors::WrongBodyDataFileHeader:  StringController::getString(0x0073, ret); break;
		case Errors::WrongSettingsFileVersion: StringController::getString(0x0074, ret); break;
		case Errors::WrongBodyDataFileVersion: StringController::getString(0x0075, ret); break;
		case Errors::WrongSettingsFileSize:	   StringController::getString(0x0076, ret); break;
		case Errors::WrongBodyDataFileSize:	   StringController::getString(0x0077, ret); break;
		case Errors::WrongBodyDataFileSize2:   StringController::getString(0x0078, ret); break;
		case Errors::NegativeDT:			   StringController::getString(0x0079, ret); break;
		case Errors::NonPositiveFieldOfView:   StringController::getString(0x007A, ret); break;
		case Errors::NegativeMoveSpeed0:	   StringController::getString(0x007B, ret); break;
		case Errors::NonPositiveMoveSpeed1:	   StringController::getString(0x007C, ret); break;
		case Errors::NegativeZoomSpeed0:	   StringController::getString(0x007D, ret); break;
		case Errors::NonPositiveZoomSpeed1:	   StringController::getString(0x007E, ret); break;
		case Errors::NegativeBodyMass:		   StringController::getString(0x007F, ret); break;
		case Errors::NegativeBodyRadius:	   StringController::getString(0x0080, ret); break;
		case Errors::NegativeBodyTrailWidth:   StringController::getString(0x0081, ret); break;
		case Errors::TooManyBodies:			   StringController::getString(0x0082, ret); break;
		case Errors::CannotOpenLogFile:		   StringController::getString(0x00B3, ret); break;
		case Errors::Other:					   StringController::getString(0x0083, ret); break;
		default:							   StringController::getString(0x0084, ret); break;
	}
	switch (error)
	{
		case Errors::MissingSettingsFile:
		case Errors::MissingBodyDataFile:
		case Errors::WrongSettingsFileHeader:
		case Errors::WrongBodyDataFileHeader:
		case Errors::WrongSettingsFileVersion:
		case Errors::WrongBodyDataFileVersion:
		case Errors::WrongSettingsFileSize:
		case Errors::WrongBodyDataFileSize:
		case Errors::WrongBodyDataFileSize2:
		case Errors::NegativeDT:
		case Errors::NonPositiveFieldOfView:
		case Errors::NegativeMoveSpeed0:
		case Errors::NonPositiveMoveSpeed1:
		case Errors::NegativeZoomSpeed0:
		case Errors::NonPositiveZoomSpeed1:
		case Errors::NegativeBodyMass:
		case Errors::NegativeBodyRadius:
		case Errors::NegativeBodyTrailWidth:
		case Errors::TooManyBodies:
			StringController::getString(0x0085, ret);
			break;
		case Errors::CannotOpenLogFile:
			StringController::getString(0x00B4, ret);
			break;
	}
}
