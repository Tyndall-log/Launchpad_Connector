#include "core.h"

using namespace std;
using namespace juce;
using namespace uniq;

API bool uniq_init()
{
	return true;
}

//#ifdef ANDROID
//extern "C" JNIEXPORT void JNICALL
//Java_uniq_launchpad_test_Test_juceInit(JNIEnv* env, jclass clazz, jobject context) {
//	Thread::initialiseJUCE(env, context);
//}
//#endif