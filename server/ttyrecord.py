import sys, os, time

user = os.environ["LOGNAME"];
recdir = os.environ["HOME"]+"/ttyrecs"

ps_lines = os.popen("pgrep -u " + str(os.getuid()) + " termrec").readlines()

if len(ps_lines) == 0:
	tstr = "-".join(time.ctime(time.time()).split())
	os.system("termrec -f ttyrec \"" + recdir + "/ttyrec-" + tstr + ".gz\"")
	sys.exit(1)

sys.exit(0)
