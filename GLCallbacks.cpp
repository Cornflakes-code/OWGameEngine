
class MyGlWindow {
public:
	std::function<void(MyGlWindow*)> MyGlWindow::onClose;
	std::function<void(MyGlWindow*, int, int, int)> onMouseClick = [](auto self, int, int, int) { /*some default behavior*/ };
};
