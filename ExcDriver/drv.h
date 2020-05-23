#ifndef WNDHIJACK_DRIVER_DEFS_HPP
#define WNDHIJACK_DRIVER_DEFS_HPP

#include "ExternalFunctions.h"
#include <ntifs.h>
#include <stdint.h>

namespace wnd_hjk
{
	struct image_data_t
	{
		uint64_t base_address;
		uint32_t image_size;
	};
	constexpr ULONG PoolTag = 'LOOB';
}

#endif