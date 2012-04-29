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

void getpath(char* arg, char* file, char* out)
{
	log_line(0x0014, arg, file, out);
	log_line(0x0015, arg);
	log_line(0x0016, file);
	log_line(0x0017, out);
	strcat(out, arg);
	strcat(out, "\\");
	strcat(out, file);
	log_line(0x0018, out);
	log_line(0x0019, out);
}
void copy_double_to_char_array(char* arr, int index, double d)
{
	for (int i = 0; i < 8; i++) arr[index + i] = ((char*)(&d))[i];
}
void copy_long_to_char_array(char* arr, int index, long long l)
{
	for (int i = 0; i < 8; i++) arr[index + i] = ((char*)(&l))[i];
}
int add_before(int* a, int b)
{
	int ret = *a;
	*a += b;
	return ret;
}
char* get_algorithm_name(char algorithm)
{
	switch (algorithm)
	{
		case 0x02: return StringController::getStringh(0x001A);
		case 0x03: return StringController::getStringh(0x001B);
		case 0x00: return StringController::getStringh(0x001C);
		case 0x01: return StringController::getStringh(0x001D);
	}
	return "";
}
long long get_current_time_usec()
{
	#ifdef _SYSTEM_WIN
		LARGE_INTEGER freq;	LARGE_INTEGER time;
		QueryPerformanceFrequency(&freq); QueryPerformanceCounter(&time);
		return time.QuadPart * 1000000 / freq.QuadPart;
	#endif
	#ifdef _SYSTEM_POSIX
		struct typespec time; clock_gettime(CLOCK_REALTIME,&time);
		return time.tv_sec * 1000000 + (time.tv_nsec / 1000);
	#endif
}
void usleep(long long useconds)
{
	tthread::this_thread::sleep_for(tthread::chrono::microseconds(useconds));
}
void reset_shared_data()
{
	_shared->frames = 0;
	_shared->frames_saved = 0;
	_shared->frames_per_second = 0;
	_shared->calculations = 0;
	_shared->calculations_saved = 0;
	_shared->calculations_per_second = 0;
	_shared->ms_calculations = 0;
	_shared->ms_frames = 0;
	_shared->real_start_time = get_current_time_usec();
	_shared->start_time = get_current_time_usec();
	_shared->error_energy = 0;
	_shared->error_momentum = 0;
	_shared->log_file = 0;
	_shared->pause = false;
	_shared->exit = false;
	_shared->reached_max_calculations = false;
	_shared->error = Errors::NoError;
	_shared->error_data_int = 0;
	_shared->error_data_charptr = 0;
	_shared->halt_graphics = true;
}
float getAngle(float x, float y)
{
	x /= sqrt(x*x+y*y);
	y /= sqrt(x*x+y*y);
	if (y == 0 && x == -1) return  (float)M_PI;
	if (y >  0)           return  (float)acos(x);
	if (y <  0)           return (float)-acos(x);
	return 0;
}
void start_log(char* filename)
{
	_shared->log_file = filename;
	FILE* log_f = fopen(_shared->log_file, "a");
	if (log_f == 0) _shared->error = Errors::CannotOpenLogFile;
	if (log_f == 0) _shared->error_data_charptr = _shared->log_file;
	if (log_f == 0) return;
	time_t now = time(0);
	fprintf(log_f, StringController::getStringh(0x001E));
	fprintf(log_f, StringController::getStringh(0x001F), ctime(&now));
	fprintf(log_f, StringController::getStringh(0x0020));
	fprintf(log_f, StringController::getStringh(0x0021), _data->width, _data->height);
	fprintf(log_f, StringController::getStringh(0x0022), _data->two_dimensional_calculation, _data->two_dimensional_graphic, _data->two_dimensional_binary);
	fprintf(log_f, StringController::getStringh(0x0023), _data->fullscreen, _data->clear_screen, _data->show_trails);
	fprintf(log_f, StringController::getStringh(0x0024), _data->min_text, _data->crosshair, _data->paused, _data->log);
	fprintf(log_f, StringController::getStringh(0x0025), _data->algorithm, _data->graphic_max_rate, _data->binary_max_rate);
	fprintf(log_f, StringController::getStringh(0x0026), _data->max_trails, _data->stick_to_body, _data->sphere_slices, _data->sphere_stacks);
	fprintf(log_f, StringController::getStringh(0x0027), _data->field_of_view, _data->near_plane_distance, _data->far_plane_distance);
	fprintf(log_f, StringController::getStringh(0x0028), _data->camera_positionX, _data->camera_positionY, _data->camera_positionZ);
	fprintf(log_f, StringController::getStringh(0x0029), _data->camera_targetX, _data->camera_targetY, _data->camera_targetZ);
	fprintf(log_f, StringController::getStringh(0x002A), _data->camera_upX, _data->camera_upY, _data->camera_upZ);
	fprintf(log_f, StringController::getStringh(0x002B), _data->keyboard_move_speed0, _data->keyboard_move_speed1);
	fprintf(log_f, StringController::getStringh(0x002C), _data->keyboard_zoom_speed0, _data->keyboard_zoom_speed1);
	fclose(log_f);
}
void log_line(int stringID, ...)
{
	if (!data_log) return;
	if (_shared == 0 || _data == 0 || !_data->log || _shared->log_file == 0) return;
	_shared->logging_now.lock();
	FILE* log_f = fopen(_shared->log_file, "a");
	if (log_f == 0) _shared->error = Errors::CannotOpenLogFile;
	if (log_f == 0) _shared->error_data_charptr = _shared->log_file;
	else
	{
		double runtime = (double)(get_current_time_usec() - _shared->real_start_time) / 1000000;
		va_list args;
		va_start(args, stringID);
		fprintf(log_f, "%013.8f: ", runtime);
		vfprintf(log_f, StringController::getStringh(stringID), args);
		fprintf(log_f, "\n");
		va_end(args);
		fclose(log_f);
	}
	_shared->logging_now.unlock();
}
void end_log()
{
	_shared->logging_now.lock();
	double simulationtime = (double)((get_current_time_usec() - (_shared->start_time + (_shared->pause ? (get_current_time_usec() - _shared->pause_start_time) : 0)))) / 1000000;
	double runtime = (double)(get_current_time_usec() - _shared->real_start_time) / 1000000;
	FILE* log_f = fopen(_shared->log_file, "a");
	if (log_f == 0) _shared->error = Errors::CannotOpenLogFile;
	if (log_f == 0) _shared->error_data_charptr = _shared->log_file;
	if (log_f == 0) return;
	time_t now = time(0);
	fprintf(log_f, StringController::getStringh(0x002D), ctime(&now));
	fprintf(log_f, StringController::getStringh(0x002E), runtime);
	fprintf(log_f, StringController::getStringh(0x002F), simulationtime);
	fprintf(log_f, StringController::getStringh(0x0030), _shared->calculations);
	fprintf(log_f, StringController::getStringh(0x0031), _shared->calculations / runtime);
	fprintf(log_f, StringController::getStringh(0x0032));
	fprintf(log_f, StringController::getStringh(0x0033));
	fclose(log_f);
}
void set_shared_data(SharedData* shared, Data* data)
{
	_shared = shared;
	_data = data;
	data_log = _data->log;
}

void* safe_malloc(int size, int fail_stringID)
{
	void* ret = 0;
	if (size < 0 || (ret = malloc(size)) == 0)
	{
		printf(StringController::getStringh(fail_stringID), size);
		while (getchar());
	}
	return ret;
}
