#pragma once
#include <Windows.h>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

#include "portable_executable.hpp"
#include "utils.hpp"
#include "nt.hpp"
#include "intel_driver.hpp"

namespace kdmapper
{
	uint64_t MapDriver(HANDLE iqvw64e_device_handle, int driver);
	void RelocateImageByDelta(const portable_executable::vec_relocs& relocs, const uint64_t delta);
	bool ResolveImports(HANDLE iqvw64e_device_handle, const portable_executable::vec_imports& imports);
	void InitStackCookie(void* base);
}
