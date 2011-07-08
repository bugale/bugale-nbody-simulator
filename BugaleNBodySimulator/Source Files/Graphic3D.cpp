#include "..\SharedHeader.h"

Graphic3D* graphic3d;

void NewGraphic3D(Data* data, SharedData* shared)
{
	if (data->graphic_max_rate == 0) return;

	graphic3d = new Graphic3D();
	graphic3d->data = data;
	graphic3d->shared = shared;

	graphic3d->width = data->width;
	graphic3d->height = data->height;
	graphic3d->ratio = (double)data->width / (double)graphic3d->height;
	graphic3d->title = "Bugale N-Body Simulation 3D Graphical Output";
	graphic3d->fullscreen = data->fullscreen;
	graphic3d->clear_screen = true;
	graphic3d->show_trails = false;
	graphic3d->min_text = false;
	graphic3d->crosshair = true;
	graphic3d->sin_field_of_view = 2 * sin(graphic3d->data->field_of_view * M_PI / (180 * 2));
	
	graphic3d->camera_positionX = data->camera_positionX;
	graphic3d->camera_positionY = data->camera_positionY;
	graphic3d->camera_positionZ = data->camera_positionZ;
	graphic3d->camera_targetX   = data->camera_targetX;
	graphic3d->camera_targetY   = data->camera_targetY;
	graphic3d->camera_targetZ   = data->camera_targetZ;
	graphic3d->camera_upX       = data->camera_upX;
	graphic3d->camera_upY       = data->camera_upY;
	graphic3d->camera_upZ       = data->camera_upZ;

	graphic3d->cam_target_distance = sqrt((graphic3d->camera_positionX - graphic3d->camera_targetX) * (graphic3d->camera_positionX - graphic3d->camera_targetX) + 
										  (graphic3d->camera_positionY - graphic3d->camera_targetY) * (graphic3d->camera_positionY - graphic3d->camera_targetY) + 
										  (graphic3d->camera_positionZ - graphic3d->camera_targetZ) * (graphic3d->camera_positionZ - graphic3d->camera_targetZ));
	graphic3d->height_meters = graphic3d->cam_target_distance * graphic3d->sin_field_of_view;
	graphic3d->width_meters  = graphic3d->height_meters * graphic3d->ratio;

	graphic3d->trail_curpos = 0;
	graphic3d->trailX = (double*)malloc(sizeof(double) * graphic3d->data->num_of_bodies * graphic3d->data->max_trails);
	graphic3d->trailY = (double*)malloc(sizeof(double) * graphic3d->data->num_of_bodies * graphic3d->data->max_trails);
	graphic3d->trailZ = (double*)malloc(sizeof(double) * graphic3d->data->num_of_bodies * graphic3d->data->max_trails);
	for (int i = 0; i < graphic3d->data->num_of_bodies * graphic3d->data->max_trails; i++) graphic3d->trailX[i] = (DBL_MAX + 1);
	for (int i = 0; i < graphic3d->data->num_of_bodies * graphic3d->data->max_trails; i++) graphic3d->trailY[i] = (DBL_MAX + 1);
	for (int i = 0; i < graphic3d->data->num_of_bodies * graphic3d->data->max_trails; i++) graphic3d->trailZ[i] = (DBL_MAX + 1);

	graphic3d->keyboard_move_starttime   = (long long*)malloc(sizeof(long long) * 0xFF);
	graphic3d->keyboard_move_started     = (bool     *)malloc(sizeof(bool     ) * 0xFF);
	graphic3d->keyboard_move_start_value = (double   *)malloc(sizeof(double   ) * 0xFF);
	for (int i = 0; i < 0xFF; i++) graphic3d->keyboard_move_starttime  [i] = 0;
	for (int i = 0; i < 0xFF; i++) graphic3d->keyboard_move_started    [i] = false;
	for (int i = 0; i < 0xFF; i++) graphic3d->keyboard_move_start_value[i] = false;

	graphic3d->body_index = 0;
	graphic3d->body_index_entered = false;
	graphic3d->stick_body_index = 0;
	graphic3d->stick_body_index_entered = false;

	char* instructions = (char*)malloc(4096);
	instructions[0] = '\0';
	sprintf(instructions, "%s%s", instructions, "2D Graphical Output Instructions:\n\n");
	sprintf(instructions, "%s%s", instructions, "    ESC  : Close the Simulator\n");
	sprintf(instructions, "%s%s", instructions, "   Arrows: Move Your Camera\n");
	sprintf(instructions, "%s%s", instructions, "   + or -: Zoom In or Zoom Out\n");
	sprintf(instructions, "%s%s", instructions, "     r   : Reset Your Camera Position\n");
	sprintf(instructions, "%s%s", instructions, "     m   : Toggle Minimal Text Mode = Much More Frames Per Second\n");
	sprintf(instructions, "%s%s", instructions, "     t   : Toggle Trail Showal\n");
	sprintf(instructions, "%s%s", instructions, "     c   : Toggle Screen Clearance After Every Frame\n");
	sprintf(instructions, "%s%s", instructions, "     h   : Toggle Crosshair Showal\n");
	sprintf(instructions, "%s%s", instructions, "     p   : Toggle Pause\n");
	sprintf(instructions, "%s%s", instructions, "   After a Number Has Been Assigned, Press Enter to Show the Body with the\n");
	sprintf(instructions, "%s%s", instructions, "   Given Index on the Center of the Screen, or Press Space to Always show the\n");
	sprintf(instructions, "%s%s", instructions, "   Body with the Given Index on the Center of the Screen and press again to\n");
	sprintf(instructions, "%s%s", instructions, "   disable it.\n");
	sprintf(instructions, "%s%s", instructions, "\nThank you for using Bugale N-Body Simulator, and have a pleasant day!\n\n\n\n");
	std::cout << instructions;
	free(instructions);

	Graphic3DInitialize();
}
void Graphic3DInitialize()
{
	//Initialize GLUT and create the window
	glutInitDisplayMode   (GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize    (graphic3d->width, graphic3d->height);
	glutCreateWindow      (graphic3d->title);
	glutIgnoreKeyRepeat   (1);
	if (graphic3d->fullscreen) glutFullScreen();

	//Handlers
	glutDisplayFunc   (Graphic3DRenderHandler     );
	glutReshapeFunc   (Graphic3DRatioHandler      );
	glutKeyboardFunc  (Graphic3DKeyboardHandler   );
	glutKeyboardUpFunc(Graphic3DKeyboardUpHandler );
	glutSpecialFunc   (Graphic3DSKeyboardHandler  );
	glutSpecialUpFunc (Graphic3DSKeyboardUpHandler);

	//Enter GLUT event processing cycle
	glutMainLoop();
}

void Graphic3DRenderHandler()
{
	Graphic3DProcessCameraMove();
	Graphic3DCalculateTemp();
	Graphic3DClearScreen();
	Graphic3DSetCamera();

	Graphic3DDrawTrails();
	Graphic3DDrawBodies();
	Graphic3DDrawCrosshair(true);
	Graphic3DDrawBodyIndex();
	Graphic3DDrawMinText();
	Graphic3DDrawText();
	Graphic3DSaveTrails();

	Graphic3DFinalizeFrame();
}
void Graphic3DRatioHandler(int width, int height)
{
	if (height == 0) height++; //Prevent a division by zero.

	graphic3d->width = width;
	graphic3d->height = height;
	graphic3d->ratio = (double)width / (double)height;

	//Recreate the Projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluPerspective(graphic3d->data->field_of_view / 2, graphic3d->ratio, graphic3d->data->near_plane_distance, graphic3d->data->far_plane_distance);

	//Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
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
	if (key == '+') Graphic3DToggleCameraMove(0x00, true);
	if (key == '-') Graphic3DToggleCameraMove(0x10, true);

	if (key == '\r') Graphic3DShowBodyIndex();
	if (key == ' ') Graphic3DToggleStickToBody();
	if (key > 47 && key < 58) Graphic3DAddDigitToBodyIndex((int)(key - 48)); //Digit
}
void Graphic3DKeyboardUpHandler(unsigned char key, int x, int y)
{
	if (key == '+') Graphic3DToggleCameraMove(0x00, false);
	if (key == '-') Graphic3DToggleCameraMove(0x10, false);
}
void Graphic3DSKeyboardHandler(int key, int x, int y)
{
	Graphic3DClearScreen();
}
void Graphic3DSKeyboardUpHandler(int key, int x, int y)
{
}

void Graphic3DClearScreen()
{
	if (graphic3d->clear_screen) glClear(GL_COLOR_BUFFER_BIT);
	else
	{
		Graphic3DSetOrthographicProjection();
		glColor4f(0, 0, 0, 1);
		glRasterPos2f(0, 0);
		glBegin(GL_QUADS);
		if (graphic3d->body_index_entered)
		{
			glVertex3f(graphic3d->width - 9 * 8, 0, 0);
			glVertex3f(graphic3d->width - 9 * 8, 2 * 13, 0);
			glVertex3f(graphic3d->width, 2 * 13, 0);
			glVertex3f(graphic3d->width, 0, 0);
		}
		if (graphic3d->stick_body_index_entered)
		{
			glVertex3f(graphic3d->width - 9 * 8, 2 * 13, 0);
			glVertex3f(graphic3d->width - 9 * 8, 3 * 13, 0);
			glVertex3f(graphic3d->width, 3 * 13, 0);
			glVertex3f(graphic3d->width, 2 * 13, 0);
		}
		if (!graphic3d->min_text)
		{
			glVertex3f(0, 0, 0);
			glVertex3f(0, 13 * 13, 0);
			glVertex3f(42 * 8, 13 * 13, 0);
			glVertex3f(42 * 8, 0, 0);

			glVertex3f(0, graphic3d->height - 13 * 5, 0);
			glVertex3f(0, graphic3d->height, 0);
			glVertex3f(38 * 8, graphic3d->height, 0);
			glVertex3f(38 * 8, graphic3d->height - 13 * 5, 0);
		}
		if (graphic3d->min_text)
		{
			glVertex3f(0, 0, 0);
			glVertex3f(0, 2 * 13, 0);
			glVertex3f(30 * 8, 2 * 13, 0);
			glVertex3f(30 * 8, 0, 0);
		}
		glEnd();
		Graphic3DRestorePerspectiveProjection();
	}
}
void Graphic3DSetCamera()
{
	glLoadIdentity();
	gluLookAt(graphic3d->camera_positionX, graphic3d->camera_positionY, graphic3d->camera_positionZ,
			  graphic3d->camera_targetX,   graphic3d->camera_targetY,   graphic3d->camera_targetZ,
			  graphic3d->camera_upX,       graphic3d->camera_upY,       graphic3d->camera_upZ);
}
void Graphic3DFinalizeFrame()
{
	graphic3d->shared->frames++;
	if (graphic3d->data->graphic_max_rate > 0) usleep((long long)1000000 / graphic3d->data->graphic_max_rate);
	glFlush();
	glutPostRedisplay();
}

void Graphic3DExit()
{
	graphic3d->shared->exit = true;
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
}
void Graphic3DToggleCameraMove(char direction, bool pressed)
{
	if (!graphic3d->keyboard_move_started[(direction & 0x0F) + 0x00] && !graphic3d->keyboard_move_started[(direction & 0x0F) + 0x10] && pressed)
	{
		graphic3d->keyboard_move_starttime[direction] = get_current_time_usec();
		graphic3d->keyboard_move_started  [direction] = true;
		if ((direction & 0x0F) == 0) graphic3d->keyboard_move_start_value[direction] = graphic3d->camera_targetX;
		if ((direction & 0x0F) == 1) graphic3d->keyboard_move_start_value[direction] = graphic3d->camera_targetY;
		if ((direction & 0x0F) == 2) graphic3d->keyboard_move_start_value[direction] = graphic3d->camera_targetZ;
		if ((direction & 0x0F) == 3) graphic3d->keyboard_move_start_value[direction] = graphic3d->cam_target_distance;
	}
	if (graphic3d->keyboard_move_started[direction] && !pressed) graphic3d->keyboard_move_started[direction] = false;
}
void Graphic3DTogglePause()
{
	if (graphic3d->shared->reached_max_calculations) return;
	if (graphic3d->shared->pause) graphic3d->shared->start_time += get_current_time_usec() - graphic3d->shared->pause_start_time;
	else graphic3d->shared->pause_start_time = get_current_time_usec();
	graphic3d->shared->pause = !graphic3d->shared->pause;
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
		graphic3d->camera_positionX = graphic3d->data->bodies[graphic3d->body_index]._positionX;
		graphic3d->camera_positionY = graphic3d->data->bodies[graphic3d->body_index]._positionY;
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
	if (graphic3d->keyboard_move_started[0x00]) //Zoom In
	{
		double duration = (double)(get_current_time_usec() - graphic3d->keyboard_move_starttime[0x00]) / 1000000;
		double acceleration = graphic3d->data->keyboard_zoom_speed1 - graphic3d->data->keyboard_zoom_speed0;
		double new_val = graphic3d->keyboard_move_start_value[0x00] * pow(graphic3d->data->keyboard_zoom_speed0 + acceleration * duration, -duration);
		graphic3d->camera_positionX = graphic3d->camera_targetX + (graphic3d->camera_positionX - graphic3d->camera_targetX) * (new_val / graphic3d->cam_target_distance);
		graphic3d->camera_positionY = graphic3d->camera_targetY + (graphic3d->camera_positionY - graphic3d->camera_targetY) * (new_val / graphic3d->cam_target_distance);
		graphic3d->camera_positionZ = graphic3d->camera_targetZ + (graphic3d->camera_positionZ - graphic3d->camera_targetZ) * (new_val / graphic3d->cam_target_distance);
	}
	if (graphic3d->keyboard_move_started[0x10]) //Zoom Out
	{
		double duration = (double)(get_current_time_usec() - graphic3d->keyboard_move_starttime[0x10]) / 1000000;
		double acceleration = graphic3d->data->keyboard_zoom_speed1 - graphic3d->data->keyboard_zoom_speed0;
		double new_val = graphic3d->keyboard_move_start_value[0x10] * pow(graphic3d->data->keyboard_zoom_speed0 + acceleration * duration, duration);
		graphic3d->camera_positionX = graphic3d->camera_targetX + (graphic3d->camera_positionX - graphic3d->camera_targetX) * (new_val / graphic3d->cam_target_distance);
		graphic3d->camera_positionY = graphic3d->camera_targetY + (graphic3d->camera_positionY - graphic3d->camera_targetY) * (new_val / graphic3d->cam_target_distance);
		graphic3d->camera_positionZ = graphic3d->camera_targetZ + (graphic3d->camera_positionZ - graphic3d->camera_targetZ) * (new_val / graphic3d->cam_target_distance);
	}

	//Fix indefined values
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
	if (graphic3d->camera_upX       >  DBL_MAX) graphic3d->camera_upX       =  DBL_MAX;
	if (graphic3d->camera_upX       < -DBL_MAX) graphic3d->camera_upX       = -DBL_MAX;
	if (graphic3d->camera_upY       >  DBL_MAX) graphic3d->camera_upY       =  DBL_MAX;
	if (graphic3d->camera_upY       < -DBL_MAX) graphic3d->camera_upY       = -DBL_MAX;
	if (graphic3d->camera_upZ       >  DBL_MAX) graphic3d->camera_upZ       =  DBL_MAX;
	if (graphic3d->camera_upZ       < -DBL_MAX) graphic3d->camera_upZ       = -DBL_MAX;
	if (graphic3d->cam_target_distance == 0)    graphic3d->camera_positionX =  DBL_MIN;
	if (graphic3d->cam_target_distance == 0)    graphic3d->camera_positionY =  DBL_MIN;
	if (graphic3d->cam_target_distance == 0)    graphic3d->camera_positionZ =  DBL_MIN;
}
void Graphic3DCalculateTemp()
{
	graphic3d->cam_target_distance = sqrt((graphic3d->camera_positionX - graphic3d->camera_targetX) * (graphic3d->camera_positionX - graphic3d->camera_targetX) + 
										  (graphic3d->camera_positionY - graphic3d->camera_targetY) * (graphic3d->camera_positionY - graphic3d->camera_targetY) + 
										  (graphic3d->camera_positionZ - graphic3d->camera_targetZ) * (graphic3d->camera_positionZ - graphic3d->camera_targetZ));
	graphic3d->height_meters = graphic3d->cam_target_distance * graphic3d->sin_field_of_view;
	graphic3d->width_meters  = graphic3d->height_meters * graphic3d->ratio;
}
void Graphic3DDrawBodies()
{
	for (int i = 0; i < graphic3d->data->num_of_bodies; i++)
	{
		if (!Graphic3DIsInSight(graphic3d->data->bodies[i]._positionX, graphic3d->data->bodies[i]._positionY, graphic3d->data->bodies[i]._positionZ, graphic3d->data->bodies[i]._radius)) continue;
		Graphic3DDrawBody(graphic3d->data->bodies[i]._positionX,
						  graphic3d->data->bodies[i]._positionY,
						  graphic3d->data->bodies[i]._positionZ,
						  graphic3d->data->bodies[i]._radius,
				   (double)graphic3d->data->bodies[i]._colorR * 0.003921568,
				   (double)graphic3d->data->bodies[i]._colorG * 0.003921568,
				   (double)graphic3d->data->bodies[i]._colorB * 0.003921568,
				   (double)graphic3d->data->bodies[i]._colorA * 0.003921568,
						  false);
	}
}
void Graphic3DDrawBodyIndex()
{
	char* string = (char*)malloc(1024);
	Graphic3DSetOrthographicProjection();

	if (graphic3d->body_index_entered)
	{
		glColor4f(1, 0, 0, 1);
		sprintf(string, "%d", graphic3d->body_index);
		Graphic3DRenderBitmapString(graphic3d->width - strlen(string) * 8, 13, string);
	}

	if (graphic3d->stick_body_index_entered)
	{
		glColor4f(0, 0, 1, 1);
		sprintf(string, "%d", graphic3d->stick_body_index);
		Graphic3DRenderBitmapString(graphic3d->width - strlen(string) * 8, 2 * 13, string);
	}

	free(string);
	Graphic3DRestorePerspectiveProjection();
}
void Graphic3DDrawCrosshair(bool white)
{
	if (!graphic3d->crosshair) return;

	Graphic3DSetOrthographicProjection();
	glColor4f(white, white, white, 1);
	float length = graphic3d->height * 0.01;
	glBegin(GL_LINES);
		glVertex3f(graphic3d->width * 0.5 - length * 0.5, graphic3d->height * 0.5, 0);
		glVertex3f(graphic3d->width * 0.5 + length * 0.5, graphic3d->height * 0.5, 0);
		glVertex3f(graphic3d->width * 0.5, graphic3d->height * 0.5 - length * 0.5, 0);
		glVertex3f(graphic3d->width * 0.5, graphic3d->height * 0.5 + length * 0.5, 0);
	glEnd();
	Graphic3DRestorePerspectiveProjection();
}
void Graphic3DDrawText()
{
	if (graphic3d->min_text) return;

	long long real_time;
	char* string = (char*)malloc(1024);
	int curY = 0;
	if (graphic3d->shared->pause) real_time = graphic3d->shared->pause_start_time - graphic3d->shared->start_time;
	else real_time = get_current_time_usec() - graphic3d->shared->start_time;
	Graphic3DSetOrthographicProjection();

	glColor4f(1, 1, 1, 1);

	sprintf(string, "Frames Per Second: %G", graphic3d->shared->frames_per_second);
	Graphic3DRenderBitmapString(0, curY += 13, string);

	sprintf(string, "Calculations Per Second:   %+E", graphic3d->shared->calculations_per_second);
	Graphic3DRenderBitmapString(0, curY += 13, string);

	sprintf(string, "Total Calculations Done:   %+E", (double)graphic3d->shared->calculations);
	Graphic3DRenderBitmapString(0, curY += 13, string);

	sprintf(string, "DeltaTime:                 %+E", graphic3d->data->dt);
	Graphic3DRenderBitmapString(0, curY += 13, string);

	sprintf(string, "Simulated Seconds Past:    %+E", (double)graphic3d->data->dt * (double)graphic3d->shared->calculations);
	Graphic3DRenderBitmapString(0, curY += 13, string);

	sprintf(string, "Simulated Years Past:      %+E", (double)graphic3d->data->dt * (double)graphic3d->shared->calculations / (double)(31557600));
	Graphic3DRenderBitmapString(0, curY += 13, string);

	sprintf(string, "Real Seconds Past:         %+E", (double)real_time / (double)1000000);
	Graphic3DRenderBitmapString(0, curY += 13, string);

	sprintf(string, "Simulated-Real Time Ratio: %+E", ((double)graphic3d->data->dt * (double)graphic3d->shared->calculations) / ((double)real_time / (double)1000000));
	Graphic3DRenderBitmapString(0, curY += 13, string);

	sprintf(string, "Gravitational Constant:    %+E", graphic3d->data->g);
	Graphic3DRenderBitmapString(0, curY += 13, string);
	
	sprintf(string, "Number of Bodies: %d", graphic3d->data->num_of_bodies);
	Graphic3DRenderBitmapString(0, curY += 13, string);

	sprintf(string, "Integration Algorithm: %s %s", (graphic3d->data->two_dimensional_calculation ? "3D": "3D"), get_algorithm_name(graphic3d->data->algorithm));
	Graphic3DRenderBitmapString(0, curY += 13, string);

	if (graphic3d->shared->calculated_energy)
		sprintf(string, "Energy Error: %+E", graphic3d->shared->error);
	else
		sprintf(string, "Energy Error: Pause to Calculate...");
	Graphic3DRenderBitmapString(0, curY += 13, string);

	curY = graphic3d->height + 13 - 3;

	sprintf(string, "Camera Up:       (%+E,%+E,%+E)", graphic3d->camera_upX, graphic3d->camera_upY, graphic3d->camera_upZ);
	Graphic3DRenderBitmapString(0, curY -= 13, string);
	
	sprintf(string, "Camera Target:   (%+E,%+E,%+E)", graphic3d->camera_targetX, graphic3d->camera_targetY, graphic3d->camera_targetZ);
	Graphic3DRenderBitmapString(0, curY -= 13, string);

	sprintf(string, "Camera Position: (%+E,%+E,%+E)", graphic3d->camera_positionX, graphic3d->camera_positionY, graphic3d->camera_positionZ);
	Graphic3DRenderBitmapString(0, curY -= 13, string);
	
	sprintf(string, "Screen Height in Meters: %E", graphic3d->height_meters);
	Graphic3DRenderBitmapString(0, curY -= 13, string);
	
	free(string);
	Graphic3DRestorePerspectiveProjection();
}
void Graphic3DDrawMinText()
{
	if (!graphic3d->min_text) return;

	char* string = (char*)malloc(1024);
	Graphic3DSetOrthographicProjection();

	glColor4f(1, 1, 1, 1);
	sprintf(string, "fps:%d cps:%d", (int)graphic3d->shared->frames_per_second, (int)graphic3d->shared->calculations_per_second);
	Graphic3DRenderBitmapString(0, 13, string);
	
	free(string);
	Graphic3DRestorePerspectiveProjection();
}
void Graphic3DDrawTrails()
{
	if (!graphic3d->show_trails) return;
	for (int trail = graphic3d->trail_curpos; trail < graphic3d->data->max_trails; trail++)
		for (int body = 0; body < graphic3d->data->num_of_bodies; body++)
		{
			if (!Graphic3DIsInSight(graphic3d->trailX[trail*graphic3d->data->num_of_bodies + body], graphic3d->trailY[trail*graphic3d->data->num_of_bodies + body], graphic3d->trailZ[trail*graphic3d->data->num_of_bodies + body], graphic3d->data->bodies[body]._trailwidth)) continue;
			Graphic3DDrawBody(graphic3d->trailX[trail*graphic3d->data->num_of_bodies + body],
							  graphic3d->trailY[trail*graphic3d->data->num_of_bodies + body],
							  graphic3d->trailZ[trail*graphic3d->data->num_of_bodies + body],
							  graphic3d->data->bodies[body]._trailwidth,
					   (double)graphic3d->data->bodies[body]._trailcolorR * 0.003921568,
					   (double)graphic3d->data->bodies[body]._trailcolorG * 0.003921568,
					   (double)graphic3d->data->bodies[body]._trailcolorB * 0.003921568,
					   (double)graphic3d->data->bodies[body]._trailcolorA * 0.003921568,
							  true);
		}
	for (int trail = 0; trail < graphic3d->trail_curpos; trail++)
		for (int body = 0; body < graphic3d->data->num_of_bodies; body++)
		{
			if (!Graphic3DIsInSight(graphic3d->trailX[trail*graphic3d->data->num_of_bodies + body], graphic3d->trailY[trail*graphic3d->data->num_of_bodies + body], graphic3d->trailZ[trail*graphic3d->data->num_of_bodies + body], graphic3d->data->bodies[body]._trailwidth)) continue;
			Graphic3DDrawBody(graphic3d->trailX[trail*graphic3d->data->num_of_bodies + body],
							  graphic3d->trailY[trail*graphic3d->data->num_of_bodies + body],
							  graphic3d->trailZ[trail*graphic3d->data->num_of_bodies + body],
							  graphic3d->data->bodies[body]._trailwidth,
					   (double)graphic3d->data->bodies[body]._trailcolorR * 0.003921568,
					   (double)graphic3d->data->bodies[body]._trailcolorG * 0.003921568,
					   (double)graphic3d->data->bodies[body]._trailcolorB * 0.003921568,
					   (double)graphic3d->data->bodies[body]._trailcolorA * 0.003921568,
							  true);
		}
}
void Graphic3DSaveTrails()
{
	if (graphic3d->shared->pause) return;
	for (int body = 0; body < graphic3d->data->num_of_bodies; body++)
	{
		graphic3d->trailX[graphic3d->trail_curpos * graphic3d->data->num_of_bodies + body] = graphic3d->data->bodies[body]._positionX;
		graphic3d->trailY[graphic3d->trail_curpos * graphic3d->data->num_of_bodies + body] = graphic3d->data->bodies[body]._positionY;
		graphic3d->trailZ[graphic3d->trail_curpos * graphic3d->data->num_of_bodies + body] = graphic3d->data->bodies[body]._positionZ;
	}
	graphic3d->trail_curpos++;
	if (graphic3d->trail_curpos >= graphic3d->data->max_trails) graphic3d->trail_curpos = 0;
}
void Graphic3DDrawBody(double X, double Y, double Z, double radius, double R, double G, double B, double A, bool trail)
{
	glPushMatrix();
	glColor4f(R, G, B, A);
	glTranslatef(X, Y, Z);
	if (trail) 
	{
		glutSolidCube(radius);
	}
	else 
	{
		glutSolidSphere(radius, graphic3d->data->sphere_slices, graphic3d->data->sphere_stacks);
	}
	glPopMatrix();
}

void Graphic3DSetOrthographicProjection()
{
	//Switch to projection mode
	glMatrixMode(GL_PROJECTION);

	//Save previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();

	//Reset matrix
	glLoadIdentity();

	//Set a 2D orthographic projection
	gluOrtho2D(0, graphic3d->width, graphic3d->height, 0);

	//Switch back to modelview mode
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}
void Graphic3DRestorePerspectiveProjection()
{
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);

	//Restore previous projection matrix
	glPopMatrix();

	//Get back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}
void Graphic3DRenderBitmapString(double x, double y, char* string)
{
  for (char* c = string; *c != '\0'; c++) 
  {
	if (*c != ' ') glRasterPos2f(x, y);
	if (*c != ' ') glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	x += 8;
  }
}
bool Graphic3DIsInSight(double X, double Y, double Z, double radius)
{
	/*if (X - radius > graphic3d->posX_plus_width_meters_div_2  ) return false;
	if (X + radius < graphic3d->posX_minus_width_meters_div_2 ) return false;
	if (Y - radius > graphic3d->posY_plus_height_meters_div_2 ) return false;
	if (Y + radius < graphic3d->posY_minus_height_meters_div_2) return false;*/
	return true;
}
