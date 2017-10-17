
#include "c4d.h"
#include "main.h"

Bool PluginStart(void)
{
	// menu plugin examples
	if (!RegisterGraphEasyObjectID())
		return false;

	return true;
}

void PluginEnd(void)
{
}

Bool PluginMessage(Int32 id, void* data)
{
	switch (id)
	{
		case C4DPL_INIT_SYS:
			if (!resource.Init())
				return false;

			return true;
	}

	return false;
}
