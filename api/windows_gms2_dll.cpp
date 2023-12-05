// SPDX-FileCopyrightText: © 2023 Kim Eun-su <eunsu0402@gmail.com>
// SPDX-License-Identifier: LGPL-3.0-linking-exception

// 이 파일은 외부 어플리케이션에서 "Launchpad Connector"를 사용하기 위한 동적 라이브러리의 예제를 보여줍니다.
// 본 라이브러리를 외부 프로그램인 GMS2(Gamemaker Studio 2)에서 사용하기 위해 windows용 DLL빌드합니다.
// 특히, GMS2에서는 DLL과 오로지 char*, double만 주고 받을 수 있는 제약사항이 있습니다.
// 이 예제 목표는 이런 제약사항에서도 효과적이고 잘 동작하는 방법을 보여주는 것입니다.

#include "launchpad.h"

using namespace std;
using namespace juce;
using namespace uniq;

static shared_ptr<MainMessageThread> MMT;
static shared_ptr<AudioDeviceManager> ADM;
static map<string, shared_ptr<launchpad>> launchpad_list;
static map<string, queue<vector<uint8>>> launchpad_input_queue;
static map<string, vector<uint8>> launchpad_input_queue_temp;

API const char* version()
{
	return "0.6.9"; //juce 기반으로 동작하는 uniq 라이브러리의 최소 버전
}

