""" Given two locations with their latitude and longitude, 
    finds the distance in miles between them.

"""
import math

def dist(lat1,long1,lat2,long2):
    lat1 *= math.pi / 180.0
    long1 *= math.pi / 180.0
    lat2 *= math.pi / 180.0
    long2 *= math.pi / 180.0

    #  Now the real work.
    dlat = (lat1-lat2)
    dlong = (long1-long2)
    a = math.sin(dlat/2)**2 + \
        math.cos(lat1) * math.cos(lat2) * math.sin(dlong/2)**2
    c = 2*math.atan2( math.sqrt(a), math.sqrt(1-a) )
    R = 6371 / 1.609
    return R*c
