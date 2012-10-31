
class QListViewItemLdap(QListViewItem):
  def __init__(self, *args):
    QListViewItem.__init__(self, *args)

  def key(self, col, asc):
    if col == 0:
        return self.text(col).latin1().replace("=_", "=000")
    else:
        return self.text(col)

class QListViewItemLdapAttr(QListViewItem):
  def __init__(self, *args):
    QListViewItem.__init__(self, *args)

  def key(self, col, asc):
    if col == 0:
        text = self.text(col).latin1()
        if text.lower() == 'objectclass':
            return "00000%s" % text
        else:
            return self.text(col)
    else:
        return self.text(col)


class QListViewItemLdapAttrColored(QListViewItem):
  def __init__(self, *args):
    QListViewItem.__init__(self, *args)

  def paintCell(self, p, cg, column, width, align):
    g = QColorGroup(cg)
    g.setColor( QColorGroup.Base, QColor("grey89"))
#   g.setColor( QColorGroup.Text, QColor("darkgreen"))
    QListViewItem.paintCell(self, p, g, column, width, align)

  def key(self, col, asc):
    if col == 0:
        text = self.text(col).latin1()
        if text.lower() == 'objectclass':
            return "00000%s" % text
        else:
            return self.text(col)
    else:
        return self.text(col)

from time import strptime, strftime
import locale

class QListViewItemImapPartition(QListViewItem):
  def __init__(self, *args):
    QListViewItem.__init__(self, *args)

  def key(self, col, asc):
    if col > 1:
        textList = self.text(col).latin1().strip(" ").split(" ")
        if len(textList) > 1:
            lang = locale.setlocale(locale.LC_ALL)
            locale.setlocale(locale.LC_ALL, 'C')
            tmp = strftime("%Y-%m-%d %H:%M:%S", strptime("%s %s" % (textList[0], textList[1]), "%d-%b-%Y %H:%M:%S"))
            locale.setlocale(locale.LC_ALL, lang)
            return tmp
        elif textList[0].find("%") != -1:
            return "0" * (20 - len(self.text(col).ascii())) + self.text(col).ascii()[:-1]
        else:
            return "0" * (20 - len(self.text(col).ascii())) + self.text(col).ascii()
    return self.text(col)

class QListViewItemImap(QListViewItem):
  def __init__(self, *args):
    QListViewItem.__init__(self, *args)

  def key(self, col, asc):
    if col == 0:
        text = self.text(col).latin1()
        if text.find("@") == -1:
            return "0%s" % text
        else:
            textList = text.split("@")
            return "%s@%s" % (textList[1], textList[0])
    else:
        return self.text(col)

class QListViewItemQueue(QListViewItem):
  def __init__(self, *args):
    QListViewItem.__init__(self, *args)

  def key(self, col, asc):
    if col == 0:
        text = self.text(col).latin1()
        if text.find("@") == -1:
            return "0%s" % text
        else:
            textList = text.split("@")
            return "%s@%s" % (textList[1], textList[0])
    elif col == 1:
        countText = self.text(col).latin1()
        if countText is not None and countText.find("/") != -1:
            countList = countText.split("/")
            return "%s%s/%s%s" % ("0" * (20 - len(countList[1])), countList[1], "0" * (20 - len(countList[0])), countList[0])
        else:
            return self.text(col)
    else:
        return self.text(col)