// launchpad_connector_init()를 호출해야만 런치패드를 사용할 수 있습니다.
API void launchpad_connector_init()
{
	if (!MMT) MMT = make_shared<MainMessageThread>(); //메시지 관리 스레드 시작
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


//런치패드의 입력 목록을 "\n"으로 구분하여 반환합니다.
API char* midi_input_list()
{
	auto list = launchpad::get_available_input_list();
	static string str;
	str = "";
	for (auto& i : list)
	{
		str += i.name.toStdString() + "\\"s;
	}
	return (char*)str.c_str();
}

//N 번째 런치패드를 연결합니다.
API double midi_input_open(double N)
{
	auto N_t = static_cast<size_t>(N);
	auto list = launchpad::get_available_input_list();
	if (list.size() <= N_t) return 0; //인덱스 초과

	auto& element = list[N_t];
	auto [iter, success] = launchpad_list.try_emplace(element.identifier.toStdString(), nullptr);
	if (success)
	{
		iter->second = launchpad::create(ADM);
	}
	auto& lp = iter->second;
	lp->midi_input_set(element);
	auto lp_identifier = lp->input_identifier_get();
	lp->input_callback_set([lp_identifier](uint8_t* data, int length)
	{
		launchpad_input_queue[lp_identifier].emplace(data, data + length);
	});
	return static_cast<double>(lp->ID_get());
}

//midi_ID에 들어온 메시지 입력의 크기를 반환합니다.
API double midi_input_num(double midi_ID)
{
	//log::println("midi_input_num("s + to_string(midi_ID) + ")");
	auto midi_ID_t = static_cast<size_t>(midi_ID);
	auto lp_opt = ID_manager::get_shared_ptr_by_ID<launchpad>(midi_ID_t);
	if (!lp_opt) return 0;
	auto lp = lp_opt.value();
	auto lp_identifier = lp->input_identifier_get();
	auto& queue = launchpad_input_queue[lp_identifier];
	if (queue.empty()) return 0;
	auto data = queue.front();
	queue.pop();
	launchpad_input_queue_temp[lp_identifier] = data;
	return static_cast<double>(data.size());
}

//midi_ID에 들어온 메시지 입력을 반환합니다.
API double midi_input_array(double midi_ID, double index)
{
	//log::println("midi_input_array("s + to_string(midi_ID) + ", "s + to_string(index) + ")");
	auto midi_ID_t = static_cast<size_t>(midi_ID);
	auto lp_opt = ID_manager::get_shared_ptr_by_ID<launchpad>(midi_ID_t);
	if (!lp_opt) return 0;
	auto lp = lp_opt.value();
	auto lp_identifier = lp->input_identifier_get();
	auto& data = launchpad_input_queue_temp[lp_identifier];
	auto index_t = static_cast<size_t>(index);
	if (data.size() <= index_t) return 0;
	return static_cast<double>(data[index_t]);
}

//런치패드를 연결 해제합니다.
API void midi_input_close(double midi_ID)
{
	auto midi_ID_t = static_cast<size_t>(midi_ID);
	auto lp_opt = ID_manager::get_shared_ptr_by_ID<launchpad>(midi_ID_t);
	if (!lp_opt) return;
	auto lp = lp_opt.value();
	launchpad_list.erase(lp->output_identifier_get());
}

//런치패드의 출력 목록을 "\n"으로 구분하여 반환합니다.
API char* midi_output_list()
{
	auto list = launchpad::get_available_output_list();
	static string str;
	str = "";
	for (auto& i : list)
	{
		str += i.name.toStdString() + "\\"s;
	}
	return (char*)str.c_str();
}

//N 번째 런치패드를 연결합니다.
API double midi_output_open(double N)
{
	auto N_t = static_cast<size_t>(N);
	auto list = launchpad::get_available_output_list();
	if (list.size() <= N_t) return 0; //인덱스 초과
	auto& element = list[N_t];
	auto [iter, success] = launchpad_list.try_emplace(element.identifier.toStdString(), nullptr);
	if (success)
	{
		iter->second = launchpad::create(ADM);
	}
	auto& lp = iter->second;
	lp->midi_output_set(element);
	lp->program_mode_set(true); //프로그램 모드로 설정
	return static_cast<double>(iter->second->ID_get());
}

//런치패드의 LED를 설정합니다.
API void midi_output_rgb_set(double midi_ID, double x, double y, double r, double g, double b)
{
	auto midi_ID_t = static_cast<size_t>(midi_ID);
	auto lp_opt = ID_manager::get_shared_ptr_by_ID<launchpad>(midi_ID_t);
	if (!lp_opt) return;
	auto lp = lp_opt.value();
	lp->rgb_set(static_cast<uint8>(x), static_cast<uint8>(y), static_cast<uint8>(r), static_cast<uint8>(g), static_cast<uint8>(b));
}

//런치패드의 velocity를 설정합니다.
API void midi_output_velocity_set(double midi_ID, double x, double y, double v)
{
	auto midi_ID_t = static_cast<size_t>(midi_ID);
	auto lp_opt = ID_manager::get_shared_ptr_by_ID<launchpad>(midi_ID_t);
	if (!lp_opt) return;
	auto lp = lp_opt.value();
	lp->velocity_set(static_cast<uint8>(x), static_cast<uint8>(y), static_cast<uint8>(v));
}

//런치패드의 LED를 강제 업데이트합니다.
API double midi_output_send(double midi_ID)
{
	auto midi_ID_t = static_cast<size_t>(midi_ID);
	auto lp_opt = ID_manager::get_shared_ptr_by_ID<launchpad>(midi_ID_t);
	if (!lp_opt) return 0;
	auto lp = lp_opt.value();
	lp->LED_send();
	return 0;
}

//런치패드의 프로그램 모드를 설정합니다.
API void midi_output_program_mode_set(double midi_ID, double mode)
{
	auto midi_ID_t = static_cast<size_t>(midi_ID);
	auto lp_opt = ID_manager::get_shared_ptr_by_ID<launchpad>(midi_ID_t);
	if (!lp_opt) return;
	auto lp = lp_opt.value();
	lp->program_mode_set(mode != 0);
}

//런치패드의 자동 전송 모드를 설정합니다.
API void midi_output_automatic_transmission_set(double midi_ID, double mode)
{
	auto midi_ID_t = static_cast<size_t>(midi_ID);
	auto lp_opt = ID_manager::get_shared_ptr_by_ID<launchpad>(midi_ID_t);
	if (!lp_opt) return;
	auto lp = lp_opt.value();
	lp->automatic_transmission_set(mode != 0);
}

//런치패드의 즉시 전송 모드를 설정합니다.
API void midi_output_immediate_transmission_set(double midi_ID, double mode)
{
	auto midi_ID_t = static_cast<size_t>(midi_ID);
	auto lp_opt = ID_manager::get_shared_ptr_by_ID<launchpad>(midi_ID_t);
	if (!lp_opt) return;
	auto lp = lp_opt.value();
	lp->immediate_transmission_set(mode != 0);
}

//런치패드의 자동 전송 모드의 전송 주기(ms)를 설정합니다.
API void midi_output_immediate_transmission_global_timer_set(double period)
{
	launchpad::immediate_transmission_global_timer_set(static_cast<int>(period));
}

//런치패드를 연결 해제합니다.
API double midi_output_close(double midi_ID)
{
	auto midi_ID_t = static_cast<size_t>(midi_ID);
	auto lp_opt = ID_manager::get_shared_ptr_by_ID<launchpad>(midi_ID_t);
	if (!lp_opt) return 0;
	auto lp = lp_opt.value();
	launchpad_list.erase(lp->output_identifier_get());
	return 0;
}
