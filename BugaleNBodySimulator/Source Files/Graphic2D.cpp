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
	log_line(0x0061, data, shared);
	if (data->graphic_max_rate == 0) return;

	graphic2d = new Graphic2D();
	graphic2d->data = data;
	graphic2d->shared = shared;

	graphic2d->width = data->width;
	graphic2d->height = data->height;
	graphic2d->ratio = (float)data->width / (float)graphic2d->height;
	graphic2d->title = StringController::getStringh(0x0062);
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
		graphic2d->trailX = (double*)safe_malloc(sizeof(double) * graphic2d->data->num_of_bodies * graphic2d->data->max_trails, 0x00CC);
		graphic2d->trailY = (double*)safe_malloc(sizeof(double) * graphic2d->data->num_of_bodies * graphic2d->data->max_trails, 0x00CD);
		for (int i = 0; i < graphic2d->data->num_of_bodies * graphic2d->data->max_trails; i++) graphic2d->trailX[i] = (DBL_MAX + 1);
		for (int i = 0; i < graphic2d->data->num_of_bodies * graphic2d->data->max_trails; i++) graphic2d->trailY[i] = (DBL_MAX + 1);
	}

	graphic2d->camera_positionX = data->camera_positionX;
	graphic2d->camera_positionY = data->camera_positionY;
	graphic2d->camera_positionZ = data->camera_positionZ;

	graphic2d->keyboard_move_starttime   = (long long*)safe_malloc(sizeof(long long) * 0x100, 0x00CE);
	graphic2d->keyboard_move_started     = (bool     *)safe_malloc(sizeof(bool     ) * 0x100, 0x00CF);
	graphic2d->keyboard_move_start_value = (double   *)safe_malloc(sizeof(double   ) * 0x100, 0x00D0);
	for (int i = 0; i < 0xFF; i++) graphic2d->keyboard_move_starttime  [i] = 0;
	for (int i = 0; i < 0xFF; i++) graphic2d->keyboard_move_started    [i] = false;
	for (int i = 0; i < 0xFF; i++) graphic2d->keyboard_move_start_value[i] = false;

	graphic2d->body_index = 0;
	graphic2d->body_index_entered = false;
	graphic2d->stick_body_index = 0;
	graphic2d->stick_body_index_entered = false;

	graphic2d->temp_string = (char*)safe_malloc(4096, 0x00D1);

	StringController::printString(0x0063);

	Graphic2DInitialize();

	glutMainLoop();
	
	log_line(0x0064);
	graphic2d->shared->exit = true;
	
	if (graphic2d->trailX != 0) free(graphic2d->trailX);
	if (graphic2d->trailY != 0) free(graphic2d->trailY);
	if (graphic2d->temp_string != 0) free(graphic2d->temp_string);
	if (graphic2d->keyboard_move_starttime != 0) free(graphic2d->keyboard_move_starttime);
	if (graphic2d->keyboard_move_started != 0) free(graphic2d->keyboard_move_started);
	if (graphic2d->keyboard_move_start_value != 0) free(graphic2d->keyboard_move_start_value);
	if (graphic2d != 0) delete(graphic2d);

	log_line(0x0065);
}
void Graphic2DInitialize()
{
	log_line(0x0066);
	int a = 0; glutInit(&a, 0);
	//Initialize GLUT and create the window
	glutInitDisplayMode   (GLUT_SINGLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowPosition(-1, -1);
	glutInitWindowSize    (graphic2d->width, graphic2d->height);
	graphic2d->singlebuf_window = glutCreateWindow(graphic2d->title);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutIgnoreKeyRepeat   (1);
	if (graphic2d->fullscreen) glutFullScreen();

	//Handlers
	glutDisplayFunc   (Graphic2DRenderHandlerSglBfr);
	glutReshapeFunc   (Graphic2DRatioHandler       );
	glutKeyboardFunc  (Graphic2DKeyboardHandler    );
	glutKeyboardUpFunc(Graphic2DKeyboardUpHandler  );
	glutSpecialFunc   (Graphic2DSKeyboardHandler   );
	glutSpecialUpFunc (Graphic2DSKeyboardUpHandler );

	log_line(0x0067, graphic2d->singlebuf_window);

	glutInitDisplayMode   (GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	graphic2d->doublebuf_window = glutCreateSubWindow(glutGetWindow(), 0, 0, graphic2d->width, graphic2d->height);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutIgnoreKeyRepeat   (1);
	if (graphic2d->fullscreen) glutFullScreen();

	//Handlers
	glutDisplayFunc   (Graphic2DRenderHandlerDblBfr);
	glutReshapeFunc   (Graphic2DRatioHandler       );
	glutKeyboardFunc  (Graphic2DKeyboardHandler    );
	glutKeyboardUpFunc(Graphic2DKeyboardUpHandler  );
	glutSpecialFunc   (Graphic2DSKeyboardHandler   );
	glutSpecialUpFunc (Graphic2DSKeyboardUpHandler );

	log_line(0x0068, graphic2d->doublebuf_window);

	log_line(0x0069);
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
	log_line(0x006A, width, height);
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
	log_line(0x006B);
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
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
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

			glVertex3i(0, graphic2d->height - 13 * 6, 0);
			glVertex3i(0, graphic2d->height, 0);
			glVertex3i(38 * 8, graphic2d->height, 0);
			glVertex3i(38 * 8, graphic2d->height - 13 * 6, 0);
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
	if (graphic2d->clear_screen) glutSwapBuffers();
	glFlush();
	if (graphic2d->shared->exit) Graphic2DExit();
	else glutPostRedisplay();
}

void Graphic2DExit()
{
	glutLeaveMainLoop();
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
	graphic2d->vertices = 0;
	graphic2d->faces = 0;
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
	if (graphic2d->body_index_entered)
	{
		Graphic2DSetOrthographicProjection();
		glColor4f(1, 0, 0, 1);
		sprintf(graphic2d->temp_string, "%d", graphic2d->body_index);
		Graphic2DRenderBitmapString(graphic2d->width - strlen(graphic2d->temp_string) * 8, 13, graphic2d->temp_string);
		Graphic2DRestorePerspectiveProjection();
	}
	if (graphic2d->stick_body_index_entered)
	{
		Graphic2DSetOrthographicProjection();
		glColor4f(0, 0, 1, 1);
		sprintf(graphic2d->temp_string, "%d", graphic2d->stick_body_index);
		Graphic2DRenderBitmapString(graphic2d->width - strlen(graphic2d->temp_string) * 8, 2 * 13, graphic2d->temp_string);
		Graphic2DRestorePerspectiveProjection();
	}
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
	int curY = 0;
	if (graphic2d->shared->pause) real_time = graphic2d->shared->pause_start_time - graphic2d->shared->start_time;
	else real_time = get_current_time_usec() - graphic2d->shared->start_time;
	Graphic2DSetOrthographicProjection();

	glColor4f(1, 1, 1, 1);

	StringController::getStringf(0x003F, graphic2d->temp_string, graphic2d->shared->frames_per_second);
	Graphic2DRenderBitmapString(0, curY += 13, graphic2d->temp_string);
	
	StringController::getStringf(0x0040, graphic2d->temp_string, graphic2d->shared->calculations_per_second);
	Graphic2DRenderBitmapString(0, curY += 13, graphic2d->temp_string);
	
	StringController::getStringf(0x0041, graphic2d->temp_string, (double)graphic2d->shared->calculations);
	Graphic2DRenderBitmapString(0, curY += 13, graphic2d->temp_string);
	
	StringController::getStringf(0x0042, graphic2d->temp_string, graphic2d->data->dt);
	Graphic2DRenderBitmapString(0, curY += 13, graphic2d->temp_string);
	
	StringController::getStringf(0x0043, graphic2d->temp_string, (double)graphic2d->data->dt*(double)graphic2d->shared->calculations);
	Graphic2DRenderBitmapString(0, curY += 13, graphic2d->temp_string);
	
	StringController::getStringf(0x0044, graphic2d->temp_string, (double)graphic2d->data->dt*(double)graphic2d->shared->calculations / (double)(31557600));
	Graphic2DRenderBitmapString(0, curY += 13, graphic2d->temp_string);
	  
	StringController::getStringf(0x0045, graphic2d->temp_string, (double)real_time / (double)1000000);
	Graphic2DRenderBitmapString(0, curY += 13, graphic2d->temp_string);
	
	StringController::getStringf(0x0046, graphic2d->temp_string, ((double)graphic2d->data->dt*(double)graphic2d->shared->calculations) / ((double)real_time / (double)1000000));
	Graphic2DRenderBitmapString(0, curY += 13, graphic2d->temp_string);
	
	StringController::getStringf(0x0047, graphic2d->temp_string, graphic2d->data->g);
	Graphic2DRenderBitmapString(0, curY += 13, graphic2d->temp_string);
	
	StringController::getStringf(0x0048, graphic2d->temp_string, graphic2d->data->num_of_bodies);
	Graphic2DRenderBitmapString(0, curY += 13, graphic2d->temp_string);
	
	StringController::getStringf(0x0049, graphic2d->temp_string, (graphic2d->data->two_dimensional_calculation ? "2D": "3D"), graphic2d->data->algorithm_name);
	Graphic2DRenderBitmapString(0, curY += 13, graphic2d->temp_string);

	if (graphic2d->shared->calculated_energy)
		StringController::getStringf(0x004A, graphic2d->temp_string, graphic2d->shared->error_energy);
	else
		StringController::getStringf(0x004B, graphic2d->temp_string);
	Graphic2DRenderBitmapString(0, curY += 13, graphic2d->temp_string);

	if (graphic2d->shared->calculated_momentum)
		StringController::getStringf(0x004C, graphic2d->temp_string, graphic2d->shared->error_momentum);
	else
		StringController::getStringf(0x004D, graphic2d->temp_string);
	Graphic2DRenderBitmapString(0, curY += 13, graphic2d->temp_string);

	curY = graphic2d->height + 13 - 3;
	
	StringController::getStringf(0x006C, graphic2d->temp_string, graphic2d->camera_positionZ);
	Graphic2DRenderBitmapString(0, curY -= 13, graphic2d->temp_string);
	
	StringController::getStringf(0x006D, graphic2d->temp_string, graphic2d->camera_positionY);
	Graphic2DRenderBitmapString(0, curY -= 13, graphic2d->temp_string);
	
	StringController::getStringf(0x006E, graphic2d->temp_string, graphic2d->camera_positionX);
	Graphic2DRenderBitmapString(0, curY -= 13, graphic2d->temp_string);
	
	StringController::getStringf(0x006F, graphic2d->temp_string, 4 * graphic2d->height_meters);
	Graphic2DRenderBitmapString(0, curY -= 13, graphic2d->temp_string);

	StringController::getStringf(0x00BE, graphic2d->temp_string, graphic2d->vertices, graphic2d->faces);
	Graphic2DRenderBitmapString(0, curY -= 13, graphic2d->temp_string);

	Graphic2DRestorePerspectiveProjection();
}
void Graphic2DDrawMinText()
{
	if (!graphic2d->min_text) return;

	Graphic2DSetOrthographicProjection();

	glColor4f(1, 1, 1, 1);
	sprintf(graphic2d->temp_string, "fps:%d cps:%d", (int)graphic2d->shared->frames_per_second, (int)graphic2d->shared->calculations_per_second);
	Graphic2DRenderBitmapString(0, 13, graphic2d->temp_string);
	
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
		graphic2d->vertices += 4;
		graphic2d->faces++;
	}
	else
	{
		double rsin, rcos;
		glBegin(GL_TRIANGLE_FAN);
		glVertex2d(radius, 0);
		for (unsigned int i = 1; i < graphic2d->data->sphere_slices; i++)
		{
			rsin = radius * sin(graphic2d->pi_mul_2_div_slices * i);
			rcos = radius * cos(graphic2d->pi_mul_2_div_slices * i);
			glVertex2d(rcos, rsin);
			glVertex2d(rcos, rsin);
		}
		glVertex2d(radius, 0);
		glEnd();
		graphic2d->vertices += 1+(graphic2d->data->sphere_slices<<1);
		graphic2d->faces += graphic2d->data->sphere_slices;
	}
	glPopMatrix();
}

void Graphic2DSetOrthographicProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, graphic2d->width, graphic2d->height, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}
void Graphic2DRestorePerspectiveProjection()
{
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
void Graphic2DRenderBitmapString(int x, int y, char* string)
{
  for (char* c = string; *c != 0; c++) 
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
