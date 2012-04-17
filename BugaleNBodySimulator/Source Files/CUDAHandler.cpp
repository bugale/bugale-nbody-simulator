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

CUDAHandler::CUDAHandler(SharedData* shared, Data* data)
{
	log_line(0x00E0, shared, data);
	this->bodies = data->bodies;
	this->num_of_bodies = data->num_of_bodies;
	this->dt = data->dt;;
	this->g = data->g;
	this->max_calculations = data->max_calculations;
	this->data = safe_malloc(2, 0x00EA);
	this->exit = &shared->exit;
	this->pause = &shared->pause;
	this->calculations = &shared->calculations;
	((char*)this->data)[0] = (char)*this->exit;
	((char*)this->data)[1] = (char)*this->pause;
	this->Precalculations();

	this->initial_energy_sum_2d = this->GetEnergySum2D();
	this->initial_energy_sum_3d = this->GetEnergySum3D();
	this->initial_momentum_sum_2d = this->GetMomentumSum2D();
	this->initial_momentum_sum_3d = this->GetMomentumSum3D();

	this->InitializeCUDA();
	log_line(0x00E1, this, this->data);
}
CUDAHandler::~CUDAHandler()
{
	this->DeinitializeCUDA();
	if (this->data != 0) free(this->data);
	if (this->_data != 0) cuMemFree(this->_data);
	if (this->_bodies != 0) cuMemFree(this->_bodies);
	if (this->_bodies != 0) cuMemFree(this->_mutex);
}

bool CUDAHandler::getError(CUresult r, Caller c)
{
	switch (c)
	{
		case CUDAHandler::Init:              this->error_data_charptr = StringController::getStringh(0x00F1); break;
		case CUDAHandler::DeviceGet:         this->error_data_charptr = StringController::getStringh(0x00F2); break;
		case CUDAHandler::CtxCreate:         this->error_data_charptr = StringController::getStringh(0x00F3); break;
		case CUDAHandler::ModuleLoadData:    this->error_data_charptr = StringController::getStringh(0x00F4); break;
		case CUDAHandler::ModuleGetFunction: this->error_data_charptr = StringController::getStringh(0x00F5); break;
		case CUDAHandler::MemAllocData:      this->error_data_charptr = StringController::getStringh(0x00F6); this->error_data_int = 10; break;
		case CUDAHandler::MemAllocBodies:    this->error_data_charptr = StringController::getStringh(0x00F7); this->error_data_int = sizeof(Body3D)*this->num_of_bodies; break;
		case CUDAHandler::MemAllocMutex:     this->error_data_charptr = StringController::getStringh(0x00F8); this->error_data_int = this->num_of_threads; break;
		case CUDAHandler::MemsetMutex:       this->error_data_charptr = StringController::getStringh(0x0121); break;
		case CUDAHandler::MemCpyHtoDData:    this->error_data_charptr = StringController::getStringh(0x00F9); break;
		case CUDAHandler::MemCpyDtoHData:    this->error_data_charptr = StringController::getStringh(0x00FA); break;
		case CUDAHandler::MemCpyHtoDBodies:  this->error_data_charptr = StringController::getStringh(0x00FB); break;
		case CUDAHandler::MemCpyDtoHBodies:  this->error_data_charptr = StringController::getStringh(0x00FC); break;
		case CUDAHandler::LaunchKernel:      this->error_data_charptr = StringController::getStringh(0x00FD); break;
		default: this->error = CUDAErrors::Other; return true;
	}
	switch (r)
	{
		case CUDA_SUCCESS:                             this->error = CUDAErrors::NoError; return false;
		case CUDA_ERROR_INVALID_VALUE:                 this->error = CUDAErrors::InvalidValue; break;
		case CUDA_ERROR_OUT_OF_MEMORY:                 this->error = CUDAErrors::OutOfMemory; break;
		case CUDA_ERROR_NOT_INITIALIZED:               this->error = CUDAErrors::NotInitialized; break;
		case CUDA_ERROR_DEINITIALIZED:                 this->error = CUDAErrors::Deinitalized; break;
		case CUDA_ERROR_NO_DEVICE:                     this->error = CUDAErrors::NoDevice; break;
		case CUDA_ERROR_INVALID_DEVICE:                this->error = CUDAErrors::InvalidDevice; break;
		case CUDA_ERROR_INVALID_IMAGE:                 this->error = CUDAErrors::InvalidImage; break;
		case CUDA_ERROR_INVALID_CONTEXT:               this->error = CUDAErrors::InvalidContext; break;
		case CUDA_ERROR_CONTEXT_ALREADY_CURRENT:       this->error = CUDAErrors::ContextAlreadyCurrent; break;
		case CUDA_ERROR_MAP_FAILED:                    this->error = CUDAErrors::MapFailed; break;
		case CUDA_ERROR_UNMAP_FAILED:                  this->error = CUDAErrors::UnmapFailed; break;
		case CUDA_ERROR_ARRAY_IS_MAPPED:               this->error = CUDAErrors::ArrayIsMapped; break;
		case CUDA_ERROR_ALREADY_MAPPED:                this->error = CUDAErrors::AlreadyMapped; break;
		case CUDA_ERROR_NO_BINARY_FOR_GPU:             this->error = CUDAErrors::NoBinaryForGPU; break;
		case CUDA_ERROR_ALREADY_ACQUIRED:              this->error = CUDAErrors::AlreadyAcquired; break;
		case CUDA_ERROR_NOT_MAPPED:                    this->error = CUDAErrors::NotMapped; break;
		case CUDA_ERROR_INVALID_SOURCE:                this->error = CUDAErrors::InvalidSource; break;
		case CUDA_ERROR_FILE_NOT_FOUND:                this->error = CUDAErrors::FileNotFound; break;
		case CUDA_ERROR_INVALID_HANDLE:                this->error = CUDAErrors::InvalidHandle; break;
		case CUDA_ERROR_NOT_FOUND:                     this->error = CUDAErrors::NotFound; break;
		case CUDA_ERROR_NOT_READY:                     this->error = CUDAErrors::NotReady; break;
		case CUDA_ERROR_LAUNCH_FAILED:                 this->error = CUDAErrors::LaunchFailed; break;
		case CUDA_ERROR_LAUNCH_OUT_OF_RESOURCES:       this->error = CUDAErrors::LaunchOutOfResources; break;
		case CUDA_ERROR_LAUNCH_TIMEOUT:                this->error = CUDAErrors::LaunchTimeout; break;
		case CUDA_ERROR_LAUNCH_INCOMPATIBLE_TEXTURING: this->error = CUDAErrors::LaunchIncompatibleTexturing; break;
		case CUDA_ERROR_UNKNOWN:                       this->error = CUDAErrors::Unknown; break;
		default: this->error = CUDAErrors::Other; this->error_data_int = r; break;
	}
	return true;
}

