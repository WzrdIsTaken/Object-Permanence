#pragma once
#include "EngineCore.h"

#include "TypeUtils.h"

namespace nabi
{
	struct Context;
}

namespace nabi::Resource
{
	// I think it could be better to allow some flexibility here. 
	// Eg loading assets via a bitfield (see DemoAssetBank for an example)
//#define FORCE_PURE_LOAD_UNLOAD_FUNCTIONS

#ifdef FORCE_PURE_LOAD_UNLOAD_FUNCTIONS
	#define LOAD_UNLOAD_IMPLEMENTATION = 0;
#else
	#define LOAD_UNLOAD_IMPLEMENTATION { return false; };
#endif //ifdef FORCE_PURE_LOAD_UNLOAD_FUNCTIONS

	class AssetBank abstract
	{
	public:
		AssetBank(nabi::Context& context)
			: m_Context(context)
		{
		}
		virtual ~AssetBank() {}

		virtual bool LoadAssets()   LOAD_UNLOAD_IMPLEMENTATION
		virtual bool UnloadAssets() LOAD_UNLOAD_IMPLEMENTATION

	protected:
		nabi::Context& m_Context;

	private:
		DELETE_COPY_MOVE_CONSTRUCTORS(AssetBank)
	};

#undef LOAD_UNLOAD_IMPLEMENTATION
#undef FORCE_PURE_LOAD_UNLOAD_FUNCTIONS
} // namespace nabi::Resource
