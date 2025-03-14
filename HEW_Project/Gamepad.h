#pragma once
#include<Windows.h>

#include <xinput.h>
#pragma comment (lib, "xinput.lib")

enum struct GAMEPAD_KEYTYPE : char {
	Buttons, LTrigger, RTrigger,
	ThumbLL, ThumbLR, ThumbLU, ThumbLD,
	ThumbRL, ThumbRR, ThumbRU, ThumbRD,
};

struct KeyInf {
	char state;
	char codeKey;
	char codeKeyboard;
	GAMEPAD_KEYTYPE typeGamepadKey;
	short codeGamepad;
};

class InputManager {
private:
	int numRegistered;
	char statePrev[64];
	KeyInf inf[64];
public:
	InputManager();
	bool addKeycode(char codeKey, char codeKeyboard, GAMEPAD_KEYTYPE typeGamepadKey, short codeGamepad);
	void inspect();
	char getKey(char codeKey);
};
// __INPUT_H
