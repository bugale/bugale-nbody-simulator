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
#ifndef __SHAREDDATA_INCLUDED__
#define __SHAREDDATA_INCLUDED__

#include "../SharedHeader.h"

struct SharedData
{
	tthread::mutex logging_now;
	char* log_file;
	long long frames;
	long long calculations;
	long long frames_saved;
	long long calculations_saved;
	double frames_per_second;
	double calculations_per_second;
	long long ms_frames;
	long long ms_calculations;
	long long real_start_time;
	long long start_time; //Start time not including pauses
	long long pause_start_time;
	double error_energy;
	double error_momentum;
	bool pause;
	bool exit;
	bool calculated_energy;
	bool calculated_momentum;
	bool reached_max_calculations;
	bool halt_graphics;

	Errors::Error error;
	int error_data_int;
	char* error_data_charptr;
};

#endif
