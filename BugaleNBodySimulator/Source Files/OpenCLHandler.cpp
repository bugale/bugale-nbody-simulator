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
#include "..\OpenCL Kernels\KernelsData.h"

OpenCLHandler::OpenCLHandler(SharedData* shared, Data* data)
{
	log_line(0x00E0, shared, data);
	this->algorithm = data->algorithm;
	this->two_dimensional = data->two_dimensional_calculation;
	this->bodies = data->bodies;
	this->num_of_bodies = data->num_of_bodies;
	this->dt = data->dt;
	this->g = data->g;
	this->error = OpenCLErrors::NoError;
	this->num_of_threads = data->cl_num_of_threads;
	this->num_of_threads_in_workgroup = data->cl_threads_in_workgroup;
	this->num_of_calcs_in_run = data->cl_calcs_in_run;

	if (this->two_dimensional)
		switch (this->algorithm)
		{
			case 0x00: this->kernel_name = "ModifiedEuler2DKernel"; break;
			case 0x01: this->kernel_name = "ModifiedEuler2DKernel"; StringController::printString(0x017A, "Hermite", "ModifiedEuler"); data->algorithm = 0x00; break;
			case 0x02: this->kernel_name = "Euler2DKernel"; break;
			case 0x03: this->kernel_name = "Leapfrog2DKernel"; break;
		}
	else
		switch (this->algorithm)
		{
			case 0x00: this->kernel_name = "ModifiedEuler3DKernel"; break;
			case 0x01: this->kernel_name = "ModifiedEuler3DKernel"; StringController::printString(0x017A, "Hermite", "ModifiedEuler"); data->algorithm = 0x00; break;
			case 0x02: this->kernel_name = "Euler3DKernel"; break;
			case 0x03: this->kernel_name = "Leapfrog3DKernel"; break;
		}
	data->algorithm_name = get_algorithm_name(data->algorithm);

	this->Precalculations();

	this->initial_energy_sum_2d = this->GetEnergySum2D();
	this->initial_energy_sum_3d = this->GetEnergySum3D();
	this->initial_momentum_sum_2d = this->GetMomentumSum2D();
	this->initial_momentum_sum_3d = this->GetMomentumSum3D();
	

	log_line(0x00E1, this);
}
OpenCLHandler::~OpenCLHandler()
{
	if (this->dt_div_mass != 0) free(this->dt_div_mass);
	if (this->mass_mul_g != 0) free(this->mass_mul_g);
	if (this->dt_div_2mass != 0) free(this->dt_div_2mass);
	if (this->dt_squared_div_2mass != 0) free(this->dt_squared_div_2mass);
	if (this->dt_squared_div_12mass != 0) free(this->dt_squared_div_12mass);
	if (this->dt_pow_3_div_6mass != 0) free(this->dt_pow_3_div_6mass);
	if (this->posX != 0) free(this->posX);
	if (this->posY != 0) free(this->posY);
	if (this->posZ != 0) free(this->posZ);
	if (this->velX != 0) free(this->velX);
	if (this->velY != 0) free(this->velY);
	if (this->velZ != 0) free(this->velZ);
	if (this->mass != 0) free(this->mass);
}

