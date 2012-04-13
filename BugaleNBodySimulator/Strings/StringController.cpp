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
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <malloc.h>

#include "StringController.h"

#include "EnglishUS_Strings.h"

StringController::Language cur_lang;

void StringController::setLanguage(StringController::Language lang)
{
	cur_lang = lang;
}
void StringController::getString(int ID, char* buffer)
{
	switch (cur_lang)
	{
		case StringController::EnglishUS:
			switch (ID)
			{
				case 0x0000: strcat(buffer, LNG00_STRING0000); break;
				case 0x0001: strcat(buffer, LNG00_STRING0001); break;
				case 0x0002: strcat(buffer, LNG00_STRING0002); break;
				case 0x0003: strcat(buffer, LNG00_STRING0003); break;
				case 0x0004: strcat(buffer, LNG00_STRING0004); break;
				case 0x0005: strcat(buffer, LNG00_STRING0005); break;
				case 0x0006: strcat(buffer, LNG00_STRING0006); break;
				case 0x0007: strcat(buffer, LNG00_STRING0007); break;
				case 0x0008: strcat(buffer, LNG00_STRING0008); break;
				case 0x0009: strcat(buffer, LNG00_STRING0009); break;
				case 0x000A: strcat(buffer, LNG00_STRING000A); break;
				case 0x000B: strcat(buffer, LNG00_STRING000B); break;
				case 0x000C: strcat(buffer, LNG00_STRING000C); break;
				case 0x000D: strcat(buffer, LNG00_STRING000D); break;
				case 0x000E: strcat(buffer, LNG00_STRING000E); break;
				case 0x000F: strcat(buffer, LNG00_STRING000F); break;
				case 0x0010: strcat(buffer, LNG00_STRING0010); break;
				case 0x0011: strcat(buffer, LNG00_STRING0011); break;
				case 0x0012: strcat(buffer, LNG00_STRING0012); break;
				case 0x0013: strcat(buffer, LNG00_STRING0013); break;
				case 0x0014: strcat(buffer, LNG00_STRING0014); break;
				case 0x0015: strcat(buffer, LNG00_STRING0015); break;
				case 0x0016: strcat(buffer, LNG00_STRING0016); break;
				case 0x0017: strcat(buffer, LNG00_STRING0017); break;
				case 0x0018: strcat(buffer, LNG00_STRING0018); break;
				case 0x0019: strcat(buffer, LNG00_STRING0019); break;
				case 0x001A: strcat(buffer, LNG00_STRING001A); break;
				case 0x001B: strcat(buffer, LNG00_STRING001B); break;
				case 0x001C: strcat(buffer, LNG00_STRING001C); break;
				case 0x001D: strcat(buffer, LNG00_STRING001D); break;
				case 0x001E: strcat(buffer, LNG00_STRING001E); break;
				case 0x001F: strcat(buffer, LNG00_STRING001F); break;
				case 0x0020: strcat(buffer, LNG00_STRING0020); break;
				case 0x0021: strcat(buffer, LNG00_STRING0021); break;
				case 0x0022: strcat(buffer, LNG00_STRING0022); break;
				case 0x0023: strcat(buffer, LNG00_STRING0023); break;
				case 0x0024: strcat(buffer, LNG00_STRING0024); break;
				case 0x0025: strcat(buffer, LNG00_STRING0025); break;
				case 0x0026: strcat(buffer, LNG00_STRING0026); break;
				case 0x0027: strcat(buffer, LNG00_STRING0027); break;
				case 0x0028: strcat(buffer, LNG00_STRING0028); break;
				case 0x0029: strcat(buffer, LNG00_STRING0029); break;
				case 0x002A: strcat(buffer, LNG00_STRING002A); break;
				case 0x002B: strcat(buffer, LNG00_STRING002B); break;
				case 0x002C: strcat(buffer, LNG00_STRING002C); break;
				case 0x002D: strcat(buffer, LNG00_STRING002D); break;
				case 0x002E: strcat(buffer, LNG00_STRING002E); break;
				case 0x002F: strcat(buffer, LNG00_STRING002F); break;
				case 0x0030: strcat(buffer, LNG00_STRING0030); break;
				case 0x0031: strcat(buffer, LNG00_STRING0031); break;
				case 0x0032: strcat(buffer, LNG00_STRING0032); break;
				case 0x0033: strcat(buffer, LNG00_STRING0033); break;
				case 0x0034: strcat(buffer, LNG00_STRING0034); break;
				case 0x0035: strcat(buffer, LNG00_STRING0035); break;
				case 0x0036: strcat(buffer, LNG00_STRING0036); break;
				case 0x0037: strcat(buffer, LNG00_STRING0037); break;
				case 0x0038: strcat(buffer, LNG00_STRING0038); break;
				case 0x0039: strcat(buffer, LNG00_STRING0039); break;
				case 0x003A: strcat(buffer, LNG00_STRING003A); break;
				case 0x003B: strcat(buffer, LNG00_STRING003B); break;
				case 0x003C: strcat(buffer, LNG00_STRING003C); break;
				case 0x003D: strcat(buffer, LNG00_STRING003D); break;
				case 0x003E: strcat(buffer, LNG00_STRING003E); break;
				case 0x003F: strcat(buffer, LNG00_STRING003F); break;
				case 0x0040: strcat(buffer, LNG00_STRING0040); break;
				case 0x0041: strcat(buffer, LNG00_STRING0041); break;
				case 0x0042: strcat(buffer, LNG00_STRING0042); break;
				case 0x0043: strcat(buffer, LNG00_STRING0043); break;
				case 0x0044: strcat(buffer, LNG00_STRING0044); break;
				case 0x0045: strcat(buffer, LNG00_STRING0045); break;
				case 0x0046: strcat(buffer, LNG00_STRING0046); break;
				case 0x0047: strcat(buffer, LNG00_STRING0047); break;
				case 0x0048: strcat(buffer, LNG00_STRING0048); break;
				case 0x0049: strcat(buffer, LNG00_STRING0049); break;
				case 0x004A: strcat(buffer, LNG00_STRING004A); break;
				case 0x004B: strcat(buffer, LNG00_STRING004B); break;
				case 0x004C: strcat(buffer, LNG00_STRING004C); break;
				case 0x004D: strcat(buffer, LNG00_STRING004D); break;
				case 0x004E: strcat(buffer, LNG00_STRING004E); break;
				case 0x004F: strcat(buffer, LNG00_STRING004F); break;
				case 0x0050: strcat(buffer, LNG00_STRING0050); break;
				case 0x0051: strcat(buffer, LNG00_STRING0051); break;
				case 0x0052: strcat(buffer, LNG00_STRING0052); break;
				case 0x0053: strcat(buffer, LNG00_STRING0053); break;
				case 0x0054: strcat(buffer, LNG00_STRING0054); break;
				case 0x0055: strcat(buffer, LNG00_STRING0055); break;
				case 0x0056: strcat(buffer, LNG00_STRING0056); break;
				case 0x0057: strcat(buffer, LNG00_STRING0057); break;
				case 0x0058: strcat(buffer, LNG00_STRING0058); break;
				case 0x0059: strcat(buffer, LNG00_STRING0059); break;
				case 0x005A: strcat(buffer, LNG00_STRING005A); break;
				case 0x005B: strcat(buffer, LNG00_STRING005B); break;
				case 0x005C: strcat(buffer, LNG00_STRING005C); break;
				case 0x005D: strcat(buffer, LNG00_STRING005D); break;
				case 0x005E: strcat(buffer, LNG00_STRING005E); break;
				case 0x005F: strcat(buffer, LNG00_STRING005F); break;
				case 0x0060: strcat(buffer, LNG00_STRING0060); break;
				case 0x0061: strcat(buffer, LNG00_STRING0061); break;
				case 0x0062: strcat(buffer, LNG00_STRING0062); break;
				case 0x0063: strcat(buffer, LNG00_STRING0063); break;
				case 0x0064: strcat(buffer, LNG00_STRING0064); break;
				case 0x0065: strcat(buffer, LNG00_STRING0065); break;
				case 0x0066: strcat(buffer, LNG00_STRING0066); break;
				case 0x0067: strcat(buffer, LNG00_STRING0067); break;
				case 0x0068: strcat(buffer, LNG00_STRING0068); break;
				case 0x0069: strcat(buffer, LNG00_STRING0069); break;
				case 0x006A: strcat(buffer, LNG00_STRING006A); break;
				case 0x006B: strcat(buffer, LNG00_STRING006B); break;
				case 0x006C: strcat(buffer, LNG00_STRING006C); break;
				case 0x006D: strcat(buffer, LNG00_STRING006D); break;
				case 0x006E: strcat(buffer, LNG00_STRING006E); break;
				case 0x006F: strcat(buffer, LNG00_STRING006F); break;
				case 0x0070: strcat(buffer, LNG00_STRING0070); break;
				case 0x0071: strcat(buffer, LNG00_STRING0071); break;
				case 0x0072: strcat(buffer, LNG00_STRING0072); break;
				case 0x0073: strcat(buffer, LNG00_STRING0073); break;
				case 0x0074: strcat(buffer, LNG00_STRING0074); break;
				case 0x0075: strcat(buffer, LNG00_STRING0075); break;
				case 0x0076: strcat(buffer, LNG00_STRING0076); break;
				case 0x0077: strcat(buffer, LNG00_STRING0077); break;
				case 0x0078: strcat(buffer, LNG00_STRING0078); break;
				case 0x0079: strcat(buffer, LNG00_STRING0079); break;
				case 0x007A: strcat(buffer, LNG00_STRING007A); break;
				case 0x007B: strcat(buffer, LNG00_STRING007B); break;
				case 0x007C: strcat(buffer, LNG00_STRING007C); break;
				case 0x007D: strcat(buffer, LNG00_STRING007D); break;
				case 0x007E: strcat(buffer, LNG00_STRING007E); break;
				case 0x007F: strcat(buffer, LNG00_STRING007F); break;
				case 0x0080: strcat(buffer, LNG00_STRING0080); break;
				case 0x0081: strcat(buffer, LNG00_STRING0081); break;
				case 0x0082: strcat(buffer, LNG00_STRING0082); break;
				case 0x0083: strcat(buffer, LNG00_STRING0083); break;
				case 0x0084: strcat(buffer, LNG00_STRING0084); break;
				case 0x0085: strcat(buffer, LNG00_STRING0085); break;
				case 0x0086: strcat(buffer, LNG00_STRING0086); break;
				case 0x0087: strcat(buffer, LNG00_STRING0087); break;
				case 0x0088: strcat(buffer, LNG00_STRING0088); break;
				case 0x0089: strcat(buffer, LNG00_STRING0089); break;
				case 0x008A: strcat(buffer, LNG00_STRING008A); break;
				case 0x008B: strcat(buffer, LNG00_STRING008B); break;
				case 0x008C: strcat(buffer, LNG00_STRING008C); break;
				case 0x008D: strcat(buffer, LNG00_STRING008D); break;
				case 0x008E: strcat(buffer, LNG00_STRING008E); break;
				case 0x008F: strcat(buffer, LNG00_STRING008F); break;
				case 0x0090: strcat(buffer, LNG00_STRING0090); break;
				case 0x0091: strcat(buffer, LNG00_STRING0091); break;
				case 0x0092: strcat(buffer, LNG00_STRING0092); break;
				case 0x0093: strcat(buffer, LNG00_STRING0093); break;
				case 0x0094: strcat(buffer, LNG00_STRING0094); break;
				case 0x0095: strcat(buffer, LNG00_STRING0095); break;
				case 0x0096: strcat(buffer, LNG00_STRING0096); break;
				case 0x0097: strcat(buffer, LNG00_STRING0097); break;
				case 0x0098: strcat(buffer, LNG00_STRING0098); break;
				case 0x0099: strcat(buffer, LNG00_STRING0099); break;
				case 0x009A: strcat(buffer, LNG00_STRING009A); break;
				case 0x009B: strcat(buffer, LNG00_STRING009B); break;
				case 0x009C: strcat(buffer, LNG00_STRING009C); break;
				case 0x009D: strcat(buffer, LNG00_STRING009D); break;
				case 0x009E: strcat(buffer, LNG00_STRING009E); break;
				case 0x009F: strcat(buffer, LNG00_STRING009F); break;
				case 0x00A0: strcat(buffer, LNG00_STRING00A0); break;
				case 0x00A1: strcat(buffer, LNG00_STRING00A1); break;
				case 0x00A2: strcat(buffer, LNG00_STRING00A2); break;
				case 0x00A3: strcat(buffer, LNG00_STRING00A3); break;
				case 0x00A4: strcat(buffer, LNG00_STRING00A4); break;
				case 0x00A5: strcat(buffer, LNG00_STRING00A5); break;
				case 0x00A6: strcat(buffer, LNG00_STRING00A6); break;
				case 0x00A7: strcat(buffer, LNG00_STRING00A7); break;
				case 0x00A8: strcat(buffer, LNG00_STRING00A8); break;
				case 0x00A9: strcat(buffer, LNG00_STRING00A9); break;
				case 0x00AA: strcat(buffer, LNG00_STRING00AA); break;
				case 0x00AB: strcat(buffer, LNG00_STRING00AB); break;
				case 0x00AC: strcat(buffer, LNG00_STRING00AC); break;
				case 0x00AD: strcat(buffer, LNG00_STRING00AD); break;
				case 0x00AE: strcat(buffer, LNG00_STRING00AE); break;
				case 0x00AF: strcat(buffer, LNG00_STRING00AF); break;
				case 0x00B0: strcat(buffer, LNG00_STRING00B0); break;
				case 0x00B1: strcat(buffer, LNG00_STRING00B1); break;
				case 0x00B2: strcat(buffer, LNG00_STRING00B2); break;
				case 0x00B3: strcat(buffer, LNG00_STRING00B3); break;
				case 0x00B4: strcat(buffer, LNG00_STRING00B4); break;
				case 0x00B5: strcat(buffer, LNG00_STRING00B5); break;
				case 0x00B6: strcat(buffer, LNG00_STRING00B6); break;
				case 0x00B7: strcat(buffer, LNG00_STRING00B7); break;
				case 0x00B8: strcat(buffer, LNG00_STRING00B8); break;
				case 0x00B9: strcat(buffer, LNG00_STRING00B9); break;
				case 0x00BA: strcat(buffer, LNG00_STRING00BA); break;
				case 0x00BB: strcat(buffer, LNG00_STRING00BB); break;
				case 0x00BC: strcat(buffer, LNG00_STRING00BC); break;
				case 0x00BD: strcat(buffer, LNG00_STRING00BD); break;
				case 0x00BE: strcat(buffer, LNG00_STRING00BE); break;
				case 0x00BF: strcat(buffer, LNG00_STRING00BF); break;
				case 0x00C0: strcat(buffer, LNG00_STRING00C0); break;
				case 0x00C1: strcat(buffer, LNG00_STRING00C1); break;
				case 0x00C2: strcat(buffer, LNG00_STRING00C2); break;
				case 0x00C3: strcat(buffer, LNG00_STRING00C3); break;
				case 0x00C4: strcat(buffer, LNG00_STRING00C4); break;
				case 0x00C5: strcat(buffer, LNG00_STRING00C5); break;
				case 0x00C6: strcat(buffer, LNG00_STRING00C6); break;
				case 0x00C7: strcat(buffer, LNG00_STRING00C7); break;
				case 0x00C8: strcat(buffer, LNG00_STRING00C8); break;
				case 0x00C9: strcat(buffer, LNG00_STRING00C9); break;
				case 0x00CA: strcat(buffer, LNG00_STRING00CA); break;
				case 0x00CB: strcat(buffer, LNG00_STRING00CB); break;
				case 0x00CC: strcat(buffer, LNG00_STRING00CC); break;
				case 0x00CD: strcat(buffer, LNG00_STRING00CD); break;
				case 0x00CE: strcat(buffer, LNG00_STRING00CE); break;
				case 0x00CF: strcat(buffer, LNG00_STRING00CF); break;
				case 0x00D0: strcat(buffer, LNG00_STRING00D0); break;
				case 0x00D1: strcat(buffer, LNG00_STRING00D1); break;
				case 0x00D2: strcat(buffer, LNG00_STRING00D2); break;
				case 0x00D3: strcat(buffer, LNG00_STRING00D3); break;
				case 0x00D4: strcat(buffer, LNG00_STRING00D4); break;
				case 0x00D5: strcat(buffer, LNG00_STRING00D5); break;
				case 0x00D6: strcat(buffer, LNG00_STRING00D6); break;
				case 0x00D7: strcat(buffer, LNG00_STRING00D7); break;
				case 0x00D8: strcat(buffer, LNG00_STRING00D8); break;
				case 0x00D9: strcat(buffer, LNG00_STRING00D9); break;
				case 0x00DA: strcat(buffer, LNG00_STRING00DA); break;
				case 0x00DB: strcat(buffer, LNG00_STRING00DB); break;
				case 0x00DC: strcat(buffer, LNG00_STRING00DC); break;
				case 0x00DD: strcat(buffer, LNG00_STRING00DD); break;
				case 0x00DE: strcat(buffer, LNG00_STRING00DE); break;
				case 0x00DF: strcat(buffer, LNG00_STRING00DF); break;
				case 0x00E0: strcat(buffer, LNG00_STRING00E0); break;
				case 0x00E1: strcat(buffer, LNG00_STRING00E1); break;
				case 0x00E2: strcat(buffer, LNG00_STRING00E2); break;
				case 0x00E3: strcat(buffer, LNG00_STRING00E3); break;
				case 0x00E4: strcat(buffer, LNG00_STRING00E4); break;
				case 0x00E5: strcat(buffer, LNG00_STRING00E5); break;
				case 0x00E6: strcat(buffer, LNG00_STRING00E6); break;
				case 0x00E7: strcat(buffer, LNG00_STRING00E7); break;
				case 0x00E8: strcat(buffer, LNG00_STRING00E8); break;
				case 0x00E9: strcat(buffer, LNG00_STRING00E9); break;
				case 0x00EA: strcat(buffer, LNG00_STRING00EA); break;
				case 0x00EB: strcat(buffer, LNG00_STRING00EB); break;
				case 0x00EC: strcat(buffer, LNG00_STRING00EC); break;
				case 0x00ED: strcat(buffer, LNG00_STRING00ED); break;
				case 0x00EE: strcat(buffer, LNG00_STRING00EE); break;
				case 0x00EF: strcat(buffer, LNG00_STRING00EF); break;
				case 0x00F0: strcat(buffer, LNG00_STRING00F0); break;
				case 0x00F1: strcat(buffer, LNG00_STRING00F1); break;
				case 0x00F2: strcat(buffer, LNG00_STRING00F2); break;
				case 0x00F3: strcat(buffer, LNG00_STRING00F3); break;
				case 0x00F4: strcat(buffer, LNG00_STRING00F4); break;
				case 0x00F5: strcat(buffer, LNG00_STRING00F5); break;
				case 0x00F6: strcat(buffer, LNG00_STRING00F6); break;
				case 0x00F7: strcat(buffer, LNG00_STRING00F7); break;
				case 0x00F8: strcat(buffer, LNG00_STRING00F8); break;
				case 0x00F9: strcat(buffer, LNG00_STRING00F9); break;
				case 0x00FA: strcat(buffer, LNG00_STRING00FA); break;
				case 0x00FB: strcat(buffer, LNG00_STRING00FB); break;
				case 0x00FC: strcat(buffer, LNG00_STRING00FC); break;
				case 0x00FD: strcat(buffer, LNG00_STRING00FD); break;
				case 0x00FE: strcat(buffer, LNG00_STRING00FE); break;
				case 0x00FF: strcat(buffer, LNG00_STRING00FF); break;
			}
	}
}
void StringController::printString(int ID, ...)
{
	char* buffer = (char*)malloc(4096); buffer[0] = 0;
	StringController::getString(ID, buffer);
	va_list args;
    va_start(args, ID);
	vprintf(buffer, args);
	va_end(args);
	free(buffer);
}
