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
#ifndef __BINARYOUTPUTMANAGER_INCLUDED__
#define __BINARYOUTPUTMANAGER_INCLUDED__

#include "../SharedHeader.h"

class BinaryOutputManager
{
private:
	Body3D* _bodies;
	int _cur_id;
	int _num_of_bodies;
	int _buffer_frames;
	int _frame_size;
	int _buffer_size;
	char* _filename;
	int _curpos_file;
	int _curpos_mem_buffer;
	int _curpos_disk_buffer;
	char* _mem_buffer;
	char* _disk_buffer;

public:
	BinaryOutputManager(Data* data, int buffer_frames, char* filename);
	bool Capture(long long time); //Returns true If Saving Is Needed
	void Save(); //Append
	void Finalize();
};

#endif