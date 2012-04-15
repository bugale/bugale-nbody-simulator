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

Data::Data(char* settings_filename, char* bodies_filename)
{
	this->error = Errors::NoError;
	this->parseSettings(settings_filename);
	this->parseBodies(bodies_filename);
}
Data::~Data()
{
	if (this->bodies != 0) free(this->bodies);
}

void Data::parseSettings(char* filename)
{
	unsigned char* data = this->readData(filename);

	//Check Error//
	if (data == 0)
	{
		this->error = Errors::CannotOpenSettingsFile;
		return;
	}
	if (this->filelength != INFO_SETTINGS_SIZE)
	{
		this->error = Errors::WrongSettingsFileSize;
		return;
	}
	if (*(unsigned long long*)&data[0] != INFO_SETTINGS_START)
	{
		this->error = Errors::WrongSettingsFileHeader;
		return;
	}
	if (*(int*)&data[8] != INFO_DATA_FILES_VERSION)
	{
		this->error = Errors::WrongSettingsFileVersion;
		return;
	}

	int cur = 0;

	//Parse booleans
	this->two_dimensional_calculation = this->readBool(data, 12, cur++);
	this->two_dimensional_graphic     = this->readBool(data, 12, cur++);
	this->two_dimensional_binary      = this->readBool(data, 12, cur++);
	this->fullscreen                  = this->readBool(data, 12, cur++);
	this->clear_screen                = this->readBool(data, 12, cur++);
	this->show_trails                 = this->readBool(data, 12, cur++);
	this->min_text                    = this->readBool(data, 12, cur++);
	this->crosshair                   = this->readBool(data, 12, cur++);

	cur = 0;
	this->wireframe                   = this->readBool(data, 13, cur++);
	this->paused                      = this->readBool(data, 13, cur++);
	this->log                         = this->readBool(data, 13, cur++);

	cur = 14;
	this->width                = *(unsigned int*)&data[cur]; cur += 4;
	this->height               = *(unsigned int*)&data[cur]; cur += 4;
	this->algorithm            =                  data[cur]; cur += 1;
	this->dt                   = *(double      *)&data[cur]; cur += 8;
	this->graphic_max_rate     = *(float       *)&data[cur]; cur += 4;
	this->binary_max_rate      = *(float       *)&data[cur]; cur += 4;
	this->max_calculations     = *(long long   *)&data[cur]; cur += 8;
	this->max_trails           = *(unsigned int*)&data[cur]; cur += 4;
	this->stick_to_body        = *(unsigned int*)&data[cur]; cur += 4;
	this->sphere_slices        = *(unsigned int*)&data[cur]; cur += 4;
	this->sphere_stacks        = *(unsigned int*)&data[cur]; cur += 4;
	this->field_of_view        = *(float       *)&data[cur]; cur += 4;
	this->near_plane_distance  = *(double      *)&data[cur]; cur += 8;
	this->far_plane_distance   = *(double      *)&data[cur]; cur += 8;
	this->camera_positionX     = *(double      *)&data[cur]; cur += 8;
	this->camera_positionY     = *(double      *)&data[cur]; cur += 8;
	this->camera_positionZ     = *(double      *)&data[cur]; cur += 8;
	this->camera_targetX       = *(double      *)&data[cur]; cur += 8;
	this->camera_targetY       = *(double      *)&data[cur]; cur += 8;
	this->camera_targetZ       = *(double      *)&data[cur]; cur += 8;
	this->camera_upX           = *(float       *)&data[cur]; cur += 4;
	this->camera_upY           = *(float       *)&data[cur]; cur += 4;
	this->camera_upZ           = *(float       *)&data[cur]; cur += 4;
	this->keyboard_move_speed0 = *(float       *)&data[cur]; cur += 4;
	this->keyboard_move_speed1 = *(float       *)&data[cur]; cur += 4;
	this->keyboard_zoom_speed0 = *(float       *)&data[cur]; cur += 4;
	this->keyboard_zoom_speed1 = *(float       *)&data[cur]; cur += 4;

	free(data);

	this->algorithm_name = get_algorithm_name(this->algorithm);

	this->validateData();
}
void Data::parseBodies(char* filename)
{
	unsigned char* data = this->readData(filename);

	//Check Error//
	if (data == 0)
	{
		this->error = Errors::CannotOpenBodyDataFile;
		return;
	}
	if (this->filelength < INFO_BODY_HEADER_SIZE)
	{
		this->error = Errors::WrongBodyDataFileSize;
		return;
	}
	if (*(unsigned long long*)&data[0] != INFO_BODIES_START)
	{
		this->error = Errors::WrongBodyDataFileHeader;
		return;
	}
	if (*(int*)&data[8] != INFO_DATA_FILES_VERSION)
	{
		this->error = Errors::WrongBodyDataFileVersion;
		return;
	}

	this->g = *(double*)&data[12];
	this->num_of_bodies = *(int*)&data[20];

	//Check Error//
	if (this->num_of_bodies < 0)
	{
		this->error = Errors::TooManyBodies;
		return;
	}
	if (this->filelength < (INFO_BODY_HEADER_SIZE + this->num_of_bodies * INFO_BODY_SIZE))
	{
		this->error = Errors::WrongBodyDataFileSize2;
		return;
	}

	this->bodies = (Body3D*)safe_malloc(num_of_bodies * sizeof(Body3D), 0x00C1);
	for (int i = 0; i < this->num_of_bodies; i++)
		readBody(data, i * INFO_BODY_SIZE + INFO_BODY_HEADER_SIZE, this->bodies, i);

	this->validateBodies();

	free(data);
}
void Data::readBody(unsigned char* data, int byte_index, Body3D* bodies, int body_index)
{
	int cur = byte_index;
	bodies[body_index] = Body3D();

	//Input
	bodies[body_index]._positionX   = *(double*)&data[cur]; cur += 8;
	bodies[body_index]._positionY   = *(double*)&data[cur]; cur += 8;
	bodies[body_index]._positionZ   = *(double*)&data[cur]; cur += 8;
	bodies[body_index]._velocityX   = *(double*)&data[cur]; cur += 8;
	bodies[body_index]._velocityY   = *(double*)&data[cur]; cur += 8;
	bodies[body_index]._velocityZ   = *(double*)&data[cur]; cur += 8;
	bodies[body_index]._mass        = *(double*)&data[cur]; cur += 8;
	bodies[body_index]._radius      = *(double*)&data[cur]; cur += 8;
	bodies[body_index]._trailwidth  = *(double*)&data[cur]; cur += 8;
	bodies[body_index]._colorR      =            data[cur++];
	bodies[body_index]._colorG      =            data[cur++];
	bodies[body_index]._colorB      =            data[cur++];
	bodies[body_index]._colorA      =            data[cur++];
	bodies[body_index]._trailcolorR =            data[cur++];
	bodies[body_index]._trailcolorG =            data[cur++];
	bodies[body_index]._trailcolorB =            data[cur++];
	bodies[body_index]._trailcolorA =            data[cur++];

	//Zeroing
	bodies[body_index]._t_positionX = 0;
	bodies[body_index]._t_positionY = 0;
	bodies[body_index]._t_positionZ = 0;
	bodies[body_index]._l_positionX = 0;
	bodies[body_index]._l_positionY = 0;
	bodies[body_index]._l_positionZ = 0;
	bodies[body_index]._t_velocityX = 0;
	bodies[body_index]._t_velocityY = 0;
	bodies[body_index]._t_velocityZ = 0;
	bodies[body_index]._l_velocityX = 0;
	bodies[body_index]._l_velocityY = 0;
	bodies[body_index]._l_velocityZ = 0;
	bodies[body_index]._forceX = 0;
	bodies[body_index]._forceY = 0;
	bodies[body_index]._forceZ = 0;
	bodies[body_index]._l_forceX = 0;
	bodies[body_index]._l_forceY = 0;
	bodies[body_index]._l_forceZ = 0;
	bodies[body_index]._jerkX = 0;
	bodies[body_index]._jerkY = 0;
	bodies[body_index]._jerkZ = 0;
	bodies[body_index]._l_jerkX = 0;
	bodies[body_index]._l_jerkY = 0;
	bodies[body_index]._l_jerkZ = 0;
}
bool Data::readBool(unsigned char* data, int byte_index, int bit_index)
{
	return (bool)((((int)data[byte_index] & 0xff) >> bit_index) & 0x1);
}
unsigned char* Data::readData(char* filename)
{
	FILE* data_file = fopen(filename, "rb"); //Open as Read-Binary
	if (data_file)
	{
		fseek(data_file, 0, SEEK_END); //Seek to the End of the File
		this->filelength = ftell(data_file); //Get Current Position = End of the File = File Length
		unsigned char* data = (unsigned char*)safe_malloc(this->filelength, 0x00C2);
		rewind(data_file); //Go Back to the Start of the File
		fread(data, 1, this->filelength, data_file); //Read the File
		fclose(data_file);
		return data;
	}
	return 0;
}
void Data::validateData()
{
	if (this->dt < 0)                    this->error = Errors::NegativeDT;
	if (this->field_of_view <= 0)        this->error = Errors::NonPositiveFieldOfView;
	if (this->keyboard_move_speed0 < 0)  this->error = Errors::NegativeMoveSpeed0;
	if (this->keyboard_move_speed1 <= 0) this->error = Errors::NonPositiveMoveSpeed1;
	if (this->keyboard_zoom_speed0 < 0)  this->error = Errors::NegativeZoomSpeed0;
	if (this->keyboard_zoom_speed1 <= 0) this->error = Errors::NonPositiveZoomSpeed1;
}
void Data::validateBodies()
{
	for (int i = 0; i < this->num_of_bodies; i++)
	{
		if (this->bodies[i]._mass < 0) this->error = Errors::NegativeBodyMass;
		if (this->bodies[i]._radius < 0) this->error = Errors::NegativeBodyRadius;
		if (this->bodies[i]._trailwidth < 0) this->error = Errors::NegativeBodyTrailWidth;
	}
}
