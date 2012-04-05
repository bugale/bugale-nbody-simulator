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

SharedData* _shared = 0;
Data* _data = 0;
bool data_log = false;

char* getpath(char* arg, char* file)
{
	log_line("Entered getpath(1/3) with arg at 0x%08X and file at 0x%08X.", arg, file);
	log_line("Entered getpath(2/3) with arg as \"%s\".", arg);
	log_line("Entered getpath(3/3) with file as \"%s\".", file);
	std::string str = std::string(arg).substr(0, std::string(arg).find_last_of("\\/"));
	std::string final = (str.append("\\").append((std::string)file));
	char* ret = (char*)malloc(final.length());
	final.copy(ret,	final.length(), 0);
	ret[final.length()] = 0;
	log_line("Ended getpath(1/2) with result at 0x%08X.", ret);
	log_line("Ended getpath(2/2) with result as \"%s\".", ret);
	return ret;
}
void copy_double_to_char_array(char* arr, int index, double d)
{
	if (data_log) log_line("Entered copy_double_to_char_array with arr at 0x%08X index as %d and d as %d.", arr, index, d);
	for (int i = 0; i < 8; i++) arr[index + i] = ((char*)(&d))[i];
	if (data_log) log_line("Ended copy_double_to_char_array.");
}
void copy_long_to_char_array(char* arr, int index, long long l)
{
	if (data_log) log_line("Entered copy_long_to_char_array with arr at 0x%08X index as %d and l as %d.", arr, index, l);
	for (int i = 0; i < 8; i++) arr[index + i] = ((char*)(&l))[i];
	if (data_log) log_line("Ended copy_long_to_char_array.");
}
int add_before(int* a, int b)
{
	if (data_log) log_line("Entered add_before(1/2) with a at 0x%08X and b as %d.", a, b);
	if (data_log) log_line("Entered add_before(2/2) with a as %d.", *a);
	int ret = *a;
	*a += b;
	if (data_log) log_line("Ended add_before with result as %d.", ret);
	return ret;
}
long long get_current_time_usec()
{
	LARGE_INTEGER freq;
	LARGE_INTEGER time;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&time);
	time.QuadPart *= 1000000;
	return time.QuadPart / freq.QuadPart;
}
char* get_algorithm_name(char algorithm)
{
	switch (algorithm)
	{
		case 0x00: return "ModifiedEuler";
		case 0x01: return "Hermite";
	}
	return "";
}
void usleep(long long useconds)
{
	long long time = get_current_time_usec();
	while (get_current_time_usec() - time < useconds);
}
void reset_shared_data(SharedData* shared)
{
	shared->frames = 0;
	shared->frames_saved = 0;
	shared->frames_per_second = 0;
	shared->calculations = 0;
	shared->calculations_saved = 0;
	shared->calculations_per_second = 0;
	shared->ms_calculations = 0;
	shared->ms_frames = 0;
	shared->real_start_time = get_current_time_usec();
	shared->start_time = get_current_time_usec();
	shared->error_energy = 0;
	shared->error_momentum = 0;
	shared->log_file = 0;
	shared->pause = false;
	shared->exit = false;
	shared->reached_max_calculations = false;
}
double getAngle(double x, double y)
{
	x /= sqrt(x*x+y*y);
	y /= sqrt(x*x+y*y);
	if (y == 0 && x == -1) return  M_PI;
	if (y >  0)           return  acos(x);
	if (y <  0)           return -acos(x);
	return 0;
}
void start_log(char* filename)
{
	_shared->log_file = fopen(filename, "a");
	time_t real_start_time; time(&real_start_time);
	fprintf(_shared->log_file, "--------------------------------\nStart of logging:\n");
	fprintf(_shared->log_file, "Start Time: %s", asctime(localtime(&real_start_time)));
	fprintf(_shared->log_file, "Fetched Settings:\n");
	fprintf(_shared->log_file, " - width: %d  height: %d\n", _data->width, _data->height);
	fprintf(_shared->log_file, " - two_dimensional_calculation: %d  two_dimensional_graphic: %d  two_dimensional_binary: %d\n", _data->two_dimensional_calculation, _data->two_dimensional_graphic, _data->two_dimensional_binary);
	fprintf(_shared->log_file, " - fullscreen: %d  clear_screen: %d  show_trails: %d\n", _data->fullscreen, _data->clear_screen, _data->show_trails);
	fprintf(_shared->log_file, " - min_text: %d  crosshair: %d  paused: %d  log: %d\n", _data->min_text, _data->crosshair, _data->paused, _data->log);
	fprintf(_shared->log_file, " - algorithm: %d  graphic_max_rate: %G  binary_max_rate: %G\n", _data->algorithm, _data->graphic_max_rate, _data->binary_max_rate);
	fprintf(_shared->log_file, " - max_trails: %d  stick_to_body: %d  sphere_slices: %d  sphere_stacks: %d\n", _data->max_trails, _data->stick_to_body, _data->sphere_slices, _data->sphere_stacks);
	fprintf(_shared->log_file, " - field_of_view: %G  near_plane_distance: %G  far_plane_distance: %G\n", _data->field_of_view, _data->near_plane_distance, _data->far_plane_distance);
	fprintf(_shared->log_file, " - camera_position: (%G,%G,%G)\n", _data->camera_positionX, _data->camera_positionY, _data->camera_positionZ);
	fprintf(_shared->log_file, " - camera_target: (%G,%G,%G)\n", _data->camera_targetX, _data->camera_targetY, _data->camera_targetZ);
	fprintf(_shared->log_file, " - camera_up: (%G,%G,%G)\n", _data->camera_upX, _data->camera_upY, _data->camera_upZ);
	fprintf(_shared->log_file, " - keyboard_move_speed0: %G  keyboard_move_speed1: %G\n", _data->keyboard_move_speed0, _data->keyboard_move_speed1);
	fprintf(_shared->log_file, " - keyboard_zoom_speed0: %G  keyboard_zoom_speed1: %G\n\n\n", _data->keyboard_zoom_speed0, _data->keyboard_zoom_speed1);
}
void log_line(char* line, ...)
{
	if (!data_log) return;
	if (_shared == 0 || _data == 0 || !_data->log || _shared->log_file == 0) return;
	double runtime = (double)(get_current_time_usec() - _shared->real_start_time) / 1000000;
	va_list args;
    va_start(args, line);
	fprintf(_shared->log_file, "%013.8f: ", runtime);
	vfprintf(_shared->log_file, line, args);
	fprintf(_shared->log_file, "\n");
	va_end(args);
}
void end_log()
{
	time_t end_time; time(&end_time);
	double simulationtime = (double)((get_current_time_usec() - (_shared->start_time + (_shared->pause ? (get_current_time_usec() - _shared->pause_start_time) : 0)))) / 1000000;
	double runtime = (double)(get_current_time_usec() - _shared->real_start_time) / 1000000;
	fprintf(_shared->log_file, "\nEnd Time: %s", asctime(localtime(&end_time)));
	fprintf(_shared->log_file, "Running Time(seconds): %G\n", runtime);
	fprintf(_shared->log_file, "Simulation Running Time(seconds): %G\n", simulationtime);
	fprintf(_shared->log_file, "Number of Calculations Done: %d\n", _shared->calculations);
	fprintf(_shared->log_file, "Average Calculations Per Second: %G\n", _shared->calculations / runtime);
	fprintf(_shared->log_file, "\n\nEnd of Log!\n");
	fprintf(_shared->log_file, "--------------------------------\n\n\n");
	fclose(_shared->log_file);
}
void set_shared_data(SharedData* shared, Data* data)
{
	_shared = shared;
	_data = data;
	data_log = _data->log;
}