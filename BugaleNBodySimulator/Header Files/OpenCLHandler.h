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
#ifndef __OpenCLHANDLER_INCLUDED__
#define __OpenCLHANDLER_INCLUDED__

#include "../SharedHeader.h"
namespace OpenCLCaller
{
	enum Caller {
		GetPlatformIDsCount,
		GetPlatformIDs,
		GetDeviceIDsCount, 
		GetDeviceIDs,
		CreateContext, 
		CreateCommandQueue,
		CreateProgram,
		BuildProgram,
		GetProgramBuildLogSize,
		GetProgramBuildLog,
		CreateKernel,
		CreateBufferGlobPosX,
		CreateBufferGlobPosY,
		CreateBufferGlobPosZ,
		CreateBufferGlobVelX,
		CreateBufferGlobVelY,
		CreateBufferGlobVelZ,
		CreateBufferGlobMass,
		CreateBufferDtDivMass,
		CreateBufferMassMulG,
		CreateBufferDtDiv2Mass,
		CreateBufferDtSquaredDiv2Mass,
		CreateBufferDtSquaredDiv12Mass,
		CreateBufferDtPow3Div6Mass,
		CreateBufferOutPosX,
		CreateBufferOutPosY,
		CreateBufferOutPosZ,
		CreateBufferOutVelX,
		CreateBufferOutVelY,
		CreateBufferOutVelZ,
		CreateBufferLVelX,
		CreateBufferLVelY,
		CreateBufferLVelZ,
		CreateBufferLForceX,
		CreateBufferLForceY,
		CreateBufferLForceZ,
		CreateBufferLJerkX,
		CreateBufferLJerkY,
		CreateBufferLJerkZ,
		SetKernelArgGlobPosX,
		SetKernelArgGlobPosY,
		SetKernelArgGlobPosZ,
		SetKernelArgGlobVelX,
		SetKernelArgGlobVelY,
		SetKernelArgGlobVelZ,
		SetKernelArgGlobMass,
		SetKernelArgDtDivMass,
		SetKernelArgMassMulG,
		SetKernelArgDtDiv2Mass,
		SetKernelArgDtSquaredDiv2Mass,
		SetKernelArgDtSquaredDiv12Mass,
		SetKernelArgDtPow3Div6Mass,
		SetKernelArgOutPosX,
		SetKernelArgOutPosY,
		SetKernelArgOutPosZ,
		SetKernelArgOutVelX,
		SetKernelArgOutVelY,
		SetKernelArgOutVelZ,
		SetKernelArgLocPosX,
		SetKernelArgLocPosY,
		SetKernelArgLocPosZ,
		SetKernelArgLocVelX,
		SetKernelArgLocVelY,
		SetKernelArgLocVelZ,
		SetKernelArgLVelX,
		SetKernelArgLVelY,
		SetKernelArgLVelZ,
		SetKernelArgLForceX,
		SetKernelArgLForceY,
		SetKernelArgLForceZ,
		SetKernelArgLJerkX,
		SetKernelArgLJerkY,
		SetKernelArgLJerkZ,
		EnqueueKernel,
		ReadBufferOutPosX,
		ReadBufferOutPosY,
		ReadBufferOutPosZ,
		ReadBufferOutVelX,
		ReadBufferOutVelY,
		ReadBufferOutVelZ,
		GetPlatformInfoVendor,
		GetPlatformInfoName,
		GetPlatformInfoVersion,
		GetDeviceInfoVendor,
		GetDeviceInfoName,
		GetDeviceInfoVersion,
		GetDeviceInfoCache
	};
}
class OpenCLHandler
{
private:
	cl_device_id _device;
	cl_platform_id _platform;
	cl_command_queue _queue;
	cl_context _context;
	cl_program _program;
	cl_kernel _kernel;
	cl_mem _glob_posX; //Device Memory
	cl_mem _glob_posY; //Device Memory
	cl_mem _glob_posZ; //Device Memory
	cl_mem _glob_velX; //Device Memory
	cl_mem _glob_velY; //Device Memory
	cl_mem _glob_velZ; //Device Memory
	cl_mem _glob_mass; //Device Memory
	cl_mem _out_posX; //Device Memory
	cl_mem _out_posY; //Device Memory
	cl_mem _out_posZ; //Device Memory
	cl_mem _out_velX; //Device Memory
	cl_mem _out_velY; //Device Memory
	cl_mem _out_velZ; //Device Memory
	cl_mem _dt_div_mass; //Device Memory
	cl_mem _mass_mul_g; //Device Memory
	cl_mem _dt_div_2mass; //Device Memory
	cl_mem _dt_squared_div_2mass; //Device Memory
	cl_mem _dt_squared_div_12mass; //Device Memory
	cl_mem _dt_pow_3_div_6mass; //Device Memory
	cl_mem _l_velX; //Device Memory
	cl_mem _l_velY; //Device Memory
	cl_mem _l_velZ; //Device Memory
	cl_mem _l_forceX; //Device Memory
	cl_mem _l_forceY; //Device Memory
	cl_mem _l_forceZ; //Device Memory
	cl_mem _l_jerkX; //Device Memory
	cl_mem _l_jerkY; //Device Memory
	cl_mem _l_jerkZ; //Device Memory

