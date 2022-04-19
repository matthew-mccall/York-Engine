#!/usr/bin/env python3

import platform
import subprocess
import sys

os = platform.system()
args = sys.argv

if os == "Windows":
    subprocess.run("./utils/Setup.bat")
else:
    if 'ubuntu' in platform.platform().lower():
        print("Installing VulkanSDK and other build tools on Ubuntu")
        subprocess.run("./utils/pipeline/Ubuntu.sh")
        
    args[0] = "./utils/Setup.sh"
    subprocess.run(args)