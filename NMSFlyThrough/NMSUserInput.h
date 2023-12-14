#pragma once

#include <Core/UserInput.h>

/*
	Wraps specifics of the NMS user Inputs. May be removed in later iterations.
	If not removed, it will be moved to the NMS game repo.
*/
class NMSUserInput : public UserInput
{
public:
	enum NMSUserCommand
	{
		Up,
		Down,
		Left,
		Right,
		Forward, Back,
		PitchUp, PitchDown,
		YawLeft, YawRight,
		AnyKey,
		SpecialKey1, SpecialKey2, SpecialKey3, SpecialKey4, SpecialKey5, SpecialKey6,
		Quit
	};
private:
	struct NMSKeyMapping
	{
		int userCommand;
		InputMod keyMod;
		friend bool operator<(const NMSKeyMapping& lhs, const NMSKeyMapping& rhs)
		{
			if (lhs.userCommand == rhs.userCommand)
				return lhs.keyMod < rhs.keyMod;
			return lhs.userCommand < rhs.userCommand;
		}
	};
	std::map<NMSKeyMapping, NMSUserCommand> mNMSKeyMapping;
public:
	NMSUserInput();
	void addKeyMapping(const std::string& key, const std::vector<std::string>& mods, const std::string& logicalOperator) override;
	std::string userInputToString(int value) override;
protected:
	LogicalOperator userCommand(const UserInput::UserCommandCallbackData& data) override;
};
