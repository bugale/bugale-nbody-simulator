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

SharedData* shared = 0;
Data* data = 0;
BinaryOutputManager* bom = 0;
Engine* engine = 0;
CUDAHandler* cuda_engine = 0;

tthread::thread* thread_graphic = 0;
tthread::thread* thread_shared_calc = 0;
tthread::thread* thread_calculation = 0;
tthread::thread* thread_binary = 0;

void Run(char* working_directory)
{
	StringController::printString(0x0001, working_directory);

	if (working_directory[strlen(working_directory)-1] == '\\' || working_directory[strlen(working_directory)-1] == '/')
	{
		char* buffer = (char*)safe_malloc(4096, 0x00D6);
		strcpy(buffer, working_directory);
		working_directory = buffer;
		working_directory[strlen(working_directory)-1] = 0;
	}

	signal(SIGINT, &exit_signal);
	InitializeConsole();
	
	usleep(100000);

	char* settings_path = (char*)safe_malloc(strlen(working_directory)+strlen(INFO_SETTINGS_FILENAME)+2, 0x00D7);
	char* bodies_path = (char*)safe_malloc(strlen(working_directory)+strlen(INFO_BODIES_FILENAME)+2, 0x00D8);
	char* binaryoutput_path = (char*)safe_malloc(strlen(working_directory)+strlen(INFO_BINARY_FILENAME)+2, 0x00D9);
	char* log_path = (char*)safe_malloc(strlen(working_directory)+strlen(INFO_LOG_FILENAME)+2, 0x00DA);
	settings_path[0] = 0;
	bodies_path[0] = 0;
	binaryoutput_path[0] = 0;
	log_path[0] = 0;
	getpath(working_directory, INFO_SETTINGS_FILENAME, settings_path);
	getpath(working_directory, INFO_BODIES_FILENAME, bodies_path);
	getpath(working_directory, INFO_BINARY_FILENAME, binaryoutput_path);
	getpath(working_directory, INFO_LOG_FILENAME, log_path);

	data   = new Data(settings_path, bodies_path);
	shared = new SharedData();
	set_shared_data(shared, data);
	reset_shared_data();

	if (data->error == Errors::NoError)
	{
		if (data->log)
		{
			StringController::printString(0x0002);
			start_log(log_path);
		}
		log_line(0x0003);
		shared->pause = data->paused;
		if (data->cuda)
			cuda_engine    = new CUDAHandler(shared, data);
		bom                = new BinaryOutputManager(data, 100, binaryoutput_path);
		engine             = new Engine(data);
		thread_graphic     = new tthread::thread(GraphicThread, 0);
		thread_shared_calc = new tthread::thread(SharedCalculationsThread, 0);
		thread_calculation = new tthread::thread(CalculationThread, 0);
		thread_binary      = new tthread::thread(BinaryOutputThread, 0);
	}
	else shared->error = data->error;

	while (!shared->exit)
	{
		usleep(100000);
		if (shared->error != Errors::NoError)
		{
			shared->exit = true;
			if (shared->error == Errors::CannotOpenBodyDataFile) shared->error_data_charptr = bodies_path;
			if (shared->error == Errors::CannotOpenSettingsFile) shared->error_data_charptr = settings_path;
			Errors::returnError(shared);
			while (!getchar());
		}
	}
	
	ExitThreads();

	if (thread_binary != 0)      delete(thread_binary);
	if (thread_calculation != 0) delete(thread_calculation);
	if (thread_shared_calc != 0) delete(thread_shared_calc);
	if (thread_graphic != 0)     delete(thread_graphic);
	if (engine != 0)             delete(engine);
	if (bom != 0)                delete(bom);
	if (cuda_engine != 0)        delete(cuda_engine);
	if (data != 0)               delete(data);
	if (shared != 0)             delete(shared);
	if (settings_path != 0)      free(settings_path);
	if (bodies_path != 0)        free(bodies_path);
	if (binaryoutput_path != 0)  free(binaryoutput_path);
	if (log_path != 0)           free(log_path);
}
void InitializeConsole()
{
	#ifdef _SYSTEM_WIN
		SetConsoleTitle(INFO_PROGRAM_NAME);
		SMALL_RECT windowSize = {0, 0, 79, 40};
		SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);
	#endif
	StringController::printString(0x0004, INFO_PROGRAM_NAME, INFO_PROGRAM_VERSION);
}
void exit_signal(int sig)
{
	log_line(0x0007, shared);
	shared->exit = true;
	log_line(0x0008);
}