void CUDAHandler::InitializeCUDA()
{
	log_line(0x00EE);
	if (getError(cuInit(0),
		CUDAHandler::Init)) return;
	if (getError(cuDeviceGet(&this->_device, 0),
		CUDAHandler::DeviceGet)) return;
	if (getError(cuCtxCreate(&this->_context, 0, this->_device),
		CUDAHandler::CtxCreate)) return;
	if (getError(cuModuleLoadData(&this->_module, PTX_DATA),
		CUDAHandler::ModuleLoadData)) return;
	if (getError(cuModuleGetFunction(&this->_function, this->_module, PTX_F_NAME),
		CUDAHandler::ModuleGetFunction)) return;
	if (getError(cuMemAlloc(&this->_data, 10),
		CUDAHandler::MemAllocData)) return;
	if (getError(cuMemAlloc(&this->_bodies, sizeof(Body3D)*this->num_of_bodies),
		CUDAHandler::MemAllocBodies)) return;
	if (getError(cuMemAlloc(&this->_mutex, this->num_of_threads),
		CUDAHandler::MemAllocMutex)) return;
	if (getError(cuMemsetD8(this->_mutex, 0, this->num_of_threads),
		CUDAHandler::MemsetMutex)) return;
	if (getError(cuMemcpyHtoD(this->_data, this->data, 10),
		CUDAHandler::MemCpyHtoDData)) return;
	if (getError(cuMemcpyHtoD(this->_bodies, this->bodies, sizeof(Body3D)*this->num_of_bodies),
		CUDAHandler::MemCpyHtoDBodies)) return;

	void *args[] = { &this->_bodies, &this->data, &this->num_of_bodies, &this->max_calculations, &this->num_of_threads };
	if (!getError(cuLaunchKernel(this->_function, 1, 1, 1, this->num_of_threads, 1, 1, 0, NULL, args, NULL),
		CUDAHandler::LaunchKernel)) return;
	log_line(0x00EF, this->_device, this->_context, this->_module, this->_function, this->_data, this->_bodies);
}
void CUDAHandler::DeinitializeCUDA()
{
	cuModuleUnload(this->_module);
	cuCtxDestroy(this->_context);
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

void CUDAHandler::UpdateCUDA()
{
	((char*)this->data)[0] = (char)*this->exit;
	((char*)this->data)[1] = (char)*this->pause;
	if (getError(cuMemcpyHtoD(this->_data, this->data, 2),
		CUDAHandler::MemCpyHtoDData)) return;
	if (getError(cuMemcpyDtoH(this->calculations, this->_data+2, 8),
		CUDAHandler::MemCpyDtoHData)) return;
	if (getError(cuMemcpyDtoH(this->bodies, this->_bodies, sizeof(Body3D)*this->num_of_bodies),
		CUDAHandler::MemCpyDtoHBodies)) return;
}
