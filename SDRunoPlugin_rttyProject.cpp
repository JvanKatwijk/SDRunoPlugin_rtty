#include <iunoplugin.h>
#include "SDRunoPlugin_rtty.h"

extern "C" {

	UNOPLUGINAPI IUnoPlugin* UNOPLUGINCALL CreatePlugin(IUnoPluginController& controller) {
		return new SDRunoPlugin_rtty (controller);
	}

	UNOPLUGINAPI void UNOPLUGINCALL DestroyPlugin(IUnoPlugin* plugin) {
		delete plugin;
	}

	UNOPLUGINAPI unsigned int UNOPLUGINCALL GetPluginApiLevel() {
		return UNOPLUGINAPIVERSION;
	}
}