void GraphicThread(void* arg)
{
	log_line(0x0009, data, shared);
	if (!LoadOpenGL(shared)) return;
	if (data->two_dimensional_graphic) NewGraphic2D(data, shared);
	else NewGraphic3D(data, shared);
	log_line(0x000A);
}
void CalculationThread(void* arg)
{
	log_line(0x000B, engine, cuda_engine, data, shared);
	if (data->max_calculations == 0) return;
	bool cont = true;
	if (data->cuda)
	{
		LoadCUDA(cuda_engine);
		cont = false;
		if (cuda_engine->error != CUDAErrors::NoError)
		{
			CUDAErrors::returnError(cuda_engine, true);
			cont = true;
		}
		while (!shared->exit && !cont)
		{
			if (cuda_engine->error != CUDAErrors::NoError)
			{
				CUDAErrors::returnError(cuda_engine, false);
				shared->error = Errors::CUDAError;
				return;
			}
			if (data->max_calculations > 0 && shared->calculations >= data->max_calculations) 
			{
				shared->pause = true;
				shared->reached_max_calculations = true;
				StringController::printString(0x000C);
			}
			usleep(10000);
			if (shared->pause) usleep(100000);
			cuda_engine->UpdateCUDA();
		}
	}
	while (!shared->exit && cont)
	{
		if (shared->pause) usleep(100000);
		if (shared->pause) continue;
		if ( data->two_dimensional_calculation && data->algorithm == 0x02) engine->NextFrameEuler2D();
		if (!data->two_dimensional_calculation && data->algorithm == 0x02) engine->NextFrameEuler3D();
		if ( data->two_dimensional_calculation && data->algorithm == 0x03) engine->NextFrameLeapfrog2D();
		if (!data->two_dimensional_calculation && data->algorithm == 0x03) engine->NextFrameLeapfrog3D();
		if ( data->two_dimensional_calculation && data->algorithm == 0x00) engine->NextFrameModifiedEuler2D();
		if (!data->two_dimensional_calculation && data->algorithm == 0x00) engine->NextFrameModifiedEuler3D();
		if ( data->two_dimensional_calculation && data->algorithm == 0x01) engine->NextFrameHermite2D();
		if (!data->two_dimensional_calculation && data->algorithm == 0x01) engine->NextFrameHermite3D();

		shared->calculations++;

		if (data->max_calculations > 0 && shared->calculations >= data->max_calculations) 
		{
			shared->pause = true;
			shared->reached_max_calculations = true;
			StringController::printString(0x000C);
		}
	}
	log_line(0x000D);
}
void SharedCalculationsThread(void* arg)
{
	log_line(0x000E, engine, data, shared);
	long long time = 0;
	while (!shared->exit)
	{
		time = get_current_time_usec();
		if (shared->frames != shared->frames_saved)
		{
			shared->frames_per_second = ((double)1000 / (double)shared->ms_frames) * (double)(shared->frames - shared->frames_saved);
			shared->frames_saved = shared->frames;
			shared->ms_frames = 500;
		}
		else 
		{
			shared->ms_frames += 500;
			if (shared->ms_frames >= 10000) shared->frames_per_second = 0;
		}
		if (shared->calculations != shared->calculations_saved)
		{
			shared->calculations_per_second = ((double)1000 / (double)shared->ms_calculations) * (double)(shared->calculations - shared->calculations_saved);
			shared->calculations_saved = shared->calculations;
			shared->ms_calculations = 500;
		}
		else
		{
			shared->ms_calculations += 500;
			if (shared->ms_calculations >= 10000) shared->calculations_per_second = 0;
		}
		if (shared->pause)
		{
			if ( data->two_dimensional_calculation) shared->error_energy = engine->GetEnergyError2D();
			if (!data->two_dimensional_calculation) shared->error_energy = engine->GetEnergyError3D();
			shared->calculated_energy = true;
			if ( data->two_dimensional_calculation) shared->error_momentum = engine->GetMomentumError2D();
			if (!data->two_dimensional_calculation) shared->error_momentum = engine->GetMomentumError3D();
			shared->calculated_momentum = true;
		}
		else
		{
			shared->calculated_energy = false;
			shared->calculated_momentum = false;
		}
		while (get_current_time_usec() - time < 500000) usleep(1000);
	}
	log_line(0x000F);
}
void BinaryOutputThread(void* arg)
{
	log_line(0x0010, bom, data, shared);
	if (data->binary_max_rate == 0) return;
	while (!shared->exit)
		if (shared->pause) usleep(100000);
		else if (bom->Capture(get_current_time_usec())) bom->Save();
	bom->Finalize();
	log_line(0x0011);
}
void ExitThreads()
{
	log_line(0x0012, thread_shared_calc, thread_calculation, thread_binary, data);
	thread_shared_calc->join();
	thread_calculation->join();
	thread_graphic    ->join();
	thread_binary     ->join();
	log_line(0x0013);
	if (data->log) end_log();
}
