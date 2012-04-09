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

tthread::thread* thread_graphic = 0;
tthread::thread* thread_shared_calc = 0;
tthread::thread* thread_calculation = 0;
tthread::thread* thread_binary = 0;

void Run(char* working_directory)
{
	printf("Working Directory: %s\n\n", working_directory);
	signal(SIGINT, &exit_signal);
	InitializeConsole();
	
	usleep(100000);

	char* settings_path = (char*)malloc(4096);
	char* bodies_path = (char*)malloc(4096);
	char* binaryoutput_path = (char*)malloc(4096);
	char* log_path = (char*)malloc(4096);
	settings_path[0] = 0;
	bodies_path[0] = 0;
	binaryoutput_path[0] = 0;
	log_path[0] = 0;
	getpath(working_directory, SETTINGS_FILENAME, settings_path);
	getpath(working_directory, BODIES_FILENAME, bodies_path);
	getpath(working_directory, BINARYOUTPUT_FILENAME, binaryoutput_path);
	getpath(working_directory, LOG_FILENAME, log_path);

	data   = new Data(settings_path, bodies_path);
	shared = new SharedData();
	reset_shared_data(shared);
	set_shared_data(shared, data);

	if (data->error == Errors::NoError)
	{
		if (data->log)
		{
			printf("Logging started.\n");
			start_log(log_path);
		}
		log_line("No data errors.");
		shared->pause = data->paused;
		bom    = new BinaryOutputManager(data, 100, binaryoutput_path);
		engine = new Engine(data);
		thread_graphic     = new tthread::thread(GraphicThread, 0);
		thread_shared_calc = new tthread::thread(SharedCalculationsThread, 0);
		thread_calculation = new tthread::thread(CalculationThread, 0);
		thread_binary      = new tthread::thread(BinaryOutputThread, 0);
	}
	else
	{
		char* err = (char*)malloc(4096);
		Errors::returnError(data->error, working_directory, err);
		printf("%s\n", err);
		free(err);
	}

	while (!shared->exit) usleep(100000);

	end_log();

	ExitThreads();

	if (thread_binary != 0) delete(thread_binary);
	if (thread_calculation != 0) delete(thread_calculation);
	if (thread_shared_calc != 0) delete(thread_shared_calc);
	if (thread_graphic != 0) delete(thread_graphic);
	if (engine != 0) delete(engine);
	if (bom != 0) delete(bom);
	if (data != 0) delete(data);
	if (shared != 0) free(shared);
	if (settings_path != 0) free(settings_path);
	if (bodies_path != 0) free(bodies_path);
	if (binaryoutput_path != 0) free(binaryoutput_path);
	if (log_path != 0) free(log_path);
}
void InitializeConsole()
{
	printf("Welcome to %s version %s!\n", PROGRAM_NAME, PROGRAM_VERSION);
	printf("You may interrupt the console to close the program (usually Ctrl+C).\n");
	printf("Please Wait...\n\n");
}
void exit_signal(int sig)
{
	log_line("Entered exit_signal with shared at 0x%08X.", shared);
	shared->exit = true;
	log_line("Ended exit_signal.");
}

void GraphicThread(void* arg)
{
	log_line("Entered GraphicThread with data at 0x%08X and shared at 0x%08X.", data, shared);
	if (data->two_dimensional_graphic) NewGraphic2D(data, shared);
	else NewGraphic3D(data, shared);
	log_line("Ended GraphicThread.");
}
void CalculationThread(void* arg)
{
	log_line("Entered CalculationThread with engine at 0x%08X data at 0x%08X and shared at 0x%08X.", engine, data, shared);
	if (data->max_calculations == 0) return;
	while (!shared->exit)
	{
		if (shared->pause) usleep(100000);
		if (shared->pause) continue;
		if ( data->two_dimensional_calculation && data->algorithm == 0) engine->NextFrameModifiedEuler2D();
		if (!data->two_dimensional_calculation && data->algorithm == 0) engine->NextFrameModifiedEuler3D();
		if ( data->two_dimensional_calculation && data->algorithm == 1) engine->NextFrameHermite2D();
		if (!data->two_dimensional_calculation && data->algorithm == 1) engine->NextFrameHermite3D();

		shared->calculations++;

		if (data->max_calculations > 0 && shared->calculations >= data->max_calculations) 
		{
			shared->pause = true;
			shared->reached_max_calculations = true;
			printf("Maximum Calculations Reached!\n");
		}
	}
	log_line("Ended CalculationThread.");
}
void SharedCalculationsThread(void* arg)
{
	log_line("Entered SharedCalculationsThread with engine at 0x%08X data at 0x%08X and shared at 0x%08X.", engine, data, shared);
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
	log_line("Ended SharedCalculationsThread.");
}
void BinaryOutputThread(void* arg)
{
	log_line("Entered BinaryOutputThread with bom at 0x%08X data at 0x%08X and shared at 0x%08X.", bom, data, shared);
	if (data->binary_max_rate == 0) return;
	while (!shared->exit)
		if (shared->pause) usleep(100000);
		else if (bom->Capture(get_current_time_usec())) bom->Save();
	bom->Finalize();
	log_line("Ended BinaryOutputThread.");
}
void ExitThreads()
{
	log_line("Entered ExitThreads with thread_shared_calc at 0x%08X thread_calculation at 0x%08X thread_binary at 0x%08X and data at 0x%08X.", thread_shared_calc, thread_calculation, thread_binary, data);
	thread_shared_calc->join();
	thread_calculation->join();
	thread_graphic    ->join();
	thread_binary     ->join();
	log_line("Ended ExitThreads.");
	if (data->log) end_log();
}
