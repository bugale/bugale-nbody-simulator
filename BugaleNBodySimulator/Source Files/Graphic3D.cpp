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

Graphic3D* graphic3d;

void NewGraphic3D(Data* data, SharedData* shared)
{
	log_line("Entered NewGraphic3D with data at 0x%08X and shared at 0x%08X.", data, shared);
	if (data->graphic_max_rate == 0) return;

	graphic3d = new Graphic3D();
	graphic3d->data = data;
	graphic3d->shared = shared;

	graphic3d->width = data->width;
	graphic3d->height = data->height;
	graphic3d->ratio = (float)data->width / (float)graphic3d->height;
	graphic3d->title = "Bugale N-Body Simulator 3D Graphical Output Window";
	graphic3d->fullscreen = data->fullscreen;
	graphic3d->fullscreen = data->fullscreen;
	graphic3d->clear_screen = data->clear_screen;
	graphic3d->show_trails = data->show_trails;
	graphic3d->min_text = data->min_text;
	graphic3d->crosshair = data->crosshair;
	graphic3d->wireframe = data->wireframe;
	graphic3d->sin_field_of_view = 2*(float)sin(graphic3d->data->field_of_view*M_PI / (180*2));
	
	graphic3d->camera_positionX = data->camera_positionX;
	graphic3d->camera_positionY = data->camera_positionY;
	graphic3d->camera_positionZ = data->camera_positionZ;
	graphic3d->camera_targetX   = data->camera_targetX;
	graphic3d->camera_targetY   = data->camera_targetY;
	graphic3d->camera_targetZ   = data->camera_targetZ;
	graphic3d->camera_upX       = data->camera_upX;
	graphic3d->camera_upY       = data->camera_upY;
	graphic3d->camera_upZ       = data->camera_upZ;

	graphic3d->cam_target_distance2 = (graphic3d->camera_positionX - graphic3d->camera_targetX)*(graphic3d->camera_positionX - graphic3d->camera_targetX)+
									  (graphic3d->camera_positionY - graphic3d->camera_targetY)*(graphic3d->camera_positionY - graphic3d->camera_targetY)+
									  (graphic3d->camera_positionZ - graphic3d->camera_targetZ)*(graphic3d->camera_positionZ - graphic3d->camera_targetZ);

	graphic3d->frustum[1][0] = graphic3d->frustum[1][1] = graphic3d->frustum[1][2] = graphic3d->frustum[1][3] = 0;
	graphic3d->frustum[2][0] = graphic3d->frustum[2][1] = graphic3d->frustum[2][2] = graphic3d->frustum[2][3] = 0;
	graphic3d->frustum[3][0] = graphic3d->frustum[3][1] = graphic3d->frustum[3][2] = graphic3d->frustum[3][3] = 0;
	graphic3d->frustum[4][0] = graphic3d->frustum[4][1] = graphic3d->frustum[4][2] = graphic3d->frustum[4][3] = 0;
	graphic3d->frustum[5][0] = graphic3d->frustum[5][1] = graphic3d->frustum[5][2] = graphic3d->frustum[5][3] = 0;

	graphic3d->trail_curpos = 0;
	if (graphic3d->data->max_trails != 0)
	{
		graphic3d->trailX = (double*)malloc(sizeof(double)*graphic3d->data->num_of_bodies*graphic3d->data->max_trails);
		graphic3d->trailY = (double*)malloc(sizeof(double)*graphic3d->data->num_of_bodies*graphic3d->data->max_trails);
		graphic3d->trailZ = (double*)malloc(sizeof(double)*graphic3d->data->num_of_bodies*graphic3d->data->max_trails);
		for (int i = 0; i < graphic3d->data->num_of_bodies*graphic3d->data->max_trails; i++) graphic3d->trailX[i] = (DBL_MAX + 1);
		for (int i = 0; i < graphic3d->data->num_of_bodies*graphic3d->data->max_trails; i++) graphic3d->trailY[i] = (DBL_MAX + 1);
		for (int i = 0; i < graphic3d->data->num_of_bodies*graphic3d->data->max_trails; i++) graphic3d->trailZ[i] = (DBL_MAX + 1);
	}

	graphic3d->keyboard_zoom_starttime_in = 0;
	graphic3d->keyboard_zoom_starttime_out = 0;
	graphic3d->keyboard_zoom_duration_in = 0;
	graphic3d->keyboard_zoom_duration_out = 0;
	graphic3d->keyboard_zoom_started_in = false;
	graphic3d->keyboard_zoom_started_out = false;

	graphic3d->mouse_curX = 0;
	graphic3d->mouse_curY = 0;
	graphic3d->mouse_startX = 0;
	graphic3d->mouse_startY = 0;
	graphic3d->mouse_start_value_upX = 0;
	graphic3d->mouse_start_value_upY = 0;
	graphic3d->mouse_start_value_upZ = 0;
	graphic3d->mouse_start_value_forwardX = 0;
	graphic3d->mouse_start_value_forwardY = 0;
	graphic3d->mouse_start_value_forwardZ = 0;
	graphic3d->mouse_pressed = false;
	graphic3d->mouse_left = false;

	graphic3d->body_index = 0;
	graphic3d->body_index_entered = false;
	graphic3d->stick_body_index = 0;
	graphic3d->stick_body_index_entered = false;

	graphic3d->temp_string = (char*)malloc(4096);

	printf("3D Graphical Output Instructions:\n\n");
	printf("           ESC         : Close the Simulator\n");
	printf("     Left  Mouse Key   : Move Your Camera Around the Target\n");
	printf(" Left  Mouse Key and + : Move Your Target Against the Camera\n");
	printf("     Right Mouse Key   : Move Your Target Around the Camera\n");
	printf("          + or -       : Zoom In or Zoom Out\n");
	printf("            r          : Reset Your Camera Position\n");
	printf("            m          : Toggle Minimal Text Mode = Much More Frames Per Second\n");
	printf("            t          : Toggle Trail Showal\n");
	printf("            c          : Toggle Screen Clearance After Every Frame\n");
	printf("            h          : Toggle Crosshair Showal\n");
	printf("            w          : Toggle Wireframe = faster rendering\n");
	printf("            p          : Toggle Pause\n");
	printf("   After a Number Has Been Assigned, Press Enter to Show the Body with the\n");
	printf("   Given Index on the Center of the Screen, or Press Space to Always show the\n");
	printf("   Body with the Given Index on the Center of the Screen and press again to\n");
	printf("   disable it.\n");
	printf("\nThank you for using Bugale N-Body Simulator, and have a pleasant day!\n\n\n\n");

	Graphic3DInitialize();

	glutMainLoop();
	
	log_line("Ended Glut Event Loop.");

	if (graphic3d->temp_string != 0) free(graphic3d->temp_string);
	if (graphic3d->trailX != 0) free(graphic3d->trailX);
	if (graphic3d->trailY != 0) free(graphic3d->trailY);
	if (graphic3d->trailZ != 0) free(graphic3d->trailZ);
	if (graphic3d != 0) free(graphic3d);

	log_line("Ended NewGraphic3D.");
}
void Graphic3DInitialize()
{
	log_line("Entered Graphic3DInitialize.");
	//Initialize GLUT and create the window
	int a = 0; glutInit(&a, 0);
	glutInitDisplayMode   (GLUT_SINGLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowPosition(-1, -1);
	glutInitWindowSize    (graphic3d->width, graphic3d->height);
	graphic3d->singlebuf_window = glutCreateWindow(graphic3d->title);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
	glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);
	glutIgnoreKeyRepeat   (1);
	if (graphic3d->fullscreen) glutFullScreen();

	//Handlers
	glutDisplayFunc   (Graphic3DRenderHandlerSglBfr);
	glutReshapeFunc   (Graphic3DRatioHandler       );
	glutKeyboardFunc  (Graphic3DKeyboardHandler    );
	glutKeyboardUpFunc(Graphic3DKeyboardUpHandler  );
	glutSpecialFunc   (Graphic3DSKeyboardHandler   );
	glutSpecialUpFunc (Graphic3DSKeyboardUpHandler );
	glutMouseFunc     (Graphic3DMouseHandler       );
	glutMotionFunc    (Graphic3DMotionHandler      );
	
	log_line("Graphic3DInitialize - single buffer window set at 0x%08X.", graphic3d->singlebuf_window);

	glutInitDisplayMode   (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	graphic3d->doublebuf_window = glutCreateSubWindow(glutGetWindow(), 0, 0, graphic3d->width, graphic3d->height);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);
	glutIgnoreKeyRepeat   (1);
	if (graphic3d->fullscreen) glutFullScreen();

	//Handlers
	glutDisplayFunc   (Graphic3DRenderHandlerDblBfr);
	glutReshapeFunc   (Graphic3DRatioHandler       );
	glutKeyboardFunc  (Graphic3DKeyboardHandler    );
	glutKeyboardUpFunc(Graphic3DKeyboardUpHandler  );
	glutSpecialFunc   (Graphic3DSKeyboardHandler   );
	glutSpecialUpFunc (Graphic3DSKeyboardUpHandler );
	glutMouseFunc     (Graphic3DMouseHandler       );
	glutMotionFunc    (Graphic3DMotionHandler      );
	
	log_line("Graphic3DInitialize - double buffer window set at 0x%08X.", graphic3d->doublebuf_window);

	log_line("Ended Graphic3DInitialize.");
}

