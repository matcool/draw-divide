#include <matdash.hpp>
#include <matdash/minhook.hpp>
#include <matdash/boilerplate.hpp>
#include <matdash/console.hpp>
#include <cocos2d.h>

using namespace cocos2d;

template <class T, class B>
inline __forceinline T& from(B* base, size_t off) {
    return *reinterpret_cast<T*>((uintptr_t)base + off);
}

class CCDirectorVisible : public cocos2d::CCDirector {
public:
	void calculateDeltaTime() {
		CCDirector::calculateDeltaTime();
	};

	void setNextScene() {
		CCDirector::setNextScene();
	}
};

double frame_remainder = 0;
int target_fps = 60;
int frame_counter = 0;
bool enabled = true;

void CCDirector_drawScene(cocos2d::CCDirector* self) {
	// disable for first 300 frames of game being open
	if (!enabled || self->getTotalFrames() < 300) {
		return matdash::orig<&CCDirector_drawScene>(self);
	}

    const double fps_limit = 1.0 / self->getAnimationInterval();
    // std::cout << "fps_limit " << fps_limit << std::endl;

	// scary floats
	// getAnimationInterval is 1/fps bypass
	// 1/((1/fps bypass) * target) = fps bypass/target
	const double thing = fps_limit / static_cast<double>(target_fps);

	frame_counter++;

	// run full scene draw (glClear, visit) each time the counter is full
	if (static_cast<double>(frame_counter) + frame_remainder >= thing) {
		frame_remainder += static_cast<double>(frame_counter) - thing;
		frame_counter = 0;
		return matdash::orig<&CCDirector_drawScene>(self);
	}

	// otherwise, we only run updates

	// upcast to remove protection
	auto visible_director = static_cast<CCDirectorVisible*>(self);

	// this line seems to create a speedhack
	// visible_director->calculateDeltaTime();

	if (!self->isPaused()) {
		(self->CCDirector::getScheduler())->CCScheduler::update(self->CCDirector::getDeltaTime());
	}

	if (self->getNextScene()) {
		visible_director->setNextScene();
	}
}

void mod_main(HMODULE) {
    // matdash::create_console();
    auto cocos_handle = LoadLibraryA("libcocos2d.dll");

	matdash::add_hook<&CCDirector_drawScene>(GetProcAddress(cocos_handle, "?drawScene@CCDirector@cocos2d@@QAEXXZ"));
}