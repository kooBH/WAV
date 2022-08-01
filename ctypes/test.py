from WAV import WAV
import numpy as np

wav = WAV("build/libWAV.so",1,16000)

wav.NewFile("hello.wav")
wav.Print()

x = np.zeros((16000))

wav.Append(x.astype(np.int16),16000)

x = np.random.rand(16000)

x = x -0.5
x = x*32767
wav.Append(x.astype(np.int16),16000)
wav.Finish()