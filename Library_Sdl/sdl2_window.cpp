#include <iostream>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>


const int WINDOW_WIDTH = 600;
const int Window_HEIGHT = 800;





class Window
{
public:
	std::shared_ptr<SDL_Window> _window;
	std::shared_ptr<SDL_Renderer> _renderer;

	Window();
	virtual ~Window(){}
	virtual void handle_event(const  SDL_Event &event){}
	virtual void handle_keys(const Uint8 *keys){}
	virtual void update(){}
	virtual void render_rectangle(){}
	void main_loop();
};

class WindowWithRectangle : public Window
{
public:
	SDL_Rect rect1{300,200,15,15};
	int rect1_dx=1,rect1_dy=1;
	SDL_Rect rect2{600,400,15,15};
	WindowWithRectangle();
	virtual ~WindowWithRectangle(){}
	virtual void handle_keys(const Uint8 *keys) override;
	virtual void update() override;
	virtual void render_rectangle() override;
};

WindowWithRectangle::WindowWithRectangle()
:Window()
 {

 }





Window::Window()
{
	_window = std::shared_ptr<SDL_Window>(
	SDL_CreateWindow("SDL Windows", SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED, Window_HEIGHT, WINDOW_WIDTH,
					SDL_WINDOW_SHOWN), SDL_DestroyWindow);
	if (_window == nullptr) {
		std::cerr << "error" << SDL_GetError() << std::endl;
		exit(1);
		}
	_renderer = std::shared_ptr<SDL_Renderer>(
	SDL_CreateRenderer(_window.get(), -1, SDL_RENDERER_ACCELERATED),
				SDL_DestroyRenderer);
		if (_renderer == nullptr) {
			std::cerr << "error" << SDL_GetError() << std::endl;
			exit(1);
		}



}
void WindowWithRectangle::handle_keys(const Uint8 *keys)
{
	if(keys[SDL_SCANCODE_RIGHT])rect2.x+=2;
	if(keys[SDL_SCANCODE_LEFT])rect2.x-=2;
	if(keys[SDL_SCANCODE_DOWN])rect2.y+=2;
	if(keys[SDL_SCANCODE_UP])rect2.y-=2;
}




void WindowWithRectangle::render_rectangle(){
	SDL_SetRenderDrawColor(_renderer.get(), 31, 63, 95, 255);
	SDL_RenderClear(_renderer.get());
	SDL_SetRenderDrawColor(_renderer.get(), 255, 255, 255, 255);
	for (int i = 0; i < 50; ++i) {
		SDL_RenderDrawLine(_renderer.get(), 100 + i * 8, 100, 100, 500 - i * 8);
		SDL_RenderDrawLine(_renderer.get(), 500 - i * 8, 500, 500, 100 + i * 8);
	}
	//рисуем область для передвижения квадрата
	SDL_SetRenderDrawColor(_renderer.get(), 0, 0, 0, 255);
	SDL_RenderDrawLine(_renderer.get(), 200, 200, 400, 200 );
	SDL_RenderDrawLine(_renderer.get(), 200, 400, 400, 400 );
	SDL_RenderDrawLine(_renderer.get(), 400, 200, 400, 400 );
	SDL_RenderDrawLine(_renderer.get(), 200, 200, 200, 400 );

	SDL_SetRenderDrawColor(_renderer.get(), 255, 63, 63, 255);
	SDL_RenderFillRect(_renderer.get(),&rect1);

	SDL_SetRenderDrawColor(_renderer.get(), 255, 255, 255, 255);
	SDL_Rect r;
	if(SDL_IntersectRect(&rect1,&rect2,&r))
		SDL_SetRenderDrawColor(_renderer.get(), 255, 255, 63, 255);
	SDL_RenderFillRect(_renderer.get(),&rect2);
}


void Window::main_loop() {

	auto keys = SDL_GetKeyboardState(nullptr);
	SDL_Event event;
	for (;;) {
		//обработка событий
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				exit(0);
			if (event.type==SDL_KEYDOWN){
				switch(event.key.keysym.scancode){
				case SDL_SCANCODE_ESCAPE:return;
				default:
					;
				}
			}
			handle_event(event);
		}
		handle_keys(keys);
		//изменения состояния событий
		update();



		//отображение текущего состояния
		render_rectangle();
		SDL_RenderPresent(_renderer.get());

	}
}



void WindowWithRectangle::update()
{
	rect1.x+=rect1_dx;
	rect1.y+=rect1_dy;
	if (rect1.y<=200) rect1_dy=1;
	if (rect1.y+rect1.h>=400)rect1_dy=-1;
	if (rect1.x<=200) rect1_dx=1;
	if (rect1.x+rect1.w>=400)rect1_dx=-1;
}



int main(int, char**)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	WindowWithRectangle window1;
	window1.main_loop();






	return 0;
}