bool OpenCLHandler::getError(cl_int r, OpenCLCaller::Caller c)
{
	switch (c)
	{
		case OpenCLCaller::GetPlatformIDsCount:            this->error_data_charptr = StringController::getStringh(0x00F1); break;
		case OpenCLCaller::GetPlatformIDs:                 this->error_data_charptr = StringController::getStringh(0x00F2); break;
		case OpenCLCaller::GetDeviceIDsCount:              this->error_data_charptr = StringController::getStringh(0x00F3); break;
		case OpenCLCaller::GetDeviceIDs:                   this->error_data_charptr = StringController::getStringh(0x00F4); break;
		case OpenCLCaller::CreateContext:                  this->error_data_charptr = StringController::getStringh(0x00F5); break;
		case OpenCLCaller::CreateCommandQueue:             this->error_data_charptr = StringController::getStringh(0x00F6); break;
		case OpenCLCaller::CreateProgram:                  this->error_data_charptr = StringController::getStringh(0x00F8); break;
		case OpenCLCaller::BuildProgram:                   this->error_data_charptr = StringController::getStringh(0x00F9); break;
		case OpenCLCaller::GetProgramBuildLogSize:         this->error_data_charptr = StringController::getStringh(0x00FA); break;
		case OpenCLCaller::GetProgramBuildLog:             this->error_data_charptr = StringController::getStringh(0x00FB); break;
		case OpenCLCaller::CreateKernel:                   this->error_data_charptr = StringController::getStringh(0x00FC); break;
		case OpenCLCaller::CreateBufferGlobPosX:           this->error_data_charptr = StringController::getStringh(0x00FD); break;
		case OpenCLCaller::CreateBufferGlobPosY:           this->error_data_charptr = StringController::getStringh(0x0121); break;
		case OpenCLCaller::CreateBufferGlobPosZ:           this->error_data_charptr = StringController::getStringh(0x0143); break;
		case OpenCLCaller::CreateBufferGlobVelX:           this->error_data_charptr = StringController::getStringh(0x0144); break;
		case OpenCLCaller::CreateBufferGlobVelY:           this->error_data_charptr = StringController::getStringh(0x0145); break;
		case OpenCLCaller::CreateBufferGlobVelZ:           this->error_data_charptr = StringController::getStringh(0x014C); break;
		case OpenCLCaller::CreateBufferGlobMass:           this->error_data_charptr = StringController::getStringh(0x014D); break;
		case OpenCLCaller::CreateBufferDtDivMass:          this->error_data_charptr = StringController::getStringh(0x0161); break;
		case OpenCLCaller::CreateBufferMassMulG:           this->error_data_charptr = StringController::getStringh(0x0162); break;
		case OpenCLCaller::CreateBufferDtDiv2Mass:         this->error_data_charptr = StringController::getStringh(0x0163); break;
		case OpenCLCaller::CreateBufferDtSquaredDiv2Mass:  this->error_data_charptr = StringController::getStringh(0x0164); break;
		case OpenCLCaller::CreateBufferDtSquaredDiv12Mass: this->error_data_charptr = StringController::getStringh(0x0165); break;
		case OpenCLCaller::CreateBufferDtPow3Div6Mass:     this->error_data_charptr = StringController::getStringh(0x0166); break;
		case OpenCLCaller::CreateBufferOutPosX:            this->error_data_charptr = StringController::getStringh(0x0156); break;
		case OpenCLCaller::CreateBufferOutPosY:            this->error_data_charptr = StringController::getStringh(0x015B); break;
		case OpenCLCaller::CreateBufferOutPosZ:            this->error_data_charptr = StringController::getStringh(0x0167); break;
		case OpenCLCaller::CreateBufferOutVelX:            this->error_data_charptr = StringController::getStringh(0x0168); break;
		case OpenCLCaller::CreateBufferOutVelY:            this->error_data_charptr = StringController::getStringh(0x0169); break;
		case OpenCLCaller::CreateBufferOutVelZ:            this->error_data_charptr = StringController::getStringh(0x016A); break;
		case OpenCLCaller::CreateBufferLVelX:              this->error_data_charptr = StringController::getStringh(0x01A6); break;
		case OpenCLCaller::CreateBufferLVelY:              this->error_data_charptr = StringController::getStringh(0x01A7); break;
		case OpenCLCaller::CreateBufferLVelZ:              this->error_data_charptr = StringController::getStringh(0x01A8); break;
		case OpenCLCaller::CreateBufferLForceX:            this->error_data_charptr = StringController::getStringh(0x01A9); break;
		case OpenCLCaller::CreateBufferLForceY:            this->error_data_charptr = StringController::getStringh(0x01AA); break;
		case OpenCLCaller::CreateBufferLForceZ:            this->error_data_charptr = StringController::getStringh(0x01AB); break;
		case OpenCLCaller::CreateBufferLJerkX:             this->error_data_charptr = StringController::getStringh(0x01AC); break;
		case OpenCLCaller::CreateBufferLJerkY:             this->error_data_charptr = StringController::getStringh(0x01AD); break;
		case OpenCLCaller::CreateBufferLJerkZ:             this->error_data_charptr = StringController::getStringh(0x01AE); break;
		case OpenCLCaller::SetKernelArgGlobPosX:           this->error_data_charptr = StringController::getStringh(0x016B); break;
		case OpenCLCaller::SetKernelArgGlobPosY:           this->error_data_charptr = StringController::getStringh(0x0176); break;
		case OpenCLCaller::SetKernelArgGlobPosZ:           this->error_data_charptr = StringController::getStringh(0x0177); break;
		case OpenCLCaller::SetKernelArgGlobVelX:           this->error_data_charptr = StringController::getStringh(0x0178); break;
		case OpenCLCaller::SetKernelArgGlobVelY:           this->error_data_charptr = StringController::getStringh(0x017C); break;
		case OpenCLCaller::SetKernelArgGlobVelZ:           this->error_data_charptr = StringController::getStringh(0x017D); break;
		case OpenCLCaller::SetKernelArgGlobMass:           this->error_data_charptr = StringController::getStringh(0x0182); break;
		case OpenCLCaller::SetKernelArgDtDivMass:          this->error_data_charptr = StringController::getStringh(0x00EA); break;
		case OpenCLCaller::SetKernelArgMassMulG:           this->error_data_charptr = StringController::getStringh(0x00F7); break;
		case OpenCLCaller::SetKernelArgDtDiv2Mass:         this->error_data_charptr = StringController::getStringh(0x016C); break;
		case OpenCLCaller::SetKernelArgDtSquaredDiv2Mass:  this->error_data_charptr = StringController::getStringh(0x016D); break;
		case OpenCLCaller::SetKernelArgDtSquaredDiv12Mass: this->error_data_charptr = StringController::getStringh(0x016E); break;
		case OpenCLCaller::SetKernelArgDtPow3Div6Mass:     this->error_data_charptr = StringController::getStringh(0x016F); break;
		case OpenCLCaller::SetKernelArgOutPosX:            this->error_data_charptr = StringController::getStringh(0x0183); break;
		case OpenCLCaller::SetKernelArgOutPosY:            this->error_data_charptr = StringController::getStringh(0x0184); break;
		case OpenCLCaller::SetKernelArgOutPosZ:            this->error_data_charptr = StringController::getStringh(0x0185); break;
		case OpenCLCaller::SetKernelArgOutVelX:            this->error_data_charptr = StringController::getStringh(0x0186); break;
		case OpenCLCaller::SetKernelArgOutVelY:            this->error_data_charptr = StringController::getStringh(0x0187); break;
		case OpenCLCaller::SetKernelArgOutVelZ:            this->error_data_charptr = StringController::getStringh(0x0188); break;
		case OpenCLCaller::SetKernelArgLocPosX:            this->error_data_charptr = StringController::getStringh(0x017F); break;
		case OpenCLCaller::SetKernelArgLocPosY:            this->error_data_charptr = StringController::getStringh(0x0180); break;
		case OpenCLCaller::SetKernelArgLocPosZ:            this->error_data_charptr = StringController::getStringh(0x0181); break;
		case OpenCLCaller::SetKernelArgLocVelX:            this->error_data_charptr = StringController::getStringh(0x0171); break;
		case OpenCLCaller::SetKernelArgLocVelY:            this->error_data_charptr = StringController::getStringh(0x0172); break;
		case OpenCLCaller::SetKernelArgLocVelZ:            this->error_data_charptr = StringController::getStringh(0x0173); break;
		case OpenCLCaller::SetKernelArgLVelX:              this->error_data_charptr = StringController::getStringh(0x01AF); break;
		case OpenCLCaller::SetKernelArgLVelY:              this->error_data_charptr = StringController::getStringh(0x01B0); break;
		case OpenCLCaller::SetKernelArgLVelZ:              this->error_data_charptr = StringController::getStringh(0x01B1); break;
		case OpenCLCaller::SetKernelArgLForceX:            this->error_data_charptr = StringController::getStringh(0x01B2); break;
		case OpenCLCaller::SetKernelArgLForceY:            this->error_data_charptr = StringController::getStringh(0x01B3); break;
		case OpenCLCaller::SetKernelArgLForceZ:            this->error_data_charptr = StringController::getStringh(0x01B4); break;
		case OpenCLCaller::SetKernelArgLJerkX:             this->error_data_charptr = StringController::getStringh(0x01B5); break;
		case OpenCLCaller::SetKernelArgLJerkY:             this->error_data_charptr = StringController::getStringh(0x01B6); break;
		case OpenCLCaller::SetKernelArgLJerkZ:             this->error_data_charptr = StringController::getStringh(0x01B7); break;
		case OpenCLCaller::EnqueueKernel:                  this->error_data_charptr = StringController::getStringh(0x014B); break;
		case OpenCLCaller::ReadBufferOutPosX:              this->error_data_charptr = StringController::getStringh(0x014E); break;
		case OpenCLCaller::ReadBufferOutPosY:              this->error_data_charptr = StringController::getStringh(0x0189); break;
		case OpenCLCaller::ReadBufferOutPosZ:              this->error_data_charptr = StringController::getStringh(0x018A); break;
		case OpenCLCaller::ReadBufferOutVelX:              this->error_data_charptr = StringController::getStringh(0x018B); break;
		case OpenCLCaller::ReadBufferOutVelY:              this->error_data_charptr = StringController::getStringh(0x018C); break;
		case OpenCLCaller::ReadBufferOutVelZ:              this->error_data_charptr = StringController::getStringh(0x018D); break;
		case OpenCLCaller::GetPlatformInfoVendor:          this->error_data_charptr = StringController::getStringh(0x0196); break;
		case OpenCLCaller::GetPlatformInfoName:            this->error_data_charptr = StringController::getStringh(0x0197); break;
		case OpenCLCaller::GetPlatformInfoVersion:         this->error_data_charptr = StringController::getStringh(0x0198); break;
		case OpenCLCaller::GetDeviceInfoVendor:            this->error_data_charptr = StringController::getStringh(0x0199); break;
		case OpenCLCaller::GetDeviceInfoName:              this->error_data_charptr = StringController::getStringh(0x019A); break;
		case OpenCLCaller::GetDeviceInfoVersion:           this->error_data_charptr = StringController::getStringh(0x019B); break;
		case OpenCLCaller::GetDeviceInfoCache:             this->error_data_charptr = StringController::getStringh(0x0146); break;
		default: this->error = OpenCLErrors::Other; return true;
	}
	switch (r)
	{
		case CL_SUCCESS:                                   this->error = OpenCLErrors::NoError; return false;
		case CL_OUT_OF_HOST_MEMORY:                        this->error = OpenCLErrors::OutOfHostMemory; break;
		case CL_INVALID_ARG_INDEX:                         this->error = OpenCLErrors::InvalidArgIndex; break;
		case CL_INVALID_ARG_SIZE:                          this->error = OpenCLErrors::InvalidArgSize; break;
		case CL_INVALID_ARG_VALUE:                         this->error = OpenCLErrors::InvalidArgValue; break;
		case CL_INVALID_BINARY:                            this->error = OpenCLErrors::InvalidBinary; break;
		case CL_INVALID_BUFFER_SIZE:                       this->error = OpenCLErrors::InvalidBufferSize; break;
		case CL_INVALID_BUILD_OPTIONS:                     this->error = OpenCLErrors::InvalidBuildOptions; break;
		case CL_INVALID_COMMAND_QUEUE:                     this->error = OpenCLErrors::InvalidCommandQueue; break;
		case CL_INVALID_CONTEXT:                           this->error = OpenCLErrors::InvalidContext; break;
		case CL_INVALID_DEVICE:                            this->error = OpenCLErrors::InvalidDevice; break;
		case CL_INVALID_DEVICE_TYPE:                       this->error = OpenCLErrors::InvalidDeviceType; break;
		case CL_INVALID_EVENT:                             this->error = OpenCLErrors::InvalidEvent; break;
		case CL_INVALID_EVENT_WAIT_LIST:                   this->error = OpenCLErrors::InvalidEventWaitList; break;
		case CL_INVALID_GLOBAL_OFFSET:                     this->error = OpenCLErrors::InvalidGlobalOffset; break;
		case CL_INVALID_GLOBAL_WORK_SIZE:                  this->error = OpenCLErrors::InvalidGlobalWorkSize; break;
		case CL_INVALID_GL_OBJECT:                         this->error = OpenCLErrors::InvalidGLObject; break;
		case CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR:       this->error = OpenCLErrors::InvalidGLSharegroupReferenceKHR; break;
		case CL_INVALID_HOST_PTR:                          this->error = OpenCLErrors::InvalidHostPTR; break;
		case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:           this->error = OpenCLErrors::InvalidImageFormatDescriptor; break;
		case CL_INVALID_IMAGE_SIZE:                        this->error = OpenCLErrors::InvalidImageSize; break;
		case CL_INVALID_KERNEL:                            this->error = OpenCLErrors::InvalidKernel; break;
		case CL_INVALID_KERNEL_ARGS:                       this->error = OpenCLErrors::InvalidKernelArgs; break;
		case CL_INVALID_KERNEL_DEFINITION:                 this->error = OpenCLErrors::InvalidKernelDefinition; break;
		case CL_INVALID_KERNEL_NAME:                       this->error = OpenCLErrors::InvalidKernelName; break;
		case CL_INVALID_MEM_OBJECT:                        this->error = OpenCLErrors::InvalidMemObject; break;
		case CL_INVALID_MIP_LEVEL:                         this->error = OpenCLErrors::InvalidMIPLevel; break;
		case CL_INVALID_OPERATION:                         this->error = OpenCLErrors::InvalidOperation; break;
		case CL_INVALID_PLATFORM:                          this->error = OpenCLErrors::InvalidPlatform; break;
		case CL_INVALID_PROGRAM:                           this->error = OpenCLErrors::InvalidProgram; break;
		case CL_INVALID_PROGRAM_EXECUTABLE:                this->error = OpenCLErrors::InvalidProgramExecutable; break;
		case CL_INVALID_QUEUE_PROPERTIES:                  this->error = OpenCLErrors::InvalidQueueProperties; break;
		case CL_INVALID_SAMPLER:                           this->error = OpenCLErrors::InvalidSampler; break;
		case CL_INVALID_VALUE:                             this->error = OpenCLErrors::InvalidValue; break;
		case CL_INVALID_WORK_DIMENSION:                    this->error = OpenCLErrors::InvalidWorkDimension; break;
		case CL_INVALID_WORK_GROUP_SIZE:                   this->error = OpenCLErrors::InvalidWorkGroupSize; break;
		case CL_INVALID_WORK_ITEM_SIZE:                    this->error = OpenCLErrors::InvalidWorkItemSize; break;
		case CL_DEVICE_NOT_FOUND:                          this->error = OpenCLErrors::DeviceNotFound; break;
		case CL_DEVICE_NOT_AVAILABLE:                      this->error = OpenCLErrors::DeviceNotAvailable; break;
		case CL_MEM_OBJECT_ALLOCATION_FAILURE:             this->error = OpenCLErrors::MemObjectAllocationFailure; break;
		case CL_MEM_COPY_OVERLAP:                          this->error = OpenCLErrors::MemCopyOverlap; break;
		case CL_IMAGE_FORMAT_MISMATCH:                     this->error = OpenCLErrors::ImageFormatMismatch; break;
		case CL_IMAGE_FORMAT_NOT_SUPPORTED:                this->error = OpenCLErrors::ImageFormatNotSupported; break;
		case CL_MAP_FAILURE:                               this->error = OpenCLErrors::MapFailure; break;
		case CL_COMPILER_NOT_AVAILABLE:                    this->error = OpenCLErrors::CompilerNotAvailable; break;
		case CL_OUT_OF_RESOURCES:                          this->error = OpenCLErrors::OutOfResources; break;
		case CL_PROFILING_INFO_NOT_AVAILABLE:              this->error = OpenCLErrors::ProfilingInfoNotAvailable; break;
		case CL_BUILD_PROGRAM_FAILURE:                     this->error = OpenCLErrors::BuildProgramFailure; break;
		case CL_MISALIGNED_SUB_BUFFER_OFFSET:              this->error = OpenCLErrors::MisalignedSubBufferOffset; break;
		case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST: this->error = OpenCLErrors::ExecStatusErrorForEventsInWait; break;
		default: this->error = OpenCLErrors::Other;        this->error_data_int = r; break;
	}
	return true;
}

