#!/usr/bin/env python3

# Desgined for use in automated builds only

import platform
import subprocess
import sys

os = platform.system()
args = sys.argv

if os == "Windows":
    subprocess.run(["pwsh", ".\\utils\\pipeline\\Windows.ps"])
    subprocess.run("./utils/Setup.bat")
    
else:
    args[0] = "./utils/Setup.sh"
    
    subprocess.run("./utils/pipeline/Ubuntu.sh") 
    subprocess.run(args)