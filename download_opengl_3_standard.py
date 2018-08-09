"""*************************************************************
*downloading glad for Opengl 3.3 version
"""
import sys
import urllib.request
def download(urlpath,localpath):
	url = urlpath
	f = urllib.request.urlopen(url)
	data = f.read()
	with open(localpath, "wb") as code:
		code.write(data)
	print(urlpath + " successful download")
		
print ("downloading with urllib")
download('https://glad.dav1d.de/generated/tmpo1EnXUglad/include/glad/glad.h',sys.path[0]+"/AutoEngine/RunTime/Includes/glad/glad.h")
download('https://glad.dav1d.de/generated/tmpo1EnXUglad/include/KHR/khrplatform.h',sys.path[0]+"/AutoEngine/RunTime/Includes/KHR/khrplatform.h")
download('https://glad.dav1d.de/generated/tmpo1EnXUglad/src/glad.c',sys.path[0]+"/EmptyProject/glad.c")
download('https://glad.dav1d.de/generated/tmpo1EnXUglad/src/glad.c',sys.path[0]+"/SampleProject/glad.c")
print ("downloading finish")
