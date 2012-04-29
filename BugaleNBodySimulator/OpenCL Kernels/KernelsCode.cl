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

/*Notice: This file is only for reference, and should not be compiled!*/
/*Instead, it should be passed to the OpenCL JIT compiler at run-time*/

#ifdef cl_khr_fp64
    #pragma OPENCL EXTENSION cl_khr_fp64 : enable
#endif

#ifdef cl_amd_fp64
    #pragma OPENCL EXTENSION cl_amd_fp64 : enable
#endif

kernel void Euler3DKernel(
	global double* glob_posX, global double* glob_posY, global double* glob_posZ, global double* glob_velX, global double* glob_velY, global double* glob_velZ, global const double* glob_mass,
	global const double* dt_div_mass, global const double* mass_mul_g, global const double* dt_div_2mass, global const double* dt_sqaured_div_2mass, global const double* dt_squared_div_12mass, global const double* dt_pow_3_div_6mass,
	global double* out_posX, global double* out_posY, global double* out_posZ, global double* out_velX, global double* out_velY, global double* out_velZ,
	local double* loc_posX, local double* loc_posY, local double* loc_posZ, local double* loc_velX, local double* loc_velY, local double* loc_velZ,
	global double* l_velX, global double* l_velY, global double* l_velZ, global double* l_forceX, global double* l_forceY, global double* l_forceZ, global double* l_jerkX, global double* l_jerkY, global double* l_jerkZ)
{
	#pragma region Thread Statistics Variables
		int id = get_global_id(0); if (id >= %num_of_threads%) return;
		int loc_id = get_local_id(0);
	#pragma endregion
	#pragma region Loop Boundaries Variables
		int min_loop = id*%num_of_bodies%/%num_of_threads%;
		int max_loop = (id+1)*%num_of_bodies%/%num_of_threads%;
		int max_iterations = (%num_of_bodies%+%num_of_threads%-1)/%num_of_threads%;
		int min_cache; int max_cache;
		if (%num_of_bodies% < %cache_size%) {
			min_cache = loc_id*%num_of_bodies%/%workgroup_size%;
			max_cache = (loc_id+1)*%num_of_bodies%/%workgroup_size%;
		} else {
			min_cache = loc_id*%cache_size%/%workgroup_size%;
			max_cache = (loc_id+1)*%cache_size%/%workgroup_size%;
		}
		int cache_offset;
	#pragma endregion
	#pragma region Local Cache Variables
		double temp_posX, temp_posY, temp_posZ; //Position Cache
		double temp_velX, temp_velY, temp_velZ; //Velocity Cache
	#pragma endregion
	#pragma region Force Calculation Variables
		double forceX, forceY, forceZ;
		double dRX, dRY, dRZ; //used to find the connecting vector between two bodies. The Rji vector.
		double length_pow_2; //The inverse of the third power of the length of the Rji vector.
		double force_div_length; //The force(newtons) of gravitation between the two bodies, divided by the length of the Rji vector. The Rji vector is multiplied by this value to generate the force vector.
	#pragma endregion

	for (int k = 0; k < %calculations%; k++)
	{
		for (int i = min_loop; i < max_iterations + min_loop; i++)
		{
			#pragma region Cache Local
				if (i < max_loop)
				{
					temp_posX = glob_posX[i];
					temp_posY = glob_posY[i];
					temp_posZ = glob_posZ[i];
				}
			#pragma endregion
			#pragma region Cache Shared
				mem_fence(CLK_LOCAL_MEM_FENCE);
				cache_offset = 0;
				for (int c = 0; c < %cache_size%; c++)
				{
					loc_posX[c] = out_posX[c + cache_offset];
					loc_posY[c] = out_posY[c + cache_offset];
					loc_posZ[c] = out_posZ[c + cache_offset];
				}
				mem_fence(CLK_LOCAL_MEM_FENCE);
			#pragma endregion
			forceX = forceY = forceZ = 0;
			for (int j = 0; j < %num_of_bodies%; j++)
			{
				#pragma region Use Cached Data
					//Generate Rji vector with cached values
					//dRX = loc_posX[j - cache_offset] - temp_posX;
					//dRY = loc_posY[j - cache_offset] - temp_posY;
					//dRZ = loc_posZ[j - cache_offset] - temp_posZ;
					dRX = out_posX[j] - temp_posX;
					dRY = out_posY[j] - temp_posY;
					dRZ = out_posZ[j] - temp_posZ;
				#pragma endregion
				#pragma region Cache Shared
					if (((j+1) & (%cache_size% - 1)) == 0)
					{
						mem_fence(CLK_LOCAL_MEM_FENCE);
						cache_offset = j+1;
						for (int c = min_cache; c < max_cache; c++)
						{
							loc_posX[c] = out_posX[c + cache_offset];
							loc_posY[c] = out_posY[c + cache_offset];
							loc_posZ[c] = out_posZ[c + cache_offset];
						}
						mem_fence(CLK_LOCAL_MEM_FENCE);
					}
				#pragma endregion
				#pragma region Calculate Force
					if (i>=max_loop || i==j) continue;

					//Generate force size
					length_pow_2 = (dRX * dRX) + (dRY * dRY) + (dRZ * dRZ); //Length squared
					if (length_pow_2 == 0) force_div_length = 0;
					else                   force_div_length = mass_mul_g[i]*glob_mass[j]*rsqrt((double)(length_pow_2*length_pow_2*length_pow_2));

					//Generate the force vector. Save it instead of the Rji vector
					dRX *= force_div_length;
					dRY *= force_div_length;
					dRZ *= force_div_length;

					//Add the force vector to the bodies
					forceX += dRX; forceY += dRY; forceZ += dRZ;
				#pragma endregion
			}
			#pragma region Calculate Velocity and Position
				if (i>=max_loop) continue;

				//Calculate the new velocity of the body
				temp_velX = glob_velX[i] + forceX * dt_div_mass[i];
				temp_velY = glob_velY[i] + forceY * dt_div_mass[i];
				temp_velZ = glob_velZ[i] + forceZ * dt_div_mass[i];
				glob_velX[i] = temp_velX;
				glob_velY[i] = temp_velY;
				glob_velZ[i] = temp_velZ;

				//Calculate the new position of the body
				glob_posX[i] = temp_posX + temp_velX * %dt%;
				glob_posY[i] = temp_posY + temp_velY * %dt%;
				glob_posZ[i] = temp_posZ + temp_velZ * %dt%;
			#pragma endregion
		}
		#pragma region Update Data
			mem_fence(CLK_GLOBAL_MEM_FENCE);
			for (int i = min_loop; i < max_loop; i++)
			{
				out_posX[i] = glob_posX[i];
				out_posY[i] = glob_posY[i];
				out_posZ[i] = glob_posZ[i];
				out_velX[i] = glob_velX[i];
				out_velY[i] = glob_velY[i];
				out_velZ[i] = glob_velZ[i];
			}
			mem_fence(CLK_GLOBAL_MEM_FENCE);
		#pragma endregion
	}
}

