#pragma once

#include <functional>

// https://stackoverflow.com/questions/7676971/pointing-to-a-function-that-is-a-class-member-glfw-setkeycallback

class MyGlWindow {
public:
	std::function<void(MyGlWindow*)> onClose;
	std::function<void(MyGlWindow*, int, int, int)> onMouseClick = [](auto self, int, int, int) { /*some default behavior*/ };
};
