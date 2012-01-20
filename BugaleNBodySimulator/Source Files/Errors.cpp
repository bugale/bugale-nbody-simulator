#include "..\SharedHeader.h"

char* Errors::returnError(int error)
{
	switch (error)
	{
		case NoError:
			return "";
		case MissingSettingsFile:
			return "Missing settings file!\nYou can use Bugale N-Body Data Editor to create and edit this file.\nBugale N-Body Data Editor can be found on SourceForge.\nAlternatively, you can download samples of this file for various simulations from this project's page in SourceForge.";
		case MissingBodyDataFile:
			return "Missing body data file!\nYou can use Bugale N-Body Data Editor to create and edit this file.\nBugale N-Body Data Editor can be found on SourceForge.\nAlternatively, you can download samples of this file for various simulations from this project's page in SourceForge.";
		case WrongSettingsFileSize:
			return "Settings file is corrupted(wrong file size)!\nYou can use Bugale N-Body Data Editor to create and edit this file.\nBugale N-Body Data Editor can be found on SourceForge.\nAlternatively, you can download samples of this file for various simulations from this project's page in SourceForge.";
		case WrongBodyDataFileSize:
			return "Body data file is corrupted(wrong file size)!\nYou can use Bugale N-Body Data Editor to create and edit this file.\nBugale N-Body Data Editor can be found on SourceForge.\nAlternatively, you can download samples of this file for various simulations from this project's page in SourceForge.";
		case WrongBodyDataFileSize2:
			return "Body data file is corrupted(wrong file size)!\nYou can use Bugale N-Body Data Editor to create and edit this file.\nBugale N-Body Data Editor can be found on SourceForge.\nAlternatively, you can download samples of this file for various simulations from this project's page in SourceForge.";
		case NegativeWidth:
			return "Settings file is corrupted(negative width)!\nYou can use Bugale N-Body Data Editor to create and edit this file.\nBugale N-Body Data Editor can be found on SourceForge.\nAlternatively, you can download samples of this file for various simulations from this project's page in SourceForge.";
		case NegativeHeight:
			return "Settings file is corrupted(negative height)!\nYou can use Bugale N-Body Data Editor to create and edit this file.\nBugale N-Body Data Editor can be found on SourceForge.\nAlternatively, you can download samples of this file for various simulations from this project's page in SourceForge.";
		case NegativeDT:
			return "Settings file is corrupted(negative DT)!\nYou can use Bugale N-Body Data Editor to create and edit this file.\nBugale N-Body Data Editor can be found on SourceForge.\nAlternatively, you can download samples of this file for various simulations from this project's page in SourceForge.";
		case NegativeMaxTrails:
			return "Settings file is corrupted(negative maximum trail dots count)!\nYou can use Bugale N-Body Data Editor to create and edit this file.\nBugale N-Body Data Editor can be found on SourceForge.\nAlternatively, you can download samples of this file for various simulations from this project's page in SourceForge.";
		case NonPositiveSlices:
			return "Settings file is corrupted(non-positive slices count)!\nYou can use Bugale N-Body Data Editor to create and edit this file.\nBugale N-Body Data Editor can be found on SourceForge.\nAlternatively, you can download samples of this file for various simulations from this project's page in SourceForge.";
		case NonPositiveStacks:
			return "Settings file is corrupted(non-positive stacks count)!\nYou can use Bugale N-Body Data Editor to create and edit this file.\nBugale N-Body Data Editor can be found on SourceForge.\nAlternatively, you can download samples of this file for various simulations from this project's page in SourceForge.";
		case NonPositiveFieldOfView:
			return "Settings file is corrupted(non-positive field of view)!\nYou can use Bugale N-Body Data Editor to create and edit this file.\nBugale N-Body Data Editor can be found on SourceForge.\nAlternatively, you can download samples of this file for various simulations from this project's page in SourceForge.";
		case NegativeMoveSpeed0:
			return "Settings file is corrupted(negative MoveSpeed0)!\nYou can use Bugale N-Body Data Editor to create and edit this file.\nBugale N-Body Data Editor can be found on SourceForge.\nAlternatively, you can download samples of this file for various simulations from this project's page in SourceForge.";
		case NonPositiveMoveSpeed1:
			return "Settings file is corrupted(non-positive MoveSpeed1)!\nYou can use Bugale N-Body Data Editor to create and edit this file.\nBugale N-Body Data Editor can be found on SourceForge.\nAlternatively, you can download samples of this file for various simulations from this project's page in SourceForge.";
		case NegativeZoomSpeed0:
			return "Settings file is corrupted(negative ZoomSpeed0)!\nYou can use Bugale N-Body Data Editor to create and edit this file.\nBugale N-Body Data Editor can be found on SourceForge.\nAlternatively, you can download samples of this file for various simulations from this project's page in SourceForge.";
		case NonPositiveZoomSpeed1:
			return "Settings file is corrupted(non-positive ZoomSpeed1)!\nYou can use Bugale N-Body Data Editor to create and edit this file.\nBugale N-Body Data Editor can be found on SourceForge.\nAlternatively, you can download samples of this file for various simulations from this project's page in SourceForge.";
		case NegativeBodyCount:
			return "Body data file is corrupted(negative body count)!\nYou can use Bugale N-Body Data Editor to create and edit this file.\nBugale N-Body Data Editor can be found on SourceForge.\nAlternatively, you can download samples of this file for various simulations from this project's page in SourceForge.";
		case NegativeBodyMass:
			return "Body data file is corrupted(negative body mass)!\nYou can use Bugale N-Body Data Editor to create and edit this file.\nBugale N-Body Data Editor can be found on SourceForge.\nAlternatively, you can download samples of this file for various simulations from this project's page in SourceForge.";
		case NegativeBodyRadius:
			return "Body data file is corrupted(negative body radius)!\nYou can use Bugale N-Body Data Editor to create and edit this file.\nBugale N-Body Data Editor can be found on SourceForge.\nAlternatively, you can download samples of this file for various simulations from this project's page in SourceForge.";
		case NegativeBodyTrailWidth:
			return "Body data file is corrupted(negative body trail width)!\nYou can use Bugale N-Body Data Editor to create and edit this file.\nBugale N-Body Data Editor can be found on SourceForge.\nAlternatively, you can download samples of this file for various simulations from this project's page in SourceForge.";
		case Other:
			return "Unidentified Error!";
		default:
			return "Unknown Error!";
	}
}