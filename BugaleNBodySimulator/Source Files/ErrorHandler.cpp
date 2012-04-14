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

void Errors::returnError(SharedData* shared)
{
	char* ret = (char*)safe_malloc(4096); ret[0] = 0;
	char* buffer = (char*)safe_malloc(4096); buffer[0] = 0;
	switch (shared->error)
	{
	case Errors::NoError:
			break;
		case Errors::MissingSettingsFile:
			StringController::getString(0x0070, buffer);
			sprintf(ret, buffer, shared->error_data_charptr);
			log_line(0x0070, shared->error_data_charptr);
			break;
		case Errors::MissingBodyDataFile:
			StringController::getString(0x0071, buffer);
			sprintf(ret, buffer, shared->error_data_charptr);
			log_line(0x0071, shared->error_data_charptr);
			break;
		case Errors::WrongSettingsFileHeader:  StringController::getStringf(0x0072, ret); log_line(0x0072); break;
		case Errors::WrongBodyDataFileHeader:  StringController::getStringf(0x0073, ret); log_line(0x0073); break;
		case Errors::WrongSettingsFileVersion: StringController::getStringf(0x0074, ret); log_line(0x0074); break;
		case Errors::WrongBodyDataFileVersion: StringController::getStringf(0x0075, ret); log_line(0x0075); break;
		case Errors::WrongSettingsFileSize:	   StringController::getStringf(0x0076, ret); log_line(0x0076); break;
		case Errors::WrongBodyDataFileSize:	   StringController::getStringf(0x0077, ret); log_line(0x0077); break;
		case Errors::WrongBodyDataFileSize2:   StringController::getStringf(0x0078, ret); log_line(0x0078); break;
		case Errors::NegativeDT:			   StringController::getStringf(0x0079, ret); log_line(0x0079); break;
		case Errors::NonPositiveFieldOfView:   StringController::getStringf(0x007A, ret); log_line(0x007A); break;
		case Errors::NegativeMoveSpeed0:	   StringController::getStringf(0x007B, ret); log_line(0x007B); break;
		case Errors::NonPositiveMoveSpeed1:	   StringController::getStringf(0x007C, ret); log_line(0x007C); break;
		case Errors::NegativeZoomSpeed0:	   StringController::getStringf(0x007D, ret); log_line(0x007D); break;
		case Errors::NonPositiveZoomSpeed1:	   StringController::getStringf(0x007E, ret); log_line(0x007E); break;
		case Errors::NegativeBodyMass:		   StringController::getStringf(0x007F, ret); log_line(0x007F); break;
		case Errors::NegativeBodyRadius:	   StringController::getStringf(0x0080, ret); log_line(0x0080); break;
		case Errors::NegativeBodyTrailWidth:   StringController::getStringf(0x0081, ret); log_line(0x0081); break;
		case Errors::TooManyBodies:			   StringController::getStringf(0x0082, ret); log_line(0x0082); break;
		case Errors::CannotOpenLogFile:	       StringController::getStringf(0x00B3, ret); log_line(0x00B3); break;
		case Errors::CorruptedGlu32:	       StringController::getStringf(0x00B5, ret); log_line(0x00B5); break;
		case Errors::CorruptedOpengl32:		   StringController::getStringf(0x00B6, ret); log_line(0x00B6); break;
		case Errors::MissingGlu32:		       StringController::getStringf(0x00B7, ret); log_line(0x00B7); break;
		case Errors::MissingOpengl32:		   StringController::getStringf(0x00B8, ret); log_line(0x00B8); break;
		case Errors::OtherGlu32:		       StringController::getStringf(0x00B9, ret, shared->error_data_int); log_line(0x00B9, shared->error_data_int); break;
		case Errors::OtherOpengl32:		       StringController::getStringf(0x00BA, ret, shared->error_data_int); log_line(0x00BA, shared->error_data_int); break;
		case Errors::Other:					   StringController::getStringf(0x0083, ret); log_line(0x0083); break;
		default:							   StringController::getStringf(0x0084, ret); log_line(0x0084); break;
	}
	switch (shared->error)
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
			StringController::addString(0x0085, ret);
			break;
		case Errors::CannotOpenLogFile:
			StringController::addString(0x00B4, ret);
			break;
		case Errors::CorruptedGlu32:
		case Errors::CorruptedOpengl32:
		case Errors::MissingGlu32:
		case Errors::MissingOpengl32:
		case Errors::OtherGlu32:
		case Errors::OtherOpengl32:
			StringController::addString(0x00BB, ret);
			break;			
	}
	printf("%s\n", ret);
}
