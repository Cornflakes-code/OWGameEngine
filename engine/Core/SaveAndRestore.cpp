#include "SaveAndRestore.h"

SaveAndRestore::SaveAndRestore()
{

}

glm::uvec2 SaveAndRestore::physicalWindowSize() const
{
	return glm::uvec2({ 800, 600 });
}

std::string SaveAndRestore::activeScene() const
{
	return "Splash";
}