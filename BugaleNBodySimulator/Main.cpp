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
#include "SharedHeader.h"
#include <tinythread.cpp>

int main(int argc, char* argv[])
{
	char* buffer;
	buffer = getcwd(NULL, 0);
	StringController::setLanguage(StringController::EnglishUS);
	if (buffer == 0)
	{
		StringController::printString(0x0000);
		while (true);
	}
	Run(buffer);
	free(buffer);
	return 0;
}