bool OpenCLHandler::GetPlatform()
{
	cl_uint platform_count;
	cl_platform_id* platforms;
	cl_int err;
	char* name = (char*)safe_malloc(4096, 0x0190);
	size_t cur = 0; size_t t_cur = 0; unsigned int selected = 0;

	err = clGetPlatformIDs(0, NULL, &platform_count);
	if (this->getError(err, OpenCLCaller::GetPlatformIDsCount)) return true;
	if (platform_count <= 0) { this->error = OpenCLErrors::NoPlatformsFound; return true; }

	platforms = (cl_platform_id*)safe_malloc(sizeof(cl_platform_id)*platform_count, 0x013C);
	err = clGetPlatformIDs(platform_count, platforms, NULL);
	if (this->getError(err, OpenCLCaller::GetPlatformIDs)) return true;
	
	if (platform_count == 1)
	{
		this->_platform = platforms[0];
		free(platforms);
		free(name);
		return false;
	}

	StringController::printString(0x018E);
	for (unsigned int i = 0; i < platform_count; i++)
	{
		err = clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, 4096, name, &t_cur); cur += t_cur - 1; name[cur++] = ' ';
		if (this->getError(err, OpenCLCaller::GetPlatformInfoVendor)) return true;
		err = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 4096 - cur, (void*)((int)name + cur), &t_cur); cur += t_cur - 1; name[cur++] = ' ';
		if (this->getError(err, OpenCLCaller::GetPlatformInfoName)) return true;
		err = clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, 4096 - cur, (void*)((int)name + cur), &t_cur); cur += t_cur - 1; name[cur++] = '\n'; name[cur] = 0;
		if (this->getError(err, OpenCLCaller::GetPlatformInfoVersion)) return true;
		printf("[%d] %s", i, name);
	}
	StringController::printString(0x0191);

	while (scanf("%u", &selected) != 1 || selected < 0 || selected >= platform_count) StringController::printString(0x0192);
	this->_platform = platforms[selected];
	free(platforms);
	free(name);
	return false;
}
bool OpenCLHandler::GetDevice()
{
	cl_uint device_count;
	cl_device_id* devices;
	cl_int err;
	char* name = (char*)safe_malloc(4096, 0x0193);
	size_t cur = 0; size_t t_cur = 0; unsigned int selected = 0;
	
	err = clGetDeviceIDs(this->_platform, CL_DEVICE_TYPE_GPU, 0, NULL, &device_count);
	if (this->getError(err, OpenCLCaller::GetDeviceIDsCount)) return true;
	if (device_count == 0) { this->error = OpenCLErrors::NoDevicesFound; return true; }

	devices = (cl_device_id*)safe_malloc(sizeof(cl_device_id)*device_count, 0x013F);
	err = clGetDeviceIDs(this->_platform, CL_DEVICE_TYPE_GPU, device_count, devices, NULL);
	if (this->getError(err, OpenCLCaller::GetDeviceIDs)) return true;
	
	if (device_count == 1)
	{
		this->_device = devices[0];

		cl_ulong cache;
		err = clGetDeviceInfo(this->_device, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &cache, NULL);
		if (this->getError(err, OpenCLCaller::GetDeviceInfoCache)) return true;
		this->cache_size = (int)(cache>>4);
		if (this->two_dimensional)   this->cache_size >>= 1;
		if (this->algorithm == 0x01) this->cache_size >>= 1; //Hermite

		free(devices);
		free(name);
		return false;
	}
	
	StringController::printString(0x0194);
	for (unsigned int i = 0; i < device_count; i++)
	{
		err = clGetDeviceInfo(devices[i], CL_DEVICE_VENDOR, 4096, name, &t_cur); cur += t_cur - 1; name[cur++] = ' ';
		if (this->getError(err, OpenCLCaller::GetDeviceInfoVendor)) return true;
		err = clGetDeviceInfo(devices[i], CL_DEVICE_NAME, 4096 - cur, (void*)((int)name + cur), &t_cur); cur += t_cur - 1; name[cur++] = ' ';
		if (this->getError(err, OpenCLCaller::GetDeviceInfoName)) return true;
		err = clGetDeviceInfo(devices[i], CL_DEVICE_VERSION, 4096 - cur, (void*)((int)name + cur), &t_cur); cur += t_cur - 1; name[cur++] = '\n'; name[cur] = 0;
		if (this->getError(err, OpenCLCaller::GetDeviceInfoVersion)) return true;
		printf("[%d] %s", i, name);
	}
	StringController::printString(0x0195);

	while (scanf("%u", &selected) != 1 || selected < 0 || selected >= device_count) StringController::printString(0x0192);
	this->_device = devices[selected];

	cl_ulong cache;
	err = clGetDeviceInfo(this->_device, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &cache, NULL);
	if (this->getError(err, OpenCLCaller::GetDeviceInfoCache)) return true;
	this->cache_size = (int)(cache>>4);
	if (this->two_dimensional)   this->cache_size >>= 1;
	if (this->algorithm == 0x01) this->cache_size >>= 1; //Hermite

	free(devices);
	free(name);
	return false;
}
bool OpenCLHandler::GetContext()
{
	cl_int err;
	this->_context = clCreateContext(0, 1, &this->_device, NULL, NULL, &err);
	if (this->getError(err, OpenCLCaller::CreateContext)) return true;
	return false;
}
bool OpenCLHandler::GetCommandQueue()
{
	cl_int err;

	this->_queue = clCreateCommandQueue(this->_context, this->_device, NULL, &err);
	if (this->getError(err, OpenCLCaller::CreateCommandQueue)) return true;

	return false;
}
bool OpenCLHandler::GetProgram()
{
	cl_int err;
	char* source = (char*)safe_malloc(strlen(KERNEL_SOURCE)<<2, 0x0147);
	char* var = (char*)safe_malloc(256, 0x0170);
	
	#pragma region ReplaceConstants
		int cur = 0;
		char* occ;
		for (unsigned int i = 0; i < strlen(KERNEL_SOURCE); i++)
		{
			if (KERNEL_SOURCE[i] != '%') source[cur++] = KERNEL_SOURCE[i];
			else
			{
				occ = strchr(KERNEL_SOURCE+i+1, '%');
				strncpy(var, KERNEL_SOURCE+i+1, occ - KERNEL_SOURCE-i-1);
				var[occ - KERNEL_SOURCE-i-1] = 0;
				i = occ - KERNEL_SOURCE;

				if (strcmp(var, "dt") == 0)             cur += sprintf(source+cur, "%e", this->dt);
				if (strcmp(var, "cache_size") == 0)     cur += sprintf(source+cur, "%d", this->cache_size);
				if (strcmp(var, "num_of_bodies") == 0)  cur += sprintf(source+cur, "%d", this->num_of_bodies);
				if (strcmp(var, "num_of_threads") == 0) cur += sprintf(source+cur, "%d", this->num_of_threads);
				if (strcmp(var, "calculations") == 0)   cur += sprintf(source+cur, "%d", this->num_of_calcs_in_run);
				if (strcmp(var, "workgroup_size") == 0) cur += sprintf(source+cur, "%d", this->num_of_threads_in_workgroup);
			}
		}
		source[cur] = 0;
	#pragma endregion
	
	const size_t length[] = {strlen(source)};
	this->_program = clCreateProgramWithSource(this->_context, 1, (const char **)&source, length, &err);
	if (this->getError(err, OpenCLCaller::CreateProgram)) return true;

	cl_int t_err = clBuildProgram(this->_program, 1, &this->_device, "-cl-mad-enable -cl-fast-relaxed-math", NULL, NULL);

	char* build_log;
	size_t log_size;
	err = clGetProgramBuildInfo(this->_program, this->_device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
	if (this->getError(err, OpenCLCaller::GetProgramBuildLogSize)) return true;
	build_log = (char*)safe_malloc(log_size+1, 0x013D);
	err = clGetProgramBuildInfo(this->_program, this->_device, CL_PROGRAM_BUILD_LOG, log_size, build_log, NULL);
	if (this->getError(err, OpenCLCaller::GetProgramBuildLog)) return true;
	build_log[log_size] = '\0';
	log_line(0x013E, build_log);
	free(build_log);

	if (this->getError(t_err, OpenCLCaller::BuildProgram)) return true;

	free(var);
	free(source);
	return false;
}
bool OpenCLHandler::GetKernel()
{
	cl_int err;

	this->_kernel = clCreateKernel(this->_program, this->kernel_name, &err);
	if (this->getError(err, OpenCLCaller::CreateKernel)) return true;

	this->_glob_posX = clCreateBuffer(this->_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->posX, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferGlobPosX)) return true;

	this->_glob_posY = clCreateBuffer(this->_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->posY, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferGlobPosY)) return true;

	this->_glob_posZ = clCreateBuffer(this->_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->posZ, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferGlobPosZ)) return true;

	this->_glob_velX = clCreateBuffer(this->_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->velX, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferGlobVelX)) return true;

	this->_glob_velY = clCreateBuffer(this->_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->velY, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferGlobVelY)) return true;

	this->_glob_velZ = clCreateBuffer(this->_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->velZ, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferGlobVelZ)) return true;

	this->_glob_mass = clCreateBuffer(this->_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->mass, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferGlobMass)) return true;

	this->_dt_div_mass = clCreateBuffer(this->_context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->dt_div_mass, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferDtDivMass)) return true;

	this->_mass_mul_g = clCreateBuffer(this->_context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->mass_mul_g, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferMassMulG)) return true;

	this->_dt_div_2mass = clCreateBuffer(this->_context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->dt_div_2mass, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferDtDiv2Mass)) return true;

	this->_dt_squared_div_2mass = clCreateBuffer(this->_context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->dt_squared_div_2mass, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferDtSquaredDiv2Mass)) return true;

	this->_dt_squared_div_12mass = clCreateBuffer(this->_context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->dt_squared_div_12mass, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferDtSquaredDiv12Mass)) return true;

	this->_dt_pow_3_div_6mass = clCreateBuffer(this->_context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->dt_pow_3_div_6mass, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferDtPow3Div6Mass)) return true;
	
	this->_out_posX = clCreateBuffer(this->_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->posX, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferOutPosX)) return true;

	this->_out_posY = clCreateBuffer(this->_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->posY, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferOutPosY)) return true;

	this->_out_posZ = clCreateBuffer(this->_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->posZ, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferOutPosZ)) return true;

	this->_out_velX = clCreateBuffer(this->_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->velX, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferOutVelX)) return true;

	this->_out_velY = clCreateBuffer(this->_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->velY, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferOutVelY)) return true;

	this->_out_velZ = clCreateBuffer(this->_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->velZ, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferOutVelZ)) return true;

	this->_l_velX = clCreateBuffer(this->_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->l_velX, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferLVelX)) return true;

	this->_l_velY = clCreateBuffer(this->_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->l_velY, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferLVelY)) return true;

	this->_l_velZ = clCreateBuffer(this->_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->l_velZ, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferLVelZ)) return true;

	this->_l_forceX = clCreateBuffer(this->_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->l_forceX, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferLForceX)) return true;

	this->_l_forceY = clCreateBuffer(this->_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->l_forceY, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferLForceY)) return true;

	this->_l_forceZ = clCreateBuffer(this->_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->l_forceZ, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferLForceZ)) return true;

	this->_l_jerkX = clCreateBuffer(this->_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->l_jerkX, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferLJerkX)) return true;

	this->_l_jerkY = clCreateBuffer(this->_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->l_jerkY, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferLJerkY)) return true;

	this->_l_jerkZ = clCreateBuffer(this->_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(double)*this->num_of_bodies, this->l_jerkZ, &err);
	if (this->getError(err, OpenCLCaller::CreateBufferLJerkZ)) return true;
	
	err = clSetKernelArg(this->_kernel, 0, sizeof(this->_glob_posX),             &this->_glob_posX);
	if (this->getError(err, OpenCLCaller::SetKernelArgGlobPosX)) return true;
	err = clSetKernelArg(this->_kernel, 1, sizeof(this->_glob_posY),             &this->_glob_posY);
	if (this->getError(err, OpenCLCaller::SetKernelArgGlobPosY)) return true;
	err = clSetKernelArg(this->_kernel, 2, sizeof(this->_glob_posZ),             &this->_glob_posZ);
	if (this->getError(err, OpenCLCaller::SetKernelArgGlobPosZ)) return true;
	err = clSetKernelArg(this->_kernel, 3, sizeof(this->_glob_velX),             &this->_glob_velX);
	if (this->getError(err, OpenCLCaller::SetKernelArgGlobVelX)) return true;
	err = clSetKernelArg(this->_kernel, 4, sizeof(this->_glob_velY),             &this->_glob_velY);
	if (this->getError(err, OpenCLCaller::SetKernelArgGlobVelY)) return true;
	err = clSetKernelArg(this->_kernel, 5, sizeof(this->_glob_velZ),             &this->_glob_velZ);
	if (this->getError(err, OpenCLCaller::SetKernelArgGlobVelZ)) return true;
	err = clSetKernelArg(this->_kernel, 6, sizeof(this->_glob_mass),             &this->_glob_mass);
	if (this->getError(err, OpenCLCaller::SetKernelArgGlobMass)) return true;
	err = clSetKernelArg(this->_kernel, 7, sizeof(this->_dt_div_mass),           &this->_dt_div_mass);
	if (this->getError(err, OpenCLCaller::SetKernelArgDtDivMass)) return true;
	err = clSetKernelArg(this->_kernel, 8, sizeof(this->_mass_mul_g),            &this->_mass_mul_g);
	if (this->getError(err, OpenCLCaller::SetKernelArgMassMulG)) return true;
	err = clSetKernelArg(this->_kernel, 9, sizeof(this->_dt_div_2mass),          &this->_dt_div_2mass);
	if (this->getError(err, OpenCLCaller::SetKernelArgDtDiv2Mass)) return true;
	err = clSetKernelArg(this->_kernel, 10,sizeof(this->_dt_squared_div_2mass),  &this->_dt_squared_div_2mass);
	if (this->getError(err, OpenCLCaller::SetKernelArgDtSquaredDiv2Mass)) return true;
	err = clSetKernelArg(this->_kernel, 11,sizeof(this->_dt_squared_div_12mass), &this->_dt_squared_div_12mass);
	if (this->getError(err, OpenCLCaller::SetKernelArgDtSquaredDiv12Mass)) return true;
	err = clSetKernelArg(this->_kernel, 12,sizeof(this->_dt_pow_3_div_6mass),    &this->_dt_pow_3_div_6mass);
	if (this->getError(err, OpenCLCaller::SetKernelArgDtPow3Div6Mass)) return true;
	err = clSetKernelArg(this->_kernel, 13,sizeof(this->_out_posX),             &this->_out_posX);
	if (this->getError(err, OpenCLCaller::SetKernelArgOutPosX)) return true;
	err = clSetKernelArg(this->_kernel, 14,sizeof(this->_out_posY),             &this->_out_posY);
	if (this->getError(err, OpenCLCaller::SetKernelArgOutPosY)) return true;
	err = clSetKernelArg(this->_kernel, 15,sizeof(this->_out_posZ),             &this->_out_posZ);
	if (this->getError(err, OpenCLCaller::SetKernelArgOutPosZ)) return true;
	err = clSetKernelArg(this->_kernel, 16,sizeof(this->_out_velX),             &this->_out_velX);
	if (this->getError(err, OpenCLCaller::SetKernelArgOutVelX)) return true;
	err = clSetKernelArg(this->_kernel, 17,sizeof(this->_out_velY),             &this->_out_velY);
	if (this->getError(err, OpenCLCaller::SetKernelArgOutVelY)) return true;
	err = clSetKernelArg(this->_kernel, 18,sizeof(this->_out_velZ),             &this->_out_velZ);
	if (this->getError(err, OpenCLCaller::SetKernelArgOutVelZ)) return true;
	err = clSetKernelArg(this->_kernel, 19,this->algorithm==0x01 ? sizeof(double)*this->cache_size : 0, NULL);
	if (this->getError(err, OpenCLCaller::SetKernelArgLocPosX)) return true;
	err = clSetKernelArg(this->_kernel, 20,this->algorithm==0x01 ? sizeof(double)*this->cache_size : 0, NULL);
	if (this->getError(err, OpenCLCaller::SetKernelArgLocPosY)) return true;
	err = clSetKernelArg(this->_kernel, 21,(this->two_dimensional||this->algorithm!=0x01) ? 0 : sizeof(double)*this->cache_size, NULL);
	if (this->getError(err, OpenCLCaller::SetKernelArgLocPosZ)) return true;
	err = clSetKernelArg(this->_kernel, 22,sizeof(double)*this->cache_size, NULL);
	if (this->getError(err, OpenCLCaller::SetKernelArgLocVelX)) return true;
	err = clSetKernelArg(this->_kernel, 23,sizeof(double)*this->cache_size, NULL);
	if (this->getError(err, OpenCLCaller::SetKernelArgLocVelY)) return true;
	err = clSetKernelArg(this->_kernel, 24,this->two_dimensional ? 0 : sizeof(double)*this->cache_size, NULL);
	if (this->getError(err, OpenCLCaller::SetKernelArgLocVelZ)) return true;
	err = clSetKernelArg(this->_kernel, 25,sizeof(this->_l_velX),             &this->_l_velX);
	if (this->getError(err, OpenCLCaller::SetKernelArgLVelX)) return true;
	err = clSetKernelArg(this->_kernel, 26,sizeof(this->_l_velY),             &this->_l_velY);
	if (this->getError(err, OpenCLCaller::SetKernelArgLVelY)) return true;
	err = clSetKernelArg(this->_kernel, 27,sizeof(this->_l_velZ),             &this->_l_velZ);
	if (this->getError(err, OpenCLCaller::SetKernelArgLVelZ)) return true;
	err = clSetKernelArg(this->_kernel, 28,sizeof(this->_l_forceX),             &this->_l_forceX);
	if (this->getError(err, OpenCLCaller::SetKernelArgLForceX)) return true;
	err = clSetKernelArg(this->_kernel, 29,sizeof(this->_l_forceY),             &this->_l_forceY);
	if (this->getError(err, OpenCLCaller::SetKernelArgLForceY)) return true;
	err = clSetKernelArg(this->_kernel, 30,sizeof(this->_l_forceZ),             &this->_l_forceZ);
	if (this->getError(err, OpenCLCaller::SetKernelArgLForceZ)) return true;
	err = clSetKernelArg(this->_kernel, 31,sizeof(this->_l_jerkX),             &this->_l_jerkX);
	if (this->getError(err, OpenCLCaller::SetKernelArgLJerkX)) return true;
	err = clSetKernelArg(this->_kernel, 32,sizeof(this->_l_jerkY),             &this->_l_jerkY);
	if (this->getError(err, OpenCLCaller::SetKernelArgLJerkY)) return true;
	err = clSetKernelArg(this->_kernel, 33,sizeof(this->_l_jerkZ),             &this->_l_jerkZ);
	if (this->getError(err, OpenCLCaller::SetKernelArgLJerkZ)) return true;

	return false;
}

