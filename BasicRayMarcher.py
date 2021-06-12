import numpy as np
from PrimitiveObjectsDistanceFunctions import *
from Display import TextDisplay
import os
import time

#Z is the vertical axis

#Define constants
MAX_STEPS = 10 #int
MAX_DIST = 10. #float
MIN_STEP = 0.01 #float the distance at which a hit is registered

def Norm(vector):
    '''
    inputs
    ------
    vector : numpy array of any size
    
    Outputs
    -------
    The normalized version
    '''
    return vector / np.sqrt(np.sum(vector**2))
    

class Camera():
    def __init__(self, Pos = np.array([0.,0.,0.]), Dir = np.array([1.,0.,0.]), Res = np.array([46,30]), Zoom = 1.):
        '''
        inputs
        ------
        Pos : vec3 numpy array
            Position of camera
            
        Dir : vec3 numpy array
            Direction of camera, gets normalized
           
        Res : vec2 numpy array
            Number of pixels in x and y of the screen [x,y]
        
        Zoom : float
            The distance of the camera from the screen
        
        '''
        self.Pos = Pos
        self.Dir = Norm(Dir)
        self.Res = Res
        self.Zoom = Zoom


class Light():
    def __init__(self, Pos = np.array([1,0,5]), Lumens = 1):
        '''
        inputs
        ------
        Pos : vec3 numpy array
            Position of camera
            
        Lemens : float
            The brightness of the light
        
        '''
        self.Pos = Pos
        self.Lumens = Lumens
        
def SceneDF(Pos):
    return min(Sphere(Pos - np.array([3.,0.,1.]), 1.), Pos[2])
    

def RayMarcher(r_o, r_d):
    '''
    r_o : vec3 numpy array
        the start of the ray
    r_d : vec3 numpy array
        the diection of the ray, must be normalized
    '''

    s = 0 # distance travelled
    r_d = Norm(r_d)
    r_p = r_o.copy() #The position of the ray #Needs to be copy and not impact 
    # other versions as reference to that same object
    #we want copy by value not by reference, [:] acts as passing by value(python lists)
    # for numpy array .copy() for by value
    for step in range(MAX_STEPS):
        step_size = SceneDF(r_p)
        r_p += step_size*r_d #new ray_position
        s += step_size
        if step_size <= MIN_STEP:
            #Hit an object
            return s
        if s >= MAX_DIST:
            return s
    return s #Went over step limit
    
def GetHV(d):
    '''
    input
    -----
    d: vec3 numpy array
        this is the direction vector(the way the camera is pointing)
        must be normed
    
    output
    ------
    H,V: tuple of vec3 numpy array
        H=Horiztial screen direction
        V=Vertical screen direction
        calculates screen directions
    '''
    d = Norm(d)
    
    H = Norm(np.array([d[1],-d[0],0]))
    V = np.cross(H,d)
    
    return V,H
    

def mainImage(pixel, Camera, Lights):
    '''
    input
    -----
    pixel: vec2 numpy array
        the position of the pixel on the screen [0,0] is bottom left
    
    output
    ------
    pixel brightness
    
    '''
    
    uv = (pixel - 0.5*Camera.Res)/Camera.Res[1] #As y res is smaller make system correctly scaled
    #Now uv = [0,0] is the center
    
    V, H = GetHV(Camera.Dir)
    
    
    ro = Camera.Pos #ray_orign (camera position)
    rd = Norm(Camera.Zoom*Camera.Dir + uv[0]*H + uv[1]*V) #Ray direction
    
    s = RayMarcher(ro, rd) #Distance untill hit 
    
    return s
    
def GetGrid(Camera, Lights):
    '''
    each loop meant to be done on different cores(Huge advantage when using GPU but not in python)
    
    Output
    ------
    Grid of outputs from mainImage
    '''
    x_size = Camera.Res[0]
    y_size = Camera.Res[1]
    Grid = np.zeros((y_size,x_size))
    for x in range(x_size):
        for y in range(y_size):
            Grid[-(y+1)][x] = mainImage(np.array([x,y]), Camera, Lights)
    
    return Grid
    
if __name__ == "__main__":
    my_Camera = Camera(Zoom=0.5, Dir = np.array([1.,0.,-0.2]),
                                 Pos = np.array([0.0,0.0,1.0]), 
                                 Res = np.array([80,40]))
    my_Light = Light()
    start_time = time.time()
    Grid = GetGrid(my_Camera, my_Light)
    os.system('cls' if os.name == 'nt' else 'clear')
    TextDisplay(Grid)
    print(time.time()-start_time)
    
 
    
