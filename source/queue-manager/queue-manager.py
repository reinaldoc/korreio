#!/usr/bin/python
# -*- coding: utf-8 -*-
# Queue Manager
# Copyright (c) 2015 - Reinaldo Gil Lima de Carvalho <reinaldoc@gmail.com>
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


import sys
from PyQt4 import QtGui
from controller.MainBC import MainBC

if __name__ == "__main__":
	app = QtGui.QApplication(sys.argv)

	# Run Main Window
	mainBC = MainBC()
	mainBC.show()
	sys.exit(app.exec_())
