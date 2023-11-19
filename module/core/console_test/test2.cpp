// SPDX-FileCopyrightText: © 2023 Kim Eun-su <eunsu0402@gmail.com>
// SPDX-License-Identifier: LGPL-3.0-linking-exception

#include "main.h"

using namespace std;
using namespace juce;
using namespace uniq;

class hierarchy
{
private:
	std::vector<std::weak_ptr<hierarchy>> child_list_;
	std::vector<std::any> chain_list_;
	std::vector<std::function<void(std::shared_ptr<hierarchy>&)>> chain_func_list_;
//protected:
public:
	template<typename T>
	class chain
	{
	private:
	public:
		shared_ptr<T> value_;
		std::vector<std::any> sync_list_;
		bool sync_;
		chain(hierarchy* parent, T value, std::initializer_list<std::any> sync_list):
				chain(parent, value, true, sync_list){};
		chain(hierarchy* parent, T value, bool sync, std::initializer_list<std::any> sync_list)
		{
			value_ = make_shared<T>(value);
			sync_ = sync;
			sync_list_ = sync_list;
			if (parent)
			{
				parent->chain_list_.emplace_back(this);
				parent->chain_func_list_.emplace_back([this](std::shared_ptr<hierarchy>& child)
				{
					child->chain_list_.emplace_back(this);
				});
			}
		}
		
		chain& operator=(const T& value)
		{
			value_ = value;
			return *this;
		}
		
		operator T()
		{
			return *value_;
		}
		
		void register_sync(std::shared_ptr<hierarchy>& child)
		{
		
		}
	};
public:
	template<typename T>
	void child_add(std::shared_ptr<T>& child)
	{
		child_list_.emplace_back(child);
		
		for(auto& f : chain_func_list_)
		{
			f(child);
		}
	}
};

class test_class2 : public hierarchy
{
public:
	chain<int> value_{this, 0, false, {&test_class2::value_}};
};

class test_class : public hierarchy
{
private:
	chain<int> value_{this, 0, false, {&test_class2::value_, &test_class::value_}};
};

int test2()
{
	//메모: decltype: 타입 추론
	
	any a = &test_class2::value_;
	
	cout << a.type().name() << "\n";
	
	//auto a_value_ptr = any_cast<shared_ptr<int> test_class2::*>(a);
	try {
			auto a_value_ptr = any_cast<test_class2::chain<int> test_class::*>(a);
		test_class obj;
		cout << obj.*a_value_ptr << "\n"; // obj의 value_ 값을 출력
		cout << typeid(&obj).name() << "\n";
	} catch (const bad_any_cast& e) {
		cout << "Bad any_cast: " << e.what() << "\n";
	}
//
//	test_class2 obj;
//	cout << obj.value_ << "\n"; // obj의 value_ 값을 출력
//	//cout << obj.*a_value_ptr << "\n"; // obj의 value_ 값을 출력
	
	return 0;
}