void OpenCLHandler::RunKernel()
{
	cl_int err;
	size_t threads = (size_t)this->num_of_threads;
	size_t threads_in_workgroup = (size_t)this->num_of_threads_in_workgroup;
	err = clEnqueueNDRangeKernel(this->_queue, this->_kernel, 1, NULL, &threads, &threads_in_workgroup, 0, NULL, NULL);
	this->getError(err, OpenCLCaller::EnqueueKernel);
}

void OpenCLHandler::InitializeOpenCL()
{
	log_line(0x00EE);

	if (this->GetPlatform()) return;
	if (this->GetDevice()) return;
	StringController::printString(0x015D);
	if (this->GetContext()) return;
	if (this->GetCommandQueue()) return;
	StringController::printString(0x0154);
	if (this->GetProgram()) return;
	StringController::printString(0x015C);
	if (this->GetKernel()) return;
	
	log_line(0x00EF, this->_platform, this->_device, this->_context, this->_queue);
	log_line(0x014F, this->_program, this->_kernel, this->_glob_posX, this->_glob_posY, this->_glob_posZ, this->_glob_velX, this->_glob_velY, this->_glob_velZ);
	log_line(0x0159, this->_out_posX, this->_out_posY, this->_out_posZ, this->_out_velX, this->_out_velY, this->_out_velZ);
	log_line(0x015E, this->_dt_div_mass, this->_mass_mul_g, this->_dt_div_2mass);
	log_line(0x0160, this->_dt_squared_div_2mass, this->_dt_squared_div_12mass, this->_dt_pow_3_div_6mass);
}
void OpenCLHandler::DeinitializeOpenCL()
{
	log_line(0x0140);
	
	clReleaseMemObject(this->_glob_posX);
	clReleaseMemObject(this->_glob_posY);
	clReleaseMemObject(this->_glob_posZ);
	clReleaseMemObject(this->_glob_velX);
	clReleaseMemObject(this->_glob_velY);
	clReleaseMemObject(this->_glob_velZ);
	clReleaseMemObject(this->_glob_mass);
	clReleaseMemObject(this->_out_posX);
	clReleaseMemObject(this->_out_posY);
	clReleaseMemObject(this->_out_posZ);
	clReleaseMemObject(this->_out_velX);
	clReleaseMemObject(this->_out_velY);
	clReleaseMemObject(this->_out_velZ);
	clReleaseMemObject(this->_dt_div_mass);
	clReleaseMemObject(this->_mass_mul_g);
	clReleaseMemObject(this->_dt_div_2mass);
	clReleaseMemObject(this->_dt_squared_div_2mass);
	clReleaseMemObject(this->_dt_squared_div_12mass);
	clReleaseMemObject(this->_dt_pow_3_div_6mass);
	clReleaseKernel(this->_kernel);
	clReleaseProgram(this->_program);
	clReleaseCommandQueue(this->_queue);
	clReleaseContext(this->_context);

	log_line(0x0141);
}

