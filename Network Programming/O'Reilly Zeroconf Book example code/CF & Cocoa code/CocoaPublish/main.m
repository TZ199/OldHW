#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#import <Foundation/Foundation.h>

@interface MyPublisher : NSObject
	{
	NSNetService *service;
	}
@end

@implementation MyPublisher

- (void)publishService:(UInt16)thePort
	{
	service = [[NSNetService alloc]
		initWithDomain:@"" type:@"_example._tcp." name:@"Cocoa Example" port:thePort];
	[service setDelegate:self];
	[service publish];
	}

- (void)netServiceWillPublish:(NSNetService *)s
	{
	NSLog(@"WillPublish: %@.%@%@\n", [s name], [s type], [s domain]);
	}

- (void)netServiceDidPublish:(NSNetService *)s
	{
	NSLog(@"DidPublish: %@.%@%@\n", [s name], [s type], [s domain]);
	}

- (void)netService:(NSNetService *)s didNotPublish:(NSDictionary *)errorDict
	{
	NSLog(@"didNotPublish: %@.%@%@\n", [s name], [s type], [s domain]);
	}

@end

int main(int argc, char *argv[])
	{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

	// Get a new listening socket...
	int s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in sa = { sizeof(sa), AF_INET };
	int size = sizeof(sa);
	bind(s, (struct sockaddr *)&sa, sizeof(sa));
	getsockname(s, (struct sockaddr *)&sa, &size);

	// ... and advertise it
	[[[MyPublisher alloc]init] publishService: ntohs(sa.sin_port)];

	[[NSRunLoop currentRunLoop] run];
	[pool release];
	return 0;
	}
