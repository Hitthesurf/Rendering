import numpy as np

def Sphere(p, r):
    '''
    p: vec3 numpy array
    r: radius of sphere
    '''
    return np.sqrt(np.sum(p**2))-r