#!/usr/bin/env python
import sys, os, subprocess, errno

def print_time(mins):
    hours = mins/60
    minutes = mins%60
    timestr = ""

    if hours > 0:
        timestr = str(hours) + " hours, "

    timestr += str(minutes) + " minutes"

    return timestr

if len(sys.argv) != 2:
    print "Usage: update_idletimes <minutes>"
    sys.exit(1)

idletime_inc = int(sys.argv[1])

f_players = os.popen("ls /var/lib/adom/sockets/")
players_raw = f_players.readlines()
players = []

for line in players_raw:
    players.append(line.strip())

try:
    os.remove("/var/lib/adom/tmp/idletimes")
except OSError, e:
    if e.errno != errno.ENOENT:
        raise

idlelistf = open("/var/lib/adom/tmp/idletimes", "w")

for player in players:
    if len(os.popen("ps -u " + player).readlines()) < 2:
        os.system("rm -f /var/lib/adom/sockets/" + player)
        continue

    ps_lines = os.popen("ps -fu " + player + " |grep /var/lib/adom/bin/adom-| grep -E -- '-bin'").readlines()

    if len(ps_lines) < 1:
        continue

    pid = (ps_lines[0].split())[1]
    last_hash = ""
    cur_hash = (os.popen("md5sum /proc/" + pid + "/stat").readline().split())[0]
    idletime = 0

    version = "UNK"

    try:
        with open("/var/lib/adom/sockets/" + player) as f:
            lines = f.readlines()
            version = lines[0]
            version = version.strip()
    except:
        pass


    try:
        last = open("/var/lib/adom/users/" + player + "/idle_info", "r")
        last_hash = last.readline().strip()
        idletime = int(last.readline().strip())

    except:
        idleinfo = open("/var/lib/adom/users/" + player + "/idle_info", "w")
        idleinfo.write(cur_hash + "\n")
        idleinfo.write("0" + "\n")
        idlelistf.write(player + ": 0 minutes idle (v" + version + ")\n")
        continue
    
    if cur_hash != last_hash:
        idleinfo = open("/var/lib/adom/users/" + player + "/idle_info", "w")
        idleinfo.write(cur_hash + "\n")
        idleinfo.write("0" + "\n")
        idlelistf.write(player + ": 0 minutes idle (v" + version + ")\n")

    else:
        idleinfo = open("/var/lib/adom/users/" + player + "/idle_info", "w")
        idleinfo.write(cur_hash + "\n")
        idleinfo.write(str(idletime+idletime_inc) + "\n")

	if player in loggedin:
		idlelistf.write(player + ": " + print_time(idletime+idletime_inc) + " idle (v" + version + ")\n")
