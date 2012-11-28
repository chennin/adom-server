import os, sys, time

def dotwait(secs):
    for i in range(secs):
        time.sleep(1);
        sys.stdout.write(".")

print "KILLING YOUR PROCESSES"
print "----------------------"
print ""
print "Please give a brief explanation for why you are doing this."
print "It must be at least 10 characters long, and admins will"
print "most likely read it. Thank you :)"
print ""
print "Note: for interactive help, please join #adom on irc.freenode.org!"
print ""
print "Type in your message and hit enter. Your ADOM processes will then"
print "be slaughtered."
print ""
print "> ",

while True:
    msg = sys.stdin.readline()
    if len(msg) >= 10: break

    print "Your message is not long enough."
    print "> ",

#irc = os.popen("sic -h localhost &> /dev/null", "w")

#dotwait(3)
#irc.write(":m #ancardia jaakkos, sorear (problem report): %s\n" % (msg))
#irc.write("quit")
#irc.close()

print ""
print "Your message was hopefully delivered! :) Killing processes soon..."
time.sleep(3)

os.system("trap '' HUP; killall -s 9 -u %s" % (os.getenv("USER")))
