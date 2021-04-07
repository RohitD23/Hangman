#pragma once

enum Screen_Options {
	MAIN_MENU_SCREEN = 0,
	GAME_SCREEN,
	RULES_SCREEN,
	GAME_LOST_SCREEN,
	GAME_WON_SCREEN,
	EXIT
};

struct Riddle {
	std::string question;
	std::string answer;
};

enum which_screen_to_render {
	QUESTION_SCREEN = 0,
	RESULT_SCREEN,
	PAUSE_SCREEN
};
