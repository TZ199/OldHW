#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <CoreServices/CoreServices.h>

CFNetServiceRef gRegisteredService;

static void MyCancelRegistration(void)
	{
	CFNetServiceUnscheduleFromRunLoop(gRegisteredService,
		CFRunLoopGetCurrent(), kCFRunLoopCommonModes);
	CFNetServiceSetClient(gRegisteredService, NULL, NULL);
	CFRelease(gRegisteredService);
	gRegisteredService = NULL;
	}

static void MyRegisterCallBack(CFNetServiceRef theService, CFStreamError* error, void* info)
	{
	printf("MyRegisterCallBack\n");
	if (error->error == 0)
		printf("MyRegisterService advertising service\n");

	if (error->domain == kCFStreamErrorDomainNetServices)
		{
		switch(error->error)
			{
			case kCFNetServicesErrorCollision:
				MyCancelRegistration();
				fprintf(stderr, "kCFNetServicesErrorCollision occured\n");
				break;
			default:
				MyCancelRegistration();
				fprintf(stderr, "MyRegisterCallBack (domain = %d, error = %ld)\n",
					error->domain, error->error);
				break;
			}
		}
	}

static Boolean MyRegisterService(u_short thePort)
	{
	CFNetServiceClientContext context = { 0, NULL, NULL, NULL, NULL };
	CFStreamError error;
	Boolean result;

	printf("MyRegisterService advertising service on port %d\n", thePort);

	gRegisteredService = CFNetServiceCreate(kCFAllocatorDefault,
		CFSTR(""), CFSTR("_example._tcp"), CFSTR("CF Example"), // Domain, type, name
		thePort);
	assert(gRegisteredService != NULL);

	CFNetServiceSetClient(gRegisteredService, MyRegisterCallBack, &context);

	CFNetServiceScheduleWithRunLoop(gRegisteredService,
		CFRunLoopGetCurrent(), kCFRunLoopCommonModes);

	result = CFNetServiceRegister(gRegisteredService, &error);
	if (result == false) //clean up
		{
		MyCancelRegistration();
		fprintf(stderr, "CFNetServiceRegister returned (domain = %d, error = %ld)\n",
			error.domain, error.error);
		}
	return result;
	}

int main(int argc, char* argv[])
	{
	/*
	int s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in sa = { sizeof(sa), AF_INET };
	int size = sizeof(sa);
	bind(s, (struct sockaddr *)&sa, sizeof(sa));
	getsockname(s, (struct sockaddr *)&sa, &size);
	MyRegisterService(ntohs(sa.sin_port));
	*/

	CFSocketRef s = CFSocketCreate(kCFAllocatorDefault,
		PF_INET, SOCK_STREAM, IPPROTO_TCP,
		kCFSocketNoCallBack, NULL, NULL);
	struct sockaddr_in sa = { sizeof(sa), AF_INET };
	CFDataRef addr = CFDataCreateWithBytesNoCopy(kCFAllocatorDefault,
		(const UInt8*)&sa, sizeof(sa), kCFAllocatorNull);
	CFSocketSetAddress(s, addr);
	CFRelease(addr);
	addr = CFSocketCopyAddress(s);
	memmove(&sa, CFDataGetBytePtr(addr), sizeof(sa));
	CFRelease(addr);
	MyRegisterService(ntohs(sa.sin_port));

	CFRunLoopRun();
	return 0;
	}
