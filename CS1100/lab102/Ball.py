import Tkinter as tk

class Ball(object):
    def __init__ (self,x,y,dx,dy,r,color):
        (self.ball_x,self.ball_y) = x,y
        self.ball_radius = r
        (self.ball_dx,self.ball_dy) = dx,dy
        self.ball_color = color
        self.isstopped = False 
        
    def position(self):
        return (self.ball_x, self.ball_y)
        
    def move(self):
        self.ball_x += self.ball_dx
        self.ball_y += self.ball_dy
        
    def bounding_box(self):
        return (self.ball_x-self.ball_radius, \
                            self.ball_y-self.ball_radius,\
                            self.ball_x+self.ball_radius, \
                            self.ball_y+self.ball_radius) 

    def get_color(self):
        return self.ball_color
    
    def some_inside(self,maxx,mayy):
        while 0 < self.ball_x + self.ball_radius and \
                self.ball_x - self.ball_radius < maxx and \
                0 < self.ball_y + self.ball_radius and \
                self.ball_y - self.ball_radius < maxy and \
                not self.isstopped:
            return True       
