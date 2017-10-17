#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <CoreServices/CoreServices.h>

static void MyResolveCallBack(CFNetServiceRef service, CFStreamError* error, void* info)
	{
	int count;
	CFArrayRef addresses = CFNetServiceGetAddressing(service);

	assert(addresses != NULL);
	assert(CFArrayGetCount(addresses) > 0);

	// May get more than one reply
	for (count = 0; count < CFArrayGetCount(addresses); count++)
		{
		char addr[256];
		struct sockaddr_in *sa = (struct sockaddr_in *)
			CFDataGetBytePtr(CFArrayGetValueAtIndex(addresses, count));
		// inet_ntop will correctly display both IPv4 and IPv6 addresses
		if (inet_ntop(sa->sin_family, &sa->sin_addr, addr, sizeof(addr)))
			printf("%s:%d \n", addr, ntohs(sa->sin_port));
		}
	}

static void MyResolveService()
	{
	CFNetServiceClientContext context = { 0, NULL, NULL, NULL, NULL };
	CFStreamError error;

	CFNetServiceRef serviceBeingResolved = CFNetServiceCreate(kCFAllocatorDefault,
		CFSTR("local."), CFSTR("_example._tcp"), CFSTR("CF Example"), 0);
	assert(serviceBeingResolved != NULL);

	CFNetServiceSetClient(serviceBeingResolved, MyResolveCallBack, &context);
	CFNetServiceScheduleWithRunLoop(serviceBeingResolved,
		CFRunLoopGetCurrent(), kCFRunLoopCommonModes);

	if (CFNetServiceResolve(serviceBeingResolved, &error) == false)
		{ // Something went wrong so lets clean up.
		CFNetServiceUnscheduleFromRunLoop(serviceBeingResolved,
			CFRunLoopGetCurrent(), kCFRunLoopCommonModes);
		CFNetServiceSetClient(serviceBeingResolved, NULL, NULL);
		CFRelease(serviceBeingResolved);
		serviceBeingResolved = NULL;
		fprintf(stderr, "CFNetServiceResolve returned %d, %ld\n",
			error.domain, error.error);
		}
	}

int main(int argc, char* argv[])
	{
	MyResolveService();
	CFRunLoopRun();
	return 0;
	}
