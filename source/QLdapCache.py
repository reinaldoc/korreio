import re

class QLdapCacheError(Exception): pass

class QLdapCache:
    """
    Dict = {
             unicode_dn1: { QListViewItem_1 },
             unicode_dn2: { QListViewItem_2 }
           }
    """

    def __init__(self, listView):
        import re
        self.ListView = listView
        self.LdapCache = {}
        self.icons = {}

    def _sanitize(self, value):
        if type(value) != type(u'') or len(value) == 0:
            raise QLdapCacheError, "Invalid codification"

    def init(self, baseList=[]):
        """
            Initialize QLdapCache dictionary
        """
        self.LdapCache = {}
        for base in baseList:
            self._sanitize(base)
        self.baseList = baseList
    
    def setIcons(self, icons):
        self.icons = icons

    def _getIcon(self, dn):
        dn = dn.lower()
        if re.match("^uid=", dn):
            icon = "user.png"
        elif re.match("^ou?=", dn):
            icon = "folder.png"
        else:
            icon = "ldap.png"
        return self.icons.get(icon)

    def _isBase(self, dn):
        for base in self.baseList:
            if base.lower() == dn.lower():
                return True
        return False

    def _loadRoot(self, dn):
        self.LdapCache[dn.lower()] = QListViewItemLdap(self.ListView)
        self.LdapCache[dn.lower()].setOpen(True)
        self.LdapCache[dn.lower()].setText(0, dn)
        self.LdapCache[dn.lower()].setPixmap(0, self.icons.get("folder.png"))

    def _load(self, dn):
        dnList = dn.split(u",")
        dnParent = u",".join(dnList[1:])
        if not self.LdapCache.get(dnParent.lower()):
            if self._isBase(dn):
                self._loadRoot(dn)
                return
            if len(dnList) > 2:
                self._load(dnParent)
            else:
                self._loadRoot(dnParent)
        self.LdapCache[dn.lower()] = QListViewItemLdap(self.LdapCache.get(dnParent.lower()))
        self.LdapCache[dn.lower()].setText(0, dnList[0])
        self.LdapCache[dn.lower()].setPixmap(0, self._getIcon(dn))

    def _fix(self, dn):
        """
            Fix caracter case for auto-generate nodes
        """
        if self._isBase(dn):
            self.LdapCache.get(dn.lower()).setText(0, dn)
        else:
            self.LdapCache.get(dn.lower()).setText(0, dn.split(u",")[0])

    def load(self, dn):
        """
            Create QListViewItem and parents nodes
        """
        self._sanitize(dn)
        if not self.LdapCache.get(dn.lower()):
            if self._isBase(dn):
                self._loadRoot(dn)
            else:
                self._load(dn)
        self._fix(dn)

    def add(self, dn):
        """
            Create QListViewItem
        """
        dnList = dn.split(u",")
        if self._isBase(dn) or len(dnList) == 1:
            self.LdapCache[dn.lower()] = QListViewItemLdap(self.ListView)
            self.LdapCache[dn.lower()].setText(0, dn)
        else:
            itemParent = self.LdapCache.get(",".join(dnList[1:]).lower())
            itemParent.setOpen(True)
            self.LdapCache[dn.lower()] = QListViewItemLdap(itemParent)
            self.LdapCache[dn.lower()].setText(0, dnList[0])
        self.LdapCache[dn.lower()].setPixmap(0, self._getIcon(dn))
        self.ListView.ensureItemVisible(self.LdapCache.get(dn.lower()))

    def get(self, dn):
        """
            Get QListViewItem by DN
        """
        self._sanitize(dn)
        return self.LdapCache.get(dn)

    def _rename(self, dn, newDn):
        if dn != newDn:
            self.LdapCache[newDn] = self.LdapCache.get(dn)
            del self.LdapCache[dn]

    def rename(self, dn, newDn):
        """
            Rename DN key
        """
        self._sanitize(dn)
        self._sanitize(newDn)
        dnParent = dn.split(u",", 1)[1]
        newDnParent = newDn.split(u",", 1)[1]
        if dnParent != newDnParent:
            self.LdapCache.get(dnParent).takeItem(self.LdapCache.get(dn))
            self.LdapCache.get(newDnParent).insertItem(self.LdapCache.get(dn))

        self._rename(dn, newDn)
        
        if dn.lower() != newDn.lower():
            for entry in self.LdapCache.keys():
                self._rename(entry, re.sub(u",%s$" % dn, u",%s" % newDn, entry))
