// SPDX-FileCopyrightText: © 2023 Kim Eun-su <eunsu0402@gmail.com>
// SPDX-License-Identifier: LGPL-3.0-linking-exception

#include "main.h"


namespace ns_test2
{
	using namespace std;
	using namespace juce;
	using namespace uniq;
	
	class hierarchy
	{
	private:
		std::vector<std::weak_ptr<hierarchy>> child_list_;
		std::vector<std::function<void(std::any)>> chain_func_list_;
	//protected:
	public:
		
		template<typename T>
		class chain
		{
			struct sync_data
			{
				std::function<void(std::any)> chain_func;
				std::map<hierarchy*, chain<T>*> sync_map;
			};
		private:
			shared_ptr<T> value_;
			bool sync_{};
			std::map<type_index, sync_data> sync_map_;
			
			template<typename K>
			void sync_type_register(chain<T> K::* member_ptr)
			{
				cout << "sync_type_register: " << typeid(member_ptr).name() << "\n";
				cout << "K: " << typeid(K).name() << "\n";
				
				if (auto [iter, inserted] = sync_map_.try_emplace(typeid(K*)); inserted)
				{
					auto& data = iter->second;
					data.chain_func = [&data, &member_ptr](std::any obj)
					{
						auto class_ptr = std::any_cast<K*>(obj);
						cout << "obj: " << obj.type().name() << "\n";
						cout << "chain_connect: " << typeid(&(class_ptr->*member_ptr)).name() << "\n";
						cout << "chain_connect: " << typeid(class_ptr).name() << "\n";
						cout << "data_pointer: " << &data << "\n";
						cout << "data.sync_map.size(): " << data.sync_map.size() << "\n";
						data.sync_map.try_emplace(class_ptr, &(class_ptr->*member_ptr));
					};
				}
			}
		public:
			chain(hierarchy* parent, T value) : chain(parent, value, false){};
			template<typename... Others>
			chain(hierarchy* parent, T value, bool sync, Others... others)
			{
				value_ = std::make_shared<T>(value);
				sync_ = sync;
				
				// 외부에 등록
				if (parent)
				{
					parent->chain_func_list_.emplace_back([&](std::any class_ptr)
					{
						//sync_map_에 class_name 있는지 확인
						cout << "class_name: " << class_ptr.type().name() << "\n";
						if (auto it = sync_map_.find(class_ptr.type()); it != sync_map_.end())
						{
							it->second.chain_func(class_ptr);
						}
					});
				}
				
				// sync_type_register 호출
				(sync_type_register(others), ...);
				
				
			}
			
		};
	public:
		template<typename T>
		void child_add(std::shared_ptr<T>& child)
		{
			child_list_.emplace_back(child);
			for (auto& f: chain_func_list_)
			{
				f(child.get());
			}
		}
	};
	
	class test_class2 : public hierarchy
	{
	public:
		chain<int> value_{this, 0, false, &test_class2::value_};
	};
	
	class test_class : public hierarchy
	{
	private:
		chain<int> value_{this, 0, false, &test_class2::value_, &test_class::value_};
	};
}

using namespace ns_test2;

int test2()
{
	//메모: decltype: 타입 추론
	
	auto tc1_1 = make_shared<test_class>();
	auto tc1_2 = make_shared<test_class>();
	auto tc2_1 = make_shared<test_class2>();
	auto tc2_2 = make_shared<test_class2>();
	auto tc2_3 = make_shared<test_class2>();
	
	tc1_1->child_add(tc2_1);
	return 0;
}