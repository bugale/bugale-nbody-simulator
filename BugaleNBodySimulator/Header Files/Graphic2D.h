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
#ifndef __GRAPHIC2D_INCLUDED__
#define __GRAPHIC2D_INCLUDED__

#include "../SharedHeader.h"

struct Graphic2D
{
	SharedData* shared;
	Data* data;

	int singlebuf_window;
	int doublebuf_window;

	int width;
	int height;
	float ratio;
	char* title;
	bool fullscreen;
	bool clear_screen;
	bool show_trails;
	bool min_text;
	bool crosshair;

	//Precalculated values
	float pi_mul_2_div_slices;
	float sin_field_of_view;

	//Frame-precalculated values
	double height_meters; double width_meters;
	double height_meters_div_2; double width_meters_div_2;
	double posX_plus_width_meters_div_2; double posX_minus_width_meters_div_2;
	double posY_plus_height_meters_div_2; double posY_minus_height_meters_div_2;

	//Camera position
    double camera_positionX;
    double camera_positionY;
    double camera_positionZ;

	//Keyboard camera control
	long long* keyboard_move_starttime; bool* keyboard_move_started; double* keyboard_move_start_value; //X0 - x, X1 - y, X2 - z, 0X - minus, 1X - plus
	
	//Trails
	int trail_curpos;
	double* trailX; double* trailY;

	//Show Body
	int body_index;
	bool body_index_entered;
	int stick_body_index;
	bool stick_body_index_entered;

	char* temp_string;
};

void NewGraphic2D(Data* data, SharedData* shared);
void Graphic2DInitialize();
void Graphic2DRenderHandlerDblBfr();
void Graphic2DRenderHandlerSglBfr();
void Graphic2DRatioHandler       (int width, int height);
void Graphic2DKeyboardHandler    (unsigned char key, int x, int y);
void Graphic2DKeyboardUpHandler  (unsigned char key, int x, int y);
void Graphic2DSKeyboardHandler   (          int key, int x, int y);
void Graphic2DSKeyboardUpHandler (          int key, int x, int y);

void Graphic2DClearScreen();
void Graphic2DSetCamera();
void Graphic2DFinalizeFrame();

void Graphic2DExit();
void Graphic2DReset();

void Graphic2DToggleCrosshair();
void Graphic2DToggleMinText();
void Graphic2DToggleShowTrails();
void Graphic2DToggleClearScreen();
void Graphic2DToggleCameraMove(char direction, bool pressed);
void Graphic2DTogglePause();
void Graphic2DToggleStickToBody();
void Graphic2DShowBodyIndex();
void Graphic2DAddDigitToBodyIndex(int digit);

void Graphic2DProcessCameraMove();
void Graphic2DCalculateTemp();
void Graphic2DDrawBodies();
void Graphic2DDrawBodyIndex();
void Graphic2DDrawCrosshair(bool white);
void Graphic2DDrawText();
void Graphic2DDrawMinText();
void Graphic2DDrawTrails();
void Graphic2DSaveTrails();
void Graphic2DDrawBody(double X, double Y, double radius, float R, float G, float B, float A, bool trail);

void Graphic2DSetOrthographicProjection();
void Graphic2DRestorePerspectiveProjection();
void Graphic2DRenderBitmapString(int x, int y, char* string);
bool Graphic2DIsInSight(double X, double Y, double radius);

#endif