"""*************************************************************
*set this script download necessary library 
*Configure the environment variables required by the engine
"""
import os
import sys
import fileinput
import shutil
import subprocess

def _check_python_version():
    major_ver = sys.version_info[0]
    if major_ver > 3:
        print ("The python version is %d.%d. But python 3.x is required. (Version 3.6 is well tested)\n"
               "Download it here: https://www.python.org/" % (major_ver, sys.version_info[1]))
        return False

    return True

class SetEnvVar(object):
	RESULT_UPDATE_FAILED = -2
    RESULT_ADD_FAILED = -1
    RESULT_DO_NOTHING = 0
    RESULT_UPDATED = 1
    RESULT_ADDED = 2
	def __get_autoengine_path(self):
		return self