	bool getError(cl_int r, OpenCLCaller::Caller c);

	bool GetPlatform();
	bool GetDevice();
	bool GetContext();
	bool GetCommandQueue();
	bool GetProgram();
	bool GetKernel();

public:
	unsigned char algorithm;
	bool two_dimensional;
	char* kernel_name;
	Body3D* bodies;

	double* posX; //Host Memory
	double* posY; //Host Memory
	double* posZ; //Host Memory
	double* velX; //Host Memory
	double* velY; //Host Memory
	double* velZ; //Host Memory
	double* mass; //Host Memory
	double* l_velX; //Host Memory
	double* l_velY; //Host Memory
	double* l_velZ; //Host Memory
	double* l_forceX; //Host Memory
	double* l_forceY; //Host Memory
	double* l_forceZ; //Host Memory
	double* l_jerkX; //Host Memory
	double* l_jerkY; //Host Memory
	double* l_jerkZ; //Host Memory

	double* dt_div_mass; //Host Memory
	double* mass_mul_g; //Host Memory
	double dt_div_2;
	double dt_div_12;
	double dt_squared_mul_g_div_12;
	double dt_pow_3_div_6;
	double dt_squared_div_2;
	double dt_squared_div_12;
	double* dt_div_2mass; //Host Memory
	double* dt_squared_div_2mass; //Host Memory
	double* dt_squared_div_12mass; //Host Memory
	double* dt_pow_3_div_6mass; //Host Memory

	int cache_size;
	int num_of_bodies;
	double dt;
	double g;
	double initial_energy_sum_2d;
	double initial_energy_sum_3d;
	double initial_momentum_sum_2d;
	double initial_momentum_sum_3d;

	int num_of_threads;
	int num_of_threads_in_workgroup;
	int num_of_calcs_in_run;

	OpenCLErrors::Error error;
	int error_data_int;
	char* error_data_charptr;

	OpenCLHandler(SharedData* shared, Data* data);
	~OpenCLHandler();
	
	void InitializeOpenCL();
	void DeinitializeOpenCL();

	void RunKernel();

	void Precalculations(); //Precalculate some constant expressions which are commonly used in the algorithms
	
	//Energy calculations
	double GetEnergySum2D();
	double GetEnergySum3D();
	double GetEnergyError2D();
	double GetEnergyError3D();

	//Momentum calculations
	double GetMomentumSum2D();
	double GetMomentumSum3D();
	double GetMomentumError2D();
	double GetMomentumError3D();

	void UpdateOpenCL();

	void InitializeHermite2D();
	void InitializeHermite3D();
};

#endif
