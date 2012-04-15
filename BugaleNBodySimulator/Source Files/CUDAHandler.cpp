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
#include "..\CUDA\PTXFile.h"

#ifdef _DEBUG

CUDAHandler::CUDAHandler(SharedData* shared, Data* data)
{
	log_line(0x00E0, shared, data);
	this->bodies = data->bodies;
	this->num_of_bodies = data->num_of_bodies;
	this->dt = data->dt;;
	this->g = data->g;
	this->max_calculations = data->max_calculations;
	this->data = safe_malloc(10, 0x00EA);
	((char*)this->data)[0] = (char)shared->exit;
	((char*)this->data)[1] = (char)shared->pause;
	*(long long*)(((char*)this->data)[2]) = shared->calculations;
	this->Precalculations();

	this->initial_energy_sum_2d = this->GetEnergySum2D();
	this->initial_energy_sum_3d = this->GetEnergySum3D();
	this->initial_momentum_sum_2d = this->GetMomentumSum2D();
	this->initial_momentum_sum_3d = this->GetMomentumSum3D();
	log_line(0x00E1, this);
}
CUDAHandler::~CUDAHandler()
{
}

Errors::Error CUDAHandler::getError(CUresult r, Caller c)
{
	switch (c)
	{
		case CUDAHandler::Init:
			switch (r)
			{
				case CUDA_SUCCESS: return Errors::NoError;
				case CUDA_ERROR_INVALID_VALUE: return Errors::CUDAInitInvalidValue;
				case CUDA_ERROR_INVALID_DEVICE: return Errors::CUDAInitInvalidDevice;
				default: this->error_data_int = r; return Errors::CUDAInitUnrecognizedError;
			}
		case CUDAHandler::DeviceGet:
			switch (r)
			{
				case CUDA_SUCCESS: return Errors::NoError;
				case CUDA_ERROR_DEINITIALIZED: return Errors::CUDADeviceGetDeinitialized;
				case CUDA_ERROR_NOT_INITIALIZED: return Errors::CUDADeviceGetNotInitialized;
				case CUDA_ERROR_INVALID_CONTEXT: return Errors::CUDADeviceGetInvalidContext;
				case CUDA_ERROR_INVALID_VALUE: return Errors::CUDADeviceGetInvalidValue;
				case CUDA_ERROR_INVALID_DEVICE: return Errors::CUDADeviceGetInvalidDevice;
				default: this->error_data_int = r; return Errors::CUDADeviceGetUnrecognizedError;
			}
		case CUDAHandler::CtxCreate:
			switch (r)
			{
				case CUDA_SUCCESS: return Errors::NoError;
				case CUDA_ERROR_DEINITIALIZED: return Errors::CUDACtxCreateDeinitialized;
				case CUDA_ERROR_NOT_INITIALIZED: return Errors::CUDACtxCreateNotInitialized;
				case CUDA_ERROR_INVALID_CONTEXT: return Errors::CUDACtxCreateInvalidContext;
				case CUDA_ERROR_INVALID_VALUE: return Errors::CUDACtxCreateInvalidValue;
				case CUDA_ERROR_INVALID_DEVICE: return Errors::CUDACtxCreateInvalidDevice;
				case CUDA_ERROR_OUT_OF_MEMORY: return Errors::CUDACtxCreateOutOfMemory;
				case CUDA_ERROR_UNKNOWN: return Errors::CUDACtxCreateUnknownError;
				default: this->error_data_int = r; return Errors::CUDACtxCreateUnrecognizedError;
			}
		case CUDAHandler::ModuleLoadData:
			switch (r)
			{
				case CUDA_SUCCESS: return Errors::NoError;
				case CUDA_ERROR_DEINITIALIZED: return Errors::CUDAModuleLoadDataDeinitialized;
				case CUDA_ERROR_NOT_INITIALIZED: return Errors::CUDAModuleLoadDataNotInitialized;
				case CUDA_ERROR_INVALID_CONTEXT: return Errors::CUDAModuleLoadDataInvalidContext;
				case CUDA_ERROR_INVALID_VALUE: return Errors::CUDAModuleLoadDataInvalidValue;
				case CUDA_ERROR_INVALID_DEVICE: return Errors::CUDAModuleLoadDataInvalidDevice;
				case CUDA_ERROR_OUT_OF_MEMORY: return Errors::CUDAModuleLoadDataOutOfMemory;
				case CUDA_ERROR_SHARED_OBJECT_SYMBOL_NOT_FOUND: return Errors::CUDAModuleLoadDataSymbolNotFound;
				case CUDA_ERROR_SHARED_OBJECT_INIT_FAILED: return Errors::CUDAModuleLoadDataInitFailed;
				default: this->error_data_int = r; return Errors::CUDAModuleLoadDataUnrecognizedError;
			}
		case CUDAHandler::ModuleGetFunction:
			switch (r)
			{
				case CUDA_SUCCESS: return Errors::NoError;
				case CUDA_ERROR_DEINITIALIZED: return Errors::CUDAModuleGetFunctionDeinitialized;
				case CUDA_ERROR_NOT_INITIALIZED: return Errors::CUDAModuleGetFunctionNotInitialized;
				case CUDA_ERROR_INVALID_CONTEXT: return Errors::CUDAModuleGetFunctionInvalidContext;
				case CUDA_ERROR_INVALID_VALUE: return Errors::CUDAModuleGetFunctionInvalidValue;
				case CUDA_ERROR_NOT_FOUND: return Errors::CUDAModuleGetFunctionNotFound;
				default: this->error_data_int = r; return Errors::CUDAModuleGetFunctionUnrecognizedError;
			}
		case CUDAHandler::MemAllocData:
			switch (r)
			{
				case CUDA_SUCCESS: return Errors::NoError;
				case CUDA_ERROR_DEINITIALIZED: return Errors::CUDAMemAllocDataDeinitialized;
				case CUDA_ERROR_NOT_INITIALIZED: return Errors::CUDAMemAllocDataNotInitialized;
				case CUDA_ERROR_INVALID_CONTEXT: return Errors::CUDAMemAllocDataInvalidContext;
				case CUDA_ERROR_INVALID_VALUE: return Errors::CUDAMemAllocDataInvalidValue;
				case CUDA_ERROR_OUT_OF_MEMORY: this->error_data_int = 10; return Errors::CUDAMemAllocDataOutOfMemory;
				default: this->error_data_int = r; return Errors::CUDAMemAllocDataUnrecognizedError;
			}
		case CUDAHandler::MemAllocBodies:
			switch (r)
			{
				case CUDA_SUCCESS: return Errors::NoError;
				case CUDA_ERROR_DEINITIALIZED: return Errors::CUDAMemAllocBodiesDeinitialized;
				case CUDA_ERROR_NOT_INITIALIZED: return Errors::CUDAMemAllocBodiesNotInitialized;
				case CUDA_ERROR_INVALID_CONTEXT: return Errors::CUDAMemAllocBodiesInvalidContext;
				case CUDA_ERROR_INVALID_VALUE: return Errors::CUDAMemAllocBodiesInvalidValue;
				case CUDA_ERROR_OUT_OF_MEMORY: this->error_data_int = sizeof(Body3D)*this->num_of_bodies; return Errors::CUDAMemAllocBodiesOutOfMemory;
				default: this->error_data_int = r; return Errors::CUDAMemAllocBodiesUnrecognizedError;
			}
		case CUDAHandler::MemCpyHtoDData:
			switch (r)
			{
				case CUDA_SUCCESS: return Errors::NoError;
				case CUDA_ERROR_DEINITIALIZED: return Errors::CUDAMemCpyHtoDDataDeinitialized;
				case CUDA_ERROR_NOT_INITIALIZED: return Errors::CUDAMemCpyHtoDDataNotInitialized;
				case CUDA_ERROR_INVALID_CONTEXT: return Errors::CUDAMemCpyHtoDDataInvalidContext;
				case CUDA_ERROR_INVALID_VALUE: return Errors::CUDAMemCpyHtoDDataInvalidValue;
				default: this->error_data_int = r; return Errors::CUDAMemCpyHtoDDataUnrecognizedError;
			}
		case CUDAHandler::MemCpyHtoDBodies:
			switch (r)
			{
				case CUDA_SUCCESS: return Errors::NoError;
				case CUDA_ERROR_DEINITIALIZED: return Errors::CUDAMemCpyHtoDBodiesDeinitialized;
				case CUDA_ERROR_NOT_INITIALIZED: return Errors::CUDAMemCpyHtoDBodiesNotInitialized;
				case CUDA_ERROR_INVALID_CONTEXT: return Errors::CUDAMemCpyHtoDBodiesInvalidContext;
				case CUDA_ERROR_INVALID_VALUE: return Errors::CUDAMemCpyHtoDBodiesInvalidValue;
				default: this->error_data_int = r; return Errors::CUDAMemCpyHtoDBodiesUnrecognizedError;
			}
	}
}

