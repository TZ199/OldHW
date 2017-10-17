
##11/11/14

class Ball(object):
    def __init__(self,x,y,dx,dy,radius,color):
        self.x=x
        self.y=y
        self.dx=dx
        self.dy=dy
        self.radius=radius
        self.color=color
    def position(self):
        return (self.x,self.y)
    def move(self):
        self.x+=self.dx
        self.y+=self.dy
    def bounding_box(self):
        tu=(self.x-self.radius,self.y-self.radius,self.x+self.radius,self.y+self.radius)
        return tu
    def get_color(self):
        return self.color
    def some_inside(self,maxx,maxy):
        return 0 < self.x + self.radius and \
                self.x - self.radius < maxx and \
                0 < self.y + self.radius and \
                self.y - self.radius < maxy 
    def check_and_reverse(self,maxx,maxy):
        if  self.x - self.radius<=0 or self.x + self.radius >= maxx:
            self.dx=0-self.dx
        elif self.y-self.radius<=0 or self.y +self.radius >=maxy:
            self.dy=0-self.dy
