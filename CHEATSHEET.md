# Systems Engineering & Terminal Cheat Sheet

## 1. Daily Startup & Connections
* Start VirtualBox silently in background: Choose **Headless Start** in 
the UI. * One-word Mac Terminal shortcut to connect: `budgie`

### How did i set up the ssh?* The way to set up an SSh into a linux machine is : `sudo -p [PORT] name@ipaddress`

### Enter the profile file 
* `nano ~/.zshrc` on mac 
* `nano ~/.bashrc` on linux

### input this at the bottom of the file
* `alias nickname="ssh-p [PORT] name@ipaddress"`

### wait, how did i open ssh on the linux itself? 
* `sudo apt update && sudo apt install -y openssh-server`

## 2. Force-Clearing Stubborn Space & Zombie RAM
* Force kill zombie build processes locking RAM: `killall -9 node cargo rustc python3`
* Force delete locked folder/app: `sudo rm -rf /path/to/folder`
* Wipe stuck files inside Mac Trash can:
  `sudo rm -rf ~/.Trash/*`

## 3. Fixing SSH Fingerprint Errors
* Remove old port 2222 key from Mac memory: `ssh-keygen -R ":2222"`
* Surgically delete a corrupted line in known_hosts (e.g., line 4):
  `sed -i '' '4d' ~/.ssh/known_hosts`
* best use is actually `rm ~/.ssh/known_hosts`

## 4. Daily Safe Shutdown
* Run inside Linux SSH before closing your laptop: `sudo shutdown -h now`

### Safe copy files/folders into a device you SSH into (Run from Host/Mac terminal)
* `scp -P [PORT] -r /Full/Path/To/Source/Folder username@destination_ip:~/`
* Example: `scp -P 2222 -r ~/Desktop/C_projects ohimai@127.0.0.1:~/`

