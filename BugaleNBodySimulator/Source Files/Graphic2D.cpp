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

Graphic2D* graphic2d;

void NewGraphic2D(Data* data, SharedData* shared)
{
	log_line("Entered NewGraphic2D with data at 0x%08X and shared at 0x%08X.", data, shared);
	if (data->graphic_max_rate == 0) return;

	graphic2d = new Graphic2D();
	graphic2d->data = data;
	graphic2d->shared = shared;

	graphic2d->width = data->width;
	graphic2d->height = data->height;
	graphic2d->ratio = (float)data->width / (float)graphic2d->height;
	graphic2d->title = "Bugale N-Body Simulator 2D Graphical Output Window";
	graphic2d->fullscreen = data->fullscreen;
	graphic2d->clear_screen = data->clear_screen;
	graphic2d->show_trails = data->show_trails;
	graphic2d->min_text = data->min_text;
	graphic2d->crosshair = data->crosshair;
	graphic2d->pi_mul_2_div_slices = (float)2 * (float)M_PI / (float)graphic2d->data->sphere_slices;
	graphic2d->sin_field_of_view = (float)2 * (float)sin(graphic2d->data->field_of_view * M_PI / (180 * 2));

	graphic2d->trail_curpos = 0;
	if (graphic2d->data->max_trails != 0)
	{
		graphic2d->trailX = (double*)malloc(sizeof(double) * graphic2d->data->num_of_bodies * graphic2d->data->max_trails);
		graphic2d->trailY = (double*)malloc(sizeof(double) * graphic2d->data->num_of_bodies * graphic2d->data->max_trails);
		for (int i = 0; i < graphic2d->data->num_of_bodies * graphic2d->data->max_trails; i++) graphic2d->trailX[i] = (DBL_MAX + 1);
		for (int i = 0; i < graphic2d->data->num_of_bodies * graphic2d->data->max_trails; i++) graphic2d->trailY[i] = (DBL_MAX + 1);
	}

	graphic2d->camera_positionX = data->camera_positionX;
	graphic2d->camera_positionY = data->camera_positionY;
	graphic2d->camera_positionZ = data->camera_positionZ;

	graphic2d->keyboard_move_starttime   = (long long*)malloc(sizeof(long long) * 0x100);
	graphic2d->keyboard_move_started     = (bool     *)malloc(sizeof(bool     ) * 0x100);
	graphic2d->keyboard_move_start_value = (double   *)malloc(sizeof(double   ) * 0x100);
	for (int i = 0; i < 0xFF; i++) graphic2d->keyboard_move_starttime  [i] = 0;
	for (int i = 0; i < 0xFF; i++) graphic2d->keyboard_move_started    [i] = false;
	for (int i = 0; i < 0xFF; i++) graphic2d->keyboard_move_start_value[i] = false;

	graphic2d->body_index = 0;
	graphic2d->body_index_entered = false;
	graphic2d->stick_body_index = 0;
	graphic2d->stick_body_index_entered = false;

	printf("2D Graphical Output Instructions:\n\n");
	printf("    ESC  : Close the Simulator\n");
	printf("   Arrows: Move Your Camera\n");
	printf("   + or -: Zoom In or Zoom Out\n");
	printf("     r   : Reset Your Camera Position\n");
	printf("     m   : Toggle Minimal Text Mode = Much More Frames Per Second\n");
	printf("     t   : Toggle Trail Showal\n");
	printf("     c   : Toggle Screen Clearance After Every Frame\n");
	printf("     h   : Toggle Crosshair Showal\n");
	printf("     p   : Toggle Pause\n");
	printf("   After a Number Has Been Assigned, Press Enter to Show the Body with the\n");
	printf("   Given Index on the Center of the Screen, or Press Space to Always show the\n");
	printf("   Body with the Given Index on the Center of the Screen and press again to\n");
	printf("   disable it.\n");
	printf("\nThank you for using Bugale N-Body Simulator, and have a pleasant day!\n\n\n\n");

	Graphic2DInitialize();
	log_line("Ended NewGraphic2D.");
}
void Graphic2DInitialize()
{
	log_line("Entered Graphic2DInitialize.");
	int a = 0; glutInit(&a, 0);
	//Initialize GLUT and create the window
	glutInitDisplayMode   (GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(-1, -1);
	glutInitWindowSize    (graphic2d->width, graphic2d->height);
	graphic2d->singlebuf_window = glutCreateWindow(graphic2d->title);
	glutIgnoreKeyRepeat   (1);
	if (graphic2d->fullscreen) glutFullScreen();

	//Handlers
	glutDisplayFunc   (Graphic2DRenderHandlerSglBfr);
	glutReshapeFunc   (Graphic2DRatioHandler       );
	glutKeyboardFunc  (Graphic2DKeyboardHandler    );
	glutKeyboardUpFunc(Graphic2DKeyboardUpHandler  );
	glutSpecialFunc   (Graphic2DSKeyboardHandler   );
	glutSpecialUpFunc (Graphic2DSKeyboardUpHandler );

	log_line("Graphic2DInitialize - single buffer window set at 0x%08X.", graphic2d->singlebuf_window);

	glutInitDisplayMode   (GLUT_DOUBLE | GLUT_RGBA);
	graphic2d->doublebuf_window = glutCreateSubWindow(glutGetWindow(), 0, 0, graphic2d->width, graphic2d->height);
	glutIgnoreKeyRepeat   (1);
	if (graphic2d->fullscreen) glutFullScreen();

	//Handlers
	glutDisplayFunc   (Graphic2DRenderHandlerDblBfr);
	glutReshapeFunc   (Graphic2DRatioHandler       );
	glutKeyboardFunc  (Graphic2DKeyboardHandler    );
	glutKeyboardUpFunc(Graphic2DKeyboardUpHandler  );
	glutSpecialFunc   (Graphic2DSKeyboardHandler   );
	glutSpecialUpFunc (Graphic2DSKeyboardUpHandler );

	log_line("Graphic2DInitialize - double buffer window set at 0x%08X.", graphic2d->doublebuf_window);

	//Enter GLUT event processing cycle
	glutSetWindow(graphic2d->doublebuf_window);
	glutMainLoop();
	log_line("Ended Graphic2DInitialize.");
}

void Graphic2DRenderHandlerDblBfr()
{
	if (!graphic2d->clear_screen)
	{
		glutPostRedisplay();
		return;
	}
	Graphic2DProcessCameraMove();
	Graphic2DCalculateTemp();
	Graphic2DClearScreen();
	Graphic2DSetCamera();

	Graphic2DDrawTrails();
	Graphic2DDrawBodies();
	Graphic2DDrawBodyIndex();
	Graphic2DDrawCrosshair(true);
	Graphic2DDrawMinText();
	Graphic2DDrawText();
	Graphic2DSaveTrails();

	Graphic2DFinalizeFrame();
}
void Graphic2DRenderHandlerSglBfr()
{
	if (graphic2d->clear_screen)
	{
		glutPostRedisplay();
		return;
	}
	Graphic2DProcessCameraMove();
	Graphic2DCalculateTemp();
	Graphic2DClearScreen();
	Graphic2DSetCamera();

	Graphic2DDrawTrails();
	Graphic2DDrawBodies();
	Graphic2DDrawBodyIndex();
	Graphic2DDrawCrosshair(true);
	Graphic2DDrawMinText();
	Graphic2DDrawText();
	Graphic2DSaveTrails();

	Graphic2DFinalizeFrame();
}
void Graphic2DRatioHandler(int width, int height)
{
	log_line("Entered Graphic2DRatioHandler with width as %d and height as %d.", width, height);
	if (height == 0) height++; //Prevent a division by zero.

	graphic2d->width = width;
	graphic2d->height = height;
	graphic2d->ratio = (float)width / (float)height;

	//Recreate the Projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluPerspective(graphic2d->data->field_of_view / 2, graphic2d->ratio, graphic2d->data->near_plane_distance, graphic2d->data->far_plane_distance);

	//Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
	log_line("Ended Graphic2DRatioHandler.");
}
void Graphic2DKeyboardHandler(unsigned char key, int x, int y)
{
	Graphic2DClearScreen();
	if (key ==  27) Graphic2DExit(); //ESC Button
	if (key == 'r') Graphic2DReset();
	if (key == 'h') Graphic2DToggleCrosshair();
	if (key == 'm') Graphic2DToggleMinText();
	if (key == 't') Graphic2DToggleShowTrails();
	if (key == 'c') Graphic2DToggleClearScreen();
	if (key == 'p') Graphic2DTogglePause();
	if (key == '+') Graphic2DToggleCameraMove(0x02, true);
	if (key == '-') Graphic2DToggleCameraMove(0x12, true);

	if (key == '\r') Graphic2DShowBodyIndex();
	if (key == ' ') Graphic2DToggleStickToBody();
	if (key > 47 && key < 58) Graphic2DAddDigitToBodyIndex((int)(key - 48)); //Digit
}
void Graphic2DKeyboardUpHandler(unsigned char key, int x, int y)
{
	if (key == '+') Graphic2DToggleCameraMove(0x02, false);
	if (key == '-') Graphic2DToggleCameraMove(0x12, false);
}
void Graphic2DSKeyboardHandler(int key, int x, int y)
{
	Graphic2DClearScreen();
	if (key == GLUT_KEY_LEFT ) Graphic2DToggleCameraMove(0x00, true);
	if (key == GLUT_KEY_RIGHT) Graphic2DToggleCameraMove(0x10, true);
	if (key == GLUT_KEY_DOWN ) Graphic2DToggleCameraMove(0x01, true);
	if (key == GLUT_KEY_UP   ) Graphic2DToggleCameraMove(0x11, true);
}
void Graphic2DSKeyboardUpHandler(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT ) Graphic2DToggleCameraMove(0x00, false);
	if (key == GLUT_KEY_RIGHT) Graphic2DToggleCameraMove(0x10, false);
	if (key == GLUT_KEY_DOWN ) Graphic2DToggleCameraMove(0x01, false);
	if (key == GLUT_KEY_UP   ) Graphic2DToggleCameraMove(0x11, false);
}

