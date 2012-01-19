#include "..\SharedHeader.h"

char* getpath(char* arg, char* file)
{
	string str = string(arg).substr(0, string(arg).find_last_of("\\/"));
	string final = (str.append("\\").append((string)file));
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
		case 0x00: return "Leapfrog";
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
	shared->error = 0;
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
