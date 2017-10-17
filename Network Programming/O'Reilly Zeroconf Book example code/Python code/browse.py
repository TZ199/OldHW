#!/usr/bin/python
#
# Bonjour browsing sample
# 
# Usage: python browse.py <serviceType>
#
# e.g. python browse.py _http._tcp

import sys
import bonjour
import select

# Callback for service browsing
def BrowseCallback(sdRef, flags, interfaceIndex,
    errorCode, serviceName, regtype, replyDomain, userdata):
    if flags & bonjour.kDNSServiceFlagsAdd:
        print "Service added:   ", serviceName, regtype, replyDomain, interfaceIndex
    else:
        print "Service removed: ", serviceName, regtype, replyDomain, interfaceIndex

# Allocate a service discovery ref and browse for the specified service type
serviceRef = bonjour.AllocateDNSServiceRef()
ret = bonjour.pyDNSServiceBrowse(serviceRef,      # DNSServiceRef         *sdRef,
                                 0,               # DNSServiceFlags       flags,
                                 0,               # uint32_t              interfaceIndex,
                                 sys.argv[1],     # const char            *regtype,
                                 "",              # const char            *domain,
                                 BrowseCallback,  # DNSServiceBrowseReply callBack,
                                 None)

if ret != bonjour.kDNSServiceErr_NoError:
    print "ret = %d; exiting" % ret
    sys.exit(1)

# Get socket descriptor and loop
fd = bonjour.DNSServiceRefSockFD(serviceRef)
while 1:
    ret = select.select([fd], [], [])
    ret = bonjour.DNSServiceProcessResult(serviceRef)

# Deallocate the service discovery ref
bonjour.DNSServiceRefDeallocate(serviceRef)