void OpenCLHandler::Precalculations()
{
	this->dt_div_2 = this->dt / 2;
	this->dt_div_12 = this->dt / 12;
	this->dt_squared_mul_g_div_12 = this->g * this->dt * this->dt / 12;
	this->dt_pow_3_div_6 = this->dt * this->dt * this->dt / 6;
	this->dt_squared_div_2 = this->dt * this->dt / 2;
	this->dt_squared_div_12 = this->dt * this->dt / 12;
	this->dt_div_mass = (double*)safe_malloc(sizeof(double) * this->num_of_bodies, 0x00C3);
	this->dt_div_2mass = (double*)safe_malloc(sizeof(double) * this->num_of_bodies, 0x00C4);
	this->mass_mul_g = (double*)safe_malloc(sizeof(double) * this->num_of_bodies, 0x00C5);
	this->dt_squared_div_2mass = (double*)safe_malloc(sizeof(double) * this->num_of_bodies, 0x00C6);
	this->dt_squared_div_12mass = (double*)safe_malloc(sizeof(double) * this->num_of_bodies, 0x00C7);
	this->dt_pow_3_div_6mass = (double*)safe_malloc(sizeof(double) * this->num_of_bodies, 0x00C8);
	this->posX =     (double*)safe_malloc(sizeof(double) * this->num_of_bodies, 0x017B);
	this->posY =     (double*)safe_malloc(sizeof(double) * this->num_of_bodies, 0x0142);
	this->posZ =     (double*)safe_malloc(sizeof(double) * this->num_of_bodies, 0x0179);
	this->velX =     (double*)safe_malloc(sizeof(double) * this->num_of_bodies, 0x015F);
	this->velY =     (double*)safe_malloc(sizeof(double) * this->num_of_bodies, 0x0157);
	this->velZ =     (double*)safe_malloc(sizeof(double) * this->num_of_bodies, 0x0158);
	this->mass =     (double*)safe_malloc(sizeof(double) * this->num_of_bodies, 0x015A);
	this->l_velX =   (double*)safe_malloc(sizeof(double) * this->num_of_bodies, 0x019C);
	this->l_velY =   (double*)safe_malloc(sizeof(double) * this->num_of_bodies, 0x019D);
	this->l_velZ =   (double*)safe_malloc(sizeof(double) * this->num_of_bodies, 0x019E);
	this->l_forceX = (double*)safe_malloc(sizeof(double) * this->num_of_bodies, 0x019F);
	this->l_forceY = (double*)safe_malloc(sizeof(double) * this->num_of_bodies, 0x01A0);
	this->l_forceZ = (double*)safe_malloc(sizeof(double) * this->num_of_bodies, 0x01A1);
	this->l_jerkX =  (double*)safe_malloc(sizeof(double) * this->num_of_bodies, 0x01A2);
	this->l_jerkY =  (double*)safe_malloc(sizeof(double) * this->num_of_bodies, 0x01A3);
	this->l_jerkZ =  (double*)safe_malloc(sizeof(double) * this->num_of_bodies, 0x01A4);

	StringController::printString(0x01A5);

	if (this->algorithm == 0x01) //Hermite
		if (this->two_dimensional)
			this->InitializeHermite2D();
		else
			this->InitializeHermite3D();

	for (int i = 0; i < this->num_of_bodies; i++)
	{
		this->dt_div_mass[i] = this->dt / this->bodies[i]._mass;
		this->dt_div_2mass[i] = this->dt / (2 * this->bodies[i]._mass);
		this->mass_mul_g[i] = this->bodies[i]._mass * this->g;
		this->dt_squared_div_2mass[i] = this->dt * this->dt / (2 * this->bodies[i]._mass);
		this->dt_squared_div_12mass[i] = this->dt * this->dt / (12 * this->bodies[i]._mass);
		this->dt_pow_3_div_6mass[i] = this->dt * this->dt * this->dt / (6 * this->bodies[i]._mass);
		this->posX[i] = this->bodies[i]._positionX;
		this->posY[i] = this->bodies[i]._positionY;
		this->posZ[i] = this->bodies[i]._positionZ;
		this->velX[i] = this->bodies[i]._velocityX;
		this->velY[i] = this->bodies[i]._velocityY;
		this->velZ[i] = this->bodies[i]._velocityZ;
		this->mass[i] = this->bodies[i]._mass;
		this->l_velX[i] = this->bodies[i]._l_velocityX;
		this->l_velY[i] = this->bodies[i]._l_velocityY;
		this->l_velZ[i] = this->bodies[i]._l_velocityZ;
		this->l_forceX[i] = this->bodies[i]._l_forceX;
		this->l_forceY[i] = this->bodies[i]._l_forceY;
		this->l_forceZ[i] = this->bodies[i]._l_forceZ;
		this->l_jerkX[i] = this->bodies[i]._l_jerkX;
		this->l_jerkY[i] = this->bodies[i]._l_jerkY;
		this->l_jerkZ[i] = this->bodies[i]._l_jerkZ;
	}
}
double OpenCLHandler::GetEnergySum2D()
{
	log_line(0x00E2);
	double energy_kin = 0;
	double energy_pot = 0;
	double dX, dY;
	double length;
	for (int i = 0; i < this->num_of_bodies - 1; i++)
	{
		for (int j = i + 1; j < this->num_of_bodies; j++)
		{
			dX = this->bodies[j]._positionX - this->bodies[i]._positionX;
            dY = this->bodies[j]._positionY - this->bodies[i]._positionY;
            length = sqrt((dX * dX) + (dY * dY));
			if (length != 0) energy_pot -= this->g*this->bodies[i]._mass*this->bodies[j]._mass / length;
		}
		energy_kin += this->bodies[i]._mass * (this->bodies[i]._velocityX * this->bodies[i]._velocityX + this->bodies[i]._velocityY * this->bodies[i]._velocityY);
	}
	energy_kin /= 2;
	log_line(0x00E3, energy_kin + energy_pot);
	return energy_kin + energy_pot;
}
double OpenCLHandler::GetEnergySum3D()
{
	log_line(0x00E4);
	double energy_kin = 0;

	double energy_pot = 0;
	double dX, dY, dZ;
	double length;
	for (int i = 0; i < this->num_of_bodies - 1; i++)
	{
		for (int j = i + 1; j < this->num_of_bodies; j++)
		{
			dX = this->bodies[j]._positionX - this->bodies[i]._positionX;
            dY = this->bodies[j]._positionY - this->bodies[i]._positionY;
			dZ = this->bodies[j]._positionZ - this->bodies[i]._positionZ;
            length = sqrt((dX * dX) + (dY * dY) + (dZ * dZ));
			if (length != 0) energy_pot -= this->g*this->bodies[i]._mass*this->bodies[j]._mass / length;
		}
		energy_kin += this->bodies[i]._mass * (this->bodies[i]._velocityX * this->bodies[i]._velocityX + this->bodies[i]._velocityY * this->bodies[i]._velocityY + this->bodies[i]._velocityZ * this->bodies[i]._velocityZ);
	}
	energy_kin /= 2;
	log_line(0x00E5, energy_kin + energy_pot);
	return energy_kin + energy_pot;
}
double OpenCLHandler::GetEnergyError2D()
{
	return (this->GetEnergySum2D() - this->initial_energy_sum_2d) / this->initial_energy_sum_2d;
}
double OpenCLHandler::GetEnergyError3D()
{
	return (this->GetEnergySum3D() - this->initial_energy_sum_3d) / this->initial_energy_sum_3d;
}

