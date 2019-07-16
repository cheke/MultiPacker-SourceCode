/* Copyright 2019 TurboCheke, Estudio Cheke  - All Rights Reserved */
#pragma once

#include "Modules/ModuleManager.h"

#define MULTIPACKER_MODULE_NAME "MultiPackerRuntime"

DECLARE_LOG_CATEGORY_EXTERN(MultiPackerRuntime, Log, All);

/**
 * The public interface to this module
 */
class IMultiPackerRuntime : public IModuleInterface
{

public:

	/**
	 * Singleton-like access to this module's interface.  This is just for convenience!
	 * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static IMultiPackerRuntime& Get()
	{
		return FModuleManager::LoadModuleChecked< IMultiPackerRuntime >("MultiPackerRuntime");
	}

	/**
	 * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded( "MultiPackerRuntime" );
	}
};

