import termios, fcntl, struct, sys

def get_termsz():
    s = struct.pack("HHHH", 0, 0, 0, 0)
    fd_stdout = sys.stdout.fileno()
    x = fcntl.ioctl(fd_stdout, termios.TIOCGWINSZ, s)
    termsz = struct.unpack("HHHH", x)
    return termsz[0], termsz[1]

sz = get_termsz()

if len(sys.argv) == 2 and sys.argv[1] == "--silent":
    silent = True
else:
    silent = False

if sz[0] != 25 or sz[1] != 80:
    if silent == False:
        print "Your terminal size appears to be " + str(sz[1]) + "x" + str(sz[0]) + " characters. A 80x25 -sized terminal is REQUIRED for playing ADOM on this server. Please correct your terminal settings and try again. You may want to consult the readme, available at the main menu, and at http://ancardia.ath.cx/adom_server_info.txt"

    sys.exit(1)

else:
    sys.exit(0)
