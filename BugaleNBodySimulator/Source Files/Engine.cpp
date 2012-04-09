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

Engine::Engine(Data* data)
{
	log_line("Entered Engine constructor with data at 0x%08X.", data);
	this->bodies = data->bodies;
	this->num_of_bodies = data->num_of_bodies;
	this->dt = data->dt;;
	this->g = data->g;
	this->Precalculations();

	this->initial_energy_sum_2d = this->GetEnergySum2D();
	this->initial_energy_sum_3d = this->GetEnergySum3D();
	this->initial_momentum_sum_2d = this->GetMomentumSum2D();
	this->initial_momentum_sum_3d = this->GetMomentumSum3D();
	this->first_hermite = true;
	log_line("Ended Engine constructor with this at 0x%08X.", this);
}
Engine::~Engine()
{
	for (int i = 0; i < this->num_of_bodies; i++)
		if (this->mass1_mul_mass2_mul_g[i] != 0) free(this->mass1_mul_mass2_mul_g[i]);
	if (this->dt_div_mass != 0) free(this->dt_div_mass);
	if (this->dt_div_2mass != 0) free(this->dt_div_2mass);
	if (this->mass_mul_g != 0) free(this->mass_mul_g);
	if (this->dt_squared_div_2mass != 0) free(this->dt_squared_div_2mass);
	if (this->dt_squared_div_12mass != 0) free(this->dt_squared_div_12mass);
	if (this->dt_pow_3_div_6mass != 0) free(this->dt_pow_3_div_6mass);
	if (this->mass1_mul_mass2_mul_g != 0) free(this->mass1_mul_mass2_mul_g);
}

void Engine::Precalculations()
{
	log_line("Entered Engine::Precalculations.");
	this->dt_div_2 = this->dt / 2;
	this->dt_div_12 = this->dt / 12;
	this->dt_squared_mul_g_div_12 = this->g * this->dt * this->dt / 12;
	this->dt_pow_3_div_6 = this->dt * this->dt * this->dt / 6;
	this->dt_squared_div_2 = this->dt * this->dt / 2;
	this->dt_squared_div_12 = this->dt * this->dt / 12;
	this->dt_div_mass = (double*)malloc(sizeof(double) * this->num_of_bodies);
	this->dt_div_2mass = (double*)malloc(sizeof(double) * this->num_of_bodies);
	this->mass_mul_g = (double*)malloc(sizeof(double) * this->num_of_bodies);
	this->dt_squared_div_2mass = (double*)malloc(sizeof(double) * this->num_of_bodies);
	this->dt_squared_div_12mass = (double*)malloc(sizeof(double) * this->num_of_bodies);
	this->dt_pow_3_div_6mass = (double*)malloc(sizeof(double) * this->num_of_bodies);
	this->mass1_mul_mass2_mul_g = (double**)malloc(sizeof(double*) * this->num_of_bodies);
	for (int i = 0; i < this->num_of_bodies; i++)
	{
		this->dt_div_mass[i] = this->dt / this->bodies[i]._mass;
		this->dt_div_2mass[i] = this->dt / (2 * this->bodies[i]._mass);
		this->mass_mul_g[i] = this->bodies[i]._mass * this->g;
		this->dt_squared_div_2mass[i] = this->dt * this->dt / (2 * this->bodies[i]._mass);
		this->dt_squared_div_12mass[i] = this->dt * this->dt / (12 * this->bodies[i]._mass);
		this->dt_pow_3_div_6mass[i] = this->dt * this->dt * this->dt / (6 * this->bodies[i]._mass);
		this->mass1_mul_mass2_mul_g[i] = (double*)malloc(sizeof(double) * this->num_of_bodies);
		for (int j = 0; j < this->num_of_bodies; j++) this->mass1_mul_mass2_mul_g[i][j] = this->g * this->bodies[i]._mass * this->bodies[j]._mass;
	}
	log_line("Ended Engine::Precalculations(1/4) with dt_div_2 as %d dt_div_12 as %d dt_squared_mul_g_div_12 as %d and dt_pow_3_div_6 as %d.", this->dt_div_2, this->dt_div_12, this->dt_squared_mul_g_div_12, this->dt_pow_3_div_6);
	log_line("Ended Engine::Precalculations(2/4) with dt_squared_div_2 as %d dt_squared_div_12 as %d dt_div_mass at 0x%08X and dt_div_2mass at 0x%08X.", this->dt_squared_div_2, this->dt_squared_div_12, this->dt_div_mass, this->dt_div_2mass);
	log_line("Ended Engine::Precalculations(3/4) with mass_mul_g at 0x%08X dt_squared_div_2mass at 0x%08X dt_squared_div_12mass at 0x%08X and dt_pow_3_div_6mass at 0x%08X.", this->mass_mul_g, this->dt_squared_div_2mass, this->dt_squared_div_12mass, this->dt_pow_3_div_6mass);
	log_line("Ended Engine::Precalculations(4/4) with mass1_mul_mass2_mul_g at 0x%08X.", this->mass1_mul_mass2_mul_g);
}

