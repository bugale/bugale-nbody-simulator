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
		CannotOpenSettingsFile,
		CannotOpenBodyDataFile,
		WrongSettingsFileHeader,
		WrongBodyDataFileHeader,
		OldSettingsFileVersion,
		OldBodyDataFileVersion,
		NewSettingsFileVersion,
		NewBodyDataFileVersion,
		WrongSettingsFileSize,
		WrongBodyDataFileSize,
		WrongBodyDataFileSize2,
		NegativeDT,
		NonPositiveFieldOfView,
		NegativeMoveSpeed0,
		NonPositiveMoveSpeed1,
		NegativeZoomSpeed0,
		NonPositiveZoomSpeed1,
		ZeroOpenCLThreads,
		ZeroOpenCLThreadsInWorkgroup,
		OpenCLThreadsNotDivisible,
		NegativeBodyMass,
		NegativeBodyRadius,
		NegativeBodyTrailWidth,
		TooManyBodies,

		CannotOpenLogFile,
		
		CorruptedGlu32,
		CorruptedOpengl32,
		MissingGlu32,
		MissingOpengl32,
		OtherGlu32,
		OtherOpengl32,
		MissinglibGLUdylib,
		MissinglibOPENGLdylib,
		MissinglibGLUso,
		MissinglibOPENGLso,

		OpenCLError,

		Other
	};
}

namespace OpenCLErrors
{
	enum Error
	{
		NoError,

		MissingOpenCL,
		CorruptedOpenCL,
		OtherOpenCL,
		MissinglibOpenCLdylib,
		MissingOpenCLso,

		OutOfHostMemory,
		InvalidArgIndex,
		InvalidArgSize,
		InvalidArgValue,
		InvalidBinary,
		InvalidBufferSize,
		InvalidBuildOptions,
		InvalidCommandQueue,
		InvalidContext,
		InvalidDevice,
		InvalidDeviceType,
		InvalidEvent,
		InvalidEventWaitList,
		InvalidGlobalOffset,
		InvalidGlobalWorkSize,
		InvalidGLObject,
		InvalidGLSharegroupReferenceKHR,
		InvalidHostPTR,
		InvalidImageFormatDescriptor,
		InvalidImageSize,
		InvalidKernel,
		InvalidKernelArgs,
		InvalidKernelDefinition,
		InvalidKernelName,
		InvalidMemObject,
		InvalidMIPLevel,
		InvalidOperation,
		InvalidPlatform,
		InvalidProgram,
		InvalidProgramExecutable,
		InvalidQueueProperties,
		InvalidSampler,
		InvalidValue,
		InvalidWorkDimension,
		InvalidWorkGroupSize,
		InvalidWorkItemSize,
		DeviceNotFound,
		DeviceNotAvailable,
		MemObjectAllocationFailure,
		MemCopyOverlap,
		ImageFormatMismatch,
		ImageFormatNotSupported,
		MapFailure,
		CompilerNotAvailable,
		OutOfResources,
		ProfilingInfoNotAvailable,
		BuildProgramFailure,
		MisalignedSubBufferOffset,
		ExecStatusErrorForEventsInWait,

		NoPlatformsFound,
		NoDevicesFound,

		Other
	};
}

#endif
