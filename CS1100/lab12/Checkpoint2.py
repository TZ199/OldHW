
import Tkinter as tk
import sys
sys.setrecursionlimit(5000)
## Modify this function to call itself recursively to draw smaller
## plus signs at the four end points of the current plus sign.
  
        
def draw(chart_1, rootx, rooty, length):
    chart_1.create_line(rootx, rooty+length, rootx, rooty-length, fill="black") 
    chart_1.create_line(rootx+length, rooty, rootx-length, rooty, fill="black") 
    chart_1.update()
    
    
 
def draw4(chart,rootx,rooty,length):
    draw(chart, rootx+length, rooty,length/2)
    draw(chart, rootx-length, rooty, length/2)
    draw(chart, rootx, rooty+length, length/2)
    draw(chart, rootx, rooty-length, length/2)
       
def start(chart, prop, level):
    if level == 0:
        draw(chart, prop[0][0], prop[1][0], prop[2][0])
        prop_temp1 = [[],[],[]]
        prop_temp1[0]+= prop[0]
        prop_temp1[1]+=prop[1]
        prop_temp1[2] +=prop[2] 
        prop[0] = []
        prop[1] = []
        prop[2][0]=prop_temp1[2][0]/2
        prop[0].append(prop_temp1[2][0]*(-1)+prop_temp1[0][0])
        prop[0].append(prop_temp1[2][0]+prop_temp1[0][0])
        prop[0].append(prop_temp1[0][0])
        prop[0].append(prop_temp1[0][0]) 
        prop[1].append(prop_temp1[1][0])
        prop[1].append(prop_temp1[1][0])
        prop[1].append(prop_temp1[2][0]*(-1)+prop_temp1[1][0])
        prop[1].append(prop_temp1[2][0]+prop_temp1[1][0])
        return prop   
    else:
        prop = start(chart, prop, level-1)
        print prop
        for i in range(0,len(prop[0]),4):
            draw(chart_1, prop[0][i], prop[1][i], prop[2][0])
            draw(chart_1, prop[0][i+1], prop[1][i+1], prop[2][0])
            draw(chart_1, prop[0][i+2], prop[1][i+2], prop[2][0])
            draw(chart_1, prop[0][i+3], prop[1][i+3], prop[2][0])
        prop_temp = [[],[],[]]
        prop_temp[0]+=prop[0]
        prop_temp[1]+=prop[1]
        prop_temp[2]+=prop[2]     
        prop[0] = []
        prop[1] = []        
        prop[2][0]=prop_temp[2][0]/2 
        for i in range(0,4 ** (level), 1):
            prop[0].append(prop_temp[2][0]+prop_temp[0][i])
            prop[0].append(prop_temp[2][0]*(-1)+prop_temp[0][i])
            prop[0].append(prop_temp[0][i])
            prop[0].append(prop_temp[0][i])  
     
        for i in range(0,4 ** (level),1):
            prop[1].append(prop_temp[1][i])
            prop[1].append(prop_temp[1][i])
            prop[1].append(prop_temp[2][0] + prop_temp[1][i])
            prop[1].append(prop_temp[2][0]*(-1)+prop_temp[1][i])
                
        return prop        
        
               
## Increases max level and calls the draw_plus function with the 
## given level. Note: maxlevel_info must be a list due to a 
## non-standard use of t0inter.

 
    
def restart(chart):
    chart_1.delete(tk.ALL)
    draw_plus(chart, 400,300, 150, 0, maxlevel_info[0])
    maxlevel_info[0] += 1


### The main code simply creates a canvas and two buttons. 
##  We use a global list called maxlevel to 0eep trac0 of 
##  how many iterations we would li0e to use for the recursion.
##  Whenever button2 is clic0ed, the maxlevel increases by one.
if __name__ == "__main__":
    root = tk.Tk()
    root.title("Recursion Example")
    chart_1 = tk.Canvas(root, width=800, height=600, background="white")
    chart_1.grid(row=0, column=0)
    maxlevel_info = [1]
    propla=[[400.0],[300.0],[150.0]]
    start(chart_1, propla,2)
    root.frame = tk.Frame(root)
    root.frame.button = tk.Button(root.frame, text="quit", \
                                  command=lambda:root.destroy())
    root.frame.button2 = tk.Button(root.frame, text="draw again!", \
                                   command=lambda:restart(chart_1))
    root.frame.button.grid()
    root.frame.button2.grid()
    root.frame.grid()
    root.mainloop()
