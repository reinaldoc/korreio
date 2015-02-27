
from PyQt4 import QtGui,QtCore

def centered_position(geometry):
	x = (QtGui.QApplication.desktop().width() / 2) - (geometry.width() / 2)
	y = (QtGui.QApplication.desktop().height() / 2) - (geometry.height() / 2)
	return QtCore.QRect(x, y, geometry.width(), geometry.height())
