import ctypes
from numpy.ctypeslib import ndpointer
import numpy as np

class WAV(object):
    def __init__(self, path, n_channel, samplerate=16000):
        #self.lib = ctypes.cdll.LoadLibrary(path)
        self.lib = ctypes.cdll.LoadLibrary(path)
    #print(dir(lib))

        self.lib.WAV_new.argtypes = [ctypes.c_int,ctypes.c_int]
        self.lib.WAV_new.restype = ctypes.c_void_p

        self.lib.WAV_NewFile.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
        self.lib.WAV_NewFile.restype = ctypes.c_void_p

        self.lib.WAV_Finish.argtypes = [ctypes.c_void_p]
        self.lib.WAV_Finish.restype = ctypes.c_void_p

        self.lib.WAV_Print.argtypes = [ctypes.c_void_p]
        self.lib.WAV_Print.restype = ctypes.c_void_p

        self.lib.WAV_Append.argtypes = [ctypes.c_void_p, ndpointer(ctypes.c_short,flags="C_CONTIGUOUS"),ctypes.c_uint]
        self.lib.WAV_Append.restype = ctypes.c_void_p

        self.obj = self.lib.WAV_new(n_channel,samplerate)

        print("INFO::WAV::loaded")

    def NewFile(self,filename):
        self.lib.WAV_NewFile(self.obj, filename)

    def Finish(self):
        self.lib.WAV_Finish(self.obj)

    def Print(self):
        self.lib.WAV_Print(self.obj)

    def Append(self, app_data,app_size):
        return self.lib.WAV_Append(self.obj,app_data,app_size)
        