void Graphic2DClearScreen()
{
	if (graphic2d->clear_screen) glClear(GL_COLOR_BUFFER_BIT);
	else
	{
		Graphic2DSetOrthographicProjection();
		glColor4i(0, 0, 0, 1);
		glRasterPos2i(0, 0);
		glBegin(GL_QUADS);
		if (graphic2d->body_index_entered)
		{
			glVertex3i(graphic2d->width - 9 * 8, 0, 0);
			glVertex3i(graphic2d->width - 9 * 8, 2 * 13, 0);
			glVertex3i(graphic2d->width, 2 * 13, 0);
			glVertex3i(graphic2d->width, 0, 0);
		}
		if (graphic2d->stick_body_index_entered)
		{
			glVertex3i(graphic2d->width - 9 * 8, 2 * 13, 0);
			glVertex3i(graphic2d->width - 9 * 8, 3 * 13, 0);
			glVertex3i(graphic2d->width, 3 * 13, 0);
			glVertex3i(graphic2d->width, 2 * 13, 0);
		}
		if (!graphic2d->min_text)
		{
			glVertex3i(0, 0, 0);
			glVertex3i(0, 14 * 13, 0);
			glVertex3i(42 * 8, 14 * 13, 0);
			glVertex3i(42 * 8, 0, 0);

			glVertex3i(0, graphic2d->height - 13 * 5, 0);
			glVertex3i(0, graphic2d->height, 0);
			glVertex3i(63 * 8, graphic2d->height, 0);
			glVertex3i(63 * 8, graphic2d->height - 13 * 5, 0);
		}
		if (graphic2d->min_text)
		{
			glVertex3i(0, 0, 0);
			glVertex3i(0, 2 * 13, 0);
			glVertex3i(30 * 8, 2 * 13, 0);
			glVertex3i(30 * 8, 0, 0);
		}
		glEnd();
		Graphic2DRestorePerspectiveProjection();
	}
}
void Graphic2DSetCamera()
{
	glLoadIdentity();
	gluLookAt(graphic2d->camera_positionX, graphic2d->camera_positionY, graphic2d->camera_positionZ,
			  graphic2d->camera_positionX, graphic2d->camera_positionY, 0,
			  0,                           1,                           0);
}
void Graphic2DFinalizeFrame()
{
	graphic2d->shared->frames++;
	if (graphic2d->data->graphic_max_rate > 0) usleep((long long)(1000000 / graphic2d->data->graphic_max_rate));
	glFinish();
	if (graphic2d->clear_screen) glutSwapBuffers();
	glutPostRedisplay();
}

