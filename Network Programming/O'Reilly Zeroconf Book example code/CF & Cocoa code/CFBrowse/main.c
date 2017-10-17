#include <CoreServices/CoreServices.h>

CFNetServiceBrowserRef gBrowserService;
CFMutableDictionaryRef gServiceDictionary;

typedef struct
	{
	int refCount;
	char name[64];
	char type[1005];
	char domain[1005];
	} MyService;

CFStringRef MyCreateDictionaryKey(CFNetServiceRef service)
	{
	return CFStringCreateWithFormat(kCFAllocatorDefault, 0, CFSTR("%@.%@%@"),
		CFNetServiceGetName(service),
		CFNetServiceGetType(service),
		CFNetServiceGetDomain(service));
	}

static void MyAddService(CFNetServiceRef service, CFOptionFlags flags)
	{
	CFStringRef dictKey = MyCreateDictionaryKey(service);
	MyService *s;

	// We need to do reference counting of each service because if the computer
	// has two network interfaces set up, like Ethernet and AirPort, you may
	// get notified about the same service twice, once from each interface.
	// You probably don't want both items to be shown to the user.
	// On Mac OS X 10.4 and later, the CFNetServices code does this reference
	// counting for you, so you'll get at most one "add" event for a given
	// name/type/domain, but if your code is also going to run on Mac OS X
	// 10.3, you'll want to implement the reference counting as shown here.

	if (CFDictionaryGetValueIfPresent(gServiceDictionary, dictKey, (const void **)&s) == false)
		{
		s = malloc(sizeof(MyService));
		assert(s != NULL);
		s->refCount = 0;
		CFStringGetCString(CFNetServiceGetName  (service), s->name,   sizeof(s->name),
			kCFStringEncodingUTF8);
		CFStringGetCString(CFNetServiceGetType  (service), s->type,   sizeof(s->type),
			kCFStringEncodingUTF8);
		CFStringGetCString(CFNetServiceGetDomain(service), s->domain, sizeof(s->domain),
			kCFStringEncodingUTF8);
		CFDictionarySetValue(gServiceDictionary, dictKey, (const void **)s);
		printf("ADD %s.%s%s\n", s->name, s->type, s->domain);
		}

	s->refCount++;
	CFRelease(dictKey);
	}

static void MyRemoveService(CFNetServiceRef service, CFOptionFlags flags)
	{
	CFStringRef dictKey = MyCreateDictionaryKey(service);
	MyService *s;

	if (CFDictionaryGetValueIfPresent(gServiceDictionary, dictKey, (const void **)&s))
		{
		s->refCount--;
		if (s->refCount == 0)
			{
			CFDictionaryRemoveValue(gServiceDictionary, dictKey);
			printf("RMV %s.%s%s\n", s->name, s->type, s->domain);
			free(s);
			}
		}

	CFRelease(dictKey);
	}

static void MyBrowseCallBack(CFNetServiceBrowserRef theService,
	CFOptionFlags flags, CFTypeRef service, CFStreamError* err, void* info)
	{
	if (err->error)
		fprintf(stderr, "MyBrowseCallBack %d,%ld\n", err->domain, err->error);
	else if (flags & kCFNetServiceFlagRemove)
		MyRemoveService((CFNetServiceRef)service, flags);
	else
		MyAddService((CFNetServiceRef)service, flags);
	}

static Boolean MyBrowseService()
	{
	CFNetServiceClientContext context = { 0, NULL, NULL, NULL, NULL };
	CFStreamError error;
	Boolean result;

	gServiceDictionary = CFDictionaryCreateMutable(kCFAllocatorDefault,
		0, &kCFCopyStringDictionaryKeyCallBacks, NULL);
	assert(gServiceDictionary != NULL);

	gBrowserService = CFNetServiceBrowserCreate(kCFAllocatorDefault,
		MyBrowseCallBack, &context);
	assert(gBrowserService != NULL);

	CFNetServiceBrowserScheduleWithRunLoop(gBrowserService,
		CFRunLoopGetCurrent(), kCFRunLoopCommonModes);

	result = CFNetServiceBrowserSearchForServices(gBrowserService,
		CFSTR(""), CFSTR("_example._tcp"), &error);

	if (result == false) //clean up
		{
		CFNetServiceBrowserUnscheduleFromRunLoop(gBrowserService,
			CFRunLoopGetCurrent(), kCFRunLoopCommonModes);
		CFRelease(gBrowserService);
		gBrowserService = NULL;
		fprintf(stderr, "CFNetServiceBrowserSearchForServices returned %d, %ld)\n",
			error.domain, error.error);
		}

	return result;
	}

int main(int argc, char* argv[])
	{
	MyBrowseService();
	CFRunLoopRun();
	return 0;
	}
