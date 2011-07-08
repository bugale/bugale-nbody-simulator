#define Data_included
#include "..\SharedHeader.h"

using namespace std;

class Data
{
public:
	int width;
	int height;
	bool two_dimensional_calculation;
	bool two_dimensional_graphic;
	bool two_dimensional_binary;
	bool fullscreen;
	bool log;
	char algorithm;
	double graphic_max_rate;
	double binary_max_rate;
	long long max_calculations;
	int max_trails;

	//Graphic output related
    int sphere_slices;
    int sphere_stacks;
	double field_of_view;
	double near_plane_distance;
	double far_plane_distance;
    double camera_positionX;
    double camera_positionY;
    double camera_positionZ;
    double camera_targetX;
    double camera_targetY;
    double camera_targetZ;
    double camera_upX;
    double camera_upY;
    double camera_upZ;
    double keyboard_move_speed0; //The starting move speed of the screen in screens per second
	double keyboard_move_speed1; //The move speed of the screen after one second in screens per second
	double keyboard_zoom_speed0; //The starting zoom speed 
	double keyboard_zoom_speed1; //The zoom speed after one second

	//Body Related
	int num_of_bodies;
	double g;
	double dt;
	Body3D* bodies;

	Data(char* settings_filename, char* bodies_filename);

private:
	void parseSettings(char* filename);
	void parseBodies(char* filename);
	void readBody(unsigned char* data, int byte_index, Body3D* bodies, int body_index);
	bool readBool(unsigned char* data, int byte_index, int bit_index);
	unsigned char* readData(char* filename);
};
