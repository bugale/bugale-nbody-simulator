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
/*WARNING: use only one type of dimension(2d or 3d) and only one type of integration(ModifiedEuler, hermite, etc.) for each intance of this class*/

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
	double initial_momentum_sum_2d;
	double initial_momentum_sum_3d;
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

	//Momentum calculations
	double GetMomentumSum2D();
	double GetMomentumSum3D();
	double GetMomentumError2D();
	double GetMomentumError3D();
	
	//Integration calculations
	void NextFrameModifiedEuler2D();
	void NextFrameModifiedEuler3D();
	void NextFrameHermite2D();
	void NextFrameHermite3D();
	void NextFrameHermite2DThreadInitializeFrame(int thread, int total);
	void NextFrameHermite2DThreadRun(int thread, int total);

	void InitializeHermite2D();
	void InitializeHermite3D();

	static void Hermite2DThread(Engine* engine, int thread, int total, volatile bool* done, bool* exit);

	static void Hermite2DThreaded(Engine* engine, SharedData* shared, int threads);
};
