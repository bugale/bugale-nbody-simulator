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

char* Errors::returnError(int error)
{
	char* ret = (char*)malloc(4096);
	ret[0] = 0;
	switch (error)
	{
		case NoError:
			strcat(ret, ""); break;
		case MissingSettingsFile:
			strcat(ret, "Missing settings file(expected path: ");
			strcat(ret, SETTINGS_FILENAME);
			strcat(ret, ")!"); break;
		case MissingBodyDataFile:
			strcat(ret, "Missing settings file(expected path: ");
			strcat(ret, BODIES_FILENAME);
			strcat(ret, ")!"); break;
		case WrongSettingsFileHeader:
			strcat(ret, "Settings file is corrupted(wrong header)!"); break;
		case WrongBodyDataFileHeader:
			strcat(ret, "Body data file is corrupted(wrong header)!"); break;
		case WrongSettingsFileVersion:
			strcat(ret, "Settings file is corrupted(wrong format version)!"); break;
		case WrongBodyDataFileVersion:
			strcat(ret, "Body data file is corrupted(wrong format version)!"); break;
		case WrongSettingsFileSize:
			strcat(ret, "Settings file is corrupted(wrong file size)!"); break;
		case WrongBodyDataFileSize:
			strcat(ret, "Body data file is corrupted(wrong file size)!"); break;
		case WrongBodyDataFileSize2:
			strcat(ret, "Body data file is corrupted(wrong file size)!"); break;
		case NegativeWidth:
			strcat(ret, "Settings file is corrupted(negative width)!"); break;
		case NegativeHeight:
			strcat(ret, "Settings file is corrupted(negative height)!"); break;
		case NegativeDT:
			strcat(ret, "Settings file is corrupted(negative DT)!"); break;
		case NegativeMaxTrails:
			strcat(ret, "Settings file is corrupted(negative maximum trail dots count)!"); break;
		case NonPositiveSlices:
			strcat(ret, "Settings file is corrupted(non-positive slices count)!"); break;
		case NonPositiveStacks:
			strcat(ret, "Settings file is corrupted(non-positive stacks count)!"); break;
		case NonPositiveFieldOfView:
			strcat(ret, "Settings file is corrupted(non-positive field of view)!"); break;
		case NegativeMoveSpeed0:
			strcat(ret, "Settings file is corrupted(negative MoveSpeed0)!"); break;
		case NonPositiveMoveSpeed1:
			strcat(ret, "Settings file is corrupted(non-positive MoveSpeed1)!"); break;
		case NegativeZoomSpeed0:
			strcat(ret, "Settings file is corrupted(negative ZoomSpeed0)!"); break;
		case NonPositiveZoomSpeed1:
			strcat(ret, "Settings file is corrupted(non-positive ZoomSpeed1)!"); break;
		case NegativeBodyMass:
			strcat(ret, "Body data file is corrupted(negative body mass)!"); break;
		case NegativeBodyRadius:
			strcat(ret, "Body data file is corrupted(negative body radius)!"); break;
		case NegativeBodyTrailWidth:
			strcat(ret, "Body data file is corrupted(negative body trail width)!"); break;
		case TooManyBodies:
			strcat(ret, "Body data file is probably corrupted(too many bodies)!"); break;
		case Other:
			strcat(ret, "Unidentified Error!"); break;
		default:
			strcat(ret, "Unknown Error!"); break;
	}
	switch (error)
	{
		case MissingSettingsFile:
		case MissingBodyDataFile:
		case WrongSettingsFileHeader:
		case WrongBodyDataFileHeader:
		case WrongSettingsFileVersion:
		case WrongBodyDataFileVersion:
		case WrongSettingsFileSize:
		case WrongBodyDataFileSize:
		case WrongBodyDataFileSize2:
		case NegativeWidth:
		case NegativeHeight:
		case NegativeDT:
		case NegativeMaxTrails:
		case NonPositiveSlices:
		case NonPositiveStacks:
		case NonPositiveFieldOfView:
		case NegativeMoveSpeed0:
		case NonPositiveMoveSpeed1:
		case NegativeZoomSpeed0:
		case NonPositiveZoomSpeed1:
		case NegativeBodyMass:
		case NegativeBodyRadius:
		case NegativeBodyTrailWidth:
		case TooManyBodies:
			strcat(ret, "\nYou can use Bugale N-Body Data Editor to create and edit this file.");
			strcat(ret, "\nBugale N-Body Data Editor can be found on SourceForge.");
			strcat(ret, "\nAlternatively, you can download samples of this file for various simulations from this project's page in SourceForge.");
			break;
	}
	return ret;
}