double OpenCLHandler::GetMomentumSum2D()
{
	log_line(0x00E6);
	double momentumX = 0;
	double momentumY = 0;
	double ret;
	for (int i = 0; i < this->num_of_bodies; i++)
	{
		momentumX += this->bodies[i]._mass * this->bodies[i]._velocityX;
		momentumY += this->bodies[i]._mass * this->bodies[i]._velocityY;
	}
	ret = sqrt(momentumX*momentumX + momentumY*momentumY);
	log_line(0x00E7, ret);
	return ret;
}
double OpenCLHandler::GetMomentumSum3D()
{
	log_line(0x00E8);
	double momentumX = 0;
	double momentumY = 0;
	double momentumZ = 0;
	double ret;
	for (int i = 0; i < this->num_of_bodies; i++)
	{
		momentumX += this->bodies[i]._mass * this->bodies[i]._velocityX;
		momentumY += this->bodies[i]._mass * this->bodies[i]._velocityY;
		momentumZ += this->bodies[i]._mass * this->bodies[i]._velocityZ;
	}
	ret = sqrt(momentumX*momentumX + momentumY*momentumY + momentumZ*momentumZ);
	log_line(0x00E9, ret);
	return ret;
}
double OpenCLHandler::GetMomentumError2D()
{
	return (this->GetMomentumSum2D() - this->initial_momentum_sum_2d) / this->initial_momentum_sum_2d;
}
double OpenCLHandler::GetMomentumError3D()
{
	return (this->GetMomentumSum3D() - this->initial_momentum_sum_3d) / this->initial_momentum_sum_3d;
}

