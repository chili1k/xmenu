#import <stdbool.h>
#import <stdio.h>
#import <Cocoa/Cocoa.h>
#import "view.h"
#import "items.h"

// TODO: Parse these from command-line opts.
#define window_height 50
#define display_bottom true

int main(int argc, const char * argv[])
{
	ItemList itemList = ReadStdin();
	if (itemList.len)
		itemList.item[0].sel = true;


	// Autorelease Pool:
	// Objects declared in this scope will be automatically
	// released at the end of it, when the pool is "drained".
	NSAutoreleasePool *pool = [NSAutoreleasePool new];

	// Create a shared app instance.
	// This will initialize the global variable
	// 'NSApp' with the application instance.
	[NSApplication sharedApplication];

	// Create a window:

	NSRect screenFrame = [[NSScreen mainScreen] visibleFrame];
	CGFloat y = screenFrame.origin.y;
	if (!display_bottom) {
		y += screenFrame.size.height - window_height;
	}
	// Window bounds (x, y, width, height)
	NSRect windowRect = NSMakeRect(
		screenFrame.origin.x,
		y,
		screenFrame.size.width,
		window_height);

	NSWindow *window = [[NSWindow alloc]
		initWithContentRect:windowRect
		styleMask:NSBorderlessWindowMask
		backing:NSBackingStoreBuffered
		defer:NO];
	[window autorelease];
	[window makeKeyAndOrderFront:nil];
	// Window controller
//	NSWindowController * windowController =
//		[[NSWindowController alloc] initWithWindow:window];
//	[windowController autorelease];

	// This will add a simple text view to the window,
	// so we can write a test string on it.
//	NSTextView * textView = [[NSTextView alloc] initWithFrame:windowRect];
//	[textView autorelease];
	XmenuMainView *mainView = [[XmenuMainView alloc] initWithFrame:windowRect
							 items:itemList];
	[mainView autorelease];
	window.contentView = mainView;
//	for (int i = 0; i < itemList->len; i++) {
//		[textView insertText: [NSString stringWithUTF8String: itemList->items[i].text]];
//	}
//	[textView insertText:@"Hello OSX/Cocoa world!"];

	// TODO: Create app delegate to handle system events.
	// TODO: Create menus (especially Quit!)

	// Show window and run event loop
	[window orderFrontRegardless];
	[NSApp activateIgnoringOtherApps:YES];
	[NSApp run];

	[pool drain];

	return (0);
}
