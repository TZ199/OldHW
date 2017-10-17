#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#import <Foundation/Foundation.h>

@interface MyResolver : NSObject
	{
	NSNetService *service;
	}
@end

@implementation MyResolver

- (void)resolveService
	{
	service = [[NSNetService alloc]
		initWithDomain:@"local." type:@"_example._tcp." name:@"Cocoa Example"];
	[service setDelegate:self];
	[service resolve];
	}

- (void)netServiceWillResolve:(NSNetService *)sender
	{
	NSLog(@"netServiceWillResolve: %@.%@%@\n",
		[sender name], [sender type], [sender domain]);
	}

- (void)netServiceDidResolveAddress:(NSNetService *)s
	{
	NSLog(@"DidResolve: %@.%@%@\n", [s name], [s type], [s domain]);

	// May get more than one reply
	NSArray *addresses = [s addresses];
	int count;
	for (count = 0; count < [addresses count]; count++)
		{
		char addr[256];
		struct sockaddr_in *sa = (struct sockaddr_in *)
			[[addresses objectAtIndex:count] bytes];
		if (inet_ntop(sa->sin_family, &sa->sin_addr, addr, sizeof(addr)))
			NSLog(@"DidResolve: %s:%d \n", addr, ntohs(sa->sin_port));
		}
	}

- (void)netService:(NSNetService *)sender didNotResolve:(NSDictionary *)errorDict
	{
	NSLog(@"didNotResolve: %@.%@%@\n",
	  [sender name], [sender type], [sender domain]);
	}

@end

int main(int argc, char *argv[])
	{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	[[[MyResolver alloc]init] resolveService];
	[[NSRunLoop currentRunLoop] run];
	[pool release];
	return 0;
	}