kernel void Euler2DKernel(
	global double* glob_posX, global double* glob_posY, global double* glob_posZ, global double* glob_velX, global double* glob_velY, global double* glob_velZ, global const double* glob_mass,
	global const double* dt_div_mass, global const double* mass_mul_g, global const double* dt_div_2mass, global const double* dt_squared_div_2mass, global const double* dt_squared_div_12mass, global const double* dt_pow_3_div_6mass,
	global double* out_posX, global double* out_posY, global double* out_posZ, global double* out_velX, global double* out_velY, global double* out_velZ,
	local double* loc_posX, local double* loc_posY, local double* loc_posZ, local double* loc_velX, local double* loc_velY, local double* loc_velZ,
	global double* l_velX, global double* l_velY, global double* l_velZ, global double* l_forceX, global double* l_forceY, global double* l_forceZ, global double* l_jerkX, global double* l_jerkY, global double* l_jerkZ)
{
	#pragma region Thread Statistics Variables
		int id = get_global_id(0); if (id >= %num_of_threads%) return;
		int loc_id = get_local_id(0);
	#pragma endregion
	#pragma region Loop Boundaries Variables
		int min_loop = id*%num_of_bodies%/%num_of_threads%;
		int max_loop = (id+1)*%num_of_bodies%/%num_of_threads%;
		int max_iterations = (%num_of_bodies%+%num_of_threads%-1)/%num_of_threads%;
		int min_cache; int max_cache;
		if (%num_of_bodies% < %cache_size%) {
			min_cache = loc_id*%num_of_bodies%/%workgroup_size%;
			max_cache = (loc_id+1)*%num_of_bodies%/%workgroup_size%;
		} else {
			min_cache = loc_id*%cache_size%/%workgroup_size%;
			max_cache = (loc_id+1)*%cache_size%/%workgroup_size%;
		}
		int cache_offset;
	#pragma endregion
	#pragma region Local Cache Variables
		double temp_posX, temp_posY; //Position Cache
		double temp_velX, temp_velY; //Velocity Cache
	#pragma endregion
	#pragma region Force Calculation Variables
		double forceX, forceY;
		double dRX, dRY; //used to find the connecting vector between two bodies. The Rji vector.
		double length_pow_2; //The inverse of the third power of the length of the Rji vector.
		double force_div_length; //The force(newtons) of gravitation between the two bodies, divided by the length of the Rji vector. The Rji vector is multiplied by this value to generate the force vector.
	#pragma endregion

	for (int k = 0; k < %calculations%; k++)
	{
		for (int i = min_loop; i < max_iterations + min_loop; i++)
		{
			#pragma region Cache Local
				if (i < max_loop)
				{
					temp_posX = glob_posX[i];
					temp_posY = glob_posY[i];
				}
			#pragma endregion
			#pragma region Cache Shared
				mem_fence(CLK_LOCAL_MEM_FENCE);
				cache_offset = 0;
				for (int c = 0; c < %cache_size%; c++)
				{
					loc_posX[c] = out_posX[c + cache_offset];
					loc_posY[c] = out_posY[c + cache_offset];
				}
				mem_fence(CLK_LOCAL_MEM_FENCE);
			#pragma endregion
			forceX = forceY = 0;
			for (int j = 0; j < %num_of_bodies%; j++)
			{
				#pragma region Use Cached Data
					//Generate Rji vector with cached values
					dRX = loc_posX[j - cache_offset] - temp_posX;
					dRY = loc_posY[j - cache_offset] - temp_posY;
				#pragma endregion
				#pragma region Cache Shared
					if (((j-1) & (%cache_size% - 1)) == 0)
					{
						mem_fence(CLK_LOCAL_MEM_FENCE);
						cache_offset = j+1;
						for (int c = min_cache; c < max_cache; c++)
						{
							loc_posX[c] = out_posX[c + cache_offset];
							loc_posY[c] = out_posY[c + cache_offset];
						}
						mem_fence(CLK_LOCAL_MEM_FENCE);
					}
				#pragma endregion
				#pragma region Calculate Force
					if (i>=max_loop || i==j) continue;

					//Generate force size
					length_pow_2 = (dRX * dRX) + (dRY * dRY); //Length squared
					if (length_pow_2 == 0) force_div_length = 0;
					else                   force_div_length = mass_mul_g[i]*glob_mass[j]*rsqrt((double)(length_pow_2*length_pow_2*length_pow_2));

					//Generate the force vector. Save it instead of the Rji vector
					dRX *= force_div_length;
					dRY *= force_div_length;

					//Add the force vector to the bodies
					forceX += dRX; forceY += dRY;
				#pragma endregion
			}
			#pragma region Calculate Velocity and Position
				if (i>=max_loop) continue;

				//Calculate the new velocity of the body
				temp_velX = glob_velX[i] + forceX * dt_div_mass[i];
				temp_velY = glob_velY[i] + forceY * dt_div_mass[i];
				glob_velX[i] = temp_velX;
				glob_velY[i] = temp_velY;

				//Calculate the new position of the body
				glob_posX[i] = temp_posX + temp_velX * %dt%;
				glob_posY[i] = temp_posY + temp_velY * %dt%;
			#pragma endregion
		}
		#pragma region Update Data
			mem_fence(CLK_GLOBAL_MEM_FENCE);
			for (int i = min_loop; i < max_loop; i++)
			{
				out_posX[i] = glob_posX[i];
				out_posY[i] = glob_posY[i];
				out_velX[i] = glob_velX[i];
				out_velY[i] = glob_velY[i];
			}
			mem_fence(CLK_GLOBAL_MEM_FENCE);
		#pragma endregion
	}
}

