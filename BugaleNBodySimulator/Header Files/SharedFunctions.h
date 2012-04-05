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
#define SharedFunctions_included
#include "..\SharedHeader.h"

char* getpath(char* arg, char* file); //Get the Path of the File with filename %file% in the same folder as the file with the fullpath %arg%
void copy_double_to_char_array(char* arr, int index, double d);
void copy_long_to_char_array(char* arr, int index, long long l);
int add_before(int* a, int b); //Calculate %a% + %b%, and Save the Answer in %a%. Return the Value of %a% Before the Addition.
long long get_current_time_usec();
char* get_algorithm_name(char algorithm);
void usleep(long long useconds);
void reset_shared_data(SharedData* shared);
double getAngle(double x, double y);
void start_log(char* filename);
void log_line(char* line, ...);
void end_log();
void set_shared_data(SharedData* shared, Data* data);