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
		enum class mode : std::uint8_t { add, remove };
		static std::size_t relationship_id_; //0은 무효한 ID입니다.
		std::vector<std::function<void(std::any, mode)>> chain_func_list_;
		std::vector<hierarchy*> child_list_;
		std::vector<hierarchy*> parent_list_;
	protected:
		template<typename T>
		class chain
		{
			struct chain_id
			{
				std::size_t id;
				chain<T>* ptr;
			};
			
			struct child_data
			{
				std::function<void(std::any)> child_add;
				std::map<hierarchy*, chain<T>*> child_map;
				
				void child_remove(hierarchy* class_ptr)
				{
					if (auto it = child_map.find(class_ptr); it != child_map.end())
					{
						it->second->parent_remove(class_ptr);
						child_map.erase(it);
					}
				}
			};
			struct parent_data
			{
				std::map<hierarchy*, chain_id*> parent_map; //검색용
				std::vector<chain_id*> parent_list;
				
				~parent_data()
				{
					for (auto& [key, value]: parent_map)
					{
						delete value;
					}
				}
				
				void parent_add(hierarchy* class_ptr, chain<T>* chain_ptr, std::size_t id)
				{
					if (auto [iter, inserted] = parent_map.try_emplace(class_ptr, new chain_id{id, chain_ptr}); inserted)
					{
						parent_list.emplace_back(iter->second);
					}
				}
				
				void parent_remove(hierarchy* class_ptr)
				{
					if (auto it = parent_map.find(class_ptr); it != parent_map.end())
					{
						auto id = it->second->id;
						parent_map.erase(it);
						auto iter = std::lower_bound(parent_list.begin(), parent_list.end(), id,
							[](const chain_id* a, std::size_t b) { return a->id < b; });
						if (iter != parent_list.end())
						{
							//속도를 위해 지연 삭제 구현 필요
							//*iter = nullptr;
							parent_list.erase(iter);
						}
						delete it->second;
					}
				}
			};
		private:
			hierarchy* parent_;
			shared_ptr<T> value_;
			bool sync_{};
			std::map<std::type_index, child_data> child_map_; //child_map_[class_pointer_type] = child_data
			parent_data parent_list_;
			std::vector<parent_data*> parent_priority_list_;
			std::map<std::type_index, parent_data*> parent_priority_map_;
			
			template<typename K>
			void sync_type_register(chain<T> K::* member_ptr)
			{
				if (auto [iter, inserted] = child_map_.try_emplace(typeid(K*)); inserted)
				{
					auto& data = iter->second;
					data.child_add = [this, &data, member_ptr](std::any class_ptr_any)
					{
						auto class_ptr = std::any_cast<K*>(class_ptr_any);
						auto chain_ptr = &(class_ptr->*member_ptr);
						auto cp_data = chain_ptr->value_;
						data.child_map.try_emplace(class_ptr, chain_ptr);
						
						chain_ptr->parent_add(class_ptr, this, ++relationship_id_);
					};
				}
			}
			
			void parent_add(hierarchy* class_ptr, chain<T>* chain_ptr, std::size_t id)
			{
				if (auto it = parent_priority_map_.find(typeid(class_ptr)); it != parent_priority_map_.end())
				{
					auto* pd = it->second;
					pd->parent_add(class_ptr, chain_ptr, id);
				}
				else
				{
					parent_list_.parent_add(class_ptr, chain_ptr, id);
				}
			}
			
			void parent_remove(hierarchy* class_ptr)
			{
				if (auto it = parent_priority_map_.find(typeid(class_ptr)); it != parent_priority_map_.end())
				{
					auto* pd = it->second;
					pd->parent_remove(class_ptr);
				}
				else
				{
					parent_list_.parent_remove(class_ptr);
				}
			}
		public:
			chain(hierarchy* parent, T value) : chain(parent, value, false){};
			template<typename... Others>
			requires (... && std::is_member_pointer_v<Others>) // Others는 chain<T> K::* 형식의 멤버 포인터여야 합니다.
			chain(hierarchy* parent, T value, bool sync, Others... others)
			{
				parent_ = parent;
				value_ = std::make_shared<T>(value);
				sync_ = sync;
				
				// 외부에 등록
				if (parent)
				{
					parent->chain_func_list_.emplace_back([&](std::any class_ptr, mode mode)
					{
						switch (mode)
						{
							case mode::add: // 추가 모드
								if (auto it = child_map_.find(class_ptr.type()); it != child_map_.end())
									it->second.child_add(class_ptr);
								break;
							case mode::remove: // 삭제 모드
								if (auto it = child_map_.find(class_ptr.type()); it != child_map_.end())
									it->second.child_remove(std::any_cast<hierarchy*>(class_ptr));
								break;
							default:
								break;
						}
					});
				}
				
				// sync_type_register 호출
				(sync_type_register(others), ...);
			}
			
			void sync_set(bool sync)
			{
				if (sync_ == sync) return;
				if (sync)
				{
				
				}
				else
				{
				
				}
				sync_ = sync;
			}
		};
		
	public:
		~hierarchy()
		{
			for (auto& child: child_list_)
			{
				child->child_remove(this);
			}
			for (auto& parent: parent_list_)
			{
				parent->child_remove(this);
			}
		}
		
		template<typename T>
		requires std::is_base_of_v<hierarchy, T> // T는 hierarchy를 상속받아야 합니다.
		inline void child_add(std::shared_ptr<T>& child)
		{
			child_add(child.get());
		}
		
		template<typename T>
		requires std::is_base_of_v<hierarchy, T> // T는 hierarchy를 상속받아야 합니다.
		inline void child_remove(std::shared_ptr<T>& child)
		{
			child_remove(child.get());
		}
		
		template<typename T>
		requires std::is_base_of_v<hierarchy, T> // T는 hierarchy를 상속받아야 합니다.
		inline void parent_add(std::shared_ptr<T>& parent)
		{
			parent->child_add(this);
		}
		
		template<typename T>
		requires std::is_base_of_v<hierarchy, T> // T는 hierarchy를 상속받아야 합니다.
		inline void parent_remove(std::shared_ptr<T>& parent)
		{
			parent->child_remove(this);
		}
		
	private:
		template<typename T>
		requires std::is_base_of_v<hierarchy, T> // T는 hierarchy를 상속받아야 합니다.
		void child_add(T* child)
		{
			child_list_.emplace_back(child);
			child->parent_list_.emplace_back(this);
			for (auto& f: chain_func_list_)
			{
				f(child, mode::add);
			}
		}
		
		template<typename T>
		requires std::is_base_of_v<hierarchy, T> // T는 hierarchy를 상속받아야 합니다.
		void child_remove(T* child)
		{
			for (auto& f: chain_func_list_)
			{
				f(child, mode::remove);
			}
		}
	};
	
	std::size_t hierarchy::relationship_id_ = 0;
	
	class test_class2 : public hierarchy
	{
	public:
		chain<int> value_{this, 0, false, &test_class2::value_};
	};
	
	class test_class : public hierarchy
	{
	public:
		chain<int> value_{this, 0, false, &test_class2::value_, &test_class::value_};
	};
	
	class test_class3
	{
	
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
	auto tc3_1 = make_shared<test_class3>();
	
	std::map<type_index, int> test;
	
	tc1_1->child_add(tc2_1);
	//tc1_1->child_add(tc3_1);
	
	return 0;
}