void OpenCLHandler::UpdateOpenCL()
{
	cl_int err;
	err = clEnqueueReadBuffer(this->_queue, this->_out_posX, CL_TRUE, 0, sizeof(double)*this->num_of_bodies, this->posX, 0, NULL, NULL);
	if (this->getError(err, OpenCLCaller::ReadBufferOutPosX)) return;
	err = clEnqueueReadBuffer(this->_queue, this->_out_posY, CL_TRUE, 0, sizeof(double)*this->num_of_bodies, this->posY, 0, NULL, NULL);
	if (this->getError(err, OpenCLCaller::ReadBufferOutPosY)) return;
	if (!this->two_dimensional) err = clEnqueueReadBuffer(this->_queue, this->_out_posZ, CL_TRUE, 0, sizeof(double)*this->num_of_bodies, this->posZ, 0, NULL, NULL);
	if (this->getError(err, OpenCLCaller::ReadBufferOutPosZ)) return;
	err = clEnqueueReadBuffer(this->_queue, this->_out_velX, CL_TRUE, 0, sizeof(double)*this->num_of_bodies, this->velX, 0, NULL, NULL);
	if (this->getError(err, OpenCLCaller::ReadBufferOutVelX)) return;
	err = clEnqueueReadBuffer(this->_queue, this->_out_velY, CL_TRUE, 0, sizeof(double)*this->num_of_bodies, this->velY, 0, NULL, NULL);
	if (this->getError(err, OpenCLCaller::ReadBufferOutVelY)) return;
	if (!this->two_dimensional) err = clEnqueueReadBuffer(this->_queue, this->_out_velZ, CL_TRUE , 0, sizeof(double)*this->num_of_bodies, this->velZ, 0, NULL, NULL);
	if (this->getError(err, OpenCLCaller::ReadBufferOutVelZ)) return;

	for (int i = 0; i < this->num_of_bodies; i++)
	{
		this->bodies[i]._positionX = this->posX[i];
		this->bodies[i]._positionY = this->posY[i];
		if (!this->two_dimensional) this->bodies[i]._positionZ = this->posZ[i];
		this->bodies[i]._velocityX = this->velX[i];
		this->bodies[i]._velocityY = this->velY[i];
		if (!this->two_dimensional) this->bodies[i]._velocityZ = this->velZ[i];
	}
}

