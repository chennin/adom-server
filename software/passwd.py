# Software License
# 
# Copyright (c) 2001, Chris Gonnerman
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions
# are met:
# 
# Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer. 
# 
# Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution. 
# 
# Neither the name of the author nor the names of any contributors
# may be used to endorse or promote products derived from this software
# without specific prior written permission. 
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

"""Administrator's Toolbox -- Password File Module

This module makes handling password and group files easier.  They
are referred to here as "entry files," which are flat files 
consisting of colon (:) separated fields.  The first field is the
key field and all others are data.

The load(), loadpw(), and loadgrp() functions are used
to read these files.  load() returns a mapping (OrdDict) of 
Ent objects, which have the following members:

    .seq()    Returns the list of data values

loadpw() loads a password file (/etc/passwd by default) and
returns an OrdDict of PWEnt objects, which have the following 
members:

    .passwd   Encrypted Password
    .uid      Numeric User ID
    .gid      Numeric Group ID
    .comment  Comment (GECOS field)
    .home     Home Directory
    .shell    Command Shell
    .seq()    Returns the list of data values:
                [ passwd, uid, gid, comment, home, shell ]

loadgrp() loads a group file (/etc/group by default) and returns
an OrdDict of GrpEnt objects, which have the following members:

    .passwd   Encrypted Password
    .gid      Numeric Group ID
    .users    List of Member Users
    .seq()    Returns the list of data values:
                [ passwd, gid, users ]
              (users is converted to a comma-separated string)

save() saves the given mapping of Ent-like objects to the 
given file.  The .seq() of each entry is called, the result joined 
with colons, and then written to the file as a line.  save() can 
handle any string-keyed mapping of objects which have a proper 
.seq() method.

genpw() generates a new encrypted password (with a random salt)
for use in creating new password entries.

ckpw() checks the given plaintext password against the given 
cyphertext password; returns true on a match, false otherwise.

"""

__version__ = "1.1"

import string, UserDict, random

try:
    from fcrypt import crypt
except ImportError:
    from crypt import crypt

_saltchrs = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
          + "abcdefghijklmnopqrstuvwxyz" \
          + "0123456789" \
          + "./"

class Ent:
    "Generic Entry object, used by load"
    def __init__(self, lst):
        self.__lst = lst
    def seq(self):
        return self.__lst

class PWEnt:
    "Password Entry object, used by loadpw"
    def __init__(self, lst = []):
        lst = lst + ([ None ] * 6)
        self.passwd = lst[0]
        try:
            self.uid = int(lst[1])
        except:
            self.uid = 0
        try:
            self.gid = int(lst[2])
        except:
            self.gid = 0
        self.comment = lst[3]
        self.home = lst[4]
        self.shell = lst[5]
    def seq(self):
        return [ self.passwd, self.uid, self.gid, 
            self.comment, self.home, self.shell ]

class GrpEnt:
    "Group Entry object, used by loadgrp"
    def __init__(self, lst):
        lst = lst + ([ None ] * 6)
        self.passwd = lst[0]
        self.gid = int(lst[1])
        self.users = string.split(lst[2], ",")
        self.users = map(string.strip, self.users)
    def seq(self):
        return [ self.passwd, self.gid, string.join(self.users, ",") ]

class OrdDict(UserDict.UserDict):
    "Ordered Dictionary, keeps keys() in natural order"
    def __init__(self):
        UserDict.UserDict.__init__(self)
        self.ordkeys = []
    def __setitem__(self, key, value):
        if not self.data.has_key(key):
            self.ordkeys.append(key)
        UserDict.UserDict.__setitem__(self, key, value)
    def __delitem__(self, key):
        if self.data.has_key(key):
            self.ordkeys = filter(lambda x, k = key: x != k, self.ordkeys)
        UserDict.UserDict.__delitem__(self, key)
    def keys(self):
        return self.ordkeys[:]

def _load(fname, cls):
    "_load(), internal use entry file loader"
    d = OrdDict()
    fp = open(fname, "r")
    for l in fp.readlines():
        l = l[:-1]
        lst = string.split(l, ":")
        d[lst[0]] = cls(lst[1:])
    fp.close()
    return d

def load(fname):
    "load(), load a generic entry file"
    return _load(fname, Ent)

def loadpw(fname = "/etc/passwd"):
    "loadpw(), load a passwd file"
    return _load(fname, PWEnt)

def loadgrp(fname = "/etc/group"):
    "loadgrp(), load a group file"
    return _load(fname, GrpEnt)

def save(fname, d):
    "save(), saves any kind of entry dictionary"
    fp = open(fname, "w")
    for k in d.keys():
        lst = map(str, [ k ] + d[k].seq())
        fp.write(string.join(lst, ":") + "\n")
    fp.close()

def genpw(pwd):
    pwd = pwd[:8]
    salt = random.choice(_saltchrs) + random.choice(_saltchrs)
    return crypt(pwd, salt)

def ckpw(pwd, crpwd):
    pwd = pwd[:8]
    if crpwd == crypt(pwd, crpwd[:2]):
        return 1
    return 0

if __name__ == "__main__":

    import sys

    # installer

    if len(sys.argv) > 1:

        from distutils.core import setup, Extension

        setup(name="passwd",
            version=__version__,
            description="Password File Management",
            long_description="Password File Management",
            author="Chris Gonnerman",
            author_email="chris.gonnerman@newcenturycomputers.net",
            url="http://newcenturycomputers.net/projects/passwd.html",
            py_modules=["passwd"]
        )

        sys.exit(0)

    # testrig

    crpwd = genpw('george')
    print "genpw('george') =", crpwd
    print "ckpw('george', '%s') =" % crpwd, ckpw('george', crpwd)

    p = loadpw()
    save("pwtest", p)

    g = loadgrp()
    save("grptest", g)

# end of file.
