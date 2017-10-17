/* MoodBeacon */

#import <Cocoa/Cocoa.h>

@interface MoodBeacon : NSObject
{
    IBOutlet NSSlider *happinessSlider;
    IBOutlet NSProgressIndicator *progressIndicator;
    IBOutlet NSButton *serviceStarter;
    IBOutlet NSTextField *userName;
}
- (IBAction)publishService:(id)sender;
- (IBAction)updateMood:(NSSlider *)sender;
@end
