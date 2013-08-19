import irclib
import signal
import sys
import time
from pyinotify import WatchManager, ThreadedNotifier, ProcessEvent, IN_CLOSE_WRITE
import os
from tweepy import OAuthHandler, API, TweepError
from configobj import ConfigObj
import re
import random
import feedparser

#irclib.DEBUG = True

MIN_IRC_ANC = 2000
MIN_TWIT_ANC = 8000

FILE111 = "/var/lib/adom/public_html/adom_hiscore/hiscore_v111.txt"
FILE100 = "/var/lib/adom/public_html/adom_hiscore/hiscore_v100.txt"
FILE120p3 = "/var/lib/adom/public_html/adom_hiscore/hiscore_v120p3.txt"
FILE120p4 = "/var/lib/adom/public_html/adom_hiscore/hiscore_v120p4.txt"
FILE120p6 = "/var/lib/adom/public_html/adom_hiscore/hiscore_v120p6.txt"
FILE120p14 = "/var/lib/adom/public_html/adom_hiscore/hiscore_v120p14.txt"
FILE120p16 = "/var/lib/adom/public_html/adom_hiscore/hiscore_v120p16.txt"

LOCDIR = "/var/lib/adom/tmp/player_locations"

config = ConfigObj('/var/lib/adom/etc/config')

CONSUMER_KEY = config.get("CONSUMER_KEY")
CONSUMER_SECRET = config.get("CONSUMER_SECRET")
ACCESS_KEY = config.get("ACCESS_KEY")
ACCESS_SECRET = config.get("ACCESS_SECRET")

ANCTOIRC = config.as_bool('ANNOUNCE_TO_IRC')
ANCTOTWIT = config.as_bool("ANNOUNCE_TO_TWITTER")
READTWIT = config.as_bool("READ_TWEETS")
TWITNAME = config.get("TWITTERNAME")
ANCRSS = config.as_bool("READ_RSS")
URL = config.get("RSSURL")

#try to set up twitter
if ANCTOTWIT == True or READTWIT == True:
  try:
      auth = OAuthHandler(CONSUMER_KEY, CONSUMER_SECRET)
      auth.set_access_token(ACCESS_KEY, ACCESS_SECRET)
      api = API(auth)
  except TweepError as e:
      print "Error: could not set up OAuth. {0}".format(e)

#set up RSS
lastseen = None
if ANCRSS == True:
  print "Checking RSS ... "
  feed = feedparser.parse(URL)
  for key in feed["items"]:
    if key["date_parsed"] > lastseen:
      lastseen = key["date_parsed"]

maxseen = 0
#Start reading tweets
if READTWIT == True:
  print "Reading tweets ... "
  try:
    tbtweets = api.user_timeline(TWITNAME)
    for key in tbtweets:
      if key.id > maxseen:
        maxseen = key.id

  except TweepError as e:
    print "Error: Could not retrieve tweets. {0}".format(e)

def signal_handler(signal, frame):
    print "Received signal {0}".format(signal)
    notifier.stop()
    notifierloc.stop()
    s.quit("Augh!")
    sys.exit(1)

signal.signal(signal.SIGTERM, signal_handler) 
signal.signal(signal.SIGINT, signal_handler)

def tweet(version, text):
    if ANCTOTWIT != 1:
      return
    m = re.match(r"(.*?)\. (L\d{0,1}.*?) \((M|F)\)\. \d+ xps\. \d+ turns?\. (.*?)\. Score (\d+)",text)

    if not m:
        print "Regex did not match, no tweet sent, text: " + text
        return

    charuser = m.group(1)
    raceclass = m.group(2)
    score = int(m.group(5))
    if score <= MIN_TWIT_ANC:
        return

    mapping = [ ('drakish ', 'Dr'), ('dwarven ','Dw'), ('dark elven ', 'De'), ('gray elven ', 'Ge'), ('high elven ', 'He'), ('gnomish ','Gn'), ('human ','Hm'), ('hurthling ', 'Hr'), ('orcish ','Or'), ('trollish ','Tr'), ('ratling ','Rt'), ('mist elven ','Me'), ('archer','Ar'), ('assassin','As'), ('barbarian','Bb'), ('bard','Br'), ('beastfighter','Bf'), ('druid','Dr'), ('elementalist','El'), ('farmer','Fa'), ('fighter','Fi'), ('healer','He'), ('merchant','Me'), ('mindcrafter','Mi'), ('monk','Mo'), ('necromancer','Ne'), ('paladin','Pa'), ('priest','Pr'), ('ranger','Ra'), ('thief','Th'), ('weaponsmith','We'), ('wizard','Wz'), ('chaos Knight','Ck'), ('duelist','Du') ]

    for k, v in mapping:
        raceclass = raceclass.replace(k, v)

    reason =  m.group(4)

    if " while saving h" in reason:
        reason = reason[0:reason.find(" while saving h")]
    elif " without even stopping to save h" in reason:
        reason = reason[0:reason.find(" without even stopping to save h")]

    if "He was " in reason:
        reason = reason[7:]
    elif "She was " in reason:
        reason = reason[8:]
    elif "He " in reason:
        reason = reason[3:]
    elif "She " in reason:
        reason = reason[4:]


    newtext = "#ADOM {0} score: {1}, {2}, {3}.".format(version, charuser, raceclass, reason)
    if newtext.__len__() > 140:
        makeup = newtext.__len__() - 140
        rdiff = reason.__len__() - makeup - 2
        if rdiff < 1: #can't get under 140?
            return
        reason = reason[0:rdiff] + ".."

    newtext = "#ADOM {0} score: {1}, {2}, {3}.".format(version, m.group(1), raceclass, reason)
    print newtext
    try:
        api.update_status(newtext)
    except TweepError as e:
        print "Error sending tweet. {0}".format(e)

