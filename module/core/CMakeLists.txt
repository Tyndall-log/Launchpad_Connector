cmake_minimum_required(VERSION 3.26)  #cmake 최소 버전 설정
project(UNIQ_Library_core)  #프로젝트 이름 설정

set(CMAKE_CXX_STANDARD 20)  #c++20 표준 사용

# 포함할 소스 파일을 찾습니다.
file(GLOB_RECURSE SOURCE_FILES source/*.cpp)  #source 폴더 내의 모든 cpp 파일을 찾습니다.
#message(STATUS "SOURCE_FILES: ${SOURCE_FILES}")
foreach(FILE ${SOURCE_FILES})
	string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" RELATIVE_FILE ${FILE})
	message(STATUS "Modified SOURCE_FILE: ${RELATIVE_FILE}")
endforeach()

add_executable(UNIQ_Library_core_test console_test/main.cpp ${SOURCE_FILES})  #실행 파일을 만듭니다.
target_include_directories(UNIQ_Library_core_test PUBLIC source)  #실행 파일에 source 폴더를 포함합니다.
add_library(UNIQ_Library_core ${SOURCE_FILES})  #core 라이브러리를 만듭니다.
target_include_directories(UNIQ_Library_core PUBLIC source)  #core 라이브러리에 source 폴더를 포함합니다.