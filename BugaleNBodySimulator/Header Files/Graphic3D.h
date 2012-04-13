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
#ifndef __GRAPHIC3D_INCLUDED__
#define __GRAPHIC3D_INCLUDED__

#include "../SharedHeader.h"

struct Graphic3D
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
	bool wireframe;

	//Precalculated values
	float sin_field_of_view;
	
	//Frame-precalculated values
	double cam_target_distance2;
	float frustum[6][4];

	//Camera properties
    double camera_positionX;
    double camera_positionY;
    double camera_positionZ;
	double camera_targetX;
	double camera_targetY;
	double camera_targetZ;
	float camera_upX;
	float camera_upY;
	float camera_upZ;

	//Keyboard zoom
	long long keyboard_zoom_starttime_in; long long keyboard_zoom_starttime_out; bool keyboard_zoom_started_in; bool keyboard_zoom_started_out;
	long long keyboard_zoom_duration_in; long long keyboard_zoom_duration_out;

	//Trails
	int trail_curpos;
	double* trailX; double* trailY; double* trailZ;

	//Mouse camera control
	int mouse_curX; int mouse_curY;
	int mouse_startX; int mouse_startY;
	bool mouse_pressed; bool mouse_left;
	float mouse_start_value_upX; float mouse_start_value_upY; float mouse_start_value_upZ;
	double mouse_start_value_forwardX; double mouse_start_value_forwardY; double mouse_start_value_forwardZ;

	//Show Body
	int body_index;
	bool body_index_entered;
	int stick_body_index;
	bool stick_body_index_entered;

	char* temp_string;
};

void NewGraphic3D(Data* data, SharedData* shared);
void Graphic3DInitialize();
void Graphic3DRenderHandlerDblBfr();
void Graphic3DRenderHandlerSglBfr();
void Graphic3DRatioHandler       (int width, int height);
void Graphic3DKeyboardHandler    (unsigned char key, int x, int y);
void Graphic3DKeyboardUpHandler  (unsigned char key, int x, int y);
void Graphic3DSKeyboardHandler   (          int key, int x, int y);
void Graphic3DSKeyboardUpHandler (          int key, int x, int y);
void Graphic3DMouseHandler       (int button, int state, int x, int y);
void Graphic3DMotionHandler      (int x, int y);

void Graphic3DClearScreen();
void Graphic3DSetCamera();
void Graphic3DFinalizeFrame();

void Graphic3DExit();
void Graphic3DReset();

void Graphic3DToggleCrosshair();
void Graphic3DToggleMinText();
void Graphic3DToggleShowTrails();
void Graphic3DToggleClearScreen();
void Graphic3DToggleCameraMove(char direction, bool pressed);
void Graphic3DTogglePause();
void Graphic3DToggleWireframe();
void Graphic3DToggleStickToBody();
void Graphic3DShowBodyIndex();
void Graphic3DAddDigitToBodyIndex(int digit);

void Graphic3DProcessCameraMove();
void Graphic3DCalculateTemp();
void Graphic3DDrawBodies();
void Graphic3DDrawBodyIndex();
void Graphic3DDrawCrosshair(bool white);
void Graphic3DDrawText();
void Graphic3DDrawMinText();
void Graphic3DDrawTrails();
void Graphic3DSaveTrails();
void Graphic3DDrawBody(double X, double Y, double Z, double radius, float R, float G, float B, float A, bool trail);
void Graphic3DProcessCameraMove(float angleHorizontal, float angleVertical, float zoom_duration_in, float zoom_duration_out);
void Graphic3DProcessTargetMove(float angleHorizontal, float angleVertical, float zoom_duration_in, float zoom_duration_out);
void Graphic3DFixIndefinedValues();

void Graphic3DSetOrthographicProjection();
void Graphic3DRestorePerspectiveProjection();
void Graphic3DRenderBitmapString(int x, int y, char* string);
bool Graphic3DIsInSight(double X, double Y, double Z, double r);

#endif
