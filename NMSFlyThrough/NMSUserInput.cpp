#include "NMSUserInput.h"

#include <Core/CommonUtils.h>
#include <Core/ErrorHandling.h>
#include <GLFW/glfw3.h>

std::string NMSUserInput::userInputToString(int value)
{
	NMSUserCommand input = (NMSUserInput::NMSUserCommand)value;
	switch (input)
	{
	case NMSUserCommand::Up: return "Up";
	case NMSUserCommand::Down: return "Down";
	case NMSUserCommand::Left: return "Left";
	case NMSUserCommand::Right: return "Right";
	case NMSUserCommand::Forward: return "Forward";
	case NMSUserCommand::Back: return "Back";
	case NMSUserCommand::PitchUp: return "PitchUp";
	case NMSUserCommand::PitchDown: return "PitchDown";
	case NMSUserCommand::YawLeft: return "YawLeft";
	case NMSUserCommand::YawRight: return "YawRight";
	case NMSUserCommand::SpecialKey1: return "SpecialKey1";
	case NMSUserCommand::SpecialKey2: return "SpecialKey2";
	case NMSUserCommand::SpecialKey3: return "SpecialKey3";
	case NMSUserCommand::SpecialKey4: return "SpecialKey4";
	case NMSUserCommand::SpecialKey5: return "SpecialKey5";
	case NMSUserCommand::SpecialKey6: return "SpecialKey6";
	default:
		throw NMSException(std::stringstream() << "Bad User Input pushed [" << value << "]\n");
	}
} 
void NMSUserInput::addKeyMapping(const std::string& key, const std::vector<std::string>& mods,
		const std::string& logicalOperator)
{
	UserInput::addKeyMapping(key, mods, logicalOperator);
	addKey(GLFW_KEY_LEFT_BRACKET, 0, UserInput::LogicalOperator::OptionsScreen);
	addKey(GLFW_KEY_RIGHT_BRACKET, 0, UserInput::LogicalOperator::RopeScreen);
}

UserInput::LogicalOperator NMSUserInput::userCommand(const UserInput::UserCommandCallbackData& data)
{
	// Base class key mapping takes priority
	UserInput::LogicalOperator retval = UserInput::userCommand(data);
	if (retval != LogicalOperator::NoCommand)
		return retval;
/*
	auto iter = mNMSKeyMapping.begin();
	std::map<NMSKeyMapping, NMSUserCommand>::iterator anykeyFound = mNMSKeyMapping.end();
	while (iter != mNMSKeyMapping.end())
	{
		if (iter->first.userCommand == data.key)
		{ 
			if (iter->first.keyMod == InputMod::NoMod && data.mods == 0)
				return iter->second;
			if (iter->first.keyMod & data.mods)
				return iter->second;
		}
		if (iter->first.userCommand == AnyKey)
		{
			// Use this if a key mapping is not found
			anykeyFound = iter;
		}
		++iter;
	}
	if (anykeyFound != mNMSKeyMapping.end())
		return anykeyFound->first.userCommand;
*/
	return LogicalOperator::NoCommand;
}

NMSUserInput::NMSUserInput()
{

}

