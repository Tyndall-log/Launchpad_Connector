<!--
SPDX-FileCopyrightText: © 2023 Kim Eun-su <eunsu0402@gmail.com>
SPDX-License-Identifier: LGPL-3.0-linking-exception
-->

Launchpad Connector
============

[![UNIQ](https://custom-icon-badges.demolab.com/badge/-UNIQ-FF8F00.svg)](https://fragrant-alarm-7d3.notion.site/UNIQ-fad2be60e85742268cabce1f06184ac1)
[![JUCE](https://custom-icon-badges.demolab.com/badge/JUCE_v7.0.8-F38D48.svg?logo=JUCE%2032x32)](https://juce.com/)
![C++](https://img.shields.io/badge/-C++20-F34B7D.svg?logo=c%2B%2B&style=flat)
[![license](https://custom-icon-badges.demolab.com/badge/license-LGPL--3.0--linking--exception-green.svg?logo=law)](https://spdx.org/licenses/LGPL-3.0-linking-exception.html)
[![REUSE status](https://api.reuse.software/badge/github.com/Tyndall-log/Launchpad_Connector)](https://api.reuse.software/info/github.com/Tyndall-log/Launchpad_Connector)

**Launchpad Connector**는 [UNIQ_Library](https://github.com/Tyndall-log/UNIQ_Library)를 위해 개발된 런치패드 연결용 핵심 라이브러리입니다.  
크로스 플렛폼을 지원하도록 개발되었습니다.  
누구나 사용할 수 있도록 라이브러리 형태로 공개합니다.  

## 주요 기능

주요 기능은 다음과 같습니다.
- 편리한 midi 메시지 입출력
- 별도의 우선 스레드를 통한 즉각적인 midi 메시지 반응
- 대량 midi 메시지 결합 전송 지원
- 실시간 midi 메지시 전송 지원
- 전처리를 통한 중복이거나 불필요한 midi 메시지 제거
- 런치패드 전용 간편한 고수준 함수
- 런치패드 연결 자동화

모듈별 상세 기능은 다음과 같습니다.
- core 모듈
	- 객체 소유권 관리 자동화
	- 객체 ID 번호 할당 및 ID 번호로 객체 특정
	- 상하위 객체의 연결 관리 자동화 
	- 상하위 객체의 상속 여부에 따른 변수 값 자동 동기화
	- 메시지 스레드 생성
- launchpad 모듈
	- 모든 Novation 런치패드 지원[^각주_런치패드]
	- 연결된 런치패드 목록(기종 확인)
	- 런치패드 연결 자동화
	- 런치패드 입력 감지
	- 런치패드 버튼(LED) 색 설정 ({고정 빈도, 즉시 전송} 모드 지원)
	- 프로그래머 모드 자동 진입

[^각주_런치패드]: 2023년 12월 이전에 출시된 모든 런치패드 기종


## 프로젝트 구조
- **LICENSES**  <sub>*라이선스 고지*</sub>
- **api**  <sub>*예시 api 함수<sub>ㅎ</sub>*</sub>
- **module**  <sub>*필요 모듈이 있는 폴더*</sub>
	- **core**  <sub>*핵심 함수*</sub>
		- **console_test**  <sub>*모듈 테스트 및 활용 예제*</sub>
		- **header**  <sub>*헤더 폴더*</sub>
		- **source**  <sub>*소스 폴더*</sub>
	- **juce**  <sub>*JUCE 모듈 <sub>Cmake에 의해 자동 설치</sub>*</sub>
	- **launchpad**  <sub>*런치패드 모듈*</sub>
		- [core와 동일 구조]

## 문서

내부 함수에 대한 자세한 정보는 소스 파일 내 주석(한국어)을 참고해 주세요.  
각 모듈에 대한 사용 예제는 해당 폴더 내 console_test 폴더를 참고해 주세요.

## Q&A

### Q. 왜 함수가 명사-동사 순의 스네이크 케이스로 되어 있나요?

A. 스네이크 케이스를 채택한 이유는 대소문자 구분이 영어에만 존재하는 특성이라 다른 언어에 적용하기 어려운 점과,
카멜 케이스나 파스칼 케이스보다 높은 가독성 때문입니다.  
또한, 일반적인 동사-명사 순서가 아닌 명사-동사 순서를 채택한 이유는 IDE의 자동완성 기능을 최대한 활용하기 위함입니다.
명사를 먼저 입력하면 해당 명사와 관련된 함수들이 나열되어, 사용 가능한 함수들을 쉽게 확인할 수 있습니다.

## 종속성

[JUCE](https://www.juce.com/)를 사용합니다.  
Cmake 구성에 의해 필요한 종속성이 자동으로 설치됩니다.

## 빌드

c++ 컴파일러와 [JUCE](https://www.juce.com/) 프레임워크가 필요합니다.

### 대상

Windows, Android, MacOS, iOS, Linux

### 플렛폼 공통 최소 빌드 시스템

[JUCE 7](https://juce.com/juce-7-license/)[^각주_JUCE] <sub>Cmake에 의해 자동 설치</sub>  
c++20을 지원하는 컴파일러[^각주_컴파일러]  
Cmake 3.26 이상

[^각주_JUCE]: JUCE v7.0.8 release [github](https://github.com/juce-framework/JUCE/releases/tag/7.0.8)
[^각주_컴파일러]: MSVC v143(Visual Studio 2022) 및 x64-Clang(v16.0.5)에서 테스트 되었으며, 다른 컴파일러는 추가적인 설정과 인클루드가 필요할 수 있습니다.

### Windows
Windows 환경에서 cmake를 구성해야 합니다.
추가적인 설정이 없습니다.

### Android
Windows 환경에서 cmake를 구성해야 합니다.  
아래 cmake 옵션으로 빌드 하세요.  
```-DANDROID_ABI=arm64-v8a -DANDROID_PLATFORM=android-21 -DCMAKE_TOOLCHAIN_FILE=<your NDK path>\ndk\<version>\build\cmake\android.toolchain.cmake```  
x86 아키텍처는 ```-DANDROID_ABI=armeabi-v7a```로 설정합니다.

### MacOS / iOS
MacOS 환경에서 cmake를 구성해야 합니다.  

### Linux
Linux 환경에서 cmake를 구성해야 합니다.  


## 라이선스

본 리포지토리의 모든 소스는 [LGPL-3.0-linking-exception](https://spdx.org/licenses/LGPL-3.0-linking-exception.html) 라이선스를 따릅니다. 이 라이선스는 LGPL-3.0의 조건을 따르되, 다른 소프트웨어와 링킹할 때 해당 소프트웨어가 LGPL의 일부 제한적인 요건을 따르지 않아도 되는 예외를 허용합니다.

본 라이브러리는 JUCE의 특정 모듈(juce_core, juce_events, juce_audio_devices)을 사용합니다. 이 모듈들은 ISC 라이선스 하에 있으며[^각주_JUCE_ISC], 이는 매우 관대한 라이선스로 GPL 및 LGPL 라이선스와 호환됩니다. ISC 라이선스는 사용자에게 사용, 복제, 수정, 배포에 많은 자유를 부여합니다.

따라서, 이 라이브러리를 사용하는 최종 사용자는 주로 LGPL-3.0-linking-exception 라이선스의 조건을 따라야 합니다. JUCE의 해당 모듈들이 ISC 라이선스 하에 있기 때문에, 이들 모듈을 사용함으로써 발생하는 추가적인 라이선스 부담은 없습니다.

### JUCE 종속성에 대한 중요한 안내
본 라이브러리는 CMake 구성을 통해 JUCE 라이브러리의 소스 코드를 자동으로 설치합니다. 이 과정에서 ISC 라이선스[^각주_JUCE_ISC]를 따르지 않는 다른 JUCE 모듈들도 함께 설치됩니다. 사용자가 이러한 다른 모듈들을 추가적으로 사용하기로 결정할 경우, 해당 모듈들은 JUCE의 다른 라이선스 조건(예: GPL)을 따라야 합니다.

[^각주_JUCE_ISC]: JUCE 7 End User License Agreement, 조항 12.8, JUCE의 ISC 라이선스에 대한 정보는 [여기에서 확인](https://juce.com/juce-7-license/#:~:text=the%20Revenue%20Limit.-,12.8.%C2%A0ISC%20License%3A,-the%20permissive%20ISC)할 수 있습니다.