#import "MoodBrowser.h"

@implementation MoodBrowser


- (void) setUpServiceBrowser
{
	serviceBrowser = [[NSNetServiceBrowser alloc] init];
	[serviceBrowser setDelegate:self];
	[serviceBrowser searchForServicesOfType:@"_moodring._tcp." inDomain:@""];
}

- (id) init
{
	self = [super init];
	friendsList = [[NSMutableArray alloc] init];
	moodsList = [[NSMutableArray alloc] init];
	[self setUpServiceBrowser];
	return self;
}

- (void) dealloc
{
	[friendsList release];
	[super dealloc];
}

- (int) numberOfRowsInTableView:(NSTableView *)tableView
{
	return [friendsList count];
}

- (id)tableView:(NSTableView *)aTableView
	    objectValueForTableColumn:(NSTableColumn *)aTableColumn
			row:(int)rowIndex
{
	return [[friendsList objectAtIndex:rowIndex] name];
}

- (void)netServiceBrowser:(NSNetServiceBrowser *)aNetServiceBrowser didFindService:(NSNetService *)aNetService moreComing:(BOOL)moreComing
{
    [friendsList addObject:aNetService];
	[moodsList addObject:[NSColor blackColor]];
	[aNetService setDelegate:self];
	[aNetService startMonitoring];
    if(!moreComing)
        [friendView reloadData];
}

- (void)netServiceBrowser:(NSNetServiceBrowser *)aNetServiceBrowser didRemoveService:(NSNetService *)aNetService moreComing:(BOOL)moreComing
{
	[moodsList removeObjectAtIndex:[friendsList indexOfObject:aNetService]];
	[friendsList removeObject:aNetService];
    [aNetService stopMonitoring];
    if(!moreComing)
        [friendView reloadData];
}

- (void)tableView:(NSTableView *)inTableView
	 willDisplayCell:(id)inCell
	  forTableColumn:(NSTableColumn *)inTableColumn
			  row:(int)inRow
{
	[inCell setTextColor:[moodsList objectAtIndex:inRow]];
	}

- (void)netService:(NSNetService *)sender didUpdateTXTRecordData:(NSData *)data
{
  NSString * temp = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
	float currentValue = [[temp substringFromIndex:[temp length]-1] floatValue]/4;
	[moodsList replaceObjectAtIndex:[friendsList indexOfObject:sender] withObject:[NSColor colorWithCalibratedRed:3.3 *(1- currentValue)
																											green:2.0 * (currentValue)
																											 blue:0.0 alpha:1.0]];

	[friendView reloadData];
	//}
}


@end