void Graphic3DRenderHandlerDblBfr()
{
	if (!graphic3d->clear_screen)
	{
		glutPostRedisplay();
		return;
	}
	Graphic3DProcessCameraMove();
	Graphic3DClearScreen();
	Graphic3DSetCamera();
	Graphic3DCalculateTemp();

	Graphic3DDrawTrails();
	Graphic3DDrawBodies();
	Graphic3DDrawBodyIndex();
	Graphic3DDrawCrosshair(true);
	Graphic3DDrawMinText();
	Graphic3DDrawText();
	Graphic3DSaveTrails();

	Graphic3DFinalizeFrame();
}
void Graphic3DRenderHandlerSglBfr()
{
	if (graphic3d->clear_screen)
	{
		glutPostRedisplay();
		return;
	}
	Graphic3DProcessCameraMove();
	Graphic3DClearScreen();
	Graphic3DSetCamera();
	Graphic3DCalculateTemp();

	Graphic3DDrawTrails();
	Graphic3DDrawBodies();
	Graphic3DDrawBodyIndex();
	Graphic3DDrawCrosshair(true);
	Graphic3DDrawMinText();
	Graphic3DDrawText();
	Graphic3DSaveTrails();

	Graphic3DFinalizeFrame();
}
void Graphic3DRatioHandler(int width, int height)
{
	log_line("Entered Graphic3DRatioHandler with width as %d and height as %d.", width, height);
	if (height == 0) height++; //Prevent a division by zero.

	graphic3d->width = width;
	graphic3d->height = height;
	graphic3d->ratio = (float)width / (float)height;

	//Recreate the Projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluPerspective(graphic3d->data->field_of_view / 2, graphic3d->ratio, graphic3d->data->near_plane_distance, graphic3d->data->far_plane_distance);

	//Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
	log_line("Ended Graphic3DRatioHandler.");
}
void Graphic3DKeyboardHandler(unsigned char key, int x, int y)
{
	Graphic3DClearScreen();
	if (key ==  27) Graphic3DExit(); //ESC Button
	if (key == 'r') Graphic3DReset();
	if (key == 'h') Graphic3DToggleCrosshair();
	if (key == 'm') Graphic3DToggleMinText();
	if (key == 't') Graphic3DToggleShowTrails();
	if (key == 'c') Graphic3DToggleClearScreen();
	if (key == 'p') Graphic3DTogglePause();
	if (key == 'w') Graphic3DToggleWireframe();
	if (key == '+') Graphic3DToggleCameraMove(0x02, true);
	if (key == '-') Graphic3DToggleCameraMove(0x12, true);

	if (key == '\r') Graphic3DShowBodyIndex();
	if (key == ' ') Graphic3DToggleStickToBody();
	if (key > 47 && key < 58) Graphic3DAddDigitToBodyIndex((int)(key - 48)); //Digit
}
void Graphic3DKeyboardUpHandler(unsigned char key, int x, int y)
{
	if (key == '+') Graphic3DToggleCameraMove(0x02, false);
	if (key == '-') Graphic3DToggleCameraMove(0x12, false);
}
void Graphic3DSKeyboardHandler(int key, int x, int y)
{
	Graphic3DClearScreen();
}
void Graphic3DSKeyboardUpHandler(int key, int x, int y)
{
}
void Graphic3DMouseHandler(int button, int state, int x, int y)
{
	if (state == GLUT_UP)
	{
		if (graphic3d->mouse_left && button == GLUT_LEFT_BUTTON) graphic3d->keyboard_zoom_duration_in = graphic3d->keyboard_zoom_duration_out = 0;
		if (!graphic3d->mouse_left && button == GLUT_RIGHT_BUTTON) graphic3d->keyboard_zoom_duration_in = graphic3d->keyboard_zoom_duration_out = 0;
		graphic3d->mouse_pressed = false;
	}
	else if (button == GLUT_LEFT_BUTTON && !graphic3d->mouse_pressed)
	{
		graphic3d->mouse_left = true;
		graphic3d->mouse_startX = x;
		graphic3d->mouse_startY = y;
		graphic3d->mouse_curX = x;
		graphic3d->mouse_curY = y;
		graphic3d->mouse_start_value_forwardX = graphic3d->camera_targetX - graphic3d->camera_positionX;
		graphic3d->mouse_start_value_forwardY = graphic3d->camera_targetY - graphic3d->camera_positionY;
		graphic3d->mouse_start_value_forwardZ = graphic3d->camera_targetZ - graphic3d->camera_positionZ;
		graphic3d->mouse_start_value_upX      = graphic3d->camera_upX;
		graphic3d->mouse_start_value_upY      = graphic3d->camera_upY;
		graphic3d->mouse_start_value_upZ      = graphic3d->camera_upZ;
		graphic3d->mouse_pressed = true;
	}
	else if (button == GLUT_RIGHT_BUTTON && !graphic3d->mouse_pressed)
	{
		graphic3d->mouse_left = false;
		graphic3d->mouse_startX = x;
		graphic3d->mouse_startY = y;
		graphic3d->mouse_curX = x;
		graphic3d->mouse_curY = y;
		graphic3d->mouse_start_value_forwardX = graphic3d->camera_targetX - graphic3d->camera_positionX;
		graphic3d->mouse_start_value_forwardY = graphic3d->camera_targetY - graphic3d->camera_positionY;
		graphic3d->mouse_start_value_forwardZ = graphic3d->camera_targetZ - graphic3d->camera_positionZ;
		graphic3d->mouse_start_value_upX      = graphic3d->camera_upX;
		graphic3d->mouse_start_value_upY      = graphic3d->camera_upY;
		graphic3d->mouse_start_value_upZ      = graphic3d->camera_upZ;
		graphic3d->mouse_pressed = true;
	}
}
void Graphic3DMotionHandler(int x, int y)
{
	graphic3d->mouse_curX = x;
	graphic3d->mouse_curY = y;
}

void Graphic3DClearScreen()
{
	if (graphic3d->clear_screen) glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	else
	{
		Graphic3DSetOrthographicProjection();
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glRasterPos2i(0, 0);
		glBegin(GL_QUADS);
		if (graphic3d->body_index_entered)
		{
			glVertex3i(graphic3d->width - 9*8, 0, 0);
			glVertex3i(graphic3d->width - 9*8, 2*13, 0);
			glVertex3i(graphic3d->width, 2*13, 0);
			glVertex3i(graphic3d->width, 0, 0);
		}
		if (graphic3d->stick_body_index_entered)
		{
			glVertex3i(graphic3d->width - 9*8, 2*13, 0);
			glVertex3i(graphic3d->width - 9*8, 3*13, 0);
			glVertex3i(graphic3d->width, 3*13, 0);
			glVertex3i(graphic3d->width, 2*13, 0);
		}
		if (!graphic3d->min_text)
		{
			glVertex3i(0, 0, 0);
			glVertex3i(0, 14*13, 0);
			glVertex3i(42*8, 14*13, 0);
			glVertex3i(42*8, 0, 0);

			glVertex3i(0, graphic3d->height - 13*5, 0);
			glVertex3i(0, graphic3d->height, 0);
			glVertex3i(63*8, graphic3d->height, 0);
			glVertex3i(63*8, graphic3d->height - 13*5, 0);
		}
		if (graphic3d->min_text)
		{
			glVertex3i(0, 0, 0);
			glVertex3i(0, 2 * 13, 0);
			glVertex3i(30 * 8, 2 * 13, 0);
			glVertex3i(30 * 8, 0, 0);
		}
		glEnd();
		Graphic3DRestorePerspectiveProjection();
	}
}
void Graphic3DSetCamera()
{
	float light_pos[4] = {(float)graphic3d->camera_positionX, (float)graphic3d->camera_positionY, (float)graphic3d->camera_positionZ, 0.0f};
	glLoadIdentity();
	gluLookAt(graphic3d->camera_positionX, graphic3d->camera_positionY, graphic3d->camera_positionZ,
			  graphic3d->camera_targetX,   graphic3d->camera_targetY,   graphic3d->camera_targetZ,
			  graphic3d->camera_upX,       graphic3d->camera_upY,       graphic3d->camera_upZ);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
}
void Graphic3DFinalizeFrame()
{
	graphic3d->shared->frames++;
	if (graphic3d->data->graphic_max_rate > 0) usleep((long long)(1000000 / graphic3d->data->graphic_max_rate));
	if (graphic3d->clear_screen) glutSwapBuffers();
	glFlush();
	glutPostRedisplay();
}

void Graphic3DExit()
{
	graphic3d->shared->exit = true;
	glutLeaveMainLoop();
}
void Graphic3DReset()
{
	graphic3d->stick_body_index_entered = false;
	graphic3d->body_index_entered = false;
	graphic3d->body_index = 0;
	graphic3d->camera_positionX = graphic3d->data->camera_positionX;
	graphic3d->camera_positionY = graphic3d->data->camera_positionY;
	graphic3d->camera_positionZ = graphic3d->data->camera_positionZ;
	graphic3d->camera_targetX   = graphic3d->data->camera_targetX;
	graphic3d->camera_targetY   = graphic3d->data->camera_targetY;
	graphic3d->camera_targetZ   = graphic3d->data->camera_targetZ;
	graphic3d->camera_upX       = graphic3d->data->camera_upX;
	graphic3d->camera_upY       = graphic3d->data->camera_upY;
	graphic3d->camera_upZ       = graphic3d->data->camera_upZ;
}

void Graphic3DToggleCrosshair()
{
	Graphic3DDrawCrosshair(false);
	graphic3d->crosshair = !graphic3d->crosshair;
}
void Graphic3DToggleMinText()
{
	graphic3d->min_text = !graphic3d->min_text;
}
void Graphic3DToggleShowTrails()
{
	graphic3d->show_trails = !graphic3d->show_trails;
}
void Graphic3DToggleClearScreen()
{
	graphic3d->clear_screen = !graphic3d->clear_screen;
	if (graphic3d->clear_screen)
	{
		glutSetWindow(graphic3d->doublebuf_window);
		glutShowWindow();
	}
	else
	{
		glutSetWindow(graphic3d->singlebuf_window);
		glClear(GL_COLOR_BUFFER_BIT);
		glutSetWindow(graphic3d->doublebuf_window);
		glutHideWindow();
		glutSetWindow(graphic3d->singlebuf_window);
	}
}
void Graphic3DToggleCameraMove(char direction, bool pressed)
{
	if ((direction & 0x0F) == 0x02) //Zoom
	{
		if ((direction & 0xF0) == 0x00 && graphic3d->keyboard_zoom_started_out) return;
		if ((direction & 0xF0) == 0x10 && graphic3d->keyboard_zoom_started_in) return;
		if ((direction & 0xF0) == 0x00) //Zoom In
		{
			graphic3d->keyboard_zoom_started_in = pressed;
			if (!pressed) graphic3d->keyboard_zoom_duration_in += get_current_time_usec() - graphic3d->keyboard_zoom_starttime_in;
			graphic3d->keyboard_zoom_starttime_in = get_current_time_usec();
		}
		else //Zoom Out
		{
			graphic3d->keyboard_zoom_started_out = pressed;
			if (!pressed) graphic3d->keyboard_zoom_duration_out += get_current_time_usec() - graphic3d->keyboard_zoom_starttime_out;
			graphic3d->keyboard_zoom_starttime_out = get_current_time_usec();
		}	
		if (!graphic3d->mouse_pressed && pressed)
		{
			graphic3d->mouse_start_value_forwardX = graphic3d->camera_targetX - graphic3d->camera_positionX;
			graphic3d->mouse_start_value_forwardY = graphic3d->camera_targetY - graphic3d->camera_positionY;
			graphic3d->mouse_start_value_forwardZ = graphic3d->camera_targetZ - graphic3d->camera_positionZ;
			graphic3d->mouse_start_value_upX      = graphic3d->camera_upX;
			graphic3d->mouse_start_value_upY      = graphic3d->camera_upY;
			graphic3d->mouse_start_value_upZ      = graphic3d->camera_upZ;
		}
		if (!graphic3d->mouse_pressed && !pressed)
			graphic3d->keyboard_zoom_duration_in = graphic3d->keyboard_zoom_duration_out = 0;
	}
}
void Graphic3DTogglePause()
{
	if (graphic3d->shared->reached_max_calculations) return;
	if (graphic3d->shared->pause) graphic3d->shared->start_time += get_current_time_usec() - graphic3d->shared->pause_start_time;
	else graphic3d->shared->pause_start_time = get_current_time_usec();
	graphic3d->shared->pause = !graphic3d->shared->pause;
}
void Graphic3DToggleWireframe()
{
	graphic3d->wireframe = !graphic3d->wireframe;
}
void Graphic3DToggleStickToBody()
{
	if (!graphic3d->body_index_entered || graphic3d->body_index >= graphic3d->data->num_of_bodies)
	{
		graphic3d->stick_body_index_entered = false;
	}
	else
	{
		graphic3d->stick_body_index = graphic3d->body_index;
		graphic3d->stick_body_index_entered = true;
	}
	graphic3d->body_index_entered = false;
	graphic3d->body_index = 0;
}
void Graphic3DShowBodyIndex()
{
	if (!graphic3d->body_index_entered) return;
	if (graphic3d->body_index < graphic3d->data->num_of_bodies)
	{
		graphic3d->camera_targetX = graphic3d->data->bodies[graphic3d->body_index]._positionX;
		graphic3d->camera_targetY = graphic3d->data->bodies[graphic3d->body_index]._positionY;
		graphic3d->camera_targetZ = graphic3d->data->bodies[graphic3d->body_index]._positionZ;
	}
	graphic3d->body_index_entered = false;
	graphic3d->body_index = 0;
}
void Graphic3DAddDigitToBodyIndex(int digit)
{
	if (graphic3d->body_index > 99999999) graphic3d->body_index %= 100000000;
	graphic3d->body_index *= 10;
	graphic3d->body_index += digit;
	graphic3d->body_index_entered = true;
}

