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
#ifdef _DEBUG
#ifndef __CUDAHANDLER_INCLUDED__
#define __CUDAHANDLER_INCLUDED__

#include "../SharedHeader.h"

class CUDAHandler
{
private:
	CUdevice _device;
	CUcontext _context;
	CUmodule _module;
	CUfunction _function;
	CUdeviceptr _bodies; //Device Memory
	CUdeviceptr _data; //Device Memory - exit(char),pause(char),calculations(long long)

	enum Caller {Init, DeviceGet, CtxCreate, ModuleLoadData, ModuleGetFunction, MemAllocData, MemAllocBodies, MemCpyHtoDData, MemCpyHtoDBodies, LaunchKernel};
	Errors::Error getError(CUresult r, Caller c);

public:
	Body3D* bodies; //Host Memory
	void* data; //Host Memory - exit(char),pause(char),calculations(long long)
	long long max_calculations;
	int num_of_bodies;
	double dt;
	double g;
	double initial_energy_sum_2d;
	double initial_energy_sum_3d;
	double initial_momentum_sum_2d;
	double initial_momentum_sum_3d;
	int num_of_threads;
	Errors::Error error;
	int error_data_int;

	CUDAHandler(SharedData* shared, Data* data);
	~CUDAHandler();

	void InitializeCUDA();

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

	void RefreshData();

};

#endif
#endif