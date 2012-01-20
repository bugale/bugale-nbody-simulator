#define Errors_included
#include "..\SharedHeader.h"

namespace Errors
{
	enum Errors
	{
		NoError,
		MissingSettingsFile,
		MissingBodyDataFile,
		WrongSettingsFileSize,
		WrongBodyDataFileSize,
		WrongBodyDataFileSize2,
		NegativeWidth,
		NegativeHeight,
		NegativeDT,
		NegativeMaxTrails,
		NonPositiveSlices,
		NonPositiveStacks,
		NonPositiveFieldOfView,
		NegativeMoveSpeed0,
		NonPositiveMoveSpeed1,
		NegativeZoomSpeed0,
		NonPositiveZoomSpeed1,
		NegativeBodyCount,
		NegativeBodyMass,
		NegativeBodyRadius,
		NegativeBodyTrailWidth,
		Other
	};
	char* returnError(int error);
}