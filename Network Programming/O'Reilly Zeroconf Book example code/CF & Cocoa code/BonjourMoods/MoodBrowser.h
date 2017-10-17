/* MoodBrowser */

#import <Cocoa/Cocoa.h>

@interface MoodBrowser : NSObject
{
    IBOutlet NSTableView *friendView;
	NSMutableArray * friendsList;
	NSNetServiceBrowser * serviceBrowser;
	NSMutableArray * moodsList;
}
@end
