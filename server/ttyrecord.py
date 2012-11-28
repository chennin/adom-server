import sys, os, time

user = os.environ["LOGNAME"];
recdir = os.environ["HOME"]+"/ttyrecs"

ps_lines = os.popen("pgrep -u " + str(os.getuid()) + " termrec").readlines()

if len(ps_lines) == 0:
	tstr = "-".join(time.ctime(time.time()).split())
	os.system("termrec -f ttyrec \"" + recdir + "/ttyrec-" + tstr + "\"")
	sys.exit(1)
	# os.system("/var/lib/adom/server/adom_wrapper|dosrecorder \"" + recdir + "/" + tstr + ".dm2\"");
	# os.system("kill `pgrep -u " + str(os.getuid()) + " adom_wrapper`")

sys.exit(0)
