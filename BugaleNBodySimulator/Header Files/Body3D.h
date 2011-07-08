#define Body3D_included
#include "..\SharedHeader.h"

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
