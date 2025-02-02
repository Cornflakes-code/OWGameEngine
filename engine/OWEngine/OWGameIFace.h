#pragma once

#include "OWEngine.h"

class OWENGINE_API OWGameIFace
{
	bool mActivated = false;
public:
	OWGameIFace() {}
	virtual ~OWGameIFace() {}

	// Every objects init is called before anything else
	void init()
	{
		if (!mActivated)
		{
			doInit();
			mActivated = true;
		}
	}
	virtual void active(bool newValue) { mActivated = newValue; }
	virtual bool active() const { return mActivated; }
	/** Component tick override
	 @param [in] deltaSeconds last frame durations */
	virtual void tick(float deltaSecods) = 0;

	virtual void begin() = 0;
	virtual void end() = 0;

	/** Component destroy override */
	virtual void destroy() = 0;
protected:
	virtual void doInit() = 0;
		// Serialisation functions here?
};
