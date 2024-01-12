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

double delta_count = 0.0;
bool enabled = true;

void CCDirector_drawScene(cocos2d::CCDirector* self) {
	// disable for first 300 frames of game being open
	if (!enabled || self->getTotalFrames() < 300) {
		return matdash::orig<&CCDirector_drawScene>(self);
	}

	// always target the refresh rate of the monitor
	const double target_fps = get_refresh_rate();

	const double target_delta = 1.0 / target_fps;

	delta_count += self->getActualDeltaTime();

	// run full scene draw (glClear, visit) each time the counter is full
	if (delta_count >= target_delta) {
		// keep left over
		delta_count -= target_delta;
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