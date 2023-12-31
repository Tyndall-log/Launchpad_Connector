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
	add_definitions("-DJUCE_ANDROID=1")
	include_directories(AFTER "${ANDROID_NDK}/sources/android/cpufeatures")
	set(CMAKE_SYSTEM_NAME Windows)  #시스템 이름 설정
endif()

# JUCE 라이브러리를 다운로드 및 빌드합니다.
set(JUCE_VERSION 7.0.9)  #juce 버전 설정
include(FetchContent)  #다운로드 도구 활성화
FetchContent_Declare(
		juce
		GIT_REPOSITORY https://github.com/juce-framework/JUCE.git  #juce git 주소
		GIT_TAG ${JUCE_VERSION}  #juce 버전 설정
		SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/module/juce"  # 다운로드될 디렉토리 지정
)  #juce 다운로드 설정
FetchContent_MakeAvailable(juce)  #juce 설치

add_subdirectory(module/core)  #core 모듈 추가
add_subdirectory(module/launchpad)  #launchpad 모듈 추가


function(link_library target)  #라이브러리 링크 함수
	target_link_libraries(${target} PRIVATE UNIQ_Library_core)
	target_link_libraries(${target} PRIVATE UNIQ_Library_launchpad)
	target_link_libraries(${target} PRIVATE juce::juce_core)
	target_link_libraries(${target} PRIVATE juce::juce_audio_devices)
	if (ANDROID)
		target_link_libraries(${target} PRIVATE oboe)
		target_link_libraries(${target} PRIVATE cpufeatures)
	endif()
endfunction()

#file(GLOB EXAMPLE_FILES example/*.cpp) #example 폴더 내의 모든 cpp 파일을 찾습니다.
if (ANDROID)
	set(EXAMPLE_FILES example/android_flutter_so.cpp)
else()
	set(EXAMPLE_FILES example/windows_gms2_dll.cpp)
endif()
add_library(Launchpad_Connector_Library SHARED ${EXAMPLE_FILES})
#target_include_directories(Launchpad_Connector_Library PUBLIC header)  #라이브러리에 header 폴더를 포함합니다.
link_library(Launchpad_Connector_Library)  #라이브러리 링크

#add_executable(Launchpad_Connector module/launchpad/console_test/main.cpp)