kernel void ModifiedEuler3DKernel(
	global double* glob_posX, global double* glob_posY, global double* glob_posZ, global double* glob_velX, global double* glob_velY, global double* glob_velZ, global const double* glob_mass,
	global const double* dt_div_mass, global const double* mass_mul_g, global const double* dt_div_2mass, global const double* dt_squared_div_2mass, global const double* dt_squared_div_12mass, global const double* dt_pow_3_div_6mass,
	global double* out_posX, global double* out_posY, global double* out_posZ, global double* out_velX, global double* out_velY, global double* out_velZ,
	local double* loc_posX, local double* loc_posY, local double* loc_posZ, local double* loc_velX, local double* loc_velY, local double* loc_velZ,
	global double* l_velX, global double* l_velY, global double* l_velZ, global double* l_forceX, global double* l_forceY, global double* l_forceZ, global double* l_jerkX, global double* l_jerkY, global double* l_jerkZ)
{
	#pragma region Thread Statistics Variables
		int id = get_global_id(0); if (id >= %num_of_threads%) return;
		int loc_id = get_local_id(0);
	#pragma endregion
	#pragma region Loop Boundaries Variables
		int min_loop = id*%num_of_bodies%/%num_of_threads%;
		int max_loop = (id+1)*%num_of_bodies%/%num_of_threads%;
		int max_iterations = (%num_of_bodies%+%num_of_threads%-1)/%num_of_threads%;
		int min_cache; int max_cache;
		if (%num_of_bodies% < %cache_size%) {
			min_cache = loc_id*%num_of_bodies%/%workgroup_size%;
			max_cache = (loc_id+1)*%num_of_bodies%/%workgroup_size%;
		} else {
			min_cache = loc_id*%cache_size%/%workgroup_size%;
			max_cache = (loc_id+1)*%cache_size%/%workgroup_size%;
		}
		int cache_offset;
	#pragma endregion
	#pragma region Local Cache Variables
		double temp_posX, temp_posY, temp_posZ; //Position Cache
		double temp_velX, temp_velY, temp_velZ; //Velocity Cache
	#pragma endregion
	#pragma region Force Calculation Variables
		double forceX, forceY, forceZ;
		double dRX, dRY, dRZ; //used to find the connecting vector between two bodies. The Rji vector.
		double length_pow_2; //The inverse of the third power of the length of the Rji vector.
		double force_div_length; //The force(newtons) of gravitation between the two bodies, divided by the length of the Rji vector. The Rji vector is multiplied by this value to generate the force vector.
	#pragma endregion

	for (int k = 0; k < %calculations%; k++)
	{
		for (int i = min_loop; i < max_iterations + min_loop; i++)
		{
			#pragma region Cache Local
				if (i < max_loop)
				{
					temp_posX = glob_posX[i];
					temp_posY = glob_posY[i];
					temp_posZ = glob_posZ[i];
				}
			#pragma endregion
			#pragma region Cache Shared
				mem_fence(CLK_LOCAL_MEM_FENCE);
				cache_offset = 0;
				for (int c = 0; c < %cache_size%; c++)
				{
					loc_posX[c] = out_posX[c + cache_offset];
					loc_posY[c] = out_posY[c + cache_offset];
					loc_posZ[c] = out_posZ[c + cache_offset];
				}
				mem_fence(CLK_LOCAL_MEM_FENCE);
			#pragma endregion
			forceX = forceY = forceZ = 0;
			for (int j = 0; j < %num_of_bodies%; j++)
			{
				#pragma region Use Cached Data
					//Generate Rji vector with cached values
					dRX = loc_posX[j - cache_offset] - temp_posX;
					dRY = loc_posY[j - cache_offset] - temp_posY;
					dRZ = loc_posZ[j - cache_offset] - temp_posZ;
				#pragma endregion
				#pragma region Cache Shared
					if (((j-1) & (%cache_size% - 1)) == 0)
					{
						mem_fence(CLK_LOCAL_MEM_FENCE);
						cache_offset = j+1;
						for (int c = min_cache; c < max_cache; c++)
						{
							loc_posX[c] = out_posX[c + cache_offset];
							loc_posY[c] = out_posY[c + cache_offset];
							loc_posZ[c] = out_posZ[c + cache_offset];
						}
						mem_fence(CLK_LOCAL_MEM_FENCE);
					}
				#pragma endregion
				#pragma region Calculate Force
					if (i>=max_loop || i==j) continue;

					//Generate force size
					length_pow_2 = (dRX * dRX) + (dRY * dRY) + (dRZ * dRZ); //Length squared
					if (length_pow_2 == 0) force_div_length = 0;
					else                   force_div_length = mass_mul_g[i]*glob_mass[j]*rsqrt((double)(length_pow_2*length_pow_2*length_pow_2));

					//Generate the force vector. Save it instead of the Rji vector
					dRX *= force_div_length;
					dRY *= force_div_length;
					dRZ *= force_div_length;

					//Add the force vector to the bodies
					forceX += dRX; forceY += dRY; forceZ += dRZ;
				#pragma endregion
			}
			#pragma region Calculate Velocity and Position
				if (i>=max_loop) continue;
				
				temp_velX = glob_velX[i];
				temp_velY = glob_velY[i];
				temp_velZ = glob_velZ[i];

				//Calculate the new position of the body
				glob_posX[i] = temp_posX + temp_velX * %dt% + forceX * dt_squared_div_2mass[i];
				glob_posY[i] = temp_posY + temp_velY * %dt% + forceY * dt_squared_div_2mass[i];
				glob_posZ[i] = temp_posZ + temp_velZ * %dt% + forceZ * dt_squared_div_2mass[i];

				//Calculate the new velocity of the body
				glob_velX[i] = temp_velX + forceX * dt_div_mass[i];
				glob_velY[i] = temp_velY + forceY * dt_div_mass[i];
				glob_velZ[i] = temp_velZ + forceZ * dt_div_mass[i];
			#pragma endregion
		}
		#pragma region Update Data
			mem_fence(CLK_GLOBAL_MEM_FENCE);
			for (int i = min_loop; i < max_loop; i++)
			{
				out_posX[i] = glob_posX[i];
				out_posY[i] = glob_posY[i];
				out_posZ[i] = glob_posZ[i];
				out_velX[i] = glob_velX[i];
				out_velY[i] = glob_velY[i];
				out_velZ[i] = glob_velZ[i];
			}
			mem_fence(CLK_GLOBAL_MEM_FENCE);
		#pragma endregion
	}
}

