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
	char* ret = (char*)safe_malloc(4096, 0x00CB); ret[0] = 0;
	switch (shared->error)
	{
		case Errors::NoError:
			break;
		case Errors::CannotOpenSettingsFile:
			sprintf(ret, StringController::getStringh(0x0070), shared->error_data_charptr);
			log_line(0x0070, shared->error_data_charptr);
			break;
		case Errors::CannotOpenBodyDataFile:
			sprintf(ret, StringController::getStringh(0x0071), shared->error_data_charptr);
			log_line(0x0071, shared->error_data_charptr);
			break;
		case Errors::CannotOpenLogFile:
			sprintf(ret, StringController::getStringh(0x00B3), shared->error_data_charptr);
			log_line(0x00B3, shared->error_data_charptr);
			break;
		case Errors::WrongSettingsFileHeader:  StringController::getStringf(0x0072, ret); log_line(0x0072); break;
		case Errors::WrongBodyDataFileHeader:  StringController::getStringf(0x0073, ret); log_line(0x0073); break;
		case Errors::OldSettingsFileVersion:   StringController::getStringf(0x0074, ret); log_line(0x0074); break;
		case Errors::OldBodyDataFileVersion:   StringController::getStringf(0x0075, ret); log_line(0x0075); break;
		case Errors::NewSettingsFileVersion:   StringController::getStringf(0x00EB, ret); log_line(0x00EB); break;
		case Errors::NewBodyDataFileVersion:   StringController::getStringf(0x00EC, ret); log_line(0x00EC); break;
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
		case Errors::CorruptedGlu32:	       StringController::getStringf(0x00B5, ret); log_line(0x00B5); break;
		case Errors::CorruptedOpengl32:		   StringController::getStringf(0x00B6, ret); log_line(0x00B6); break;
		case Errors::MissingGlu32:		       StringController::getStringf(0x00B7, ret); log_line(0x00B7); break;
		case Errors::MissingOpengl32:		   StringController::getStringf(0x00B8, ret); log_line(0x00B8); break;
		case Errors::OtherGlu32:		       StringController::getStringf(0x00B9, ret, shared->error_data_int); log_line(0x00B9, shared->error_data_int); break;
		case Errors::OtherOpengl32:		       StringController::getStringf(0x00BA, ret, shared->error_data_int); log_line(0x00BA, shared->error_data_int); break;
		case Errors::MissinglibGLUdylib:	   StringController::getStringf(0x00DC, ret, shared->error_data_charptr); log_line(0x00DC, shared->error_data_charptr); break;
		case Errors::MissinglibOPENGLdylib:	   StringController::getStringf(0x00DD, ret, shared->error_data_charptr); log_line(0x00DD, shared->error_data_charptr); break;
		case Errors::MissinglibGLUso:		   StringController::getStringf(0x00DE, ret, shared->error_data_charptr); log_line(0x00DE, shared->error_data_charptr); break;
		case Errors::MissinglibOPENGLso:	   StringController::getStringf(0x00DF, ret, shared->error_data_charptr); log_line(0x00DF, shared->error_data_charptr); break;
		case Errors::CUDAError:				   StringController::getStringf(0x0119, ret); log_line(0x0119); break;
		case Errors::Other:					   StringController::getStringf(0x0083, ret); log_line(0x0083); break;
		default:							   StringController::getStringf(0x0084, ret); log_line(0x0084); break;
	}
	switch (shared->error)
	{
		case Errors::CannotOpenSettingsFile:
		case Errors::CannotOpenBodyDataFile:
		case Errors::WrongSettingsFileHeader:
		case Errors::WrongBodyDataFileHeader:
		case Errors::OldSettingsFileVersion:
		case Errors::OldBodyDataFileVersion:
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
		case Errors::NewSettingsFileVersion:
		case Errors::NewBodyDataFileVersion:
			StringController::addString(0x00ED, ret);
			break;
	}
	printf("%s\n", ret);
	free(ret);
}