def poll_hiscore():
    global hiscore_100
    global hiscore_111
    global hiscore_120p3
    global hiscore_120p4
    global hiscore_120p6
    global hiscore_120p14
    global hiscore_120p16

    new_100 = import_hiscore(FILE100)
    new_111 = import_hiscore(FILE111)
    new_120p3 = import_hiscore(FILE120p3)
    new_120p4 = import_hiscore(FILE120p4)
    new_120p6 = import_hiscore(FILE120p6)
    new_120p14 = import_hiscore(FILE120p14)
    new_120p16 = import_hiscore(FILE120p16)

    ph = set(hiscore_100)
    diff_100 = [x for x in new_100 if x not in hiscore_100]
    ph = set(hiscore_111)
    diff_111 = [x for x in new_111 if x not in hiscore_111]
    ph = set(hiscore_120p3)
    diff_120p3 = [x for x in new_120p3 if x not in hiscore_120p3]
    ph = set(hiscore_120p4)
    diff_120p4 = [x for x in new_120p4 if x not in hiscore_120p4]
    ph = set(hiscore_120p6)
    diff_120p6 = [x for x in new_120p6 if x not in hiscore_120p6]
    ph = set(hiscore_120p14)
    diff_120p14 = [x for x in new_120p14 if x not in hiscore_120p14]
    ph = set(hiscore_120p16)
    diff_120p16 = [x for x in new_120p16 if x not in hiscore_120p16]

    hiscore_100 = new_100
    hiscore_111 = new_111
    hiscore_120p3 = new_120p3
    hiscore_120p4 = new_120p4
    hiscore_120p6 = new_120p6
    hiscore_120p14 = new_120p14
    hiscore_120p16 = new_120p16

    if ANCTOIRC == True and c.is_connected() == True:
        for key in diff_100:
            print key + " Version 1.0.0."
            c.privmsg(target, "\x02New high score\x02: " + key + " Version 1.0.0.")
            tweet("1.0.0", key);
            
        for key in diff_111:
            print key + " Version 1.1.1."
            c.privmsg(target, "\x02New high score\x02: " + key + " Version 1.1.1.")
            tweet("1.1.1", key);

        for key in diff_120p3:
            print key + " Version 1.2.0p1/2/3."
            c.privmsg(target, "\x02New high score\x02: " + key + " Version 1.2.0p1/2/3.")
            tweet("1.2.0p1/2/3", key);

        for key in diff_120p4:
            print key + " Version 1.2.0p4/5."
            c.privmsg(target, "\x02New high score\x02: " + key + " Version 1.2.0p4/5.")
            tweet("1.2.0p4/5", key);

        for key in diff_120p6:
            print key + " Version 1.2.0p6-13."
            c.privmsg(target, "\x02New high score\x02: " + key + " Version 1.2.0p6-13.")
            tweet("1.2.0p6-13", key);

        for key in diff_120p14:
            print key + " Version 1.2.0p14."
            c.privmsg(target, "\x02New high score\x02: " + key + " Version 1.2.0p14.")
            tweet("1.2.0p14", key);
        
        for key in diff_120p16:
            print key + " Version 1.2.0p16."
            c.privmsg(target, "\x02New high score\x02: " + key + " Version 1.2.0p16.")
            tweet("1.2.0p16", key);


def loc_changed(filename):
   if ANCTOIRC != 1:
     return

   player = filename.split("/")[-1]

   if not player:
      return
   
   location = ""
   with open(filename) as f:
      location = f.readlines()[0].strip()

   c.privmsg(target,player + " has just entered \x1F" + location + "\x1F! ")

def check_rss():
  if ANCRSS != True or c.is_connected() == False:
    return
  global lastseen
  newfeed = feedparser.parse(URL)
  for key in newfeed["items"]:
    if key["date_parsed"] > lastseen:
      lastseen = key["date_parsed"]
      c.privmsg(target, "\x02New blog post\x02: \x1F" + key["title"] + "\x1F @ " + key["link"])

  c.execute_delayed(1800, check_rss)

