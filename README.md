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
누구나 사용할 수 있도록 라이브러리 형태로 공개합니다.  

## 주요 기능

주요 기능은 다음과 같습니다.
- 런치패드 연결 자동화
    - 모든 런치패드 지원[^각주_런치패드]

- core 라이브러리
    - 객체 소유권 및 ID 관리

[^각주_런치패드]: 2023년 11월 이전에 출시된 모든 런치패드 기종

## 문서

내부 함수에 대한 정보는 소스 파일 내 주석(한국어)을 참고해 주세요.  

## Q&A

### Q. 왜 함수가 명사-동사 순의 스네이크 케이스로 되어 있나요?

A. 스네이크 케이스를 채택한 이유는 대소문자 구분이 영어에만 존재하는 특성이라 다른 언어에 적용하기 어려운 점과,
카멜 케이스나 파스칼 케이스보다 높은 가독성 때문입니다.  
또한, 일반적인 동사-명사 순서가 아닌 명사-동사 순서를 채택한 이유는 IDE의 자동완성 기능을 최대한 활용하기 위함입니다.
명사를 먼저 입력하면 해당 명사와 관련된 함수들이 나열되어, 사용 가능한 함수들을 쉽게 확인할 수 있습니다.

## 종속성

[JUCE](https://www.juce.com/)가 필요합니다.

## 빌드

c++ 컴파일러와 [JUCE](https://www.juce.com/) 프레임워크가 필요합니다.  
안드로이드는 아래 cmake 옵션으로 빌드 하세요.
```-DANDROID_ABI=armeabi-v7a -DANDROID_PLATFORM=android-21 -DCMAKE_TOOLCHAIN_FILE=<your NDK path>\ndk\<version>\build\cmake\android.toolchain.cmake```

### 최소 빌드 시스템

[JUCE 7](https://juce.com/juce-7-license/)[^각주_JUCE]  
c++20을 지원하는 컴파일러[^각주_컴파일러]  
Cmake 3.26 이상

[^각주_JUCE]: JUCE v7.0.8 release [github](https://github.com/juce-framework/JUCE/releases/tag/7.0.7)
[^각주_컴파일러]: MSVC v143(Visual Studio 2022) 및 x64-Clang(v16.0.5)에서 테스트 되었으며, 다른 컴파일러는 추가적인 설정과 인클루드가 필요할 수 있습니다.

### 대상

windows

## 라이선스

본 리포지토리의 모든 소스는 [LGPL-3.0-linking-exception](https://spdx.org/licenses/LGPL-3.0-linking-exception.html) 라이선스를 따릅니다.