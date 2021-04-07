#include "Window.h"
#include "Texture.h"
#include "MainMenu.h"
#include "Game.h"
#include "Data.h"
#include "GameOver.h"
#include "Rules.h"

Window GameWindow;

void close();
void Handle_Menu_Selection(SDL_Event& e, Screen_Options& current_option, const MainMenu& GameMenu);
void LoadMainMenu(MainMenu& GameMenu, int& frame, bool& frame_loop);
void LoadGame(Game& MainGame);
void LoadGameOver(GameOver& gameOver);
void LoadRules(Rules& gameRules);

Screen_Options current_option = MAIN_MENU_SCREEN;

void PlayGame() {

	MainMenu GameMenu;
	Game MainGame;
	GameOver gameOver;
	Rules gameRules;

	if (!GameWindow.init()) {
		std::cout << "Failed to initialize" << std::endl;
	}
	else {

		if (!GameMenu.load_Menu_Menu_Media()) {
			std::cout << "Failed to load Main Menu Media" << std::endl;
		}
		else {
			bool quit = false;
			bool frame_loop = false;

			int frame = 0;
			SDL_Event e;

			while (!quit) {

				while (SDL_PollEvent(&e) != 0) {

					if (e.type == SDL_QUIT || current_option == EXIT)
						quit = true;

					GameWindow.handle_event(e);

					if (current_option == MAIN_MENU_SCREEN) {
						GameMenu.menu_selection(e);
						Handle_Menu_Selection(e, current_option, GameMenu);
					}

					else if (current_option == GAME_SCREEN) {
						MainGame.handle_event(e);

						if (MainGame.Get_no_of_passes() < 3)
							MainGame.Get_passButton().handleEvent(&e);
					}

					else if (current_option == RULES_SCREEN)
						gameRules.handle_event(&e);

					else if (current_option == GAME_LOST_SCREEN || current_option == GAME_WON_SCREEN)
						gameOver.handle_event(e);
				}

				if (!GameWindow.Get_Minimized()) {

					if (current_option == MAIN_MENU_SCREEN) {

						if (MainGame.CheckTextureLoaded())
							MainGame.free();

						if (gameOver.CheckTextureLoaded())
							gameOver.free();

						LoadMainMenu(GameMenu, frame, frame_loop);
					}

					else if (current_option == GAME_SCREEN) {
						if (GameMenu.CheckTextureLoaded())
							GameMenu.free();

						if (gameOver.CheckTextureLoaded())
							gameOver.free();

						if (gameRules.CheckTextureLoaded())
							gameRules.free();

						LoadGame(MainGame);
					}

					else if (current_option == GAME_LOST_SCREEN || current_option == GAME_WON_SCREEN) {
						if (MainGame.CheckTextureLoaded())
							MainGame.free();

						LoadGameOver(gameOver);
					}

					else if (current_option == RULES_SCREEN)
						LoadRules(gameRules);
				}
			}
		}
	}

	close();
}

void Handle_Menu_Selection(SDL_Event& e, Screen_Options& current_option, const MainMenu& GameMenu) {

	if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {

		if (std::get<1>(GameMenu.Get_Menu_Texture()[0]) == true)
			current_option = GAME_SCREEN;

		else if (std::get<1>(GameMenu.Get_Menu_Texture()[1]) == true)
			current_option = RULES_SCREEN;

		else if (std::get<1>(GameMenu.Get_Menu_Texture()[2]) == true)
			current_option = EXIT;

	}
}

void LoadMainMenu(MainMenu& GameMenu, int& frame, bool& frame_loop) {

	if (!GameMenu.CheckTextureLoaded()) {

		if (!GameMenu.load_Menu_Menu_Media()) {
			std::cout << "Failed to load Main Menu Media" << std::endl;
		}
	}

	GameMenu.render(frame_loop, frame);
}

void LoadGame(Game& MainGame) {

#if 0
	if (!GameWindow.Get_FullScreened()) {
		SDL_SetWindowFullscreen(GameWindow.Get_Window(), SDL_TRUE);
		GameWindow.Set_FullScreened(true);
	}
#endif

	if (!MainGame.CheckTextureLoaded()) {

		if (!MainGame.load_game_media()) {
			std::cout << "Failed to load Main Menu Media" << std::endl;
		}
	}

	MainGame.render();
}

void LoadRules(Rules& gameRules) {
	if (!gameRules.CheckTextureLoaded()) {

		if (!gameRules.load_rules()) {
			std::cout << "Failed to load Game Rules" << std::endl;
		}
	}

	gameRules.render();
}

void LoadGameOver(GameOver& gameOver) {

	if (!gameOver.CheckTextureLoaded()) {

		if (!gameOver.load_game_over_media()) {
			std::cout << "Failed to load Main Menu Media" << std::endl;
		}
	}

	gameOver.render();
}

void close() {

	GameWindow.free();

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

