#!/usr/bin/env python3

import sys
import os
import io
import re
#----------------------------------ARCHIVE--------------------------------------------
def archive(rep):
    liste=os.listdir(rep)
    for fic in liste:
        if os.path.isdir(rep+"/"+fic) and not os.path.islink(rep+"/"+fic):
             archive(rep+"/"+fic)
        else:
            chem = rep+"/"+fic
            print("Compression de",fic,"--->",fic+".huff")
            compress = "./compress"+" "+chem+" "+chem+".huff"
            os.system(compress)
            rm = "rm "+chem
            os.system(rm)
#-----------------------------------DESARCHIVE-----------------------------------------
def unarchive(rep):
        liste = os.listdir(rep)
        for fic in liste:
            if os.path.isdir(rep+"/"+fic) and not os.path.islink(rep+"/"+fic):
                unarchive(rep+"/"+fic)
            else:
                chem = rep+"/"+fic
                print("Decompression de",fic)

                caca = re.search('(.*)\.huff',chem)
                if caca:
                    nom = caca.group(1)
                    mv = "mv "+chem+" "+nom
                    os.system(mv)

                    decompress = "./decompress "+nom+" "+nom
                    os.system(decompress)

#-------------------------------------------MAIN---------------------------------------


if len(sys.argv)!=3:
    print("Usage : ",sys.argv[0]," -a OU BIEN -d [repertoire_cible]")
    sys.exit(1)
res=re.search('^(-a|-d)$',sys.argv[1])
if not res:
    print("Usage : ",sys.argv[0]," -a OU -d [repertoire_cible]")
    sys.exit(1)
if(re.search('a',sys.argv[1])):
    toto = re.search('(.*)\/$',sys.argv[2])
    if toto:
        titi = toto.group(1)
        cp = "cp -r "+titi+" "+titi+".arc"
        os.system(cp)
        archive(titi+".arc")
        #os.system("clear")
        sys.exit(0)
    else:
        cp = "cp -r "+sys.argv[2]+" "+sys.argv[2]+".arc"
        os.system(cp)
        archive(sys.argv[2]+".arc")
        #os.system("clear")
        sys.exit(0)
if(re.search('d',sys.argv[1])):
    toto = re.search('(.*)\/$',sys.argv[2])
    if toto:
        titi = toto.group(1)
        caca = re.search('(.*)\.arc',titi)
        if caca:
            nom = caca.group(1)
            mv = "mv "+titi+" "+nom
            rm = "rm -rf "+nom
            os.system(rm)
            os.system(mv)
            unarchive(nom)
            #os.system("clear")

    else:
        caca = re.search('(.*)\.arc',sys.argv[2])
        if caca:
            nom = caca.group(1)
            mv = "mv "+sys.argv[2]+" "+nom
            rm = "rm -rf "+nom
            os.system(rm)
            os.system(mv)
            unarchive(nom)
            #os.system("clear")
