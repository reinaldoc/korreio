
class QTextEditSieveScript(QTextEdit):
    def __init__(self, parent, name):
        self.config = ""
        self.sievePopup = {}
        apply(QTextEdit.__init__, (self, parent, name))

    def _load_config(self):
        if not self.config:
            return

        try:
            f = open(self.config, 'r')
            sieveConfig = f.read()
            f.close()
        except IOError, e:
            print "ERROR: can't read sieve helper '%s' (%s)." % (self.config, e)

        re_sieveMenu = re.compile(r'(\[Menu\]\nid ?= ?(.*)\nparent ?= ?(.*)\ndisplay name ?= ?(.*)\n)')
        re_sieveItem = re.compile(r'(\[Item\]\nid ?= ?(.*)\nparent ?= ?(.*)\ndisplay name ?= ?(.*)\nstrip script ?= ?(.*)\nsieve script ?= ?<< ?EOF\n((.*\n)*?)EOF\n)')

        self.sieveMenu = {}
        match = re_sieveMenu.findall(sieveConfig)
        for menu in match:
            self.sieveMenu[int(menu[1])] = { "parent": int(menu[2]), "display name": menu[3] }

        self.sieveItem = {}
        match = re_sieveItem.findall(sieveConfig)
        for item in match:
            self.sieveItem[int(item[1])] = { "parent": int(item[2]), "display name": item[3], "strip script": item[4], "sieve script": item[5]}

    def _load_popup(self):

        self.sievePopup = {}
        self.sievePopup[-1] = QPopupMenu(self)

        ids = self.sieveMenu.keys()
        ids.sort()

        for id in ids:
            info = self.sieveMenu.get(id)
            self.sievePopup[id] = QPopupMenu(self)
            self.sievePopup[info.get("parent")].insertItem(info.get("display name"), self.sievePopup.get(id), 0)
            self.connect(self.sievePopup.get(id), SIGNAL('activated(int)'), self.sieve_menu_clicked)

        ids = self.sieveItem.keys()
        ids.sort()
        for id in ids:
            info = self.sieveItem.get(id)
            self.sievePopup[info.get("parent")].insertItem(info.get("display name"), id)

    def createPopupMenu(self, *args):
        if not self.sievePopup:
            self._load_config()
        self._load_popup()
        return self.sievePopup[-1]

    def sieve_menu_clicked(self, menuItem):
        script = self.sieveItem.get(menuItem).get("sieve script")
        if self.sieveItem.get(menuItem).get("strip script") == "yes":
            script = script.strip()
        self.insert(script, True, True)
        return True

    def setConfig(self, config):
        self.config = config
