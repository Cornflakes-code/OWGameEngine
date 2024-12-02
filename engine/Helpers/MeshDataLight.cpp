#include "MeshDataLight.h"
#include <Core/ErrorHandling.h>

AABBV3 MeshDataLight::bounds() const
{
	if (mVec3.size())
	{
		return AABBV3(mVec3);
	}
	else if (mVec4.size())
	{
		return convertToV3(AABBV4(mVec4));
	}
	else
	{
		throw NMSLogicException("Data for MeshDataLight not set.");
	}
}