void Graphic2DExit()
{
	log_line("Entered Graphic2DExit.");
	graphic2d->shared->exit = true;
	glutLeaveMainLoop();
	log_line("Ended Graphic2DExit.");
}
void Graphic2DReset()
{
	graphic2d->stick_body_index_entered = false;
	graphic2d->body_index_entered = false;
	graphic2d->body_index = 0;
	graphic2d->camera_positionX = graphic2d->data->camera_positionX;
	graphic2d->camera_positionY = graphic2d->data->camera_positionY;
	graphic2d->camera_positionZ = graphic2d->data->camera_positionZ;
}

void Graphic2DToggleCrosshair()
{
	Graphic2DDrawCrosshair(false);
	graphic2d->crosshair = !graphic2d->crosshair;
}
void Graphic2DToggleMinText()
{
	graphic2d->min_text = !graphic2d->min_text;
}
void Graphic2DToggleShowTrails()
{
	graphic2d->show_trails = !graphic2d->show_trails;
}
void Graphic2DToggleClearScreen()
{
	graphic2d->clear_screen = !graphic2d->clear_screen;
	if (graphic2d->clear_screen)
	{
		glutSetWindow(graphic2d->doublebuf_window);
		glutShowWindow();
	}
	else
	{
		glutSetWindow(graphic2d->doublebuf_window);
		glutHideWindow();
		glutSetWindow(graphic2d->singlebuf_window);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}
void Graphic2DToggleCameraMove(char direction, bool pressed)
{
	if (!graphic2d->keyboard_move_started[(direction & 0x0F) + 0x00] && !graphic2d->keyboard_move_started[(direction & 0x0F) + 0x10] && pressed)
	{
		graphic2d->keyboard_move_starttime[direction] = get_current_time_usec();
		graphic2d->keyboard_move_started  [direction] = true;
		if ((direction & 0x0F) == 0) graphic2d->keyboard_move_start_value[direction] = graphic2d->camera_positionX;
		if ((direction & 0x0F) == 1) graphic2d->keyboard_move_start_value[direction] = graphic2d->camera_positionY;
		if ((direction & 0x0F) == 2) graphic2d->keyboard_move_start_value[direction] = graphic2d->camera_positionZ;
	}
	if (graphic2d->keyboard_move_started[direction] && !pressed) graphic2d->keyboard_move_started[direction] = false;
}
void Graphic2DTogglePause()
{
	if (graphic2d->shared->reached_max_calculations) return;
	if (graphic2d->shared->pause) graphic2d->shared->start_time += get_current_time_usec() - graphic2d->shared->pause_start_time;
	else graphic2d->shared->pause_start_time = get_current_time_usec();
	graphic2d->shared->pause = !graphic2d->shared->pause;
}
void Graphic2DToggleStickToBody()
{
	if (!graphic2d->body_index_entered || graphic2d->body_index >= graphic2d->data->num_of_bodies)
	{
		graphic2d->stick_body_index_entered = false;
	}
	else
	{
		graphic2d->stick_body_index = graphic2d->body_index;
		graphic2d->stick_body_index_entered = true;
	}
	graphic2d->body_index_entered = false;
	graphic2d->body_index = 0;
}
void Graphic2DShowBodyIndex()
{
	if (!graphic2d->body_index_entered) return;
	if (graphic2d->body_index < graphic2d->data->num_of_bodies)
	{
		graphic2d->camera_positionX = graphic2d->data->bodies[graphic2d->body_index]._positionX;
		graphic2d->camera_positionY = graphic2d->data->bodies[graphic2d->body_index]._positionY;
	}
	graphic2d->body_index_entered = false;
	graphic2d->body_index = 0;
}
void Graphic2DAddDigitToBodyIndex(int digit)
{
	if (graphic2d->body_index > 99999999) graphic2d->body_index %= 100000000;
	graphic2d->body_index *= 10;
	graphic2d->body_index += digit;
	graphic2d->body_index_entered = true;
}

void Graphic2DProcessCameraMove()
{
	if (graphic2d->keyboard_move_started[0x02]) //Zoom In
	{
		double duration = (double)(get_current_time_usec() - graphic2d->keyboard_move_starttime[0x02]) / 1000000;
		double acceleration = graphic2d->data->keyboard_zoom_speed1 - graphic2d->data->keyboard_zoom_speed0;
		graphic2d->camera_positionZ = graphic2d->keyboard_move_start_value[0x02] * pow(graphic2d->data->keyboard_zoom_speed0 + acceleration * duration, -duration);
	}
	if (graphic2d->keyboard_move_started[0x12]) //Zoom Out
	{
		double duration = (double)(get_current_time_usec() - graphic2d->keyboard_move_starttime[0x12]) / 1000000;
		double acceleration = graphic2d->data->keyboard_zoom_speed1 - graphic2d->data->keyboard_zoom_speed0;
		graphic2d->camera_positionZ = graphic2d->keyboard_move_start_value[0x12] * pow(graphic2d->data->keyboard_zoom_speed0 + acceleration * duration, duration);
	}
	if (graphic2d->stick_body_index_entered) //Stick to Body
	{
		graphic2d->camera_positionX = graphic2d->data->bodies[graphic2d->stick_body_index]._positionX;
		graphic2d->camera_positionY = graphic2d->data->bodies[graphic2d->stick_body_index]._positionY;
	}
	else for (int i = 0; i < 0xFF; i++) //Move Left/Right/Up/Down
	{
		if (!graphic2d->keyboard_move_started[i]) continue;
		double duration = (double)(get_current_time_usec() - graphic2d->keyboard_move_starttime[i]) / 1000000;
		double acceleration = (graphic2d->data->keyboard_move_speed1 - graphic2d->data->keyboard_move_speed0)*graphic2d->height_meters;
		double move_meters =   graphic2d->data->keyboard_move_speed0 * graphic2d->height_meters*duration + 0.5*acceleration*duration*duration;
		if ((i & 0xF0) == 0) move_meters *= -1;
		if ((i & 0x0F) == 0) graphic2d->camera_positionX = graphic2d->keyboard_move_start_value[i] + move_meters;
		if ((i & 0x0F) == 1) graphic2d->camera_positionY = graphic2d->keyboard_move_start_value[i] + move_meters;
	}

	//Fix indefined values
	if (graphic2d->camera_positionX >  DBL_MAX) graphic2d->camera_positionX =  DBL_MAX;
	if (graphic2d->camera_positionX < -DBL_MAX) graphic2d->camera_positionX = -DBL_MAX;
	if (graphic2d->camera_positionY >  DBL_MAX) graphic2d->camera_positionY =  DBL_MAX;
	if (graphic2d->camera_positionY < -DBL_MAX) graphic2d->camera_positionY = -DBL_MAX;
	if (graphic2d->camera_positionZ >  DBL_MAX) graphic2d->camera_positionZ =  DBL_MAX;
	if (graphic2d->camera_positionZ < -DBL_MAX) graphic2d->camera_positionZ = -DBL_MAX;
	if (graphic2d->camera_positionZ == 0) graphic2d->camera_positionZ = DBL_MIN;
}
void Graphic2DCalculateTemp()
{
	graphic2d->height_meters = graphic2d->camera_positionZ * graphic2d->sin_field_of_view;
	graphic2d->width_meters  = graphic2d->camera_positionZ * graphic2d->sin_field_of_view * graphic2d->ratio;
	graphic2d->height_meters_div_2 = 0.5 * graphic2d->height_meters;
	graphic2d->width_meters_div_2  = 0.5 * graphic2d->width_meters;
	graphic2d->posX_plus_width_meters_div_2   = graphic2d->camera_positionX + graphic2d->width_meters_div_2;
	graphic2d->posX_minus_width_meters_div_2  = graphic2d->camera_positionX - graphic2d->width_meters_div_2;
	graphic2d->posY_plus_height_meters_div_2  = graphic2d->camera_positionY + graphic2d->height_meters_div_2;
	graphic2d->posY_minus_height_meters_div_2 = graphic2d->camera_positionY - graphic2d->height_meters_div_2;
}
void Graphic2DDrawBodies()
{
	for (int i = 0; i < graphic2d->data->num_of_bodies; i++)
	{
		double x = graphic2d->data->bodies[i]._positionX;
		double y = graphic2d->data->bodies[i]._positionY;
		double r = graphic2d->data->bodies[i]._radius;
		if (!Graphic2DIsInSight(x, y, r)) continue;
		float R = (float)graphic2d->data->bodies[i]._colorR * 0.003921568f;
		float G = (float)graphic2d->data->bodies[i]._colorG * 0.003921568f;
		float B = (float)graphic2d->data->bodies[i]._colorB * 0.003921568f;
		float A = (float)graphic2d->data->bodies[i]._colorA * 0.003921568f;
		Graphic2DDrawBody(x, y, r, R, G, B, A, false);
	}
}
void Graphic2DDrawBodyIndex()
{
	char* string = (char*)malloc(1024);
	Graphic2DSetOrthographicProjection();

	if (graphic2d->body_index_entered)
	{
		glColor4f(1, 0, 0, 1);
		sprintf(string, "%d", graphic2d->body_index);
		Graphic2DRenderBitmapString(graphic2d->width - strlen(string) * 8, 13, string);
	}

	if (graphic2d->stick_body_index_entered)
	{
		glColor4f(0, 0, 1, 1);
		sprintf(string, "%d", graphic2d->stick_body_index);
		Graphic2DRenderBitmapString(graphic2d->width - strlen(string) * 8, 2 * 13, string);
	}
	
	free(string);
	Graphic2DRestorePerspectiveProjection();
}
void Graphic2DDrawCrosshair(bool white)
{
	if (!graphic2d->crosshair) return;

	Graphic2DSetOrthographicProjection();
	glColor4f(white, white, white, 1);
	float length = graphic2d->height * 0.01f;
	glBegin(GL_LINES);
		glVertex2f(graphic2d->width * 0.5f - length * 0.5f, graphic2d->height * 0.5f);
		glVertex2f(graphic2d->width * 0.5f + length * 0.5f, graphic2d->height * 0.5f);
		glVertex2f(graphic2d->width * 0.5f, graphic2d->height * 0.5f - length * 0.5f);
		glVertex2f(graphic2d->width * 0.5f, graphic2d->height * 0.5f + length * 0.5f);
	glEnd();
	Graphic2DRestorePerspectiveProjection();
}
void Graphic2DDrawText()
{
	if (graphic2d->min_text) return;

	long long real_time;
	char* string = (char*)malloc(1024);
	int curY = 0;
	if (graphic2d->shared->pause) real_time = graphic2d->shared->pause_start_time - graphic2d->shared->start_time;
	else real_time = get_current_time_usec() - graphic2d->shared->start_time;
	Graphic2DSetOrthographicProjection();

	glColor4f(1, 1, 1, 1);

	sprintf(string, "Frames Per Second: %G", graphic2d->shared->frames_per_second);
	Graphic2DRenderBitmapString(0, curY += 13, string);

	sprintf(string, "Calculations Per Second:   %+E", graphic2d->shared->calculations_per_second);
	Graphic2DRenderBitmapString(0, curY += 13, string);

	sprintf(string, "Total Calculations Done:   %+E", (double)graphic2d->shared->calculations);
	Graphic2DRenderBitmapString(0, curY += 13, string);

	sprintf(string, "DeltaTime:                 %+E", graphic2d->data->dt);
	Graphic2DRenderBitmapString(0, curY += 13, string);

	sprintf(string, "Simulated Seconds Past:    %+E", (double)graphic2d->data->dt * (double)graphic2d->shared->calculations);
	Graphic2DRenderBitmapString(0, curY += 13, string);

	sprintf(string, "Simulated Years Past:      %+E", (double)graphic2d->data->dt * (double)graphic2d->shared->calculations / (double)(31557600));
	Graphic2DRenderBitmapString(0, curY += 13, string);

	sprintf(string, "Real Seconds Past:         %+E", (double)real_time / (double)1000000);
	Graphic2DRenderBitmapString(0, curY += 13, string);

	sprintf(string, "Simulated-Real Time Ratio: %+E", ((double)graphic2d->data->dt * (double)graphic2d->shared->calculations) / ((double)real_time / (double)1000000));
	Graphic2DRenderBitmapString(0, curY += 13, string);

	sprintf(string, "Gravitational Constant:    %+E", graphic2d->data->g);
	Graphic2DRenderBitmapString(0, curY += 13, string);
	
	sprintf(string, "Number of Bodies: %d", graphic2d->data->num_of_bodies);
	Graphic2DRenderBitmapString(0, curY += 13, string);

	sprintf(string, "Integration Algorithm: %s %s", (graphic2d->data->two_dimensional_calculation ? "2D": "3D"), get_algorithm_name(graphic2d->data->algorithm));
	Graphic2DRenderBitmapString(0, curY += 13, string);

	if (graphic2d->shared->calculated_energy)
		sprintf(string, "Energy Error: %+E", graphic2d->shared->error_energy);
	else
		sprintf(string, "Energy Error: Pause to Calculate...");
	Graphic2DRenderBitmapString(0, curY += 13, string);

	if (graphic2d->shared->calculated_momentum)
		sprintf(string, "Momentum Error: %+E", graphic2d->shared->error_momentum);
	else
		sprintf(string, "Moment Error: Pause to Calculate...");
	Graphic2DRenderBitmapString(0, curY += 13, string);

	curY = graphic2d->height + 13 - 3;
	
	sprintf(string, "Camera Position Z: %+E", graphic2d->camera_positionZ);
	Graphic2DRenderBitmapString(0, curY -= 13, string);
	
	sprintf(string, "Camera Position Y: %+E", graphic2d->camera_positionY);
	Graphic2DRenderBitmapString(0, curY -= 13, string);
	
	sprintf(string, "Camera Position X: %+E", graphic2d->camera_positionX);
	Graphic2DRenderBitmapString(0, curY -= 13, string);
	
	sprintf(string, "Screen Height in Meters: %E", 4 * graphic2d->height_meters);
	Graphic2DRenderBitmapString(0, curY -= 13, string);
	
	free(string);
	Graphic2DRestorePerspectiveProjection();
}
void Graphic2DDrawMinText()
{
	if (!graphic2d->min_text) return;

	char* string = (char*)malloc(1024);
	Graphic2DSetOrthographicProjection();

	glColor4f(1, 1, 1, 1);
	sprintf(string, "fps:%d cps:%d", (int)graphic2d->shared->frames_per_second, (int)graphic2d->shared->calculations_per_second);
	Graphic2DRenderBitmapString(0, 13, string);
	
	free(string);
	Graphic2DRestorePerspectiveProjection();
}
void Graphic2DDrawTrails()
{
	if (graphic2d->data->max_trails == 0) return;
	if (!graphic2d->show_trails) return;
	for (int trail = graphic2d->trail_curpos; trail < graphic2d->data->max_trails; trail++)
		for (int body = 0; body < graphic2d->data->num_of_bodies; body++)
		{
			double x = graphic2d->trailX[trail*graphic2d->data->num_of_bodies + body];
			double y = graphic2d->trailY[trail*graphic2d->data->num_of_bodies + body];
			double r = graphic2d->data->bodies[body]._trailwidth;
			if (!Graphic2DIsInSight(x, y, r)) continue;
			float R = (float)graphic2d->data->bodies[body]._trailcolorR * 0.003921568f;
			float G = (float)graphic2d->data->bodies[body]._trailcolorG * 0.003921568f;
			float B = (float)graphic2d->data->bodies[body]._trailcolorB * 0.003921568f;
			float A = (float)graphic2d->data->bodies[body]._trailcolorA * 0.003921568f;
			Graphic2DDrawBody(x, y, r, R, G, B, A, true);
		}
	for (int trail = 0; trail < graphic2d->trail_curpos; trail++)
		for (int body = 0; body < graphic2d->data->num_of_bodies; body++)
		{
			double x = graphic2d->trailX[trail*graphic2d->data->num_of_bodies + body];
			double y = graphic2d->trailY[trail*graphic2d->data->num_of_bodies + body];
			double r = graphic2d->data->bodies[body]._trailwidth;
			if (!Graphic2DIsInSight(x, y, r)) continue;
			float R = (float)graphic2d->data->bodies[body]._trailcolorR * 0.003921568f;
			float G = (float)graphic2d->data->bodies[body]._trailcolorG * 0.003921568f;
			float B = (float)graphic2d->data->bodies[body]._trailcolorB * 0.003921568f;
			float A = (float)graphic2d->data->bodies[body]._trailcolorA * 0.003921568f;
			Graphic2DDrawBody(x, y, r, R, G, B, A, true);
		}
}
void Graphic2DSaveTrails()
{
	if (graphic2d->data->max_trails == 0) return;
	if (graphic2d->shared->pause) return;
	for (int body = 0; body < graphic2d->data->num_of_bodies; body++)
	{
		graphic2d->trailX[graphic2d->trail_curpos * graphic2d->data->num_of_bodies + body] = graphic2d->data->bodies[body]._positionX;
		graphic2d->trailY[graphic2d->trail_curpos * graphic2d->data->num_of_bodies + body] = graphic2d->data->bodies[body]._positionY;
	}
	graphic2d->trail_curpos++;
	if (graphic2d->trail_curpos >= graphic2d->data->max_trails) graphic2d->trail_curpos = 0;
}
void Graphic2DDrawBody(double X, double Y, double radius, float R, float G, float B, float A, bool trail)
{
	glPushMatrix();
	glColor4f(R, G, B, A);
	glTranslated(X, Y, 0);
	if (trail) 
	{
		glBegin(GL_QUADS);
			glVertex3d(-radius,  radius, 0);
			glVertex3d(-radius, -radius, 0);
			glVertex3d( radius, -radius, 0);
			glVertex3d( radius,  radius, 0);
		glEnd();
	}
	else 
	{
		double rsin, rcos;
		glBegin(GL_TRIANGLE_FAN);
		glVertex2d(radius, 0);
		for (int i = 1; i < graphic2d->data->sphere_slices; i++)
		{
			rsin = radius * sin(graphic2d->pi_mul_2_div_slices * i);
			rcos = radius * cos(graphic2d->pi_mul_2_div_slices * i);
			glVertex2d(rcos, rsin);
			glVertex2d(rcos, rsin);
		}
		glVertex2d(radius, 0);
		glEnd();
	}
	glPopMatrix();
}

void Graphic2DSetOrthographicProjection()
{
	//Switch to projection mode
	glMatrixMode(GL_PROJECTION);

	//Save previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();

	//Reset matrix
	glLoadIdentity();

	//Set a 2D orthographic projection
	gluOrtho2D(0, graphic2d->width, graphic2d->height, 0);

	//Switch back to modelview mode
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}
void Graphic2DRestorePerspectiveProjection()
{
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);

	//Restore previous projection matrix
	glPopMatrix();

	//Get back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}
void Graphic2DRenderBitmapString(int x, int y, char* string)
{
  for (char* c = string; *c != '\0'; c++) 
  {
	if (*c != ' ') glRasterPos2i(x, y);
	if (*c != ' ') glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	x += 8;
  }
}
bool Graphic2DIsInSight(double X, double Y, double radius)
{
	if (X - radius > graphic2d->posX_plus_width_meters_div_2  ) return false;
	if (X + radius < graphic2d->posX_minus_width_meters_div_2 ) return false;
	if (Y - radius > graphic2d->posY_plus_height_meters_div_2 ) return false;
	if (Y + radius < graphic2d->posY_minus_height_meters_div_2) return false;
	return true;
}
