class Ball(object):
    def __init__(self,x0,y0,vx,vy,radius,color):
        self.x = x0
        self.y = y0
        self.dx = vx
        self.dy = vy
        self.r = radius
        self.c = color
        
    def position(self):
        return (self.x,self.y)
    
    def move(self):
        self.x += self.dx
        self.y += self.dy
        self.check_and_reverse(self.maxx,self.maxy)
        
    def bounding_box(self):
        return (self.x-self.r, self.y-self.r, self.x+self.r, self.y+self.r)
    
    def get_color(self):
        return self.c
    
    def some_inside(self,mx,my):
        self.maxx = mx
        self.maxy = my
        if 0 < self.x + self.r and \
                self.x - self.r < self.maxx and \
                0 < self.y + self.r and \
                self.y - self.r < self.maxy:
            result=True
        else:
            result=False
        return result
            
    
    def check_and_reverse(self,mx,my):
        self.maxx = mx
        self.maxy = my        
        if self.x-self.r <= 0 and self.dx > 0:
            self.dx = self.dx
            return 
        if self.x+self.r >= self.maxx and self.dx <0:
            self.dx = self.dx
            return
        if self.x-self.r<=0 or self.x+self.r >= self.maxx:
            self.dx = -self.dx
            return
        if self.y-self.r <= 0 and self.dy > 0:
            self.dy = self.dy
            return
        if self.y+self.r >= self.maxy and self.dy <0:
            self.dy = self.dy
            return
        if self.y-self.r<=0 or self.y+self.r >= self.maxy:
            self.dy = -self.dy
            return
