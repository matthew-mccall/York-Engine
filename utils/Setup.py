#!/usr/bin/env python3

import platform
import subprocess
import sys

os = platform.system()
args = sys.argv

if os == "Windows":
    subprocess.call("./utils/Setup.bat")
else:
    if 'ubuntu' in platform.platform().lower():
        subprocess.call("./utils/pipeline/Ubuntu.sh")
        
    args[0] = "./utils/Setup.sh"
    subprocess.call(args)