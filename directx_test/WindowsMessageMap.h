#pragma once

#include "NormWin.h"
#include <unordered_map>
#include <string>

class WindowsMessageMap
{
public:
	WindowsMessageMap() noexcept;
	std::wstring Parse(DWORD msg, LPARAM lp, WPARAM wp) const noexcept;
private:
	std::unordered_map<DWORD, std::wstring> map;
};