double Engine::GetEnergySum2D()
{
	log_line("Entered Engine::GetEnergySum2D.");
	double energy_kin = 0;
	double energy_pot = 0;
	double dX, dY;
	double length;
	for (int i = 0; i < this->num_of_bodies - 1; i++)
	{
		for (int j = i + 1; j < this->num_of_bodies; j++)
		{
			dX = this->bodies[j]._positionX - this->bodies[i]._positionX;
            dY = this->bodies[j]._positionY - this->bodies[i]._positionY;
            length = sqrt((dX * dX) + (dY * dY));
			if (length != 0) energy_pot -= this->mass1_mul_mass2_mul_g[i][j] / length;
		}
		energy_kin += this->bodies[i]._mass * (this->bodies[i]._velocityX * this->bodies[i]._velocityX + this->bodies[i]._velocityY * this->bodies[i]._velocityY);
	}
	energy_kin /= 2;
	log_line("Ended Engine::GetEnergySum2D with result as %G.", energy_kin + energy_pot);
	return energy_kin + energy_pot;
}
double Engine::GetEnergySum3D()
{
	log_line("Entered Engine::GetEnergySum3D.");
	double energy_kin = 0;
	double energy_pot = 0;
	double dX, dY, dZ;
	double length;
	for (int i = 0; i < this->num_of_bodies - 1; i++)
	{
		for (int j = i + 1; j < this->num_of_bodies; j++)
		{
			dX = this->bodies[j]._positionX - this->bodies[i]._positionX;
            dY = this->bodies[j]._positionY - this->bodies[i]._positionY;
			dZ = this->bodies[j]._positionZ - this->bodies[i]._positionZ;
            length = sqrt((dX * dX) + (dY * dY) + (dZ * dZ));
			if (length != 0) energy_pot -= this->mass1_mul_mass2_mul_g[i][j] / length;
		}
		energy_kin += this->bodies[i]._mass * (this->bodies[i]._velocityX * this->bodies[i]._velocityX + this->bodies[i]._velocityY * this->bodies[i]._velocityY + this->bodies[i]._velocityZ * this->bodies[i]._velocityZ);
	}
	energy_kin /= 2;
	log_line("Ended Engine::GetEnergySum3D with result as %G.", energy_kin + energy_pot);
	return energy_kin + energy_pot;
}
double Engine::GetEnergyError2D()
{
	return (this->GetEnergySum2D() - this->initial_energy_sum_2d) / this->initial_energy_sum_2d;
}
double Engine::GetEnergyError3D()
{
	return (this->GetEnergySum3D() - this->initial_energy_sum_3d) / this->initial_energy_sum_3d;
}

double Engine::GetMomentumSum2D()
{
	log_line("Entered Engine::GetMomentumSum2D.");
	double momentumX = 0;
	double momentumY = 0;
	double ret;
	for (int i = 0; i < this->num_of_bodies; i++)
	{
		momentumX += this->bodies[i]._mass * this->bodies[i]._velocityX;
		momentumY += this->bodies[i]._mass * this->bodies[i]._velocityY;
	}
	ret = sqrt(momentumX*momentumX + momentumY*momentumY);
	log_line("Ended Engine::GetMomentumSum2D with result as %G.", ret);
	return ret;
}
double Engine::GetMomentumSum3D()
{
	log_line("Entered Engine::GetMomentumSum3D.");
	double momentumX = 0;
	double momentumY = 0;
	double momentumZ = 0;
	double ret;
	for (int i = 0; i < this->num_of_bodies; i++)
	{
		momentumX += this->bodies[i]._mass * this->bodies[i]._velocityX;
		momentumY += this->bodies[i]._mass * this->bodies[i]._velocityY;
		momentumZ += this->bodies[i]._mass * this->bodies[i]._velocityZ;
	}
	ret = sqrt(momentumX*momentumX + momentumY*momentumY + momentumZ*momentumZ);
	log_line("Ended Engine::GetMomentumSum3D with result as %G.", ret);
	return ret;
}
double Engine::GetMomentumError2D()
{
	return (this->GetMomentumSum2D() - this->initial_momentum_sum_2d) / this->initial_momentum_sum_2d;
}
double Engine::GetMomentumError3D()
{
	return (this->GetMomentumSum3D() - this->initial_momentum_sum_3d) / this->initial_momentum_sum_3d;
}

