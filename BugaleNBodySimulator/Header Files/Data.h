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
#define Data_included
#include "..\SharedHeader.h"

class Data
{
public:
	int filelength;
	int error;

	int width;
	int height;
	bool two_dimensional_calculation;
	bool two_dimensional_graphic;
	bool two_dimensional_binary;
	bool fullscreen;
	bool log;
	char algorithm;
	double graphic_max_rate;
	double binary_max_rate;
	long long max_calculations;
	int max_trails;

	//Graphic output related
    int sphere_slices;
    int sphere_stacks;
	double field_of_view;
	double near_plane_distance;
	double far_plane_distance;
    double camera_positionX;
    double camera_positionY;
    double camera_positionZ;
    double camera_targetX;
    double camera_targetY;
    double camera_targetZ;
    double camera_upX;
    double camera_upY;
    double camera_upZ;
    double keyboard_move_speed0; //The starting move speed of the screen in screens per second
	double keyboard_move_speed1; //The move speed of the screen after one second in screens per second
	double keyboard_zoom_speed0; //The starting zoom speed 
	double keyboard_zoom_speed1; //The zoom speed after one second

	//Body Related
	int num_of_bodies;
	double g;
	double dt;
	Body3D* bodies;

	Data(char* settings_filename, char* bodies_filename);

private:
	void parseSettings(char* filename);
	void parseBodies(char* filename);
	void readBody(unsigned char* data, int byte_index, Body3D* bodies, int body_index);
	bool readBool(unsigned char* data, int byte_index, int bit_index);
	unsigned char* readData(char* filename);
	void validateData();
	void validateBodies();
};
