import sys, os

f = os.popen("ps aux|grep \"screen -x -c /var/lib/adom/server/spectator-screen -r\"")
output = f.readlines()

f_players = os.popen("ls /var/lib/adom/sockets/")
players_raw = f_players.readlines()
players = []

for line in players_raw:
    players.append(line.strip())

spectator_numbers = {}

for line in output:
    parts = line.split()

    if len(parts) < 10:
        continue

    elif parts[-2] not in players:
        continue

    if parts[-2] in spectator_numbers.keys():
        spectator_numbers[parts[-2]] += 1

    else:
        spectator_numbers[parts[-2]] = 1

if len(spectator_numbers.keys()) > 0:
    print "Currently viewing:"
    print

for entry in spectator_numbers.items():
    print entry[0] + " - " + str(entry[1]) + " spectator(s) watching"

if len(spectator_numbers.keys()) > 0:
    print
