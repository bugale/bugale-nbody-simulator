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

SharedData* shared;
Data* data;
BinaryOutputManager* bom;
Engine* engine;

boost::thread thread_graphic;
boost::thread thread_shared_calc;
boost::thread thread_calculation;
boost::thread thread_binary;

void Run(char* running_file_name)
{
	InitializeConsole();
	
	Sleep(100);

	data   = new Data(getpath(running_file_name, SETTINGS_FILENAME), getpath(running_file_name, BODIES_FILENAME));
	shared = new SharedData();
	reset_shared_data(shared);
	set_shared_data(shared, data);

	if (data->error == Errors::NoError)
	{
		if (data->log)
		{
			std::cout << "Logging started.\n\n\n\n";
			start_log(getpath(running_file_name, LOG_FILENAME));
		}
		log_line("No data errors.");
		shared->pause = data->paused;
		bom    = new BinaryOutputManager(data, 100, getpath(running_file_name, BINARYOUTPUT_FILENAME));
		engine = new Engine(data);
		thread_graphic     = boost::thread(GraphicThread);
		thread_shared_calc = boost::thread(SharedCalculationsThread);
		thread_calculation = boost::thread(CalculationThread);
		thread_binary      = boost::thread(BinaryOutputThread);
	}
	else
		std::cout << Errors::returnError(data->error);

	while (!shared->exit) Sleep(100);

	ExitThreads();
}
void InitializeConsole()
{
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, true);
	SetConsoleTitle(CONSOLE_TITLE);
	std::cout << "Press CTRL+Break or CTRL+C in the console to exit the program.\nPlease Wait...\n\n";
}
bool WINAPI ConsoleHandler(DWORD c_event)
{
	switch(c_event)
	{
		case CTRL_C_EVENT:
		case CTRL_BREAK_EVENT:
		case CTRL_CLOSE_EVENT:
			shared->exit = true;
			ExitThreads();
			break;
	}
	return false;
}

void GraphicThread()
{
	log_line("Entered GraphicThread with data at 0x%08X and shared at 0x%08X.", data, shared);
	if (data->two_dimensional_graphic) NewGraphic2D(data, shared);
	else NewGraphic3D(data, shared);
	log_line("Ended GraphicThread.");
}
void CalculationThread()
{
	log_line("Entered CalculationThread with engine at 0x%08X data at 0x%08X and shared at 0x%08X.", engine, data, shared);
	if (data->max_calculations == 0) return;
	while (!shared->exit)
	{
		if (shared->pause) Sleep(100);
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
			std::cout << "Maximum Calculations Reached!\n";
		}
	}
	log_line("Ended CalculationThread.");
}
void SharedCalculationsThread()
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
			if (shared->ms_frames >= 5000) shared->frames_per_second = 0;
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
			//if (shared->ms_calculations >= 1000) shared->calculations_per_second = 0;
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
		while (get_current_time_usec() - time < 500000) Sleep(1);
	}
	log_line("Ended SharedCalculationsThread.");
}
void BinaryOutputThread()
{
	log_line("Entered BinaryOutputThread with bom at 0x%08X data at 0x%08X and shared at 0x%08X.", bom, data, shared);
	if (data->binary_max_rate == 0) return;
	while (!shared->exit)
		if (shared->pause) Sleep(100);
		else if (bom->Capture(get_current_time_usec())) bom->Save();
	bom->Finalize();
	log_line("Ended BinaryOutputThread.");
}
void ExitThreads()
{
	log_line("Entered ExitThreads with thread_shared_calc at 0x%08X thread_calculation at 0x%08X thread_binary at 0x%08X and data at 0x%08X.", thread_shared_calc, thread_calculation, thread_binary, data);
	thread_shared_calc.join();
	thread_calculation.join();
	thread_binary     .join();
	log_line("Ended ExitThreads.");
	if (data->log) end_log();
}
