#define SharedData_included
#include "..\SharedHeader.h"

struct SharedData
{
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
	double error;
	bool pause;
	bool exit;
	bool calculated_energy;
	bool reached_max_calculations;
};
