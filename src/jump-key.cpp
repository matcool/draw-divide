#include <matdash.hpp>
#include <matdash/minhook.hpp>
#include <matdash/boilerplate.hpp>
#include <cocos2d.h>
using namespace cocos2d;

void CCKeyboardDispatcher_dispatchKeyboardMSG(void* self, cocos2d::enumKeyCodes key, bool a, bool b) {
	switch (key) {
		// turn C into W
		case KEY_C: key = KEY_W; break;
		// turn JIL into <^>
		case KEY_J: key = KEY_Left; break;
		case KEY_I: key = KEY_Up; break;
		case KEY_L: key = KEY_Right; break;
		default: break;
	}
	matdash::orig<&CCKeyboardDispatcher_dispatchKeyboardMSG>(self, key, a, b);
}

void mod_main(HMODULE) {
	// matdash::create_console();
	auto cocos_handle = LoadLibraryA("libcocos2d.dll");

	matdash::add_hook<&CCKeyboardDispatcher_dispatchKeyboardMSG>(GetProcAddress(cocos_handle, "?dispatchKeyboardMSG@CCKeyboardDispatcher@cocos2d@@QAE_NW4enumKeyCodes@2@_N1@Z"));
}