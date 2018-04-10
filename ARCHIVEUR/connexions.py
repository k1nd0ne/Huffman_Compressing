#!/usr/bin/env python3
import os,re
connex = {}
command = os.popen("last")

for char in command :
    regexp = re.search("(^\w+) .* (... ... ..) *[^\(]*\((.)\+(..):(..)\)",char)
    if (regexp) :
        i = int(regexp.group(3))
        j = int(regexp.group(4))
        print(regexp.group(1), "c'est connecté le", regexp.group(2),  "pendant ",i*60+j," minute")
        login = regexp.group(1)
        if login in connex :
            connex[login]
        else :
            connex[login] = {}
        date = regexp.group(2)
        tps = i*60+j
        if date in connex :
            connex[login][date]
        else :
            connex[login][date] = [1,tps]
        print(connex)

#^[^ ]*