kernel void ModifiedEuler2DKernel(
	global double* glob_posX, global double* glob_posY, global double* glob_posZ, global double* glob_velX, global double* glob_velY, global double* glob_velZ, global const double* glob_mass,
	global const double* dt_div_mass, global const double* mass_mul_g, global const double* dt_div_2mass, global const double* dt_squared_div_2mass, global const double* dt_squared_div_12mass, global const double* dt_pow_3_div_6mass,
	global double* out_posX, global double* out_posY, global double* out_posZ, global double* out_velX, global double* out_velY, global double* out_velZ,
	local double* loc_posX, local double* loc_posY, local double* loc_posZ, local double* loc_velX, local double* loc_velY, local double* loc_velZ,
	global double* l_velX, global double* l_velY, global double* l_velZ, global double* l_forceX, global double* l_forceY, global double* l_forceZ, global double* l_jerkX, global double* l_jerkY, global double* l_jerkZ)
{
	#pragma region Thread Statistics Variables
		int id = get_global_id(0); if (id >= %num_of_threads%) return;
		int loc_id = get_local_id(0);
	#pragma endregion
	#pragma region Loop Boundaries Variables
		int min_loop = id*%num_of_bodies%/%num_of_threads%;
		int max_loop = (id+1)*%num_of_bodies%/%num_of_threads%;
		int max_iterations = (%num_of_bodies%+%num_of_threads%-1)/%num_of_threads%;
		int min_cache; int max_cache;
		if (%num_of_bodies% < %cache_size%) {
			min_cache = loc_id*%num_of_bodies%/%workgroup_size%;
			max_cache = (loc_id+1)*%num_of_bodies%/%workgroup_size%;
		} else {
			min_cache = loc_id*%cache_size%/%workgroup_size%;
			max_cache = (loc_id+1)*%cache_size%/%workgroup_size%;
		}
		int cache_offset;
	#pragma endregion
	#pragma region Local Cache Variables
		double temp_posX, temp_posY; //Position Cache
		double temp_velX, temp_velY; //Velocity Cache
	#pragma endregion
	#pragma region Force Calculation Variables
		double forceX, forceY;
		double dRX, dRY; //used to find the connecting vector between two bodies. The Rji vector.
		double length_pow_2; //The inverse of the third power of the length of the Rji vector.
		double force_div_length; //The force(newtons) of gravitation between the two bodies, divided by the length of the Rji vector. The Rji vector is multiplied by this value to generate the force vector.
	#pragma endregion

	for (int k = 0; k < %calculations%; k++)
	{
		for (int i = min_loop; i < max_iterations + min_loop; i++)
		{
			#pragma region Cache Local
				if (i < max_loop)
				{
					temp_posX = glob_posX[i];
					temp_posY = glob_posY[i];
				}
			#pragma endregion
			#pragma region Cache Shared
				mem_fence(CLK_LOCAL_MEM_FENCE);
				cache_offset = 0;
				for (int c = 0; c < %cache_size%; c++)
				{
					loc_posX[c] = out_posX[c + cache_offset];
					loc_posY[c] = out_posY[c + cache_offset];
				}
				mem_fence(CLK_LOCAL_MEM_FENCE);
			#pragma endregion
			forceX = forceY = 0;
			for (int j = 0; j < %num_of_bodies%; j++)
			{
				#pragma region Use Cached Data
					//Generate Rji vector with cached values
					dRX = loc_posX[j - cache_offset] - temp_posX;
					dRY = loc_posY[j - cache_offset] - temp_posY;
				#pragma endregion
				#pragma region Cache Shared
					if (((j+1) & (%cache_size% - 1)) == 0)
					{
						cache_offset = j+1;
						for (int c = min_cache; c < max_cache; c++)
						{
							loc_posX[c] = out_posX[c + cache_offset];
							loc_posY[c] = out_posY[c + cache_offset];
						}
						mem_fence(CLK_LOCAL_MEM_FENCE);
					}
				#pragma endregion
				#pragma region Calculate Force
					if (i>=max_loop || i==j) continue;

					//Generate force size
					length_pow_2 = (dRX * dRX) + (dRY * dRY); //Length squared
					if (length_pow_2 == 0) force_div_length = 0;
					else                   force_div_length = mass_mul_g[i]*glob_mass[j]*rsqrt((double)(length_pow_2*length_pow_2*length_pow_2));

					//Generate the force vector. Save it instead of the Rji vector
					dRX *= force_div_length;
					dRY *= force_div_length;

					//Add the force vector to the bodies
					forceX += dRX; forceY += dRY;
				#pragma endregion
			}
			#pragma region Calculate Velocity and Position
				if (i>=max_loop) continue;

				temp_velX = glob_velX[i];
				temp_velY = glob_velY[i];

				//Calculate the new position of the body
				glob_posX[i] = temp_posX + temp_velX * %dt% + forceX * dt_squared_div_2mass[i];
				glob_posY[i] = temp_posY + temp_velY * %dt% + forceY * dt_squared_div_2mass[i];

				//Calculate the new velocity of the body
				glob_velX[i] = temp_velX + forceX * dt_div_mass[i];
				glob_velY[i] = temp_velY + forceY * dt_div_mass[i];
			#pragma endregion
		}
		#pragma region Update Data
			mem_fence(CLK_GLOBAL_MEM_FENCE);
			for (int i = min_loop; i < max_loop; i++)
			{
				out_posX[i] = glob_posX[i];
				out_posY[i] = glob_posY[i];
				out_velX[i] = glob_velX[i];
				out_velY[i] = glob_velY[i];
			}
			mem_fence(CLK_GLOBAL_MEM_FENCE);
		#pragma endregion
	}
}

