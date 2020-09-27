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

glm::vec4 gSolidColours[16] =
{
  {0.0f, 0.0f, 0.0f, 1.0f},     // BLACK
  {0.0f, 0.0f, 0.67f, 1.0f},    // BLUE
  {0.0f, 0.67f, 0.0f, 1.0f},    // GREEN
  {0.0f, 0.67f, 0.67f, 1.0f},   // CYAN
  {0.67f, 0.0f, 0.0f, 1.0f},    // RED
  {0.67f, 0.0f, 0.67f, 1.0f},   // MAGENTA
  {0.67f, 0.67f, 0.0f, 1.0f},   // YELLOW
  {0.67f, 0.67f, 0.67f, 1.0f},  // WHITE
  {0.33f, 0.33f, 0.33f, 1.0f},  // BRIGHT BLACK
  {0.33f, 0.33f, 1.0f, 1.0f},   // BRIGHT BLUE
  {0.33f, 1.0f, 0.33f, 1.0f},   // BRIGHT GREEN
  {0.33f, 1.0f, 1.0f, 1.0f},    // BRIGHT CYAN
  {1.0f, 0.33f, 0.33f, 1.0f},   // BRIGHT RED
  {1.0f, 0.33f, 1.0f, 1.0f},    // BRIGHT MAGENTA
  {1.0f, 1.0f, 0.33f, 1.0f},    // BRIGHT YELLOW
  {1.0f, 1.0f, 1.0f, 1.0f}      // BRIGHT WHITE
};

glm::vec4 OWUtils::colour(OWUtils::SolidColours colour)
{
	return gSolidColours[static_cast<unsigned int>(colour)];
}
