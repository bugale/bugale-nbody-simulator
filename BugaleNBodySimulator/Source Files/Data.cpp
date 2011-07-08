#include "..\SharedHeader.h"

Data::Data(char* settings_filename, char* bodies_filename)
{
	this->parseSettings(settings_filename);
	this->parseBodies(bodies_filename);
}

void Data::parseSettings(char* filename)
{
	unsigned char* data = this->readData(filename);
	int cur = 0;

	//Parse booleans
	this->two_dimensional_calculation = this->readBool(data, 0, cur++);
	this->two_dimensional_graphic     = this->readBool(data, 0, cur++);
	this->two_dimensional_binary      = this->readBool(data, 0, cur++);
	this->fullscreen                  = this->readBool(data, 0, cur++);
	this->log                         = this->readBool(data, 0, cur++);

	cur = 1;
	this->width                = *(int      *)&data[cur]; cur += 4;
	this->height               = *(int      *)&data[cur]; cur += 4;
	this->algorithm            =               data[cur]; cur += 1;
	this->dt                   = *(double   *)&data[cur]; cur += 8;
	this->graphic_max_rate     = *(double   *)&data[cur]; cur += 8;
	this->binary_max_rate      = *(double   *)&data[cur]; cur += 8;
	this->max_calculations     = *(long long*)&data[cur]; cur += 8;
	this->max_trails           = *(int      *)&data[cur]; cur += 4;
	this->sphere_slices        = *(int      *)&data[cur]; cur += 4;
	this->sphere_stacks        = *(int      *)&data[cur]; cur += 4;
	this->field_of_view        = *(double   *)&data[cur]; cur += 8;
	this->near_plane_distance  = *(double   *)&data[cur]; cur += 8;
	this->far_plane_distance   = *(double   *)&data[cur]; cur += 8;
	this->camera_positionX     = *(double   *)&data[cur]; cur += 8;
	this->camera_positionY     = *(double   *)&data[cur]; cur += 8;
	this->camera_positionZ     = *(double   *)&data[cur]; cur += 8;
	this->camera_targetX       = *(double   *)&data[cur]; cur += 8;
	this->camera_targetY       = *(double   *)&data[cur]; cur += 8;
	this->camera_targetZ       = *(double   *)&data[cur]; cur += 8;
	this->camera_upX           = *(double   *)&data[cur]; cur += 8;
	this->camera_upY           = *(double   *)&data[cur]; cur += 8;
	this->camera_upZ           = *(double   *)&data[cur]; cur += 8;
	this->keyboard_move_speed0 = *(double   *)&data[cur]; cur += 8;
	this->keyboard_move_speed1 = *(double   *)&data[cur]; cur += 8;
	this->keyboard_zoom_speed0 = *(double   *)&data[cur]; cur += 8;
	this->keyboard_zoom_speed1 = *(double   *)&data[cur]; cur += 8;
}
void Data::parseBodies(char* filename)
{
	unsigned char* data = this->readData(filename);
	this->g = *(double*)&data[0];
	this->num_of_bodies = *(int*)&data[8];
	this->bodies = (Body3D*)malloc(num_of_bodies * sizeof(Body3D));
	for (int i = 0; i < this->num_of_bodies; i++)
		readBody(data, i * body_size + 12, this->bodies, i);
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
	fseek(data_file, 0, SEEK_END); //Seek to the End of the File
	long length = ftell(data_file); //Get Current Position = End of the File = File Length
	unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char) * length);
	rewind(data_file); //Go Back to the Start of the File
	fread(data, 1, length, data_file); //Read the File
	return data;
}