kernel void Leapfrog3DKernel(
	global double* glob_posX, global double* glob_posY, global double* glob_posZ, global double* glob_velX, global double* glob_velY, global double* glob_velZ, global const double* glob_mass,
	global const double* dt_div_mass, global const double* mass_mul_g, global const double* dt_div_2mass, global const double* dt_squared_div_2mass, global const double* dt_squared_div_12mass, global const double* dt_pow_3_div_6mass,
	global double* out_posX, global double* out_posY, global double* out_posZ, global double* out_velX, global double* out_velY, global double* out_velZ,
	local double* loc_posX, local double* loc_posY, local double* loc_posZ, local double* loc_velX, local double* loc_velY, local double* loc_velZ,
	global double* l_velX, global double* l_velY, global double* l_velZ, global double* l_forceX, global double* l_forceY, global double* l_forceZ, global double* l_jerkX, global double* l_jerkY, global double* l_jerkZ)
{
	#pragma region Thread Statistics Variables
		int id = get_global_id(0); if (id >= %num_of_threads%) return;
		int loc_id = get_local_id(0);
	#pragma endregion
	#pragma region Loop Boundaries Variables
		int min_loop = id*%num_of_bodies%/%num_of_threads%;
		int max_loop = (id+1)*%num_of_bodies%/%num_of_threads%;
		int max_iterations = (%num_of_bodies%+%num_of_threads%-1)/%num_of_threads%;
		int min_cache; int max_cache;
		if (%num_of_bodies% < %cache_size%) {
			min_cache = loc_id*%num_of_bodies%/%workgroup_size%;
			max_cache = (loc_id+1)*%num_of_bodies%/%workgroup_size%;
		} else {
			min_cache = loc_id*%cache_size%/%workgroup_size%;
			max_cache = (loc_id+1)*%cache_size%/%workgroup_size%;
		}
		int cache_offset;
	#pragma endregion
	#pragma region Local Cache Variables
		double temp_posX, temp_posY, temp_posZ; //Position Cache
		double temp_velX, temp_velY, temp_velZ; //Velocity Cache
	#pragma endregion
	#pragma region Force Calculation Variables
		double forceX, forceY, forceZ;
		double dRX, dRY, dRZ; //used to find the connecting vector between two bodies. The Rji vector.
		double length_pow_2; //The inverse of the third power of the length of the Rji vector.
		double force_div_length; //The force(newtons) of gravitation between the two bodies, divided by the length of the Rji vector. The Rji vector is multiplied by this value to generate the force vector.
	#pragma endregion

	for (int k = 0; k < %calculations%; k++)
	{
		for (int i = min_loop; i < max_iterations + min_loop; i++)
		{
			#pragma region Cache Local
				if (i < max_loop)
				{
					temp_posX = glob_posX[i];
					temp_posY = glob_posY[i];
					temp_posZ = glob_posZ[i];
				}
			#pragma endregion
			#pragma region Cache Shared
				mem_fence(CLK_LOCAL_MEM_FENCE);
				cache_offset = 0;
				for (int c = 0; c < %cache_size%; c++)
				{
					loc_posX[c] = out_posX[c + cache_offset];
					loc_posY[c] = out_posY[c + cache_offset];
					loc_posZ[c] = out_posZ[c + cache_offset];
				}
				mem_fence(CLK_LOCAL_MEM_FENCE);
			#pragma endregion
			forceX = forceY = forceZ = 0;
			for (int j = 0; j < %num_of_bodies%; j++)
			{
				#pragma region Use Cached Data
					//Generate Rji vector with cached values
					dRX = loc_posX[j - cache_offset] - temp_posX;
					dRY = loc_posY[j - cache_offset] - temp_posY;
					dRZ = loc_posZ[j - cache_offset] - temp_posZ;
				#pragma endregion
				#pragma region Cache Shared
					if (((j+1) & (%cache_size% - 1)) == 0)
					{
						mem_fence(CLK_LOCAL_MEM_FENCE);
						cache_offset = j+1;
						for (int c = min_cache; c < max_cache; c++)
						{
							loc_posX[c] = out_posX[c + cache_offset];
							loc_posY[c] = out_posY[c + cache_offset];
							loc_posZ[c] = out_posZ[c + cache_offset];
						}
						mem_fence(CLK_LOCAL_MEM_FENCE);
					}
				#pragma endregion
				#pragma region Calculate Force
					if (i>=max_loop || i==j) continue;

					//Generate force size
					length_pow_2 = (dRX * dRX) + (dRY * dRY) + (dRZ * dRZ); //Length squared
					if (length_pow_2 == 0) force_div_length = 0;
					else                   force_div_length = mass_mul_g[i]*glob_mass[j]*rsqrt((double)(length_pow_2*length_pow_2*length_pow_2));

					//Generate the force vector. Save it instead of the Rji vector
					dRX *= force_div_length;
					dRY *= force_div_length;
					dRZ *= force_div_length;

					//Add the force vector to the bodies
					forceX += dRX; forceY += dRY; forceZ += dRZ;
				#pragma endregion
			}
			#pragma region Calculate Velocity and Position
				if (i>=max_loop) continue;

				temp_velX = glob_velX[i];
				temp_velY = glob_velY[i];
				temp_velZ = glob_velZ[i];

				//Calculate the new position of the body
				glob_posX[i] = temp_posX + temp_velX * %dt% + forceX * dt_squared_div_2mass[i];
				glob_posY[i] = temp_posY + temp_velY * %dt% + forceY * dt_squared_div_2mass[i];
				glob_posZ[i] = temp_posZ + temp_velZ * %dt% + forceZ * dt_squared_div_2mass[i];

				//Calculate the new velocity of the body
				glob_velX[i] = temp_velX + (forceX + l_forceX[i]) * dt_div_2mass[i];
				glob_velY[i] = temp_velY + (forceY + l_forceY[i]) * dt_div_2mass[i];
				glob_velZ[i] = temp_velZ + (forceZ + l_forceZ[i]) * dt_div_2mass[i];

				//Save the old force
				l_forceX[i] = forceX;
				l_forceY[i] = forceY;
				l_forceZ[i] = forceZ;
			#pragma endregion
		}
		#pragma region Update Data
			mem_fence(CLK_GLOBAL_MEM_FENCE);
			for (int i = min_loop; i < max_loop; i++)
			{
				out_posX[i] = glob_posX[i];
				out_posY[i] = glob_posY[i];
				out_posZ[i] = glob_posZ[i];
				out_velX[i] = glob_velX[i];
				out_velY[i] = glob_velY[i];
				out_velZ[i] = glob_velZ[i];
			}
			mem_fence(CLK_GLOBAL_MEM_FENCE);
		#pragma endregion
	}
}

