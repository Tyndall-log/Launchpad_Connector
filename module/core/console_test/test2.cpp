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
		std::vector<std::any> chain_list;
		//std::vector<std::any> chain_list_2;
		std::vector<std::function<void(std::any)>> chain_func_list_;
	//protected:
	public:
		
		template<typename T>
		class chain
		{
			struct sync_data
			{
				function<void(std::any)> chain_func;
				std::map<hierarchy*, chain<T>*> sync_map;
			};
		private:
			shared_ptr<T> value_;
			std::vector<function<void(T&)>> sync_func_list_;
			std::vector<chain<T>*> sync_list_;
			std::set<type_index> sync_type_list_;
			std::map<type_index, sync_data> sync_map_;
			bool sync_;
			
			template<typename K>
			void sync_type_register(chain<T> K::* member_ptr)
			{
				cout << "sync_type_register: " << typeid(member_ptr).name() << "\n";
				cout << "K: " << typeid(K).name() << "\n";
				
				auto data = sync_data();
				data.chain_func = [&data, &member_ptr](std::any obj){
					if (auto pObj = std::any_cast<K>(&obj)) {
						cout << "chain_connect: " << typeid(&(pObj->*member_ptr)).name() << "\n";
						data.sync_map[pObj] = &(pObj->*member_ptr);
					}
				};
				sync_map_.try_emplace(typeid(K), data);
			}
		protected:
			template<typename K>
			void sync_register(std::shared_ptr<K>& child)
			{
				cout << "sync_register: " << typeid(K).name() << "\n";
				sync_map_[typeid(K)].emplace(child.get(), this);
			}
		public:
			chain(hierarchy* parent, T value):
					chain(parent, value, true){};
			template<typename... Others>
			chain(hierarchy* parent, T value, bool sync, Others... others) {
				value_ = std::make_shared<T>(value);
				sync_ = sync;
				
				// 외부에 등록
				if (parent) {
					parent->chain_list.emplace_back(this);
					parent->chain_func_list_.emplace_back([&](std::any class_name){
						//sync_map_에 class_name 있는지 확인
						cout << "class_name: " << class_name.type().name() << "\n";
						if (auto it = sync_map_.find(class_name.type()); it != sync_map_.end())
						{
							it->second.chain_func(class_name);
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
				f(*child);
				cout << "chain_func_list_ called\n";
			}
		}
	};
	
	class test_class2 : public hierarchy
	{
	public:
		chain<int> value_{this, 0, false};
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
	
	auto a = make_shared<test_class>();
	auto b = make_shared<test_class2>();
	
	a->child_add(b);
	
	return 0;
}