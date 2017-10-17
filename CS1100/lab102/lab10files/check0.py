import Tkinter as tk

class BallDraw(object):
    def __init__ (self, parent):
        ##=====DATA RELEVANT TO BALL===============
        ##  We are going to repeatedly draw a ball on the canvas, "moving"
        ##  it across the canvas.  The ball is specified by (a) its x and
        ##  y center coordinates (a tuple), (b) its radius, (c) the delta
        ##  x and delta y to move the ball in each time increment, and (d)
        ##  its color.
        (self.ball_x,self.ball_y) = 80,200    # initial location
        self.ball_radius = 10
        (self.ball_dx,self.ball_dy) = 6,-3
        self.ball_color = "red"

        #========DATA NEEDED FOR ANIMATION=========
        #  Here is the time in milliseconds between consecutive instances
        #  of drawing the ball.  If this time is too small the ball will
        #  zip across the canvas in a blur.
        self.wait_time = 100 

        #this will allow us to stop moving the ball when the program quits
        self.isstopped = False 

        #=============CREATE THE NEEDED GUI ELEMENTS===========
        self.maxx = 400 # canvas width, in pixels
        self.maxy = 400 # canvas height, in pixels

        ##  Create a frame, attach a canvas and a button to this frame
        ##  Button is used to cleanly exit the program
        ##  Canvas, like an image, is an object that we can draw objects on.
        ##  This canvas is called chart_1.  
        ##  Parent = root window, contains a frame
        ##  The frame contains the canvas and the quit button.
        ##  We only care about the canvas in our animation
        self.parent = parent
        self.frame = tk.Frame(parent)
        self.frame.pack()
        self.chart_1 = tk.Canvas(self.frame, \
                                     width=self.maxx,\
                                     height=self.maxy,\
                                     background="white")
        self.chart_1.grid(row=0, column=0)
        self.chart_1.pack()
        self.quit = tk.Button(self.frame, text="Quit", command=self.quit)
        self.quit.pack()

    def quit(self):
        self.isstopped = True
        self.parent.destroy()
        
    def draw_ball(self):
        #  Remove all the previously-drawn balls
        self.chart_1.delete(tk.ALL)
        
        # Draw an oval on the canvas within the bounding box
        bounding_box = (self.ball_x-self.ball_radius, \
                            self.ball_y-self.ball_radius,\
                            self.ball_x+self.ball_radius, \
                            self.ball_y+self.ball_radius) 
        self.chart_1.create_oval(bounding_box, fill=self.ball_color)
        self.chart_1.update()      # Actually refresh the drawing on the canvas.

        # Pause execution.  This allows the eye to catch up
        self.chart_1.after(self.wait_time)


    def animate(self):
        ##  Loop until the ball runs off the screen.
        while 0 < self.ball_x + self.ball_radius and \
                self.ball_x - self.ball_radius < self.maxx and \
                0 < self.ball_y + self.ball_radius and \
                self.ball_y - self.ball_radius < self.maxy and \
                not self.isstopped:
            # Move the ball
            self.draw_ball()
            self.ball_x += self.ball_dx
            self.ball_y += self.ball_dy



if __name__ == "__main__":
    ##  We will create a root object, which will contain all 
    ##  our user interface elements
    ##
    root = tk.Tk()
    root.title("Lab 1")

    ## Create a class to handle all our animation
    bd = BallDraw(root)

    ## Run the animation by continuously drawing the ball and then moving it
    bd.animate()

    ## This is an infinite loop that allows the window to listen to
    ## "events", which are user inputs.  The only user event here is
    ## closing the window, which ends the program. 
    root.mainloop()


    