void Engine::NextFrameModifiedEuler2D()
{
    double dRX, dRY; //used to find the connecting vector between two bodies. The Rji vector.
    double length_pow_3; //The third power of the length of the Rji vector.
    double force_div_length; //The force(newtons) of gravitation between the two bodies, divided by the length of the Rji vector. The Rji vector is multiplied by this value to generate the force vector.
    for (int i = 0; i < this->num_of_bodies - 1; i++)
        for (int j = i + 1; j < this->num_of_bodies; j++) //Run on every two bodies
        {
            //Generate Rji vector
            dRX = this->bodies[j]._positionX - this->bodies[i]._positionX;
            dRY = this->bodies[j]._positionY - this->bodies[i]._positionY;

            //Generate force size
            length_pow_3 = (dRX * dRX) + (dRY * dRY);
            length_pow_3 = length_pow_3 * sqrt(length_pow_3);
            if (length_pow_3 == 0) force_div_length = 0;
            else                   force_div_length = this->mass1_mul_mass2_mul_g[i][j] / length_pow_3;

            //Generate the force vector. Save it instead of the Rji vector
            dRX *= force_div_length;
            dRY *= force_div_length;

            //Add the force vector to the bodies
            this->bodies[i]._forceX += dRX; this->bodies[i]._forceY += dRY;
            this->bodies[j]._forceX -= dRX; this->bodies[j]._forceY -= dRY;
        }
    for (int i = 0; i < this->num_of_bodies; i++) //Calculate the new position and velocity of every body.
    {
        //Calculate the new position of the body
        this->bodies[i]._positionX += this->bodies[i]._velocityX * this->dt + this->bodies[i]._forceX * this->dt_squared_div_2mass[i];
        this->bodies[i]._positionY += this->bodies[i]._velocityY * this->dt + this->bodies[i]._forceY * this->dt_squared_div_2mass[i];

        //Calculate the new velocity of the body
        this->bodies[i]._velocityX += this->bodies[i]._forceX * this->dt_div_mass[i];
        this->bodies[i]._velocityY += this->bodies[i]._forceY * this->dt_div_mass[i];

        //Zeroing Forces
        this->bodies[i]._forceX = 0; this->bodies[i]._forceY = 0;
    }
}
void Engine::NextFrameModifiedEuler3D()
{
    double dRX, dRY, dRZ; //used to find the connecting vector between two bodies. The Rji vector.
    double length_pow_3; //The third power of the length of the Rji vector.
    double force_div_length; //The force(newtons) of gravitation between the two bodies, divided by the length of the Rji vector. The Rji vector is multiplied by this value to generate the force vector.
    for (int i = 0; i < this->num_of_bodies - 1; i++)
        for (int j = i + 1; j < this->num_of_bodies; j++) //Run on every two bodies
        {
            //Generate Rji vector
            dRX = this->bodies[j]._positionX - this->bodies[i]._positionX;
            dRY = this->bodies[j]._positionY - this->bodies[i]._positionY;
			dRZ = this->bodies[j]._positionZ - this->bodies[i]._positionZ;

            //Generate force size
            length_pow_3 = (dRX * dRX) + (dRY * dRY) + (dRZ * dRZ); //Length squared
            length_pow_3 = length_pow_3 * sqrt(length_pow_3);
            if (length_pow_3 == 0) force_div_length = 0;
            else                   force_div_length = this->mass1_mul_mass2_mul_g[i][j] / length_pow_3;

            //Generate the force vector. Save it instead of the Rji vector
            dRX *= force_div_length;
            dRY *= force_div_length;
			dRZ *= force_div_length;

            //Add the force vector to the bodies
            this->bodies[i]._forceX += dRX; this->bodies[i]._forceY += dRY; this->bodies[i]._forceZ += dRZ;
            this->bodies[j]._forceX -= dRX; this->bodies[j]._forceY -= dRY; this->bodies[j]._forceZ -= dRZ;
        }
    for (int i = 0; i < this->num_of_bodies; i++) //Calculate the new position and velocity of every body.
    {
        //Calculate the new position of the body
        this->bodies[i]._positionX += this->bodies[i]._velocityX * this->dt + this->bodies[i]._forceX * this->dt_squared_div_2mass[i];
        this->bodies[i]._positionY += this->bodies[i]._velocityY * this->dt + this->bodies[i]._forceY * this->dt_squared_div_2mass[i];
		this->bodies[i]._positionZ += this->bodies[i]._velocityZ * this->dt + this->bodies[i]._forceZ * this->dt_squared_div_2mass[i];

        //Calculate the new velocity of the body
        this->bodies[i]._velocityX += this->bodies[i]._forceX * this->dt_div_mass[i];
        this->bodies[i]._velocityY += this->bodies[i]._forceY * this->dt_div_mass[i];
		this->bodies[i]._velocityZ += this->bodies[i]._forceZ * this->dt_div_mass[i];

        //Zeroing Forces
        this->bodies[i]._forceX = 0; this->bodies[i]._forceY = 0; this->bodies[i]._forceZ = 0;
    }
}
void Engine::NextFrameHermite2D()
{
	if (this->first_hermite)
	{
		this->InitializeHermite2D();
		return;
	}

    double dRX, dRY; //used to find the connecting vector between two bodies. The Rji vector.
	double dVX, dVY; //used to find the Vji for the hermite formula.
	double dRdV_mul_three_div_length_pow_2; //(dR dot-multiply dV) multiplied by the minus-second(-2) power of the length of the Rji vector multiplied by 3 for the hermite formula.
	double inv_length_pow_2; //The minus-second(-2) power of the length of the Rji vector.
    double inv_length_pow_3; //The minus-third(-3) power of the length of the Rji vector.
    double force_div_length; //The force(newtons) of gravitation between the two bodies, divided by the length of the Rji vector. The Rji vector is multiplied by this value to generate the force vector.
	double jerk_sharedX, jerk_sharedY; //A part of the jerk formula
	for (int i = 0; i < this->num_of_bodies; i++) //Initialize current calculation
	{
		//Calculate the new position and velocity of every body.
		this->bodies[i]._velocityX = this->bodies[i]._l_velocityX + (this->bodies[i]._l_forceX    + this->bodies[i]._forceX   ) * this->dt_div_2mass[i] + (this->bodies[i]._l_jerkX  - this->bodies[i]._jerkX ) * this->dt_squared_div_12mass[i];
		this->bodies[i]._velocityY = this->bodies[i]._l_velocityY + (this->bodies[i]._l_forceY    + this->bodies[i]._forceY   ) * this->dt_div_2mass[i] + (this->bodies[i]._l_jerkY  - this->bodies[i]._jerkY ) * this->dt_squared_div_12mass[i];
		this->bodies[i]._positionX = this->bodies[i]._l_positionX + (this->bodies[i]._l_velocityX + this->bodies[i]._velocityX) * this->dt_div_2        + (this->bodies[i]._l_forceX - this->bodies[i]._forceX) * this->dt_squared_div_12mass[i];
		this->bodies[i]._positionY = this->bodies[i]._l_positionY + (this->bodies[i]._l_velocityY + this->bodies[i]._velocityY) * this->dt_div_2        + (this->bodies[i]._l_forceY - this->bodies[i]._forceY) * this->dt_squared_div_12mass[i];
		
		//Duplicate some data
		this->bodies[i]._l_forceX    = this->bodies[i]._forceX;
		this->bodies[i]._l_forceY    = this->bodies[i]._forceY;
		this->bodies[i]._l_jerkX     = this->bodies[i]._jerkX;
		this->bodies[i]._l_jerkY     = this->bodies[i]._jerkY;
		this->bodies[i]._l_positionX = this->bodies[i]._positionX;
		this->bodies[i]._l_positionY = this->bodies[i]._positionY;
		this->bodies[i]._l_velocityX = this->bodies[i]._velocityX;
		this->bodies[i]._l_velocityY = this->bodies[i]._velocityY;

		//Zeroing
		this->bodies[i]._forceX = 0; this->bodies[i]._forceY = 0;
		this->bodies[i]._jerkX  = 0; this->bodies[i]._jerkY  = 0;

		//Calculate pre-position and pre-velocity
		this->bodies[i]._t_positionX = this->bodies[i]._l_positionX + this->bodies[i]._l_velocityX * this->dt             + this->bodies[i]._l_forceX * this->dt_squared_div_2mass[i] + this->bodies[i]._l_jerkX * this->dt_pow_3_div_6mass[i];
		this->bodies[i]._t_positionY = this->bodies[i]._l_positionY + this->bodies[i]._l_velocityY * this->dt             + this->bodies[i]._l_forceY * this->dt_squared_div_2mass[i] + this->bodies[i]._l_jerkY * this->dt_pow_3_div_6mass[i];
		this->bodies[i]._t_velocityX = this->bodies[i]._l_velocityX + this->bodies[i]._l_forceX    * this->dt_div_mass[i] + this->bodies[i]._l_jerkX  * this->dt_squared_div_2mass[i];
		this->bodies[i]._t_velocityY = this->bodies[i]._l_velocityY + this->bodies[i]._l_forceY    * this->dt_div_mass[i] + this->bodies[i]._l_jerkY  * this->dt_squared_div_2mass[i];
	}
    for (int i = 0; i < this->num_of_bodies - 1; i++)
        for (int j = i + 1; j < this->num_of_bodies; j++) //Run on every two bodies
        {
            //Generate Rji and Vji vectors
            dRX = this->bodies[j]._t_positionX - this->bodies[i]._t_positionX;
            dRY = this->bodies[j]._t_positionY - this->bodies[i]._t_positionY;
            dVX = this->bodies[j]._t_velocityX - this->bodies[i]._t_velocityX;
            dVY = this->bodies[j]._t_velocityY - this->bodies[i]._t_velocityY;

            //Generate lengths - the distance between the bodies pow -2 and -3
            inv_length_pow_2 = (dRX * dRX) + (dRY * dRY);
			if (inv_length_pow_2 != 0) inv_length_pow_2 = 1 / inv_length_pow_2;
            inv_length_pow_3 = inv_length_pow_2 * sqrt(inv_length_pow_2);

			//Calculate the jerk and the force size
			force_div_length = this->mass1_mul_mass2_mul_g[i][j] * inv_length_pow_3;
			dRdV_mul_three_div_length_pow_2 = (dRX*dVX + dRY*dVY) * 3 * inv_length_pow_2;
			jerk_sharedX = (dVX - dRdV_mul_three_div_length_pow_2 * dRX) * force_div_length;
			jerk_sharedY = (dVY - dRdV_mul_three_div_length_pow_2 * dRY) * force_div_length;

			//Add the calculated jerk
			this->bodies[i]._jerkX += jerk_sharedX; this->bodies[i]._jerkY += jerk_sharedY;
			this->bodies[j]._jerkX -= jerk_sharedX;	this->bodies[j]._jerkY -= jerk_sharedY;

			//Generate the force vector and save it instead of the Rji vector
            dRX *= force_div_length;
            dRY *= force_div_length;

            //Add the force vector to the bodies
            this->bodies[i]._forceX += dRX; this->bodies[i]._forceY += dRY;
            this->bodies[j]._forceX -= dRX; this->bodies[j]._forceY -= dRY;
        }
}
void Engine::NextFrameHermite3D()
{
	if (this->first_hermite) 
	{
			this->InitializeHermite3D();
			return;
	}

    double dRX, dRY, dRZ; //used to find the connecting vector between two bodies. The Rji vector.
	double dVX, dVY, dVZ; //used to find the Vji for the hermite formula.
	double dRdV_mul_three_div_length_pow_2; //(dR dot-multiply dV) multiplied by the minus-second(-2) power of the length of the Rji vector multiplied by 3 for the hermite formula.
	double inv_length_pow_2; //The minus-second(-2) power of the length of the Rji vector.
    double inv_length_pow_3; //The minus-third(-3) power of the length of the Rji vector.
    double force_div_length; //The force(newtons) of gravitation between the two bodies, divided by the length of the Rji vector. The Rji vector is multiplied by this value to generate the force vector.
	double jerk_sharedX, jerk_sharedY, jerk_sharedZ; //A part of the jerk formula
	for (int i = 0; i < this->num_of_bodies; i++) //Initialize current calculation
	{
		//Calculate the new position and velocity of every body.
		this->bodies[i]._velocityX = this->bodies[i]._l_velocityX + (this->bodies[i]._l_forceX    + this->bodies[i]._forceX   ) * this->dt_div_2mass[i] + (this->bodies[i]._l_jerkX  - this->bodies[i]._jerkX ) * this->dt_squared_div_12mass[i];
		this->bodies[i]._velocityY = this->bodies[i]._l_velocityY + (this->bodies[i]._l_forceY    + this->bodies[i]._forceY   ) * this->dt_div_2mass[i] + (this->bodies[i]._l_jerkY  - this->bodies[i]._jerkY ) * this->dt_squared_div_12mass[i];
		this->bodies[i]._velocityZ = this->bodies[i]._l_velocityZ + (this->bodies[i]._l_forceZ    + this->bodies[i]._forceZ   ) * this->dt_div_2mass[i] + (this->bodies[i]._l_jerkZ  - this->bodies[i]._jerkZ ) * this->dt_squared_div_12mass[i];
		this->bodies[i]._positionX = this->bodies[i]._l_positionX + (this->bodies[i]._l_velocityX + this->bodies[i]._velocityX) * this->dt_div_2        + (this->bodies[i]._l_forceX - this->bodies[i]._forceX) * this->dt_squared_div_12mass[i];
		this->bodies[i]._positionY = this->bodies[i]._l_positionY + (this->bodies[i]._l_velocityY + this->bodies[i]._velocityY) * this->dt_div_2        + (this->bodies[i]._l_forceY - this->bodies[i]._forceY) * this->dt_squared_div_12mass[i];
		this->bodies[i]._positionZ = this->bodies[i]._l_positionZ + (this->bodies[i]._l_velocityZ + this->bodies[i]._velocityZ) * this->dt_div_2        + (this->bodies[i]._l_forceZ - this->bodies[i]._forceZ) * this->dt_squared_div_12mass[i];

		//Duplicate some data
		this->bodies[i]._l_forceX    = this->bodies[i]._forceX;
		this->bodies[i]._l_forceY    = this->bodies[i]._forceY;
		this->bodies[i]._l_forceZ    = this->bodies[i]._forceZ;
		this->bodies[i]._l_jerkX     = this->bodies[i]._jerkX;
		this->bodies[i]._l_jerkY     = this->bodies[i]._jerkY;
		this->bodies[i]._l_jerkZ     = this->bodies[i]._jerkZ;
		this->bodies[i]._l_positionX = this->bodies[i]._positionX;
		this->bodies[i]._l_positionY = this->bodies[i]._positionY;
		this->bodies[i]._l_positionZ = this->bodies[i]._positionZ;
		this->bodies[i]._l_velocityX = this->bodies[i]._velocityX;
		this->bodies[i]._l_velocityY = this->bodies[i]._velocityY;
		this->bodies[i]._l_velocityZ = this->bodies[i]._velocityZ;
		
		//Zeroing
		this->bodies[i]._forceX = 0; this->bodies[i]._forceY = 0; this->bodies[i]._forceZ = 0;
		this->bodies[i]._jerkX  = 0; this->bodies[i]._jerkY  = 0; this->bodies[i]._jerkZ  = 0;

		//Calculate pre-position and pre-velocity
		this->bodies[i]._t_positionX = this->bodies[i]._l_positionX + this->bodies[i]._l_velocityX * this->dt             + this->bodies[i]._l_forceX * this->dt_squared_div_2mass[i] + this->bodies[i]._l_jerkX * this->dt_pow_3_div_6mass[i];
		this->bodies[i]._t_positionY = this->bodies[i]._l_positionY + this->bodies[i]._l_velocityY * this->dt             + this->bodies[i]._l_forceY * this->dt_squared_div_2mass[i] + this->bodies[i]._l_jerkY * this->dt_pow_3_div_6mass[i];
		this->bodies[i]._t_positionZ = this->bodies[i]._l_positionZ + this->bodies[i]._l_velocityZ * this->dt             + this->bodies[i]._l_forceZ * this->dt_squared_div_2mass[i] + this->bodies[i]._l_jerkZ * this->dt_pow_3_div_6mass[i];
		this->bodies[i]._t_velocityX = this->bodies[i]._l_velocityX + this->bodies[i]._l_forceX    * this->dt_div_mass[i] + this->bodies[i]._l_jerkX  * this->dt_squared_div_2mass[i];
		this->bodies[i]._t_velocityY = this->bodies[i]._l_velocityY + this->bodies[i]._l_forceY    * this->dt_div_mass[i] + this->bodies[i]._l_jerkY  * this->dt_squared_div_2mass[i];
		this->bodies[i]._t_velocityZ = this->bodies[i]._l_velocityZ + this->bodies[i]._l_forceZ    * this->dt_div_mass[i] + this->bodies[i]._l_jerkZ  * this->dt_squared_div_2mass[i];
	}
    for (int i = 0; i < this->num_of_bodies - 1; i++)
        for (int j = i + 1; j < this->num_of_bodies; j++) //Run on every two bodies
        {
            //Generate Rji and Vji vectors
            dRX = this->bodies[j]._t_positionX - this->bodies[i]._t_positionX;
            dRY = this->bodies[j]._t_positionY - this->bodies[i]._t_positionY;
			dRZ = this->bodies[j]._t_positionZ - this->bodies[i]._t_positionZ;
            dVX = this->bodies[j]._t_velocityX - this->bodies[i]._t_velocityX;
            dVY = this->bodies[j]._t_velocityY - this->bodies[i]._t_velocityY;
            dVZ = this->bodies[j]._t_velocityZ - this->bodies[i]._t_velocityZ;

            //Generate lengths - the distance between the bodies pow -2 and -3
            inv_length_pow_2 = (dRX * dRX) + (dRY * dRY) + (dRZ * dRZ);
			if (inv_length_pow_2 != 0) inv_length_pow_2 = 1 / inv_length_pow_2;
            inv_length_pow_3 = inv_length_pow_2 * sqrt(inv_length_pow_2);

			//Calculate the jerk (without G and mass) and force size
			force_div_length = this->mass1_mul_mass2_mul_g[i][j] * inv_length_pow_3;
			dRdV_mul_three_div_length_pow_2 = (dRX*dVX + dRY*dVY + dRZ*dVZ) * 3 * inv_length_pow_2;
			jerk_sharedX = (dVX - dRdV_mul_three_div_length_pow_2 * dRX) * force_div_length;
			jerk_sharedY = (dVY - dRdV_mul_three_div_length_pow_2 * dRY) * force_div_length;
			jerk_sharedZ = (dVZ - dRdV_mul_three_div_length_pow_2 * dRZ) * force_div_length;

			//Add the calculated jerk
			this->bodies[i]._jerkX += jerk_sharedX;	this->bodies[i]._jerkY += jerk_sharedY;	this->bodies[i]._jerkZ += jerk_sharedZ;
			this->bodies[j]._jerkX -= jerk_sharedX;	this->bodies[j]._jerkY -= jerk_sharedY;	this->bodies[j]._jerkZ -= jerk_sharedZ;

			//Generate force vector and save it instead of the Rji vector
            dRX *= force_div_length;
            dRY *= force_div_length;
			dRZ *= force_div_length;

            //Add the force vector to the bodies
            this->bodies[i]._forceX += dRX; this->bodies[i]._forceY += dRY; this->bodies[i]._forceZ += dRZ;
            this->bodies[j]._forceX -= dRX; this->bodies[j]._forceY -= dRY; this->bodies[j]._forceZ -= dRZ;
        }
}