void CUDAErrors::returnError(CUDAHandler* cuda, bool cont)
{
	char* ret = (char*)safe_malloc(4096, 0x00F0); ret[0] = 0;
	char* c = cuda->error_data_charptr;
	int   i = cuda->error_data_int;
	switch (cuda->error)
	{
		case CUDAErrors::NoError: break;
		case CUDAErrors::MissingNvcuda:               StringController::getStringf(0x011C, ret); log_line(0x00FE); break;
		case CUDAErrors::CorruptedNvcuda:             StringController::getStringf(0x011D, ret); log_line(0x00FE); break;
		case CUDAErrors::OtherNvcuda:                 StringController::getStringf(0x011E, ret, i); log_line(0x00FE, i); break;
		case CUDAErrors::MissinglibCUDAdylib:         StringController::getStringf(0x011F, ret, c); log_line(0x00FE, c); break;
		case CUDAErrors::MissingCUDAGLUso:            StringController::getStringf(0x0120, ret, c); log_line(0x00FE, c); break;
		case CUDAErrors::InvalidValue:                StringController::getStringf(0x00FE, ret, c); log_line(0x00FE, c); break;
		case CUDAErrors::OutOfMemory:                 StringController::getStringf(0x00FF, ret, c); log_line(0x00FF, c); break;
		case CUDAErrors::NotInitialized:              StringController::getStringf(0x0100, ret, c); log_line(0x0100, c); break;
		case CUDAErrors::Deinitalized:                StringController::getStringf(0x0101, ret, c); log_line(0x0101, c); break;
		case CUDAErrors::NoDevice:                    StringController::getStringf(0x0102, ret, c); log_line(0x0102, c); break;
		case CUDAErrors::InvalidDevice:               StringController::getStringf(0x0103, ret, c); log_line(0x0103, c); break;
		case CUDAErrors::InvalidImage:                StringController::getStringf(0x0104, ret, c); log_line(0x0104, c); break;
		case CUDAErrors::InvalidContext:              StringController::getStringf(0x0105, ret, c); log_line(0x0105, c); break;
		case CUDAErrors::ContextAlreadyCurrent:       StringController::getStringf(0x0106, ret, c); log_line(0x0106, c); break;
		case CUDAErrors::MapFailed:                   StringController::getStringf(0x0107, ret, c); log_line(0x0107, c); break;
		case CUDAErrors::UnmapFailed:                 StringController::getStringf(0x0108, ret, c); log_line(0x0108, c); break;
		case CUDAErrors::ArrayIsMapped:               StringController::getStringf(0x0109, ret, c); log_line(0x0109, c); break;
		case CUDAErrors::AlreadyMapped:               StringController::getStringf(0x010A, ret, c); log_line(0x010A, c); break;
		case CUDAErrors::NoBinaryForGPU:              StringController::getStringf(0x010B, ret, c); log_line(0x010B, c); break;
		case CUDAErrors::AlreadyAcquired:             StringController::getStringf(0x010C, ret, c); log_line(0x010C, c); break;
		case CUDAErrors::NotMapped:                   StringController::getStringf(0x010D, ret, c); log_line(0x010D, c); break;
		case CUDAErrors::InvalidSource:               StringController::getStringf(0x010E, ret, c); log_line(0x010E, c); break;
		case CUDAErrors::FileNotFound:                StringController::getStringf(0x010F, ret, c); log_line(0x010F, c); break;
		case CUDAErrors::InvalidHandle:               StringController::getStringf(0x0110, ret, c); log_line(0x0110, c); break;
		case CUDAErrors::NotFound:                    StringController::getStringf(0x0111, ret, c); log_line(0x0111, c); break;
		case CUDAErrors::NotReady:                    StringController::getStringf(0x0112, ret, c); log_line(0x0112, c); break;
		case CUDAErrors::LaunchFailed:                StringController::getStringf(0x0113, ret, c); log_line(0x0113, c); break;
		case CUDAErrors::LaunchOutOfResources:        StringController::getStringf(0x0114, ret, c); log_line(0x0114, c); break;
		case CUDAErrors::LaunchTimeout:               StringController::getStringf(0x0115, ret, c); log_line(0x0115, c); break;
		case CUDAErrors::LaunchIncompatibleTexturing: StringController::getStringf(0x0116, ret, c); log_line(0x0116, c); break;
		case CUDAErrors::Unknown:                     StringController::getStringf(0x0117, ret, c); log_line(0x0117, c); break;
		case CUDAErrors::Other:                       StringController::getStringf(0x0118, ret, i, c); log_line(0x0118, i, c); break;
		default:                                      StringController::getStringf(0x011A, ret, c); log_line(0x0119, c); break;
	}
	if (cont) StringController::addString(0x011B, ret);
	printf("%s\n", ret);
	free(ret);
}
