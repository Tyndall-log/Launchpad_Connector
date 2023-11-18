// SPDX-FileCopyrightText: © 2023 Kim Eun-su <eunsu0402@gmail.com>
// SPDX-License-Identifier: LGPL-3.0-linking-exception

#include "core.h"
#include "juce_core/juce_core.h"
#include "juce_audio_devices/juce_audio_devices.h"

int main()
{
	std::cout << "Hello World!\n";
	auto str = juce::String("Hello juce!");
	std::cout << str << "\n";
	return 0;
}