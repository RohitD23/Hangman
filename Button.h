#pragma once
#include "Texture.h"

class Button {

public:
	Button();

	~Button();

	void free();

	void render(int x, int y);

	bool load_button_media(std::string text, int x, int y);

	//Handles mouse event
	void handleEvent(SDL_Event* e);

	bool Get_isButtonPressed() { return isButtonPressed; }

	void Set_isButtonPressed(bool isButtonPressed) { this->isButtonPressed = isButtonPressed; }

	bool load_lock_image();

	void set_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	void Set_BUTTON_WIDTH_AND_HEIGHT(int width, int height) { 
		BUTTON_WIDTH = width;
		BUTTON_HEIGHT = height;
	}

	int Get_Width() { return ButtonText.Get_Width(); }

private:
	int BUTTON_WIDTH;
	int BUTTON_HEIGHT;

	Uint8 red;
	Uint8 blue;
	Uint8 green;
	Uint8 alpha;

	Texture ButtonText;

	SDL_Rect outer_box;
	SDL_Rect inner_box;

	SDL_Point mouse_position;

	bool isButtonPressed;
};