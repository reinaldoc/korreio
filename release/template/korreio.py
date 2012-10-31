#!/usr/bin/python
# -*- coding: utf-8 -*-
# Korreio v0.1-20090512
# Copyright (c) 2007-2009 - Reinaldo de Carvalho <reinaldoc@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.

#
# Korreio Configuration Path
#

korreioConfigPath = "${workdir}/inc/korreio.conf"

#
# End
#

import sys, os

if __name__ != "__main__":
    print "ERROR: this is not a class."
    sys.exit(1)

workdir = os.path.dirname(sys.argv[0])
if not workdir:
    workdir = '.'

if len(sys.argv) > 1:
    korreioConfigPath = sys.argv[1]
else:
    korreioConfigPath = korreioConfigPath.replace("${workdir}", workdir)

#
# Load configuration file
#

try:
    f = open(korreioConfigPath, 'r')
    korreioConfig = f.read()
    f.close()
except IOError, e:
    print "ERROR: can't read configuration file '%s' (%s)." % (korreioConfigPath, e)
    sys.exit(1)

#
# Build a dictionary
#

korreioConfigLines = korreioConfig.split("\n")
korreioConfigLines.pop()
korreioConfigDict = {}
for line in korreioConfigLines:
    if line.find("=") != -1:
        key = line.split("=", 1)
        korreioConfigDict[key[0]] = key[1].replace("${workdir}", workdir)

#
# Check configuration file
#

try:
    korreioConfigDict['path.lib']
    korreioConfigDict['path.lang']
    korreioConfigDict['path.icons']
    korreioConfigDict['path.sieveConf']
    korreioConfigDict['path.sieveDir']
except Exception, e:
    print "ERROR: can't find configuration for %s" % e

sys.path.append(korreioConfigDict.get('path.lib'))

#
# Load modules
#

try:
    from qt import *
    from korreiolib import *
except ImportError, e:
    print "ERROR:", e
    sys.exit(1)

#
# Load Translation
#

app = QApplication(sys.argv)

try:
    LANG = "%s/korreio-%s.qm" % (korreioConfigDict.get('path.lang'), unicode(QLocale.system().name().latin1(), 'iso-8859-1').encode('utf-8'))
    if os.access(LANG, os.F_OK):
        translator = QTranslator()
        translator.load(unicode(LANG, 'utf-8').encode('iso-8859-1'))
        app.installTranslator(translator)
except:
    print "WARNING: translation unvailable (korreio-%s.qm)." % QLocale.system().name()


f = dKorreio(korreioConfigDict=korreioConfigDict)
f.show()
app.setMainWidget(f)
app.exec_loop()
