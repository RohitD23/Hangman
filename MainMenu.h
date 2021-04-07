#pragma once
#include "Texture.h"
#include <tuple>

class MainMenu {
public:
	~MainMenu();

	void free();

	void menu_selection(SDL_Event& e);

	void load_menu_texture(const SDL_Color& textColor);

	bool load_Menu_Menu_Media();

	const std::tuple<Texture, bool>* Get_Menu_Texture() const { return menu_text_texture; }

	bool CheckTextureLoaded();

	void render(bool& frame_loop, int& frame);

private:
	Texture introTexture;
	Texture intro_text_texture;

	static const int TOTAL_MENU_TEXT_TEXTURE = 3;
	std::tuple<Texture, bool> menu_text_texture[TOTAL_MENU_TEXT_TEXTURE];

	static const int INTRO_ANIMATION_FRAMES = 5;
	SDL_Rect intro_animation[INTRO_ANIMATION_FRAMES];

	void down_key_menu_selection(const SDL_Color& textColor, const std::string each_option_text[][2]);
	void up_key_menu_selection(const SDL_Color& textColor, const std::string each_option_text[][2]);
};