void CUDAHandler::InitializeCUDA()
{
	if (!(this->error = getError(cuInit(0),
		CUDAHandler::Init))) return;
	if (!(this->error = getError(cuDeviceGet(&this->_device, 0),
		CUDAHandler::DeviceGet))) return;
	if (!(this->error = getError(cuCtxCreate(&this->_context, 0, this->_device),
		CUDAHandler::CtxCreate))) return;
	if (!(this->error = getError(cuModuleLoadData(&this->_module, PTX_DATA),
		CUDAHandler::ModuleLoadData))) return;
	if (!(this->error = getError(cuModuleGetFunction(&_function, this->_module, PTX_F_NAME),
		CUDAHandler::ModuleGetFunction))) return;
	if (!(this->error = getError(cuMemAlloc(&this->_data, 10),
		CUDAHandler::MemAllocData))) return;
	if (!(this->error = getError(cuMemAlloc(&this->_bodies, sizeof(Body3D)*this->num_of_bodies),
		CUDAHandler::MemAllocBodies))) return;
	if (!(this->error = getError(cuMemcpyHtoD(this->_data, this->data, 10),
		CUDAHandler::MemCpyHtoDData))) return;
	if (!(this->error = getError(cuMemcpyHtoD(this->_bodies, this->bodies, sizeof(Body3D)*this->num_of_bodies),
		CUDAHandler::MemCpyHtoDBodies))) return;

	void *args[] = { &this->_bodies, &this->data, &this->num_of_bodies, &this->max_calculations, &this->num_of_threads };
	if (!(this->error = getError(cuLaunchKernel(this->_function, 1, 1, 1, this->num_of_threads, 1, 1, 0, NULL, args, NULL),
		CUDAHandler::LaunchKernel))) return;
}

void CUDAHandler::Precalculations()
{
}
double CUDAHandler::GetEnergySum2D()
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
double CUDAHandler::GetEnergySum3D()
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
double CUDAHandler::GetEnergyError2D()
{
	return (this->GetEnergySum2D() - this->initial_energy_sum_2d) / this->initial_energy_sum_2d;
}
double CUDAHandler::GetEnergyError3D()
{
	return (this->GetEnergySum3D() - this->initial_energy_sum_3d) / this->initial_energy_sum_3d;
}

double CUDAHandler::GetMomentumSum2D()
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
double CUDAHandler::GetMomentumSum3D()
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
double CUDAHandler::GetMomentumError2D()
{
	return (this->GetMomentumSum2D() - this->initial_momentum_sum_2d) / this->initial_momentum_sum_2d;
}
double CUDAHandler::GetMomentumError3D()
{
	return (this->GetMomentumSum3D() - this->initial_momentum_sum_3d) / this->initial_momentum_sum_3d;
}
#endif