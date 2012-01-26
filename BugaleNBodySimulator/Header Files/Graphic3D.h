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
#define Graphic3D_included
#include "..\SharedHeader.h"

struct Graphic3D
{
	SharedData* shared;
	Data* data;

	int width;
	int height;
	double ratio;
	char* title;
	bool fullscreen;
	bool clear_screen;
	bool show_trails;
	bool min_text;
	bool crosshair;

	//Precalculated values
	double sin_field_of_view;

	//Camera properties
    double camera_positionX;
    double camera_positionY;
    double camera_positionZ;
	double camera_targetX;
	double camera_targetY;
	double camera_targetZ;
	double camera_upX;
	double camera_upY;
	double camera_upZ;

	double height_meters;
	double width_meters;
	double cam_target_distance;

	//Keyboard zoom
	long long keyboard_zoom_starttime; bool keyboard_zoom_started; bool keyboard_zoom_in; double keyboard_zoom_positionX_start_value;  double keyboard_zoom_positionY_start_value;  double keyboard_zoom_positionZ_start_value;
	
	//Trails
	int trail_curpos;
	double* trailX; double* trailY; double* trailZ;

	//Mouse camera control
	int mouse_curX; int mouse_curY;
	int mouse_startX; int mouse_startY;
	bool mouse_pressed; bool mouse_left;
	double mouse_start_value_upX; double mouse_start_value_upY; double mouse_start_value_upZ;
	double mouse_start_value_forwardX; double mouse_start_value_forwardY; double mouse_start_value_forwardZ;

	//Show Body
	int body_index;
	bool body_index_entered;
	int stick_body_index;
	bool stick_body_index_entered;
};

void NewGraphic3D(Data* data, SharedData* shared);
void Graphic3DInitialize();
void Graphic3DRenderHandler     ();
void Graphic3DRatioHandler      (int width, int height);
void Graphic3DKeyboardHandler   (unsigned char key, int x, int y);
void Graphic3DKeyboardUpHandler (unsigned char key, int x, int y);
void Graphic3DSKeyboardHandler  (          int key, int x, int y);
void Graphic3DSKeyboardUpHandler(          int key, int x, int y);
void Graphic3DMouseHandler      (int button, int state, int x, int y);
void Graphic3DMotionHandler     (int x, int y);

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
void Graphic3DDrawBody(double X, double Y, double Z, double radius, double R, double G, double B, double A, bool trail);
void Graphic3DProcessCameraZoom(double duration);
void Graphic3DProcessCameraMoveHorizontal(double angleHorizontal);
void Graphic3DProcessCameraMoveVertical(double angleVertical, double forwardX, double forwardY, double forwardZ);
void Graphic3DProcessTargetMoveHorizontal(double angleHorizontal);
void Graphic3DProcessTargetMoveVertical(double angleVertical, double forwardX, double forwardY, double forwardZ);
void Graphic3DFixIndefinedValues();

void Graphic3DSetOrthographicProjection();
void Graphic3DRestorePerspectiveProjection();
void Graphic3DRenderBitmapString(double x, double y, char* string);
bool Graphic3DIsInSight(double X, double Y, double Z, double radius);
