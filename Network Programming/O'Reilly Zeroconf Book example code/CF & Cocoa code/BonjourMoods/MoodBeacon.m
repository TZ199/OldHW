#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#import "MoodBeacon.h"

@implementation MoodBeacon
NSNetService * service;

- (void)netServiceWillPublish:(NSNetService *)sender
{
	[progressIndicator startAnimation:self];
}
- (void)netServiceDidPublish:(NSNetService *)sender
{
	[progressIndicator stopAnimation:self];
	[userName setEnabled:YES];
	[serviceStarter setHidden:YES];
	[userName setHidden:NO];
	[happinessSlider setHidden:NO];
	[userName setStringValue:[sender name]];
}

- (void)netService:(NSNetService *)sender didNotPublish:(NSDictionary *)errorDict
{
	[userName setStringValue:@"Error: did not publish"];
}

- (IBAction)publishService:(id)sender
{
	// Get us a unique listening socket number to advertise
	int s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in sa = { sizeof(sa), AF_INET };
	int size = sizeof(sa);
	bind(s, (struct sockaddr *)&sa, sizeof(sa));
	getsockname(s, (struct sockaddr *)&sa, &size);

	service = [[NSNetService alloc] initWithDomain:@""
											  type:@"_moodring._tcp."
											  name:NSFullUserName()
											  port:ntohs(sa.sin_port)];
	if ( [[serviceStarter title] isEqualToString:@"Start Service"])
	{
		[service setDelegate:self];
		[service publish];
		[self updateMood:happinessSlider];
	}
}
- (IBAction)updateMood:(NSSlider *)sender
{
	NSMutableDictionary * moodDictionary = [NSMutableDictionary dictionaryWithCapacity:2];
	NSString * txtversKey = @"txtvers";
	NSString * moodKey = @"mood";

	NSString * txtversValue = @"1";
	NSString * moodValue = [sender stringValue];
	[moodDictionary setObject:txtversValue forKey:txtversKey];
	[moodDictionary setObject:moodValue forKey:moodKey];
	[service setTXTRecordData:[NSNetService dataFromTXTRecordDictionary:moodDictionary]];
	float currentValue = [sender floatValue]/4;
	[userName setBackgroundColor:[NSColor colorWithCalibratedRed:3.3 * (1.0 - currentValue)
														   green:2.0 * (currentValue)
															blue:0.0 alpha:1.0]];
}

@end
