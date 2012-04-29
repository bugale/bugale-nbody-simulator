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
		case Errors::WrongSettingsFileHeader:      StringController::getStringf(0x0072, ret); log_line(0x0072); break;
		case Errors::WrongBodyDataFileHeader:      StringController::getStringf(0x0073, ret); log_line(0x0073); break;
		case Errors::OldSettingsFileVersion:       StringController::getStringf(0x0074, ret); log_line(0x0074); break;
		case Errors::OldBodyDataFileVersion:       StringController::getStringf(0x0075, ret); log_line(0x0075); break;
		case Errors::NewSettingsFileVersion:       StringController::getStringf(0x00EB, ret); log_line(0x00EB); break;
		case Errors::NewBodyDataFileVersion:       StringController::getStringf(0x00EC, ret); log_line(0x00EC); break;
		case Errors::WrongSettingsFileSize:	       StringController::getStringf(0x0076, ret); log_line(0x0076); break;
		case Errors::WrongBodyDataFileSize:	       StringController::getStringf(0x0077, ret); log_line(0x0077); break;
		case Errors::WrongBodyDataFileSize2:       StringController::getStringf(0x0078, ret); log_line(0x0078); break;
		case Errors::NegativeDT:			       StringController::getStringf(0x0079, ret); log_line(0x0079); break;
		case Errors::NonPositiveFieldOfView:       StringController::getStringf(0x007A, ret); log_line(0x007A); break;
		case Errors::NegativeMoveSpeed0:	       StringController::getStringf(0x007B, ret); log_line(0x007B); break;
		case Errors::NonPositiveMoveSpeed1:	       StringController::getStringf(0x007C, ret); log_line(0x007C); break;
		case Errors::NegativeZoomSpeed0:	       StringController::getStringf(0x007D, ret); log_line(0x007D); break;
		case Errors::NonPositiveZoomSpeed1:	       StringController::getStringf(0x007E, ret); log_line(0x007E); break;
		case Errors::NegativeBodyMass:		       StringController::getStringf(0x007F, ret); log_line(0x007F); break;
		case Errors::NegativeBodyRadius:	       StringController::getStringf(0x0080, ret); log_line(0x0080); break;
		case Errors::NegativeBodyTrailWidth:       StringController::getStringf(0x0081, ret); log_line(0x0081); break;
		case Errors::TooManyBodies:			       StringController::getStringf(0x0082, ret); log_line(0x0082); break;
		case Errors::ZeroOpenCLThreads:			   StringController::getStringf(0x0150, ret); log_line(0x0150); break;
		case Errors::ZeroOpenCLThreadsInWorkgroup: StringController::getStringf(0x0151, ret); log_line(0x0151); break;
		case Errors::OpenCLThreadsNotDivisible:    StringController::getStringf(0x0151, ret); log_line(0x0187); break;
		case Errors::CorruptedGlu32:	           StringController::getStringf(0x00B5, ret); log_line(0x00B5); break;
		case Errors::CorruptedOpengl32:	    	   StringController::getStringf(0x00B6, ret); log_line(0x00B6); break;
		case Errors::MissingGlu32:		           StringController::getStringf(0x00B7, ret); log_line(0x00B7); break;
		case Errors::MissingOpengl32:		       StringController::getStringf(0x00B8, ret); log_line(0x00B8); break;
		case Errors::OtherGlu32:		           StringController::getStringf(0x00B9, ret, shared->error_data_int); log_line(0x00B9, shared->error_data_int); break;
		case Errors::OtherOpengl32:		           StringController::getStringf(0x00BA, ret, shared->error_data_int); log_line(0x00BA, shared->error_data_int); break;
		case Errors::MissinglibGLUdylib:	       StringController::getStringf(0x00DC, ret, shared->error_data_charptr); log_line(0x00DC, shared->error_data_charptr); break;
		case Errors::MissinglibOPENGLdylib:	       StringController::getStringf(0x00DD, ret, shared->error_data_charptr); log_line(0x00DD, shared->error_data_charptr); break;
		case Errors::MissinglibGLUso:		       StringController::getStringf(0x00DE, ret, shared->error_data_charptr); log_line(0x00DE, shared->error_data_charptr); break;
		case Errors::MissinglibOPENGLso:	       StringController::getStringf(0x00DF, ret, shared->error_data_charptr); log_line(0x00DF, shared->error_data_charptr); break;
		case Errors::OpenCLError:			       StringController::getStringf(0x0119, ret); log_line(0x0119); break;
		case Errors::Other:					       StringController::getStringf(0x0083, ret); log_line(0x0083); break;
		default:							       StringController::getStringf(0x0084, ret); log_line(0x0084); break;
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

