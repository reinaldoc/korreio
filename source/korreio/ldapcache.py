
class LdapCacheError(Exception): pass

class LdapCache:
    """
    Dict = {
             unicode_dn1: { utf8_attr1: [utf8_values], utf8_attr2: [utf8_values] },
             unicode_dn2: { utf8_attr1: [utf8_values], utf8_attr2: [utf8_values] }
           }
    """

    def __init__(self):
        self.ldap_cache = {}
        import re

    def _sanitize(self, value):
        if type(value) != type(u'') or len(value) == 0:
            raise LdapCacheError, "Invalid codification"

    def load(self, dnDict):
        if dnDict is None:
            self.ldap_cache = {}
        else:
            self.ldap_cache = dnDict

    def get(self, dn):
        self._sanitize(dn)
        return self.ldap_cache.get(dn)

    def set(self, dn, attrs):
        self._sanitize(dn)
        self.ldap_cache[dn] = attrs

    def unset(self, dn):
        self._sanitize(dn)
        del self.ldap_cache[dn]
    
    def add(self, dn, modlistAdd):
        self._sanitize(dn)
        self.ldap_cache[dn] = {}
        for tuple in modlistAdd:
            self.ldap_cache[dn][tuple[0]] = tuple[1]

    def modify(self, dn, modlistModify):
        self._sanitize(dn)
        for tuple in modlistModify:
            if tuple[2] is None:
                del self.ldap_cache[dn][tuple[1]]
            else:
                self.ldap_cache[dn][tuple[1]] = tuple[2]

    def show(self):
        print self.ldap_cache

    def isEmpty(self):
        if self.ldap_cache == {}:
            return True
        else:
            return False
    
    def keys(self):
        return self.ldap_cache.keys()
    
    def _rename(self, dn, newDn):
        if dn != newDn:
            self.ldap_cache[newDn] = self.ldap_cache.get(dn)
            del self.ldap_cache[dn]

    def rename(self, dn, newDn):
        self._sanitize(dn)
        self._sanitize(newDn)
        self._rename(dn, newDn)
        if dn.lower() != newDn.lower():
            for entry in self.ldap_cache.keys():
                self._rename(entry, re.sub(u",%s$" % dn, u",%s" % newDn, entry))

    def removeAttr(self, dn, rdn):
        self._sanitize(dn)
        rdnList = rdn.split("=")
        i = 0
        for attr in self.ldap_cache[dn][rdnList[0]]:
            if rdnList[1].lower() == attr.lower():
                del self.ldap_cache[dn][rdnList[0]][i]
                break
            i += 1
        if self.ldap_cache[dn][rdnList[0]] == []:
            del self.ldap_cache[dn][rdnList[0]]

    def addAttr(self, dn, rdn):
        self._sanitize(dn)
        rdnList = rdn.split("=")
        if self.ldap_cache[dn].get(rdnList[0]):
             self.ldap_cache[dn][rdnList[0]].append(rdnList[1])
        else:
             self.ldap_cache[dn][rdnList[0]] = [rdnList[1]]