void OpenCLHandler::InitializeHermite2D()
{
	double dRX, dRY; //used to find the connecting vector between two bodies. The Rji vector.
	double dVX, dVY; //used to find the Vji for the hermite formula.
	double dRdV_mul_three_div_length_pow_2; //(dR dot-multiply dV) multiplied by the minus-second(-2) power of the length of the Rji vector multiplied by 3 for the hermite formula.
	double inv_length_pow_2; //The minus-second(-2) power of the length of the Rji vector.
    double inv_length_pow_3; //The minus-third(-3) power of the length of the Rji vector.
    double force_div_length; //The force(newtons) of gravitation between the two bodies, divided by the length of the Rji vector. The Rji vector is multiplied by this value to generate the force vector.
	double jerk_sharedX, jerk_sharedY; //A part of the jerk formula
	for (int i = 0; i < this->num_of_bodies - 1; i++)
        for (int j = i + 1; j < this->num_of_bodies; j++) //Run on every two bodies
        {
            //Generate Rji and Vji vectors
            dRX = this->bodies[j]._positionX - this->bodies[i]._positionX;
            dRY = this->bodies[j]._positionY - this->bodies[i]._positionY;
            dVX = this->bodies[j]._velocityX - this->bodies[i]._velocityX;
            dVY = this->bodies[j]._velocityY - this->bodies[i]._velocityY;

            //Generate lengths - the distance between the bodies pow -2 and -3
            inv_length_pow_2 = (dRX * dRX) + (dRY * dRY);
			if (inv_length_pow_2 != 0) inv_length_pow_2 = 1 / inv_length_pow_2;
            inv_length_pow_3 = inv_length_pow_2 * sqrt(inv_length_pow_2);

			//Calculate the jerk (without G and mass) and the force size
			force_div_length = this->mass_mul_g[i]*this->bodies[j]._mass * inv_length_pow_3;
			dRdV_mul_three_div_length_pow_2 = (dRX*dVX + dRY*dVY) * 3 * inv_length_pow_2;
			jerk_sharedX = (dVX - dRdV_mul_three_div_length_pow_2 * dRX) * force_div_length;
			jerk_sharedY = (dVY - dRdV_mul_three_div_length_pow_2 * dRY) * force_div_length;

			//Add the calculated jerk
			this->bodies[i]._jerkX += jerk_sharedX; this->bodies[i]._jerkY += jerk_sharedY;
			this->bodies[j]._jerkX -= jerk_sharedX;	this->bodies[j]._jerkY -= jerk_sharedY;

			//Generate the force vector and save it instead of the Rij vector
            dRX *= force_div_length;
            dRY *= force_div_length;

            //Add the force vector to the bodies
            this->bodies[i]._forceX += dRX; this->bodies[i]._forceY += dRY;
            this->bodies[j]._forceX -= dRX; this->bodies[j]._forceY -= dRY;
        }
	for (int i = 0; i < this->num_of_bodies; i++) //Initialize current calculation
	{
		//Duplicate some data
		this->bodies[i]._l_forceX    = this->bodies[i]._forceX;
		this->bodies[i]._l_forceY    = this->bodies[i]._forceY;
		this->bodies[i]._l_jerkX     = this->bodies[i]._jerkX;
		this->bodies[i]._l_jerkY     = this->bodies[i]._jerkY;
		this->bodies[i]._l_positionX = this->bodies[i]._positionX;
		this->bodies[i]._l_positionY = this->bodies[i]._positionY;
		this->bodies[i]._l_velocityX = this->bodies[i]._velocityX;
		this->bodies[i]._l_velocityY = this->bodies[i]._velocityY;
		
		//Zeroing
		this->bodies[i]._forceX = 0; this->bodies[i]._forceY = 0;
		this->bodies[i]._jerkX  = 0; this->bodies[i]._jerkY  = 0;

		//Calculate pre-position and pre-velocity
		this->bodies[i]._t_positionX = this->bodies[i]._l_positionX + this->bodies[i]._l_velocityX * this->dt             + this->bodies[i]._l_forceX * this->dt_squared_div_2mass[i] + this->bodies[i]._l_jerkX * this->dt_pow_3_div_6mass[i];
		this->bodies[i]._t_positionY = this->bodies[i]._l_positionY + this->bodies[i]._l_velocityY * this->dt             + this->bodies[i]._l_forceY * this->dt_squared_div_2mass[i] + this->bodies[i]._l_jerkY * this->dt_pow_3_div_6mass[i];
		this->bodies[i]._t_velocityX = this->bodies[i]._l_velocityX + this->bodies[i]._l_forceX    * this->dt_div_mass[i] + this->bodies[i]._l_jerkX  * this->dt_squared_div_2mass[i];
		this->bodies[i]._t_velocityY = this->bodies[i]._l_velocityY + this->bodies[i]._l_forceY    * this->dt_div_mass[i] + this->bodies[i]._l_jerkY  * this->dt_squared_div_2mass[i];
	}
    for (int i = 0; i < this->num_of_bodies - 1; i++)
        for (int j = i + 1; j < this->num_of_bodies; j++) //Run on every two bodies
        {
            //Generate Rji and Vji vectors
            dRX = this->bodies[j]._t_positionX - this->bodies[i]._t_positionX;
            dRY = this->bodies[j]._t_positionY - this->bodies[i]._t_positionY;
            dVX = this->bodies[j]._t_velocityX - this->bodies[i]._t_velocityX;
            dVY = this->bodies[j]._t_velocityY - this->bodies[i]._t_velocityY;

            //Generate lengths - the distance between the bodies pow -2 and -3
            inv_length_pow_2 = (dRX * dRX) + (dRY * dRY);
			if (inv_length_pow_2 != 0) inv_length_pow_2 = 1 / inv_length_pow_2;
            inv_length_pow_3 = inv_length_pow_2 * sqrt(inv_length_pow_2);

			//Calculate the jerk and the force size
			force_div_length = this->mass_mul_g[i]*this->bodies[j]._mass * inv_length_pow_3;
			dRdV_mul_three_div_length_pow_2 = (dRX*dVX + dRY*dVY) * 3 * inv_length_pow_2;
			jerk_sharedX = (dVX - dRdV_mul_three_div_length_pow_2 * dRX) * force_div_length;
			jerk_sharedY = (dVY - dRdV_mul_three_div_length_pow_2 * dRY) * force_div_length;

			//Add the calculated jerk
			this->bodies[i]._jerkX += jerk_sharedX; this->bodies[i]._jerkY += jerk_sharedY;
			this->bodies[j]._jerkX -= jerk_sharedX;	this->bodies[j]._jerkY -= jerk_sharedY;

			//Generate the force vector and save it instead of the Rij vector
            dRX *= force_div_length;
            dRY *= force_div_length;

            //Add the force vector to the bodies
            this->bodies[i]._forceX += dRX; this->bodies[i]._forceY += dRY;
            this->bodies[j]._forceX -= dRX; this->bodies[j]._forceY -= dRY;
        }
}
void OpenCLHandler::InitializeHermite3D()
{
	double dRX, dRY, dRZ; //used to find the connecting vector between two bodies. The Rji vector.
	double dVX, dVY, dVZ; //used to find the Vji for the hermite formula.
	double dRdV_mul_three_div_length_pow_2; //(dR dot-multiply dV) multiplied by the minus-second(-2) power of the length of the Rji vector multiplied by 3 for the hermite formula.
	double inv_length_pow_2; //The minus-second(-2) power of the length of the Rji vector.
    double inv_length_pow_3; //The minus-third(-3) power of the length of the Rji vector.
    double force_div_length; //The force(newtons) of gravitation between the two bodies, divided by the length of the Rji vector. The Rji vector is multiplied by this value to generate the force vector.
	double jerk_sharedX, jerk_sharedY, jerk_sharedZ; //A part of the jerk formula
	for (int i = 0; i < this->num_of_bodies - 1; i++)
        for (int j = i + 1; j < this->num_of_bodies; j++) //Run on every two bodies
        {
            //Generate Rji and Vji vectors
            dRX = this->bodies[j]._positionX - this->bodies[i]._positionX;
            dRY = this->bodies[j]._positionY - this->bodies[i]._positionY;
			dRZ = this->bodies[j]._positionZ - this->bodies[i]._positionZ;
            dVX = this->bodies[j]._velocityX - this->bodies[i]._velocityX;
            dVY = this->bodies[j]._velocityY - this->bodies[i]._velocityY;
            dVZ = this->bodies[j]._velocityZ - this->bodies[i]._velocityZ;

            //Generate lengths - the distance between the bodies pow -2 and -3
            inv_length_pow_2 = (dRX * dRX) + (dRY * dRY) + (dRZ * dRZ);
			if (inv_length_pow_2 != 0) inv_length_pow_2 = 1 / inv_length_pow_2;
            inv_length_pow_3 = inv_length_pow_2 * sqrt(inv_length_pow_2);

			//Calculate the jerk (without G and mass) and force size
			force_div_length = this->mass_mul_g[i]*this->bodies[j]._mass * inv_length_pow_3;
			dRdV_mul_three_div_length_pow_2 = (dRX*dVX + dRY*dVY + dRZ*dVZ) * 3 * inv_length_pow_2;
			jerk_sharedX = (dVX - dRdV_mul_three_div_length_pow_2 * dRX) * force_div_length;
			jerk_sharedY = (dVY - dRdV_mul_three_div_length_pow_2 * dRY) * force_div_length;
			jerk_sharedZ = (dVZ - dRdV_mul_three_div_length_pow_2 * dRZ) * force_div_length;

			//Add the calculated jerk
			this->bodies[i]._jerkX += jerk_sharedX;	this->bodies[i]._jerkY += jerk_sharedY;	this->bodies[i]._jerkZ += jerk_sharedZ;
			this->bodies[j]._jerkX -= jerk_sharedX;	this->bodies[j]._jerkY -= jerk_sharedY;	this->bodies[j]._jerkZ -= jerk_sharedZ;

			//Generate force vector and save it instead of the Rji vector
            dRX *= force_div_length;
            dRY *= force_div_length;
			dRZ *= force_div_length;

            //Add the force vector to the bodies
            this->bodies[i]._forceX += dRX; this->bodies[i]._forceY += dRY; this->bodies[i]._forceZ += dRZ;
            this->bodies[j]._forceX -= dRX; this->bodies[j]._forceY -= dRY; this->bodies[j]._forceZ -= dRZ;
        }
	for (int i = 0; i < this->num_of_bodies; i++) //Initialize current calculation
	{
		//Duplicate some data
		this->bodies[i]._l_forceX    = this->bodies[i]._forceX;
		this->bodies[i]._l_forceY    = this->bodies[i]._forceY;
		this->bodies[i]._l_forceZ    = this->bodies[i]._forceZ;
		this->bodies[i]._l_jerkX     = this->bodies[i]._jerkX;
		this->bodies[i]._l_jerkY     = this->bodies[i]._jerkY;
		this->bodies[i]._l_jerkZ     = this->bodies[i]._jerkZ;
		this->bodies[i]._l_positionX = this->bodies[i]._positionX;
		this->bodies[i]._l_positionY = this->bodies[i]._positionY;
		this->bodies[i]._l_positionZ = this->bodies[i]._positionZ;
		this->bodies[i]._l_velocityX = this->bodies[i]._velocityX;
		this->bodies[i]._l_velocityY = this->bodies[i]._velocityY;
		this->bodies[i]._l_velocityZ = this->bodies[i]._velocityZ;
		
		//Zeroing
		this->bodies[i]._forceX = 0; this->bodies[i]._forceY = 0; this->bodies[i]._forceZ = 0;
		this->bodies[i]._jerkX  = 0; this->bodies[i]._jerkY  = 0; this->bodies[i]._jerkZ  = 0;

		//Calculate pre-position and pre-velocity
		this->bodies[i]._t_positionX = this->bodies[i]._l_positionX + this->bodies[i]._l_velocityX * this->dt             + this->bodies[i]._l_forceX * this->dt_squared_div_2mass[i] + this->bodies[i]._l_jerkX * this->dt_pow_3_div_6mass[i];
		this->bodies[i]._t_positionY = this->bodies[i]._l_positionY + this->bodies[i]._l_velocityY * this->dt             + this->bodies[i]._l_forceY * this->dt_squared_div_2mass[i] + this->bodies[i]._l_jerkY * this->dt_pow_3_div_6mass[i];
		this->bodies[i]._t_positionZ = this->bodies[i]._l_positionZ + this->bodies[i]._l_velocityZ * this->dt             + this->bodies[i]._l_forceZ * this->dt_squared_div_2mass[i] + this->bodies[i]._l_jerkZ * this->dt_pow_3_div_6mass[i];
		this->bodies[i]._t_velocityX = this->bodies[i]._l_velocityX + this->bodies[i]._l_forceX    * this->dt_div_mass[i] + this->bodies[i]._l_jerkX  * this->dt_squared_div_2mass[i];
		this->bodies[i]._t_velocityY = this->bodies[i]._l_velocityY + this->bodies[i]._l_forceY    * this->dt_div_mass[i] + this->bodies[i]._l_jerkY  * this->dt_squared_div_2mass[i];
		this->bodies[i]._t_velocityZ = this->bodies[i]._l_velocityZ + this->bodies[i]._l_forceZ    * this->dt_div_mass[i] + this->bodies[i]._l_jerkZ  * this->dt_squared_div_2mass[i];
	}
    for (int i = 0; i < this->num_of_bodies - 1; i++)
        for (int j = i + 1; j < this->num_of_bodies; j++) //Run on every two bodies
        {
            //Generate Rji and Vji vectors
            dRX = this->bodies[j]._t_positionX - this->bodies[i]._t_positionX;
            dRY = this->bodies[j]._t_positionY - this->bodies[i]._t_positionY;
			dRZ = this->bodies[j]._t_positionZ - this->bodies[i]._t_positionZ;
            dVX = this->bodies[j]._t_velocityX - this->bodies[i]._t_velocityX;
            dVY = this->bodies[j]._t_velocityY - this->bodies[i]._t_velocityY;
            dVZ = this->bodies[j]._t_velocityZ - this->bodies[i]._t_velocityZ;

            //Generate lengths - the distance between the bodies pow -2 and -3
            inv_length_pow_2 = (dRX * dRX) + (dRY * dRY) + (dRZ * dRZ);
			if (inv_length_pow_2 != 0) inv_length_pow_2 = 1 / inv_length_pow_2;
            inv_length_pow_3 = inv_length_pow_2 * sqrt(inv_length_pow_2);

			//Calculate the jerk (without G and mass) and force size
			force_div_length = this->mass_mul_g[i]*this->bodies[j]._mass * inv_length_pow_3;
			dRdV_mul_three_div_length_pow_2 = (dRX*dVX + dRY*dVY + dRZ*dVZ) * 3 * inv_length_pow_2;
			jerk_sharedX = (dVX - dRdV_mul_three_div_length_pow_2 * dRX) * force_div_length;
			jerk_sharedY = (dVY - dRdV_mul_three_div_length_pow_2 * dRY) * force_div_length;
			jerk_sharedZ = (dVZ - dRdV_mul_three_div_length_pow_2 * dRZ) * force_div_length;

			//Add the calculated jerk
			this->bodies[i]._jerkX += jerk_sharedX;	this->bodies[i]._jerkY += jerk_sharedY;	this->bodies[i]._jerkZ += jerk_sharedZ;
			this->bodies[j]._jerkX -= jerk_sharedX;	this->bodies[j]._jerkY -= jerk_sharedY;	this->bodies[j]._jerkZ -= jerk_sharedZ;

			//Generate force vector and save it instead of the Rji vector
            dRX *= force_div_length;
            dRY *= force_div_length;
			dRZ *= force_div_length;

            //Add the force vector to the bodies
            this->bodies[i]._forceX += dRX; this->bodies[i]._forceY += dRY; this->bodies[i]._forceZ += dRZ;
            this->bodies[j]._forceX -= dRX; this->bodies[j]._forceY -= dRY; this->bodies[j]._forceZ -= dRZ;
        }
}