void Graphic3DProcessCameraMove()
{
	if (!graphic3d->keyboard_zoom_started_in) graphic3d->keyboard_zoom_starttime_in = get_current_time_usec();
	if (!graphic3d->keyboard_zoom_started_out) graphic3d->keyboard_zoom_starttime_out = get_current_time_usec();

	float zoom_duration_in = (float)(graphic3d->keyboard_zoom_duration_in + get_current_time_usec() - graphic3d->keyboard_zoom_starttime_in) / 1000000;
	float zoom_duration_out = (float)(graphic3d->keyboard_zoom_duration_out + get_current_time_usec() - graphic3d->keyboard_zoom_starttime_out) / 1000000;
	if (graphic3d->mouse_pressed && !graphic3d->mouse_left) //Move camera around target
		Graphic3DProcessCameraMove(((float)(graphic3d->mouse_curX - graphic3d->mouse_startX) / graphic3d->width)*(float)M_PI,
								   ((float)(graphic3d->mouse_curY - graphic3d->mouse_startY) / graphic3d->height)*(float)M_PI,
								   zoom_duration_in, zoom_duration_out);
	if (graphic3d->stick_body_index_entered) //Stick to Body
	{
		graphic3d->camera_targetX = graphic3d->data->bodies[graphic3d->stick_body_index]._positionX;
		graphic3d->camera_targetY = graphic3d->data->bodies[graphic3d->stick_body_index]._positionY;
		graphic3d->camera_targetZ = graphic3d->data->bodies[graphic3d->stick_body_index]._positionZ;
	}
	else if (graphic3d->mouse_pressed && graphic3d->mouse_left) //Move target around camera
		Graphic3DProcessTargetMove(((float)(graphic3d->mouse_curX - graphic3d->mouse_startX) / graphic3d->width)*(float)M_PI_2,
								   ((float)(graphic3d->mouse_curY - graphic3d->mouse_startY) / graphic3d->height)*(float)M_PI_2,
								   zoom_duration_in, zoom_duration_out);
	if (!graphic3d->mouse_pressed && (graphic3d->keyboard_zoom_started_in || graphic3d->keyboard_zoom_started_out))
		Graphic3DProcessCameraMove(0, 0, zoom_duration_in, zoom_duration_out);
	Graphic3DFixIndefinedValues();
}
void Graphic3DCalculateTemp()
{
	graphic3d->cam_target_distance2 = (graphic3d->camera_positionX - graphic3d->camera_targetX)*(graphic3d->camera_positionX - graphic3d->camera_targetX)+
									  (graphic3d->camera_positionY - graphic3d->camera_targetY)*(graphic3d->camera_positionY - graphic3d->camera_targetY)+
									  (graphic3d->camera_positionZ - graphic3d->camera_targetZ)*(graphic3d->camera_positionZ - graphic3d->camera_targetZ);

	//Normalize Up Vector
	float t = 1/sqrt(graphic3d->camera_upX*graphic3d->camera_upX + graphic3d->camera_upY*graphic3d->camera_upY + graphic3d->camera_upZ*graphic3d->camera_upZ);
	graphic3d->camera_upX *= t;
	graphic3d->camera_upY *= t;
	graphic3d->camera_upZ *= t;

	float   proj[16];
	float   modl[16];
	float   clip[16];

	/* Get the current PROJECTION matrix from OpenGL */
	glGetFloatv( GL_PROJECTION_MATRIX, proj );

	/* Get the current MODELVIEW matrix from OpenGL */
	glGetFloatv( GL_MODELVIEW_MATRIX, modl );

	/* Combine the two matrices (multiply projection by modelview) */
	clip[ 0] = modl[ 0]*proj[ 0] + modl[ 1]*proj[ 4] + modl[ 2]*proj[ 8] + modl[ 3]*proj[12];
	clip[ 1] = modl[ 0]*proj[ 1] + modl[ 1]*proj[ 5] + modl[ 2]*proj[ 9] + modl[ 3]*proj[13];
	clip[ 2] = modl[ 0]*proj[ 2] + modl[ 1]*proj[ 6] + modl[ 2]*proj[10] + modl[ 3]*proj[14];
	clip[ 3] = modl[ 0]*proj[ 3] + modl[ 1]*proj[ 7] + modl[ 2]*proj[11] + modl[ 3]*proj[15];

	clip[ 4] = modl[ 4]*proj[ 0] + modl[ 5]*proj[ 4] + modl[ 6]*proj[ 8] + modl[ 7]*proj[12];
	clip[ 5] = modl[ 4]*proj[ 1] + modl[ 5]*proj[ 5] + modl[ 6]*proj[ 9] + modl[ 7]*proj[13];
	clip[ 6] = modl[ 4]*proj[ 2] + modl[ 5]*proj[ 6] + modl[ 6]*proj[10] + modl[ 7]*proj[14];
	clip[ 7] = modl[ 4]*proj[ 3] + modl[ 5]*proj[ 7] + modl[ 6]*proj[11] + modl[ 7]*proj[15];

	clip[ 8] = modl[ 8]*proj[ 0] + modl[ 9]*proj[ 4] + modl[10]*proj[ 8] + modl[11]*proj[12];
	clip[ 9] = modl[ 8]*proj[ 1] + modl[ 9]*proj[ 5] + modl[10]*proj[ 9] + modl[11]*proj[13];
	clip[10] = modl[ 8]*proj[ 2] + modl[ 9]*proj[ 6] + modl[10]*proj[10] + modl[11]*proj[14];
	clip[11] = modl[ 8]*proj[ 3] + modl[ 9]*proj[ 7] + modl[10]*proj[11] + modl[11]*proj[15];

	clip[12] = modl[12]*proj[ 0] + modl[13]*proj[ 4] + modl[14]*proj[ 8] + modl[15]*proj[12];
	clip[13] = modl[12]*proj[ 1] + modl[13]*proj[ 5] + modl[14]*proj[ 9] + modl[15]*proj[13];
	clip[14] = modl[12]*proj[ 2] + modl[13]*proj[ 6] + modl[14]*proj[10] + modl[15]*proj[14];
	clip[15] = modl[12]*proj[ 3] + modl[13]*proj[ 7] + modl[14]*proj[11] + modl[15]*proj[15];

	/* Extract the numbers for the RIGHT plane */
	graphic3d->frustum[0][0] = clip[ 3] - clip[ 0];
	graphic3d->frustum[0][1] = clip[ 7] - clip[ 4];
	graphic3d->frustum[0][2] = clip[11] - clip[ 8];
	graphic3d->frustum[0][3] = clip[15] - clip[12];

	/* Normalize the result */
	t = 1/sqrt(graphic3d->frustum[0][0]*graphic3d->frustum[0][0] + graphic3d->frustum[0][1]*graphic3d->frustum[0][1] + graphic3d->frustum[0][2]*graphic3d->frustum[0][2]);
	graphic3d->frustum[0][0] *= t;
	graphic3d->frustum[0][1] *= t;
	graphic3d->frustum[0][2] *= t;
	graphic3d->frustum[0][3] *= t;

	/* Extract the numbers for the LEFT plane */
	graphic3d->frustum[1][0] = clip[ 3] + clip[ 0];
	graphic3d->frustum[1][1] = clip[ 7] + clip[ 4];
	graphic3d->frustum[1][2] = clip[11] + clip[ 8];
	graphic3d->frustum[1][3] = clip[15] + clip[12];

	/* Normalize the result */
	t = 1/sqrt(graphic3d->frustum[1][0]*graphic3d->frustum[1][0] + graphic3d->frustum[1][1]*graphic3d->frustum[1][1] + graphic3d->frustum[1][2]*graphic3d->frustum[1][2]);
	graphic3d->frustum[1][0] *= t;
	graphic3d->frustum[1][1] *= t;
	graphic3d->frustum[1][2] *= t;
	graphic3d->frustum[1][3] *= t;

	/* Extract the BOTTOM plane */
	graphic3d->frustum[2][0] = clip[ 3] + clip[ 1];
	graphic3d->frustum[2][1] = clip[ 7] + clip[ 5];
	graphic3d->frustum[2][2] = clip[11] + clip[ 9];
	graphic3d->frustum[2][3] = clip[15] + clip[13];

	/* Normalize the result */
	t = 1/sqrt(graphic3d->frustum[2][0]*graphic3d->frustum[2][0] + graphic3d->frustum[2][1]*graphic3d->frustum[2][1] + graphic3d->frustum[2][2]*graphic3d->frustum[2][2]);
	graphic3d->frustum[2][0] *= t;
	graphic3d->frustum[2][1] *= t;
	graphic3d->frustum[2][2] *= t;
	graphic3d->frustum[2][3] *= t;

	/* Extract the TOP plane */
	graphic3d->frustum[3][0] = clip[ 3] - clip[ 1];
	graphic3d->frustum[3][1] = clip[ 7] - clip[ 5];
	graphic3d->frustum[3][2] = clip[11] - clip[ 9];
	graphic3d->frustum[3][3] = clip[15] - clip[13];

	/* Normalize the result */
	t = 1/sqrt(graphic3d->frustum[3][0]*graphic3d->frustum[3][0] + graphic3d->frustum[3][1]*graphic3d->frustum[3][1] + graphic3d->frustum[3][2]*graphic3d->frustum[3][2]);
	graphic3d->frustum[3][0] *= t;
	graphic3d->frustum[3][1] *= t;
	graphic3d->frustum[3][2] *= t;
	graphic3d->frustum[3][3] *= t;

	/* Extract the FAR plane */
	graphic3d->frustum[4][0] = clip[ 3] - clip[ 2];
	graphic3d->frustum[4][1] = clip[ 7] - clip[ 6];
	graphic3d->frustum[4][2] = clip[11] - clip[10];
	graphic3d->frustum[4][3] = clip[15] - clip[14];

	/* Normalize the result */
	t = 1/sqrt(graphic3d->frustum[4][0]*graphic3d->frustum[4][0] + graphic3d->frustum[4][1]*graphic3d->frustum[4][1] + graphic3d->frustum[4][2]*graphic3d->frustum[4][2]);
	graphic3d->frustum[4][0] *= t;
	graphic3d->frustum[4][1] *= t;
	graphic3d->frustum[4][2] *= t;
	graphic3d->frustum[4][3] *= t;

	/* Extract the NEAR plane */
	graphic3d->frustum[5][0] = clip[ 3] + clip[ 2];
	graphic3d->frustum[5][1] = clip[ 7] + clip[ 6];
	graphic3d->frustum[5][2] = clip[11] + clip[10];
	graphic3d->frustum[5][3] = clip[15] + clip[14];

	/* Normalize the result */
	t = 1/sqrt(graphic3d->frustum[5][0]*graphic3d->frustum[5][0] + graphic3d->frustum[5][1]*graphic3d->frustum[5][1] + graphic3d->frustum[5][2]*graphic3d->frustum[5][2]);
	graphic3d->frustum[5][0] *= t;
	graphic3d->frustum[5][1] *= t;
	graphic3d->frustum[5][2] *= t;
	graphic3d->frustum[5][3] *= t;
}
void Graphic3DDrawBodies()
{
	for (int i = 0; i < graphic3d->data->num_of_bodies; i++)
	{
		double x = graphic3d->data->bodies[i]._positionX;
		double y = graphic3d->data->bodies[i]._positionY;
		double z = graphic3d->data->bodies[i]._positionZ;
		double r = graphic3d->data->bodies[i]._radius;
		if (!Graphic3DIsInSight(x, y, z, r)) continue;
		float R = (float)graphic3d->data->bodies[i]._colorR*0.003921568f;
		float G = (float)graphic3d->data->bodies[i]._colorG*0.003921568f;
		float B = (float)graphic3d->data->bodies[i]._colorB*0.003921568f;
		float A = (float)graphic3d->data->bodies[i]._colorA*0.003921568f;
		Graphic3DDrawBody(x, y, z, r, R, G, B, A, false);
	}
}
void Graphic3DDrawBodyIndex()
{
	if (graphic3d->body_index_entered)
	{
		Graphic3DSetOrthographicProjection();
		glColor4f(1, 0, 0, 1);
		sprintf(graphic3d->temp_string, "%d", graphic3d->body_index);
		Graphic3DRenderBitmapString(graphic3d->width - strlen(graphic3d->temp_string)*8, 13, graphic3d->temp_string);
		Graphic3DRestorePerspectiveProjection();
	}
	if (graphic3d->stick_body_index_entered)
	{
		Graphic3DSetOrthographicProjection();
		glColor4f(0, 0, 1, 1);
		sprintf(graphic3d->temp_string, "%d", graphic3d->stick_body_index);
		Graphic3DRenderBitmapString(graphic3d->width - strlen(graphic3d->temp_string)*8, 2*13, graphic3d->temp_string);
		Graphic3DRestorePerspectiveProjection();
	}
}
void Graphic3DDrawCrosshair(bool white)
{
	if (!graphic3d->crosshair) return;

	Graphic3DSetOrthographicProjection();
	glColor4f(white, white, white, 1);
	float length = graphic3d->height*0.01f;
	glBegin(GL_LINES);
		glVertex3f(graphic3d->width*0.5f - length*0.5f, graphic3d->height*0.5f, 0);
		glVertex3f(graphic3d->width*0.5f + length*0.5f, graphic3d->height*0.5f, 0);
		glVertex3f(graphic3d->width*0.5f, graphic3d->height*0.5f - length*0.5f, 0);
		glVertex3f(graphic3d->width*0.5f, graphic3d->height*0.5f + length*0.5f, 0);
	glEnd();
	Graphic3DRestorePerspectiveProjection();
}
void Graphic3DDrawText()
{
	if (graphic3d->min_text) return;

	long long real_time;
	int curY = 0;
	if (graphic3d->shared->pause) real_time = graphic3d->shared->pause_start_time - graphic3d->shared->start_time;
	else real_time = get_current_time_usec() - graphic3d->shared->start_time;
	Graphic3DSetOrthographicProjection();

	double forwardX, forwardY, forwardZ, forwardL;
	double upX,      upY,      upZ,      upL     ;
	double leftX,    leftY,    leftZ,    leftL   ;
	upX       = graphic3d->camera_upX;
	upY       = graphic3d->camera_upY;
	upZ       = graphic3d->camera_upZ;
	forwardX  = graphic3d->camera_targetX - graphic3d->camera_positionX;
	forwardY  = graphic3d->camera_targetY - graphic3d->camera_positionY;
	forwardZ  = graphic3d->camera_targetZ - graphic3d->camera_positionZ;
	leftX     = forwardY*upZ - upY*forwardZ;
	leftY     = forwardZ*upX - upZ*forwardX;
	leftZ     = forwardX*upY - upX*forwardY;
	upL       = 1/sqrt(upX*upX + upY*upY + upZ*upZ);
	forwardL  = 1/sqrt(forwardX*forwardX + forwardY*forwardY + forwardZ*forwardZ);
	leftL     = 1/sqrt(leftX*leftX + leftY*leftY + leftZ*leftZ);
	upX *= upL; upY *= upL; upZ *= upL;
	forwardX *= forwardL; forwardY *= forwardL; forwardZ *= forwardL;
	leftX *= leftL; leftY *= leftL; leftZ *= leftL;

	glColor4f(1, 1, 1, 1);

	sprintf(graphic3d->temp_string, "Frames Per Second: %G", graphic3d->shared->frames_per_second);
	Graphic3DRenderBitmapString(0, curY += 13, graphic3d->temp_string);

	sprintf(graphic3d->temp_string, "Calculations Per Second:   %+E", graphic3d->shared->calculations_per_second);
	Graphic3DRenderBitmapString(0, curY += 13, graphic3d->temp_string);

	sprintf(graphic3d->temp_string, "Total Calculations Done:   %+E", (double)graphic3d->shared->calculations);
	Graphic3DRenderBitmapString(0, curY += 13, graphic3d->temp_string);

	sprintf(graphic3d->temp_string, "DeltaTime:                 %+E", graphic3d->data->dt);
	Graphic3DRenderBitmapString(0, curY += 13, graphic3d->temp_string);

	sprintf(graphic3d->temp_string, "Simulated Seconds Past:    %+E", (double)graphic3d->data->dt*(double)graphic3d->shared->calculations);
	Graphic3DRenderBitmapString(0, curY += 13, graphic3d->temp_string);

	sprintf(graphic3d->temp_string, "Simulated Years Past:      %+E", (double)graphic3d->data->dt*(double)graphic3d->shared->calculations / (double)(31557600));
	Graphic3DRenderBitmapString(0, curY += 13, graphic3d->temp_string);

	sprintf(graphic3d->temp_string, "Real Seconds Past:         %+E", (double)real_time / (double)1000000);
	Graphic3DRenderBitmapString(0, curY += 13, graphic3d->temp_string);

	sprintf(graphic3d->temp_string, "Simulated-Real Time Ratio: %+E", ((double)graphic3d->data->dt*(double)graphic3d->shared->calculations) / ((double)real_time / (double)1000000));
	Graphic3DRenderBitmapString(0, curY += 13, graphic3d->temp_string);

	sprintf(graphic3d->temp_string, "Gravitational Constant:    %+E", graphic3d->data->g);
	Graphic3DRenderBitmapString(0, curY += 13, graphic3d->temp_string);
	
	sprintf(graphic3d->temp_string, "Number of Bodies: %d", graphic3d->data->num_of_bodies);
	Graphic3DRenderBitmapString(0, curY += 13, graphic3d->temp_string);

	sprintf(graphic3d->temp_string, "Integration Algorithm: %s %s", (graphic3d->data->two_dimensional_calculation ? "3D": "3D"), graphic3d->data->algorithm_name);
	Graphic3DRenderBitmapString(0, curY += 13, graphic3d->temp_string);

	if (graphic3d->shared->calculated_energy)
		sprintf(graphic3d->temp_string, "Energy Error: %+E", graphic3d->shared->error_energy);
	else
		sprintf(graphic3d->temp_string, "Energy Error: Pause to Calculate...");
	Graphic3DRenderBitmapString(0, curY += 13, graphic3d->temp_string);

	if (graphic3d->shared->calculated_momentum)
		sprintf(graphic3d->temp_string, "Momentum Error: %+E", graphic3d->shared->error_momentum);
	else
		sprintf(graphic3d->temp_string, "Momentum Error: Pause to Calculate...");
	Graphic3DRenderBitmapString(0, curY += 13, graphic3d->temp_string);

	curY = graphic3d->height + 13 - 3;
	
	sprintf(graphic3d->temp_string, "Camera Left:     (%+E,%+E,%+E)", leftX, leftY, leftZ);
	Graphic3DRenderBitmapString(0, curY -= 13, graphic3d->temp_string);

	sprintf(graphic3d->temp_string, "Camera Forward:  (%+E,%+E,%+E)", forwardX, forwardY, forwardZ);
	Graphic3DRenderBitmapString(0, curY -= 13, graphic3d->temp_string);

	sprintf(graphic3d->temp_string, "Camera Up:       (%+E,%+E,%+E)", upX, upY, upZ);
	Graphic3DRenderBitmapString(0, curY -= 13, graphic3d->temp_string);
	
	sprintf(graphic3d->temp_string, "Camera Target:   (%+E,%+E,%+E)", graphic3d->camera_targetX, graphic3d->camera_targetY, graphic3d->camera_targetZ);
	Graphic3DRenderBitmapString(0, curY -= 13, graphic3d->temp_string);

	sprintf(graphic3d->temp_string, "Camera Position: (%+E,%+E,%+E)", graphic3d->camera_positionX, graphic3d->camera_positionY, graphic3d->camera_positionZ);
	Graphic3DRenderBitmapString(0, curY -= 13, graphic3d->temp_string);
	
	Graphic3DRestorePerspectiveProjection();
}
void Graphic3DDrawMinText()
{
	if (!graphic3d->min_text) return;

	Graphic3DSetOrthographicProjection();
	sprintf(graphic3d->temp_string, "fps:%d cps:%d", (int)graphic3d->shared->frames_per_second, (int)graphic3d->shared->calculations_per_second);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	Graphic3DRenderBitmapString(0, 13, graphic3d->temp_string);
	Graphic3DRestorePerspectiveProjection();
}
void Graphic3DDrawTrails()
{
	if (graphic3d->data->max_trails == 0) return;
	if (!graphic3d->show_trails) return;
	for (int trail = graphic3d->trail_curpos; trail < graphic3d->data->max_trails; trail++)
		for (int body = 0; body < graphic3d->data->num_of_bodies; body++)
		{
			double x = graphic3d->trailX[trail*graphic3d->data->num_of_bodies + body];
			double y = graphic3d->trailY[trail*graphic3d->data->num_of_bodies + body];
			double z = graphic3d->trailZ[trail*graphic3d->data->num_of_bodies + body];
			double r = graphic3d->data->bodies[body]._trailwidth;
			if (!Graphic3DIsInSight(x, y, z, r)) continue;
			float R = (float)graphic3d->data->bodies[body]._trailcolorR*0.003921568f;
			float G = (float)graphic3d->data->bodies[body]._trailcolorG*0.003921568f;
			float B = (float)graphic3d->data->bodies[body]._trailcolorB*0.003921568f;
			float A = (float)graphic3d->data->bodies[body]._trailcolorA*0.003921568f;
			Graphic3DDrawBody(x, y, z, r, R, G, B, A, true);
		}
	for (int trail = 0; trail < graphic3d->trail_curpos; trail++)
		for (int body = 0; body < graphic3d->data->num_of_bodies; body++)
		{
			double x = graphic3d->trailX[trail*graphic3d->data->num_of_bodies + body];
			double y = graphic3d->trailY[trail*graphic3d->data->num_of_bodies + body];
			double z = graphic3d->trailZ[trail*graphic3d->data->num_of_bodies + body];
			double r = graphic3d->data->bodies[body]._trailwidth;
			if (!Graphic3DIsInSight(x, y, z, r)) continue;
			float R = (float)graphic3d->data->bodies[body]._trailcolorR*0.003921568f;
			float G = (float)graphic3d->data->bodies[body]._trailcolorG*0.003921568f;
			float B = (float)graphic3d->data->bodies[body]._trailcolorB*0.003921568f;
			float A = (float)graphic3d->data->bodies[body]._trailcolorA*0.003921568f;
			Graphic3DDrawBody(x, y, z, r, R, G, B, A, true);
		}
}
void Graphic3DSaveTrails()
{
	if (graphic3d->data->max_trails == 0) return;
	if (graphic3d->shared->pause) return;
	for (int body = 0; body < graphic3d->data->num_of_bodies; body++)
	{
		graphic3d->trailX[graphic3d->trail_curpos*graphic3d->data->num_of_bodies + body] = graphic3d->data->bodies[body]._positionX;
		graphic3d->trailY[graphic3d->trail_curpos*graphic3d->data->num_of_bodies + body] = graphic3d->data->bodies[body]._positionY;
		graphic3d->trailZ[graphic3d->trail_curpos*graphic3d->data->num_of_bodies + body] = graphic3d->data->bodies[body]._positionZ;
	}
	graphic3d->trail_curpos++;
	if (graphic3d->trail_curpos >= graphic3d->data->max_trails) graphic3d->trail_curpos = 0;
}
void Graphic3DDrawBody(double X, double Y, double Z, double radius, float R, float G, float B, float A, bool trail)
{
	float color[4] = {R, G, B, A};
	glPushMatrix();
	glColor4fv(color);
	glTranslated(X, Y, Z);
	if (graphic3d->wireframe)
		if (trail) glutWireCube(radius);
		else glutWireSphere(radius, graphic3d->data->sphere_slices, graphic3d->data->sphere_stacks);
	else
	{
		glEnable(GL_LIGHTING);
		if (trail) glutSolidCube(radius);
		else glutSolidSphere(radius, graphic3d->data->sphere_slices, graphic3d->data->sphere_stacks);
		glDisable(GL_LIGHTING);
	}
	glPopMatrix();
}
void Graphic3DProcessCameraMove(float angleHorizontal, float angleVertical, float zoom_duration_in, float zoom_duration_out)
{
	float angleRotateX, angleRotateY, angleRotateZ, upAngle, forwardAngle, leftAngle;
	double upX,      upY,      upZ,      upL;
	double forwardX, forwardY, forwardZ, forwardL;
	double leftX,    leftY,    leftZ,    leftL;

	//Set vectors
	upX       = graphic3d->mouse_start_value_upX;
	upY       = graphic3d->mouse_start_value_upY;
	upZ       = graphic3d->mouse_start_value_upZ;
	forwardX  = graphic3d->mouse_start_value_forwardX;
	forwardY  = graphic3d->mouse_start_value_forwardY;
	forwardZ  = graphic3d->mouse_start_value_forwardZ;
	leftX     = forwardY*upZ - upY*forwardZ;
	leftY     = forwardZ*upX - upZ*forwardX;
	leftZ     = forwardX*upY - upX*forwardY;
	double forward_start_length = sqrt(forwardX*forwardX + forwardY*forwardY + forwardZ*forwardZ);

	log_line("Entered Graphic3DProcessCameraMove(1/3) with angleVertical as %G angleHorizontal as %G zoom_duration_in as %G zoom_duration_out as %G forwardX as %G forwardY as %G.", angleVertical, angleHorizontal, zoom_duration_in, zoom_duration_out, forwardX, forwardY);
	log_line("Entered Graphic3DProcessCameraMove(2/3) with forwardZ as %G upX as %G upY as %G upZ as %G leftX as %G.", forwardZ, upX, upY, upZ);
	log_line("Entered Graphic3DProcessCameraMove(3/3) with leftX as %G leftY as %G and leftZ as %G.", leftX, leftY, leftZ);

	//Vertical Rotate
	#pragma region Rotate about Z
		//Normalize vecotrs for XY
		upL       = sqrt(upX     *upX      + upY     *upY     );
		forwardL  = sqrt(forwardX*forwardX + forwardY*forwardY);
		if (upL      != 0) { upX      /= upL;      upY      /= upL;      upZ      /= upL;      }
		if (forwardL != 0) { forwardX /= forwardL; forwardY /= forwardL; forwardZ /= forwardL; }

		//Rotate space about the z axis so that the up vector lies in the xz plane
		upAngle       = getAngle((float)upX,      (float)upY);
		forwardAngle  = getAngle((float)forwardX, (float)forwardY);
		angleRotateZ  = -upAngle;
		upAngle += angleRotateZ; forwardAngle += angleRotateZ;
		upX      = cos(upAngle); forwardX      = cos(forwardAngle);
		upY      = sin(upAngle); forwardY      = sin(forwardAngle);
		if (upL      == 0) upX      = upY      = 0;
		if (forwardL == 0) forwardX = forwardY = 0;
		log_line("Graphic3DProcessCameraMove: Vertical rotate about Z result is upX as %G upY as %G forwardX as %G and forwardY as %G.", upX, upY, forwardX, forwardY);
	#pragma endregion
	#pragma region Rotate about Y
		//Normalize vecotrs for XZ
		upL       = sqrt(upX     *upX      + upZ     *upZ     );
		forwardL  = sqrt(forwardX*forwardX + forwardZ*forwardZ);
		if (upL      != 0) { upX      /= upL;      upY      /= upL;      upZ      /= upL;      }
		if (forwardL != 0) { forwardX /= forwardL; forwardY /= forwardL; forwardZ /= forwardL; }

		//Rotate space about the y axis so that the right vector lies along the x axis
		upAngle       = getAngle((float)upX,      (float)upZ     );
		forwardAngle  = getAngle((float)forwardX, (float)forwardZ);
		angleRotateY  = -upAngle;
		upAngle += angleRotateY; forwardAngle += angleRotateY;
		upX      = cos(upAngle); forwardX      = cos(forwardAngle);
		upZ      = sin(upAngle); forwardZ      = sin(forwardAngle);
		if (upL      == 0) upX      = upZ      = 0;
		if (forwardL == 0) forwardX = forwardZ = 0;
		log_line("Graphic3DProcessCameraMove: Vertical rotate about Y result is upX as %G upZ as %G forwardX as %G and forwardZ as %G.", upX, upZ, forwardX, forwardZ);
	#pragma endregion
	#pragma region Make the actual rotation(about X)
		//Normalize vecotrs for YZ
		upL       = sqrt(upY     *upY      + upZ     *upZ     );
		forwardL  = sqrt(forwardY*forwardY + forwardZ*forwardZ);
		if (upL      != 0) { upX      /= upL;      upY      /= upL;      upZ      /= upL;      }
		if (forwardL != 0) { forwardX /= forwardL; forwardY /= forwardL; forwardZ /= forwardL; }

		//Rotate space about the x axis to get desired rotation
		upAngle       = getAngle((float)upY,      (float)upZ     );
		forwardAngle  = getAngle((float)forwardY, (float)forwardZ);
		angleRotateX  = -angleHorizontal;
		upAngle += angleRotateX; forwardAngle += angleRotateX;
		upY      = cos(upAngle); forwardY      = cos(forwardAngle);
		upZ      = sin(upAngle); forwardZ      = sin(forwardAngle);
		if (upL      == 0) upY      = upZ      = 0;
		if (forwardL == 0) forwardY = forwardZ = 0;
		log_line("Graphic3DProcessCameraMove: Vertical rotate about X result is upY as %G upZ as %G forwardY as %G and forwardZ as %G.", upY, upZ, forwardY, forwardZ);
	#pragma endregion
	#pragma region Rotate about Y back
		//Normalize vecotrs for XZ
		upL       = sqrt(upX     *upX      + upZ     *upZ     );
		forwardL  = sqrt(forwardX*forwardX + forwardZ*forwardZ);
		if (upL      != 0) { upX      /= upL;      upY      /= upL;      upZ      /= upL;      }
		if (forwardL != 0) { forwardX /= forwardL; forwardY /= forwardL; forwardZ /= forwardL; }

		//Rotate space about the y axis back to original
		upAngle       = getAngle((float)upX,      (float)upZ     );
		forwardAngle  = getAngle((float)forwardX, (float)forwardZ);
		angleRotateY  = -angleRotateY;
		upAngle += angleRotateY; forwardAngle += angleRotateY;
		upX      = cos(upAngle); forwardX      = cos(forwardAngle);
		upZ      = sin(upAngle); forwardZ      = sin(forwardAngle);
		if (upL      == 0) upX      = upZ      = 0;
		if (forwardL == 0) forwardX = forwardZ = 0;
		log_line("Graphic3DProcessCameraMove: Vertical rotate back about Y result is upX as %G upZ as %G forwardX as %G and forwardZ as %G.", upX, upZ, forwardX, forwardZ);
	#pragma endregion
	#pragma region Rotate about Z back
		//Normalize vecotrs for XY
		upL       = sqrt(upX     *upX      + upY     *upY     );
		forwardL  = sqrt(forwardX*forwardX + forwardY*forwardY);
		if (upL      != 0) { upX      /= upL;      upY      /= upL;      upZ      /= upL;      }
		if (forwardL != 0) { forwardX /= forwardL; forwardY /= forwardL; forwardZ /= forwardL; }

		//Rotate space about the z axis so that the right vector lies in the xz plane
		upAngle       = getAngle((float)upX,      (float)upY     );
		forwardAngle  = getAngle((float)forwardX, (float)forwardY);
		angleRotateZ  = -angleRotateZ;
		upAngle += angleRotateZ; forwardAngle += angleRotateZ;
		upX      = cos(upAngle); forwardX      = cos(forwardAngle);
		upY      = sin(upAngle); forwardY      = sin(forwardAngle);
		if (upL      == 0) upX      = upY      = 0;
		if (forwardL == 0) forwardX = forwardY = 0;
		log_line("Graphic3DProcessCameraMove: Vertical rotate back about Z result is upX as %G upY as %G forwardX as %G and forwardY as %G.", upX, upY, forwardX, forwardY);
	#pragma endregion

	//Horizontal Rotate
	#pragma region Rotate about Z
		//Normalize vecotrs for XY
		upL       = sqrt(upX     *upX      + upY     *upY     );
		forwardL  = sqrt(forwardX*forwardX + forwardY*forwardY);
		leftL     = sqrt(leftX   *leftX    + leftY   *leftY   );
		if (upL      != 0) { upX      /= upL;      upY      /= upL;      upZ      /= upL;      }
		if (forwardL != 0) { forwardX /= forwardL; forwardY /= forwardL; forwardZ /= forwardL; }
		if (leftL    != 0) { leftX    /= leftL;    leftY    /= leftL;    leftZ    /= leftL;    }

		//Rotate space about the z axis so that the left vector lies in the xz plane
		upAngle       = getAngle((float)upX,      (float)upY     );
		forwardAngle  = getAngle((float)forwardX, (float)forwardY);
		leftAngle     = getAngle((float)leftX,    (float)leftY   );
		angleRotateZ  = -leftAngle;
		upAngle += angleRotateZ; forwardAngle += angleRotateZ; leftAngle += angleRotateZ;
		upX      = cos(upAngle); forwardX      = cos(forwardAngle); leftX = cos(leftAngle);
		upY      = sin(upAngle); forwardY      = sin(forwardAngle); leftY = sin(leftAngle);
		if (upL      == 0) upX      = upY      = 0;
		if (forwardL == 0) forwardX = forwardY = 0;
		if (leftL    == 0) leftX    = leftY    = 0;
		log_line("Graphic3DProcessCameraMove: Horizontal rotate about Z result is upX as %G upY as %G forwardX as %G forwardY as %G leftX as %G and leftY as %G.", upX, upY, forwardX, forwardY, leftX, leftY);
	#pragma endregion
	#pragma region Rotate about Y
		//Normalize vecotrs for XZ
		upL       = sqrt(upX     *upX      + upZ     *upZ     );
		forwardL  = sqrt(forwardX*forwardX + forwardZ*forwardZ);
		leftL     = sqrt(leftX   *leftX    + leftZ   *leftZ   );
		if (upL      != 0) { upX      /= upL;      upY      /= upL;      upZ      /= upL;      }
		if (forwardL != 0) { forwardX /= forwardL; forwardY /= forwardL; forwardZ /= forwardL; }
		if (leftL    != 0) { leftX    /= leftL;    leftY    /= leftL;    leftZ    /= leftL;    }

		//Rotate space about the y axis so that the left vector lies along the x axis
		upAngle       = getAngle((float)upX,      (float)upZ     );
		forwardAngle  = getAngle((float)forwardX, (float)forwardZ);
		leftAngle     = getAngle((float)leftX,    (float)leftZ   );
		angleRotateY  = -leftAngle;
		upAngle += angleRotateY; forwardAngle += angleRotateY; leftAngle += angleRotateY;
		upX      = cos(upAngle); forwardX      = cos(forwardAngle); leftX = cos(leftAngle);
		upZ      = sin(upAngle); forwardZ      = sin(forwardAngle); leftZ = sin(leftAngle);
		if (upL      == 0) upX      = upZ      = 0;
		if (forwardL == 0) forwardX = forwardZ = 0;
		if (leftL    == 0) leftX    = leftZ    = 0;
		log_line("Graphic3DProcessCameraMove: Horizontal rotate about Y result is upX as %G upZ as %G forwardX as %G forwardZ as %G leftX as %G and leftZ as %G.", upX, upZ, forwardX, forwardZ, leftX, leftZ);
	#pragma endregion
	#pragma region Make the actual rotation(about X)
		//Normalize vecotrs for YZ
		upL       = sqrt(upY     *upY      + upZ     *upZ     );
		forwardL  = sqrt(forwardY*forwardY + forwardZ*forwardZ);
		if (upL      != 0) { upX      /= upL;      upY      /= upL;      upZ      /= upL;      }
		if (forwardL != 0) { forwardX /= forwardL; forwardY /= forwardL; forwardZ /= forwardL; }

		//Rotate space about the x axis to get desired rotation
		upAngle       = getAngle((float)upY,      (float)upZ     );
		forwardAngle  = getAngle((float)forwardY, (float)forwardZ);
		angleRotateX  = -angleVertical;
		upAngle += angleRotateX; forwardAngle += angleRotateX;
		upY      = cos(upAngle); forwardY      = cos(forwardAngle);
		upZ      = sin(upAngle); forwardZ      = sin(forwardAngle);
		if (upL      == 0) upY      = upZ      = 0;
		if (forwardL == 0) forwardY = forwardZ = 0;
		log_line("Graphic3DProcessCameraMove: Horizontal rotate about X result is upY as %G upZ as %G forwardY as %G forwardZ as %G.", upY, upZ, forwardY, forwardZ);
	#pragma endregion
	#pragma region Rotate about Y back
		//Normalize vecotrs for XZ
		upL       = sqrt(upX     *upX      + upZ     *upZ     );
		forwardL  = sqrt(forwardX*forwardX + forwardZ*forwardZ);
		if (upL      != 0) { upX      /= upL;      upY      /= upL;      upZ      /= upL;      }
		if (forwardL != 0) { forwardX /= forwardL; forwardY /= forwardL; forwardZ /= forwardL; }

		//Rotate space about the y axis back to original
		upAngle       = getAngle((float)upX,      (float)upZ     );
		forwardAngle  = getAngle((float)forwardX, (float)forwardZ);
		angleRotateY  = -angleRotateY;
		upAngle += angleRotateY; forwardAngle += angleRotateY;
		upX      = cos(upAngle); forwardX      = cos(forwardAngle);
		upZ      = sin(upAngle); forwardZ      = sin(forwardAngle);
		if (upL      == 0) upX      = upZ      = 0;
		if (forwardL == 0) forwardX = forwardZ = 0;
		log_line("Graphic3DProcessCameraMove: Horizontal rotate back about Y result is upX as %G upZ as %G forwardX as %G forwardZ as %G.", upX, upZ, forwardX, forwardZ);
	#pragma endregion
	#pragma region Rotate about Z back
		//Normalize vecotrs for XY
		upL       = sqrt(upX     *upX      + upY     *upY     );
		forwardL  = sqrt(forwardX*forwardX + forwardY*forwardY);
		if (upL      != 0) { upX      /= upL;      upY      /= upL;      upZ      /= upL;      }
		if (forwardL != 0) { forwardX /= forwardL; forwardY /= forwardL; forwardZ /= forwardL; }

		//Rotate space about the z axis so that the left vector lies in the xz plane
		upAngle       = getAngle((float)upX,      (float)upY     );
		forwardAngle  = getAngle((float)forwardX, (float)forwardY);
		angleRotateZ  = -angleRotateZ;
		upAngle += angleRotateZ; forwardAngle += angleRotateZ;
		upX      = cos(upAngle); forwardX      = cos(forwardAngle);
		upY      = sin(upAngle); forwardY      = sin(forwardAngle);
		if (upL      == 0) upX      = upY      = 0;
		if (forwardL == 0) forwardX = forwardY = 0;
		log_line("Graphic3DProcessCameraMove: Horizontal rotate back about Z result is upX as %G upY as %G forwardX as %G forwardY as %G.", upX, upY, forwardX, forwardY);
	#pragma endregion

	//Zoom
	float zoom_acceleration = graphic3d->data->keyboard_zoom_speed1 - graphic3d->data->keyboard_zoom_speed0;
	float zoom_distance_ratio = pow(graphic3d->data->keyboard_zoom_speed0 + zoom_acceleration*zoom_duration_in, zoom_duration_in) / pow(graphic3d->data->keyboard_zoom_speed0 + zoom_acceleration*zoom_duration_out, zoom_duration_out);
	forward_start_length /= zoom_distance_ratio;
	log_line("Graphic3DProcessCameraMove: Zoom result is zoom_acceleration as %G zoom_distance_ratio as %G and forward_start_length as %G.", zoom_acceleration, zoom_distance_ratio, forward_start_length);

	//Normalize vecotrs
	upL       = sqrt(upX     *upX      + upY     *upY      + upZ     *upZ     );
	forwardL  = sqrt(forwardX*forwardX + forwardY*forwardY + forwardZ*forwardZ);
	if (upL      != 0) { upX      /= upL;      upY      /= upL;      upZ      /= upL;      }
	if (forwardL != 0) { forwardX /= forwardL; forwardY /= forwardL; forwardZ /= forwardL; }
	forwardX *= forward_start_length; forwardY *= forward_start_length; forwardZ *= forward_start_length;

	//Save the vectors
	graphic3d->camera_positionX = graphic3d->camera_targetX - forwardX;
	graphic3d->camera_positionY = graphic3d->camera_targetY - forwardY;
	graphic3d->camera_positionZ = graphic3d->camera_targetZ - forwardZ;
	graphic3d->camera_upX = (float)upX; graphic3d->camera_upY = (float)upY; graphic3d->camera_upZ = (float)upZ;
	log_line("Ended Graphic3DProcessCameraMove with result positionX as %G positionY as %G and positionZ as %G.", graphic3d->camera_positionX, graphic3d->camera_positionY, graphic3d->camera_positionZ);
}
void Graphic3DProcessTargetMove(float angleHorizontal, float angleVertical, float zoom_duration_in, float zoom_duration_out)
{
	float angleRotateX, angleRotateY, angleRotateZ, upAngle, forwardAngle, leftAngle;
	double upX,      upY,      upZ,      upL;
	double forwardX, forwardY, forwardZ, forwardL;
	double leftX,    leftY,    leftZ,    leftL;
	
	//Set vectors
	upX       = graphic3d->mouse_start_value_upX;
	upY       = graphic3d->mouse_start_value_upY;
	upZ       = graphic3d->mouse_start_value_upZ;
	forwardX  = graphic3d->mouse_start_value_forwardX;
	forwardY  = graphic3d->mouse_start_value_forwardY;
	forwardZ  = graphic3d->mouse_start_value_forwardZ;
	leftX     = forwardY*upZ - upY*forwardZ;
	leftY     = forwardZ*upX - upZ*forwardX;
	leftZ     = forwardX*upY - upX*forwardY;
	double forward_start_length = sqrt(forwardX*forwardX + forwardY*forwardY + forwardZ*forwardZ);
	
	log_line("Entered Graphic3DProcessTargetMove(1/3) with angleVertical as %G angleHorizontal as %G zoom_duration_in as %G zoom_duration_out as %G forwardX as %G forwardY as %G.", angleVertical, angleHorizontal, zoom_duration_in, zoom_duration_out, forwardX, forwardY);
	log_line("Entered Graphic3DProcessTargetMove(2/3) with forwardZ as %G upX as %G upY as %G upZ as %G leftX as %G.", forwardZ, upX, upY, upZ);
	log_line("Entered Graphic3DProcessTargetMove(3/3) with leftX as %G leftY as %G and leftZ as %G.", leftX, leftY, leftZ);

	//Vertical rotate
	#pragma region Rotate about Z
		//Normalize vecotrs for XY
		upL       = sqrt(upX     *upX      + upY     *upY     );
		forwardL  = sqrt(forwardX*forwardX + forwardY*forwardY);
		if (upL      != 0) { upX      /= upL;      upY      /= upL;      upZ      /= upL;      }
		if (forwardL != 0) { forwardX /= forwardL; forwardY /= forwardL; forwardZ /= forwardL; }

		//Rotate space about the z axis so that the up vector lies in the xz plane
		upAngle       = getAngle((float)upX,      (float)upY     );
		forwardAngle  = getAngle((float)forwardX, (float)forwardY);
		angleRotateZ  = -upAngle;
		upAngle += angleRotateZ; forwardAngle += angleRotateZ;
		upX      = cos(upAngle); forwardX      = cos(forwardAngle);
		upY      = sin(upAngle); forwardY      = sin(forwardAngle);
		if (upL      == 0) upX      = upY      = 0;
		if (forwardL == 0) forwardX = forwardY = 0;
		log_line("Graphic3DProcessTargetMove: Vertical rotate about Z result is upX as %G upY as %G forwardX as %G and forwardY as %G.", upX, upY, forwardX, forwardY);
	#pragma endregion
	#pragma region Rotate about Y
		//Normalize vecotrs for XZ
		upL       = sqrt(upX     *upX      + upZ     *upZ     );
		forwardL  = sqrt(forwardX*forwardX + forwardZ*forwardZ);
		if (upL      != 0) { upX      /= upL;      upY      /= upL;      upZ      /= upL;      }
		if (forwardL != 0) { forwardX /= forwardL; forwardY /= forwardL; forwardZ /= forwardL; }

		//Rotate space about the y axis so that the right vector lies along the x axis
		upAngle       = getAngle((float)upX,      (float)upZ     );
		forwardAngle  = getAngle((float)forwardX, (float)forwardZ);
		angleRotateY  = -upAngle;
		upAngle += angleRotateY; forwardAngle += angleRotateY;
		upX      = cos(upAngle); forwardX      = cos(forwardAngle);
		upZ      = sin(upAngle); forwardZ      = sin(forwardAngle);
		if (upL      == 0) upX      = upZ      = 0;
		if (forwardL == 0) forwardX = forwardZ = 0;
		log_line("Graphic3DProcessTargetMove: Vertical rotate about Y result is upX as %G upZ as %G forwardX as %G and forwardZ as %G.", upX, upZ, forwardX, forwardZ);
	#pragma endregion
	#pragma region Make the actual rotation(about X)
		//Normalize vecotrs for YZ
		upL       = sqrt(upY     *upY      + upZ     *upZ     );
		forwardL  = sqrt(forwardY*forwardY + forwardZ*forwardZ);
		if (upL      != 0) { upX      /= upL;      upY      /= upL;      upZ      /= upL;      }
		if (forwardL != 0) { forwardX /= forwardL; forwardY /= forwardL; forwardZ /= forwardL; }

		//Rotate space about the x axis to get desired rotation
		upAngle       = getAngle((float)upY,      (float)upZ     );
		forwardAngle  = getAngle((float)forwardY, (float)forwardZ);
		angleRotateX  = angleHorizontal;
		upAngle += angleRotateX; forwardAngle += angleRotateX;
		upY      = cos(upAngle); forwardY      = cos(forwardAngle);
		upZ      = sin(upAngle); forwardZ      = sin(forwardAngle);
		if (upL      == 0) upY      = upZ      = 0;
		if (forwardL == 0) forwardY = forwardZ = 0;
		log_line("Graphic3DProcessTargetMove: Vertical rotate about X result is upY as %G upZ as %G forwardY as %G and forwardZ as %G.", upY, upZ, forwardY, forwardZ);
	#pragma endregion
	#pragma region Rotate about Y back
		//Normalize vecotrs for XZ
		upL       = sqrt(upX     *upX      + upZ     *upZ     );
		forwardL  = sqrt(forwardX*forwardX + forwardZ*forwardZ);
		if (upL      != 0) { upX      /= upL;      upY      /= upL;      upZ      /= upL;      }
		if (forwardL != 0) { forwardX /= forwardL; forwardY /= forwardL; forwardZ /= forwardL; }

		//Rotate space about the y axis back to original
		upAngle       = getAngle((float)upX,      (float)upZ     );
		forwardAngle  = getAngle((float)forwardX, (float)forwardZ);
		angleRotateY  = -angleRotateY;
		upAngle += angleRotateY; forwardAngle += angleRotateY;
		upX      = cos(upAngle); forwardX      = cos(forwardAngle);
		upZ      = sin(upAngle); forwardZ      = sin(forwardAngle);
		if (upL      == 0) upX      = upZ      = 0;
		if (forwardL == 0) forwardX = forwardZ = 0;
		log_line("Graphic3DProcessTargetMove: Vertical rotate back about Y result is upX as %G upZ as %G forwardX as %G and forwardZ as %G.", upX, upZ, forwardX, forwardZ);
	#pragma endregion
	#pragma region Rotate about Z back
		//Normalize vecotrs for XY
		upL       = sqrt(upX     *upX      + upY     *upY     );
		forwardL  = sqrt(forwardX*forwardX + forwardY*forwardY);
		if (upL      != 0) { upX      /= upL;      upY      /= upL;      upZ      /= upL;      }
		if (forwardL != 0) { forwardX /= forwardL; forwardY /= forwardL; forwardZ /= forwardL; }

		//Rotate space about the z axis so that the right vector lies in the xz plane
		upAngle       = getAngle((float)upX,      (float)upY     );
		forwardAngle  = getAngle((float)forwardX, (float)forwardY);
		angleRotateZ  = -angleRotateZ;
		upAngle += angleRotateZ; forwardAngle += angleRotateZ;
		upX      = cos(upAngle); forwardX      = cos(forwardAngle);
		upY      = sin(upAngle); forwardY      = sin(forwardAngle);
		if (upL      == 0) upX      = upY      = 0;
		if (forwardL == 0) forwardX = forwardY = 0;
		log_line("Graphic3DProcessTargetMove: Vertical rotate back about Z result is upX as %G upY as %G forwardX as %G and forwardY as %G.", upX, upY, forwardX, forwardY);
	#pragma endregion

	//Horizontal rotate
	#pragma region Rotate about Z
		//Normalize vecotrs for XY
		upL       = sqrt(upX     *upX      + upY     *upY     );
		forwardL  = sqrt(forwardX*forwardX + forwardY*forwardY);
		leftL     = sqrt(leftX   *leftX    + leftY   *leftY   );
		if (upL      != 0) { upX      /= upL;      upY      /= upL;      upZ      /= upL;      }
		if (forwardL != 0) { forwardX /= forwardL; forwardY /= forwardL; forwardZ /= forwardL; }
		if (leftL    != 0) { leftX    /= leftL;    leftY    /= leftL;    leftZ    /= leftL;    }

		//Rotate space about the z axis so that the left vector lies in the xz plane
		upAngle       = getAngle((float)upX,      (float)upY     );
		forwardAngle  = getAngle((float)forwardX, (float)forwardY);
		leftAngle     = getAngle((float)leftX,    (float)leftY   );
		angleRotateZ  = -leftAngle;
		upAngle += angleRotateZ; forwardAngle += angleRotateZ; leftAngle += angleRotateZ;
		upX      = cos(upAngle); forwardX      = cos(forwardAngle); leftX = cos(leftAngle);
		upY      = sin(upAngle); forwardY      = sin(forwardAngle); leftY = sin(leftAngle);
		if (upL      == 0) upX      = upY      = 0;
		if (forwardL == 0) forwardX = forwardY = 0;
		if (leftL    == 0) leftX    = leftY    = 0;
		log_line("Graphic3DProcessTargetMove: Horizontal rotate about Z result is upX as %G upY as %G forwardX as %G forwardY as %G leftX as %G and leftY as %G.", upX, upY, forwardX, forwardY, leftX, leftY);
	#pragma endregion
	#pragma region Rotate about Y
		//Normalize vecotrs for XZ
		upL       = sqrt(upX     *upX      + upZ     *upZ     );
		forwardL  = sqrt(forwardX*forwardX + forwardZ*forwardZ);
		leftL     = sqrt(leftX   *leftX    + leftZ   *leftZ   );
		if (upL      != 0) { upX      /= upL;      upY      /= upL;      upZ      /= upL;      }
		if (forwardL != 0) { forwardX /= forwardL; forwardY /= forwardL; forwardZ /= forwardL; }
		if (leftL    != 0) { leftX    /= leftL;    leftY    /= leftL;    leftZ    /= leftL;    }

		//Rotate space about the y axis so that the left vector lies along the x axis
		upAngle       = getAngle((float)upX,      (float)upZ     );
		forwardAngle  = getAngle((float)forwardX, (float)forwardZ);
		leftAngle     = getAngle((float)leftX,    (float)leftZ   );
		angleRotateY  = -leftAngle;
		upAngle += angleRotateY; forwardAngle += angleRotateY; leftAngle += angleRotateY;
		upX      = cos(upAngle); forwardX      = cos(forwardAngle); leftX = cos(leftAngle);
		upZ      = sin(upAngle); forwardZ      = sin(forwardAngle); leftZ = sin(leftAngle);
		if (upL      == 0) upX      = upZ      = 0;
		if (forwardL == 0) forwardX = forwardZ = 0;
		if (leftL    == 0) leftX    = leftZ    = 0;
		log_line("Graphic3DProcessTargetMove: Horizontal rotate about Y result is upX as %G upZ as %G forwardX as %G forwardZ as %G leftX as %G and leftZ as %G.", upX, upZ, forwardX, forwardZ, leftX, leftZ);
	#pragma endregion
	#pragma region Make the actual rotation(about X)
		//Normalize vecotrs for YZ
		upL       = sqrt(upY     *upY      + upZ     *upZ     );
		forwardL  = sqrt(forwardY*forwardY + forwardZ*forwardZ);
		if (upL      != 0) { upX      /= upL;      upY      /= upL;      upZ      /= upL;      }
		if (forwardL != 0) { forwardX /= forwardL; forwardY /= forwardL; forwardZ /= forwardL; }

		//Rotate space about the x axis to get desired rotation
		upAngle       = getAngle((float)upY,      (float)upZ     );
		forwardAngle  = getAngle((float)forwardY, (float)forwardZ);
		angleRotateX  = angleVertical;
		upAngle += angleRotateX; forwardAngle += angleRotateX;
		upY      = cos(upAngle); forwardY      = cos(forwardAngle);
		upZ      = sin(upAngle); forwardZ      = sin(forwardAngle);
		if (upL      == 0) upY      = upZ      = 0;
		if (forwardL == 0) forwardY = forwardZ = 0;
		log_line("Graphic3DProcessTargetMove: Horizontal rotate about X result is upY as %G upZ as %G forwardY as %G forwardZ as %G.", upY, upZ, forwardY, forwardZ);
	#pragma endregion
	#pragma region Rotate about Y back
		//Normalize vecotrs for XZ
		upL       = sqrt(upX     *upX      + upZ     *upZ     );
		forwardL  = sqrt(forwardX*forwardX + forwardZ*forwardZ);
		if (upL      != 0) { upX      /= upL;      upY      /= upL;      upZ      /= upL;      }
		if (forwardL != 0) { forwardX /= forwardL; forwardY /= forwardL; forwardZ /= forwardL; }

		//Rotate space about the y axis back to original
		upAngle       = getAngle((float)upX,      (float)upZ     );
		forwardAngle  = getAngle((float)forwardX, (float)forwardZ);
		angleRotateY  = -angleRotateY;
		upAngle += angleRotateY; forwardAngle += angleRotateY;
		upX      = cos(upAngle); forwardX      = cos(forwardAngle);
		upZ      = sin(upAngle); forwardZ      = sin(forwardAngle);
		if (upL      == 0) upX      = upZ      = 0;
		if (forwardL == 0) forwardX = forwardZ = 0;
		log_line("Graphic3DProcessTargetMove: Horizontal rotate back about Y result is upX as %G upZ as %G forwardX as %G forwardZ as %G.", upX, upZ, forwardX, forwardZ);
	#pragma endregion
	#pragma region Rotate about Z back
		//Normalize vecotrs for XY
		upL       = sqrt(upX     *upX      + upY     *upY     );
		forwardL  = sqrt(forwardX*forwardX + forwardY*forwardY);
		if (upL      != 0) { upX      /= upL;      upY      /= upL;      upZ      /= upL;      }
		if (forwardL != 0) { forwardX /= forwardL; forwardY /= forwardL; forwardZ /= forwardL; }

		//Rotate space about the z axis so that the left vector lies in the xz plane
		upAngle       = getAngle((float)upX,      (float)upY     );
		forwardAngle  = getAngle((float)forwardX, (float)forwardY);
		angleRotateZ  = -angleRotateZ;
		upAngle += angleRotateZ; forwardAngle += angleRotateZ;
		upX      = cos(upAngle); forwardX      = cos(forwardAngle);
		upY      = sin(upAngle); forwardY      = sin(forwardAngle);
		if (upL      == 0) upX      = upY      = 0;
		if (forwardL == 0) forwardX = forwardY = 0;
		log_line("Graphic3DProcessTargetMove: Horizontal rotate back about Z result is upX as %G upY as %G forwardX as %G forwardY as %G.", upX, upY, forwardX, forwardY);
	#pragma endregion
	
	//Zoom
	float zoom_acceleration = graphic3d->data->keyboard_zoom_speed1 - graphic3d->data->keyboard_zoom_speed0;
	float zoom_distance_ratio = pow(graphic3d->data->keyboard_zoom_speed0 + zoom_acceleration*zoom_duration_in, zoom_duration_in) / pow(graphic3d->data->keyboard_zoom_speed0 + zoom_acceleration*zoom_duration_out, zoom_duration_out);
	forward_start_length *= zoom_distance_ratio;
	log_line("Graphic3DProcessTargetMove: Zoom result is zoom_acceleration as %G zoom_distance_ratio as %G and forward_start_length as %G.", zoom_acceleration, zoom_distance_ratio, forward_start_length);

	//Normalize vecotrs
	upL       = sqrt(upX     *upX      + upY     *upY      + upZ     *upZ     );
	forwardL  = sqrt(forwardX*forwardX + forwardY*forwardY + forwardZ*forwardZ);
	if (upL      != 0) { upX      /= upL;      upY      /= upL;      upZ      /= upL;      }
	if (forwardL != 0) { forwardX /= forwardL; forwardY /= forwardL; forwardZ /= forwardL; }
	forwardX *= forward_start_length; forwardY *= forward_start_length; forwardZ *= forward_start_length;

	//Save the vectors
	graphic3d->camera_targetX = graphic3d->camera_positionX + forwardX;
	graphic3d->camera_targetY = graphic3d->camera_positionY + forwardY;
	graphic3d->camera_targetZ = graphic3d->camera_positionZ + forwardZ;
	graphic3d->camera_upX = (float)upX; graphic3d->camera_upY = (float)upY; graphic3d->camera_upZ = (float)upZ;
	log_line("Ended Graphic3DProcessTargetMoveVertical with result positionX as %G positionY as %G and positionZ as %G.", graphic3d->camera_positionX, graphic3d->camera_positionY, graphic3d->camera_positionZ);
}
void Graphic3DFixIndefinedValues()
{
	if (graphic3d->camera_positionX >  DBL_MAX) graphic3d->camera_positionX =  DBL_MAX;
	if (graphic3d->camera_positionX < -DBL_MAX) graphic3d->camera_positionX = -DBL_MAX;
	if (graphic3d->camera_positionY >  DBL_MAX) graphic3d->camera_positionY =  DBL_MAX;
	if (graphic3d->camera_positionY < -DBL_MAX) graphic3d->camera_positionY = -DBL_MAX;
	if (graphic3d->camera_positionZ >  DBL_MAX) graphic3d->camera_positionZ =  DBL_MAX;
	if (graphic3d->camera_positionZ < -DBL_MAX) graphic3d->camera_positionZ = -DBL_MAX;
	if (graphic3d->camera_targetX   >  DBL_MAX) graphic3d->camera_targetX   =  DBL_MAX;
	if (graphic3d->camera_targetX   < -DBL_MAX) graphic3d->camera_targetX   = -DBL_MAX;
	if (graphic3d->camera_targetY   >  DBL_MAX) graphic3d->camera_targetY   =  DBL_MAX;
	if (graphic3d->camera_targetY   < -DBL_MAX) graphic3d->camera_targetY   = -DBL_MAX;
	if (graphic3d->camera_targetZ   >  DBL_MAX) graphic3d->camera_targetZ   =  DBL_MAX;
	if (graphic3d->camera_targetZ   < -DBL_MAX) graphic3d->camera_targetZ   = -DBL_MAX;
	if (graphic3d->camera_upX       >  FLT_MAX) graphic3d->camera_upX       =  FLT_MAX;
	if (graphic3d->camera_upX       < -FLT_MAX) graphic3d->camera_upX       = -FLT_MAX;
	if (graphic3d->camera_upY       >  FLT_MAX) graphic3d->camera_upY       =  FLT_MAX;
	if (graphic3d->camera_upY       < -FLT_MAX) graphic3d->camera_upY       = -FLT_MAX;
	if (graphic3d->camera_upZ       >  FLT_MAX) graphic3d->camera_upZ       =  FLT_MAX;
	if (graphic3d->camera_upZ       < -FLT_MAX) graphic3d->camera_upZ       = -FLT_MAX;
	if (graphic3d->cam_target_distance2 == 0)   graphic3d->camera_positionX =  DBL_MIN + graphic3d->camera_targetX;
	if (graphic3d->cam_target_distance2 == 0)   graphic3d->camera_positionY =  DBL_MIN + graphic3d->camera_targetY;
	if (graphic3d->cam_target_distance2 == 0)   graphic3d->camera_positionZ =  DBL_MIN + graphic3d->camera_targetZ;
}

