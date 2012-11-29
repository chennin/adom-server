import irclib
import sys
import time
import os

def poll_hiscore(hiscore_100, hiscore_111, hiscore_etr):
    new_100 = import_hiscore("/var/lib/adom/public_html/adom_hiscore/hiscore_v100.txt")
    new_111 = import_hiscore("/var/lib/adom/public_html/adom_hiscore/hiscore_v111.txt")
    new_etr = import_hiscore("/var/lib/adom/public_html/adom_hiscore/hiscore_eternium_man.txt")

    diff_100 = set(new_100.keys()).difference(hiscore_100.keys())
    diff_111 = set(new_111.keys()).difference(hiscore_111.keys())
    diff_etr = set(new_etr.keys()).difference(hiscore_etr.keys())

    hiscore_100 = new_100
    hiscore_111 = new_111
    hiscore_etr = new_etr

    if c.is_connected() == True:
        for key in diff_100:
            print hiscore_100[key] + " Version 1.0.0."
            c.privmsg(target, hiscore_100[key] + " Version 1.0.0.")
            
        for key in diff_111:
            print hiscore_111[key] + " Version 1.1.1."
            c.privmsg(target, hiscore_111[key] + " Version 1.1.1.")

        for key in diff_etr:
            print hiscore_etr[key] + " Played The Eternium Man challenge."
            c.privmsg(target, hiscore_etr[key] + " Played The Eternium Man challenge.")


    status_names = []
    cur_players = []

    for root, dirs, files in os.walk("/var/lib/adom/player_locations"):
        if root != "/var/lib/adom/player_locations":
            continue

        for s in files:
            pid = s.split("-")[0]
            
            if len(os.popen("ps -p " + pid).readlines()) < 2:
                os.system("rm -f " + root + "/" + s)
                continue

            status_names.append(s)

    for root, dirs, files in os.walk("/var/lib/adom/sockets"):
        if root != "/var/lib/adom/sockets":
            continue
            
        for s in files:
            cur_players.append(s)
        
    for player in cur_players:
        ps_lines = os.popen("ps -fu " + player + " |grep /var/lib/adom/bin/adom-111-bin").readlines()

        if len(ps_lines) < 1:
            continue

        pid = (ps_lines[0].split())[1]

        toef_name = str(pid) + "-toef"
        d50_name = str(pid) + "-d50"

#        if (toef_name in status_names) and ((toef_name + ".announced") not in status_names):
#            c.privmsg(target, player + " entered the final level of the Tower of eternal flames on the server.")
#            open("/var/lib/adom/player_locations/" + toef_name + ".announced", "w")

#        if (d50_name in status_names) and ((d50_name + ".announced") not in status_names):
#            c.privmsg(target, player + " entered D:50 on the server.")
#            open("/var/lib/adom/player_locations/" + d50_name + ".announced", "w")

    c.execute_delayed(60, poll_hiscore, (hiscore_100, hiscore_111, hiscore_etr))

def import_hiscore(file):
    f = open(file, "r")
    lines = f.readlines()

    for i in range(4):
        lines.pop(0)

    hiscore = {}
    hiscore_line = lines.pop(0)

    for line in lines:
        if "Died on " in line:
            line = line[0:line.find("Died on ")]
            
        elif "Won on " in line:
            line = line[0:line.find("Won on ")]

        elif "Ascended on " in line:
            line = line[0:line.find("Ascended on ")]

        if "(M)" in line or "(F)" in line:
            key = " ".join((hiscore_line.split())[1:])
            parsed = " ".join((hiscore_line.split())[2:])
            parsed += " Rank: #" + hiscore_line.split()[0].strip() + ", score " + hiscore_line.split()[1].strip() + "."

            if int(hiscore_line.split()[1].strip()) >= 8000:
                hiscore[key] = parsed

            hiscore_line = line
            
        else:
            hiscore_line += line

    key= " ".join((hiscore_line.split())[1:])
    parsed = " ".join((hiscore_line.split())[2:])
    parsed += " Rank: #" + hiscore_line.split()[0].strip() + ", score " + hiscore_line.split()[1].strip() + "."

    if int(hiscore_line.split()[1].strip()) >= 8000:
        hiscore[key] = parsed

    return hiscore

def on_connect(connection, event):
    connection.join(target)

if len(sys.argv) < 4:
    print "Usage: adombot <server[:port]> <nickname> <target> [realname] [ssl]"
    sys.exit(1)

def on_disconnect(connection, event):
    time.sleep(60)
    connection.connect(server, port, nickname, ircname=ircname, ssl=dossl)

s = sys.argv[1].split(":", 1)
server = s[0]
if len(s) == 2:
    try:
        port = int(s[1])
    except ValueError:
        print "Error: Erroneous port."
        sys.exit(1)
else:
    port = 6667
nickname = sys.argv[2]
target = sys.argv[3]

if len(sys.argv) >= 5:
        ircname = sys.argv[4]
else:
        ircname = nickname

if len(sys.argv) >= 6:
        dossl = bool(sys.argv[5])
else:
        dossl = False

if (dossl != False) and (dossl != True):
        print "Invalid value for SSL: must be True or False"
        exit(1) 

hiscore_111 = import_hiscore("/var/lib/adom/public_html/adom_hiscore/hiscore_v111.txt")
hiscore_100 = import_hiscore("/var/lib/adom/public_html/adom_hiscore/hiscore_v100.txt")
hiscore_etr = import_hiscore("/var/lib/adom/public_html/adom_hiscore/hiscore_vetr.txt")

irc = irclib.IRC()
try:
    c = irc.server().connect(server, port, nickname, ircname=ircname, ssl=dossl)
except irclib.ServerConnectionError, x:
    print x
    sys.exit(1)

c.add_global_handler("welcome", on_connect)
c.add_global_handler("disconnect", on_disconnect)
c.execute_delayed(60, poll_hiscore, (hiscore_100, hiscore_111, hiscore_etr))

irc.process_forever()
