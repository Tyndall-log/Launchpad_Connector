cmake_minimum_required(VERSION 3.28)  #cmake 최소 버전 설정
project(Launchpad_Connector)  #프로젝트 이름 설정

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
endif()
include(FetchContent)  #다운로드 도구 활성화
FetchContent_Declare(
		juce
		URL ${JUCE_URL}
)  #juce 다운로드 설정
FetchContent_MakeAvailable(juce)  #juce 설치


# 포함할 소스 파일을 찾습니다.
file(GLOB_RECURSE SOURCE_FILES source/*.cpp*)  #source 폴더 내의 모든 cpp 파일을 찾습니다.
list(FILTER SOURCE_FILES EXCLUDE REGEX "source/console_test/.*")  #console_test 폴더 내의 파일을 제외합니다.
list(FILTER SOURCE_FILES EXCLUDE REGEX "source/std/.*")  #std 폴더 내의 파일을 제외합니다.
#message(STATUS "SOURCE_FILES: ${SOURCE_FILES}")
foreach(FILE ${SOURCE_FILES})
	string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" RELATIVE_FILE ${FILE})
	message(STATUS "Modified SOURCE_FILE: ${RELATIVE_FILE}")
endforeach()

add_library(main2)

target_sources(main2
		PUBLIC
		FILE_SET CXX_MODULES FILES
		${SOURCE_FILES}
)

add_library(std2)

#set_target_properties(std2 PROPERTIES LINKER_LANGUAGE CXX)
target_sources(std2
		#PUBLIC
		PRIVATE
#		FILE_SET CXX_MODULES FILES
		source/std/module.cppm
)

add_executable(Launchpad_Connector source/console_test/main.cpp)
target_link_libraries(Launchpad_Connector std2)
target_link_libraries(Launchpad_Connector main2)
#target_link_libraries(Launchpad_Connector PRIVATE juce::juce_core)
#target_link_libraries(Launchpad_Connector PRIVATE juce::juce_audio_devices)