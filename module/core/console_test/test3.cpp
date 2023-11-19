#include "main.h"


namespace ns_test3
{
	using namespace std;
	
	class test_class
	{
	public:
		int value_ = 10;
	};
	
	class test_class2
	{
	public:
		int value_ = 20;
	};
	
	class test_class3
	{
	public:
		int value_ = 30;
	};
	
	class MemberValueRetriever
	{
	private:
		std::unordered_map<std::type_index, std::function<int(std::any)>> valueRetrievers
				= std::unordered_map<std::type_index, std::function<int(std::any)>>();
	
	public:
		template<typename T>
		void register_member(int T::* memberPtr)
		{
			std::cout << std::type_index(typeid(T)).name() << "\n";
			valueRetrievers[std::type_index(typeid(T))] = [memberPtr](std::any obj) -> int {
				if (T* pObj = std::any_cast<T>(&obj)) {
					return pObj->*memberPtr;
				}
				return -1; // 혹은 적절한 기본값 반환
			};
		}
		
		template<typename T>
		int get_value(T *obj)
		{
			std::type_index typeIndex = std::type_index(typeid(*obj));
			if (valueRetrievers.find(typeIndex) != valueRetrievers.end()) {
				//std::cout << "found\n";
				return valueRetrievers[typeIndex](std::any(*obj));
			}
			return -1; // 혹은 적절한 기본값 반환
		}
	};
	
	class hierarchy
	{
	private:
		std::vector<std::any> chain_list;
	public:
		template<typename T>
		class chain
		{
		protected:
			template<typename K>
			void sync_register(std::shared_ptr<K>& child)
			{
				cout << "sync_register: " << typeid(K).name() << "\n";
			}
		};
	public:
		template<typename T>
		void child_add(std::shared_ptr<T>& child)
		{
			//for(auto& c : chain_list)
			//	c->sync_register(child);
		}
	};
}

using namespace ns_test3;

int test3()
{
	MemberValueRetriever retriever;
	retriever.register_member(&test_class::value_);
	retriever.register_member(&test_class2::value_);
	
	test_class tc;
	test_class2 tc2;
	test_class3 tc3;
	
	std::cout << "test_class value: " << retriever.get_value(&tc) << std::endl;
	std::cout << "test_class2 value: " << retriever.get_value(&tc2) << std::endl;
	std::cout << "test_class3 value: " << retriever.get_value(&tc3) << std::endl;
	
	return 0;
}