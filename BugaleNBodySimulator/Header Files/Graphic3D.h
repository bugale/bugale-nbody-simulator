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


	//Keyboard target control
	long long* keyboard_move_starttime; bool* keyboard_move_started; double* keyboard_move_start_value; //X0 - zoom, 0X - minus, 1X - plus
	
	//Trails
	int trail_curpos;
	double* trailX; double* trailY; double* trailZ;

	//Mouse camera control
	int mouse_curX; int mouse_curY;

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

void Graphic3DSetOrthographicProjection();
void Graphic3DRestorePerspectiveProjection();
void Graphic3DRenderBitmapString(double x, double y, char* string);
bool Graphic3DIsInSight(double X, double Y, double Z, double radius);