def check_tweets():
  if READTWIT != True or c.is_connected() == False:
    return

  global maxseen
  try:
    newtweets = api.user_timeline(TWITNAME, since_id=maxseen)
  except TweepError as e:
    print "Error: Could not retrieve tweets. {0}".format(e)
    return

  for key in newtweets:
    if key.in_reply_to_user_id == None: # and key.retweeted_status == None:
      c.privmsg(target, "\x02New tweet from the Creator\x02: \"" + key.text + "\"")
    if key.id > maxseen:
        maxseen = key.id

  c.execute_delayed(600, check_tweets)

def import_hiscore(file):
    f = open(file, "r")
    lines = f.readlines()

    if (len(lines) <= 0):
	return []
    for i in range(4):
        lines.pop(0)

    hiscore = []
    hiscore_line = lines.pop(0)
    for line in lines:
        #check for the next one
        match = re.search(r"^[ 0-9][ 0-9][0-9]", line)
        if match:
            rank = hiscore_line.split()[0].strip()
            parsed = " ".join((hiscore_line.split())[2:])
            parsed += " Score " + hiscore_line.split()[1].strip() + "."

            #strip date
            dated = re.search(r"(\S+ on \d{1,2}/\d{1,2}/\d{4}. )", parsed)
            if dated:
                parsed = parsed.replace(dated.group(0), "")

            if int(hiscore_line.split()[1].strip()) >= MIN_IRC_ANC:
                hiscore.append(parsed)

            hiscore_line = line

        # reached the end (EOF also falls through)
        elif "----------------" in line:
            break
        # else continue constructing
        else:
            hiscore_line += line

    # do the last
    rank = hiscore_line.split()[0].strip()
    parsed = " ".join((hiscore_line.split())[2:])
    parsed += " Score " + hiscore_line.split()[1].strip() + "."
    dated = re.search(r"(\S+ on \d{1,2}/\d{1,2}/\d{4}. )", parsed)
    if dated:
        parsed = parsed.replace(dated.group(0), "")

    if int(hiscore_line.split()[1].strip()) >= MIN_IRC_ANC:
        hiscore.append(parsed)

    return hiscore

def on_connect(connection, event):
    print "Connected. Joining channel..."
    connection.join(target)

def on_disconnect(connection, event):
    time.sleep(60)
    connection.connect(server, port, nickname, ircname=ircname, ssl=dossl)

server = config.get("IRCSERV")
port = config.as_int("IRCPORT")
nickname = config.get("ANCNICK")
target = config.get("IRCCHAN")

ircname = "Public ADOM Server bot - " + config.get("SERVNAME")

dossl = bool(config.get("IRCSSL"))
if (dossl != False) and (dossl != True):
        print "Invalid value for SSL: must be True or False"
        exit(1) 

hiscore_111 = import_hiscore(FILE111)
hiscore_100 = import_hiscore(FILE100)
hiscore_120p3 = import_hiscore(FILE120p3)
hiscore_120p4 = import_hiscore(FILE120p4)
hiscore_120p6 = import_hiscore(FILE120p6)
hiscore_120p14 = import_hiscore(FILE120p14)
hiscore_120p16 = import_hiscore(FILE120p16)

print "Connecting announce bot...\n"
irc = irclib.IRC()
try:
    s = irc.server();
    c = s.connect(server, port, nickname, ssl=dossl, ircname=ircname)
except irclib.ServerConnectionError, x:
    print x
    sys.exit(1)

c.add_global_handler("welcome", on_connect)
c.add_global_handler("disconnect", on_disconnect)

wm = WatchManager()
wmloc = WatchManager()

class ScoreHandler(ProcessEvent):
    def process_IN_CLOSE_WRITE(self, evt):
        poll_hiscore()

class LocHandler(ProcessEvent):
    def process_IN_CLOSE_WRITE(self, evt):
        loc_changed(evt.pathname)

handler = ScoreHandler()
handlerloc = LocHandler()

notifier = ThreadedNotifier(wm, handler)
notifierloc = ThreadedNotifier(wmloc, handlerloc)

wm.add_watch(FILE100, IN_CLOSE_WRITE)
wm.add_watch(FILE111, IN_CLOSE_WRITE)
wm.add_watch(FILE120p3, IN_CLOSE_WRITE)
wm.add_watch(FILE120p4, IN_CLOSE_WRITE)
wm.add_watch(FILE120p6, IN_CLOSE_WRITE)
wm.add_watch(FILE120p14, IN_CLOSE_WRITE)
wm.add_watch(FILE120p16, IN_CLOSE_WRITE)

wmloc.add_watch(LOCDIR, IN_CLOSE_WRITE)

notifier.start()
notifierloc.start()

if ANCRSS == True:
  c.execute_delayed(60, check_rss)
if READTWIT == True:
  c.execute_delayed(30, check_tweets)

irc.process_forever()
