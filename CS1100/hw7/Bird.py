import math

class Bird(object):
    def __init__(self, n, x0, y0, r0, x1, y1):
        self.name = n
        self.x = x0
        self.y = y0
        self.radius = r0
        self.dx = x1
        self.dy = y1

    def move(self):
        self.x += self.dx
        self.y += self.dy

    def decrease_speed(self):
        self.dx = self.dx * 0.5

    def speed(self):
        return math.sqrt(self.dx * self.dx + self.dy * self.dy)

    def is_poped(self,pig):
        x = self.x - pig.x
        y = self.y - pig.y
        distance = math.sqrt(x * x + y * y)
        r = self.radius + pig.radius
        return distance <= r

    def is_left(self):
        if self.x - self.radius < 0:
            return True
        if self.y - self.radius < 0:
            return True
        if self.x + self.radius > 1000:
            return True
        if self.y + self.radius > 1000:
            return True
        return False
