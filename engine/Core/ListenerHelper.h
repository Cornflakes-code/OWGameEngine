#pragma once

#include "../OWEngine/OWEngine.h"

class OWENGINE_API ListenerHelper
{
	/* There does not appear to be an '==' operator for std::function,
	 hence there is no way to remove listeners frome event generators.
	Google idicates passing a unique Id to allow this to happen. Derived
	classes pass mId to the event manager when adding and removing listeners
	*/
public:
	size_t mUniqueId;
protected:
	ListenerHelper()
	{
		// handle == 0 is never generated. 
		static size_t handle = 1;
		mUniqueId = handle++;
	}
	ListenerHelper(const ListenerHelper&)
		: ListenerHelper()
	{}
};
