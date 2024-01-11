# Geometry Dash Frame Dividing

An inverse physics bypass. If rendering is a slowdown for GD, this may allow you the benefits of increased physics ticks with less lag. By being an inverse physics bypass, you should be able to click in between drawing frames, which would not be possible with traditional physics bypasses.

No, this mod **does not change the FPS physics run at**, it shouldn't affect physics at all (unless rob did some real nasty things in his code :P)

## Usage

The mod has no configuration, it will simply always be active whenever installed.
When installed, the mod will target your monitor's refresh rate, and make the game only render frames at that rate.

To unlock the fps, use the option ingame! (new in 2.2!). You can unlock your FPS to whatever value and this mod will still only render at your refresh rate. 

## How it really works

The mod hooks `CCDirector::drawScene`, which [originally looks something like this](https://github.com/cocos2d/cocos2d-x/blob/cocos2d-x-2.2.3/cocos2dx/CCDirector.cpp#L249):
```cpp
void CCDirector::drawScene() {
    // Runs the update methods on everything that wants it
    m_scheduler->update(m_deltaTime);

    glStuff();

    // Draw the scene
    m_currentScene->draw();
}
```

This mod makes it work somewhat like this:
```cpp
void CCDirector::drawScene() {
    // Runs the update methods on everything that wants it
    m_scheduler->update(m_deltaTime);

    // you get the idea
    auto now = time::now();
    
    if (now - lastRender < 1.0 / 60.0) {
        // dont draw the scene if its been less than
        // 1/60th of a second!
        return;
    }
    lastRender = now;

    glStuff();

    // Draw the scene
    m_currentScene->draw();
}
```

## Credits
* Chloe for the original implementation: [https://github.com/qimiko/draw-divide](https://github.com/qimiko/draw-divide)
* blanket addict