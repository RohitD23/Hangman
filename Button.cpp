#include "Button.h"

Button::Button() {
	
	BUTTON_HEIGHT = 0;
	BUTTON_WIDTH = 0;

	mouse_position.x = 0;
	mouse_position.y = 0;

	set_color(0, 0, 170, 255);

	outer_box = { 0,0,0,0 };
	inner_box = { 0,0,0,0 };

	isButtonPressed = false;
}

Button::~Button() { free(); }

void Button::free() {

	mouse_position.x = 0;
	mouse_position.y = 0;

	outer_box = { 0,0,0,0 };
	inner_box = { 0,0,0,0 };

	set_color(0, 0, 170, 255);

	ButtonText.free();

	isButtonPressed = false;
}

bool Button::load_button_media(std::string text, int x, int y) {

	bool success = true;

	if (!ButtonText.loadFromRenderedText(text, { 0xFF,0xFF,0xFF }, 500)){
		std::cout << "Failed to laod Button text" << std::endl;
		success = false;
	}

	mouse_position.x = x;
	mouse_position.y = y;

	outer_box.x = x;
	outer_box.y = y;
	outer_box.w = BUTTON_WIDTH;
	outer_box.h = BUTTON_HEIGHT;

	inner_box.x = x + 5;
	inner_box.y = y + 5;
	inner_box.w = BUTTON_WIDTH - 10;
	inner_box.h = BUTTON_HEIGHT - 10;

	return success;
}

bool Button::load_lock_image() {
	
	bool success = true;

	if (!ButtonText.load_media_from_file("src/Lock.png")) {
		std::cout << "Failed to load Lock.png" << std::endl;
		success = false;
	}

	return success;
}

void Button::render(int x, int y) {

	SDL_SetRenderDrawColor(GameWindow.Get_Renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect(GameWindow.Get_Renderer(), &outer_box);

	SDL_SetRenderDrawColor(GameWindow.Get_Renderer(), red, green, blue, alpha);
	SDL_RenderFillRect(GameWindow.Get_Renderer(), &inner_box);

	ButtonText.renderTexture(nullptr, x, y);
}

void Button::handleEvent(SDL_Event* e) {

	//If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP) {

		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if (x < mouse_position.x)
		{
			inside = false;
		}
		//Mouse is right of the button
		else if (x > mouse_position.x + BUTTON_WIDTH)
		{
			inside = false;
		}
		//Mouse above the button
		else if (y < mouse_position.y)
		{
			inside = false;
		}
		//Mouse below the button
		else if (y > mouse_position.y + BUTTON_HEIGHT)
		{
			inside = false;
		}

		//If mouse is inside button
		if (inside) {

			//Set mouse over sprite
			switch (e->type)
			{
			case SDL_MOUSEMOTION:
				//SDL_SetRenderDrawBlendMode(GameWindow.Get_Renderer(), SDL_BLENDMODE_BLEND);
				ButtonText.setColor(0, 0, 0xAA);

				set_color(255, 255, 255, 255);
				break;

			case SDL_MOUSEBUTTONDOWN:
				ButtonText.setColor(255, 255, 255);

				set_color(0, 0, 100, 255);
				break;

			case SDL_MOUSEBUTTONUP:
				ButtonText.setColor(0, 0, 0xAA);

				isButtonPressed = true;

				set_color(255, 255, 255, 255);
				break;
			}
		}
		else {
			set_color(0, 0, 170, 255);

			ButtonText.setColor(0xFF, 0xFF, 0xFF);
		}
	}

}

void Button::set_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	red = r;
	green = g;
	blue = b;
	alpha = a;
}