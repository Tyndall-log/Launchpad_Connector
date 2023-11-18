// SPDX-FileCopyrightText: © 2023 Kim Eun-su <eunsu0402@gmail.com>
// SPDX-License-Identifier: LGPL-3.0-linking-exception

#include <iostream>
#include "core.h"

using namespace std;
using namespace juce;
using namespace uniq;

class A : public ID<A>
{
protected:
	A() = default;
public:
	static std::shared_ptr<A> create()
	{
		return ID<A>::create();
	}
	std::string get_name()
	{
		return "Class A";
	}
};

class B : public ID<B>
{
protected:
	B() = default;
public:
	static std::shared_ptr<B> create()
	{
		return ID<B>::create();
	}
	std::string get_name()
	{
		return "Class B";
	}
};

class C : public ID<C>
{
protected:
	C() = default;
public:
	static std::shared_ptr<C> create()
	{
		return ID<C>::create();
	}
	std::string get_name()
	{
		return "Class C";
	}
};


class Outer {
private:
	int value;
	int value2;
public:
	class Inner1 {
		Outer& outer;
	public:
		Inner1(Outer& outer) : outer(outer) {}
		void someMethod(int x) {
			outer.value = x;
		}
	} Inner1;
	class Inner2 {
		Outer& outer;
	public:
		Inner2(Outer& outer) : outer(outer) {}
		void someMethod(int x) {
			outer.value = x;
		}
	} Inner2;
	// ...
	class Inner100 {
		Outer& outer;
	public:
		Inner100(Outer& outer) : outer(outer) {}
		void someMethod(int x) {
			outer.value = x;
		}
	} Inner100;
};

int main()
{
	system("chcp 65001"); //한글 설정
	
	cout << "Hello World!\n";
	
	MainMessageThread MMT;
	
	auto str = String("Hello juce!");
	cout << str << "\n";
	
	auto a = A::create();
	auto b = B::create();
	auto c = C::create();
	//c->test();
	
	//객체 크기 확인
	std::cout << "sizeof(A): " << sizeof(A) << "\n";
	std::cout << "sizeof(B): " << sizeof(B) << "\n";
	std::cout << "sizeof(a): " << sizeof(a) << "\n";
	std::cout << "sizeof(b): " << sizeof(b) << "\n";
	
	auto a_from_ID = ID_manager::get_shared_ptr_by_ID<A>(a->get_ID()).value_or(nullptr);
	if (a_from_ID)
	{
		std::cout << "ID " << a->get_ID() << ": " << a_from_ID->get_name() << "\n";
	}
	
	auto b_from_ID = ID_manager::get_shared_ptr_by_ID<B>(b->get_ID()).value_or(nullptr);
	if (b_from_ID)
	{
		std::cout << "ID " << b->get_ID() << ": " << b_from_ID->get_name() << "\n";
	}
	
	auto test = ID_manager::get_shared_ptr_by_ID<B>(a->get_ID()).value_or(nullptr);
	if (test)
	{
		std::cout << "ID " << a->get_ID() << ": " << test->get_name() << "\n";
	}
	else
	{
		std::cout << "ID " << a->get_ID() << ": " << "nullptr" << "\n";
	}
	
	auto id_2 = 5;
	auto test2 = ID_manager::get_shared_ptr_by_ID<A>(id_2).value_or(nullptr);
	if (test)
	{
		std::cout << "ID " << id_2 << ": " << test->get_name() << "\n";
	}
	else
	{
		std::cout << "ID " << id_2 << ": " << "nullptr" << "\n";
	}
	
	cout << "KK size: " << sizeof(KK) << "\n";
	
	return 0;
}