void OpenCLErrors::returnError(OpenCLHandler* opencl, bool cont)
{
	char* ret = (char*)safe_malloc(4096, 0x00F0); ret[0] = 0;
	char* c = opencl->error_data_charptr;
	int   i = opencl->error_data_int;
	switch (opencl->error)
	{
		case OpenCLErrors::NoError: break;
		case OpenCLErrors::MissingOpenCL:                   StringController::getStringf(0x011C, ret); log_line(0x011C); break;
		case OpenCLErrors::CorruptedOpenCL:                 StringController::getStringf(0x011D, ret); log_line(0x011D); break;
		case OpenCLErrors::OtherOpenCL:                     StringController::getStringf(0x011E, ret, i); log_line(0x011E, i); break;
		case OpenCLErrors::MissinglibOpenCLdylib:           StringController::getStringf(0x011F, ret, c); log_line(0x011F, c); break;
		case OpenCLErrors::MissingOpenCLso:                 StringController::getStringf(0x0120, ret, c); log_line(0x0120, c); break;
		case OpenCLErrors::OutOfHostMemory:                 StringController::getStringf(0x00FE, ret, c); log_line(0x00FE, c); break;
		case OpenCLErrors::InvalidArgIndex:                 StringController::getStringf(0x00FF, ret, c); log_line(0x00FF, c); break;
		case OpenCLErrors::InvalidArgSize:                  StringController::getStringf(0x0100, ret, c); log_line(0x0100, c); break;
		case OpenCLErrors::InvalidArgValue:                 StringController::getStringf(0x0101, ret, c); log_line(0x0101, c); break;
		case OpenCLErrors::InvalidBinary:                   StringController::getStringf(0x0102, ret, c); log_line(0x0102, c); break;
		case OpenCLErrors::InvalidBufferSize:               StringController::getStringf(0x0103, ret, c); log_line(0x0103, c); break;
		case OpenCLErrors::InvalidBuildOptions:             StringController::getStringf(0x0104, ret, c); log_line(0x0104, c); break;
		case OpenCLErrors::InvalidCommandQueue:             StringController::getStringf(0x0155, ret, c); log_line(0x0155, c); break;
		case OpenCLErrors::InvalidContext:                  StringController::getStringf(0x0105, ret, c); log_line(0x0105, c); break;
		case OpenCLErrors::InvalidDevice:                   StringController::getStringf(0x0106, ret, c); log_line(0x0106, c); break;
		case OpenCLErrors::InvalidDeviceType:               StringController::getStringf(0x0107, ret, c); log_line(0x0107, c); break;
		case OpenCLErrors::InvalidEvent:                    StringController::getStringf(0x0108, ret, c); log_line(0x0108, c); break;
		case OpenCLErrors::InvalidEventWaitList:            StringController::getStringf(0x0109, ret, c); log_line(0x0109, c); break;
		case OpenCLErrors::InvalidGlobalOffset:             StringController::getStringf(0x010A, ret, c); log_line(0x010A, c); break;
		case OpenCLErrors::InvalidGlobalWorkSize:           StringController::getStringf(0x010B, ret, c); log_line(0x010B, c); break;
		case OpenCLErrors::InvalidGLObject:                 StringController::getStringf(0x010C, ret, c); log_line(0x010C, c); break;
		case OpenCLErrors::InvalidGLSharegroupReferenceKHR: StringController::getStringf(0x010D, ret, c); log_line(0x010D, c); break;
		case OpenCLErrors::InvalidHostPTR:                  StringController::getStringf(0x010E, ret, c); log_line(0x010E, c); break;
		case OpenCLErrors::InvalidImageFormatDescriptor:    StringController::getStringf(0x010F, ret, c); log_line(0x010F, c); break;
		case OpenCLErrors::InvalidImageSize:                StringController::getStringf(0x0110, ret, c); log_line(0x0110, c); break;
		case OpenCLErrors::InvalidKernel:                   StringController::getStringf(0x0111, ret, c); log_line(0x0111, c); break;
		case OpenCLErrors::InvalidKernelArgs:               StringController::getStringf(0x0112, ret, c); log_line(0x0112, c); break;
		case OpenCLErrors::InvalidKernelDefinition:         StringController::getStringf(0x0113, ret, c); log_line(0x0113, c); break;
		case OpenCLErrors::InvalidKernelName:               StringController::getStringf(0x0122, ret, c); log_line(0x0122, c); break;
		case OpenCLErrors::InvalidMemObject:                StringController::getStringf(0x0123, ret, c); log_line(0x0123, c); break;
		case OpenCLErrors::InvalidMIPLevel:                 StringController::getStringf(0x0124, ret, c); log_line(0x0124, c); break;
		case OpenCLErrors::InvalidOperation:                StringController::getStringf(0x0125, ret, c); log_line(0x0125, c); break;
		case OpenCLErrors::InvalidPlatform:                 StringController::getStringf(0x0126, ret, c); log_line(0x0126, c); break;
		case OpenCLErrors::InvalidProgram:                  StringController::getStringf(0x0127, ret, c); log_line(0x0127, c); break;
		case OpenCLErrors::InvalidProgramExecutable:        StringController::getStringf(0x0128, ret, c); log_line(0x0128, c); break;
		case OpenCLErrors::InvalidQueueProperties:          StringController::getStringf(0x0129, ret, c); log_line(0x0129, c); break;
		case OpenCLErrors::InvalidSampler:                  StringController::getStringf(0x012A, ret, c); log_line(0x012A, c); break;
		case OpenCLErrors::InvalidValue:                    StringController::getStringf(0x012B, ret, c); log_line(0x012B, c); break;
		case OpenCLErrors::InvalidWorkDimension:            StringController::getStringf(0x012C, ret, c); log_line(0x012C, c); break;
		case OpenCLErrors::InvalidWorkGroupSize:            StringController::getStringf(0x012D, ret, c); log_line(0x012D, c); break;
		case OpenCLErrors::InvalidWorkItemSize:             StringController::getStringf(0x012E, ret, c); log_line(0x012E, c); break;
		case OpenCLErrors::DeviceNotFound:                  StringController::getStringf(0x012F, ret, c); log_line(0x012F, c); break;
		case OpenCLErrors::DeviceNotAvailable:              StringController::getStringf(0x0130, ret, c); log_line(0x0130, c); break;
		case OpenCLErrors::MemObjectAllocationFailure:      StringController::getStringf(0x0131, ret, c); log_line(0x0131, c); break;
		case OpenCLErrors::MemCopyOverlap:                  StringController::getStringf(0x0132, ret, c); log_line(0x0132, c); break;
		case OpenCLErrors::ImageFormatMismatch:             StringController::getStringf(0x0133, ret, c); log_line(0x0133, c); break;
		case OpenCLErrors::ImageFormatNotSupported:         StringController::getStringf(0x0134, ret, c); log_line(0x0134, c); break;
		case OpenCLErrors::MapFailure:                      StringController::getStringf(0x0135, ret, c); log_line(0x0135, c); break;
		case OpenCLErrors::CompilerNotAvailable:            StringController::getStringf(0x0136, ret, c); log_line(0x0136, c); break;
		case OpenCLErrors::OutOfResources:                  StringController::getStringf(0x0137, ret, c); log_line(0x0137, c); break;
		case OpenCLErrors::ProfilingInfoNotAvailable:       StringController::getStringf(0x0138, ret, c); log_line(0x0138, c); break;
		case OpenCLErrors::BuildProgramFailure:             StringController::getStringf(0x0139, ret, c); log_line(0x0139, c); break;
		case OpenCLErrors::MisalignedSubBufferOffset:       StringController::getStringf(0x013A, ret, c); log_line(0x013A, c); break;
		case OpenCLErrors::ExecStatusErrorForEventsInWait:  StringController::getStringf(0x013B, ret, c); log_line(0x013B, c); break;
		case OpenCLErrors::Other:                           StringController::getStringf(0x0118, ret, i, c); log_line(0x0118, i, c); break;
		default:                                            StringController::getStringf(0x011A, ret, c); log_line(0x011A, c); break;
	}
	if (cont) StringController::addString(0x011B, ret);
	printf("%s\n", ret);
	free(ret);
}
