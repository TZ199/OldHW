
class Pig(object):
    def __init__( self, n, x0, y0, r0 ):
        self.name = n
        self.x = x0
        self.y = y0
        self.radius = r0
        self.popped = False

    def is_popped ( self ):
        return self.popped

    def pop( self ):
        self.popped = True
