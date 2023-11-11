cmake_minimum_required(VERSION 3.26)  #cmake 최소 버전 설정
project(UNIQ_Library_launchpad)  #프로젝트 이름 설정

set(CMAKE_CXX_STANDARD 20)  #c++20 표준 사용

# JUCE 라이브러리를 다운로드 및 빌드합니다.
set(JUCE_VERSION 7.0.7)  #juce 버전 설정
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
		SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../juce
)  #juce 다운로드 설정
FetchContent_MakeAvailable(juce)  #juce 설치

# 포함할 소스 파일을 찾습니다.
file(GLOB_RECURSE SOURCE_FILES source/*.cpp)  #source 폴더 내의 모든 cpp 파일을 찾습니다.
#message(STATUS "SOURCE_FILES: ${SOURCE_FILES}")
foreach(FILE ${SOURCE_FILES})
	string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" RELATIVE_FILE ${FILE})
	message(STATUS "Modified SOURCE_FILE: ${RELATIVE_FILE}")
endforeach()

if (NOT TARGET UNIQ_Library_core)  #core 모듈이 없다면
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../core)
endif()

function(link_library target)  #라이브러리 링크 함수
	target_link_libraries(${target} PRIVATE UNIQ_Library_core)
	target_link_libraries(${target} PRIVATE juce::juce_core)
	target_link_libraries(${target} PRIVATE juce::juce_audio_devices)
endfunction()

add_executable(UNIQ_Library_launchpad_test console_test/main.cpp ${SOURCE_FILES})  #실행 파일 생성
link_library(UNIQ_Library_launchpad_test)  #라이브러리 링크

add_library(UNIQ_Library_launchpad ${SOURCE_FILES})  #라이브러리 생성
link_library(UNIQ_Library_launchpad)  #라이브러리 링크