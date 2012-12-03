import sys, os

f = open("/var/lib/adom/tmp/registered_ips")

lines = f.readlines()

if len(lines) > 100:
    print "Sorry, too many users have registered today!"
    print "Please try again tomorrow. Thank you for your interest!"
    sys.exit(1)

cur_ip = os.environ["SSH_CONNECTION"].split()[0]
occurrences = 0

for ip in lines:
    if ip.strip() == cur_ip:
        occurrences += 1

#if occurrences >= 3:
#    print "Sorry, but there have already been 3 registrations"
#    print "from your IP-address. Please try again tomorrow."
#    print "Thank you for your interest!"
#    sys.exit(1)

f.close()

if len(sys.argv) == 2 and sys.argv[1] == "--checkonly":
    sys.exit(0)

else:
    os.system("echo \"" + cur_ip + "\" >> /var/lib/adom/tmp/registered_ips")

sys.exit(0)
