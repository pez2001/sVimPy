"""
  Play
  Plays a mp3 file
 
  This example code is in the public domain.
  
"""
from fmod import fmod
f = fmod()
f.playSound("test.mp3","")
f.Sleep(154000)              
print("playing done");
