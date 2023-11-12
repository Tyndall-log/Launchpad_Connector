<!--
SPDX-FileCopyrightText: © 2023 Kim Eun-su <eunsu0402@gmail.com>
SPDX-License-Identifier: LGPL-3.0-linking-exception
-->

Launchpad Connector
============

[![UNIQ](https://custom-icon-badges.demolab.com/badge/-UNIQ-FF8F00.svg)](https://fragrant-alarm-7d3.notion.site/UNIQ-fad2be60e85742268cabce1f06184ac1)
[![JUCE](https://custom-icon-badges.demolab.com/badge/JUCE_v7.0.7-F38D48.svg?logo=JUCE%2032x32)](https://juce.com/)
![C++](https://img.shields.io/badge/-C++20-F34B7D.svg?logo=c%2B%2B&style=flat)
[![license](https://custom-icon-badges.demolab.com/badge/license-LGPL--3.0--linking--exception-green.svg?logo=law)](https://spdx.org/licenses/LGPL-3.0-linking-exception.html)
[![REUSE status](https://api.reuse.software/badge/github.com/IreneStella/Launchpad_Connector)](https://api.reuse.software/info/github.com/IreneStella/Launchpad_Connector)

**Launchpad Connector**는 [UNIQ_Library](https://github.com/Tyndall-log/UNIQ_Library)를 위해 개발된 런치패드 연결용 핵심 라이브러리입니다.  
누구나 사용할 수 있도록 라이브러리 형태로 공개합니다.  

## 주요 기능

주요 기능은 다음과 같습니다.
- 런치패드 연결 자동화
    - 모든 런치패드 지원[^각주_런치패드]

[^각주_런치패드]: 2023년 11월 이전에 출시된 모든 런치패드 기종

## 문서

내부 함수에 대한 정보는 소스 파일 내 주석(한국어)을 참고해 주세요.  

## 종속성

[JUCE](https://www.juce.com/)가 필요합니다.

## 빌드

c++ 컴파일러와 [JUCE](https://www.juce.com/) 프레임워크가 필요합니다.  

### 최소 빌드 시스템

[JUCE 7](https://juce.com/juce-7-license/)[^각주_JUCE]  
c++20을 지원하는 컴파일러[^각주_컴파일러]  
Cmake 3.26 이상

[^각주_JUCE]: JUCE v7.0.7 release [github](https://github.com/juce-framework/JUCE/releases/tag/7.0.7)
[^각주_컴파일러]: MSVC v143(Visual Studio 2022) 및 x64-Clang(v16.0.5)에서 테스트 되었으며, 다른 컴파일러는 추가적인 설정과 인클루드가 필요할 수 있습니다.

### 대상

windows

## 라이선스

본 리포지토리의 모든 소스는 [LGPL-3.0-linking-exception](https://spdx.org/licenses/LGPL-3.0-linking-exception.html) 라이선스를 따릅니다.