void Graphic3DSetOrthographicProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, graphic3d->width, graphic3d->height, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}
void Graphic3DRestorePerspectiveProjection()
{
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
void Graphic3DRenderBitmapString(int x, int y, char* string)
{
  for (char* c = string; *c != '\0'; c++) 
  {
	if (*c != ' ') glRasterPos2i(x, y);
	if (*c != ' ') glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	x += 8;
  }
}
bool Graphic3DIsInSight(double X, double Y, double Z, double r)
{
	if(graphic3d->frustum[0][0]*X + graphic3d->frustum[0][1]*Y + graphic3d->frustum[0][2]*Z + graphic3d->frustum[0][3] <= -r) return false;
	if(graphic3d->frustum[1][0]*X + graphic3d->frustum[1][1]*Y + graphic3d->frustum[1][2]*Z + graphic3d->frustum[1][3] <= -r) return false;
	if(graphic3d->frustum[2][0]*X + graphic3d->frustum[2][1]*Y + graphic3d->frustum[2][2]*Z + graphic3d->frustum[2][3] <= -r) return false;
	if(graphic3d->frustum[3][0]*X + graphic3d->frustum[3][1]*Y + graphic3d->frustum[3][2]*Z + graphic3d->frustum[3][3] <= -r) return false;
	if(graphic3d->frustum[4][0]*X + graphic3d->frustum[4][1]*Y + graphic3d->frustum[4][2]*Z + graphic3d->frustum[4][3] <= -r) return false;
	if(graphic3d->frustum[5][0]*X + graphic3d->frustum[5][1]*Y + graphic3d->frustum[5][2]*Z + graphic3d->frustum[5][3] <= -r) return false;
   return true;
}
