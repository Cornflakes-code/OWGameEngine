#include "CommonUtils.h"
#include "ErrorHandling.h"

namespace Compass
{
	glm::vec4 Rose[NumDirections] = {
			glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),	// North
			glm::vec4(0.0f, -1.0f, 0.0f, 0.0f),	// South
			glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),	// East
			glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f),	// West
			glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),	// in (maybe wrong)
			glm::vec4(1.0f, 0.0f, -1.0f, 0.0f),	// out (maybe wrong)
			glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)	// No Direction
	};

	std::string asString(Direction dir)
	{
		switch (dir)
		{
			case North: return "North";
			case South: return "South";
			case East: return "East";
			case West: return "West";
			case In: return "In";
			case Out: return "Out";
			case NoDirection: return "NoDirection";
			case NumDirections: return "NumDirections";
			default: throw NMSLogicException(std::stringstream() << "Unknown Direction [" << dir << "]\n");
		}
	}
}

