# MOVED - UPDATE YOUR LINKS

https://github.com/chennin

Public ADOM Server scripts README.

**See LICENSE for copyright and licensing details.**

**Table of contents:**

1. [About](#about)
2. [Features](#features)
3. [Install](#install)
4. [Usage](#usage)
5. [Administration](#administration)
6. [Changelog](#changelog)
7. [Contributing](#contributing)
8. [Contact](#contact)
9. [Authors](#authors)
	
About
======

These are scripts to support running a multi-user, multi-version ADOM server,
with useful mods for the game, IRC bots useful for ADOM players, and social
features.

Why?

- ADOM was not at all designed to run multiple versions on the same system.
- For the 10 years with no new versions, mods were the only way to prevent or 
  recover from crashes.
- Mods add neat features like a starsign selector and autoswap with friendlies.
- Database dumps provide invaluable information to the optimal player.
- It is a single player game, but watching other players and seeing their wins
  (or deaths) is social, and having a win be recorded publicly is a great
  ego boost.

ADOM can be found at http://adom.de or http://ancardia.com. 

Features
=========

1. Play multiple versions of ADOM on the same system without overlap
2. Allow any number of users to play on the same system
3. Automatic save backup and restore in case of an ADOM crash
4. Via ADoM Sage, convenience features and starsign selector
5. Never lose play time if your Internet dies or battery runs out
6. Play co-op mode (multiple users controlling the same game)
7. Spectate other users' games
8. Send messages to other playing users
9. Record games and play them back
10. Allow download of save games and other files via HTTP
11. Guidebook built in
12. Announce IRC bot:
    1. Announce new high scores (wins and deaths) for all versions on IRC
    2. Announce new high scores (with a different, higher threshold) on Twitter
    3. Announce when a player enters certain locations (to attract spectators)
13. Database IRC bot: relays exact item and monster details
14. Almost entirely self-contained; once set up, root is no longer required

Install
========

A basic x86 or x86_64 Linux system is required*. The distribution should be 
relatively modern; Debian 6+ or Ubuntu 12.04+ are recommended, but almost any 
will work.

\* Linux/ARM or FreeBSD may also work, as ADOM supports those, but this product 
  does not entirely support that.

Please carefully read the included INSTALL file, as there are many moving parts
to touch.

**IMPORTANT:**

The INSTALL file provides details and the process, but in particular:

 * /var/lib/adom must exist and be empty, as for now it must be used as the
   server's base dir.
 * A non-root user must be created to own the server files.

Usage
======

Once set up, the full usage readme is at public_html/readme.php.

The short version:

* SSH in with user adom and password adom, and follow prompts to create a new 
user.

* Then SSH in as the new user.

Administration
===============

* To edit the banner on the top of the main menu, edit etc/motd in the server 
  homedir. Bash color codes can be used (but double-escape any \s).

* To add support for new 1.2.0 prereleases, just edit etc/config and change
  MAX_PRE. This will 'just work', but it will be vanilla with none of the 
  servers' binary modifications.  Sage will need to be recompiled once it 
  supports the new version, and the same for the binary wrapper (INSTALL 
  section II.x.b).

* Setting up a web server and domain is out of scope for this document, but
  is recommended to do. See section III.xix of the INSTALL for more.

* New ADOM versions require small edits to the binary to enable sharing of
  HISCORES and avoid clobbering other versions. See section II.x of the INSTALL
  for more details.

* New versions of Sage may be released.  To use them, see the install directions
  in section II.x.c in the INSTALL file.

* When a user signs up, he is asked for his e-mail address. The address is 
  stored in the user's GECOS field in /etc/passwd. If he needs a password reset,
  the admin can extract his e-mail address with command:

  getent passwd "USERNAMEHERE" | cut -d ':' -f 5 | cut -d ',' -f 2

  then set a new password for him and e-mail it to him. If he didn't set an 
  email (the previous command returns nothing), it's up to the admin what to do.

Changelog
==========

See CHANGELOG file.

Contributing
=============

Submit a pull request on GitHub and I'll figure it out.

Contact
========

http://www.adom.de/forums/showthread.php/493

Alucard on #adom on irc.FreeNode.net  
Other netizens there can also answer usage questions.

Alucard <saturos@gmail.com>

Authors
========
Christopher Henning aka Alucard(Zero), author of the rest  
Jaakko Salo aka jaakkos, author of the vast majority of 1.0  
Sampsa "Tuplanolla" Kiiskinen

ADOM is Copyright (c) 1994-2013, Thomas Biskup  
The ADOM Guidebook is Copyright (c) 2010 Andy Williams  
Passwd.py is Copyright (c) 2001-2006, Chris Gonnerman  
Termrec is Copyright (c) 2008, Adam Borowski

