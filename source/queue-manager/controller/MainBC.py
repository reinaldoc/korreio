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


import sys, os, re
from PyQt4 import QtCore, QtGui
from view.MainUI import Ui_MainUI
from util import Window

class MainBC(QtGui.QMainWindow):
	def __init__(self, parent=None):
		QtGui.QWidget.__init__(self, parent)

		# GUI
		self.ui = Ui_MainUI()
		self.ui.setupUi(self)

		# GUI position
		self.setGeometry(Window.centered_position(self.geometry()))

		# DAO
		#self.configDAO = ConfigDAO()

		# Controllers
		#self.propertiesBC = PropertiesBC()

		# Signals
		QtCore.QObject.connect(self.ui.pushButton,   QtCore.SIGNAL("clicked()"), self.search)

	# Methods for SIGNALS
	def search(self):
		print("xxx")
