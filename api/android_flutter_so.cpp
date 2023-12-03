// SPDX-FileCopyrightText: © 2023 Kim Eun-su <eunsu0402@gmail.com>
// SPDX-License-Identifier: LGPL-3.0-linking-exception

// 이 파일은 안드로이드에서 "Launchpad Connector"를 사용하기 위한 동적 라이브러리의 예제를 보여줍니다.

#include "launchpad.h"

using namespace std;
using namespace juce;
using namespace uniq;

static unique_ptr<MessageManager> main_mm;
static shared_ptr<MainMessageThread> MMT;
static shared_ptr<AudioDeviceManager> ADM;
static map<string, shared_ptr<launchpad>> launchpad_list;
static map<string, queue<vector<uint8>>> launchpad_input_queue;
static map<string, vector<uint8>> launchpad_input_queue_temp;


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
}

API int test1(int a, int b)
{
	return a + b;
}

API const char* log_get()
{
	return log::get().c_str();
}

static unique_ptr<launchpad> main_launchpad;
API void test()
{
	main_launchpad.reset();
	//auto list = launchpad::get_available_output_list();
	auto input_list = MidiInput::getAvailableDevices();
	if (0 < input_list.size())
	{
		log::println("midi 입력 목록: ");
		for(auto i = 0; i < input_list.size(); ++i)
		{
			auto l = input_list[i];
			log::println("[" + to_string(i) + "]: " + l.name.toStdString());
			log::println("    -> id: " + l.identifier.toStdString());
		}
	}
	auto output_list = MidiOutput::getAvailableDevices();
	if (0 < output_list.size())
	{
		log::println("midi 출력 목록: ");
		for(auto i = 0; i < output_list.size(); ++i)
		{
			auto l = output_list[i];
			log::println("[" + to_string(i) + "]: " + l.name.toStdString());
			log::println("    -> id: " + l.identifier.toStdString());
		}
	}
	
	if (input_list.size() <= 0 && output_list.size() <= 0)
	{
		log::println("인식된 런치패드 없음.");
		return;
	}
	main_launchpad = make_unique<launchpad>(ADM, input_list[0], output_list[0]);
	main_launchpad->program_mode_set(true);
	
	main_launchpad->input_callback_set([](uint8* data, int size)
	{
		log::println("MIDI> " + String::toHexString(data, size).toStdString());
	});
}

extern "C" JNIEXPORT void JNICALL
Java_com_rmsl_juce_Java_messagesThreadRun()
{
	auto mm = unique_ptr<MessageManager>(MessageManager::getInstance());
	//log::println(mm ? "MessageManager::getInstance() success." : "MessageManager::getInstance() failed.");
	//log::println(MessageManager::getInstanceWithoutCreating() ? "MessageManager::getInstanceWithoutCreating() success." : "MessageManager::getInstanceWithoutCreating() failed.");
	mm->runDispatchLoop();
	main_mm = std::move(mm);
	//MMT = make_shared<MainMessageThread>();
	if (!ADM)
	{
		std::promise<shared_ptr<AudioDeviceManager>> adm_promise;
		auto b = MessageManager::callAsync([&adm_promise]()
		{
			auto ADM = make_shared<AudioDeviceManager>();
			adm_promise.set_value(ADM);
		});
		if (!b)
		{
			auto msg = "MessageManager::callAsync() failed.";
			log::println(msg);
			ADM = nullptr;
			return;
		}
		ADM = adm_promise.get_future().get();
	}
}
#endif