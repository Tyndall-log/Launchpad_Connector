cmake_minimum_required(VERSION 3.26)  #cmake 최소 버전 설정
project(Launchpad_Connector)  #프로젝트 이름 설정

set(CMAKE_CXX_STANDARD 20)  #c++20 표준 사용

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
