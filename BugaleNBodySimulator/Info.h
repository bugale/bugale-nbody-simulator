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
#ifndef __INFO_INCLUDED__
#define __INFO_INCLUDED__

	#define INFO_SETTINGS_SIZE 159 //Size of the settings file
	#define INFO_BODY_HEADER_SIZE 24 //Size of the header in the body data file
	#define INFO_BODY_SIZE 80 //Size in Bytes of One Body on the Disk
	#define INFO_SETTINGS_START 0xBDF0BDF01111BDF0
	#define INFO_BODIES_START   0xBDF0BDF02222BDF0
	#define INFO_DATA_FILES_VERSION 2
	#define INFO_SETTINGS_FILENAME "settings.bdf"
	#define INFO_BODIES_FILENAME "bodies.bdf"
	#define INFO_BINARY_FILENAME "binary.bin"
	#define INFO_LOG_FILENAME "log.txt"
	#define INFO_PROGRAM_NAME "Bugale N-Body Simulator"
	#define INFO_PROGRAM_VERSION "0.2.1 (Beta 2.1)"

#endif
