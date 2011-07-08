/*WARNING: use only one type of dimension(2d or 3d) and only one type of integration(leapfrog, hermite, etc.) for each intance of this class*/

#define Engine_included
#include "..\SharedHeader.h"

class Engine
{
public:
	//Input
	Body3D* bodies;
	int num_of_bodies;
	double dt;
	double g;
	
	//Logic
	double initial_energy_sum_2d;
	double initial_energy_sum_3d;
	bool first_hermite;

	//Precalculations
	double* dt_div_mass; //dt / mass   for each mass
	double* dt_div_2mass; //dt / (2*mass)   for each mass
	double* dt_squared_div_2mass; //dt^2 / (2*mass)   for each mass
	double* dt_squared_div_12mass; //dt^2 / (12*mass)   for each mass
	double* dt_pow_3_div_6mass; //dt^3 / (6*mass)   for each mass
	double* mass_mul_g; //G * mass   for each mass
	double** mass1_mul_mass2_mul_g; //G * m1 * m2
	double dt_pow_3_div_6; //dt^3 / 6
	double dt_squared_div_2; //dt^2 / 2
	double dt_squared_div_12; //dt^2 / 12
	double dt_div_2; //dt / 2
	double dt_div_12; //dt / 12
	double dt_squared_mul_g_div_12; //dt^2 * G / 12
	
	//Initialization
	Engine(Data* data);
	void Precalculations(); //Precalculate some constant expressions which are commonly used in the algorithms

	//Energy calculations
	double GetEnergySum2D();
	double GetEnergySum3D();
	double GetEnergyError2D();
	double GetEnergyError3D();
	
	//Integration calculations
	void NextFrameLeapfrog2D();
	void NextFrameLeapfrog3D();
	void NextFrameHermite2D();
	void NextFrameHermite3D();

	void InitializeHermite2D();
	void InitializeHermite3D();

};