kernel void Leapfrog2DKernel(
	global double* glob_posX, global double* glob_posY, global double* glob_posZ, global double* glob_velX, global double* glob_velY, global double* glob_velZ, global const double* glob_mass,
	global const double* dt_div_mass, global const double* mass_mul_g, global const double* dt_div_2mass, global const double* dt_squared_div_2mass, global const double* dt_squared_div_12mass, global const double* dt_pow_3_div_6mass,
	global double* out_posX, global double* out_posY, global double* out_posZ, global double* out_velX, global double* out_velY, global double* out_velZ,
	local double* loc_posX, local double* loc_posY, local double* loc_posZ, local double* loc_velX, local double* loc_velY, local double* loc_velZ,
	global double* l_velX, global double* l_velY, global double* l_velZ, global double* l_forceX, global double* l_forceY, global double* l_forceZ, global double* l_jerkX, global double* l_jerkY, global double* l_jerkZ)
{
	#pragma region Thread Statistics Variables
		int id = get_global_id(0); if (id >= %num_of_threads%) return;
		int loc_id = get_local_id(0);
	#pragma endregion
	#pragma region Loop Boundaries Variables
		int min_loop = id*%num_of_bodies%/%num_of_threads%;
		int max_loop = (id+1)*%num_of_bodies%/%num_of_threads%;
		int max_iterations = (%num_of_bodies%+%num_of_threads%-1)/%num_of_threads%;
		int min_cache; int max_cache;
		if (%num_of_bodies% < %cache_size%) {
			min_cache = loc_id*%num_of_bodies%/%workgroup_size%;
			max_cache = (loc_id+1)*%num_of_bodies%/%workgroup_size%;
		} else {
			min_cache = loc_id*%cache_size%/%workgroup_size%;
			max_cache = (loc_id+1)*%cache_size%/%workgroup_size%;
		}
		int cache_offset;
	#pragma endregion
	#pragma region Local Cache Variables
		double temp_posX, temp_posY; //Position Cache
		double temp_velX, temp_velY; //Velocity Cache
	#pragma endregion
	#pragma region Force Calculation Variables
		double forceX, forceY;
		double dRX, dRY; //used to find the connecting vector between two bodies. The Rji vector.
		double length_pow_2; //The inverse of the third power of the length of the Rji vector.
		double force_div_length; //The force(newtons) of gravitation between the two bodies, divided by the length of the Rji vector. The Rji vector is multiplied by this value to generate the force vector.
	#pragma endregion

	for (int k = 0; k < %calculations%; k++)
	{
		for (int i = min_loop; i < max_iterations + min_loop; i++)
		{
			#pragma region Cache Local
				if (i < max_loop)
				{
					temp_posX = glob_posX[i];
					temp_posY = glob_posY[i];
				}
			#pragma endregion
			#pragma region Cache Shared
				mem_fence(CLK_LOCAL_MEM_FENCE);
				cache_offset = 0;
				for (int c = 0; c < %cache_size%; c++)
				{
					loc_posX[c] = out_posX[c + cache_offset];
					loc_posY[c] = out_posY[c + cache_offset];
				}
				mem_fence(CLK_LOCAL_MEM_FENCE);
			#pragma endregion
			forceX = forceY = 0;
			for (int j = 0; j < %num_of_bodies%; j++)
			{
				#pragma region Use Cached Data
					//Generate Rji vector with cached values
					dRX = loc_posX[j - cache_offset] - temp_posX;
					dRY = loc_posY[j - cache_offset] - temp_posY;
				#pragma endregion
				#pragma region Cache Shared
					if (((j+1) & (%cache_size% - 1)) == 0)
					{
						mem_fence(CLK_LOCAL_MEM_FENCE);
						cache_offset = j+1;
						for (int c = min_cache; c < max_cache; c++)
						{
							loc_posX[c] = out_posX[c + cache_offset];
							loc_posY[c] = out_posY[c + cache_offset];
						}
						mem_fence(CLK_LOCAL_MEM_FENCE);
					}
				#pragma endregion
				#pragma region Calculate Force
					if (i>=max_loop || i==j) continue;

					//Generate force size
					length_pow_2 = (dRX * dRX) + (dRY * dRY); //Length squared
					if (length_pow_2 == 0) force_div_length = 0;
					else                   force_div_length = mass_mul_g[i]*glob_mass[j]*rsqrt((double)(length_pow_2*length_pow_2*length_pow_2));

					//Generate the force vector. Save it instead of the Rji vector
					dRX *= force_div_length;
					dRY *= force_div_length;

					//Add the force vector to the bodies
					forceX += dRX; forceY += dRY;
				#pragma endregion
			}
			#pragma region Calculate Velocity and Position
				if (i>=max_loop) continue;

				temp_velX = glob_velX[i];
				temp_velY = glob_velY[i];
				
				//Calculate the new position of the body
				glob_posX[i] = temp_posX + temp_velX * %dt% + forceX * dt_squared_div_2mass[i];
				glob_posY[i] = temp_posY + temp_velY * %dt% + forceY * dt_squared_div_2mass[i];

				//Calculate the new velocity of the body
				glob_velX[i] = temp_velX + (forceX + l_forceX[i]) * dt_div_2mass[i];
				glob_velY[i] = temp_velY + (forceY + l_forceY[i]) * dt_div_2mass[i];

				//Save the old force
				l_forceX[i] = forceX;
				l_forceY[i] = forceY;
			#pragma endregion
		}
		#pragma region Update Data
			mem_fence(CLK_GLOBAL_MEM_FENCE);
			for (int i = min_loop; i < max_loop; i++)
			{
				out_posX[i] = glob_posX[i];
				out_posY[i] = glob_posY[i];
				out_velX[i] = glob_velX[i];
				out_velY[i] = glob_velY[i];
			}
			mem_fence(CLK_GLOBAL_MEM_FENCE);
		#pragma endregion
	}
}
