#import <Foundation/Foundation.h>

@interface MyBrowser : NSObject
	{
	NSNetServiceBrowser *serviceBrowser;
	}
- (void)browseForServices;

@end

@implementation MyBrowser

- (void)browseForServices
	{
	serviceBrowser = [[NSNetServiceBrowser alloc] init];
	[serviceBrowser setDelegate:self];
	[serviceBrowser searchForServicesOfType:@"_example._tcp." inDomain:@""];
	}

- (void)netServiceBrowserWillSearch:(NSNetServiceBrowser *)aNetServiceBrowser
	{
	NSLog(@"Starting to search . . .\n");
	}


- (void)netServiceBrowser:(NSNetServiceBrowser *)aNetServiceBrowser
		didFindService:(NSNetService *)s moreComing:(BOOL)moreComing
	{
	NSLog(@"Add %@.%@%@\n", [s name], [s type], [s domain]);
	}


- (void)netServiceBrowser:(NSNetServiceBrowser *)aNetServiceBrowser
		didRemoveService:(NSNetService *)s moreComing:(BOOL)moreComing
	{
	NSLog(@"Rmv %@.%@%@\n", [s name], [s type], [s domain]);
	}

@end

int main(int argc, char *argv[])
	{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	[[[MyBrowser alloc]init] browseForServices];
	[[NSRunLoop currentRunLoop] run];
	[pool release];
	return 0;
	}
