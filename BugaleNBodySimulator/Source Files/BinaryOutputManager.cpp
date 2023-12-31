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

BinaryOutputManager::BinaryOutputManager(Data* data, int buffer_frames, char* filename)
{
	log_line(0x0099, data, buffer_frames, filename);
	log_line(0x009A, filename);
	this->_buffer_frames = buffer_frames;
	this->_num_of_bodies = data->num_of_bodies;
	this->_filename = filename;
	this->_bodies = data->bodies;
	this->_frame_size = 8 + 24 * this->_num_of_bodies;
	this->_buffer_size = this->_buffer_frames * this->_frame_size;
	this->_cur_id = 0;
	this->_curpos_mem_buffer = 0;
	this->_curpos_disk_buffer = 0;
	this->_curpos_file = 0;
	this->_mem_buffer  = (char*)safe_malloc(this->_buffer_size, 0x00BF);
	this->_disk_buffer = (char*)safe_malloc(this->_buffer_size, 0x00C0);
	log_line(0x009B, this);
}
BinaryOutputManager::~BinaryOutputManager()
{
	if (this->_mem_buffer != 0) free(this->_mem_buffer);
	if (this->_disk_buffer != 0) free(this->_disk_buffer);
}
bool BinaryOutputManager::Capture(long long time)
{
	log_line(0x009C, time);
	copy_long_to_char_array(this->_mem_buffer, this->_curpos_mem_buffer, time);
	this->_curpos_mem_buffer += 8;
	for (int i = 0; i < this->_num_of_bodies; i++)
	{
		copy_double_to_char_array(this->_mem_buffer, add_before(&this->_curpos_mem_buffer, 8), this->_bodies[i]._positionX);
		copy_double_to_char_array(this->_mem_buffer, add_before(&this->_curpos_mem_buffer, 8), this->_bodies[i]._positionY);
		copy_double_to_char_array(this->_mem_buffer, add_before(&this->_curpos_mem_buffer, 8), this->_bodies[i]._positionZ);
	}
	if (this->_curpos_mem_buffer >= this->_buffer_size) 
	{
		this->_curpos_mem_buffer = this->_buffer_size;
		log_line(0x009D);
		return true;
	}
	log_line(0x009E);
	return false;
}
void BinaryOutputManager::Save()
{
	log_line(0x009F);
	for (int i = 0; i < this->_buffer_size; i++) this->_disk_buffer[i] = this->_mem_buffer[i]; //Copy Memory Buffer to Disk Buffer
	this->_curpos_disk_buffer = this->_curpos_mem_buffer; //Copy Position in Memory Buffer to the Position in Disk Buffer
	this->_curpos_mem_buffer = 0; //Zero the Position in Memory Buffer
	FILE* file = fopen(this->_filename, "ab"); //Open the File as Append-Binary
	fwrite(this->_disk_buffer, 1, this->_curpos_disk_buffer, file); //Write the Buffer
	fclose(file);
	log_line(0x00A0);
}
void BinaryOutputManager::Finalize()
{
	log_line(0x00A1);
	char fin[3 * 8];
	long long t = -1;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 8; j++)
			fin[i*8 + j] = ((char*)&t)[j];
	FILE* file = fopen(this->_filename, "ab"); //Open the File as Append-Binary
	fwrite(fin, 1, 3 * 8, file); //Write the Buffer
	fclose(file);
	log_line(0x00A2);
}
