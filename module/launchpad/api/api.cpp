// SPDX-FileCopyrightText: © 2023 Kim Eun-su <eunsu0402@gmail.com>
// SPDX-License-Identifier: LGPL-3.0-linking-exception

#include "launchpad.h"

using namespace std;
using namespace juce;
using namespace uniq;

static shared_ptr<MainMessageThread> MMT;
static shared_ptr<AudioDeviceManager> ADM;

API void test()
{
	if (MMT) log::println("MMT가 존재합니다.");
	else
	{
		log::println("MMT가 존재하지 않습니다.");
		//Thread::initialiseJUCE(juce::getEnv(), juce::getAppContext());
		//MMT = make_shared<MainMessageThread>();
	}
	
	auto list = launchpad::get_available_output_list();
	for (auto& l : list)
	{
		cout << l.name << endl;
		log::println(l.name.toStdString());
	}

	if (list.size() <= 0)
	{
		cout << "인식된 런치패드 없음." << endl;
		log::println("인식된 런치패드 없음.");
		return;
	}
//	unique_ptr<launchpad> main_launchpad = make_unique<launchpad>(ADM, list[0]);
//	main_launchpad->program_mode_set(true);
}

API int test1(int a, int b)
{
	return a + b;
}

API const char* log_get()
{
	return log::get().c_str();
}

#ifdef ANDROID
extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	//jassert(true);
	//jassert(false);
	//std::runtime_error("JNI_OnLoad");
	return JNI_OnLoad_test(vm, reserved);
	//return JNI_VERSION_1_2;
}

extern "C" JNIEXPORT void JNICALL
Java_com_rmsl_juce_Java_juceInit(JNIEnv* env, jobject obj)
{
//	jassert(true);
//	jassert(false);
	log::println("juceInit");
	Thread::initialiseJUCE(env, obj);
	if (!MMT) MMT = std::move(make_shared<MainMessageThread>());
	if (!ADM) ADM = make_shared<AudioDeviceManager>();
}
#endif