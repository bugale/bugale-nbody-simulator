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
#ifndef __BODY3D_INCLUDED__
#define __BODY3D_INCLUDED__

#include "../SharedHeader.h"

struct Body3D
{
	//All units are metric: meters, seconds, kilograms
	double   _positionX; double   _positionY; double   _positionZ; //Position Vector
	double _l_positionX; double _l_positionY; double _l_positionZ; //Last(old) Position Vector
	double _t_positionX; double _t_positionY; double _t_positionZ; //Temporary Position Vector(for hermite)
	double   _velocityX; double   _velocityY; double   _velocityZ; //Velocity Vector
	double _l_velocityX; double _l_velocityY; double _l_velocityZ; //Last(old) Velocity Vector
	double _t_velocityX; double _t_velocityY; double _t_velocityZ; //Temporary Velocity Vector(for hermite)
	double      _forceX; double      _forceY; double      _forceZ; //Force Sum(on this body) Vector
	double    _l_forceX; double    _l_forceY; double    _l_forceZ; //Last(old) Force Sum(on this body) Vector
	double       _jerkX; double       _jerkY; double       _jerkZ; //Jerk(d^3x/dt^3) Vector Multiplied By the Mass
	double     _l_jerkX; double     _l_jerkY; double     _l_jerkZ; //Last(old) Jerk(d^3x/dt^3) Vector Multiplied By the Mass
	double        _mass; double      _radius; double  _trailwidth; //Mass, Radius, and the Radius of the Body's Trail
	
	//Colors
	unsigned char      _colorR; unsigned char      _colorG; unsigned char      _colorB; unsigned char      _colorA; // Body's ARGB Color
	unsigned char _trailcolorR; unsigned char _trailcolorG; unsigned char _trailcolorB; unsigned char _trailcolorA; // Body's Trail ARGB Color
};

#endif