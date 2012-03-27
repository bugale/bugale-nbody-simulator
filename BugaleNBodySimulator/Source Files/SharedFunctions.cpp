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

char* getpath(char* arg, char* file)
{
	std::string str = std::string(arg).substr(0, std::string(arg).find_last_of("\\/"));
	std::string final = (str.append("\\").append((std::string)file));
	char* ret = (char*)malloc(final.length());
	final.copy(ret,	final.length(), 0);
	ret[final.length()] = 0;
	return ret;
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
	shared->start_time = get_current_time_usec();
	shared->error_energy = 0;
	shared->error_momentum = 0;
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
