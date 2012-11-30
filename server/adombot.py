import irclib
import signal
import sys
import time
from pyinotify import WatchManager, ThreadedNotifier, ProcessEvent, IN_CLOSE_WRITE, IN_MODIFY
import os

MIN_IRC_ANC = 2000

FILE111 = "/var/lib/adom/public_html/adom_hiscore/hiscore_v111.txt"
FILE100 = "/var/lib/adom/public_html/adom_hiscore/hiscore_v100.txt"
FILEETR = "/var/lib/adom/public_html/adom_hiscore/hiscore_vetr.txt"

def signal_handler(signal, frame):
    print "Received signal {0}".format(signal)
    notifier.stop()
    sys.exit(1)

signal.signal(signal.SIGINT, signal_handler) 

def poll_hiscore():
    global hiscore_100
    global hiscore_111
    global hiscore_etr

    new_100 = import_hiscore(FILE100)
    new_111 = import_hiscore(FILE111)
    new_etr = import_hiscore(FILEETR)

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

            if int(hiscore_line.split()[1].strip()) >= MIN_IRC_ANC:
                hiscore[key] = parsed

            hiscore_line = line
            
        else:
            hiscore_line += line

    key= " ".join((hiscore_line.split())[1:])
    parsed = " ".join((hiscore_line.split())[2:])
    parsed += " Rank: #" + hiscore_line.split()[0].strip() + ", score " + hiscore_line.split()[1].strip() + "."

    if int(hiscore_line.split()[1].strip()) >= MIN_IRC_ANC:
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

hiscore_111 = import_hiscore(FILE111)
hiscore_100 = import_hiscore(FILE100)
hiscore_etr = import_hiscore(FILEETR)

irc = irclib.IRC()
try:
    c = irc.server().connect(server, port, nickname, ircname=ircname, ssl=dossl)
except irclib.ServerConnectionError, x:
    print x
    sys.exit(1)

c.add_global_handler("welcome", on_connect)
c.add_global_handler("disconnect", on_disconnect)

wm = WatchManager()
class ScoreHandler(ProcessEvent):
    def process_IN_CLOSE_WRITE(self, evt):
        poll_hiscore()

handler = ScoreHandler()
notifier = ThreadedNotifier(wm, handler)
wd1 = wm.add_watch(FILE100, IN_CLOSE_WRITE)
wd2 = wm.add_watch(FILE111, IN_CLOSE_WRITE)
wd4 = wm.add_watch(FILEETR, IN_CLOSE_WRITE)

notifier.start()

try:
    irc.process_forever()
except KeyboardInterrupt:
    print "Caught ^C\n"
    notifier.stop()
    self.connection.quit("Aieeee!")
    sys.exit(1)