void Engine::InitializeHermite2D()
{
	log_line("Entered Engine::InitializeHermite2D.");
	double dRX, dRY; //used to find the connecting vector between two bodies. The Rji vector.
	double dVX, dVY; //used to find the Vji for the hermite formula.
	double dRdV_mul_three_div_length_pow_2; //(dR dot-multiply dV) multiplied by the minus-second(-2) power of the length of the Rji vector multiplied by 3 for the hermite formula.
	double inv_length_pow_2; //The minus-second(-2) power of the length of the Rji vector.
    double inv_length_pow_3; //The minus-third(-3) power of the length of the Rji vector.
    double force_div_length; //The force(newtons) of gravitation between the two bodies, divided by the length of the Rji vector. The Rji vector is multiplied by this value to generate the force vector.
	double jerk_sharedX, jerk_sharedY; //A part of the jerk formula
	for (int i = 0; i < this->num_of_bodies - 1; i++)
        for (int j = i + 1; j < this->num_of_bodies; j++) //Run on every two bodies
        {
            //Generate Rji and Vji vectors
            dRX = this->bodies[j]._positionX - this->bodies[i]._positionX;
            dRY = this->bodies[j]._positionY - this->bodies[i]._positionY;
            dVX = this->bodies[j]._velocityX - this->bodies[i]._velocityX;
            dVY = this->bodies[j]._velocityY - this->bodies[i]._velocityY;

            //Generate lengths - the distance between the bodies pow -2 and -3
            inv_length_pow_2 = (dRX * dRX) + (dRY * dRY);
			if (inv_length_pow_2 != 0) inv_length_pow_2 = 1 / inv_length_pow_2;
            inv_length_pow_3 = inv_length_pow_2 * sqrt(inv_length_pow_2);

			//Calculate the jerk (without G and mass) and the force size
			force_div_length = this->mass1_mul_mass2_mul_g[i][j] * inv_length_pow_3;
			dRdV_mul_three_div_length_pow_2 = (dRX*dVX + dRY*dVY) * 3 * inv_length_pow_2;
			jerk_sharedX = (dVX - dRdV_mul_three_div_length_pow_2 * dRX) * force_div_length;
			jerk_sharedY = (dVY - dRdV_mul_three_div_length_pow_2 * dRY) * force_div_length;

			//Add the calculated jerk
			this->bodies[i]._jerkX += jerk_sharedX; this->bodies[i]._jerkY += jerk_sharedY;
			this->bodies[j]._jerkX -= jerk_sharedX;	this->bodies[j]._jerkY -= jerk_sharedY;

			//Generate the force vector and save it instead of the Rij vector
            dRX *= force_div_length;
            dRY *= force_div_length;

            //Add the force vector to the bodies
            this->bodies[i]._forceX += dRX; this->bodies[i]._forceY += dRY;
            this->bodies[j]._forceX -= dRX; this->bodies[j]._forceY -= dRY;
        }
	for (int i = 0; i < this->num_of_bodies; i++) //Initialize current calculation
	{
		//Duplicate some data
		this->bodies[i]._l_forceX    = this->bodies[i]._forceX;
		this->bodies[i]._l_forceY    = this->bodies[i]._forceY;
		this->bodies[i]._l_jerkX     = this->bodies[i]._jerkX;
		this->bodies[i]._l_jerkY     = this->bodies[i]._jerkY;
		this->bodies[i]._l_positionX = this->bodies[i]._positionX;
		this->bodies[i]._l_positionY = this->bodies[i]._positionY;
		this->bodies[i]._l_velocityX = this->bodies[i]._velocityX;
		this->bodies[i]._l_velocityY = this->bodies[i]._velocityY;
		
		//Zeroing
		this->bodies[i]._forceX = 0; this->bodies[i]._forceY = 0;
		this->bodies[i]._jerkX  = 0; this->bodies[i]._jerkY  = 0;

		//Calculate pre-position and pre-velocity
		this->bodies[i]._t_positionX = this->bodies[i]._l_positionX + this->bodies[i]._l_velocityX * this->dt             + this->bodies[i]._l_forceX * this->dt_squared_div_2mass[i] + this->bodies[i]._l_jerkX * this->dt_pow_3_div_6mass[i];
		this->bodies[i]._t_positionY = this->bodies[i]._l_positionY + this->bodies[i]._l_velocityY * this->dt             + this->bodies[i]._l_forceY * this->dt_squared_div_2mass[i] + this->bodies[i]._l_jerkY * this->dt_pow_3_div_6mass[i];
		this->bodies[i]._t_velocityX = this->bodies[i]._l_velocityX + this->bodies[i]._l_forceX    * this->dt_div_mass[i] + this->bodies[i]._l_jerkX  * this->dt_squared_div_2mass[i];
		this->bodies[i]._t_velocityY = this->bodies[i]._l_velocityY + this->bodies[i]._l_forceY    * this->dt_div_mass[i] + this->bodies[i]._l_jerkY  * this->dt_squared_div_2mass[i];
	}
    for (int i = 0; i < this->num_of_bodies - 1; i++)
        for (int j = i + 1; j < this->num_of_bodies; j++) //Run on every two bodies
        {
            //Generate Rji and Vji vectors
            dRX = this->bodies[j]._t_positionX - this->bodies[i]._t_positionX;
            dRY = this->bodies[j]._t_positionY - this->bodies[i]._t_positionY;
            dVX = this->bodies[j]._t_velocityX - this->bodies[i]._t_velocityX;
            dVY = this->bodies[j]._t_velocityY - this->bodies[i]._t_velocityY;

            //Generate lengths - the distance between the bodies pow -2 and -3
            inv_length_pow_2 = (dRX * dRX) + (dRY * dRY);
			if (inv_length_pow_2 != 0) inv_length_pow_2 = 1 / inv_length_pow_2;
            inv_length_pow_3 = inv_length_pow_2 * sqrt(inv_length_pow_2);

			//Calculate the jerk and the force size
			force_div_length = this->mass1_mul_mass2_mul_g[i][j] * inv_length_pow_3;
			dRdV_mul_three_div_length_pow_2 = (dRX*dVX + dRY*dVY) * 3 * inv_length_pow_2;
			jerk_sharedX = (dVX - dRdV_mul_three_div_length_pow_2 * dRX) * force_div_length;
			jerk_sharedY = (dVY - dRdV_mul_three_div_length_pow_2 * dRY) * force_div_length;

			//Add the calculated jerk
			this->bodies[i]._jerkX += jerk_sharedX; this->bodies[i]._jerkY += jerk_sharedY;
			this->bodies[j]._jerkX -= jerk_sharedX;	this->bodies[j]._jerkY -= jerk_sharedY;

			//Generate the force vector and save it instead of the Rij vector
            dRX *= force_div_length;
            dRY *= force_div_length;

            //Add the force vector to the bodies
            this->bodies[i]._forceX += dRX; this->bodies[i]._forceY += dRY;
            this->bodies[j]._forceX -= dRX; this->bodies[j]._forceY -= dRY;
        }
	this->first_hermite = false;
	log_line("Ended Engine::InitializeHermite2D.");
}
void Engine::InitializeHermite3D()
{
	log_line("Entered Engine::InitializeHermite3D.");
	double dRX, dRY, dRZ; //used to find the connecting vector between two bodies. The Rji vector.
	double dVX, dVY, dVZ; //used to find the Vji for the hermite formula.
	double dRdV_mul_three_div_length_pow_2; //(dR dot-multiply dV) multiplied by the minus-second(-2) power of the length of the Rji vector multiplied by 3 for the hermite formula.
	double inv_length_pow_2; //The minus-second(-2) power of the length of the Rji vector.
    double inv_length_pow_3; //The minus-third(-3) power of the length of the Rji vector.
    double force_div_length; //The force(newtons) of gravitation between the two bodies, divided by the length of the Rji vector. The Rji vector is multiplied by this value to generate the force vector.
	double jerk_sharedX, jerk_sharedY, jerk_sharedZ; //A part of the jerk formula
	for (int i = 0; i < this->num_of_bodies - 1; i++)
        for (int j = i + 1; j < this->num_of_bodies; j++) //Run on every two bodies
        {
            //Generate Rji and Vji vectors
            dRX = this->bodies[j]._positionX - this->bodies[i]._positionX;
            dRY = this->bodies[j]._positionY - this->bodies[i]._positionY;
			dRZ = this->bodies[j]._positionZ - this->bodies[i]._positionZ;
            dVX = this->bodies[j]._velocityX - this->bodies[i]._velocityX;
            dVY = this->bodies[j]._velocityY - this->bodies[i]._velocityY;
            dVZ = this->bodies[j]._velocityZ - this->bodies[i]._velocityZ;

            //Generate lengths - the distance between the bodies pow -2 and -3
            inv_length_pow_2 = (dRX * dRX) + (dRY * dRY) + (dRZ * dRZ);
			if (inv_length_pow_2 != 0) inv_length_pow_2 = 1 / inv_length_pow_2;
            inv_length_pow_3 = inv_length_pow_2 * sqrt(inv_length_pow_2);

			//Calculate the jerk (without G and mass) and force size
			force_div_length = this->mass1_mul_mass2_mul_g[i][j] * inv_length_pow_3;
			dRdV_mul_three_div_length_pow_2 = (dRX*dVX + dRY*dVY + dRZ*dVZ) * 3 * inv_length_pow_2;
			jerk_sharedX = (dVX - dRdV_mul_three_div_length_pow_2 * dRX) * force_div_length;
			jerk_sharedY = (dVY - dRdV_mul_three_div_length_pow_2 * dRY) * force_div_length;
			jerk_sharedZ = (dVZ - dRdV_mul_three_div_length_pow_2 * dRZ) * force_div_length;

			//Add the calculated jerk
			this->bodies[i]._jerkX += jerk_sharedX;	this->bodies[i]._jerkY += jerk_sharedY;	this->bodies[i]._jerkZ += jerk_sharedZ;
			this->bodies[j]._jerkX -= jerk_sharedX;	this->bodies[j]._jerkY -= jerk_sharedY;	this->bodies[j]._jerkZ -= jerk_sharedZ;

			//Generate force vector and save it instead of the Rji vector
            dRX *= force_div_length;
            dRY *= force_div_length;
			dRZ *= force_div_length;

            //Add the force vector to the bodies
            this->bodies[i]._forceX += dRX; this->bodies[i]._forceY += dRY; this->bodies[i]._forceZ += dRZ;
            this->bodies[j]._forceX -= dRX; this->bodies[j]._forceY -= dRY; this->bodies[j]._forceZ -= dRZ;
        }
	for (int i = 0; i < this->num_of_bodies; i++) //Initialize current calculation
	{
		//Duplicate some data
		this->bodies[i]._l_forceX    = this->bodies[i]._forceX;
		this->bodies[i]._l_forceY    = this->bodies[i]._forceY;
		this->bodies[i]._l_forceZ    = this->bodies[i]._forceZ;
		this->bodies[i]._l_jerkX     = this->bodies[i]._jerkX;
		this->bodies[i]._l_jerkY     = this->bodies[i]._jerkY;
		this->bodies[i]._l_jerkZ     = this->bodies[i]._jerkZ;
		this->bodies[i]._l_positionX = this->bodies[i]._positionX;
		this->bodies[i]._l_positionY = this->bodies[i]._positionY;
		this->bodies[i]._l_positionZ = this->bodies[i]._positionZ;
		this->bodies[i]._l_velocityX = this->bodies[i]._velocityX;
		this->bodies[i]._l_velocityY = this->bodies[i]._velocityY;
		this->bodies[i]._l_velocityZ = this->bodies[i]._velocityZ;
		
		//Zeroing
		this->bodies[i]._forceX = 0; this->bodies[i]._forceY = 0; this->bodies[i]._forceZ = 0;
		this->bodies[i]._jerkX  = 0; this->bodies[i]._jerkY  = 0; this->bodies[i]._jerkZ  = 0;

		//Calculate pre-position and pre-velocity
		this->bodies[i]._t_positionX = this->bodies[i]._l_positionX + this->bodies[i]._l_velocityX * this->dt             + this->bodies[i]._l_forceX * this->dt_squared_div_2mass[i] + this->bodies[i]._l_jerkX * this->dt_pow_3_div_6mass[i];
		this->bodies[i]._t_positionY = this->bodies[i]._l_positionY + this->bodies[i]._l_velocityY * this->dt             + this->bodies[i]._l_forceY * this->dt_squared_div_2mass[i] + this->bodies[i]._l_jerkY * this->dt_pow_3_div_6mass[i];
		this->bodies[i]._t_positionZ = this->bodies[i]._l_positionZ + this->bodies[i]._l_velocityZ * this->dt             + this->bodies[i]._l_forceZ * this->dt_squared_div_2mass[i] + this->bodies[i]._l_jerkZ * this->dt_pow_3_div_6mass[i];
		this->bodies[i]._t_velocityX = this->bodies[i]._l_velocityX + this->bodies[i]._l_forceX    * this->dt_div_mass[i] + this->bodies[i]._l_jerkX  * this->dt_squared_div_2mass[i];
		this->bodies[i]._t_velocityY = this->bodies[i]._l_velocityY + this->bodies[i]._l_forceY    * this->dt_div_mass[i] + this->bodies[i]._l_jerkY  * this->dt_squared_div_2mass[i];
		this->bodies[i]._t_velocityZ = this->bodies[i]._l_velocityZ + this->bodies[i]._l_forceZ    * this->dt_div_mass[i] + this->bodies[i]._l_jerkZ  * this->dt_squared_div_2mass[i];
	}
    for (int i = 0; i < this->num_of_bodies - 1; i++)
        for (int j = i + 1; j < this->num_of_bodies; j++) //Run on every two bodies
        {
            //Generate Rji and Vji vectors
            dRX = this->bodies[j]._t_positionX - this->bodies[i]._t_positionX;
            dRY = this->bodies[j]._t_positionY - this->bodies[i]._t_positionY;
			dRZ = this->bodies[j]._t_positionZ - this->bodies[i]._t_positionZ;
            dVX = this->bodies[j]._t_velocityX - this->bodies[i]._t_velocityX;
            dVY = this->bodies[j]._t_velocityY - this->bodies[i]._t_velocityY;
            dVZ = this->bodies[j]._t_velocityZ - this->bodies[i]._t_velocityZ;

            //Generate lengths - the distance between the bodies pow -2 and -3
            inv_length_pow_2 = (dRX * dRX) + (dRY * dRY) + (dRZ * dRZ);
			if (inv_length_pow_2 != 0) inv_length_pow_2 = 1 / inv_length_pow_2;
            inv_length_pow_3 = inv_length_pow_2 * sqrt(inv_length_pow_2);

			//Calculate the jerk (without G and mass) and force size
			force_div_length = this->mass1_mul_mass2_mul_g[i][j] * inv_length_pow_3;
			dRdV_mul_three_div_length_pow_2 = (dRX*dVX + dRY*dVY + dRZ*dVZ) * 3 * inv_length_pow_2;
			jerk_sharedX = (dVX - dRdV_mul_three_div_length_pow_2 * dRX) * force_div_length;
			jerk_sharedY = (dVY - dRdV_mul_three_div_length_pow_2 * dRY) * force_div_length;
			jerk_sharedZ = (dVZ - dRdV_mul_three_div_length_pow_2 * dRZ) * force_div_length;

			//Add the calculated jerk
			this->bodies[i]._jerkX += jerk_sharedX;	this->bodies[i]._jerkY += jerk_sharedY;	this->bodies[i]._jerkZ += jerk_sharedZ;
			this->bodies[j]._jerkX -= jerk_sharedX;	this->bodies[j]._jerkY -= jerk_sharedY;	this->bodies[j]._jerkZ -= jerk_sharedZ;

			//Generate force vector and save it instead of the Rji vector
            dRX *= force_div_length;
            dRY *= force_div_length;
			dRZ *= force_div_length;

            //Add the force vector to the bodies
            this->bodies[i]._forceX += dRX; this->bodies[i]._forceY += dRY; this->bodies[i]._forceZ += dRZ;
            this->bodies[j]._forceX -= dRX; this->bodies[j]._forceY -= dRY; this->bodies[j]._forceZ -= dRZ;
        }
	this->first_hermite = false;
	log_line("Ended Engine::InitializeHermite3D.");
}