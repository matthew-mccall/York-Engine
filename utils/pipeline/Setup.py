#!/usr/bin/env python3

# Desgined for use in automated builds only

import distro
import platform
import subprocess

os = platform.system()

if os == "Windows":
    subprocess.run(["pwsh", ".\\utils\\pipeline\\Windows.ps1"])
    subprocess.run(".\\utils\\Setup.bat")
    
else:
    subprocess.run("./utils/pipeline/InstallVulkan.sh")

    if distro.id() == "ubuntu":
        subprocess.run("./utils/pipeline/Ubuntu.sh")
        subprocess.run(["./utils/Setup.sh", "--install-sdl"])
    elif distro.id() == "fedora":
        subprocess.run("./utils/pipeline/Fedora.sh")
        subprocess.run("./utils/Setup.sh")
    else:
        subprocess.run("./utils/Setup.sh")
