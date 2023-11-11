module;

//#include <string>
//#include <compare>

#include <iostream>
export module launchpad;
//export import :launchpad2;
//import <iostream>;

export class launchpad {
public:
	launchpad();
	~launchpad();
	void helloworld();
	void test()
	{
		std::cout << "test\n";
	}
};

launchpad::launchpad() = default;
launchpad::~launchpad() = default;
void launchpad::helloworld() {
	//std::cout << "hello world and hi tyndall.log!\n";
	//multiply(2, 3);
}

//void launchpad::test()
//{
//	std::cout << "test\n";
//}


export namespace uniq
{
	class test_class
	{
	public:
		test_class() = default;
		~test_class() = default;
		void helloworld()
		{
			std::cout << "[test_class] hello world and hi tyndall.log!\n";
		}
		void test()
		{
			helloworld();
		}
	};
}
