# SPDX-FileCopyrightText: © 2023 Kim Eun-su <eunsu0402@gmail.com>
# SPDX-License-Identifier: LGPL-3.0-linking-exception

cmake_minimum_required(VERSION 3.26)  #cmake 최소 버전 설정
project(Launchpad_Connector)  #프로젝트 이름 설정

set(CMAKE_CXX_STANDARD 20)  #c++20 표준 사용

if(ANDROID)
	# 참고: https://atsushieno.github.io/2021/01/16/juce-cmake-android-now-works.html
	set(OBOE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/module/juce/modules/juce_audio_devices/native/oboe")
	add_subdirectory(${OBOE_DIR} ./oboe)

	add_library(cpufeatures STATIC "${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c")
	#set_source_files_properties("${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c")
	add_definitions("-DJUCE_ANDROID=1" "-DJUCE_ANDROID_API_VERSION=16" "-DJUCE_PUSH_NOTIFICATIONS=1"
			"-DJUCER_ANDROIDSTUDIO_7F0E4A25=1"
    "-DJUCE_APP_VERSION=1.0.0" "-DJUCE_APP_VERSION_HEX=0x10000")
	include_directories(AFTER "${ANDROID_NDK}/sources/android/cpufeatures")
endif()

set(CMAKE_SYSTEM_NAME Windows)  #시스템 이름 설정

# JUCE 라이브러리를 다운로드 및 빌드합니다.
set(JUCE_VERSION 7.0.8)  #juce 버전 설정
set(JUCE_URL "https://github.com/juce-framework/JUCE/releases/download/")  #juce 다운 경로
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	set(JUCE_URL "${JUCE_URL}${JUCE_VERSION}/juce-${JUCE_VERSION}-windows.zip")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	set(JUCE_URL "${JUCE_URL}${JUCE_VERSION}/juce-${JUCE_VERSION}-linux.zip")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
	set(JUCE_URL "${JUCE_URL}${JUCE_VERSION}/juce-${JUCE_VERSION}-osx.zip")
else()
	message(FATAL_ERROR "Unsupported system: ${CMAKE_SYSTEM_NAME}")
endif()
include(FetchContent)  #다운로드 도구 활성화
FetchContent_Declare(
		juce
		URL ${JUCE_URL}
		SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/module/juce"  # 다운로드될 디렉토리 지정
)  #juce 다운로드 설정
FetchContent_MakeAvailable(juce)  #juce 설치

add_subdirectory(module/core)  #core 모듈 추가
add_subdirectory(module/launchpad)  #launchpad 모듈 추가

#add_executable(Launchpad_Connector module/launchpad/console_test/main.cpp)
