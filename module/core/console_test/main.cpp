#include <iostream>
#include "core.h"

using namespace std;
using namespace juce;
using namespace uniq;

int main()
{
	cout << "Hello World!\n";
	
	MainMessageThread MMT;
	
	auto str = String("Hello juce!");
	cout << str << "\n";
}
