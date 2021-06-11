import numpy as np

#Z is the vertical axis

#Define constants
MAX_STEPS = 10 #int
MAX_DIST = 10. #float
dHIT_DIST = 0.01 #float

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
    def __init__(self, Pos = np.arrat([0,0,1]), Dir = np.array([1,0,0]), Res = [25,20], Zoom = 1):
        '''
        inputs
        ------
        Pos : vec3 numpy array
            Position of camera
            
        Dir : vec3 numpy array
            Direction of camera, gets normalized
           
        Res : 2 element list
            Number of pixels in x and y of the screen [x,y]
        
        Zoom : float
            The distance of the camera from the screen
        
        '''
        self.Pos = Pos
        self.Dir = Norm(self.Dir)
        self.Res = Res
        self.Zoom = Zoom


class Light():
    def __init__(self, Pos = np.arrat([1,0,5]), Lumens = 1):
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
