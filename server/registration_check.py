import sys, os

cur_ip = os.environ["SSH_CONNECTION"].split()[0]

if os.path.exists("/var/lib/adom/tmp/registered_ips"):
  f = open("/var/lib/adom/tmp/registered_ips", 'r')

  lines = f.readlines()

  if len(lines) > 100:
    print "Sorry, too many users have registered today!"
    print "Please try again tomorrow. Thank you for your interest!"
    sys.exit(1)

  occurrences = 0

  for ip in lines:
    if ip.strip() == cur_ip:
        occurrences += 1

  if occurrences >= 3:
    print "Sorry, but there have already been 3 registrations"
    print "from your IP address. Please try again tomorrow."
    print "Thank you for your interest!"
    sys.exit(1)

  f.close()

if len(sys.argv) == 2 and sys.argv[1] == "--checkonly":
    sys.exit(0)

else:
  f = open("/var/lib/adom/tmp/registered_ips", 'a')
  f.write(cur_ip + "\n")
  f.close()

sys.exit(0)
