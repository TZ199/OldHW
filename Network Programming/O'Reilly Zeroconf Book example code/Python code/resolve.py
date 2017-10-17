#!/usr/bin/python
#
# Bonjour resolving sample
# 
# Usage: python resolve.py <serviceName> <serviceType> <serviceDomain>
#
# e.g. python resolve.py "The Name" _http._tcp

import sys
import bonjour
import select

# Callback for service resolving
def ResolveCallback(sdRef, flags, interfaceIndex,
                    errorCode, fullname, hosttarget,
                    port, txtLen, txtRecord, userdata):
    print "Service:", fullname
    print "is at", hosttarget, ":", port

if len(sys.argv) < 4:
    print "Usage: resolve.py serviceName serviceType serviceDomain"
    sys.exit(1)

serviceName   = sys.argv[1]
serviceType   = sys.argv[2]
serviceDomain = sys.argv[3]

# Allocate a service discovery ref and resolve the named service
serviceRef = bonjour.AllocateDNSServiceRef()
ret = bonjour.pyDNSServiceResolve(serviceRef,
                                  0,
                                  0,
                                  serviceName,
                                  serviceType,
                                  serviceDomain,
                                  ResolveCallback,
                                  None);

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
