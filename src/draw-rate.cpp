#include <matdash.hpp>
#include <matdash/minhook.hpp>
#include <matdash/boilerplate.hpp>

// #include <matdash/console.hpp>
// #include <format>

namespace cocos2d {

class CCScene {};

class CCScheduler {
public:
    virtual void update(float dt);
};

class CCDirector {
public:
	void calculateDeltaTime_() {
		CCDirector::calculateDeltaTime();
	};

	void setNextScene_() {
		CCDirector::setNextScene();
	}

    unsigned int getTotalFrames(void);
    bool isPaused(void);
    double getAnimationInterval();

	virtual CCScheduler* getScheduler();
	CCScene* getNextScene(void);

	virtual float getDeltaTime();
	float getActualDeltaTime() const;

protected:
    void calculateDeltaTime();
    void setNextScene(void);
};

}

using namespace cocos2d;

double get_refresh_rate() {
	static const double refresh_rate = [] {
		DEVMODEA device_mode;
		memset(&device_mode, 0, sizeof(device_mode));
		device_mode.dmSize = sizeof(device_mode);
		device_mode.dmDriverExtra = 0;

		if (EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &device_mode) == 0) {
			return 60.0;
		} else {
			// TODO: see if there is a way to get the exact frequency?
			// like 59.940hz
			auto freq = device_mode.dmDisplayFrequency;
			// interlaced screens actually display twice of the reported frequency
			if (device_mode.dmDisplayFlags & DM_INTERLACED) {
				freq *= 2;
			}
			return static_cast<double>(freq);
		}
	}();
	return refresh_rate;
}

double frame_remainder = 0;
int frame_counter = 0;
bool enabled = true;

void CCDirector_drawScene(cocos2d::CCDirector* self) {
	// disable for first 300 frames of game being open
	if (!enabled || self->getTotalFrames() < 300) {
		return matdash::orig<&CCDirector_drawScene>(self);
	}

	// always target the refresh rate of the monitor
	const double target_fps = get_refresh_rate();

	const double fps_limit = 1.0 / self->getAnimationInterval();

	// how many times this function is called until it actually renders
	// so for example, target_fps=60 and fps_limit=240, this variable would 4
	// since the mod would only render every 4th frame.
	const double frames_per_render = fps_limit / static_cast<double>(target_fps);

	frame_counter++;

	// run full scene draw (glClear, visit) each time the counter is full
	if (static_cast<double>(frame_counter) + frame_remainder >= frames_per_render) {
		// frames_per_render isnt always a whole number, so theres a bit of left over
		frame_remainder += static_cast<double>(frame_counter) - frames_per_render;
		frame_counter = 0;
		return matdash::orig<&CCDirector_drawScene>(self);
	}

	// otherwise, we only run updates

	// this line seems to create a speedhack
	// self->calculateDeltaTime_();

	if (!self->isPaused()) {
		self->CCDirector::getScheduler()->CCScheduler::update(self->CCDirector::getDeltaTime());
	}

	if (self->getNextScene()) {
		self->setNextScene_();
	}
}

void mod_main(HMODULE) {
	// matdash::create_console();
	auto cocos_handle = LoadLibraryA("libcocos2d.dll");

	matdash::add_hook<&CCDirector_drawScene>(GetProcAddress(cocos_handle, "?drawScene@CCDirector@cocos2d@@QAEXXZ"));
}