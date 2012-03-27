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

boost::thread thread_log;
boost::thread thread_graphic;
boost::thread thread_shared_calc;
boost::thread thread_calculation;
boost::thread thread_binary;

void Run(char* running_file_name)
{
	InitializeConsole();
	
	Sleep(100);

	shared = new SharedData();
	reset_shared_data(shared);
	data   = new Data(getpath(running_file_name, SETTINGS_FILENAME), getpath(running_file_name, BODIES_FILENAME));

	if (data->error == Errors::NoError)
	{
		bom    = new BinaryOutputManager(data, 100, getpath(running_file_name, BINARYOUTPUT_FILENAME));
		engine = new Engine(data);
		thread_log         = boost::thread(LogThread, getpath(running_file_name, LOG_FILENAME), engine, data, shared);
		thread_graphic     = boost::thread(GraphicThread, data, shared);
		thread_shared_calc = boost::thread(SharedCalculationsThread, engine, data, shared);
		thread_calculation = boost::thread(CalculationThread, engine, data, shared);
		thread_binary      = boost::thread(BinaryOutputThread, bom, data, shared);
	}
	else std::cout << Errors::returnError(data->error);

	while (!shared->exit) Sleep(100);

	ExitThreads();
}
void InitializeConsole()
{
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, true);
	SetConsoleTitle("Bugale N-Body Simulator Beta 0.3");
	std::cout << "Press CTRL+Break or CTRL+C in the console to exit the program.\n\n";
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

void GraphicThread(Data* data, SharedData* shared)
{
	if (data->two_dimensional_graphic) NewGraphic2D(data, shared);
	else NewGraphic3D(data, shared);
}
void CalculationThread(Engine* engine, Data* data, SharedData* shared)
{
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
}
void SharedCalculationsThread(Engine* engine, Data* data, SharedData* shared)
{
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
}
void BinaryOutputThread(BinaryOutputManager* bom, Data* data, SharedData* shared)
{
	if (data->binary_max_rate == 0) return;

	while (!shared->exit)
		if (shared->pause) Sleep(100);
		else if (bom->Capture(get_current_time_usec())) bom->Save();

	bom->Finalize();
}
void LogThread(char* filename, Engine* engine, Data* data, SharedData* shared)
{
	if (!data->log) return;

	FILE* file = fopen(filename, "a");
	time_t start_time; time(&start_time);
	std::cout << "Log started. The file will be created when you close the program\n\n\n\n";

	while (!shared->exit) Sleep(10);

	time_t end_time; time(&end_time);
	double runtime = (double)((get_current_time_usec() - (shared->start_time + (shared->pause ? (get_current_time_usec() - shared->pause_start_time) : 0)))) / 1000000;

	fprintf(file, "Start Time: %s", asctime(localtime(&start_time)));
	fprintf(file, "End Time:   %s",   asctime(localtime(&end_time)));
	fprintf(file, "Running Time(seconds): %G\n", runtime);
	fprintf(file, "Dt: %G\n", data->dt);
	fprintf(file, "G:  %G\n", data->g);
	fprintf(file, "Number of Calculations Done: %d\n", shared->calculations);
	fprintf(file, "Simulated Time(seconds): %G\n", data->dt * shared->calculations);
	fprintf(file, "Number of Bodies: %d\n", data->num_of_bodies);
	fprintf(file, "Average Calculations Per Second: %G\n", shared->calculations / runtime);
	fprintf(file, "Energy Error: %G\n", data->two_dimensional_calculation ? engine->GetEnergyError2D(): engine->GetEnergyError3D());
	fprintf(file, "\n");
	fprintf(file, "\n");
	fprintf(file, "Position and Velocity of Each Body: \n");
	fprintf(file, "\n");
	for (int i = 0; i < data->num_of_bodies; i++)
	{
		fprintf(file, " Body #%d: \n", i);
		fprintf(file, "  Position: (%+E, %+E, %+E)\n", data->bodies[i]._positionX, data->bodies[i]._positionY, data->bodies[i]._positionZ);
		fprintf(file, "  Velocity: (%+E, %+E, %+E)\n", data->bodies[i]._velocityX, data->bodies[i]._velocityY, data->bodies[i]._velocityZ);
		fprintf(file, "\n");
	}
	fprintf(file, "End of Log!\n");
	fprintf(file, "--------------------\n\n\n");
}
void ExitThreads()
{
	thread_log		  .join();
	thread_shared_calc.join();
	thread_calculation.join();
	thread_binary     .join();
}
