#pragma once
#include <Core/UserInput.h>


class NMSUserInput : public UserInput
{
public:
	enum NMSUserCommand
	{
		Up = UserInput::BaseUserCommand::NumBaseCommands,
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
		InputMods keyMod;
		friend bool operator<(const NMSKeyMapping& lhs, const NMSKeyMapping& rhs)
		{
			if (lhs.userCommand == rhs.userCommand)
				return lhs.keyMod < rhs.keyMod;
			return lhs.userCommand < rhs.userCommand;
		}
	};
	std::map<NMSKeyMapping, NMSUserCommand> mNMSKeyMapping;
public:

	void addKeyMapping(int key, InputMods mod, NMSUserCommand action)
	{
		mNMSKeyMapping[{key, mod}] = action;
	}
	std::string userInputToString(int value) override;
protected:
	int userCommand(const UserInput::UserCommandCallbackData& data) override;
};
