# Copyright (c) 2007-2009 -  Reinaldo de Carvalho <reinaldoc@gmail.com>
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

void dKorreio::init() {

#
# Declare basic functions
#

global utf2uni, Q2uni, Q2utf, QExpandChilds, QSelectFile

def utf2uni(string):
    return unicode(string, 'utf-8')

def Q2uni(QString2uni):
    if type(QString2uni) == type(QString()):
        str = unicode(QString2uni.latin1(), 'iso-8859-1')
    else:
        try:
            str = unicode(QString2uni, 'utf-8')
        except:
            str = u""
    return str

def Q2utf(QString2utf):
    if type(QString2utf) == type(QString()):
        str = unicode(QString2utf.latin1(), 'iso-8859-1')
    else:
        str = u""
    return str.encode('utf-8')

def QExpandChilds(item):
    if item is not None:
        item.setOpen(True)
        item = item.firstChild()
        while item is not None:
            QExpandChilds(item)
            item = item.nextSibling()

def QSelectFile(titleText, mode):
    dlg = QFileDialog(os.getenv("HOME"))
    dlg.setCaption(titleText)
    dlg.setMode(mode)
    dlg.exec_loop()
    file = dlg.selectedFile()
    if file == "/":
        file = ""
    return file

#
# Declare variables
#

self.CONSOLE_OK = 0
self.CONSOLE_ERR = 1
self.CONSOLE_WARN = 2
self.CONSOLE_INFO = 3
self.module_failed = []
self.ldap_cache = LdapCache()
self.ldap_items = QLdapCache(self.lvLdap)
self.imap_cache_items = {}
self.inet_cache = {}
self.confDict = {}
self.ldap_dn_rename = None
self.ldap_attr_modify = None
self.ldap_add_or_modify = True
self.imap_mailbox_rename = None
self.sieve_scripts = {}
self.queue_search_auto = False
self.queue_search_mode = 0

#
# Set Icons
#

self.icons = {}
for icon in QDir(self.korreioConfigDict.get("path.icons"), "*.png").entryList():
    self.icons[Q2utf(icon)] = QPixmap("%s/%s" % (self.korreioConfigDict.get("path.icons"), Q2utf(icon)))
self.ldap_items.setIcons(self.icons)

#
# LDAP Icons
#

self.pLdapSearch.setIconSet(QIconSet(self.icons.get("find.png")))
self.pLdapDelete.setIconSet(QIconSet(self.icons.get("no.png")))
self.pLdapAddAttr.setIconSet(QIconSet(self.icons.get("add.png")))
self.pLdapDeleteAttr.setIconSet(QIconSet(self.icons.get("del.png")))
self.pLdapModify.setIconSet(QIconSet(self.icons.get("ok.png")))
self.pLdapAddDhcp.setIconSet(QIconSet(self.icons.get("ok.png")))
self.pLdapFormBack.setIconSet(QIconSet(self.icons.get("previous.png")))
self.pLdapFormNext.setIconSet(QIconSet(self.icons.get("next.png")))
self.pLdapAddUser.setIconSet(QIconSet(self.icons.get("ok.png")))
self.pLdapAddOu.setIconSet(QIconSet(self.icons.get("ok.png")))
self.pLdapPasswd.setIconSet(QIconSet(self.icons.get("ok.png")))
self.pLdapSambaPopulate.setIconSet(QIconSet(self.icons.get("ok.png")))
self.pLdapDhcpPopulate.setIconSet(QIconSet(self.icons.get("ok.png")))
self.pLdapAddGroup.setIconSet(QIconSet(self.icons.get("ok.png")))
self.pLdapImportCSV.setIconSet(QIconSet(self.icons.get("ok.png")))
self.pLdapImportLdif.setIconSet(QIconSet(self.icons.get("ok.png")))

#
# IMAP Icons
#

self.pImapSearch.setIconSet(QIconSet(self.icons.get("find.png")))
self.pCyrAdd.setIconSet(QIconSet(self.icons.get("add.png")))
self.pCyrDelete.setIconSet(QIconSet(self.icons.get("del.png")))
self.pImapAclAdd.setIconSet(QIconSet(self.icons.get("add.png")))
self.pImapAclDel.setIconSet(QIconSet(self.icons.get("del.png")))
self.pCyrReconstruct.setIconSet(QIconSet(self.icons.get("wizard.png")))
self.pImapQuota.setIconSet(QIconSet(self.icons.get("ok.png")))
self.pImapAnnotation.setIconSet(QIconSet(self.icons.get("ok.png")))
self.pImapImport.setIconSet(QIconSet(self.icons.get("ok.png")))
self.pImapExport.setIconSet(QIconSet(self.icons.get("ok.png")))
self.pImapImportfindFile.setIconSet(QIconSet(self.icons.get("filefind.png")))
self.pImapExportfindFile.setIconSet(QIconSet(self.icons.get("filefind.png")))

self.pImapPartitionSearch.setIconSet(QIconSet(self.icons.get("find.png")))

self.pSieveSearch.setIconSet(QIconSet(self.icons.get("find.png")))
self.pSieveScriptActive.setIconSet(QIconSet(self.icons.get("add.png")))
self.pSieveScriptDisable.setIconSet(QIconSet(self.icons.get("del.png")))
self.pSieveScriptRemove.setIconSet(QIconSet(self.icons.get("no.png")))

#
# SSH Icons
#

self.pQueueSearch.setIconSet(QIconSet(self.icons.get("find.png")))
self.pQueueLoad.setIconSet(QIconSet(self.icons.get("reload.png")))

self.pSaveConfig.setIconSet(QIconSet(self.icons.get("filesave.png")))

#
# Load Python modules
#

try:
    global sys, re, datetime, os
    import sys, re, datetime, os.path

    reload(sys)
    sys.setdefaultencoding("utf-8")

    global  sha, md5, b2a_base64, choice, letters, digits
    import sha, md5
    from binascii import b2a_base64
    from random import choice
    from string import letters, digits
except:
    print "ERROR: python core modules can not be found."
    sys.exit()

self.module_failed = []

try:
    global crypt
    import crypt
except:
    self.module_failed.append("crypt")

try:
    global ldap, modlist
    import ldap, ldap.modlist as modlist
except:
    self.wsKorreio.widget(1).setEnabled(False)
    self.pGetBaseDN.setEnabled(False)
    self.module_failed.append("ldap")

try:
    global smbpasswd
    import smbpasswd
except:
    self.cbLdapSambaPassword.setEnabled(False)
    self.pLdapSambaPopulate.setEnabled(False)
    self.cLdapFormSamba.setEnabled(False)
    self.module_failed.append("smbpasswd")

try:
    global cyruslib, sievelib
    import cyruslib, sievelib
except:
    self.wsKorreio.widget(2).setEnabled(False)
    self.wsKorreio.widget(3).setEnabled(False)
    self.wsKorreio.widget(4).setEnabled(False)
    self.module_failed.append("cyrus")

try:
    global pexpect, pxssh
    import pexpect, pxssh
except:
    self.wsKorreio.widget(5).setEnabled(False)
    self.wsKorreio.widget(6).setEnabled(False)
    self.module_failed.append("pexpect")

if self.module_failed:
    msg = ""
    for mod in self.module_failed:
        msg = "%s%s\n" % (msg, "    - python-%s" % mod)
    QMessageBox.warning(None, self.__tr("Can't load libraries"), self.__tr("This libraries can not be found:\n%1").arg(msg))


#
# Servers Menu
#

self.lvServersImapMenu = QPopupMenu(self)
self.lvServersLdapMenu = QPopupMenu(self)
self.lvServersSSHMenu = QPopupMenu(self)

self.connect(self.lvServersImapMenu, SIGNAL('activated(int)'), self.korreio_servers_menu_set_imap)
self.connect(self.lvServersLdapMenu, SIGNAL('activated(int)'), self.korreio_servers_menu_set_ldap)
self.connect(self.lvServersSSHMenu, SIGNAL('activated(int)'), self.korreio_servers_menu_set_ssh)

self.lvServersMenu = QPopupMenu(self)
self.lvServersMenu.insertItem(QIconSet(self.icons.get("server.png")), '&IMAP', self.lvServersImapMenu, 0)
self.lvServersMenu.insertItem(QIconSet(self.icons.get("server.png")), '&LDAP', self.lvServersLdapMenu, 1)
self.lvServersMenu.insertItem(QIconSet(self.icons.get("server.png")), '&SSH', self.lvServersSSHMenu, 2)
self.lvServersMenu.insertSeparator()

#
# Korreio Menu
#

self.mainMenuKorreio = QPopupMenu(self)
self.mainMenuKorreio.insertItem(QIconSet(self.icons.get("configure.png")), self.__tr('&Configuration'), 0)
self.mainMenuKorreio.insertItem(QIconSet(self.icons.get("log.png")), self.__tr('&Log'), 1)
self.mainMenuKorreio.insertSeparator()
self.mainMenuKorreio.insertItem(QIconSet(self.icons.get("server.png")), self.__tr('Select &Server'), self.lvServersMenu, 2)
self.mainMenuKorreio.insertSeparator()
self.mainMenuKorreio.insertItem(QIconSet(self.icons.get("exit.png")), self.__tr('&Exit'), 3)

self.mainMenuModules = QPopupMenu(self)
self.mainMenuModules.insertItem(QIconSet(self.icons.get("ldap.png")), self.__tr('Open&LDAP Manager'), 10)
self.mainMenuModules.insertItem(QIconSet(self.icons.get("mail_new.png")), self.__tr('&Cyrus-Imap Manager'), 11)
self.mainMenuModules.insertItem(QIconSet(self.icons.get("mail_post_to.png")), self.__tr('Cyrus-Imap &Partitions'), 12)
self.mainMenuModules.insertItem(QIconSet(self.icons.get("sieve.png")), self.__tr('Cyrus-&Sieve Manager'), 13)
self.mainMenuModules.insertItem(QIconSet(self.icons.get("queue.png")), self.__tr('Postfix &Queue Manager'), 14)
self.mainMenuModules.insertSeparator()
self.mainMenuModules.insertItem(QIconSet(self.icons.get("folder_crystal.png")), self.__tr('&Tools'), 15)

self.mainMenuHelp = QPopupMenu(self)
self.mainMenuHelp.insertItem(QIconSet(self.icons.get("info.png")), self.__tr('&About...'), 20)

self.menuBar.insertItem(self.__tr('&Korreio'), self.mainMenuKorreio, 0)
self.menuBar.insertItem(self.__tr('&Modules'), self.mainMenuModules, 1)
self.menuBar.insertItem(self.__tr('&Help'), self.mainMenuHelp, 2)
self.menuBar.setFrameShape(QMenuBar.Panel)
self.menuBar.setFrameShadow(QMenuBar.Raised)

self.connect(self.mainMenuKorreio, SIGNAL('activated(int)'), self.korreio_menu_clicked)
self.connect(self.mainMenuModules, SIGNAL('activated(int)'), self.korreio_menu_clicked)
self.connect(self.mainMenuHelp, SIGNAL('activated(int)'), self.korreio_menu_clicked)

#
# LDAP Menu
#

self.lvLdapMenu   = QPopupMenu(self)
self.lvLdapMenu1 = QPopupMenu(self)
self.lvLdapMenu2 = QPopupMenu(self)

self.lvLdapMenu.insertItem(QIconSet(self.icons.get("add_user.png")), self.__tr('&New'), self.lvLdapMenu1, 10)

self.lvLdapMenu1.insertItem(QIconSet(self.icons.get("edit_user.png")), self.__tr('&Entry...'), 11)
self.lvLdapMenu1.insertItem(QIconSet(self.icons.get("user.png")), self.__tr('&User...'), 12)
self.lvLdapMenu1.insertItem(QIconSet(self.icons.get("ldap.png")), self.__tr('&Group...'), 13)
self.lvLdapMenu1.insertItem(QIconSet(self.icons.get("group.png")), self.__tr('&Organization...'), 14)
self.lvLdapMenu1.insertItem(QIconSet(self.icons.get("network_local.png")), self.__tr('&DHCP entry...'), 15)

self.lvLdapMenu.insertSeparator()
self.lvLdapMenu.clipBoard = u"None"
self.lvLdapMenu.insertItem(QIconSet(self.icons.get("edit.png")), self.__tr('&Rename'), 20)
self.lvLdapMenu.insertItem(QIconSet(self.icons.get("editcopy.png")), self.__tr('&Copy'), 21)
self.lvLdapMenu.insertItem(QIconSet(self.icons.get("editcut.png")), self.__tr('C&ut'), 22)
self.lvLdapMenu.insertItem(QIconSet(self.icons.get("editpaste.png")), self.__tr('&Paste'), 23)
self.lvLdapMenu.insertItem(QIconSet(self.icons.get("editdelete.png")), self.__tr('&Delete'), 24)
self.lvLdapMenu.insertSeparator()
self.lvLdapMenu.insertItem(QIconSet(self.icons.get("folder_crystal.png")), self.__tr('&Tools'), self.lvLdapMenu2, 30)

self.lvLdapMenu2.insertItem(QIconSet(self.icons.get("password.png")), self.__tr('Set &Password'), 31)
self.lvLdapMenu2.insertItem(QIconSet(self.icons.get("network_local.png")), self.__tr('&Samba Populate'), 32)
self.lvLdapMenu2.insertItem(QIconSet(self.icons.get("network_local.png")), self.__tr('&DHCP Populate'), 33)
self.lvLdapMenu2.insertSeparator()
self.lvLdapMenu2.insertItem(QIconSet(self.icons.get("bottom.png")), self.__tr('View: &expand'), 36)
self.lvLdapMenu2.insertItem(QIconSet(self.icons.get("bottom.png")), self.__tr('View: &expand all'), 37)
self.lvLdapMenu2.insertItem(QIconSet(self.icons.get("bottom.png")), self.__tr('View: &set as LDAP Base'), 34)
self.lvLdapMenu2.insertItem(QIconSet(self.icons.get("bottom.png")), self.__tr('View: &back to LDAP Base'), 35)
self.lvLdapMenu2.insertSeparator()
self.lvLdapMenu2.insertItem(QIconSet(self.icons.get("start.png")), self.__tr('&CSV to LDIF converter'), 38)
#self.lvLdapMenu2.insertItem(QIconSet(self.icons.get("start.png")), self.__tr('Import: from &CSV'), 38)
#self.lvLdapMenu2.insertItem(QIconSet(self.icons.get("start.png")), self.__tr('Import: from &LDIF'), 39)
self.pLdapImportCSV.setHidden(True)

self.lvLdapMenu.insertSeparator()
self.lvLdapMenu.insertItem(QIconSet(self.icons.get("server.png")), self.__tr('&Server'), self.lvServersLdapMenu, 42)

self.connect(self.lvLdapMenu,   SIGNAL('activated(int)'), self.ldap_menu_clicked)
self.connect(self.lvLdapMenu1, SIGNAL('activated(int)'), self.ldap_menu_clicked)
self.connect(self.lvLdapMenu2, SIGNAL('activated(int)'), self.ldap_menu_clicked)

#
# IMAP Menu
#

self.lvImapMenu = QPopupMenu(self)

self.lvImapMenu.insertItem(QIconSet(self.icons.get("edit.png")), self.__tr('&Rename'), 0)
self.lvImapMenu.insertItem(QIconSet(self.icons.get("editdelete.png")), self.__tr('&Delete'), 1)
self.lvImapMenu.insertItem(QIconSet(self.icons.get("wizard.png")), self.__tr('Re&construct'), 2)

self.lvImapMenu.insertSeparator()
self.lvImapMenu.insertItem(QIconSet(self.icons.get("bottom.png")), self.__tr('Expand &All'), 7)
self.lvImapMenu.insertSeparator()
self.lvImapMenu.insertItem(QIconSet(self.icons.get("finish.png")), self.__tr('&Export'), 10)
#self.lvImapMenu.insertItem(QIconSet(self.icons.get("start.png")), self.__tr('&Import'), 13)

self.lvImapMenu.insertSeparator()
self.lvImapMenu.insertItem(QIconSet(self.icons.get("server.png")), self.__tr('&Server'), self.lvServersImapMenu, 3)

self.connect(self.lvImapMenu, SIGNAL('activated(int)'), self.imap_menu_clicked)

#
# IMAP Partition Menu
#

self.lvImapPartMenu = QPopupMenu(self)
self.lvImapPartMenu.insertItem(QIconSet(self.icons.get("all.png")), self.__tr('Select &all'), 0)
self.lvImapPartMenu.insertSeparator()
self.lvImapPartMenu.insertItem(self.__tr('&Quota info'), 7)
self.lvImapPartMenu.insertItem(self.__tr('&Mailbox info'), 8)
self.lvImapPartMenu.insertSeparator()
#self.lvImapPartMenu.insertItem(QIconSet(self.icons.get("wizard.png")), self.__tr('Re&construct'), 3)
#self.lvImapPartMenu.insertItem(QIconSet(self.icons.get("editdelete.png")), self.__tr('&Delete'), 2)
self.lvImapPartMenu.insertItem(QIconSet(self.icons.get("quota.png")), self.__tr('&Set Quota...'), 1)
self.lvImapPartMenu.insertItem(QIconSet(self.icons.get("finish.png")), self.__tr('Export to &CSV...'), 4)
self.lvImapPartMenu.insertSeparator()
self.lvImapPartMenu.insertItem(QIconSet(self.icons.get("server.png")), self.__tr('&Server'), self.lvServersImapMenu, 10)

self.connect(self.lvImapPartMenu, SIGNAL('activated(int)'), self.imap_partition_menu_clicked)

#
# Sieve Menu
#

self.lvSieveMenu = QPopupMenu(self)
self.lvSieveMenu.insertItem(QIconSet(self.icons.get("all.png")), self.__tr('Select &all'), 0)
self.lvSieveMenu.insertSeparator()
self.lvSieveMenu.insertItem(QIconSet(self.icons.get("server.png")), self.__tr('&Server'), self.lvServersImapMenu, 1)

self.lvSieveTemplateMenu = QPopupMenu(self)
self.lvSieveTemplateMenu.insertItem(self.__tr('&New...'), 10)
self.lvSieveTemplateMenu.insertItem(self.__tr('&Save'), 11)
self.lvSieveTemplateMenu.insertItem(self.__tr('Save &as...'), 12)
self.lvSieveTemplateMenu.insertItem(self.__tr('&Rename'), 13)
self.lvSieveTemplateMenu.insertItem(self.__tr('&Delete'), 14)

self.connect(self.lvSieveMenu, SIGNAL('activated(int)'), self.sieve_menu_clicked)
self.connect(self.lvSieveTemplateMenu, SIGNAL('activated(int)'), self.sieve_menu_clicked)

#
# Services Menu
#

self.lvServicesMenu = QPopupMenu(self)
self.lvServicesMenu.insertItem(QIconSet(self.icons.get("server.png")), self.__tr('&Server'), self.lvServersSSHMenu, 0)

#
# Queue Menu
#

self.lvQueueMenu1 = QPopupMenu(self)
self.lvQueueMenu1.insertItem(QIconSet(self.icons.get("mail_forward.png")), self.__tr('&Flush'), 10)
self.lvQueueMenu1.insertItem(QIconSet(self.icons.get("start.png")), self.__tr('&Hold'), 11)
self.lvQueueMenu1.insertItem(QIconSet(self.icons.get("finish.png")), self.__tr('&Unhold'), 12)
self.lvQueueMenu1.insertItem(QIconSet(self.icons.get("reload.png")), self.__tr('&Requeue'), 13)
self.lvQueueMenu1.insertItem(QIconSet(self.icons.get("editdelete.png")), self.__tr('&Delete'), 14)

self.lvQueueMenu2 = QPopupMenu(self)
self.lvQueueMenu2.insertItem(self.__tr('On &typing'), 20)
self.lvQueueMenu2.insertSeparator()
self.lvQueueMenu2.insertItem(QIconSet(self.icons.get("ok.png")), self.__tr('&Sender'), 21)
self.lvQueueMenu2.insertItem(self.__tr('&Recipient'), 22)
self.lvQueueMenu2.insertItem(self.__tr('S&ender/recipient'), 23)

self.lvQueueMenu = QPopupMenu(self)
self.lvQueueMenu.insertItem(QIconSet(self.icons.get("reload.png")), self.__tr('&Update'), 0)
self.lvQueueMenu.insertSeparator()
self.lvQueueMenu.insertItem(QIconSet(self.icons.get("mail_find.png")), self.__tr('&Show message'), 1)
self.lvQueueMenu.insertItem(QIconSet(self.icons.get("mail_send.png")), self.__tr('&Show already sent'), 2)
self.lvQueueMenu.insertItem(QIconSet(self.icons.get("start.png")), self.__tr('&Hold'), 3)
self.lvQueueMenu.insertItem(QIconSet(self.icons.get("finish.png")), self.__tr('&Unhold'), 4)
self.lvQueueMenu.insertItem(QIconSet(self.icons.get("reload.png")), self.__tr('&Requeue'), 5)
self.lvQueueMenu.insertItem(QIconSet(self.icons.get("editdelete.png")), self.__tr('&Delete'), 6)
self.lvQueueMenu.insertSeparator()
self.lvQueueMenu.insertItem(QIconSet(self.icons.get("bottom.png")), self.__tr('Expand &All'), 9)
self.lvQueueMenu.insertSeparator()
self.lvQueueMenu.insertItem(QIconSet(self.icons.get("queue.png")), self.__tr('All &Queue'), self.lvQueueMenu1, 99)
self.lvQueueMenu.insertSeparator()
self.lvQueueMenu.insertItem(QIconSet(self.icons.get("find.png")), self.__tr('Search &options'), self.lvQueueMenu2, 99)
self.lvQueueMenu.insertSeparator()
self.lvQueueMenu.insertItem(QIconSet(self.icons.get("server.png")), self.__tr('&Server'), self.lvServersSSHMenu, 99)

self.connect(self.lvQueueMenu, SIGNAL('activated(int)'), self.queue_menu_clicked)
self.connect(self.lvQueueMenu1, SIGNAL('activated(int)'), self.queue_menu_clicked)
self.connect(self.lvQueueMenu2, SIGNAL('activated(int)'), self.queue_menu_clicked)

#
# End Menu
#

self.cbConfLdapSmbSIDEntry.listBox().setMinimumWidth(600)
self.cbConfLdapSmbCounterEntry.listBox().setMinimumWidth(600)
self.teSieveScript.setConfig(self.korreioConfigDict.get("path.sieveConf"))

#
# Load configuration
#

self.config_load()
self.sieve_template_load()

#
# Update Interface
#

self.wsKorreio.raiseWidget(1)
self.korreio_module_changed()

}

void dKorreio::statusBar() {

#
# Remove default status bar
#

pass

}

void dKorreio::console( a0, a1 ) {
# a0=str:text, a1=int:textMode
# textMode: 0 (OK), 1 (ERROR), 2 (WARN), 3 (INFO)

text = Q2uni(a0)
status = a2

textMode = QString("<b>%1:</b> ")
if a1 is None or a1 == self.CONSOLE_OK:
    textMode = textMode.arg(self.__tr("Ok"))
elif a1 == self.CONSOLE_ERR:
    textMode = textMode.arg(self.__tr("Error"))
elif a1 == self.CONSOLE_WARN:
    textMode = textMode.arg(self.__tr("Warn"))
elif a1 == self.CONSOLE_INFO:
    textMode = textMode.arg(self.__tr("Info"))
else:
    print "ERROR: unexpected console() mode."
    raise "KORREIOError", "ERROR: unexpected console() mode."

textMode = Q2uni(textMode)
textDate = datetime.datetime.now().strftime("%d %b %Y %H:%M:%S :/# ")

#
# Update Console
#

if status:
    self.tlConsole.setText(u"&nbsp;%s%s%s" % (textDate, textMode, text))
    self.tlConsole.setIndent(0)

#
# Update Log Interface
#

textMode = re.sub(u"(</?b>)", u"", textMode)
self.lbLog.insertItem(u"%s%s%s" % (textDate, textMode, text))
self.lbLog.setCurrentItem(self.lbLog.count() - 1)
self.lbLog.ensureCurrentVisible()

#
# Update Log File
#

try:
    f = open(os.path.expanduser("~/.korreio/korreio.log"), 'a')
    f.write("%s%s%s\n" % (textDate, textMode.encode('utf-8'), text.encode('utf-8')))
    f.close()
except OSError, e:
    print "ERROR: can't save log file. "+e

}

void dKorreio::console_debug( a0 ) {
# a0=str:text

print "#\n# Korreio Debug Started\n#\n\n", a0, "\n\n#\n# Korreio Debug Stopped\n#\n"

}

void dKorreio::parse_exception( a0, a1 ) {
# a0=str:type, a1=list:reason
# type: "LDAPError", "IMAPError", "pexpect.EOF", "pexpect.TIMEOUT"

#
# Parse LDAPError
#

if a0 == "LDAPError":
    if a1[0]["desc"] == "Size limit exceeded":
        self.console(self.__tr("search results many entries, server limit anwser. Set a filter properly."), self.CONSOLE_INFO)
    elif a1[0]["desc"] == "Bad search filter":
        self.console(self.__tr("invalid syntax for search filter. Set a filter properly."), self.CONSOLE_ERR)
    elif a1[0]["desc"] == "No such object":
        self.console(self.__tr("search don't have results."), self.CONSOLE_WARN)
    elif a1[0]["desc"] == "Already exists":
        self.console(self.__tr("entry already exists."), self.CONSOLE_ERR)
    elif a1[0]["desc"] == "Invalid syntax":
        self.console(self.__tr("invalid syntax for attribute '%1'.").arg(a1[0]['info'].split(":")[0]), self.CONSOLE_ERR)
    elif a1[0]['desc'] == "Object class violation":
        errList = a1[0]['info'].split("'")
        if len(errList) == 4:
            self.console(self.__tr("objectClass '%1' require attribute '%2'.").arg(errList[1]).arg(errList[3]), self.CONSOLE_ERR)
        elif len(errList) == 3:
            self.console(self.__tr("attribute '%1' is not supported by these objectClass's.").arg(errList[1]), self.CONSOLE_ERR)
        else:
            errList = a1[0]['info'].split("/")
            if len(errList) == 2:
                self.console(self.__tr("incompatible objectClasses '%1' e '%2'.").arg(errList[0].split("(")[1]).arg(errList[1].split(")")[0]), self.CONSOLE_ERR)
            else:
                self.console(self.__tr("object class violation (%1).").arg(a1[0]['info']), self.CONSOLE_ERR)
    elif a1[0]['desc'] == "Naming violation":
        self.console(a1[0]['info'], self.CONSOLE_ERR)
    elif a1[0]['desc'] == "Can't contact LDAP server":
        server = "%s%s:%s" % (self.cbLdapMode.currentText().ascii(), self.iLdapHost.text().ascii(), self.iLdapPort.text().ascii())
        self.console(self.__tr("connection refused to %1.").arg(server), self.CONSOLE_ERR)
        try:
            del self.l
        except:
            pass
    elif a1[0]['desc'] == "Undefined attribute type":
        self.console(self.__tr("attribute '%1' undefined at schema.").arg(a1[0]['info'].split(":")[0]), self.CONSOLE_ERR)
    elif a1[0]['desc'] == "Type or value exists":
        self.console(self.__tr("duplicate attribute '%1'.").arg(a1[0]['info'].split(":")[0]), self.CONSOLE_ERR)
    elif a1[0]['info'] == "no write access to parent":
        self.console(self.__tr("no write permission to parent entry '%1'.").arg(self.ldap_current_dn()), self.CONSOLE_ERR)
    elif a1[0]['info'] == "modifications require authentication":
        user = self.iLdapUser.text().ascii()
        if not user: user = "anonymous"
        self.console(self.__tr("no write permission by user '%1' or wrong ldap authentication.").arg(user), self.CONSOLE_ERR)
    elif a1[0]['desc'] == "No such attribute":
        self.console(a1[0]['info'], self.CONSOLE_ERR)
    else:
        self.console(str(a1), self.CONSOLE_ERR)

#
# Parse IMAPError
#

elif a0 == "IMAPError":
    print [a0, a1]
    self.korreio_module_clear("imap")
    server = "%s%s:%s/%s" % (self.cbCyrusMode.currentText().ascii(), self.iCyrusHost.text().ascii(), self.iCyrusPort.text().ascii(), self.iCyrusUser.text().ascii())
    if a1[2] == "Connection error":
        self.console(self.__tr("connection refused to %1.").arg(server), self.CONSOLE_ERR)
    elif a1[2] == "authentication failure":
        self.console(self.__tr("wrong user or password to %1.").arg(server), self.CONSOLE_ERR)
    elif a1[2] == "User is not cyrus administrator":
        self.console(self.__tr("user is not cyrus administrator to %1.").arg(server), self.CONSOLE_ERR)
    else:
        self.console(self.__tr("%1 (%2).").arg(server).arg("%s" % a1[2]), self.CONSOLE_ERR)

#
# Parse pexpect.EOF
#

elif a0 == "pexpect.EOF":
    server = "ssh://%s:%s" % (self.iSshHost.text().ascii(), self.iSshPort.text().ascii())
    if re.search("match: pexpect.TIMEOUT", "%s" % a1):
        self.console(self.__tr("connection timeout to %1.").arg(server), self.CONSOLE_ERR)
    elif re.search("after: Permission denied", "%s" % a1):
        self.console(self.__tr("wrong user or password to %1.").arg(server), self.CONSOLE_ERR)
    elif re.search("Connection refused", "%s" % a1):
        self.console(self.__tr("connection refused to %1.").arg(server), self.CONSOLE_ERR)
    elif re.search("Host key verification failed", "%s" % a1):
        self.console(self.__tr("invalid fingerprint to %1.").arg(server), self.CONSOLE_ERR)
    else:
        self.console(self.__tr("connection dropped to %1.").arg(server), self.CONSOLE_ERR)
        self.console_debug(a1)

#
# Parse pexpect.TIMEOUT
#

elif a0 == "pexpect.TIMEOUT":
    server = "ssh://%s:%s" % (self.iSshHost.text().ascii(), self.iSshPort.text().ascii())
    self.console(self.__tr("connection timeout to %1.").arg(server), self.CONSOLE_ERR)
    self.console_debug(a1)

#
# Parse Anything
#

else:
    self.console(str(a1), self.CONSOLE_ERR)

}

void dKorreio::korreio_update_servers_menu() {

#
# Update Imap Servers Menu
#

self.lvServersImapMenu.clear()
for server in range(0, self.cbCyrusConnection.count()):
    if server == self.cbCyrusConnection.currentItem():
        self.lvServersImapMenu.insertItem(QIconSet(self.icons.get("ok.png")), self.cbCyrusConnection.text(server).ascii(), int(server))
    else:
        self.lvServersImapMenu.insertItem(self.cbCyrusConnection.text(server).ascii(), int(server))

#
# Update Ldap Servers Menu
#

self.lvServersLdapMenu.clear()
for server in range(0, self.cbLdapConnection.count()):
    if server == self.cbLdapConnection.currentItem():
        self.lvServersLdapMenu.insertItem(QIconSet(self.icons.get("ok.png")), self.cbLdapConnection.text(server).ascii(), int(server))
    else:
        self.lvServersLdapMenu.insertItem(self.cbLdapConnection.text(server).ascii(), int(server))

#
# Update SSH Servers Menu
#

self.lvServersSSHMenu.clear()
for server in range(0, self.cbSSHConnection.count()):
    if server == self.cbSSHConnection.currentItem():
        self.lvServersSSHMenu.insertItem(QIconSet(self.icons.get("ok.png")), self.cbSSHConnection.text(server).ascii(), int(server))
    else:
        self.lvServersSSHMenu.insertItem(self.cbSSHConnection.text(server).ascii(), int(server))

}

void dKorreio::korreio_servers_menu_set_ldap( a0 ) {

#
# Change current Ldap server
#

self.cbLdapConnection.setCurrentItem(a0)
self.config_ldap_set_connection()

}


void dKorreio::korreio_servers_menu_set_imap( a0 ) {

#
# Change current Imap server
#

self.cbCyrusConnection.setCurrentItem(a0)
self.config_imap_set_connection()

}

void dKorreio::korreio_servers_menu_set_ssh( a0 ) {

#
# Change current SSH server
#

self.cbSSHConnection.setCurrentItem(a0)
self.config_ssh_set_connection()

}


void dKorreio::korreio_module_clear( a0 ) {
# a0=str:module
# module: "ldap", "imap", "imap-partition", "sieve", "ssh"
#               "ldap.form", "ldap.form.unit", "ldap.form.password"
#               "ldap.smb.populate", "ldap.dhcp.populate"

module = a0

if module == "ldap":
    self.lvLdap.clear()
    self.lvLdapAttr.clear()
elif module == "imap":
    self.lvImap.clear()
    self.iImapMailbox.clear()
    self.lvImapAcl.clear()
    self.iImapAclUser.clear()
    self.cbACL.setCurrentItem(0)
    self.iQuota.clear()
    self.iQuotaUsed.clear()
    self.iAnnotationValue.clear()
    self.cbImapAnnotation.setCurrentItem(0)
    self.iAnnotationValue.clear()
elif module == "imap-partition":
    self.lvImapPartition.clear()
    self.cbImapPartition.clear()
    self.pImapPartitionMove.setEnabled(False)
    self.tlImapSize.setText("0")
elif module == "sieve":
    self.lvSieve.clear()
#    self.lvSieve.setColumnAlignment(1, Qt.AlignHCenter)
    self.cbSieveScript.clear()
    self.teSieveScript.clear()
elif module == "ssh":
    self.cbServicesPostconf.clear()
    self.teServicesPostconf.clear()
    self.teServicesFileOpen.clear()
    self.korreio_module_clear("ssh.queue")
elif module == "ssh.queue":
    self.lvQueue.clear()
    self.iQueueMessage.clear()
    self.tlQueueMsgs.setText("0/0")
elif module == "ldap.form":
    self.iLdapFormCn.setFocus()
    self.iLdapFormCn.clear()
    self.iLdapFormMail.clear()
    self.iLdapFormStreet.clear()
    self.iLdapFormL.clear()
    self.iLdapFormPostalCode.clear()
    self.iLdapFormHomePhone.clear()
    self.iLdapFormUserP.clear()
    self.iLdapFormUserP2.clear()
    self.cLdapFormPosix.setChecked(False)
    self.fLdapFormPosix.setEnabled(False)
    self.iLdapFormUid.clear()
    self.iLdapFormUidNumber.setText("1000")
    self.iLdapFormGidNumber.setText("100")
    self.iLdapFormUidNumber.setEnabled(True)
    self.pLdapGetUidNumber.setEnabled(True)
    self.iLdapFormGidNumber.setEnabled(True)
    self.iLdapFormLoginShell.setText("/bin/bash")
    self.iLdapFormHomeDirectory.setText("/home")
    self.cLdapFormSamba.setChecked(False)
    self.fLdapFormSamba.setEnabled(False)
    self.cbLdapFormSambaDomain.clear()
    self.cbLdapFormPrimaryGroup.setCurrentItem(0)
    self.iLdapFormSambaPwdMustChange.setChecked(True)
    self.cbLdapFormProfileType.setCurrentItem(0)
    self.iLdapFormProfilePath.clear()
    self.iLdapFormProfilePath.setEnabled(False)
    self.iLdapFormHomeDrive.clear()
    self.iLdapFormDrivePath.clear()
    self.iLdapFormLogonScript.clear()
    self.cLdapFormAst.setChecked(False)
    self.fLdapFormAst.setEnabled(False)
    self.iLdapFormAstUsername.clear()
    self.iLdapFormAstName.setText("1000")
    self.cLdapFormRadius.setChecked(False)
    self.fLdapFormRadius.setEnabled(False)
    self.iLdapFormRadiusGroup.clear()
    self.wsLdapForm.raiseWidget(0)
elif module == "ldap.form.unit":
    self.iLdapFormUnit.setFocus()
    self.cbLdapFormUnit.setCurrentItem(0)
    self.iLdapFormUnit.clear()
    self.iLdapFormUnitStreet.clear()
    self.iLdapFormUnitL.clear()
    self.iLdapFormUnitPostalCode.clear()
    self.iLdapFormUnitTelephoneNumber.clear()
elif module == "ldap.form.group":
    self.iLdapFormGroupName.setFocus()
    self.iLdapFormGroupName.clear()
    self.iLdapFormGroupDesc.clear()
    self.iLdapFormGroupGidNumber.clear()
elif module == "ldap.form.dhcp":
    self.ldap_wizard_dhcp_type()
    self.iLdapFormDhcpGroupName.clear()
    self.iLdapFormDhcpName.clear()
    self.iLdapFormDhcpIPaddress.clear()
    self.iLdapFormDhcpMACaddress.clear()
    self.iLdapFormDhcpComments.clear()
    self.cbLdapFormDhcpInterface.setCurrentItem(0)
    self.iLdapFormDhcpNetwork.clear()
    self.iLdapFormDhcpBroadcast.clear()
    self.iLdapFormDhcpRange.clear()
    self.cbLdapFormDhcpNetmask.setCurrentItem(8)
    self.iLdapFormDhcpGateway.clear()
elif module == "ldap.form.password":
    self.iLdapPasswd.clear()
    self.iLdapPasswd2.clear()
    self.iLdapPasswd.setFocus()
elif module == "ldap.smb.populate":
    self.iLdapSMBdomain.setFocus()
    self.iLdapSMBdomain.clear()
    self.iLdapSMBSID.clear()
    self.iLdapSMBpassword.clear()
    self.iLdapSMBuidNumber.setText(self.iConfLdapSMBuidNumber.text().ascii())
    self.iLdapSMBgidNumber.setText(self.iConfLdapSMBgidNumber.text().ascii())
    self.iLdapSMBminPwdLength.setText(self.iConfLdapSMBminPwdLength.text().ascii())
    self.iLdapSMBpwdHistLenght.setText(self.iConfLdapSMBpwdHistLenght.text().ascii())
    self.iLdapSMBmaxPwdAge.setText(self.iConfLdapSMBmaxPwdAge.text().ascii())
    self.cbLdapSMBmaxPwdAge.setCurrentItem(self.cbConfLdapSMBmaxPwdAge.currentItem())
    self.iLdapSMBminPwdAge.setText(self.iConfLdapSMBminPwdAge.text().ascii())
    self.cbLdapSMBminPwdAge.setCurrentItem(self.cbConfLdapSMBminPwdAge.currentItem())
    self.iLdapSMBlockout.setText(self.iConfLdapSMBlockout.text().ascii())
    self.iLdapSMBlockoutDuration.setText(self.iConfLdapSMBlockoutDuration.text().ascii())
    self.cbLdapSMBlockoutDuration.setCurrentItem(self.cbConfLdapSMBlockoutDuration.currentItem())
    self.iLdapSMBlockoutWindow.setText(self.iConfLdapSMBlockoutWindow.text().ascii())
    self.cbLdapSMBlockoutWindow.setCurrentItem(self.cbConfLdapSMBlockoutWindow.currentItem())
elif module == "ldap.dhcp.populate":
    self.iLdapDhcpName.setFocus()
    self.iLdapDhcpName.clear()
    self.iLdapDhcpDefaultLeaseTime.setText(self.iConfLdapDhcpDefaultLeaseTime.text().ascii())
    self.cbLdapDhcpDefaultLeaseTime.setCurrentItem(self.cbConfLdapDhcpDefaultLeaseTime.currentItem())
    self.iLdapDhcpMaxLeaseTime.setText(self.iConfLdapDhcpMaxLeaseTime.text().ascii())
    self.cbLdapDhcpMaxLeaseTime.setCurrentItem(self.cbConfLdapDhcpMaxLeaseTime.currentItem())
    self.iLdapDhcpDomainName.setText(self.iConfLdapDhcpDomainName.text().ascii())
    self.iLdapDhcpNetbiosServers.setText(self.iConfLdapDhcpNetbiosServers.text().ascii())
    self.iLdapDhcpDNSservers.setText(self.iConfLdapDhcpDNSservers.text().ascii())
    self.cbLdapDhcpInterface.setCurrentText(self.cbConfLdapDhcpInterface.currentText().ascii())
    self.iLdapDhcpNetwork.setText(self.iConfLdapDhcpNetwork.text().ascii())
    self.iLdapDhcpBroadcast.setText(self.iConfLdapDhcpBroadcast.text().ascii())
    self.iLdapDhcpRange.setText(self.iConfLdapDhcpRange.text().ascii())
    self.cbLdapDhcpNetmask.setCurrentItem(self.cbConfLdapDhcpNetmask.currentItem())
    self.iLdapDhcpGateway.setText(self.iConfLdapDhcpGateway.text().ascii())

}

void dKorreio::korreio_module_changed() {

try:
    self.startup
except:
    self.startup = 1
    # Update servers menu
    self.korreio_update_servers_menu()
    # Retrive log
    self.config_log_mode()
    # Default Config Widget
    self.wsConfig.raiseWidget(3)
    # Default Imap Width
    self.spImap.setSizes([400, 457])
    # Default Sieve Width
    self.spSieve.setSizes([600, 257])
    # Close IMAP annotation
    self.lvConfig.findItem("2.2", 1).setOpen(False)

#
# Korreio Module was changed
#

page = self.wsKorreio.id(self.wsKorreio.visibleWidget())

if page == 5:
        # Queue
        self.lvQueue.setColumnWidth(1, 126)
        self.lvQueue.setColumnWidth(0, self.lvQueue.width() - 130)

elif page == 6:
    # Services
    if self.cbSSHsudo.currentItem() == 0:
        self.pServicesFileSave.setEnabled(True)
    else:
        self.console("File saving is not available with sudo mode.", self.CONSOLE_INFO)
        self.pServicesFileSave.setEnabled(False)
    self.lvServices.setColumnWidth(1, 0)
    self.lvServices.setColumnWidth(0, self.lvServices.width() - 4)
elif page == 7:
    # Configuration
    self.lvConfig.setColumnWidth(1, 0)
    self.lvConfig.setColumnWidth(0, self.lvServices.width() - 4)
    self.lvConfig.currentItem().setSelected(True)
    self.config_change_widgetstack()

}

void dKorreio::korreio_menu_clicked( a0 ) {

if a0 == 0:
    self.wsKorreio.raiseWidget(7)
elif a0 == 1:
    self.wsKorreio.raiseWidget(0)
elif a0 == 3:
    sys.exit(0)
elif a0 == 10:
    self.wsKorreio.raiseWidget(1)
elif a0 == 11:
    self.wsKorreio.raiseWidget(2)
elif a0 == 12:
    self.wsKorreio.raiseWidget(3)
elif a0 == 13:
    self.wsKorreio.raiseWidget(4)
elif a0 == 14:
    self.wsKorreio.raiseWidget(5)
elif a0 == 15:
    self.wsKorreio.raiseWidget(6)
elif a0 == 20:
    QMessageBox.information( None, self.__tr("About..."), self.__tr("\n     Korreio - Copyright 2007-2009, Reinaldo de Carvalho     \n\n     Website: http://korreio.sf.net\n     Mailing list: http://groups.google.com/group/korreio\n\n"))


self.korreio_module_changed()

}

void dKorreio::config_load() {

#
# Make Korreio work directory
#

try:
    os.makedirs(os.path.expanduser("~/.korreio/sieve"), 0700)
except OSError, e:
    # e:17 directory already exists
    if e[0] != 17:
        self.console(self.__tr("can't make directory ~/.korreio (%1).").arg(e), self.CONSOLE_ERR)
        print "ERROR: make directory ~/.korreio failed (%s)." % e
        return False

#
# Create default configuration file
#

try:
    if not os.path.isfile(os.path.expanduser("~/.korreio/korreio.conf")):
        self.config_save()
except OSError, e:
    pass

#
# Read configuration file
#

try:
    f = open(os.path.expanduser("~/.korreio/korreio.conf"), 'r')
    conf = f.read()
    f.close()
except IOError, e:
    self.console(self.__tr("can't read configuration file ~/.korreio/korreio.conf (%1).").arg(e), self.CONSOLE_ERR)
    print "ERROR: can't read configuration file ~/.korreio/korreio.conf (%s)." % e
    return False

confList = conf.split("\n")
confList.pop()
confDict = {}

for line in confList:
   key=line.split("=")
   confDict[key[0]] = "=".join(key[1:])

self.confDict = confDict

#
# LDAP Connection
#

i=0
self.cbLdapConnection.clear()
while confDict.get("ldap%s.name" % i):
    self.cbLdapConnection.insertItem(confDict.get("ldap%s.name" % i))
    i+=1

lastConn=confDict.get("ldap.last")
if lastConn:
    self.cbLdapConnection.setCurrentText(confDict.get("%s.name" % lastConn))
    self.iLdapConnection.setText(confDict.get("%s.name" % lastConn))
    self.cbLdapMode.setCurrentText(confDict.get("%s.mode" % lastConn))
    self.iLdapHost.setText(confDict.get("%s.host" % lastConn))
    self.iLdapPort.setText(confDict.get("%s.port" % lastConn))
    self.cbLdapBaseDN.clear()
    self.cbLdapBaseDN.insertItem(confDict.get("%s.basedn" % lastConn))
    self.iLdapUser.setText(confDict.get("%s.user" % lastConn))
    self.iLdapPass.setText(confDict.get("%s.pass" % lastConn))
    if confDict.get("%s.ref" % lastConn) == "True":
        self.cLdapRef.setChecked(True)
    if confDict.get("%s.cert" % lastConn) == "True":
        self.cLdapCert.setChecked(True)

#
# IMAP Connection
#

i=0
self.cbCyrusConnection.clear()
while confDict.get("imap%s.name" % i):
    self.cbCyrusConnection.insertItem(confDict.get("imap%s.name" % i))
    i+=1

lastConn=confDict.get("imap.last")
if lastConn:
    self.cbCyrusConnection.setCurrentText(confDict.get("%s.name" % lastConn))
    self.iCyrusConnection.setText(confDict.get("%s.name" % lastConn))
    self.cbCyrusMode.setCurrentText(confDict.get("%s.mode" % lastConn))
    self.iCyrusHost.setText(confDict.get("%s.host" % lastConn))
    self.iCyrusPort.setText(confDict.get("%s.port" % lastConn))
    self.iCyrusSievePort.setText(confDict.get("%s.sieport" % lastConn))
    self.iCyrusUser.setText(confDict.get("%s.user" % lastConn))
    self.iCyrusPass.setText(confDict.get("%s.pass" % lastConn))
    self.iCyrusPart.setText(confDict.get("%s.part" % lastConn))

#
# SSH Connection
#

i=0
self.cbSSHConnection.clear()
while confDict.get("ssh%s.name" % i):
    self.cbSSHConnection.insertItem(confDict.get("ssh%s.name" % i))
    i+=1

lastConn=confDict.get("ssh.last")
if lastConn:
    self.cbSSHConnection.setCurrentText(confDict.get("%s.name" % lastConn))
    self.iSSHConnection.setText(confDict.get("%s.name" % lastConn))
    self.iSshHost.setText(confDict.get("%s.host" % lastConn))
    self.iSshPort.setText(confDict.get("%s.port" % lastConn))
    self.iSshUser.setText(confDict.get("%s.user" % lastConn))
    self.iSshPass.setText(confDict.get("%s.pass" % lastConn))
try:
    self.cbSSHsudo.setCurrentItem(int(confDict.get("%s.sudo" % lastConn)))
except:
    pass

#
# IMAP Prefs
#

self.lvConfImapFolders.clear()
i=0
while confDict.get("imap.defaultfolder%s" % i):
    folderList = confDict.get("imap.defaultfolder%s" % i).split(":")
    item = QListViewItem(self.lvConfImapFolders)
    item.setText(0, folderList[0])
    if folderList[1] != "None" and folderList[1] != "0":
        item.setText(1, folderList[1])
    if folderList[2] != "None":
        item.setText(2, folderList[2])
    item.setPixmap(0, self.icons.get("folder.png"))
    i+=1

if confDict.get("imap.defaultquota"):
    self.iConfImapQuotaMbytes.setText(confDict.get("imap.defaultquota"))

#
# LDAP Prefs
#

if confDict.get("ldap.admin.autocomplete"):
    self.cbConfLdapCompleteUser.setCurrentText(confDict.get("ldap.admin.autocomplete"))

if confDict.get("ldap.passwd.sambaPwdMustChange") == "True":
    self.cConfLdapsambaPwdMustChange.setChecked(True)
else:
    self.cConfLdapsambaPwdMustChange.setChecked(False)

if confDict.get("ldap.entry.attributeDN"):
    self.cbConfLdapUserDN.setCurrentItem(int(confDict.get("ldap.entry.attributeDN")))

self.lvConfLdapSchema.clear()
item = QListViewItem(self.lvConfLdapSchema)
item.setText(0,'objectClass')
item.setOpen(True)
objnodes = {}
i=0
while confDict.get("ldap.objectclass%s" % i):
    objclass = confDict.get("ldap.objectclass%s" % i).split(".")
    if not objnodes.get(objclass[0]):
        objnodes[objclass[0]] = QListViewItem(item)
        objnodes[objclass[0]].setText(0,objclass[0])
    subitem=QListViewItem(objnodes[objclass[0]])
    subitem.setText(0,objclass[1])
    subitem.setText(1,".".join(objclass[2:]))
    i+=1

if confDict.get("ldap.schema.addattrs") == "True" or not confDict.get("ldap.schema.addattrs"):
    self.cConfLdapSchemaAddAttr.setChecked(True)
else:
    self.cConfLdapSchemaAddAttr.setChecked(False)

if confDict.get("ldap.schema.delattrs") == "True" or not confDict.get("ldap.schema.delattrs"):
    self.cConfLdapSchemaDelAttr.setChecked(True)
else:
    self.cConfLdapSchemaDelAttr.setChecked(False)

self.lbConfLdapFilter.clear()
self.cbLdapFilter.clear()
i=0
while confDict.get("ldap.filter%s" % i):
    self.lbConfLdapFilter.insertItem(confDict.get("ldap.filter%s" % i))
    self.cbLdapFilter.insertItem(confDict.get("ldap.filter%s" % i))
    i+=1

#
# LDAP SMB Prefs
#

i=0
self.cbConfLdapSmbDomain.clear()
while confDict.get("ldap.smb%s.domain" % i):
    self.cbConfLdapSmbDomain.insertItem(confDict.get("ldap.smb%s.domain" % i))
    i+=1

lastConn=confDict.get("ldap.smb.last")
if lastConn:
    self.cbConfLdapSmbDomain.setCurrentText(confDict.get("%s.domain" % lastConn))
    self.iConfLdapSmbDomain.setText(confDict.get("%s.domain" % lastConn))
    if confDict.get("%s.SIDEntry" % lastConn):
        self.cbConfLdapSmbSIDEntry.clear()
        self.cbConfLdapSmbSIDEntry.insertItem(confDict.get("%s.SIDEntry" % lastConn))
    self.cbConfLdapSmbSIDEntry.insertItem("")
    self.cbConfLdapSmbCounterEntry.clear()
    if confDict.get("%s.counterEntry" % lastConn):
        self.cbConfLdapSmbCounterEntry.clear()
        self.cbConfLdapSmbCounterEntry.insertItem(confDict.get("%s.counterEntry" % lastConn))
    self.cbConfLdapSmbCounterEntry.insertItem("")
    self.cbConfLdapSmbProfileType.setCurrentItem(int(confDict.get("%s.profileType" % lastConn)))
    if int(confDict.get("%s.profileType" % lastConn)) == 0:
        self.cbConfLdapSmbProfilePath.setEnabled(False)
    else:
        self.cbConfLdapSmbProfilePath.setEnabled(True)
    self.cbConfLdapSmbProfilePath.setCurrentText(confDict.get("%s.profilePath" % lastConn))
    self.iConfLdapSmbHomeDrive.setText(confDict.get("%s.homeDrive" % lastConn))
    self.iConfLdapSmbDrivePath.setText(confDict.get("%s.drivePath" % lastConn))
    self.cbConfLdapSmbLogonScript.setCurrentText(confDict.get("%s.logonScript" % lastConn))
    if confDict.get("%s.pwdMustChange" % lastConn) == "True":
        self.cConfLdapSmbPwdMustChange.setChecked(True)
    else:
        self.cConfLdapSmbPwdMustChange.setChecked(False)
    self.cbConfLdapSmbPrimaryGroup.setCurrentItem(int(confDict.get("%s.primaryGroup" % lastConn)))

#
# LDAP SMB Populate Prefs
#

if confDict.get("ldap.smb.populate.1stUidNumber"):
    self.iConfLdapSMBuidNumber.setText(confDict.get("ldap.smb.populate.1stUidNumber"))

if confDict.get("ldap.smb.populate.1stGidNumber"):
    self.iConfLdapSMBgidNumber.setText(confDict.get("ldap.smb.populate.1stGidNumber"))

if confDict.get("ldap.smb.populate.sambaMinPwdLenght"):
    self.iConfLdapSMBminPwdLength.setText(confDict.get("ldap.smb.populate.sambaMinPwdLenght"))

if confDict.get("ldap.smb.populate.sambaPwdHistoryLenght"):
    self.iConfLdapSMBpwdHistLenght.setText(confDict.get("ldap.smb.populate.sambaPwdHistoryLenght"))

if confDict.get("ldap.smb.populate.sambaMaxPwdAge"):
    self.iConfLdapSMBmaxPwdAge.setText(confDict.get("ldap.smb.populate.sambaMaxPwdAge"))

if confDict.get("ldap.smb.populate.sambaMaxPwdAgeTime"):
    self.cbConfLdapSMBmaxPwdAge.setCurrentItem(int(confDict.get("ldap.smb.populate.sambaMaxPwdAgeTime")))

if confDict.get("ldap.smb.populate.sambaMinPwdAge"):
    self.iConfLdapSMBminPwdAge.setText(confDict.get("ldap.smb.populate.sambaMinPwdAge"))

if confDict.get("ldap.smb.populate.sambaMinPwdAgeTime"):
    self.cbConfLdapSMBminPwdAge.setCurrentItem(int(confDict.get("ldap.smb.populate.sambaMinPwdAgeTime")))

if confDict.get("ldap.smb.populate.sambaLockoutThreshold"):
    self.iConfLdapSMBlockout.setText(confDict.get("ldap.smb.populate.sambaLockoutThreshold"))

if confDict.get("ldap.smb.populate.sambaLockoutDuration"):
    self.iConfLdapSMBlockoutDuration.setText(confDict.get("ldap.smb.populate.sambaLockoutDuration"))

if confDict.get("ldap.smb.populate.sambaLockoutDurationTime"):
    self.cbConfLdapSMBlockoutDuration.setCurrentItem(int(confDict.get("ldap.smb.populate.sambaLockoutDurationTime")))

if confDict.get("ldap.smb.populate.sambaLockoutObservationWindow"):
    self.iConfLdapSMBlockoutWindow.setText(confDict.get("ldap.smb.populate.sambaLockoutObservationWindow"))

if confDict.get("ldap.smb.populate.sambaLockoutObservationWindowTime"):
    self.cbConfLdapSMBlockoutWindow.setCurrentItem(int(confDict.get("ldap.smb.populate.sambaLockoutObservationWindowTime")))

#
# LDAP DHCP Populate Prefs
#

if confDict.get("ldap.dhcp.populate.defaultLeaseTime"):
    self.iConfLdapDhcpDefaultLeaseTime.setText(confDict.get("ldap.dhcp.populate.defaultLeaseTime"))

if confDict.get("ldap.dhcp.populate.defaultLeaseTimeTime"):
    self.cbConfLdapDhcpDefaultLeaseTime.setCurrentItem(int(confDict.get("ldap.dhcp.populate.defaultLeaseTimeTime")))

if confDict.get("ldap.dhcp.populate.maxLeaseTime"):
    self.iConfLdapDhcpMaxLeaseTime.setText(confDict.get("ldap.dhcp.populate.maxLeaseTime"))

if confDict.get("ldap.dhcp.populate.maxLeaseTimeTime"):
    self.cbConfLdapDhcpMaxLeaseTime.setCurrentItem(int(confDict.get("ldap.dhcp.populate.maxLeaseTimeTime")))

if confDict.get("ldap.dhcp.populate.domainName"):
    self.iConfLdapDhcpDomainName.setText(confDict.get("ldap.dhcp.populate.domainName"))

if confDict.get("ldap.dhcp.populate.netbiosNameServers"):
    self.iConfLdapDhcpNetbiosServers.setText(confDict.get("ldap.dhcp.populate.netbiosNameServers"))

if confDict.get("ldap.dhcp.populate.domainNameServers"):
    self.iConfLdapDhcpDNSservers.setText(confDict.get("ldap.dhcp.populate.domainNameServers"))

if confDict.get("ldap.dhcp.populate.dhcpSharedNetwork"):
    self.cbConfLdapDhcpInterface.setCurrentText(confDict.get("ldap.dhcp.populate.dhcpSharedNetwork"))

if confDict.get("ldap.dhcp.populate.dhcpNetworkAddress"):
    self.iConfLdapDhcpNetwork.setText(confDict.get("ldap.dhcp.populate.dhcpNetworkAddress"))

if confDict.get("ldap.dhcp.populate.dhcpBroadcastAddress"):
    self.iConfLdapDhcpBroadcast.setText(confDict.get("ldap.dhcp.populate.dhcpBroadcastAddress"))

if confDict.get("ldap.dhcp.populate.dhcpRange"):
    self.iConfLdapDhcpRange.setText(confDict.get("ldap.dhcp.populate.dhcpRange"))

if confDict.get("ldap.dhcp.populate.dhcpNetmask"):
    self.cbConfLdapDhcpNetmask.setCurrentItem(int(confDict.get("ldap.dhcp.populate.dhcpNetmask")))
else:
    self.cbConfLdapDhcpNetmask.setCurrentItem(8)

if confDict.get("ldap.dhcp.populate.dhcpGateway"):
    self.iConfLdapDhcpGateway.setText(confDict.get("ldap.dhcp.populate.dhcpGateway"))

}

void dKorreio::config_save() {

confList = []

#
# LDAP Connection
#

i=0
while self.confDict.get("ldap%s.name" % i):
    if self.confDict.get("ldap%s.name" % i) == self.iLdapConnection.text().ascii():
        break
    i+=1

#
# Update selected Ldap connection before save
#

if self.iLdapConnection.text().ascii():
    confList.append("ldap.last=ldap%s" % i)
    self.confDict["ldap%s.name" % i] = self.iLdapConnection.text().ascii()
    self.confDict["ldap%s.mode" % i] = self.cbLdapMode.currentText().ascii()
    self.confDict["ldap%s.host" % i] = self.iLdapHost.text().ascii()
    self.confDict["ldap%s.port" % i] = self.iLdapPort.text().ascii()
    self.confDict["ldap%s.basedn" % i] = self.cbLdapBaseDN.currentText().ascii()
    self.confDict["ldap%s.user" % i] = self.iLdapUser.text().ascii()
    self.confDict["ldap%s.pass" % i] = self.iLdapPass.text().ascii()
    self.confDict["ldap%s.ref" % i] = str(self.cLdapRef.isChecked())
    self.confDict["ldap%s.cert" % i] = str(self.cLdapCert.isChecked())

k=0
self.cbLdapConnection.clear()
while self.confDict.get("ldap%s.name" % k):
    self.cbLdapConnection.insertItem(self.confDict.get("ldap%s.name" % k))
    for j in ['name','mode','host','port','basedn','user','pass','ref','cert']:
        opt = "ldap%s.%s" % (k, j)
        confList.append("%s=%s" % (opt, self.confDict.get(opt)))
    k+=1

self.cbLdapConnection.setCurrentItem(i)

#
# IMAP Connection
#

i=0
while self.confDict.get("imap%s.name" % i):
    if self.confDict.get("imap%s.name" % i) == self.iCyrusConnection.text().ascii():
        break
    i+=1

#
# Update selected Imap connection before save
#

if self.iCyrusConnection.text().ascii():
    confList.append("imap.last=imap%s" % i)
    self.confDict["imap%s.name" % i] = self.iCyrusConnection.text().ascii()
    self.confDict["imap%s.mode" % i] = self.cbCyrusMode.currentText().ascii()
    self.confDict["imap%s.host" % i] = self.iCyrusHost.text().ascii()
    self.confDict["imap%s.port" % i] = self.iCyrusPort.text().ascii()
    self.confDict["imap%s.sieport" % i] = self.iCyrusSievePort.text().ascii()
    self.confDict["imap%s.user" % i] = self.iCyrusUser.text().ascii()
    self.confDict["imap%s.pass" % i] = self.iCyrusPass.text().ascii()
    self.confDict["imap%s.part" % i] = self.iCyrusPart.text().ascii()

k=0
self.cbCyrusConnection.clear()
while self.confDict.get("imap%s.name" % k):
    self.cbCyrusConnection.insertItem(self.confDict.get("imap%s.name" % k))
    for j in ['name','mode','host','port','sieport','user','pass','part']:
        opt = "imap%s.%s" % (k, j)
        confList.append("%s=%s" % (opt, self.confDict.get(opt)))
    k+=1

self.cbCyrusConnection.setCurrentItem(i)

#
# SSH Connection
#

i=0
while self.confDict.get("ssh%s.name" % i):
    if self.confDict.get("ssh%s.name" % i) == self.iSSHConnection.text().ascii():
        break
    i+=1

#
# Update selected SSH connection before save
#

if self.iSSHConnection.text().ascii():
    confList.append("ssh.last=ssh%s" % i)
    self.confDict["ssh%s.name" % i] = self.iSSHConnection.text().ascii()
    self.confDict["ssh%s.host" % i] = self.iSshHost.text().ascii()
    self.confDict["ssh%s.port" % i] = self.iSshPort.text().ascii()
    self.confDict["ssh%s.user" % i] = self.iSshUser.text().ascii()
    self.confDict["ssh%s.pass" % i] = self.iSshPass.text().ascii()
    self.confDict["ssh%s.sudo" % i] = self.cbSSHsudo.currentItem()

k=0
self.cbSSHConnection.clear()
while self.confDict.get("ssh%s.name" % k):
    self.cbSSHConnection.insertItem(self.confDict.get("ssh%s.name" % k))
    for j in ['name','host','port','user','pass','sudo']:
        opt="ssh%s.%s" % (k, j)
        confList.append("%s=%s" % (opt, self.confDict.get(opt)))
    k+=1

self.cbSSHConnection.setCurrentItem(i)

#
# IMAP Prefs
#

i=0
item = self.lvConfImapFolders.firstChild()
while item is not None:
    confList.append("imap.defaultfolder%s=%s:%s:%s" % (i, item.text(0).ascii(), item.text(1).ascii(), item.text(2).ascii()))
    item = item.nextSibling()
    i+=1
confList.append("imap.defaultquota=%s" % self.iConfImapQuotaMbytes.text().ascii())

#
# LDAP Prefs
#

confList.append("ldap.admin.autocomplete=%s" % self.cbConfLdapCompleteUser.currentText().ascii())
confList.append("ldap.passwd.sambaPwdMustChange=%s" % self.cConfLdapsambaPwdMustChange.isChecked())
confList.append("ldap.entry.attributeDN=%i" % self.cbConfLdapUserDN.currentItem())
confList.append("ldap.schema.addattrs=%s" % self.cConfLdapSchemaAddAttr.isChecked())
confList.append("ldap.schema.delattrs=%s" % self.cConfLdapSchemaDelAttr.isChecked())

item=self.lvConfLdapSchema.firstChild()
item=item.firstChild()
i=0
while item is not None:
    subitem=item.firstChild()
    while subitem is not None:
        if subitem.text(1).ascii() is None: value=""
        else: value=subitem.text(1).ascii()
        confList.append("ldap.objectclass%s=%s.%s.%s" % (i, item.text(0).ascii(), subitem.text(0).ascii(), value))
        subitem=subitem.nextSibling()
        i+=1
    item=item.nextSibling()

i=0
item = self.lbConfLdapFilter.firstItem()
while item is not None:
    confList.append("ldap.filter%s=%s" % (i, item.text().ascii()))
    i+=1
    item = item.next()

#
# LDAP SMB Prefs
#

i=0
while self.confDict.get("ldap.smb%s.domain" % i):
    if self.confDict.get("ldap.smb%s.domain" % i) == self.iConfLdapSmbDomain.text().ascii():
        break
    i+=1

#
# Update selected SMB domain before save
#

if self.iConfLdapSmbDomain.text().ascii():
    confList.append("ldap.smb.last=ldap.smb%s" % i)
    self.confDict["ldap.smb%s.domain" % i] = self.iConfLdapSmbDomain.text().ascii()
    self.confDict["ldap.smb%s.SIDEntry" % i] = self.cbConfLdapSmbSIDEntry.currentText().ascii()
    self.confDict["ldap.smb%s.counterEntry" % i] = self.cbConfLdapSmbCounterEntry.currentText().ascii()
    self.confDict["ldap.smb%s.profileType" % i] = "%s" % self.cbConfLdapSmbProfileType.currentItem()
    self.confDict["ldap.smb%s.profilePath" % i] = self.cbConfLdapSmbProfilePath.currentText().ascii()
    self.confDict["ldap.smb%s.homeDrive" % i] = self.iConfLdapSmbHomeDrive.text().ascii()
    self.confDict["ldap.smb%s.drivePath" % i] = self.iConfLdapSmbDrivePath.text().ascii()
    self.confDict["ldap.smb%s.logonScript" % i] = self.cbConfLdapSmbLogonScript.currentText().ascii()
    self.confDict["ldap.smb%s.pwdMustChange" % i] = "%s" % self.cConfLdapSmbPwdMustChange.isChecked()
    self.confDict["ldap.smb%s.primaryGroup" % i] = self.cbConfLdapSmbPrimaryGroup.currentItem()

k=0
self.cbConfLdapSmbDomain.clear()
while self.confDict.get("ldap.smb%s.domain" % k):
    self.cbConfLdapSmbDomain.insertItem(self.confDict.get("ldap.smb%s.domain" % k))
    for j in ['domain', 'SIDEntry', 'counterEntry', 'profileType', 'profilePath', 'homeDrive', 'drivePath', 'logonScript', 'pwdMustChange', 'primaryGroup']:
        opt = "ldap.smb%s.%s" % (k, j)
        confList.append("%s=%s" % (opt, self.confDict.get(opt)))
    k+=1

self.cbConfLdapSmbDomain.setCurrentItem(i)

#
# LDAP SMB Populate Prefs
#

confList.append("ldap.smb.populate.1stUidNumber=%s" % self.iConfLdapSMBuidNumber.text().ascii())
confList.append("ldap.smb.populate.1stGidNumber=%s" % self.iConfLdapSMBgidNumber.text().ascii())
confList.append("ldap.smb.populate.sambaMinPwdLenght=%s" % self.iConfLdapSMBminPwdLength.text().ascii())
confList.append("ldap.smb.populate.sambaPwdHistoryLenght=%s" % self.iConfLdapSMBpwdHistLenght.text().ascii())
confList.append("ldap.smb.populate.sambaMaxPwdAge=%s" % self.iConfLdapSMBmaxPwdAge.text().ascii())
confList.append("ldap.smb.populate.sambaMaxPwdAgeTime=%s" % self.cbConfLdapSMBmaxPwdAge.currentItem())
confList.append("ldap.smb.populate.sambaMinPwdAge=%s" % self.iConfLdapSMBminPwdAge.text().ascii())
confList.append("ldap.smb.populate.sambaMinPwdAgeTime=%s" % self.cbConfLdapSMBminPwdAge.currentItem())
confList.append("ldap.smb.populate.sambaLockoutThreshold=%s" % self.iConfLdapSMBlockout.text().ascii())
confList.append("ldap.smb.populate.sambaLockoutDuration=%s" % self.iConfLdapSMBlockoutDuration.text().ascii())
confList.append("ldap.smb.populate.sambaLockoutDurationTime=%s" % self.cbConfLdapSMBlockoutDuration.currentItem())
confList.append("ldap.smb.populate.sambaLockoutObservationWindow=%s" % self.iConfLdapSMBlockoutWindow.text().ascii())
confList.append("ldap.smb.populate.sambaLockoutObservationWindowTime=%s" % self.cbConfLdapSMBlockoutWindow.currentItem())

#
# LDAP DHCP Populate Prefs
#

confList.append("ldap.dhcp.populate.defaultLeaseTime=%s" % self.iConfLdapDhcpDefaultLeaseTime.text().ascii())
confList.append("ldap.dhcp.populate.defaultLeaseTimeTime=%s" % self.cbConfLdapDhcpDefaultLeaseTime.currentItem())
confList.append("ldap.dhcp.populate.maxLeaseTime=%s" % self.iConfLdapDhcpMaxLeaseTime.text().ascii())
confList.append("ldap.dhcp.populate.maxLeaseTimeTime=%s" % self.cbConfLdapDhcpMaxLeaseTime.currentItem())
confList.append("ldap.dhcp.populate.domainName=%s" % self.iConfLdapDhcpDomainName.text().ascii())
confList.append("ldap.dhcp.populate.netbiosNameServers=%s" % self.iConfLdapDhcpNetbiosServers.text().ascii())
confList.append("ldap.dhcp.populate.domainNameServers=%s" % self.iConfLdapDhcpDNSservers.text().ascii())
confList.append("ldap.dhcp.populate.dhcpSharedNetwork=%s" % self.cbConfLdapDhcpInterface.currentText().ascii())
confList.append("ldap.dhcp.populate.dhcpNetworkAddress=%s" % self.iConfLdapDhcpNetwork.text().ascii())
confList.append("ldap.dhcp.populate.dhcpBroadcastAddress=%s" % self.iConfLdapDhcpBroadcast.text().ascii())
confList.append("ldap.dhcp.populate.dhcpRange=%s" % self.iConfLdapDhcpRange.text().ascii())
confList.append("ldap.dhcp.populate.dhcpNetmask=%s" % self.cbConfLdapDhcpNetmask.currentItem())
confList.append("ldap.dhcp.populate.dhcpGateway=%s" % self.iConfLdapDhcpGateway.text().ascii())

#
# Save configuration
#

try:
    f = open(os.path.expanduser("~/.korreio/korreio.conf"), 'w')
    f.write("\n".join(confList))
    f.write("\n")
    f.close()
    os.chmod(os.path.expanduser("~/.korreio/korreio.conf"), 0600)
    self.console(self.__tr("Configuration saved."))
except OSError, e:
    self.parse_exception("OSError", e)

self.korreio_update_servers_menu()

}

void dKorreio::config_sambaDomains() {

i=0
domains = []
while self.confDict.get("ldap.smb%s.domain" % i):
    domains.append(self.confDict.get("ldap.smb%s.domain" % i))
    i+=1
domains.sort()
return domains

}

void dKorreio::config_sambaDomain_getConf( a0 ) {

domain = a0

i=0
while self.confDict.get("ldap.smb%s.domain" % i):
    if self.confDict.get("ldap.smb%s.domain" % i) == domain:
        break
    i+=1

if not self.confDict.get("ldap.smb%s.domain" % i):
    return None, None

SIDEntry = unicode(self.confDict["ldap.smb%s.SIDEntry" % i], 'iso-8859-1')
counterEntry = unicode(self.confDict["ldap.smb%s.counterEntry" % i], 'iso-8859-1')

return SIDEntry, counterEntry

}

void dKorreio::config_log_mode() {

self.lbLog.clear()
try:
    if os.path.isfile(os.path.expanduser("~/.korreio/korreio.log")):
        f = open(os.path.expanduser("~/.korreio/korreio.log"), 'r')
        if self.rbConfLogModeRecent.isChecked():
            f.seek(-1000, 2)
        for line in f.xreadlines():
            line=line.strip('\n')
            try:
                self.lbLog.insertItem(utf2uni(line))
            except:
                pass
        f.close()
        if self.rbConfLogModeRecent.isChecked():
            self.lbLog.removeItem(0)
        self.lbLog.setCurrentItem(self.lbLog.count() - 1)
        self.lbLog.ensureCurrentVisible()
except:
    pass

}

void dKorreio::config_change_widgetstack() {

self.pSaveConfig.setEnabled(True)

#
# Set servers page as first page view
#

try:
    self.config1st
except:
    item = self.lvConfig.firstChild()
    while item is not None:
        if item.text(1).ascii() == "2":
            self.lvConfig.setCurrentItem(item)
            break
        item = item.nextSibling()
    self.config1st = 1

#
# Show selected configuration
#

item=self.lvConfig.currentItem()

if  item.text(1).ascii() == "2":
    #
    # Servers
    #
    self.wsConfig.raiseWidget(3)
    self.tConfShowLdapServer.setText(self.__tr("Host: %1").arg("%s%s:%s" % (self.cbLdapMode.currentText().ascii(), self.iLdapHost.text().ascii(), self.iLdapPort.text().ascii())))
    self.tConfShowLdapUser.setText(self.__tr("User: %1").arg(self.iLdapUser.text().ascii()))
    self.tConfShowLdapBaseDN.setText(self.__tr("Base: %1").arg(self.cbLdapBaseDN.currentText().ascii()))
    self.tConfShowImapServer.setText(self.__tr("Host: %1").arg("%s%s:%s" % (self.cbCyrusMode.currentText().ascii(), self.iCyrusHost.text().ascii(), self.iCyrusPort.text().ascii())))
    self.tConfShowImapUser.setText(self.__tr("User: %1").arg(self.iCyrusUser.text().ascii()))
    self.tConfShowSshServer.setText(self.__tr("Host: %1").arg("ssh://%s:%s" % (self.iSshHost.text().ascii(), self.iSshPort.text().ascii())))
    self.tConfShowSshUser.setText(self.__tr("User: %1").arg(self.iSshUser.text().ascii()))

elif  item.text(1).ascii() == "2.1":
    #
    # Servers LDAP
    #
    self.wsConfig.raiseWidget(0)

elif  item.text(1).ascii() == "2.2":
    #
    # Servers IMAP
    #
    self.wsConfig.raiseWidget(1)

elif  item.text(1).ascii() == "2.2.1":
    #
    # IMAP Annotation
    #
    self.wsConfig.raiseWidget(10)
    self.pSaveConfig.setEnabled(False)
    self.cbImapAnnotationServer.setCurrentItem(0)
    self.iAnnotationValueServer.clear()

elif  item.text(1).ascii() == "2.3":
    #
    # Servers SSH
    #
    self.wsConfig.raiseWidget(2)

elif  item.text(1).ascii() == "4.1":
    #
    # Preferences LDAP
    #
    self.wsConfig.raiseWidget(4)

elif  item.text(1).ascii() == "4.2":
    #
    # Preferences IMAP
    #
    self.wsConfig.raiseWidget(5)

elif  item.text(1).ascii() == "4.1.1":
    #
    # Preferences LDAP DHCP Populate
    #
    self.wsConfig.raiseWidget(11)

elif  item.text(1).ascii() == "4.1.2":
    #
    # Preferences LDAP SMB Populate
    #
    self.wsConfig.raiseWidget(7)

elif  item.text(1).ascii() == "4.1.3":
    #
    # Preferences LDAP SMB
    #
    self.wsConfig.raiseWidget(6)

elif  item.text(1).ascii() == "4.1.4":
    #
    # Preferences LDAP Schema
    #
    self.wsConfig.raiseWidget(8)


}

void dKorreio::config_imap_set_port() {

#
# Imap mode was changed
#

if self.cbCyrusMode.currentItem() == 0:
    self.iCyrusPort.setText("143")
else:
    self.iCyrusPort.setText("993")

}

void dKorreio::config_ldap_set_port() {

#
# Ldap mode was changed
#

if self.cbLdapMode.currentItem() == 0:
    self.iLdapPort.setText("389")
else:
    self.iLdapPort.setText("636")

}

void dKorreio::config_ldap_get_basedn() {

#
# Detect BaseDN
#

self.cbLdapBaseDN.clear()
dnDict = self.ldap_query("objectclass=*", ["namingContexts"], "", ldap.SCOPE_BASE)
if dnDict is not None:
    if dnDict.get('') is None:
        self.console(self.__tr("BaseDN is not available, you must enter it."), self.CONSOLE_WARN)
    elif dnDict.get('').get('namingContexts'):
        for base in dnDict.get('').get('namingContexts'):
            self.cbLdapBaseDN.insertItem(base)
        if self.cbLdapBaseDN.count() > 1:
            self.cbLdapBaseDN.popup()
        elif self.cbLdapBaseDN.count() == 1:
            self.config_ldap_set_admin()

}

void dKorreio::config_ldap_set_admin() {

#
# Set Ldap user if BaseDN was changed
#

self.iLdapUser.setText("%s,%s" % (self.cbConfLdapCompleteUser.currentText().ascii(), self.cbLdapBaseDN.currentText().ascii()))

}

void dKorreio::config_ldap_schema() {

item = self.lvConfLdapSchema.currentItem()
if item.parent() is None:
    self.iConfLdapSchemaValue.setEnabled(False)
    self.pConfLdapSchemaDelItem.setEnabled(False)
    self.iConfLdapSchemaAttr.clear()
    self.iConfLdapSchemaValue.clear()
else:
    self.iConfLdapSchemaValue.setEnabled(True)
    self.pConfLdapSchemaDelItem.setEnabled(True)
    if item.parent().parent() is None:
        self.iConfLdapSchemaAttr.clear()
        self.iConfLdapSchemaValue.clear()
    else:
        if item.text(0).ascii() is not None:
            self.iConfLdapSchemaAttr.setText(item.text(0).ascii())
        if item.text(1).ascii() is not None:
            self.iConfLdapSchemaValue.setText(item.text(1).ascii())

}

void dKorreio::config_ldap_schema_checked() {

#
# Disable auxiliary schema if not in use
#

if not self.cConfLdapSchemaAddAttr.isChecked() and not self.cConfLdapSchemaDelAttr.isChecked():
    self.fConfLdapSchema.setEnabled(False)
else:
    self.fConfLdapSchema.setEnabled(True)

}

void dKorreio::config_ldap_schema_add_attr() {

if not self.iConfLdapSchemaAttr.text().ascii():
    self.console(self.__tr("set objectClass/attribute."), self.CONSOLE_WARN)
    self.iConfLdapSchemaAttr.setFocus()
    return True

item = self.lvConfLdapSchema.currentItem()

if item.parent() is None or item.parent().parent() is None:
    item.setOpen(True)
    newitem = QListViewItem(item)
else:
    if self.iConfLdapSchemaAttr.text().ascii() == item.text(0).ascii():
        item.setText(1, self.iConfLdapSchemaValue.text().ascii())
        return True
    newitem = QListViewItem(item.parent())

newitem.setText(0, self.iConfLdapSchemaAttr.text().ascii())
newitem.setText(1, self.iConfLdapSchemaValue.text().ascii())

}

void dKorreio::config_ldap_schema_del_attr() {

item = self.lvConfLdapSchema.currentItem()

if item.parent() is not None:
    item.parent().takeItem(item)
    self.lvConfLdapSchema.currentItem().setSelected(True)

}

void dKorreio::config_ldap_smb_perfil_changed() {

#
# Disable Profile Path if is profile is local
#

if self.cbConfLdapSmbProfileType.currentItem() == 0:
    self.cbConfLdapSmbProfilePath.setEnabled(False)
else:
    self.cbConfLdapSmbProfilePath.setEnabled(True)

}

void dKorreio::config_ldap_smb_get_sambaUnixIdPool() {

#
# Search uid counters entries
#

self.cbConfLdapSmbCounterEntry.clear()
self.cbConfLdapSmbCounterEntry.insertItem("")
ldap_result = self.ldap_query("objectClass=sambaUnixIdPool")
if ldap_result is not None:
    for dn in ldap_result:
        self.cbConfLdapSmbCounterEntry.insertItem(dn)

if self.cbConfLdapSmbCounterEntry.count() > 1:
    self.cbConfLdapSmbCounterEntry.popup()

}

void dKorreio::config_ldap_smb_get_sambaDomain() {

#
# Search Samba domain/SID entries
#

self.cbConfLdapSmbSIDEntry.clear()
self.cbConfLdapSmbSIDEntry.insertItem("")
ldap_result = self.ldap_query("objectClass=sambaDomain")
if ldap_result is not None:
    for dn in ldap_result:
        self.cbConfLdapSmbSIDEntry.insertItem(dn)

if self.cbConfLdapSmbSIDEntry.count() > 1:
    self.cbConfLdapSmbSIDEntry.popup()

}

void dKorreio::config_ldap_smb_sambaDomain_clicked() {

#
# Set domain name based on sambaDomain entry
#

SIDdn = self.cbConfLdapSmbSIDEntry.currentText().ascii()
if SIDdn:
    self.iConfLdapSmbDomain.setText(SIDdn.split(",")[0].split("=")[1].lower())
else:
    self.iConfLdapSmbDomain.clear()

}

void dKorreio::config_imap_add_default_folder() {

#
# Add folder to creation with user imap mailbox
#

if not self.iConfImapFolder.text().ascii():
    return False

item = self.lvConfImapFolders.firstChild()
while item is not None:
    if item.text(0).ascii() == self.iConfImapFolder.text().ascii():
        tmp = item.nextSibling()
        self.lvConfImapFolders.takeItem(item)
        item = tmp
    else:
        item = item.nextSibling()

item = QListViewItem(self.lvConfImapFolders)
item.setPixmap(0, self.icons.get("folder.png"))
item.setText(0, self.iConfImapFolder.text().ascii())
item.setText(1, self.iConfImapExpire.text().ascii())
if self.cbConfImapACLp.currentItem() == 1:
    item.setText(2, "p")

}

void dKorreio::config_imap_del_default_folder() {

#
# Del folder to creation with user imap mailbox
#

self.lvConfImapFolders.takeItem(self.lvConfImapFolders.currentItem())

}

void dKorreio::config_ldap_set_connection() {

#
# Get selected LDAP server
#

i=0
while self.confDict.get("ldap%s.name" % i):
    if self.confDict.get("ldap%s.name" % i) == self.cbLdapConnection.currentText().ascii():
        lastConn="ldap%s" % i
    i+=1

self.iLdapConnection.setText(self.confDict.get("%s.name" % lastConn))
self.cbLdapMode.setCurrentText(self.confDict.get("%s.mode" % lastConn))
self.iLdapHost.setText(self.confDict.get("%s.host" % lastConn))
self.iLdapPort.setText(self.confDict.get("%s.port" % lastConn))
self.cbLdapBaseDN.clear()
self.cbLdapBaseDN.insertItem(self.confDict.get("%s.basedn" % lastConn))
self.iLdapUser.setText(self.confDict.get("%s.user" % lastConn))
self.iLdapPass.setText(self.confDict.get("%s.pass" % lastConn))
if self.confDict.get("%s.ref" % lastConn) == "True":
    self.cLdapRef.setChecked(True)
else:
    self.cLdapRef.setChecked(False)
if self.confDict.get("%s.cert" % lastConn) == "True":
    self.cLdapCert.setChecked(True)
else:
    self.cLdapCert.setChecked(False)

self.korreio_module_clear("ldap")
self.korreio_update_servers_menu()

}

void dKorreio::config_imap_set_connection() {

#
# Get selected IMAP server
#

i=0
while self.confDict.get("imap%s.name" % i):
    if self.confDict.get("imap%s.name" % i) == self.cbCyrusConnection.currentText().ascii():
        lastConn="imap%s" % i
    i+=1

self.iCyrusConnection.setText(self.confDict.get("%s.name" % lastConn))
self.cbCyrusMode.setCurrentText(self.confDict.get("%s.mode" % lastConn))
self.iCyrusHost.setText(self.confDict.get("%s.host" % lastConn))
self.iCyrusPort.setText(self.confDict.get("%s.port" % lastConn))
self.iCyrusSievePort.setText(self.confDict.get("%s.sieport" % lastConn))
self.iCyrusUser.setText(self.confDict.get("%s.user" % lastConn))
self.iCyrusPass.setText(self.confDict.get("%s.pass" % lastConn))
self.iCyrusPart.setText(self.confDict.get("%s.part" % lastConn))

self.korreio_module_clear("imap")
self.korreio_module_clear("imap-partition")
self.korreio_module_clear("sieve")
self.korreio_update_servers_menu()

}

void dKorreio::config_ssh_set_connection() {

#
# Get selected SSH server
#

i=0
while self.confDict.get("ssh%s.name" % i):
    if self.confDict.get("ssh%s.name" % i) == self.cbSSHConnection.currentText().ascii():
        lastConn="ssh%s" % i
    i+=1
self.iSSHConnection.setText(self.confDict.get("%s.name" % lastConn))
self.iSshHost.setText(self.confDict.get("%s.host" % lastConn))
self.iSshPort.setText(self.confDict.get("%s.port" % lastConn))
self.iSshUser.setText(self.confDict.get("%s.user" % lastConn))
self.iSshPass.setText(self.confDict.get("%s.pass" % lastConn))
try:
    self.cbSSHsudo.setCurrentItem(int(self.confDict.get("%s.sudo" % lastConn)))
except:
    self.cbSSHsudo.setCurrentItem(0)

self.korreio_module_clear("ssh")
self.korreio_update_servers_menu()

}

void dKorreio::config_smb_set_domain() {

#
# Get selected Samba Domain
#

i=0
while self.confDict.get("ldap.smb%s.domain" % i):
    if self.confDict.get("ldap.smb%s.domain" % i) == self.cbConfLdapSmbDomain.currentText().ascii():
        lastConn = "ldap.smb%s" % i
        break
    i+=1

self.iConfLdapSmbDomain.setText(self.confDict.get("%s.domain" % lastConn))

self.cbConfLdapSmbSIDEntry.clear()
self.cbConfLdapSmbSIDEntry.insertItem(self.confDict.get("%s.SIDEntry" % lastConn))
self.cbConfLdapSmbSIDEntry.insertItem("")

self.cbConfLdapSmbCounterEntry.clear()
if self.confDict.get("%s.counterEntry" % lastConn) != "None":
    self.cbConfLdapSmbCounterEntry.insertItem(self.confDict.get("%s.counterEntry" % lastConn))
self.cbConfLdapSmbProfileType.setCurrentItem(int(self.confDict.get("%s.profileType" % lastConn)))
if int(self.confDict.get("%s.profileType" % lastConn)) == 0:
    self.cbConfLdapSmbProfilePath.setEnabled(False)
else:
    self.cbConfLdapSmbProfilePath.setEnabled(True)
self.cbConfLdapSmbProfilePath.setCurrentText(self.confDict.get("%s.profilePath" % lastConn))
self.iConfLdapSmbHomeDrive.setText(self.confDict.get("%s.homeDrive" % lastConn))
self.iConfLdapSmbDrivePath.setText(self.confDict.get("%s.drivePath" % lastConn))
self.cbConfLdapSmbLogonScript.setCurrentText(self.confDict.get("%s.logonScript" % lastConn))
if self.confDict.get("%s.pwdMustChange" % lastConn) == "True":
    self.cConfLdapSmbPwdMustChange.setChecked(True)
else:
    self.cConfLdapSmbPwdMustChange.setChecked(False)
self.cbConfLdapSmbPrimaryGroup.setCurrentItem(int(self.confDict.get("%s.primaryGroup" % lastConn)))

}

void dKorreio::config_ldap_del_connection() {

#
# Delete selected Ldap server
#

if not self.cbLdapConnection.currentText().ascii():
    return True

i=0
while self.confDict.get("ldap%s.name" % i):
    if self.confDict.get("ldap%s.name" % i) == self.cbLdapConnection.currentText().ascii():
        j=i+1
        while self.confDict.get("ldap%s.name" % j):
            for opt in ['name','mode','host','port','basedn','user','pass','ref','cert']:
                self.confDict["ldap%s.%s" % ((j-1), opt)]=self.confDict.get("ldap%s.%s" % (j, opt))
            j+=1
    i+=1

for opt in ['name','mode','host','port','basedn','user','pass','ref','cert']:
    del self.confDict["ldap%s.%s" % ((i-1), opt)]

i=self.cbLdapConnection.currentItem()
self.cbLdapConnection.removeItem(i)
if i > 0:
    self.cbLdapConnection.setCurrentItem(i-1)
    self.config_ldap_set_connection()
elif self.cbLdapConnection.count() > 0:
    self.cbLdapConnection.setCurrentItem(0)
    self.config_ldap_set_connection()
else:
    self.iLdapConnection.clear()
    self.cbLdapMode.setCurrentText("ldap://")
    self.iLdapHost.clear()
    self.iLdapPort.setText("389")
    self.cbLdapBaseDN.clear()
    self.iLdapUser.clear()
    self.iLdapPass.clear()
    self.cLdapRef.setChecked(False)
    self.cLdapCert.setChecked(False)

}

void dKorreio::config_imap_del_connection() {

#
# Delete selected Imap server
#

if not self.cbCyrusConnection.currentText().ascii():
    return True

i=0
while self.confDict.get("imap%s.name" % i):
    if self.confDict.get("imap%s.name" % i) == self.cbCyrusConnection.currentText().ascii():
        j=i+1
        while self.confDict.get("imap%s.name" % j):
            for opt in ['name','mode','host','port','sieport','user','pass','part']:
                self.confDict["imap%s.%s" % ((j-1), opt)]=self.confDict.get("imap%s.%s" % (j, opt))
            j+=1
    i+=1

for opt in ['name','mode','host','port','sieport','user','pass','part']:
    del self.confDict["imap%s.%s" % ((i-1), opt)]

i=self.cbCyrusConnection.currentItem()
self.cbCyrusConnection.removeItem(i)
if i > 0:
    self.cbCyrusConnection.setCurrentItem(i-1)
    self.config_imap_set_connection()
elif self.cbCyrusConnection.count() > 0:
    self.cbCyrusConnection.setCurrentItem(0)
    self.config_imap_set_connection()
else:
    self.iCyrusConnection.clear()
    self.cbCyrusMode.setCurrentText("imap://")
    self.iCyrusHost.clear()
    self.iCyrusPort.setText("143")
    self.iCyrusSievePort.setText("2000")
    self.iCyrusUser.clear()
    self.iCyrusPass.clear()
    self.iCyrusPart.clear()

}

void dKorreio::config_ssh_del_connection() {

#
# Delete selected SSH server
#

if not self.cbSSHConnection.currentText().ascii():
    return True
i=0
while self.confDict.get("ssh%s.name" % i):
    if self.confDict.get("ssh%s.name" % i) == self.cbSSHConnection.currentText().ascii():
        j=i+1
        while self.confDict.get("ssh%s.name" % j):
            for opt in ['name','host','port','user','pass']:
                self.confDict["ssh%s.%s" % ((j-1), opt)]=self.confDict.get("ssh%s.%s" % (j, opt))
            j+=1
    i+=1

for opt in ['name','host','port','user','pass']:
    del self.confDict["ssh%s.%s" % ((i-1), opt)]

i=self.cbSSHConnection.currentItem()
self.cbSSHConnection.removeItem(i)
if i > 0:
    self.cbSSHConnection.setCurrentItem(i-1)
    self.config_ssh_set_connection()
elif self.cbSSHConnection.count() > 0:
    self.cbSSHConnection.setCurrentItem(0)
    self.config_ssh_set_connection()
else:
    self.iSSHConnection.clear()
    self.iSshHost.clear()
    self.iSshPort.setText("22")
    self.iSshUser.clear()
    self.iSshPass.clear()

}

void dKorreio::config_smb_del_domain() {

#
# Delete selected Samba Domain
#

if not self.cbConfLdapSmbDomain.currentText().ascii():
    return True
i=0
while self.confDict.get("ldap.smb%s.domain" % i):
    if self.confDict.get("ldap.smb%s.domain" % i) == self.cbConfLdapSmbDomain.currentText().ascii():
        j=i+1
        while self.confDict.get("ldap.smb%s.domain" % j):
            for opt in['domain', 'SIDEntry', 'counterEntry', 'profileType', 'profilePath', 'homeDrive', 'drivePath', 'logonScript', 'pwdMustChange', 'primaryGroup']:
                self.confDict["ldap.smb%s.%s" % ((j-1), opt)]=self.confDict.get("ldap.smb%s.%s" % (j, opt))
            j+=1
    i+=1

for opt in ['domain', 'SIDEntry', 'counterEntry', 'profileType', 'profilePath', 'homeDrive', 'drivePath', 'logonScript', 'pwdMustChange', 'primaryGroup']:
    del self.confDict["ldap.smb%s.%s" % ((i-1), opt)]

i=self.cbConfLdapSmbDomain.currentItem()
self.cbConfLdapSmbDomain.removeItem(i)
if i > 0:
    self.cbConfLdapSmbDomain.setCurrentItem(i-1)
    self.config_smb_set_domain()
elif self.cbConfLdapSmbDomain.count() > 0:
    self.cbConfLdapSmbDomain.setCurrentItem(0)
    self.config_smb_set_domain()
else:
    self.cbConfLdapSmbDomain.clear()
    self.iConfLdapSmbDomain.clear()
    self.cbConfLdapSmbSIDEntry.clear()
    self.cbConfLdapSmbCounterEntry.clear()
    self.cbConfLdapSmbProfileType.setCurrentItem(0)
    self.cbConfLdapSmbProfilePath.setEnabled(False)
    self.cbConfLdapSmbProfilePath.setCurrentText("\\server\profiles\#UID#")
    self.iConfLdapSmbHomeDrive.setText("Z:")
    self.iConfLdapSmbDrivePath.setText("\\server\#UID#")
    self.cbConfLdapSmbLogonScript.setCurrentText("netlogon.bat")
    self.cConfLdapSmbPwdMustChange.setChecked(True)
    self.cbConfLdapSmbPrimaryGroup.setCurrentItem(0)

}

void dKorreio::config_ldap_filter_add() {

#
# Add ldap filter and update Ldap module
#

self.lbConfLdapFilter.insertItem(self.iConfLdapFilter.text().ascii())

self.config_ldap_filter_sync()

}

void dKorreio::config_ldap_filter_del() {

#
# Delete selected ldap filter and update Ldap module
#

for filter in range(self.lbConfLdapFilter.count()-1, -1, -1):
    if self.lbConfLdapFilter.item(filter).isSelected():
        self.lbConfLdapFilter.removeItem(filter)

self.config_ldap_filter_sync()
  
}

void dKorreio::config_ldap_filter_sync() {

#
# Update Ldap module after change ldap filters
#

self.cbLdapFilter.clear()
for filter in range(0, self.lbConfLdapFilter.count()):
    self.cbLdapFilter.insertItem(self.lbConfLdapFilter.item(filter).text().ascii())

}

void dKorreio::imap_connect() {

#
# Verify IMAP connection cache
#

try:
    if self.m:
        if self.inet_cache.get("imap.mode") == self.cbCyrusMode.currentText().ascii() and self.inet_cache.get("imap.host") == self.iCyrusHost.text().ascii() and self.inet_cache.get("imap.port") == self.iCyrusPort.text().ascii() and self.inet_cache.get("imap.user") == self.iCyrusUser.text().ascii() and self.inet_cache.get("imap.pass") == self.iCyrusPass.text().ascii():
            if self.m.m.isadmin():
                return self.m
except:
    pass


#
# Connect to IMAP
#

try:
    if self.cbCyrusMode.currentItem() == 0:
        mode = "imap"
    else:
        mode = "imaps"
    server = "%s://%s:%s" % (mode, self.iCyrusHost.text().ascii(), self.iCyrusPort.text().ascii())
    self.m = cyruslib.CYRUS(server)
    self.m.login(self.iCyrusUser.text().ascii(), self.iCyrusPass.text().ascii())
    self.m.setEncoding('iso-8859-1')
    self.m.setNormalize(True)
    self.console(self.__tr("%1 connected.").arg("%s/%s" % (server, self.iCyrusUser.text().ascii())))
except cyruslib.CYRUSError, e:
    try:        del self.m
    except:  pass
    self.parse_exception("IMAPError", e)
    return None

#
# Save current connection information
#

self.inet_cache["imap.mode"] = self.cbCyrusMode.currentText().ascii()
self.inet_cache["imap.host"] = self.iCyrusHost.text().ascii()
self.inet_cache["imap.port"] = self.iCyrusPort.text().ascii()
self.inet_cache["imap.user"] = self.iCyrusUser.text().ascii()
self.inet_cache["imap.pass"] = self.iCyrusPass.text().ascii()

self.tConfShowImapSep.setText(self.__tr("Imap delimiter: %1").arg(self.m.SEP))

return self.m

}

void dKorreio::imap_current_mailbox( a0 ) {
# a0: str:SEP (Imap Delimiter)

#
# Get selected mailbox
#

SEP = a0

item = self.lvImap.currentItem()
if item is None or not item.text(0).ascii():
    return None

mailbox = item.text(0).ascii()
while item.parent() is not None:
    mailbox = "%s%s%s" % (item.parent().text(0).ascii(), SEP, mailbox)
    item = item.parent()

if self.rbImapMailboxMode.isChecked():
    mailbox = "user%s%s" % (SEP, mailbox)

return mailbox

}

void dKorreio::imap_search() {

imap = self.imap_connect()
if imap is None:
    return False

#
# Save current selection
#

try:
    oldMailbox = re.sub("^user%s" % imap.SEP, "", self.imap_current_mailbox(imap.SEP))
except:
    oldMailbox = None
self.korreio_module_clear("imap")

#
# Imap query
#

textSearch = self.iImapSearch.text().latin1()

if self.rbImapMailboxMode.isChecked():
    if textSearch:
        textSearch = textSearch.replace("*", "")
        if textSearch.find("@") != -1:
            query = "user%s*%s" % (imap.SEP, textSearch)
        else:
            query = "user%s%s*" % (imap.SEP, textSearch)
    else:
        query = "user%s*" % imap.SEP
else:
    query = "*"

mailbox = imap.lm(query)

#
# Select global folders (query not supported)
#

if self.rbImapMailboxMode2.isChecked():
    globalList = []
    for mbx in mailbox:
        if re.search("^user%s" % imap.SEP, mbx):
            continue
        if textSearch:
            if textSearch.find("@") == 0 and re.search(textSearch, mbx):
                globalList.append(mbx)
            elif re.search("^%s" % textSearch, mbx):
                globalList.append(mbx)
        else:
            globalList.append(mbx)
    mailbox = globalList

#
# Create Tree Item
#
def __create_item(mbx):
    if self.imap_cache_items.get(mbx):
        return True
    mbxList = mbx.split(imap.SEP)
    if len(mbxList) == 1:
        self.imap_cache_items[mbx] = QListViewItemImap(self.lvImap)
        self.imap_cache_items[mbx].setText(0, mbx)
        self.imap_cache_items[mbx].setPixmap(0, self.icons.get("folder.png"))
        return True
    mbxParent = imap.SEP.join(mbxList[:-1])
    if not self.imap_cache_items.get(mbxParent):
        self.console(self.__tr("sub-folder '%1' listed before folder '%2'.").arg(mbx).arg(mbxParent), self.CONSOLE_WARN)
        __create_item(mbxParent)
    self.imap_cache_items[mbx] = QListViewItem(self.imap_cache_items.get(mbxParent))
    self.imap_cache_items[mbx].setText(0, mbxList[-1])
    self.imap_cache_items[mbx].setPixmap(0, self.icons.get("folder.png"))

self.imap_cache_items = {}
for mbx in mailbox:
    mbx = re.sub("^user%s" % imap.SEP, "", mbx)
    __create_item(mbx)

#
# Set last selection
#

if self.imap_cache_items.get(oldMailbox):
        self.lvImap.setCurrentItem(self.imap_cache_items.get(oldMailbox))
        self.lvImap.ensureItemVisible(self.imap_cache_items.get(oldMailbox))
        self.imap_cache_items[oldMailbox].setSelected(True)
        while self.imap_cache_items.get(oldMailbox).parent() is not None:
            self.imap_cache_items[oldMailbox].parent().setOpen(True)
            oldMailbox = imap.SEP.join(oldMailbox.split(imap.SEP)[:-1])
elif self.lvImap.childCount() > 0:
        item = self.lvImap.firstChild()
        self.lvImap.setCurrentItem(item)
        self.lvImap.ensureItemVisible(item)
        item.setSelected(True)

#
# Get selected mailbox information
#

self.imap_mailbox_clicked(None, imap)
self.lvImap.setFocus()

}

void dKorreio::imap_mailbox_clicked() {
# a0=obj:QListViewItem or None, a1=obj:imap or None

self.wsImap.raiseWidget(0)

imap = a1

if imap is None:
    imap = self.imap_connect()
    if imap is None:
        return False

#
# Mailbox Path
#

mailbox = self.imap_current_mailbox(imap.SEP)
if mailbox is None:
    return True

mbxtop = re.sub("^user%s" % imap.SEP, "", mailbox)
self.iImapMailbox.setText(mbxtop)

#
# Quota
#

if len(mbxtop.split(imap.SEP)) == 1:
    self.pImapQuota.setEnabled(True)
else:
    self.pImapQuota.setEnabled(False)

if re.search("^user%s" % imap.SEP, mailbox):
    mbxtop = "user%s%s" % (imap.SEP, mailbox.split(imap.SEP)[1])
else:
    mbxtop = mailbox.split(imap.SEP)[0]

try:
    quota = imap.lq(mbxtop)
except:
    quota = (0, 0)
self.iQuotaUsed.setText("%s" % quota[0])
self.iQuota.setText("%s" % quota[1])

#
# ACL
#

self.lvImapAcl.clear()

try:
    acls = imap.lam(mailbox)
except:
    return False

for user, acl in acls.items():
    item = QListViewItem(self.lvImapAcl)
    item.setText(0, user)
    item.setPixmap(0, self.icons.get("lock_overlay.png"))
    item.setText(1, acl)

item = self.lvImapAcl.firstChild()
if item is not None:
    self.lvImapAcl.setCurrentItem(item)
    item.setSelected(False)
    self.imap_acl_clicked(item)

#
# Annotation
#

self.imap_annotation_user_get(None, imap)

}

void dKorreio::imap_add() {

def __imap_add(mailbox, partition=None):
    mbxList = mailbox.split(imap.SEP)
    if mbxList[0] == "user":
        mbxItem = imap.SEP.join(mbxList[1:])
        mbxParent = None
        if len(mbxList) != 2:
            mbxParent = imap.SEP.join(mbxList[1:-1])
            if not self.imap_cache_items.get(mbxParent):
                self.console(self.__tr("parent mailbox '%1' don't exist.").arg(mbxParent), self.CONSOLE_ERR)
                return False
    else:
        mbxItem = mailbox
        if len(mbxList) == 1:
            mbxParent = None
        else:
            mbxParent = imap.SEP.join(mbxList[:-1])

    imap.cm(mailbox, partition)
    if mbxParent is None:
        self.imap_cache_items[mbxItem] = QListViewItemImap(self.lvImap)
    else:
        self.imap_cache_items[mbxItem] = QListViewItem(self.imap_cache_items.get(mbxParent))
        self.imap_cache_items[mbxParent].setOpen(True)
    self.imap_cache_items[mbxItem].setPixmap(0, self.icons.get("folder.png"))
    self.imap_cache_items[mbxItem].setText(0, mbxList[-1])
    self.console(self.__tr("mailbox '%1' created.").arg(mailbox))

mailbox = re.sub("^ *", "", "%s" % self.iImapMailbox.text().ascii())

if not mailbox:
    self.console(self.__tr("set mailbox."), self.CONSOLE_WARN)
    self.iImapMailbox.setFocus()
    return True

if self.imap_cache_items.get(mailbox):
    self.console(self.__tr("mailbox '%1' already exists.").arg(mailbox), self.CONSOLE_WARN)
    return True

imap = self.imap_connect()
if imap is None:
    return False

if self.rbImapMailboxMode.isChecked():
    mbPath = "user%s%s" % (imap.SEP, mailbox)
else:
    mbPath = mailbox

partition = None
if self.iCyrusPart.text().ascii():
    partition = self.iCyrusPart.text().ascii()

try:
    __imap_add(mbPath, partition)
except cyruslib.CYRUSError, e:
    self.console(self.__tr("mailbox '%1' creation failed (%2).").arg(mailbox).arg(e[2]), self.CONSOLE_ERR)
    return False

if len(mailbox.split(imap.SEP)) == 1:
    if self.iConfImapQuotaMbytes.text().ascii():
        imap.sq(mbPath, (int(self.iConfImapQuotaMbytes.text().ascii()) * 1024))
    if self.rbImapMailboxMode.isChecked():
        item = self.lvConfImapFolders.firstChild()
        while item is not None:
            __imap_add("%s%s%s" % (mbPath, imap.SEP, item.text(0).ascii()))
            if item.text(1).ascii() and item.text(1).ascii() != "0":
                imap.setannotation("%s%s%s" % (mbPath, imap.SEP, item.text(0).ascii()), "/vendor/cmu/cyrus-imapd/expire", item.text(1).ascii())
            if item.text(2).ascii():
                imap.sam("%s%s%s" % (mbPath, imap.SEP, item.text(0).ascii()), "anyone", "p")
            item = item.nextSibling()

self.lvImap.setCurrentItem(self.imap_cache_items.get(mailbox))
self.lvImap.ensureItemVisible(self.imap_cache_items.get(mailbox))
self.imap_mailbox_clicked()

}

void dKorreio::imap_delete() {

imap = self.imap_connect()
if imap is None:
    return False

mailbox = self.imap_current_mailbox(imap.SEP)
if mailbox is None:
    self.console(self.__tr("select mailbox for deletion."), self.CONSOLE_WARN)
    return True

#
# Ask about deletion
#

if QMessageBox.information( None, self.__tr("Confirm!"),
                                               self.__tr("Check mailbox for deletion:\n\n    - %1\n\n").arg(mailbox),
                                               self.__tr("&Yes"), self.__tr("&No") ) != 0:
    self.console(self.__tr("mailbox '%1' deletion aborted.").arg(mailbox), self.CONSOLE_WARN)
    return True

#
# Delete Imap Mailbox
#

try:
    imap.dm(mailbox)
    del self.imap_cache_items[re.sub("^user%s" % imap.SEP, "", mailbox)]
    self.console(self.__tr("mailbox '%1' deleted.").arg(mailbox))
except cyruslib.CYRUSError, e:
    self.console(self.__tr("mailbox '%1' deletion failed. (%2)").arg(mailbox).arg(e[2]), self.CONSOLE_ERR)
    return False

#
# Delete Interface item
#

item = self.lvImap.currentItem()
if item is not None:
    if item.parent() is None:
        self.lvImap.takeItem(item)
    else:
        item.parent().takeItem(item)

item = self.lvImap.currentItem()
if item is not None:
    item.setSelected(True)

}

void dKorreio::imap_set_quota() {

imap = self.imap_connect()
if imap is None:
    return False

mailbox = self.imap_current_mailbox(imap.SEP)
if mailbox is None:
    self.console(self.__tr("select mailbox to set quota."), self.CONSOLE_WARN)
    return True

#
# IMAP Set Quota
#

try:
    imap.sq(mailbox, self.iQuota.text().ascii())
    self.console(self.__tr("mailbox '%1' seted quota to '%2 Kbytes'.").arg(mailbox).arg(self.iQuota.text().ascii()))
except:
    self.console(self.__tr("can't set quota to mailbox '%1'.").arg(mailbox), self.CONSOLE_ERR)

}

void dKorreio::imap_reconstruct() {

imap = self.imap_connect()
if imap is None:
    return False

mailbox = self.imap_current_mailbox(imap.SEP)
if mailbox is None:
    self.console(self.__tr("select mailbox for reconstruct."), self.CONSOLE_WARN)
    return True

try:
    imap.reconstruct(mailbox)
    self.console(self.__tr("mailbox '%1' has been reconstructed.").arg(mailbox))
except:
    self.console(self.__tr("can't reconstruct mailbox '%1'.").arg(mailbox), self.CONSOLE_ERR)

}

void dKorreio::imap_edit() {

#
# Turn field editable
#

imap = self.imap_connect()
if imap is None:
    return False

self.imap_mailbox_rename = self.imap_current_mailbox(imap.SEP)
self.lvImap.currentItem().setRenameEnabled(0, True)
self.lvImap.currentItem().startRename(0)

}

void dKorreio::imap_rename() {

#
# Undo rename
#

def rename_undo():
    self.lvImap.currentItem().setText(0, self.imap_mailbox_rename.split(imap.SEP)[-1])

#
# Current mailbox is a root mailbox
#

def is_root_folder():
    if len(re.sub("^user%s" % imap.SEP, "", self.imap_mailbox_rename).split(imap.SEP)) == 1:
        return True
    return False

#
# Imap Connect
#

imap = self.imap_connect()
if imap is None:
    return False

#
# Mailbox has a name?
#

mailbox = self.imap_current_mailbox(imap.SEP)
if mailbox is None:
    self.console(self.__tr("type a valid mailbox name."), self.CONSOLE_WARN)
    rename_undo()
    return False

#
# Mailbox was renamed?
#

if self.imap_mailbox_rename == mailbox:
    return False

#
# Mailbox already exist?
#

if len(imap.lm(mailbox)) > 0:
    self.console(self.__tr("mailbox '%1' already exist.").arg(mailbox), self.CONSOLE_WARN)
    rename_undo()
    return False

#
# Confirm
#

msg = self.__tr("Please verify if user is not logged by POP/IMAP before you proceed.\n         # grep -r johndoe /var/lib/cyrus/proc/")
if QMessageBox.information( None, self.__tr("Warning!"), msg, self.__tr("&Ok")) != 0:
    self.console(self.__tr("rename aborted."), self.CONSOLE_WARN)
    rename_undo()
    return False

#
# Detect mailbox partition
#

partition = None
if is_root_folder():
    try:
        partitionDict = imap.getannotation(self.imap_mailbox_rename, "/vendor/cmu/cyrus-imapd/partition")
        partition = partitionDict[self.imap_mailbox_rename]["/vendor/cmu/cyrus-imapd/partition"]
    except:
        self.console(self.__tr("can't detect IMAP-partition to '%1'.").arg(mailbox), self.CONSOLE_ERR)
        rename_undo()
        return None

#
# Do rename
#

try:
    imap.rename(self.imap_mailbox_rename, mailbox, partition)
    self.console(self.__tr("mailbox '%1' renamed to '%2'.").arg(self.imap_mailbox_rename).arg(mailbox))
except:
    self.console(self.__tr("can't rename '%1'. Set option: \"allowusermoves: yes\".").arg(self.imap_mailbox_rename), self.CONSOLE_ERR)
    rename_undo()

self.imap_mailbox_clicked()

}

void dKorreio::imap_annotation_set( a0, a1, a2, a3 ) {
# a0=obj:imap, a1=str:mailbox, a2=str:annot, a3=str:annotvalue

imap = a0
mailbox = a1
annot = a2
annotvalue = a3

if not annot:
    self.console(self.__tr("select annotation."), self.CONSOLE_WARN)
    return False

try:
    imap.setannotation(mailbox, annot, annotvalue)
    self.console(self.__tr("mailbox '%1' has annotation '%2' set to '%3'.").arg(mailbox).arg(annot).arg(annotvalue))
except cyruslib.CYRUSError, e:
    self.console(self.__tr("can't set annotation '%1' for mailbox '%2' (%3).").arg(annot).arg(mailbox).arg(e[2]), self.CONSOLE_ERR)

}

void dKorreio::imap_annotation_user_set() {

imap = self.imap_connect()
if imap is None:
    return False

mailbox = self.imap_current_mailbox(imap.SEP)
if mailbox is None:
    self.console(self.__tr("select mailbox to set annotation."), self.CONSOLE_WARN)
    return True

self.imap_annotation_set(imap, mailbox, self.cbImapAnnotation.currentText().ascii(), self.iAnnotationValue.text().ascii())

}

void dKorreio::imap_annotation_server_set() {

imap = self.imap_connect()
if imap is None:
    return False

self.imap_annotation_set(imap, "", self.cbImapAnnotationServer.currentText().ascii(), self.iAnnotationValueServer.text().ascii())

}

void dKorreio::imap_annotation_get(a0, a1, a2) {
# a0=obj:imap, a1=str:mailbox, a2=str:annot

imap = a0
mailbox = a1
annot = a2

if imap is None:
    return False

if not annot:
    self.console(self.__tr("select annotation."), self.CONSOLE_WARN)
    return False

try:
    annotation = imap.getannotation(mailbox, annot)
    return annotation.get(mailbox).get(annot)
except cyruslib.CYRUSError, e:
    self.console(self.__tr("can't get annotation '%1' for mailbox '%2' (%3)." % (annot, mailbox, e[2])), self.CONSOLE_ERR)
    return ""
except:
    return ""

}

void dKorreio::imap_annotation_user_get() {
# a0=obj:QString or None, a1=obj:imap or None

imap = a1

if imap is None:
    imap = self.imap_connect()
    if imap is None:
        return False

mailbox = self.imap_current_mailbox(imap.SEP)
if mailbox is None:
    self.console(self.__tr("select a mailbox."), self.CONSOLE_WARN)
    return True

annot = self.imap_annotation_get(imap, mailbox, self.cbImapAnnotation.currentText().ascii())
if annot:
    self.iAnnotationValue.setText(annot)
else:
    self.iAnnotationValue.setText("")

}

void dKorreio::imap_annotation_server_get() {

imap = self.imap_connect()
if imap is None:
    return False

annot = self.imap_annotation_get(imap, "", self.cbImapAnnotationServer.currentText().ascii())
if annot:
    self.iAnnotationValueServer.setText(annot)
else:
    self.iAnnotationValueServer.setText("")

}

void dKorreio::imap_acl_wizard() {

if self.cbACL.currentItem() == 1:
    self.cbACL.setCurrentText("lrsw")
elif self.cbACL.currentItem() == 2:
    self.cbACL.setCurrentText("lrswi")
elif self.cbACL.currentItem() == 3:
    self.cbACL.setCurrentText("lrswicd")
elif self.cbACL.currentItem() == 4:
    self.cbACL.setCurrentText("p")
elif self.cbACL.currentItem() == 5:
    self.cbACL.setCurrentText("lrswipcda")

}

void dKorreio::imap_acl_del() {

imap = self.imap_connect()
if imap is None:
    return False

mailbox = self.imap_current_mailbox(imap.SEP)
if mailbox is None:
    self.console(self.__tr("select mailbox for ACL deletion."), self.CONSOLE_WARN)
    return True

def __acl_del(user):
    try:
        imap.sam(mailbox, user, "")
        self.console(self.__tr("mailbox '%1' has ACL to '%2' deleted.").arg(mailbox).arg(user))
        return True
    except:
        self.console(self.__tr("can't delete ACL to '%1' for mailbox '%2'.").arg(user).arg(mailbox), self.CONSOLE_ERR)
        return False

itemDel = []
item = self.lvImapAcl.firstChild()
while item is not None:
    if item.isSelected():
        if __acl_del(item.text(0).ascii()):
            itemDel.append(item)
    item = item.nextSibling()

for item in itemDel:
    self.lvImapAcl.takeItem(item)

}

void dKorreio::imap_acl_add() {

imap = self.imap_connect()
if imap is None:
    return False

mailbox = self.imap_current_mailbox(imap.SEP)
if mailbox is None:
    self.console(self.__tr("select mailbox to set ACL."), self.CONSOLE_WARN)
    return True

AclUser = self.iImapAclUser.text().latin1().replace(" ", "").replace(";", ",")
Acl = self.cbACL.currentText().ascii()

for user in AclUser.split(","):
    if not user:
        continue
    try:
        imap.sam(mailbox, user, Acl)
        self.console(self.__tr("mailbox '%1' has added ACL '%2' to '%3'.").arg(mailbox).arg(Acl).arg(user))
        item = self.lvImapAcl.firstChild()
        while item is not None:
            if item.text(0).ascii() == user:
                self.lvImapAcl.takeItem(item)
                break
            item = item.nextSibling()
        item = QListViewItem(self.lvImapAcl)
        item.setText(0, user)
        item.setText(1, Acl)
        item.setPixmap(0, self.icons.get("lock_overlay.png"))
        self.lvImapAcl.setCurrentItem(item)
    except:
        self.console(self.__tr("can't set ACL '%1' to user '%2' for mailbox '%3'.").arg(Acl).arg(user).arg(self.iImapMailbox.text().ascii()), self.CONSOLE_ERR)

}

void dKorreio::imap_acl_clicked() {
# a0=item:current ACL item

item = a0
if item is not None:
    self.iImapAclUser.setText(item.text(0).ascii())
    self.cbACL.setCurrentText(item.text(1).ascii())
else:
    self.iImapAclUser.clear()
    self.cbACL.setCurrentItem(0)

}

void dKorreio::imap_menu() {
# a0: item, a1: pos-xy

item = a0
pos = a1

if item is None:
    self.lvImapMenu.setItemEnabled(0, False)
    self.lvImapMenu.setItemEnabled(1, False)
    self.lvImapMenu.setItemEnabled(2, False)
else:
    self.lvImapMenu.setItemEnabled(0, True)
    self.lvImapMenu.setItemEnabled(1, True)
    self.lvImapMenu.setItemEnabled(2, True)

self.korreio_update_servers_menu()
self.lvImapMenu.popup(pos)

}

void dKorreio::imap_menu_clicked( a0 ) {

menuItem = a0

if menuItem == 0:
    self.imap_edit()
elif menuItem == 1:
    self.imap_delete()
elif menuItem == 2:
    self.imap_reconstruct()
elif menuItem == 7:
    item = self.lvImap.firstChild()
    while item is not None:
        QExpandChilds(item)
        item = item.nextSibling()
elif menuItem == 10:
    self.wsImap.raiseWidget(2)
elif menuItem == 13:
    self.wsImap.raiseWidget(1)

}

void dKorreio::imap_form_clear() {

self.korreio_module_clear("imap")

}

void dKorreio::imap_export_findFile() {

file = QSelectFile(self.__tr("Korreio: save file..."), QFileDialog.AnyFile)
self.iImapExportFile.setText(file)

}

void dKorreio::imap_export_apply() {

file = Q2utf(self.iImapExportFile.text())
if not file:
    self.console(self.__tr("select a file."))
    return True

imap = self.imap_connect()
imap.setNormalize(False)
try:
    content = ""

    if self.rbImapExportMbxAll.isChecked():
        content = "\n".join(imap.lm())

    elif self.rbImapExportMbxRoot.isChecked():
        content = "\n".join(imap.lm("user%s%%" % imap.SEP))

        pattern = re.compile("^user%s" % imap.SEP, re.MULTILINE)
        content = pattern.sub("", content)

    else:
        contentList = []
        mbxs = imap.lm()
        annot = imap.getannotation("*", "/vendor/cmu/cyrus-imapd/expire")
        for mbx in mbxs:
            try:
                contentList.append("0;%s;%s" % (mbx, imap.lq(mbx)[1]))
            except:
                contentList.append("0;%s;-1" % mbx)
            for user, acl in imap.lam(mbx).items():
                contentList.append("1;%s;%s;%s" % (mbx, user, acl))
            if annot.get(mbx):
                contentList.append("2;%s;/vendor/cmu/cyrus-imapd/expire;%s" % (mbx, annot.get(mbx).get("/vendor/cmu/cyrus-imapd/expire")))
        content = "\n".join(contentList)
except:
    pass
imap.setNormalize(True)

try:
    f = open(file, 'w')
    f.write(content)
    f.write("\n")
    f.close()
    self.console(self.__tr("file '%1' saved sussesfully.").arg(file))
except OSError, e:
    self.parse_exception("OSError", e)

}

void dKorreio::imap_partition_identifyMenu() {

#
# Create a dictionary with columns indexed by name
#

colId = {}

cols = self.lvImapPartition.columns()
for col in range(cols-1, -1, -1):
    for colName in ["IMAP Mailbox", "Partition", "Limit", "Used", "%", "Last message", "Last POP3 connection"]:
        if self.lvImapPartition.columnText(col) == self.__tr(colName):
            colId[colName] = col
            continue

return colId

}

void dKorreio::imap_partition_search() {

self.korreio_module_clear("imap-partition")
imap = self.imap_connect()
if imap is None:
    return False

#
# Imap query
#

queryInput = self.iImapPartitionSearch.text().latin1()
queryInput = queryInput.replace("*", "")
if queryInput.find("@") == 0:
    query = "%%%s" % queryInput
elif queryInput.find("@") != -1:
    query = "%s" % queryInput
else:
    query = "%s%%" % queryInput

if self.rbImapPartMailboxMode.isChecked():
    query = "user%s%s" % (imap.SEP, query)

#
# Get columns
#

colId = self.imap_partition_identifyMenu()

#
# Get Partition and Mailbox Information
#
try:
    annotPart = imap.getannotation(query, "/vendor/cmu/cyrus-imapd/partition")
    if colId.get("Last message"):
        annotPOP = imap.getannotation(query, "/vendor/cmu/cyrus-imapd/lastpop")
        annotUpdate = imap.getannotation(query, "/vendor/cmu/cyrus-imapd/lastupdate")
except:
    return False

#
# Detect Partitions, Get quota, Show Annotations
#

partitions = {}
for mailbox in annotPart:
    #
    # Remember partitions
    #
    partitions[annotPart.get(mailbox).get("/vendor/cmu/cyrus-imapd/partition")] = None

    #
    # Show Mailbox and Partition
    #
    item = QListViewItemImapPartition(self.lvImapPartition)
    item.setPixmap(0, self.icons.get("folder.png"))
    item.setText(colId.get("IMAP Mailbox"), re.sub("^user%s" % imap.SEP, "", mailbox))
    item.setText(colId.get("Partition"), annotPart.get(mailbox).get("/vendor/cmu/cyrus-imapd/partition"))

    #
    # Show Quota Information
    #
    if colId.get("Used"):
        try:
            limit = imap.lq(mailbox)
            item.setText(colId.get("Used"), "%s" % limit[0])
            item.setText(colId.get("Limit"), "%s" % limit[1])
            item.setText(colId.get("%"), "%s%%" % (limit[0] * 100 / limit[1]) )
        except:
            item.setText(colId.get("Used"), "-")
            item.setText(colId.get("Limit"), "-")
            item.setText(colId.get("%"), "0%")

    #
    # Show Mailbox Information
    #
    if colId.get("Last message"):
        item.setText(colId.get("Last POP3 connection"), annotPOP.get(mailbox).get("/vendor/cmu/cyrus-imapd/lastpop"))
        item.setText(colId.get("Last message"), annotUpdate.get(mailbox).get("/vendor/cmu/cyrus-imapd/lastupdate"))

item = self.lvImapPartition.firstChild()
if item is None:
    self.pImapPartitionMove.setEnabled(False)
else:
    #
    # List available partitions
    #
    self.pImapPartitionMove.setEnabled(True)
    for part in partitions:
        self.cbImapPartition.insertItem(part)

    #
    # Select first item and update counter
    #
    self.lvImapPartition.setCurrentItem(item)
    item.setSelected(True)
    self.imap_partition_size()

}

void dKorreio::imap_partition_move() {

#
# Check partition
#

partition = self.cbImapPartition.currentText().latin1()
if partition == "" or re.search(" ", partition):
    self.cbImapPartition.setFocus()
    self.console(self.__tr("select IMAP-Partition."), self.CONSOLE_WARN)
    return False

imap = self.imap_connect()
if imap is None:
    return False

#
# Get columns
#

colId = self.imap_partition_identifyMenu()

#
# Change partition for selected items
#

item = self.lvImapPartition.firstChild()
while item is not None:
    if item.isSelected():
        mailbox = item.text(colId.get("IMAP Mailbox")).latin1()
        if self.rbImapPartMailboxMode.isChecked():
            mailbox = "user%s%s" % (imap.SEP, mailbox)
        try:
            imap.rename(mailbox, mailbox, partition)
            item.setText(colId.get("Partition"), partition)
            self.console(self.__tr("mailbox '%1' moved to IMAP-Partition '%2'.").arg(item.text(colId.get("IMAP Mailbox"))).arg(partition))
        except cyruslib.CYRUSError, e:
            self.console(self.__tr("can't move mailbox '%1' to IMAP-Partition '%2'. (%3)").arg(item.text(colId.get("IMAP Mailbox"))).arg(partition).arg(e[2]), self.CONSOLE_ERR)
    item = item.nextSibling()

}

void dKorreio::imap_partition_size() {

#
# Update information about selected items
#

if not self.cImapSizeUpdate.isChecked():
    self.tlImapSize.setText("0")
    return True

#
# Get columns
#

colId = self.imap_partition_identifyMenu()

#
# With quota Information
#

if colId.get("Used"):
    size = percent = count = countSelected = 0
    item = self.lvImapPartition.firstChild()
    while item is not None:
        if item.isSelected():
            countSelected += 1
            if item.text(colId.get("Used")).ascii() not in ["-", "", None]:
                size += int("%s" % item.text(colId.get("Used")))
                percent += int(item.text(colId.get("%")).ascii().replace("%", ""))
                count += 1
        item = item.nextSibling()

    if count != 0:
        count = percent / count

    self.tlImapSize.setText("%s ~ %s Mbytes ~ %s%%" % ((countSelected), (size / 1024), count))

#
# Simple count
#

else:
    countSelected = 0
    item = self.lvImapPartition.firstChild()
    while item is not None:
        if item.isSelected():
            countSelected += 1
        item = item.nextSibling()
    self.tlImapSize.setText("%i" % countSelected)

}

void dKorreio::imap_partition_menu() {
# a0: item, a1: pos-xy

self.korreio_update_servers_menu()
self.lvImapPartMenu.popup(a1)

}

void dKorreio::imap_partition_menu_clicked( a0 ) {

colId = self.imap_partition_identifyMenu()

menuItem = a0

if menuItem == 0:
    #
    # Select All
    #
    self.lvImapPartition.selectAll(True)

elif menuItem == 1:
    #
    # Set quota for selected items
    #
    colId = self.imap_partition_identifyMenu()

    #
    # Get default quota
    #
    try:
        dflquota = int(self.iConfImapQuotaMbytes.text()) * 1024
    except:
        dflquota = 0

    #
    # Ask confirmation
    #
    quota, msg = QInputDialog.getInteger(self.__tr("Quota"), self.__tr("Set quota limit (Kbytes):"), dflquota, 0, 2147483647, 512)

    if msg:
        imap = self.imap_connect()
        if imap is None:
            return False
        item = self.lvImapPartition.firstChild()
        while item is not None:
            if item.isSelected():
                mbx = item.text(colId.get("IMAP Mailbox")).latin1()
                if self.rbImapPartMailboxMode.isChecked():
                    mbx = "user%s%s" % (imap.SEP, mbx)
                try:
                    imap.sq(mbx, quota)

                    #
                    # Update interface
                    #
                    if colId.get("Used"):
                        item.setText(colId.get("Limit"), "%s" % quota)
                        try:
                            used = "%s%%" % (int(item.text(colId.get("Used"))) * 100 / quota)
                        except:
                            used = "0%"
                        item.setText(colId.get("%"), used)
                    self.console(self.__tr("mailbox '%1' has set quota to '%2 Kbytes'.").arg(item.text(colId.get("IMAP Mailbox"))).arg(str(quota)))
                except:
                    self.console(self.__tr("can't set quota to mailbox '%1'.").arg(item.text(colId.get("IMAP Mailbox"))), self.CONSOLE_ERR)
            item = item.nextSibling()
    else:
        self.console(self.__tr("set quota aborted."), self.CONSOLE_INFO)

elif menuItem == 2:
    #
    # Delete
    #
    item = self.lvImapPartition.firstChild()
    while item is not None:
        if item.isSelected():
            print "delete"
        item = item.nextSibling()

elif menuItem == 3:
    #
    # Reconstruct
    #
    pass

elif menuItem == 4:
    #
    # Export to CSV
    #

    #
    # Read filename
    #

    file = Q2utf(QSelectFile(self.__tr("Korreio: save CSV file..."), QFileDialog.AnyFile))
    if not file:
        return True
    #
    # Create content
    #
    CSVfile = []
    cols = self.lvImapPartition.columns()

    line = []
    for col in range(0, cols):
        line.append(Q2uni(self.lvImapPartition.columnText(col)).encode('iso-8859-1'))
    CSVfile.append(";".join(line))

    item = self.lvImapPartition.firstChild()
    while item is not None:
        if item.isSelected():
            line = []
            for col in range(0, cols):
                field = item.text(col).ascii()
                if field is None:
                    line.append("")
                else:
                    line.append(field)
            CSVfile.append(";".join(line))
        item = item.nextSibling()

    #
    # Save file
    #
    try:
        f = open(file, 'w')
        f.write("\n".join(CSVfile))
        f.write("\n")
        f.close()
        self.console(self.__tr("CSV file '%1' saved sussesfully.").arg(file))
    except OSError, e:
        self.parse_exception("OSError", e)

elif menuItem == 7:
    #
    # [En|Dis]able quota Info
    #
    if colId.get("Used"):
        self.lvImapPartMenu.changeItem(7, QIconSet(QPixmap()), self.__tr('&Quota info'))
        cols = self.lvImapPartition.columns()
        for col in range(cols-1,-1,-1):
            if self.lvImapPartition.columnText(col) in [self.__tr("Used"), self.__tr("Limit"), self.__tr("%")]:
                self.lvImapPartition.removeColumn(col)
    else:
        self.lvImapPartMenu.changeItem(7, QIconSet(self.icons.get("ok.png")), self.__tr('&Quota info'))
        self.lvImapPartition.addColumn(self.__tr("Used"))
        self.lvImapPartition.addColumn(self.__tr("Limit"))
        self.lvImapPartition.addColumn(self.__tr("%"))

    if self.lvImapPartition.columns() == 2:
        self.lvImapPartition.setResizeMode(QListView.LastColumn)
    else:
        self.lvImapPartition.setResizeMode(QListView.AllColumns)

elif menuItem == 8:
    #
    # [En|Dis]able mailbox info
    #
    if colId.get("Last message"):
        self.lvImapPartMenu.changeItem(8, QIconSet(QPixmap()), self.__tr('&Mailbox info'))
        cols = self.lvImapPartition.columns()
        for col in range(cols-1, -1, -1):
            if self.lvImapPartition.columnText(col) in [self.__tr("Last POP3 connection"), self.__tr("Last message")]:
                self.lvImapPartition.removeColumn(col)
    else:
        self.lvImapPartMenu.changeItem(8, QIconSet(self.icons.get("ok.png")), self.__tr('&Mailbox info'))
        self.lvImapPartition.addColumn(self.__tr("Last POP3 connection"))
        self.lvImapPartition.addColumn(self.__tr("Last message"))

    if self.lvImapPartition.columns() == 2:
        self.lvImapPartition.setResizeMode(QListView.LastColumn)
    else:
        self.lvImapPartition.setResizeMode(QListView.AllColumns)

}

void dKorreio::imap_partition_form_clear() {

self.korreio_module_clear("imap-partition")

}

void dKorreio::ldap_connect() {

#
# Verify LDAP connection cache
#

try:
    if self.l:
        if self.inet_cache.get("ldap.mode") == self.cbLdapMode.currentText().ascii() and self.inet_cache.get("ldap.host") == self.iLdapHost.text().ascii() and self.inet_cache.get("ldap.port") == self.iLdapPort.text().ascii() and self.inet_cache.get("ldap.user") == self.iLdapUser.text().ascii() and self.inet_cache.get("ldap.pass") == self.iLdapPass.text().ascii() and self.inet_cache.get("ldap.ref") == "%s" % self.cLdapRef.isChecked():
            return self.l
except:
    pass

#
# Connect to LDAP
#

try:
    if self.cLdapCert.isChecked():
        ldap.set_option(ldap.OPT_X_TLS_REQUIRE_CERT, 0)
    else:
        ldap.set_option(ldap.OPT_X_TLS_REQUIRE_CERT, 2)
    if self.cLdapRef.isChecked():
        ldap.set_option(ldap.OPT_REFERRALS, 1)
    else:
        ldap.set_option(ldap.OPT_REFERRALS, 0)
    server = "%s%s:%s" % (self.cbLdapMode.currentText().ascii(), self.iLdapHost.text().ascii(), self.iLdapPort.text().ascii())
    self.l = ldap.initialize(server)
    self.l.protocol_version = ldap.VERSION3
    if self.iLdapUser.text().ascii() and self.iLdapPass.text().ascii():
        self.l.simple_bind(Q2utf(self.iLdapUser.text()), Q2utf(self.iLdapPass.text()))
    else:
        self.l.simple_bind()
except ldap.LDAPError, e:
    try:
        del self.l
    except:
        pass
    raise ldap.LDAPError, e

#
# Save current LDAP connection information
#

self.inet_cache["ldap.mode"] = self.cbLdapMode.currentText().ascii()
self.inet_cache["ldap.host"] = self.iLdapHost.text().ascii()
self.inet_cache["ldap.port"] = self.iLdapPort.text().ascii()
self.inet_cache["ldap.user"] = self.iLdapUser.text().ascii()
self.inet_cache["ldap.pass"] = self.iLdapPass.text().ascii()
self.inet_cache["ldap.ref"] = "%s" % self.cLdapRef.isChecked()

self.console(self.__tr("%1 connected.").arg("%s/%s" % (server, self.iLdapUser.text().ascii())))
return self.l

}

void dKorreio::ldap_search() {

#
# Save current ldap filter and remove oldest
#

self.cbLdapFilter.insertItem(self.cbLdapFilter.currentText().latin1())
if self.cbLdapFilter.count() > self.lbConfLdapFilter.count() + 7:
    self.cbLdapFilter.removeItem(self.lbConfLdapFilter.count())
self.cbLdapFilter.setCurrentItem(self.cbLdapFilter.count() - 1)

#
# Save selected entry
#

basedn = Q2uni(self.cbLdapBaseDN.currentText())
dnOld = self.ldap_current_dn()
if dnOld is None:
    dnOld = basedn

self.korreio_module_clear("ldap")

#
# Ldap query
#

if not self.cbLdapFilter.currentText().latin1():
    filter = "(objectClass=*)"
else:
    filter = "(%s)" % Q2utf(self.cbLdapFilter.currentText())
self.ldap_cache.load(self.ldap_query(filter))
if self.ldap_cache.isEmpty():
    return True

#
# Update interface
#

self.ldap_items.init([basedn])

for dn in self.ldap_cache.keys():
    self.ldap_items.load(dn)
    self.ldap_cache.rename(dn, dn.lower())

#
# Set last selection
#

item = self.ldap_items.get(dnOld.lower())
if item is None:
    item = self.lvLdap.firstChild()
if item is not None:
    self.lvLdap.setCurrentItem(item)
    self.lvLdap.currentItem().setSelected(True)
    while item is not None:
        item.setOpen(True)
        item = item.parent()

#
# ensureItemVisible improvent
#

item = self.lvLdap.currentItem()
if item is not None:
    self.lvLdap.scrollBy(0, item.itemPos() - 30)

#
# Show attibutes for selected item
#

self.ldap_dn_clicked()
self.lvLdap.setFocus()

}

void dKorreio::ldap_query( a0 ) {
# a0=str:ldap_filter, a1=list:attrs or None (retrive attributes)
# a2=str:baseDN or None (default base), a3:ldap.SCOPE:scope or None (subtree)

ldap_filter = a0
attrs = a1
baseDN = a2
scope = a3

if baseDN is None:
    baseDN = Q2utf(self.cbLdapBaseDN.currentText())
if scope is None:
    scope = ldap.SCOPE_SUBTREE

try:
    l = self.ldap_connect()
    if l is None:
        return False
    ldap_result_id = l.search(baseDN, scope, ldap_filter, attrs)
    ldap_result = {}
    while 1:
        result_type, result_data = l.result(ldap_result_id, 0)
        if result_type == ldap.RES_SEARCH_RESULT:
            break
        elif result_type == ldap.RES_SEARCH_ENTRY:
            ldap_result[utf2uni(result_data[0][0])] = result_data[0][1]
        elif result_type == ldap.RES_SEARCH_REFERENCE:
            dn = result_data[0][1][0].split('/', 3)[3].split("?")[0]
            attr, value = dn.split(',')[0].split('=')
            ldap_result[utf2uni(dn)] = {'ref': [result_data[0][1][0]], 'objectClass': ['referral', 'extensibleObject'], attr: [value]}
        else:
            print "ERROR: result type not implemented. %s" % result_type
    return ldap_result
except ldap.LDAPError, e:
    self.parse_exception("LDAPError", e)
    if e[0]["desc"] == "Size limit exceeded":
        return ldap_result

}


void dKorreio::ldap_current_dn() {

item = self.lvLdap.currentItem()
if item is None:
    return None

dn = Q2uni(item.text(0))
while item.parent() is not None:
    dn = dn + u"," + Q2uni(item.parent().text(0))
    item = item.parent()

return dn

}

void dKorreio::ldap_dn_clicked() {

#
# Default is modify action
#
self.ldap_add_or_modify = True

self.lvLdapAttr.clear()
self.wsLdap.raiseWidget(0)

dn = self.ldap_current_dn()
if dn is None:
    return True

if self.ldap_cache.get(dn.lower()):
    self.pLdapDelete.setEnabled(True)
    if 'ref' in self.ldap_cache.get(dn.lower()).keys():
        self.pLdapModify.setEnabled(False)
    else:
        self.pLdapModify.setEnabled(True)
    for attribute, values in self.ldap_cache.get(dn.lower()).items():
        for value in values:
            item = QListViewItemLdapAttr(self.lvLdapAttr)
            item.setText(0, attribute)
            try:
                item.setText(1, utf2uni(value))
            except:
                item.setText(1, value)
else:
    self.pLdapModify.setEnabled(False)
    self.pLdapDelete.setEnabled(False)

#
# Select first attribute
#

if self.lvLdapAttr.childCount() > 0:
    self.lvLdapAttr.setCurrentItem(self.lvLdapAttr.firstChild())
    self.lvLdapAttr.currentItem().setSelected(True)

}

void dKorreio::ldap_wizard_sambaDomain_getSID( a0 ) {

SIDEntry, uidCounter = self.config_sambaDomain_getConf(a0)
SID = self.ldap_query(SIDEntry.split(u",")[0].encode('utf-8'), ['sambaSID'], ",".join(SIDEntry.encode('utf-8').split(",")[1:]))
if not SID:
    return None

return SID.get(SIDEntry).get("sambaSID")[0]

}

void dKorreio::ldap_wizard_sambaDomain_getUidNumber( a0 ) {

SIDEntry, uidCounter = self.config_sambaDomain_getConf(a0)
uidNumber = self.ldap_query(uidCounter.split(u",")[0].encode('utf-8'), ['uidNumber'], ",".join(uidCounter.encode('utf-8').split(",")[1:]))
if not uidNumber:
    return None

return uidNumber.get(uidCounter).get("uidNumber")[0]

}

void dKorreio::ldap_wizard_sambaDomain_getGidNumber( a0 ) {

SIDEntry, uidCounter = self.config_sambaDomain_getConf(a0)
gidNumber = self.ldap_query(uidCounter.split(u",")[0].encode('utf-8'), ['gidNumber'], ",".join(uidCounter.encode('utf-8').split(",")[1:]))
if not gidNumber:
    return None

return gidNumber.get(uidCounter).get("gidNumber")[0]

}

void dKorreio::ldap_wizard_sambaDomain_incUidNumber( a0, a1 ) {

sambaDomain = a0
uidNumber = a1

SIDEntry, uidCounter = self.config_sambaDomain_getConf(sambaDomain)

attrsOld = {}
attrsOld['uidNumber'] = [uidNumber]
attrsNew = {}
attrsNew['uidNumber'] = ["%s" % (int(uidNumber) + 1)]
self.ldap_modify(uidCounter, attrsOld, attrsNew)

}

void dKorreio::ldap_wizard_sambaDomain_incGidNumber( a0, a1 ) {

sambaDomain = a0
gidNumber = a1

SIDEntry, uidCounter = self.config_sambaDomain_getConf(sambaDomain)

attrsOld = {}
attrsOld['gidNumber'] = [gidNumber]
attrsNew = {}
attrsNew['gidNumber'] = ["%s" % (int(gidNumber) + 1)]
self.ldap_modify(uidCounter, attrsOld, attrsNew)

}

void dKorreio::ldap_wizard_user_next() {

    self.wsLdapForm.raiseWidget(self.wsLdapForm.id(self.wsLdapForm.visibleWidget()) + 1)

}

void dKorreio::ldap_wizard_user_back() {

    self.wsLdapForm.raiseWidget(self.wsLdapForm.id(self.wsLdapForm.visibleWidget()) - 1)

}

void dKorreio::ldap_wizard_posixAccount_enable() {

    if self.cLdapFormPosix.isChecked():
        self.fLdapFormPosix.setEnabled(True)
    else:
        self.fLdapFormPosix.setEnabled(False)
        self.fLdapFormSamba.setEnabled(False)
        self.cLdapFormSamba.setChecked(False)

}

void dKorreio::ldap_wizard_posixAccount_get_uidNumber() {

try:
    uidNumbers = self.ldap_query("(&(uidNumber=*)(!(objectClass=sambaUnixIdPool)))", ["uidNumber"])
    if uidNumbers:
        nextUidNumber = 999
        for item in uidNumbers:
            uidNumber = uidNumbers.get(item).get("uidNumber")[0]
            if int(uidNumber) > nextUidNumber:
                nextUidNumber = int(uidNumber)

        if nextUidNumber > 998:
            self.iLdapFormUidNumber.setText("%s" % (nextUidNumber + 1))
    
except ldap.LDAPError, e:
    self.parse_exception("LDAPError", e)

}

void dKorreio::ldap_wizard_sambaSamAccount_enable() {

if self.cLdapFormSamba.isChecked():
    if not self.iLdapFormUid.text().latin1():
        self.console(self.__tr("set email or uid."), self.CONSOLE_WARN)
        self.cLdapFormSamba.setChecked(False)
        return False

    self.cbLdapFormSambaDomain.clear()
    for domain in self.config_sambaDomains():
        self.cbLdapFormSambaDomain.insertItem(domain)

    if self.cbLdapFormSambaDomain.count() == 0:
        self.console(self.__tr("set netbios domains."), self.CONSOLE_WARN)
        self.cLdapFormSamba.setChecked(False)
    else:
        self.cLdapFormPosix.setChecked(True)
        self.fLdapFormPosix.setEnabled(True)
        self.iLdapFormUidNumber.setEnabled(False)
        self.pLdapGetUidNumber.setEnabled(False)
        self.iLdapFormGidNumber.setEnabled(False)
        self.fLdapFormSamba.setEnabled(True)
        self.ldap_wizard_sambaSamAccount_domain_clicked()
else:
    self.fLdapFormSamba.setEnabled(False)
    self.iLdapFormUidNumber.setEnabled(True)
    self.pLdapGetUidNumber.setEnabled(True)
    self.iLdapFormGidNumber.setEnabled(True)
    self.iLdapFormGidNumber.setText("100")

}

void dKorreio::ldap_wizard_sambaSamAccount_domain_clicked() {

domain = self.cbLdapFormSambaDomain.currentText().ascii()
i=0
while self.confDict.get("ldap.smb%s.domain" % i):
    if self.confDict.get("ldap.smb%s.domain" % i) == domain:
        break
    i+=1

#
# sambaPwdMustChange
#
if self.confDict["ldap.smb%s.pwdMustChange" % i] == "True":
    self.iLdapFormSambaPwdMustChange.setChecked(True)
else:
    self.iLdapFormSambaPwdMustChange.setChecked(False)

#
# sambaPrimaryGroupSID / gidNumber
#
self.cbLdapFormPrimaryGroup.setCurrentItem(int(self.confDict["ldap.smb%s.primaryGroup" % i]))
gidNumber = self.cbLdapFormPrimaryGroup.currentText().ascii().split("(")[1].split(")")[0]
self.iLdapFormGidNumber.setText(gidNumber)

#
# sambaProfilePath
#
if self.confDict["ldap.smb%s.profileType" % i] == "0":
    self.cbLdapFormProfileType.setCurrentItem(0)
    self.iLdapFormProfilePath.setEnabled(False)
else:
    self.cbLdapFormProfileType.setCurrentItem(1)
    self.iLdapFormProfilePath.setEnabled(True)
profilePath = re.sub("#UID#", self.iLdapFormUid.text().ascii(), self.confDict["ldap.smb%s.profilePath" % i])
profilePath = re.sub("#GID#", gidNumber, profilePath)
self.iLdapFormProfilePath.setText(profilePath)

#
# sambaHomeDrive
#
self.iLdapFormHomeDrive.setText(self.confDict["ldap.smb%s.homeDrive" % i])

#
# sambaHomePath
#
drivePath = re.sub("#UID#", self.iLdapFormUid.text().ascii(), self.confDict["ldap.smb%s.drivePath" % i])
drivePath = re.sub("#GID#", gidNumber, drivePath)
self.iLdapFormDrivePath.setText(drivePath)

#
# sambaLogonScript
#
logonScript = re.sub("#UID#", self.iLdapFormUid.text().ascii(), self.confDict["ldap.smb%s.logonScript" % i])
logonScript = re.sub("#GID#", gidNumber, logonScript)
self.iLdapFormLogonScript.setText(logonScript)

}

void dKorreio::ldap_wizard_sambaSamAccount_perfil_clicked() {

    if self.cbLdapFormProfileType.currentItem() == 0:
        self.iLdapFormProfilePath.setEnabled(False)
    else:
        self.iLdapFormProfilePath.setEnabled(True)

}

void dKorreio::ldap_wizard_astSipUser_enable() {

    if self.cLdapFormAst.isChecked():
        self.fLdapFormAst.setEnabled(True)
    else:
        self.fLdapFormAst.setEnabled(False)

}

void dKorreio::ldap_wizard_radiusProfile_enable() {

    if self.cLdapFormRadius.isChecked():
        self.fLdapFormRadius.setEnabled(True)
    else:
        self.fLdapFormRadius.setEnabled(False)

}

void dKorreio::ldap_wizard_uid_changed() {

    self.iLdapFormHomeDirectory.setText("/home/%s" % self.iLdapFormUid.text().ascii())
    self.iLdapFormAstUsername.setText(self.iLdapFormUid.text().ascii())

}

void dKorreio::ldap_wizard_inetOrgPerson_mail_changed() {

    self.iLdapFormUid.setText(self.iLdapFormMail.text().ascii().split("@")[0])

}

void dKorreio::ldap_wizard_user_apply() {

if not self.iLdapFormCn.text().ascii():
    self.console(self.__tr("set user name."), self.CONSOLE_WARN)
    self.wsLdapForm.raiseWidget(0)
    self.iLdapFormCn.setFocus()
    return False

cn = Q2utf(self.iLdapFormCn.text())

attrs = {}

#
# inetOrgPerson
#

attrs['objectClass'] = ['inetOrgPerson']
attrs['cn'] = [cn]
attrs['sn'] = [cn.split(" ")[-1]]
if self.iLdapFormMail.text().ascii():
    attrs['mail'] = [self.iLdapFormMail.text().ascii()]

if self.iLdapFormStreet.text().ascii():
    attrs['street'] = [Q2utf(self.iLdapFormStreet.text())]

if self.iLdapFormL.text().ascii():
    attrs['l'] = [Q2utf(self.iLdapFormL.text())]

if self.iLdapFormPostalCode.text().ascii():
    attrs['postalCode'] = [self.iLdapFormPostalCode.text().ascii()]

if self.iLdapFormHomePhone.text().ascii():
    attrs['homePhone'] = [self.iLdapFormHomePhone.text().ascii()]

if self.iLdapFormUserP.text().ascii() != self.iLdapFormUserP2.text().ascii():
    self.console(self.__tr("password don't match, type again."), self.CONSOLE_WARN)
    self.wsLdapForm.raiseWidget(0)
    self.iLdapFormUserP.clear()
    self.iLdapFormUserP2.clear()
    self.iLdapFormUserP.setFocus()
    return False

if self.iLdapFormUserP.text().ascii():
    salt = ''
    for i in range(16):
        salt += choice(letters+digits)
    attrs['userPassword'] = ["{SSHA}%s"  % b2a_base64(sha.new(self.iLdapFormUserP.text().ascii() + salt).digest() + salt)[:-1]]

#
# posixAccount
#

if self.cLdapFormPosix.isChecked():
    attrs['objectClass'].extend(['posixAccount', 'shadowAccount'])

    if not self.iLdapFormUid.text().ascii():
        self.console(self.__tr("set uid."), self.CONSOLE_INFO)
        self.iLdapFormUid.setFocus()
        return False
    attrs['uid'] = [Q2utf(self.iLdapFormUid.text())]

    if not self.iLdapFormUidNumber.text().ascii():
        self.console(self.__tr("set uidNumber."), self.CONSOLE_INFO)
        self.iLdapFormUidNumber.setFocus()
        return False
    attrs['uidNumber'] = [self.iLdapFormUidNumber.text().ascii()]

    if not self.iLdapFormGidNumber.text().ascii():
        self.console(self.__tr("set gidNumber."), self.CONSOLE_INFO)
        self.iLdapFormGidNumber.setFocus()
        return False
    attrs['gidNumber'] = [self.iLdapFormGidNumber.text().ascii()]

    if self.iLdapFormLoginShell.text().ascii():
        attrs['loginShell'] = [Q2utf(self.iLdapFormLoginShell.text())]

    if not self.iLdapFormHomeDirectory.text().ascii():
        self.console(self.__tr("set home directory."), self.CONSOLE_INFO)
        self.iLdapFormHomeDirectory.setFocus()
        return False
    attrs['homeDirectory'] = [Q2utf(self.iLdapFormHomeDirectory.text())]

#
# sambaSamAccount
#

if self.cLdapFormSamba.isChecked():
    attrs['objectClass'].extend(['sambaSamAccount'])
    attrs['uid'] = [Q2utf(self.iLdapFormUid.text())]

    sambaDomain = self.cbLdapFormSambaDomain.currentText().ascii()
    SID = self.ldap_wizard_sambaDomain_getSID(sambaDomain)
    uidNumber = self.ldap_wizard_sambaDomain_getUidNumber(sambaDomain)

    if not SID:
        self.console(self.__tr("SIDdn for domain '%1' is not configured.").arg(sambaDomain), self.CONSOLE_WARN)
        self.wsLdapForm.raiseWidget(2)
        return False

    if not uidNumber:
        self.console(self.__tr("uidNumber counter for domain '%1' is not configured.").arg(sambaDomain), self.CONSOLE_WARN)
        self.wsLdapForm.raiseWidget(2)
        return False

    #
    # default attributes
    #
    attrs['sambaLogonTime'] = ["0"]
    attrs['sambaLogoffTime'] = ["2147483647"]
    attrs['sambaKickoffTime'] = ["2147483647"]
    attrs['sambaPwdCanChange'] = ["0"]
    attrs['sambaAcctFlags'] = ["[U          ]"]
    attrs['sambaPwdLastSet'] = ["1"]

    #
    # sambaPwdMustChange
    #
    if self.iLdapFormSambaPwdMustChange.isChecked():
        attrs['sambaPwdMustChange'] = ["1"]
    else:
        attrs['sambaPwdMustChange'] = ["2147483647"]

    #
    # sambaSID / uidNumber
    #
    attrs['uidNumber'] = [uidNumber]
    attrs['sambaSID'] = ["%s-%s" % (SID, int(uidNumber) * 2 + 1000)]

    #
    # sambaPrimaryGroupSID / gidNumber
    #
    gidNumber = self.cbLdapFormPrimaryGroup.currentText().ascii().split("(")[1].split(")")[0]
    attrs['gidNumber'] = [gidNumber]
    attrs['sambaPrimaryGroupSID'] = ["%s-%s" % (SID, gidNumber)]

    #
    # sambaProfilePath
    #
    if self.cbLdapFormProfileType.currentItem() == 1:
        attrs['sambaProfilePath'] = [self.iLdapFormProfilePath.text().ascii()]

    #
    # sambaHomeDrive
    #
    if self.iLdapFormHomeDrive.text().ascii():
        attrs['sambaHomeDrive'] = [self.iLdapFormHomeDrive.text().ascii()]

    #
    # sambaHomePath
    #
    if self.iLdapFormDrivePath.text().ascii():
        attrs['sambaHomePath'] = [self.iLdapFormDrivePath.text().ascii()]

    #
    # sambaLogonScript
    #
    if self.iLdapFormLogonScript.text().ascii():
        attrs['sambaLogonScript'] = [self.iLdapFormLogonScript.text().ascii()]

    #
    # samba{NT-LM}Password
    #
    if self.iLdapFormUserP.text().ascii():
        attrs['sambaNTPassword'] = [smbpasswd.nthash(self.iLdapFormUserP.text().ascii())]
        attrs['sambaLMPassword'] = [smbpasswd.lmhash(self.iLdapFormUserP.text().ascii())]

#
# astSipGeneric
#

if self.cLdapFormAst.isChecked():
    attrs['objectClass'].extend(['astSipGeneric', 'astSipPeer'])
    attrs['astContext'] = ["from-sip"]
    attrs['astRegseconds'] = ["10"]
    attrs['astLanguage'] = ["en"]
    attrs['astHost'] = ["dynamic"]

    if not self.iLdapFormAstUsername.text().ascii():
        self.console(self.__tr("set user for asterisk."), self.CONSOLE_INFO)
        self.wsLdapForm.raiseWidget(3)
        self.iLdapFormAstUsername.setFocus()
        return False
    attrs['astUsername'] = [Q2utf(self.iLdapFormAstUsername.text())]

    if not self.iLdapFormAstName.text().ascii():
        self.console(self.__tr("set ramal for asterisk user."), self.CONSOLE_INFO)
        self.wsLdapForm.raiseWidget(3)
        self.iLdapFormAstName.setFocus()
        return False
    attrs['astName'] = [Q2utf(self.iLdapFormAstName.text())]

    if not self.iLdapFormAstPort.text().ascii():
        self.console(self.__tr("set port for asterisk."), self.CONSOLE_INFO)
        self.wsLdapForm.raiseWidget(3)
        self.iLdapFormAstPort.setFocus()
        return False
    attrs['astPort'] = [Q2utf(self.iLdapFormAstPort.text())]

    if self.iLdapFormUserP.text().ascii() and self.cLdapFormAstSecret.isChecked():
        attrs['astSecret'] = [Q2utf(self.iLdapFormUserP.text())]

#
# radiusProfile
#

if self.cLdapFormRadius.isChecked():
    attrs['objectClass'].extend(['radiusProfile'])
    if not self.iLdapFormRadiusGroup.text().ascii():
        self.console(self.__tr("set user radius group."), self.CONSOLE_INFO)
        self.wsLdapForm.raiseWidget(4)
        self.iLdapFormRadiusGroup.setFocus()
        return False
    attrs['radiusGroupName'] = [Q2utf(self.iLdapFormRadiusGroup.text())]

#
# Distinguist Name
#

dn = u"cn=%s,%s" % (Q2uni(self.iLdapFormCn.text()), self.ldap_current_dn())

if self.cbConfLdapUserDN.currentItem() == 1 and self.iLdapFormMail.text().ascii():
    dn = u"mail=%s,%s" % (Q2uni(self.iLdapFormMail.text()), self.ldap_current_dn())
elif self.cbConfLdapUserDN.currentItem() == 2 and self.cLdapFormPosix.isChecked():
    dn = u"uid=%s,%s" % (Q2uni(self.iLdapFormUid.text()), self.ldap_current_dn())

if self.ldap_add(dn, attrs):
    if self.cLdapFormSamba.isChecked():
        self.ldap_wizard_sambaDomain_incUidNumber(sambaDomain, uidNumber)
    self.korreio_module_clear("ldap.form")

}

void dKorreio::ldap_wizard_ou_apply() {

attrs = {}
if self.cbLdapFormUnit.currentItem() == 0:
    attrs['objectClass'] = ['organizationalUnit']
    attrs['ou'] = [Q2utf(self.iLdapFormUnit.text())]
    dn = u"ou=%s,%s" % (Q2uni(self.iLdapFormUnit.text()), self.ldap_current_dn())
else:
    attrs['objectClass'] = ['organization']
    attrs['o'] = [Q2utf(self.iLdapFormUnit.text())]
    dn = u"o=%s,%s" % (Q2uni(self.iLdapFormUnit.text()), self.ldap_current_dn())
if self.iLdapFormUnitStreet.text().ascii():
    attrs['street'] = [Q2utf(self.iLdapFormUnitStreet.text())]
if self.iLdapFormUnitL.text().ascii():
    attrs['l'] = [Q2utf(self.iLdapFormUnitL.text())]
if self.iLdapFormUnitPostalCode.text().ascii():
    attrs['postalCode'] = [Q2utf(self.iLdapFormUnitPostalCode.text())]
if self.iLdapFormUnitTelephoneNumber.text().ascii():
    attrs['telephoneNumber'] = [Q2utf(self.iLdapFormUnitTelephoneNumber.text())]

if self.ldap_add(dn, attrs):
    self.korreio_module_clear("ldap.form.unit")

}

void dKorreio::ldap_wizard_dhcp_type() {

item = self.cbLdapFormDhcpType.currentItem()

if item == 0:
    self.iLdapFormDhcpGroupName.setFocus()
    self.iLdapFormDhcpGroupName.setEnabled(True)
    self.tlLdapFormDhcpGroupName.setEnabled(True)
elif item == 1:
    item = 0
    self.iLdapFormDhcpName.setFocus()
    self.iLdapFormDhcpGroupName.setEnabled(False)
    self.tlLdapFormDhcpGroupName.setEnabled(False)
elif item == 2:
    item = 1
    self.cbLdapFormDhcpInterface.setFocus()
    self.cbLdapFormDhcpInterface.setEnabled(True)
    self.tlLdapFormDhcpInterface.setEnabled(True)
elif item == 3:
    item = 1
    self.iLdapFormDhcpNetwork.setFocus()
    self.cbLdapFormDhcpInterface.setEnabled(False)
    self.tlLdapFormDhcpInterface.setEnabled(False)

self.wsLdapDhcp.raiseWidget(item)

}

void dKorreio::ldap_wizard_dhcp_apply() {

attrs = {}

if self.cbLdapFormDhcpType.currentItem() < 2:
    #
    # dhcpGroup
    #

    if self.cbLdapFormDhcpType.currentItem() == 0:
        dn = u"cn=%s,%s" % (Q2uni(self.iLdapFormDhcpGroupName.text()), self.ldap_current_dn())
        attrs['objectClass'] = ['dhcpGroup']
        attrs['cn'] = [self.iLdapFormDhcpGroupName.text().ascii()]
        if not self.ldap_add(dn, attrs):
            return False
        dn = u"cn=%s,cn=%s,%s" % (Q2uni(self.iLdapFormDhcpName.text()), Q2uni(self.iLdapFormDhcpGroupName.text()), self.ldap_current_dn())
    else:
        dn = u"cn=%s,%s" % (Q2uni(self.iLdapFormDhcpName.text()), self.ldap_current_dn())

    #
    # dhcpHost
    #

    attrs['objectClass'] = ['dhcpHost']
    attrs['cn'] = [Q2utf(self.iLdapFormDhcpName.text())]
    attrs['dhcpStatements'] = ["fixed-address %s" % self.iLdapFormDhcpIPaddress.text().ascii()]
    attrs['dhcpHWAddress'] = ["ethernet %s" % self.iLdapFormDhcpMACaddress.text().ascii()]
    if self.iLdapFormDhcpComments.text().ascii():
        attrs['dhcpComments'] = [Q2utf(self.iLdapFormDhcpComments.text())]

else:
    #
    # dhcpSharedNetwork
    #

    if self.cbLdapFormDhcpType.currentItem() == 2:
        dn = u"cn=%s,%s" % (Q2uni(self.cbLdapFormDhcpInterface.currentText()), self.ldap_current_dn())
        attrs['objectClass'] = ['dhcpSharedNetwork']
        attrs['cn'] = [self.cbLdapFormDhcpInterface.currentText().ascii()]
        if not self.ldap_add(dn, attrs):
            return False
        dn = u"cn=%s,cn=%s,%s" % (Q2uni(self.iLdapFormDhcpNetwork.text()), Q2uni(self.cbLdapFormDhcpInterface.currentText()), self.ldap_current_dn())
    else:
        dn = u"cn=%s,%s" % (Q2uni(self.iLdapFormDhcpNetwork.text()), self.ldap_current_dn())

    #
    # dhcpSubnet
    #

    attrs = {}
    attrs['objectClass'] = ['dhcpSubnet', 'dhcpOptions']
    attrs['cn'] = [self.iLdapFormDhcpNetwork.text().ascii()]
    attrs['dhcpOption'] = ['routers %s' % self.iLdapFormDhcpGateway.text().ascii()]
    if self.iLdapFormDhcpRange.text().ascii():
        attrs['dhcpRange'] = [self.iLdapFormDhcpRange.text().ascii()]
    attrs['dhcpNetMask'] = [self.cbLdapFormDhcpNetmask.currentText().ascii()]

if self.ldap_add(dn, attrs):
    self.korreio_module_clear("ldap.form.dhcp")

}

void dKorreio::ldap_wizard_group_type() {

item = self.cbLdapFormGroupType.currentItem()
self.wsLdapGroup.raiseWidget(item)
self.iLdapFormGroupName.setFocus()

}

void dKorreio::ldap_wizard_group_getGidNumber() {

try:
    gidNumbers = self.ldap_query("(&(gidNumber=*)(!(objectClass=sambaUnixIdPool)))", ["gidNumber"])
    if gidNumbers:
        nextGidNumber = 999
        for item in gidNumbers:
            gidNumber = gidNumbers.get(item).get("gidNumber")[0]
            if int(gidNumber) > nextGidNumber:
                nextGidNumber = int(gidNumber)

        if nextGidNumber > 998:
            self.iLdapFormGroupGidNumber.setText("%s" % (nextGidNumber + 1))

except ldap.LDAPError, e:
    self.parse_exception("LDAPError", e)

}

void dKorreio::ldap_wizard_group_apply() {

groupName = Q2uni(self.iLdapFormGroupName.text())
if not groupName:
    self.console(self.__tr("set group name."), self.CONSOLE_WARN)
    self.iLdapFormGroupName.setFocus()
    return False

gidNumber = Q2utf(self.iLdapFormGroupGidNumber.text())
if self.cbLdapFormGroupType.currentItem() == 1:
    if not gidNumber:
        self.console(self.__tr("set gidNumber."), self.CONSOLE_WARN)
        self.iLdapFormGroupGidNumber.setFocus()
        return False

dn = u"cn=%s,%s" % (groupName, self.ldap_current_dn())
attrs = {}
attrs['objectClass'] = ['posixGroup']
attrs['cn'] = [groupName.encode('utf-8')]
description = Q2utf(self.iLdapFormGroupDesc.text())
if description:
    attrs['description'] = [description]
else:
    attrs['description'] = attrs.get('cn')
attrs['gidNumber'] = [gidNumber]

if self.cbLdapFormGroupType.currentItem() == 0:
    #
    # sambaGroupMapping
    #

    sambaDomain = self.cbLdapFormGroupSIDdn.currentText().ascii()
    if not sambaDomain:
        self.console(self.__tr("set netbios domains."), self.CONSOLE_WARN)
        self.cbLdapFormGroupSIDdn.setFocus()
        return False

    attrs['objectClass'].append("sambaGroupMapping")
    attrs['displayName'] = attrs.get('description')
    gidNumber = self.ldap_wizard_sambaDomain_getGidNumber(sambaDomain)
    attrs['gidNumber'] = [gidNumber]
    if self.cbLdapFormGroupSmbType.currentItem() == 0:
        SID = self.ldap_wizard_sambaDomain_getSID(sambaDomain)
        attrs['sambaGroupType'] = ["2"]
        attrs['sambaSID'] = ["%s-%s" % (SID, gidNumber)]
    else:
        attrs['sambaGroupType'] = ["5"]
        attrs['sambaSID'] = ["S-1-5-32-%s" % gidNumber]

if self.ldap_add(dn, attrs):
    if self.cbLdapFormGroupType.currentItem() == 0:
        self.ldap_wizard_sambaDomain_incGidNumber(sambaDomain, gidNumber)
    self.korreio_module_clear("ldap.form.group")

}

void dKorreio::ldap_add( a0, a1 ) {
# a0=unicode:DN, a1=list:attrs

dn = a0
attrs = a1

try:

    #
    # Ldap Add
    #

    ldif = modlist.addModlist(attrs)
    l = self.ldap_connect()
    l.add_s(dn.encode('utf-8'), ldif)

    #
    # Update Ldap Cache and Interface
    #

    self.ldap_cache.add(dn.lower(), ldif)
    self.ldap_items.add(dn)

    #
    # Message
    #

    self.console(self.__tr("entry '%1' added.").arg(dn))
    return True

except ldap.LDAPError, e:
    self.parse_exception("LDAPError", e)
    return False

}

void dKorreio::ldap_modify( a0, a1, a2 ) {
# a0=unicode:dn, a1=list:attrold, a2=list:attrnew

dn = a0

try:

    #
    # Ldap Modify
    #

    ldif = modlist.modifyModlist(a1, a2)
    if not ldif:
        return True
    l = self.ldap_connect()
    l.modify_s(dn.encode('utf-8'), ldif)

    #
    # Update Ldap Cache
    #

    self.ldap_cache.modify(dn.lower(), ldif)

    self.console(self.__tr("entry '%1' modified.").arg(a0))
    return True

except ldap.LDAPError, e:
    self.parse_exception("LDAPError", e)
    return False

}

void dKorreio::ldap_modify_clicked() {

if self.lvLdap.currentItem() is None:
    self.console(self.__tr("select a entry."), self.CONSOLE_WARN)
    return True

#
# Modify current entry
#

if self.ldap_add_or_modify:
    attrsOld = {}

    for attribute, values in self.ldap_cache.get(self.ldap_current_dn().lower()).items():
        if attrsOld.get(attribute):
           attrsOld[attribute].extend(values)
        else:
           attrsOld[attribute] = values

    attrsNew = {}
    item = self.lvLdapAttr.firstChild()
    while item is not None:
        if item.isVisible():
            if attrsNew.get(Q2utf(item.text(0))):
                attrsNew[Q2utf(item.text(0))].extend([Q2utf(item.text(1))])
            else:
                attrsNew[Q2utf(item.text(0))] = [Q2utf(item.text(1))]
        item = item.nextSibling()

    if self.ldap_modify(self.ldap_current_dn(), attrsOld, attrsNew):
        self.ldap_dn_clicked()

#
# Add current entry
#

else:
    attrs = {}
    dn = u""
    dnParent = self.ldap_current_dn()

    item = self.lvLdapAttr.firstChild()
    while item is not None:
        if item.isVisible():
            if item.isSelected():
                if dnParent is None:
                    dn = u"%s=%s" % (Q2uni(item.text(0)), Q2uni(item.text(1)))
                else:
                    dn = u"%s=%s,%s" % (Q2uni(item.text(0)), Q2uni(item.text(1)), dnParent)
            if attrs.get(Q2utf(item.text(0))):
                attrs[Q2utf(item.text(0))].extend([Q2utf(item.text(1))])
            else:
                attrs[Q2utf(item.text(0))] = [Q2utf(item.text(1))]
        item = item.nextSibling()

    if dn == u"":
        self.console(self.__tr("select a attribute to be DN."), self.CONSOLE_WARN)
        return False

    try:
        if attrs.get('ref'):
            refCheckList = attrs.get('ref')[0].split('/', 3)[3].split('?')[0].split(',')[0].split('=')
            item = self.lvLdapAttr.firstChild()
            refCheckBool = False
            while item is not None:
                if item.text(0).ascii() == refCheckList[0] and item.text(1).ascii() == re.sub("%20", " ", refCheckList[1]):
                    refCheckBool = True
                    dn = u"%s=%s,%s" % (Q2uni(item.text(0)), Q2uni(item.text(1)), dnParent)
                item = item.nextSibling()
            if not refCheckBool:
                self.console(self.__tr("referral dn '%1' don't present in this entry.").arg("%s=%s" % (refCheckList[0], refCheckList[1])), self.CONSOLE_INFO)
                return False
    except:
        self.console("invalid referral sintaxe", self.CONSOLE_WARN)
        return False

    self.ldap_add(dn, attrs)
    self.ldap_add_or_modify = True
    self.ldap_dn_clicked()

}

void dKorreio::ldap_attr_edit() {

self.ldap_attr_modify = u"%s=%s" % (Q2uni(self.lvLdapAttr.currentItem().text(0)), Q2uni(self.lvLdapAttr.currentItem().text(1)))
if Q2uni(self.lvLdap.currentItem().text(0)) != self.ldap_attr_modify:
    self.lvLdapAttr.currentItem().setRenameEnabled(1, True)
    self.lvLdapAttr.currentItem().startRename(1)

}

void dKorreio::ldap_attr_edit_renamed() {

item = self.lvLdapAttr.currentItem()
attr = Q2uni(item.text(0))
value = Q2uni(item.text(1))
if self.ldap_attr_modify != u"%s=%s" % (attr, value):
    self.lvLdapAttr.currentItem().setVisible(False)
    item = QListViewItemLdapAttrColored(self.lvLdapAttr)
    item.setText(0, attr)
    item.setText(1, value)
}

void dKorreio::ldap_del_entry( a0 ) {
# a0=str:dn

dn = a0

try:
    l = self.ldap_connect()
    if 'ref' in self.ldap_cache.get(dn.lower()).keys():
        l.manage_dsa_it(True)
    l.delete_s(dn.encode('utf-8'))
    l.manage_dsa_it(False)
    self.ldap_cache.unset(dn.lower())
    return True
except ldap.LDAPError, e:
    self.parse_exception("LDAPError", e)
    return False
except KeyError, e:
    return True

}

void dKorreio::ldap_remove( a0 ) {

dn = a0

def __ldap_remove(dn):
    item = self.ldap_items.get(dn.lower())
    subitem = item.firstChild()
    while subitem is not None:
        __ldap_remove(u"%s,%s" % (Q2uni(subitem.text(0)), dn))
        subitem = subitem.nextSibling()

    if self.ldap_del_entry(dn):
        self.console(self.__tr("entry '%1' deleted.").arg(dn))
        return True
    else:
        self.console(self.__tr("can't delete entry '%1'.").arg(dn), self.CONSOLE_ERR)
        return False

if __ldap_remove(dn):
    item = self.ldap_items.get(dn.lower())
    if item.parent() is None:
        self.korreio_module_clear("ldap")
    else:
        item.parent().takeItem(item)
        self.lvLdap.currentItem().setSelected(True)

}

void dKorreio::ldap_remove_entry() {

dn =  self.ldap_current_dn()

if dn is not None:
    ok = QMessageBox.information( None, self.__tr("Confirm!"), self.__tr("Confirm entry deletion:\n\n    - %1\n\n").arg(dn), self.__tr("&Yes"), self.__tr("&No"))
    if ok == 0:
        self.ldap_remove(dn)
        self.ldap_dn_clicked()
    else:
        self.console(self.__tr("entry '%1' deletion been aborted.").arg(dn), self.CONSOLE_INFO)

}

void dKorreio::ldap_copy( a0, a1, a2 ) {

dnFrom = a0
dnTo = a1
rdnTo = a2

attrs = self.ldap_cache.get(dnFrom.lower())
if rdnTo is None:
    rdnTo = u"%s,%s" % (dnFrom.split(u",")[0], dnTo)
else:
    rdnToList = rdnTo.split(u"=")
    rdnTo = u"%s,%s" % (rdnTo, dnTo)
    attrs[rdnToList[0].encode('utf-8')] = [rdnToList[1].encode('utf-8')]

if self.ldap_add(rdnTo, attrs):
    item = self.ldap_items.get(dnFrom.lower()).firstChild()
    while item is not None:
        self.ldap_copy(u"%s,%s" % (Q2uni(item.text(0)), dnFrom), rdnTo, None)
        self.ldap_items.get(rdnTo.lower()).setOpen(False)
        item = item.nextSibling()
    return True
else:
    return False

}

void dKorreio::ldap_remove_attr() {

itemAttr = self.lvLdapAttr.currentItem()
if itemAttr is None:
    return

if self.cConfLdapSchemaDelAttr.isChecked() and itemAttr.text(0).ascii().lower() == 'objectclass':
    depends = []

    item=self.lvConfLdapSchema.firstChild().firstChild()
    while item is not None:
        if item.text(0).ascii().lower() == itemAttr.text(1).ascii().lower():
            subitem=item.firstChild()
            while subitem is not None:
                if subitem.text(0).ascii().lower() != 'objectclass':
                    depends.append(subitem.text(0).ascii())
                subitem=subitem.nextSibling()
            break
        item=item.nextSibling()

    item = self.lvLdapAttr.firstChild()
    while item is not None:
        if item.text(0).ascii() in depends:
            tmp=item
            item=item.nextSibling()
            self.lvLdapAttr.takeItem(tmp)
        else:
            item=item.nextSibling()

self.lvLdapAttr.takeItem(itemAttr)

try:
    self.lvLdapAttr.currentItem().setSelected(True)
except AttributeError, e:
    pass

}

void dKorreio::ldap_add_attr() {

attr = self.cbLdapAttr.currentText().ascii()
value = self.cbLdapValue.currentText().ascii()
if not attr:
    return True

attrs = [(attr,value)]

if self.cConfLdapSchemaAddAttr.isChecked() and attr.lower() == 'objectclass':
    if not value:
        self.console(self.__tr("set objectClass name."), self.CONSOLE_INFO)
        self.cbLdapValue.setFocus()
        return True
    item = self.lvConfLdapSchema.firstChild().firstChild()
    while item is not None:
        subitem = item.firstChild()
        if item.text(0).ascii().lower() == value.lower():
            while subitem is not None:
                if subitem.text(1).ascii() is None:
                    newvalue = ""
                else:
                    newvalue = subitem.text(1).ascii()
                attrs.extend([(subitem.text(0).ascii(), newvalue)])
                subitem = subitem.nextSibling()
            break
        item = item.nextSibling()

for attr, value in attrs:
    item = self.lvLdapAttr.firstChild()
    jump = False
    while item is not None:
        if item.text(0).ascii() == attr and (item.text(1).ascii() == value or value == ''):
            jump = True
            break
        item = item.nextSibling()
    if jump:
        continue
    item = QListViewItemLdapAttrColored(self.lvLdapAttr)
    item.setText(0, attr)
    item.setText(1, value)

if not self.ldap_add_or_modify:
    self.console(self.__tr("selected attribute will be Distinguish Name."), self.CONSOLE_INFO)

}

void dKorreio::ldap_edit_rdn() {

#
# Save current dn name and start edition
#

if self.lvLdap.currentItem().parent() is not None:
    self.ldap_dn_rename = self.ldap_current_dn()
    self.lvLdap.currentItem().setRenameEnabled(0,True)
    self.lvLdap.currentItem().startRename(0)

}

void dKorreio::ldap_rename_rdn() {

item = self.lvLdap.currentItem()
ldap_dn_new = Q2uni(item.text(0))

#
# Verify change
#

if self.ldap_dn_rename.split(u",")[0].lower() == ldap_dn_new.lower():
    item.setText(0, self.ldap_dn_rename.split(u",")[0])
    return True

#
# If new dn attribute does not already existed, then delete old dn attribute
#

delete_dn_attribute = 1
item = self.lvLdapAttr.firstChild()
while item is not None:
    searchDn = u"%s=%s" % (Q2uni(item.text(0)), Q2uni(item.text(1)))
    if searchDn.lower() == ldap_dn_new.lower():
        delete_dn_attribute = 0
        break
    item = item.nextSibling()

if self.ldap_modify_rdn(self.ldap_dn_rename, ldap_dn_new, None, delete_dn_attribute):
    self.console(self.__tr("entry Distinguish Name '%1' changed.").arg(self.ldap_dn_rename))
else:
    self.lvLdap.currentItem().setText(0, self.ldap_dn_rename.split(u",")[0])
    return False

self.ldap_dn_clicked()
return True

}

void dKorreio::ldap_modify_rdn( a0, a1, a2, a3 ) {
# a0=str:ldap_dn_rename, a1=str:ldap_dn_new, a2=str:newParent or None, a3=delete_dn_attribute

ldap_dn_rename = a0
ldap_dn_new = a1
delete_dn_attribute = a3
if a2 is None:
    newParent = None
else:
    newParent = a2.encode('utf-8')

try:
    l = self.ldap_connect()

#
# Change DN
#

    l.rename_s(ldap_dn_rename.encode('utf-8'), ldap_dn_new.encode('utf-8'), newParent, delete_dn_attribute)

#
# Update Ldap Attrs Cache and Interface
#

    if newParent is None:
        dn = u",".join(ldap_dn_rename.split(u",")[1:])
    else:
        dn = newParent
    dn = u"%s,%s" % (ldap_dn_new, dn)

    self.ldap_items.rename(ldap_dn_rename.lower(), dn.lower())
    self.ldap_cache.rename(ldap_dn_rename.lower(), dn.lower())
    if delete_dn_attribute == 1:
        self.ldap_cache.removeAttr(dn.lower(), ldap_dn_rename.split(u",")[0].encode('utf-8'))
        self.ldap_cache.addAttr(dn.lower(), ldap_dn_new.encode('utf-8'))
    return True

except ldap.LDAPError, e:
    self.parse_exception("LDAPError", e)
    return False


}

void dKorreio::ldap_passwd() {

if not self.cbLdapUserPassword.isChecked() and not self.cbLdapSambaPassword.isChecked() and not self.cbLdapAstPassword.isChecked() and not self.cLdapSambaPasswordPwdMustChange.isChecked():
    self.console(self.__tr("select at least one option."), self.CONSOLE_WARN)
    return False

if self.iLdapPasswd.text().ascii() != self.iLdapPasswd2.text().ascii():
    self.console(self.__tr("passwords don't match, type again."), self.CONSOLE_WARN)
    self.korreio_module_clear("ldap.form.password")
    return False

#
# Salt method
#

def __salt(mode):
    salt = ''
    if mode == 2:
        for i in [0,1]:
            salt += choice(letters+digits)
        return salt
    elif mode == 16:
        for i in range(16):
            salt += choice(letters+digits)
        return salt
    else:
        self.console(self.__tr("unsupported salt."), self.CONSOLE_ERR)
        raise "KORREIOError", "unsupported salt"

#
# Init variables
#

dn = self.ldap_current_dn()
attrOld={}
attrNew={}

#
# userPassword
#

if self.cbLdapUserPassword.isChecked():
    if self.ldap_cache.get(dn.lower()).get("userPassword"):
        attrOld["userPassword"] = 'None'
    hash = self.cbUserPassword.currentItem()
    password = Q2utf(self.iLdapPasswd.text())
    salt = __salt(16)
    if hash == 0:
        attrNew["userPassword"] = ["{SSHA}%s" % b2a_base64(sha.new(password + salt).digest() + salt).strip()]
    elif hash == 1:
        attrNew["userPassword"] = ["{SMD5}%s" % b2a_base64(md5.new(password + salt).digest() + salt).strip()]
    elif hash == 2:
        if "crypt" in self.module_failed:
            self.console(self.__tr("unavailable hash."), self.CONSOLE_ERR)
	return True
        salt = __salt(2)
        attrNew["userPassword"] = ["{CRYPT}%s" % crypt.crypt(password, salt)]
    elif hash == 3:
        attrNew["userPassword"] = ["{SHA}%s" % b2a_base64(sha.new(password).digest()).strip()]
    elif hash == 4:
        attrNew["userPassword"] = ["{MD5}%s" % b2a_base64(md5.new(password).digest()).strip()]
    elif hash == 5:
        attrNew["userPassword"] = [password]
    else:
        self.console(self.__tr("unsupported hash."), self.CONSOLE_ERR)
        raise "KORREIOError", "unsupported hash"

if self.cbLdapSambaPassword.isChecked():

    #
    # sambaNTPassword
    #

    if self.ldap_cache.get(dn.lower()).get("sambaNTPassword"):
        attrOld["sambaNTPassword"] = 'None'
    attrNew["sambaNTPassword"] = [smbpasswd.nthash(self.iLdapPasswd.text().ascii())]

    #
    # sambaLMPassword
    #

    if self.ldap_cache.get(dn.lower()).get("sambaLMPassword"):
        attrOld["sambaLMPassword"] = 'None'
    attrNew["sambaLMPassword"] = [smbpasswd.lmhash(self.iLdapPasswd.text().ascii())]

#
# sambaPwdMustChange
#

if self.cLdapSambaPasswordPwdMustChange.isChecked():
    if self.ldap_cache.get(dn.lower()).get("sambaPwdMustChange"):
        attrOld["sambaPwdMustChange"] = 'None'
    attrNew["sambaPwdMustChange"] = ["1"]

#
# astSecret
#

if self.cbLdapAstPassword.isChecked():
    if self.ldap_cache.get(dn.lower()).get("astSecret"):
        attrOld["astSecret"] = 'None'
    attrNew["astSecret"] = [self.iLdapPasswd.text().ascii()]

#
# Set Passwords
#

if self.ldap_modify(dn, attrOld, attrNew):
    self.iLdapPasswd.clear()
    self.iLdapPasswd2.clear()
    self.iLdapPasswd.setFocus()

#
# Show entry
#

self.ldap_dn_clicked()

}

void dKorreio::ldap_samba_populate() {

dnPopulate = self.ldap_current_dn()

domain = Q2uni(self.iLdapSMBdomain.text())
if not domain:
    self.console(self.__tr("set netbios domain name."), self.CONSOLE_WARN)
    self.iLdapSMBdomain.setFocus()
    return False

SID = self.iLdapSMBSID.text().ascii()
if not SID:
    self.console(self.__tr("set netbios domain SID."), self.CONSOLE_WARN)
    self.iLdapSMBSID.setFocus()
    return False

passwd = self.iLdapSMBpassword.text().ascii()
if not passwd:
    self.console(self.__tr("set uid=root password."), self.CONSOLE_WARN)
    self.iLdapSMBpassword.setFocus()
    return False


#
# Units
#

units = []
units.append("Users")
units.append("Groups")
units.append("Computers")
units.append("Idmap")

for unit in units:
    dn = u"ou=%s,%s" % (unicode(unit), dnPopulate)
    attrs = {}
    attrs['objectClass'] = ['organizationalUnit']
    attrs['ou'] = [unit]
    if not self.ldap_add(dn, attrs):
        self.console(self.__tr("can't add entry '%1'.").arg(dn), self.CONSOLE_ERR)

#
# Groups
#

groups = []
groups.append(["2", "512", "Domain Admins", "Netbios Domain Administrators"])
groups.append(["2", "513", "Domain Users", "Netbios Domain Users"])
groups.append(["2", "514", "Domain Guests", "Netbios Domain Guests Users"])
groups.append(["2", "515", "Domain Computers", "Netbios Domain Computers accounts"])
groups.append(["5", "544", "Administrators", "Netbios Domain Members can fully administer the computer/sambaDomainName"])
groups.append(["5", "548", "Account Operators", "Netbios Domain Users to manipulate users accounts"])
groups.append(["5", "550", "Print Operators", "Netbios Domain Print Operators"])
groups.append(["5", "551", "Backup Operators", "Netbios Domain Members can bypass file security to back up files"])
groups.append(["5", "552", "Replicators", "Netbios Domain Supports file replication in a sambaDomainName"])

for group in groups:
    dn = u"cn=%s,ou=Groups,%s" % (group[2], dnPopulate)
    attrs = {}
    attrs['objectClass'] = ['posixGroup', 'sambaGroupMapping']
    attrs['gidNumber'] = [group[1]]
    attrs['cn'] = [group[2]]
    attrs['displayName'] = [group[2]]
    if group[1] == '512':
        attrs['memberUid'] = ['root']
    attrs['description'] = [group[3]]
    attrs['sambaGroupType'] = [group[0]]
    if group[0] == "2":
        attrs['sambaSID'] = ["%s-%s" % (SID, group[1])]
    elif group[0] == "5":
        attrs['sambaSID'] = ["S-1-5-32-%s" % group[1]]
    if not self.ldap_add(dn, attrs):
        self.console(self.__tr("can't add entry '%1'.").arg(dn), self.CONSOLE_ERR)

#
# Users: Root
#

dn = u"uid=root,ou=Users,%s" % dnPopulate
attrs = {}
attrs['objectClass'] = ['inetOrgPerson', 'sambaSamAccount','posixAccount','shadowAccount']
attrs['cn'] = ["Administrador"]
attrs['sn'] = ["root"]
attrs['uidNumber'] = ["0"]
attrs['gidNumber'] = ["0"]
attrs['uid'] = ["root"]
attrs['homeDirectory'] = ["/root"]
attrs['loginShell'] = ["/bin/false"]
attrs['gecos'] = ["Netbios Domain Administrator"]
attrs['sambaLogonTime'] = ["0"]
attrs['sambaLogoffTime'] = ["2147483647"]
attrs['sambaKickoffTime'] = ["2147483647"]
attrs['sambaPwdMustChange'] = ["2147483647"]
attrs['sambaPwdCanChange'] = ["0"]
attrs['sambaHomeDrive'] = ["H:"]
attrs['sambaHomePath'] = ["\\\\server\\root"]
attrs['sambaProfilePath'] = ["\\\\server\\profiles\\root"]
attrs['sambaSID'] = ["%s-500" % SID]
attrs['sambaPrimaryGroupSID'] = ["%s-512" % SID]
attrs['sambaNTPassword'] = [smbpasswd.nthash(passwd)]
attrs['sambaLMPassword'] = [smbpasswd.lmhash(passwd)]
attrs['sambaAcctFlags'] = ["[U          ]"]
attrs['sambaPwdLastSet'] = ["1"]
salt = ''
for i in range(16):
    salt += choice(letters+digits)
attrs["userPassword"] = ["{SSHA}%s" % b2a_base64(sha.new("%s%s" % (passwd, salt)).digest() + salt).strip()]

if not self.ldap_add(dn, attrs):
    self.console(self.__tr("can't add entry '%1'.").arg(dn), self.CONSOLE_ERR)

#
# Users: Nobody
#

dn = u"uid=nobody,ou=Users,%s" % dnPopulate
attrs = {}
attrs['objectClass'] = ['inetOrgPerson', 'sambaSamAccount','posixAccount','shadowAccount']
attrs['cn'] = ["nobody"]
attrs['sn'] = ["nobody"]
attrs['uidNumber'] = ["999"]
attrs['gidNumber'] = ["514"]
attrs['uid'] = ["nobody"]
attrs['homeDirectory'] = ["/dev/null"]
attrs['loginShell'] = ["/bin/false"]
attrs['sambaLogonTime'] = ["0"]
attrs['sambaLogoffTime'] = ["2147483647"]
attrs['sambaKickoffTime'] = ["2147483647"]
attrs['sambaPwdMustChange'] = ["2147483647"]
attrs['sambaPwdCanChange'] = ["0"]
attrs['sambaHomeDrive'] = ["H:"]
attrs['sambaHomePath'] = ["\\\\server\\nonexistent"]
attrs['sambaProfilePath'] = ["\\\\server\\profiles\\nonexistent"]
attrs['sambaSID'] = ["%s-2998" % SID]
attrs['sambaPrimaryGroupSID'] = ["%s-514" % SID]
attrs['sambaNTPassword'] = ["NO PASSWORDXXXXXXXXXXXXXXXXXXXXX"]
attrs['sambaLMPassword'] = ["NO PASSWORDXXXXXXXXXXXXXXXXXXXXX"]
attrs['sambaAcctFlags'] = ["[NUD        ]"]
attrs['sambaPwdLastSet'] = ["0"]
if not self.ldap_add(dn, attrs):
    self.console(self.__tr("can't add entry '%1'.").arg(dn), self.CONSOLE_ERR)

#
# sambaDomain
#

# from: 0=m, 1=h, 2=d
def time2seconds(value, type):
    valueInt=int(value)
    if type == 0:
        return "%i" % (valueInt * 60)
    elif type == 1:
        return "%i" % (valueInt * 60 * 60)
    elif type == 2:
        return "%i" % (valueInt * 24 * 60 * 60)
    else:
        raise "KORREIOError", "ERROR: unexpected time mode."

def time2minutes(value, type):
    valueInt=int(value)
    if type == 0:
        return "%i" % (valueInt)
    elif type == 1:
        return "%i" % (valueInt * 60)
    elif type == 2:
        return "%i" % (valueInt * 24 * 60)
    else:
        raise "KORREIOError", "ERROR: unexpected time mode."

dn = u"sambaDomainName=%s,%s" % (domain.upper(), dnPopulate)
attrs = {}
attrs['objectClass'] = ['sambaDomain', 'sambaUnixIdPool']
attrs['sambaDomainName'] = [domain.upper().encode('utf-8')]
attrs['sambaSID'] = [SID]
attrs['uidNumber'] = [self.iLdapSMBuidNumber.text().ascii()]
attrs['gidNumber'] = [self.iLdapSMBgidNumber.text().ascii()]
attrs['sambaAlgorithmicRidBase'] = ["1000"]
attrs['sambaNextUserRid'] = ["1000"]
attrs['sambaNextRid'] = ["1000"]
attrs['sambaMinPwdLength'] = [self.iLdapSMBminPwdLength.text().ascii()]
attrs['sambaPwdHistoryLength'] = [self.iLdapSMBpwdHistLenght.text().ascii()]
attrs['sambaMaxPwdAge'] = [time2seconds(self.iLdapSMBmaxPwdAge.text().ascii(), self.cbLdapSMBmaxPwdAge.currentItem())]
attrs['sambaMinPwdAge'] = [time2seconds(self.iLdapSMBminPwdAge.text().ascii(), self.cbLdapSMBminPwdAge.currentItem())]
attrs['sambaLockoutThreshold'] = [self.iLdapSMBlockout.text().ascii()]
attrs['sambaLockoutDuration'] = [time2minutes(self.iLdapSMBlockoutDuration.text().ascii(), self.cbLdapSMBlockoutDuration.currentItem())]
attrs['sambaLockoutObservationWindow'] = [time2minutes(self.iLdapSMBlockoutWindow.text().ascii(), self.cbLdapSMBlockoutWindow.currentItem())]
attrs['sambaLogonToChgPwd'] = ["0"]
attrs['sambaForceLogoff'] = ["-1"]
attrs['sambaRefuseMachinePwdChange'] = ["0"]
if not self.ldap_add(dn, attrs):
    self.console(self.__tr("can't add entry '%1'.").arg(dn), self.CONSOLE_ERR)

self.lvLdap.currentItem().setOpen(True)

}

void dKorreio::ldap_dhcp_populate() {

# from: 0=s, 1=m, 2=h, 3=d
def __time2sec(value, mode):
    try:
        valueInt = int(value)
    except:
        return "0"
    if mode == 0:
        return value
    elif mode == 1:
        return "%i" % (valueInt * 60)
    elif mode == 2:
        return "%i" % (valueInt * 60 * 60)
    elif mode == 3:
        return "%i" % (valueInt * 24 * 60 * 60)
    else:
        raise "KORREIOError", "ERROR: unexpected time mode."

serverName = Q2uni(self.iLdapDhcpName.text())
if not serverName:
    self.console(self.__tr("set server name."), self.CONSOLE_WARN)
    self.iLdapDhcpName.setFocus()
    return False

sharedName = Q2uni(self.cbLdapDhcpInterface.currentText())
if not sharedName:
    self.console(self.__tr("set shared network name."), self.CONSOLE_WARN)
    self.cbLdapDhcpInterface.setFocus()
    return False

networkAddress = Q2uni(self.iLdapDhcpNetwork.text())
if not networkAddress:
    self.console(self.__tr("set network address."), self.CONSOLE_WARN)
    self.iLdapDhcpNetwork.setFocus()
    return False

#
# dhcpServer
#

dnPopulate = self.ldap_current_dn()
dn = u"cn=%s,%s" % (serverName, dnPopulate)
attrs = {}
attrs['objectClass'] = ['dhcpServer']
attrs['cn'] = [serverName.encode('utf-8')]
attrs['dhcpServiceDN'] = ['cn=dhcpConfig,%s' % dn.encode('utf-8')]

if not self.ldap_add(dn, attrs):
    self.console(self.__tr("can't add entry '%1'.").arg(dn), self.CONSOLE_ERR)

#
# dhcpService
#

dnPopulate = self.ldap_current_dn()
dn = u"cn=dhcpConfig,cn=%s,%s" % (serverName, dnPopulate)
attrs = {}
attrs['objectClass'] = ['dhcpService']
attrs['cn'] = ['dhcpConfig']
attrs['dhcpPrimaryDN'] = ["cn=%s,%s" % (serverName.encode('utf-8'), dnPopulate.encode('utf-8'))]
attrs['dhcpStatements'] = ['ddns-update-style none', 'authoritative']
if self.iLdapDhcpDefaultLeaseTime.text().ascii():
    attrs['dhcpStatements'].append('default-lease-time %s' % __time2sec(self.iLdapDhcpDefaultLeaseTime.text().ascii(), self.cbLdapDhcpDefaultLeaseTime.currentItem()))
if self.iLdapDhcpMaxLeaseTime.text().ascii():
    attrs['dhcpStatements'].append('max-lease-time %s' % __time2sec(self.iLdapDhcpMaxLeaseTime.text().ascii(), self.cbLdapDhcpMaxLeaseTime.currentItem()))
attrs['dhcpOption'] = []
if self.iLdapDhcpDomainName.text().ascii():
    attrs['dhcpOption'].append('domain-name "%s"' % self.iLdapDhcpDomainName.text().ascii())
if self.iLdapDhcpDNSservers.text().ascii():
    attrs['dhcpOption'].append('domain-name-servers %s' % self.iLdapDhcpDNSservers.text().ascii())
if self.iLdapDhcpNetbiosServers.text().ascii():
    attrs['dhcpOption'].append('netbios-name-servers %s' % self.iLdapDhcpNetbiosServers.text().ascii())
if attrs['dhcpOption']:
    attrs['objectClass'].append('dhcpOptions')
else:
    del attrs['dhcpOption']

if not self.ldap_add(dn, attrs):
    self.console(self.__tr("can't add entry '%1'.").arg(dn), self.CONSOLE_ERR)

#
# dhcpSharedNetwork
#

dn = "cn=%s,cn=dhcpConfig,cn=%s,%s" % (sharedName, serverName, dnPopulate)
attrs = {}
attrs['objectClass'] = ['dhcpSharedNetwork']
attrs['cn'] = [sharedName.encode('utf-8')]

if not self.ldap_add(dn, attrs):
    self.console(self.__tr("can't add entry '%1'.").arg(dn), self.CONSOLE_ERR)

#
# dhcpSubnet
#

dn = u"cn=%s,cn=%s,cn=dhcpConfig,cn=%s,%s" % (networkAddress, sharedName, serverName, dnPopulate)
attrs = {}
attrs['objectClass'] = ['dhcpSubnet']
attrs['cn'] = [networkAddress.encode('utf-8')]
if self.iLdapDhcpGateway.text().ascii():
    attrs['objectClass'].append('dhcpOptions')
    attrs['dhcpOption'] = ['routers %s' % self.iLdapDhcpGateway.text().ascii()]
if self.iLdapDhcpRange.text().ascii():
    attrs['dhcpRange'] = [self.iLdapDhcpRange.text().ascii()]
attrs['dhcpNetMask'] = [self.cbLdapDhcpNetmask.currentText().ascii()]

if not self.ldap_add(dn, attrs):
    self.console(self.__tr("can't add entry '%1'.").arg(dn), self.CONSOLE_ERR)

}

void dKorreio::ldap_menu() {

self.korreio_update_servers_menu()

if a0 is None:
    self.lvLdapMenu.setItemEnabled(10, False)
    self.lvLdapMenu.setItemEnabled(20, False)
    self.lvLdapMenu.setItemEnabled(21, False)
    self.lvLdapMenu.setItemEnabled(22, False)
    self.lvLdapMenu.setItemEnabled(23, False)
    self.lvLdapMenu.setItemEnabled(24, False)
    self.lvLdapMenu.setItemEnabled(30, False)
    self.lvLdapMenu.setItemEnabled(40, False)
    self.lvLdapMenu.setItemEnabled(41, False)
else:
    self.lvLdapMenu.setItemEnabled(10, True)
    if self.ldap_cache.get(self.ldap_current_dn().lower()):
        self.lvLdapMenu.setItemEnabled(20, True)
        self.lvLdapMenu.setItemEnabled(21, True)
        self.lvLdapMenu.setItemEnabled(22, True)
        self.lvLdapMenu.setItemEnabled(24, True)
        self.lvLdapMenu2.setItemEnabled(31, True)
    else:
        self.lvLdapMenu.setItemEnabled(20, False)
        self.lvLdapMenu.setItemEnabled(21, False)
        self.lvLdapMenu.setItemEnabled(22, False)
        self.lvLdapMenu.setItemEnabled(24, False)
        self.lvLdapMenu2.setItemEnabled(31, False)
    if self.lvLdapMenu.clipBoard == u"None" or not self.ldap_cache.get(self.lvLdapMenu.clipBoard.lower()):
        self.lvLdapMenu.setItemEnabled(23, False)
    else:
        self.lvLdapMenu.setItemEnabled(23, True)
    self.lvLdapMenu.setItemEnabled(30, True)
    self.lvLdapMenu.setItemEnabled(40, True)
    self.lvLdapMenu.setItemEnabled(41, True)

self.lvLdapMenu.popup(a1)

}

void dKorreio::ldap_menu_attr() {

self.lvLdapMenuAttr = QPopupMenu(self)
self.lvLdapMenuAttr.insertItem(QIconSet(self.icons.get("editdelete.png")), self.__tr('Delete attribute'), 54)
self.lvLdapMenuAttr.insertSeparator()

if self.cConfLdapSchemaAddAttr.isChecked():
    self.lvLdapMenuAttr.insertItem(QIconSet(self.icons.get("ok.png")), self.__tr('Use Auxiliary Schema on Add'), 50)
else:
    self.lvLdapMenuAttr.insertItem(self.__tr('Use Auxiliary Schema on Add'), 50)

if self.cConfLdapSchemaDelAttr.isChecked():
    self.lvLdapMenuAttr.insertItem(QIconSet(self.icons.get("ok.png")), self.__tr('Use Auxiliary Schema on Delete'), 51)
else:
    self.lvLdapMenuAttr.insertItem(self.__tr('Use Auxiliary Schema on Delete'), 51)

self.connect(self.lvLdapMenuAttr, SIGNAL('activated(int)'), self.ldap_menu_clicked)

self.lvLdapMenuAttr.popup(a1)

}

void dKorreio::ldap_menu_clicked( a0 ) {
# a0=int:menuItem

menuItem = a0
dn = self.ldap_current_dn()

if menuItem == 11:
    #
    # Add new entry
    #
    self.ldap_add_or_modify = False
    self.wsLdap.raiseWidget(0)
    self.lvLdapAttr.clear()
    self.cbLdapAttr.setFocus()
    self.console(self.__tr("the entry will be added to '%1'.").arg(dn), self.CONSOLE_INFO)

elif menuItem == 12:
    #
    # Add new User Wizard
    #
    self.wsLdap.raiseWidget(2)
    self.korreio_module_clear("ldap.form")
    self.console(self.__tr("the entry will be added to '%1'.").arg(dn), self.CONSOLE_INFO)

elif menuItem == 13:
    #
    # Add new Group Wizard
    #
    self.wsLdap.raiseWidget(7)
    self.korreio_module_clear("ldap.form.group")
    self.cbLdapFormGroupSIDdn.clear()
    for domain in self.config_sambaDomains():
        self.cbLdapFormGroupSIDdn.insertItem(domain)
    self.console(self.__tr("the entry will be added to '%1'.").arg(dn), self.CONSOLE_INFO)

elif menuItem == 14:
    #
    # Add new Organization[alUnit] Wizard
    #
    self.wsLdap.raiseWidget(3)
    self.korreio_module_clear("ldap.form.unit")
    self.console(self.__tr("the entry will be added to '%1'.").arg(dn), self.CONSOLE_INFO)

elif menuItem == 15:
    #
    # Add new DHCP Wizard
    #
    self.wsLdap.raiseWidget(1)
    self.korreio_module_clear("ldap.form.dhcp")
    self.console(self.__tr("the entry will be added to '%1'.").arg(dn), self.CONSOLE_INFO)

elif menuItem == 20:
    #
    # Turn QListViewItem editable
    #
    self.ldap_edit_rdn()

elif menuItem == 21 or menuItem == 22:
    #
    # Copy or Cut and store operation
    #
    self.lvLdapMenu.clipBoardMode = menuItem
    self.lvLdapMenu.clipBoard = dn

elif menuItem == 23:
    #
    # Paste and verify operation
    #
    if re.search(self.lvLdapMenu.clipBoard, dn):
        self.console(self.__tr("invalid request. Operation can't be done."), self.CONSOLE_WARN)
    else:
        if self.lvLdapMenu.clipBoardMode == 21:
            self.ldap_copy(self.lvLdapMenu.clipBoard, dn, None)
        elif self.ldap_modify_rdn(self.lvLdapMenu.clipBoard, self.lvLdapMenu.clipBoard.split(u",")[0], dn, 0):
            self.console(self.__tr("entry Distinguish Name '%1' changed.").arg(self.lvLdapMenu.clipBoard))

elif menuItem == 24:
    #
    # Proceed entry deletion
    #
    self.ldap_remove_entry()    

elif menuItem == 31:
    #
    # Set password
    #
    self.wsLdap.raiseWidget(4)
    self.korreio_module_clear("ldap.form.password")

    if "smbpasswd" not in self.module_failed:
        if "sambaSamAccount" in self.ldap_cache.get(dn.lower()).get("objectClass"):
            self.cbLdapSambaPassword.setEnabled(True)
            self.cbLdapSambaPassword.setChecked(True)
            self.cLdapSambaPasswordPwdMustChange.setEnabled(True)
            self.cLdapSambaPasswordPwdMustChange.setChecked(self.cConfLdapsambaPwdMustChange.isChecked())
        else:
            self.cbLdapSambaPassword.setEnabled(False)
            self.cbLdapSambaPassword.setChecked(False)
            self.cLdapSambaPasswordPwdMustChange.setEnabled(False)
            self.cLdapSambaPasswordPwdMustChange.setChecked(False)

    if "astSipGeneric" in self.ldap_cache.get(dn.lower()).get("objectClass"):
        self.cbLdapAstPassword.setEnabled(True)
        self.cbLdapAstPassword.setChecked(True)
    else:
        self.cbLdapAstPassword.setEnabled(False)
        self.cbLdapAstPassword.setChecked(False)


    self.console(self.__tr("selected entry to password change is '%1'.").arg(dn), self.CONSOLE_INFO)

elif menuItem == 32:
    #
    # Samba populate
    #
    self.wsLdap.raiseWidget(5)
    self.korreio_module_clear("ldap.smb.populate")
    self.console(self.__tr("selected entry to populate is '%1'.").arg(dn), self.CONSOLE_INFO)

elif menuItem == 33:
    #
    # DHCP populate
    #
    self.wsLdap.raiseWidget(6)
    self.korreio_module_clear("ldap.dhcp.populate")
    self.console(self.__tr("selected entry to populate is '%1'.").arg(dn), self.CONSOLE_INFO)

elif menuItem == 34:
    #
    # View: Change BaseDN
    #
    self.cbLdapBaseDN.setCurrentText(dn)
    self.ldap_search()

elif menuItem == 35:
    #
    # View: Set origin BaseDN
    #
    conn = self.iLdapConnection.text().ascii()
    i=0
    while self.confDict.get("ldap%s.name" % i):
        if self.confDict.get("ldap%s.name" % i) == conn:
            self.cbLdapBaseDN.setCurrentText(self.confDict.get("ldap%s.basedn" % i))
            self.ldap_search()
            break
        i+=1
elif menuItem == 36:
    #
    # View: Expand selected items
    #
    QExpandChilds(self.lvLdap.currentItem())

elif menuItem == 37:
    #
    # View: Expand all items
    #
    item = self.lvLdap.firstChild()
    while item is not None:
        QExpandChilds(item)
        item = item.nextSibling()

elif menuItem == 38:
    #
    # Import: from CSV
    #
    self.wsLdap.raiseWidget(8)
    self.korreio_module_clear("ldap.import.CSV")
    self.ldap_import_csv_dn()
    self.console(self.__tr("selected entry will be imported to '%1'.").arg(dn), self.CONSOLE_INFO)

elif menuItem == 39:
    #
    # Import: from LDIF
    #
    self.wsLdap.raiseWidget(9)

elif menuItem == 50:
    if self.cConfLdapSchemaAddAttr.isChecked():
        self.cConfLdapSchemaAddAttr.setChecked(False)
    else:
        self.cConfLdapSchemaAddAttr.setChecked(True)

elif menuItem == 51:
    if self.cConfLdapSchemaDelAttr.isChecked():
        self.cConfLdapSchemaDelAttr.setChecked(False)
    else:
        self.cConfLdapSchemaDelAttr.setChecked(True)


elif menuItem == 54:
    self.ldap_remove_attr()

}

void dKorreio::ssh_connect() {

#
# Verify SSH connection cache
#

try:
    if self.ssh.isalive():
        if self.inet_cache.get("ssh.host") == self.iSshHost.text().ascii() and self.inet_cache.get("ssh.port") == self.iSshPort.text().ascii() and self.inet_cache.get("ssh.user") == self.iSshUser.text().ascii() and self.inet_cache.get("ssh.pass") == self.iSshPass.text().ascii():
            return self.ssh
except:
    pass

#
# Connect to SSH
#

try:
    self.ssh = pxssh.pxssh()
    if not self.ssh.login("-p%s %s" % (self.iSshPort.text().ascii(), self.iSshHost.text().ascii()), self.iSshUser.text().ascii(), self.iSshPass.text().ascii(), login_timeout=40):
        raise pexpect.EOF, self.ssh
except pexpect.EOF, t:
    self.parse_exception("pexpect.EOF", t)
    del self.ssh
    return None

#
# Save current LDAP connection information
#

self.inet_cache["ssh.host"] = self.iSshHost.text().ascii()
self.inet_cache["ssh.port"] = self.iSshPort.text().ascii()
self.inet_cache["ssh.user"] = self.iSshUser.text().ascii()
self.inet_cache["ssh.pass"] = self.iSshPass.text().ascii()

server = "ssh://%s@%s:%s" % (self.iSshUser.text().ascii(), self.iSshHost.text().ascii(), self.iSshPort.text().ascii())
self.console(self.__tr("%1 connected.").arg(server))
return self.ssh

}

void dKorreio::ssh_open( a0 ) {

if self.cbSSHsudo.currentItem() == 0:
    cmd = a0
else:
    cmd = "sudo %s" % a0

result = []

s = self.ssh_connect()
if s is None:
    return None

self.console(self.__tr("running remote request: %1.").arg(cmd), self.CONSOLE_INFO, False)
try:
    s.sendline(cmd)
    s.prompt()
    for line in s.before.split("\n")[1:-1]:
        line = re.sub("\r","",line)
        result.append(line)
    return result
except pexpect.EOF, t:
    self.parse_exception("pexpect.EOF", t)
except pexpect.TIMEOUT, t:
    self.parse_exception("pexpect.TIMEOUT", t)

}

void dKorreio::ssh_exec( a0 ) {

if self.cbSSHsudo.currentItem() == 0:
    cmd = a0
else:
    cmd = "sudo %s" % a0

s = self.ssh_connect()
if s is None:
    return False

s.sendline ("%s > /dev/null 2>&1 && echo OK" % cmd)
s.prompt()
if re.search("\nOK", s.before):
    self.console(self.__tr("running remote request: %1.").arg(cmd), self.CONSOLE_OK, False)
    return True
else:
    self.console(self.__tr("running remote request: %1.").arg(cmd), self.CONSOLE_ERR)
    return False

}

void dKorreio::scp_exec( a0 ) {

try:
    self.console(self.__tr("saving file: %1").arg(a0), self.CONSOLE_ERR)
    child = pexpect.spawn('scp -P%s /tmp/korreio.tmp %s@%s:"%s"' % (self.iSshPort.text().ascii(), self.iSshUser.text().ascii(), self.iSshHost.text().ascii(), a0))
    i = child.expect(['assword','want to continue connecting'], timeout=5)
    if i==0:
        child.sendline(self.iSshPass.text().ascii())
    elif i==1:
        child.sendline('yes')
        child.expect('assword', timeout=2)
        child.sendline(self.iSshPass.text().ascii())
    print_cmd=[]
    for line in child:
        print_cmd.append(line)
    child.kill(0)
except pexpect.EOF, t:
    self.parse_exception("pexpect.EOF", t)
except pexpect.TIMEOUT, t:
    self.parse_exception("pexpect.TIMEOUT", t)

}

void dKorreio::services_file_open() {

if not self.cbServicesFileOpen.currentText().ascii():
    self.console(self.__tr("set file name."), self.CONSOLE_INFO)
    self.cbServicesFileOpen.setFocus()
    return True

contentList = self.ssh_open("cat %s" % self.cbServicesFileOpen.currentText().ascii())

if contentList is None:
    return False

content = "\n".join(contentList)

if re.search("^cat: ", content):
    self.console(self.__tr("file '%1' not found.").arg(self.cbServicesFileOpen.currentText().ascii()), self.CONSOLE_ERR)
    return True

self.teServicesFileOpen.setText(content)

}

void dKorreio::services_file_save() {

if self.teServicesFileOpen.length () == 0:
    self.ssh_exec("rm -f %s" % self.cbServicesFileOpen.currentText().ascii())
    return True

# pexpect dont handle tab correctly.
self.ssh_open("cat <<< \"%s\" | sed -e \'s/#TAB#/\\t/g\' > %s" % (re.sub("\t", "#TAB#", self.teServicesFileOpen.text().ascii()), self.cbServicesFileOpen.currentText().ascii()))

}

void dKorreio::services_postconf() {

if self.rbServicesPostconfN.isChecked():
    cmd = "postconf -n"
elif self.rbServicesPostconfAll.isChecked():
    cmd = "postconf"
if self.rbServicesPostconfD.isChecked():
    cmd = "postconf -d"

cmd = self.ssh_open(cmd)

if cmd is None:
    return False

self.postconf = {}
lastOpt = self.cbServicesPostconf.currentText().ascii()
self.cbServicesPostconf.clear()
i = 0
for config in cmd:
    if re.search("=",config):
        configlist = config.strip().split("=")
        configlist[0]=configlist[0].strip(" ")
        self.postconf[configlist[0]] = configlist[1]
        self.cbServicesPostconf.insertItem(configlist[0])
        if configlist[0] == lastOpt:
            lastItem = i
        i += 1
try:
    self.cbServicesPostconf.setCurrentItem(lastItem)
except UnboundLocalError, e:
    pass
self.services_postconf_changed()

}

void dKorreio::services_postconf_changed() {

value = self.postconf.get(self.cbServicesPostconf.currentText().ascii())
value = re.sub("( )+"," ",value)
value = re.sub(", ",",",value)
value = re.sub(",",",\n",value)
value = re.sub("^ ","",value)
self.teServicesPostconf.setText(value)

}

void dKorreio::services_postconf_save() {

value = re.sub(",", ", ", self.teServicesPostconf.text().ascii())
value = re.sub(" +", " ",value)
value = re.sub("\n","",value)
value = re.sub("\r","",value)
option = self.cbServicesPostconf.currentText().ascii()

if self.ssh_exec("postconf -e %s=%s" % (option, re.sub(" ", "\ ", value))):
    self.console(self.__tr("option '%1' is set.").arg(option))
    self.postconf[option] = value
else:
    self.console(self.__tr("can't set option '%1'.").arg(option), self.CONSOLE_ERR)

}

void dKorreio::services_postmap() {

file = self.cbServicesFileOpen.currentText().ascii()
if self.ssh_exec("postmap %s" % file):
    self.console(self.__tr("hash '%1.db' created.").arg(file))
else:
    self.console(self.__tr("can't create hash '%1.db'.").arg(file), self.CONSOLE_ERR)

}

void dKorreio::services_status() {

service = self.cbServiceService.currentText().ascii()
status = self.cbServiceStatus.currentText().ascii()
log = self.ssh_open("/etc/init.d/%s %s" % (service, status))

if log is None:
    return False

self.teServiceStatus.setText("\n".join(log))

}

void dKorreio::services_change_widgetstack() {

item = self.lvServices.currentItem()

if  item.text(1).ascii() == "1":
    self.wsServices.raiseWidget(0)
elif item.text(1).ascii() == "2":
   self.wsServices.raiseWidget(2)
elif item.text(1).ascii() == "3.1":
    self.wsServices.raiseWidget(1)

}

void dKorreio::services_menu() {
# a0: item, a1: pos-xy

self.korreio_update_servers_menu()
self.lvServicesMenu.popup(a1)

}

void dKorreio::sieve_search() {

self.korreio_module_clear("sieve")

imap = self.imap_connect()
if imap is None:
    return False

queryInput = self.iSieveSearch.text().latin1()
queryInput = queryInput.replace("*", "")
if queryInput.find("@") == 0:
    query = "user%s%%%s" % (imap.SEP, queryInput)
elif queryInput.find("@") != -1:
    query = "user%s%s" % (imap.SEP, queryInput)
else:
    query = "user%s%s%%" % (imap.SEP, queryInput)

for user in imap.lm(query):
    user = user.split(imap.SEP)[1]
    item = QListViewItemImap(self.lvSieve)
    item.setPixmap(0, self.icons.get("user.png"))
    item.setText(0, user)
    if self.cSieveScript.isChecked():
        try:
            s = self.sieve_connect(user)
            ok, scripts = s.listscripts()
            s.logout()
            if ok == 'OK':
                item.setText(1, "   (%s)" % len(scripts))
                for script, active in scripts:
                    if active:
                        item.setText(2, script)
                        break
        except:
            pass

item = self.lvSieve.firstChild()
if item is not None:
    self.lvSieve.setCurrentItem(item)
    item.setSelected(True)
    self.sieve_user_clicked()

}

void dKorreio::sieve_connect( a0 ) {

admin = self.iCyrusUser.text().ascii().split("@")
if admin[0] != self.iCyrusUser.text().ascii():
    user = "%s@%s" % (a0, admin[1])
else:
    user = a0

server = "sieve://%s:%s/%s" % (self.iCyrusHost.text().ascii(), self.iCyrusSievePort.text().ascii(), user)

s = sievelib.MANAGESIEVE(self.iCyrusHost.text().ascii(),int(self.iCyrusSievePort.text().ascii()))
if s.alive:
    if s.login('PLAIN',user,self.iCyrusPass.text().ascii(),self.iCyrusUser.text().ascii()):
        self.console(self.__tr("%1 connected.").arg(server))
        return s
    else:
        self.console(self.__tr("%1 disconnected. (wrong user or password)").arg(server), self.CONSOLE_ERR)
else:
    self.console(self.__tr("%1 disconnected. (connection refused)").arg(server), self.CONSOLE_ERR)

return False

}

void dKorreio::sieve_user_clicked() {

self.teSieveScript.clear()
self.cbSieveScript.clear()

item = self.lvSieve.currentItem()
if item is None:
    return True

try:
    s = self.sieve_connect(item.text(0).ascii())
    ok, scripts = s.listscripts()
    s.logout()
except:
    return True

if ok == 'OK':
    i = 0
    for script, active in scripts:
        self.cbSieveScript.insertItem(script)
        if active:
            self.cbSieveScript.setCurrentItem(i)
        i += 1

if self.cbSieveScript.count() > 0:
    self.sieve_get_script()

}

void dKorreio::sieve_get_script() {

item = self.lvSieve.currentItem()

if item is None:
    return True

s = self.sieve_connect(item.text(0).ascii())
ok, script = s.getscript(self.cbSieveScript.currentText().ascii())
s.logout()

self.teSieveScript.clear()
if ok == 'OK':
    self.teSieveScript.setText(script)

}

void dKorreio::sieve_set_script() {

if not self.cbSieveScript.currentText().ascii():
    self.console(self.__tr("set sieve script name."), self.CONSOLE_WARN)
    return True

item = self.lvSieve.firstChild()
if item is None:
    self.console(self.__tr("select user for set sieve script."), self.CONSOLE_WARN)
    return True

while item is not None:
    if item.isSelected():
        s = self.sieve_connect(item.text(0).ascii())
        status = s.putscript(self.cbSieveScript.currentText().ascii(), self.teSieveScript.text().ascii().replace("#USER#", item.text(0).ascii()))
        if status == 'OK':
            item.setText(2, self.cbSieveScript.currentText().ascii())
            s.setactive(self.cbSieveScript.currentText().ascii())
        s.logout()
    item = item.nextSibling()

self.sieve_get_script()

}

void dKorreio::sieve_unset_script() {

item = self.lvSieve.firstChild()
while item is not None:
    if item.isSelected():
        s = self.sieve_connect(item.text(0).ascii())
        s.setactive()
        s.logout()
        item.setText(2, "")
    item = item.nextSibling()

}

void dKorreio::sieve_del_script() {

if not self.cbSieveScript.currentText().ascii():
    self.console(self.__tr("set sieve script name."), self.CONSOLE_WARN)
    return True

item = self.lvSieve.firstChild()
while item is not None:
    if item.isSelected():
        s = self.sieve_connect(item.text(0).ascii())
        if s.deletescript(self.cbSieveScript.currentText().ascii()) == 'OK':
            if self.cbSieveScript.currentText().ascii() == item.text(2).ascii():
                item.setText(2, "")
        s.logout()
    item = item.nextSibling()

self.sieve_get_script()

}


void dKorreio::sieve_template_show() {
# a0=QListBoxItem:templateName

templateName = a0
if templateName is None:
    return True

ssList = self.sieve_scripts.get(Q2uni(templateName.text()))
if not ssList:
    self.console(self.__tr("Sieve script not available: %1 (Probably a bug).").arg("%s" % templateName.text().ascii()), self.CONSOLE_WARN)
    return False

self.cbSieveScript.setCurrentText(ssList[1])

#
# %(sep)
#

try:
    sep = self.m.SEP
except AttributeError, e:
    sep = "/"

#
# %(Domain)
#

if self.iCyrusUser.text().ascii().split("@")[0] == self.iCyrusUser.text().ascii():
    domain = "example.com"
else:
    domain = self.iCyrusUser.text().ascii().split("@")[1]

#
# Replace
#

script = ssList[2].replace('%(sep)', sep).replace('%(domain)', domain)

if re.search("#USER#", script):
    self.console(self.__tr("'#USER#' macro will be replaced properly."), self.CONSOLE_INFO)

self.teSieveScript.setText(script)

}

void dKorreio::sieve_template_load() {

#
# Load Sieve Scripts
#

self.lbSieveScripts.clear()

#
# Load Dist Sieve Scripts
#

try:
    sievePath = self.korreioConfigDict.get("path.sieveDir")
    tmpSieveTemplates = os.listdir(sievePath)
    sieveTemplates = []
    for template in tmpSieveTemplates:
        sieveTemplates.append("%s/%s" % (sievePath, template))
    del tmpSieveTemplates
except Exception, e:
    self.console(self.__tr("can't read sieve templates directory (%1).").arg(utf2uni(str(e))), self.CONSOLE_ERR)
    print "ERROR: can't read sieve templates (%s)." % e
    return False

#
# Load User Sieve Scripts
#

try:
    sieveUserTemplates = os.listdir(os.path.expanduser("~/.korreio/sieve"))
    for template in sieveUserTemplates:
        sieveTemplates.append("%s/%s" % (os.path.expanduser("~/.korreio/sieve"), template))
except Exception, e:
    self.console(self.__tr("can't read user sieve templates directory (%1).").arg(utf2uni(str(e))), self.CONSOLE_WARN)

re_kstFile = re.compile(r'^\[General\]\nfile name ?= ?(.*?)\ndisplay name ?= ?(.*?)\n(.*\n)*\n\[SieveScript\]\n((.*\n)*)$')

sieveTemplates.sort()
for templateFile in sieveTemplates:
    try:
        f = open(templateFile, "r")
        kstFile = f.read()
        f.close()
    except Exception, e:
        self.console(self.__tr("can't read sieve template: %1 (%2).").arg(utf2uni(templateFile)).arg(utf2uni(str(e))), self.CONSOLE_ERR)
        continue

    match = re_kstFile.match(kstFile)
    if match:
        displayName = Q2uni(self.__tr(match.group(2)))
        self.sieve_scripts[displayName] = []
        self.sieve_scripts[displayName].extend([templateFile, match.group(1), match.group(4)])
        self.lbSieveScripts.insertItem(displayName)
    else:
        self.console(self.__tr("invalid sintax for kstFile: %1.").arg(utf2uni(templateFile)), self.CONSOLE_ERR)

#
# pylupdate forget for default sieve scripts
#

nullList = []
nullList.append(self.__tr("Forward"))
nullList.append(self.__tr("Forward and save message"))
nullList.append(self.__tr("Select folder by sender"))
nullList.append(self.__tr("Select folder by senders"))
nullList.append(self.__tr("Select folder if Spam"))
nullList.append(self.__tr("Discard Spam"))
nullList.append(self.__tr("Vacation"))
nullList.append(self.__tr("Vacation if not Spam"))
del nullList

}

void dKorreio::sieve_menu() {
# a0: item, a1: pos-xy

self.korreio_update_servers_menu()
self.lvSieveMenu.popup(a1)

}

void dKorreio::sieve_menu_template() {
# a0: item, a1: pos-xy

item = a0

if item is None:
    self.lvSieveTemplateMenu.setItemEnabled(11, False)
    self.lvSieveTemplateMenu.setItemEnabled(12, False)
    self.lvSieveTemplateMenu.setItemEnabled(13, False)
    self.lvSieveTemplateMenu.setItemEnabled(14, False)
else:
    ssList = self.sieve_scripts.get(item.text().ascii())
    try:
        f = open(ssList[0], "a")
        f.close()
        self.lvSieveTemplateMenu.setItemEnabled(11, True)
        self.lvSieveTemplateMenu.setItemEnabled(12, True)
        self.lvSieveTemplateMenu.setItemEnabled(13, True)
        self.lvSieveTemplateMenu.setItemEnabled(14, True)
    except:
        self.lvSieveTemplateMenu.setItemEnabled(11, False)
        self.lvSieveTemplateMenu.setItemEnabled(12, True)
        self.lvSieveTemplateMenu.setItemEnabled(13, False)
        self.lvSieveTemplateMenu.setItemEnabled(14, False)

self.lvSieveTemplateMenu.popup(a1)

}

void dKorreio::sieve_menu_clicked( a0 ) {
# a0=QListViewMenu:menuItem

menuItem = a0

if menuItem == 0:
    #
    # Select all listed users
    #

    self.lvSieve.selectAll(True)

elif menuItem == 10:
    #
    # New template
    #

    quota, msg = QInputDialog.getInteger(self.__tr("Quota"), self.__tr("Set quota limit (Kbytes):"), dflquota, 0, 2147483647, 512)

    ssList = self.sieve_scripts.get(self.lbSieveScripts.currentText().ascii())
    print ssList

elif menuItem == 11:
    #
    # Save current template
    #

    pass

elif menuItem == 12:
    #
    # Save as new current template
    #

    pass
elif menuItem == 13:
    #
    # Rename current template
    #

    ssList = self.sieve_scripts.get(self.lbSieveScripts.currentText().ascii())
    print ssList
elif menuItem == 14:
    #
    # Delete current template
    #

    ssList = self.sieve_scripts.get(self.lbSieveScripts.currentText().ascii())
    print ssList

}

void dKorreio::queue_load() {

re_queueid  = re.compile(r'\b([A-Z0-9]+\*?\!?)\s+(\d+)\s([A-Z][a-z][a-z])\s([A-Z][a-z][a-z])\s+(\d+)\s(\d\d:\d\d:\d\d)\s+(.*)')
re_rcpt  = re.compile(r'\s+(.*@.*)\b')
re_log  = re.compile(r'(\s+)?\(.*\)')

oldItem = self.lvQueue.currentItem()
if oldItem is not None:
    while oldItem.parent() is not None:
        oldItem = oldItem.parent()
    oldFrom = oldItem.text(0).ascii()
else:
    oldFrom = None

self.korreio_module_clear("ssh.queue")

itemFrom = {}
itemQueueID = {}
self.itemLog = {}

queueList = self.ssh_open("postqueue -p")
if queueList is None:
    return False

if re.search("command not found", queueList[0]):
    self.console(self.__tr("command not found: %1.").arg("postqueue"), self.CONSOLE_ERR)
    return False

for line in queueList:
    match = re_queueid.match(line)
    if match is not None:
        tmp = ""
        self.itemLog[match.group(1)] = "%s %s %s %s" % (match.group(3), match.group(4), match.group(5), match.group(6))
        #print "regexp: %s %s %s %s %s %s %s" % (match.group(1), match.group(2), match.group(3), match.group(4), match.group(5), match.group(6), match.group(7))
        mailFrom = match.group(7)
        if not itemFrom.get(mailFrom):
            itemFrom[mailFrom] = QListViewItemQueue(self.lvQueue)
            itemFrom[mailFrom].setPixmap(0, self.icons.get("user.png"))
            itemFrom[mailFrom].setText(0,match.group(7))
            if match.group(7) == oldFrom:
                self.lvQueue.setCurrentItem(itemFrom[mailFrom])
                itemFrom[mailFrom].setSelected(True)
                itemFrom[mailFrom].setOpen(True)
        queueid = match.group(1)
        itemQueueID[queueid] = QListViewItem(itemFrom[mailFrom])
        itemQueueID[queueid].setPixmap(0, self.icons.get("queue.png"))
        itemQueueID[queueid].setText(0,"%s - %s Kb" % (queueid, int(match.group(2)) / 1024) )
        itemQueueID[queueid].setOpen(True)
        continue
    match = re_log.match(line)
    if match is not None:
        tmp = line
        continue
    match = re_rcpt.match(line)
    if match is not None:
        try:
            self.itemLog["%s:%s" % (queueid, match.group(1) )] = tmp
        except:
            pass
        tmp = ""
        itemRcpt = QListViewItem(itemQueueID[queueid])
        itemRcpt.setPixmap(0, self.icons.get("mail_forward.png"))
        itemRcpt.setText(0, match.group(1))

if oldFrom is None:
    item = self.lvQueue.firstChild()
    if item is not None:
        self.lvQueue.setCurrentItem(item)
        item.setSelected(True)

item = self.lvQueue.firstChild()
total = [0, 0]
while item is not None:
    count = item.childCount()
    subcount = 0
    if count > 0:
        subitem = item.firstChild()
        while subitem is not None:
            subcount = subcount + subitem.childCount()
            subitem.setText(1, "%s" % subitem.childCount())
            subitem = subitem.nextSibling()
    item.setText(1, "%s/%s" % (count, subcount))
    item = item.nextSibling()
    total[0] += count
    total[1] += subcount

self.tlQueueMsgs.setText("%s/%s" % (total[0], total[1]))
self.lvQueue.ensureItemVisible(self.lvQueue.currentItem())
self.queue_info()
self.queue_search()

}

void dKorreio::queue_search() {

def search_item(item):
        try:
            if re.search(search, Q2utf(item.text(0))):
                return True
            else:
                return False
        except:
            return False

search = Q2utf(self.iQueueSearch.text())
item = self.lvQueue.firstChild()

while item is not None:
    flag = False
    if self.queue_search_mode in [0, 2]:
        flag = search_item(item)

    if flag:
        item.setVisible(True)
    elif self.queue_search_mode in [1, 2]:
        item2 = item.firstChild()
        while item2 is not None:
            item3 = item2.firstChild()
            while item3 is not None:
                flag = search_item(item3)
                if flag: break
                item3 = item3.nextSibling()
            if flag: break
            item2 = item2.nextSibling()
        if flag:
            item.setVisible(True)
        else:
            item.setVisible(False)
    else:
        item.setVisible(False)

    item = item.nextSibling()

}

void dKorreio::queue_search_auto() {

if self.queue_search_auto:
    self.queue_search()

}

void dKorreio::queue_info() {

item = self.lvQueue.currentItem()

if item is None:
    return True

if item.parent() is None:
    size = 0
    queueidList = QStringList()
    subitem = item.firstChild()
    while subitem is not None:
        queueid = subitem.text(0).ascii()
        re_queueid  = re.compile(r'\b([A-Z0-9]+)\*?\!? - (\d+) Kb')
        match = re_queueid.match(queueid)
        if match is not None:
            queueidList.append(match.group(1))
            size += int(match.group(2))
        subitem.text(0).ascii()
        subitem = subitem.nextSibling()
    self.iQueueMessage.setText(self.__tr("*** SENDER ***\n%1\n\n*** TOTAL SIZE ***\n%2 Kbytes\n\n*** PENDING MESSAGES ***\n%3\n").arg(item.text(0)).arg(size).arg(queueidList.join("\n")))
    return True

if item.childCount() == 0:
    re_queueid  = re.compile(r'\b([A-Z0-9]+\*?\!?)')
    match = re_queueid.match(item.parent().text(0).ascii())
    if match is not None:
        self.iQueueMessage.setText(self.__tr("*** SENDER ***\n%1\n\n*** RECIPIENT ***\n%2\n\n*** MESSAGE ***\n%3\n\n*** DELAY REASON ***\n%4").arg(item.parent().parent().text(0)).arg(item.text(0)).arg(match.group(1)).arg(re.sub(" *\(", "(", self.itemLog.get("%s:%s" % (match.group(1), item.text(0).ascii()) ) )))
        return True

queueid = item.text(0).ascii()
re_queueid  = re.compile(r'\b([A-Z0-9]+\*?\!?)')
match = re_queueid.match(queueid)
if match is not None:
    recipient = QStringList()
    subitem = item.firstChild()
    while subitem is not None:
        recipient.append(subitem.text(0))
        subitem = subitem.nextSibling()
    self.iQueueMessage.setText(self.__tr("*** SENDER ***\n%1\n\n*** MESSAGE ***\n%2\n\n*** ARRIVAL TIME ***\n%3\n\n*** PENDING RECIPIENTS ***\n%4").arg(item.parent().text(0)).arg(match.group(1)).arg(self.itemLog.get(match.group(1))).arg(recipient.join("\n")))

}

void dKorreio::queue_message( a0, a1 ) {
# a0=cmd, a1=remove item

self.iQueueMessage.clear()

item = self.lvQueue.currentItem()
re_queueid  = re.compile(r'\b([A-Z0-9]+)\*?\!?')

if item.parent() is None:
    subitem = item.firstChild()
    success = False
    queuelist = []
    while subitem is not None:
        queuelist.append(re_queueid.match(subitem.text(0).ascii()).group(1))
        if len(queuelist) == 50 or subitem.nextSibling() is None:
            if self.ssh_exec("%s - <<< \"%s\"" % (a0, "\n".join(queuelist))):
                success = True
            queuelist = []
        subitem = subitem.nextSibling()
    if success:
        self.console(self.__tr("'%1' processed message(s).").arg(item.childCount()))
    if a1 == True:
        self.lvQueue.takeItem(item)
else:
    match = re_queueid.match(item.text(0).ascii())
    if match is not None:
        self.ssh_exec("%s %s" % (a0, match.group(1)))
        if a1 == True:
            count = item.parent().text(1).ascii().split("/")
            if int(count[0]) == 1:
                self.lvQueue.takeItem(item.parent())
            else:
                item.parent().setText(1 ,"%s/%s" % (str(int(count[0]) - 1), str(int(count[1]) - int(item.childCount()))))
                item.parent().takeItem(item)

}

void dKorreio::queue_menu() {

self.korreio_update_servers_menu()

item = a0

if item is None or item.childCount() == 0:
    self.lvQueueMenu.setItemEnabled(1, False)
    self.lvQueueMenu.setItemEnabled(2, False)
    self.lvQueueMenu.setItemEnabled(3, False)
    self.lvQueueMenu.setItemEnabled(4, False)
    self.lvQueueMenu.setItemEnabled(5, False)
    self.lvQueueMenu.setItemEnabled(6, False)
elif item.parent() is None:
    self.lvQueueMenu.setItemEnabled(1, False)
    self.lvQueueMenu.setItemEnabled(2, False)
    self.lvQueueMenu.setItemEnabled(3, True)
    self.lvQueueMenu.setItemEnabled(4, True)
    self.lvQueueMenu.setItemEnabled(5, True)
    self.lvQueueMenu.setItemEnabled(6, True)
else:
    self.lvQueueMenu.setItemEnabled(1, True)
    self.lvQueueMenu.setItemEnabled(2, True)
    self.lvQueueMenu.setItemEnabled(3, True)
    self.lvQueueMenu.setItemEnabled(4, True)
    self.lvQueueMenu.setItemEnabled(5, True)
    self.lvQueueMenu.setItemEnabled(6, True)

self.lvQueueMenu.popup(a1)

}


void dKorreio::queue_menu_clicked( a0 ) {
# a0=int:menuItem

menuItem = a0

if menuItem == 0:
    #
    # Update message list
    #

    self.queue_load()

elif menuItem == 1:
    #
    # Get message content
    #

    queueid = self.lvQueue.currentItem().text(0).ascii()
    re_queueid  = re.compile(r'\b([A-Z0-9]+)\*?\!?')
    match = re_queueid.match(queueid)
    if match is not None:
        cmd = self.ssh_open("postcat -q %s" % match.group(1))
        if cmd is not None:
            self.iQueueMessage.setText("\n".join(cmd))

elif menuItem == 2:
    #
    # Get recipient already sent
    #

    item = self.lvQueue.currentItem()
    queueid = item.text(0).ascii()
    re_queueid  = re.compile(r'\b([A-Z0-9]+)\*?\!?')
    match = re_queueid.match(queueid)
    if match is not None:
        cmd = self.ssh_open("postcat -q %s | grep done_recipient:" % match.group(1))
        if cmd is not None:
            cmd.sort()
            tmpCmd = []
            for line in cmd:
                tmpCmd.append(line.replace("done_recipient: ", ""))
            self.iQueueMessage.setText(self.__tr("*** SENDER ***\n%1\n\n*** MESSAGE ***\n%2\n\n*** DELIVERED RECIPIENT ***\nTotal: %3\n\n*** RECIPIENTS ***\n%4").arg(item.parent().text(0)).arg(match.group(1)).arg(len(tmpCmd)).arg("\n".join(tmpCmd)))

elif menuItem == 3:
    #
    # Hold selected messages
    #

    self.queue_message("postsuper -h", False)

elif menuItem == 4:
    #
    # Unhold selected messages
    #

    self.queue_message("postsuper -H", False)

elif menuItem == 5:
    #
    # Requeue selected messages
    #

    self.queue_message("postsuper -r", False)

elif menuItem == 6:
    #
    # Delete selected messages
    #

    item = self.lvQueue.currentItem()
    if item.parent() is None:
        msg = self.__tr("Confirm all messages deletion request from user '%1'?").arg(item.text(0).ascii())
    else:
        re_queueid  = re.compile(r'\b([A-Z0-9]+)\*?\!?')
        match = re_queueid.match(item.text(0).ascii())
        msg = self.__tr("Confirm message '%1' deletion request from user '%2'?").arg(match.group(1)).arg(item.parent().text(0).ascii())
    if QMessageBox.information( None, self.__tr("Confirm!"), msg, self.__tr("&Yes"), self.__tr("&No") ) != 0:
        self.console(self.__tr("deletion request was aborted."), self.CONSOLE_INFO)
        return True
    self.queue_message("postsuper -d", True)

elif menuItem == 9:
    #
    # Expand
    #
    item = self.lvQueue.firstChild()
    while item is not None:
        item.setOpen(True)
        item = item.nextSibling()

elif menuItem == 10:
    #
    # Flush messages
    #

    if QMessageBox.information( None, self.__tr("Warning!"),
           self.__tr("    Warning: the FLUSH operation cause negative interference in QMGR \n"+
                       "sent messages scheduler algorithm, especially in large queues.\n"+
                       "Use only when necessary."), self.__tr("&Ok")) != 0:
        self.console(self.__tr("flush request was aborted."), self.CONSOLE_INFO)
        return True
    cmd = "postqueue -f"

elif menuItem == 11:
    #
    # Hold all messages
    #

    if QMessageBox.information( None, self.__tr("Confirm!"),
           self.__tr("Confim set all messages on hold?"), self.__tr("&Yes"), self.__tr("&No")) != 0:
        self.console(self.__tr("hold on request was aborted."), self.CONSOLE_INFO)
        return True
    cmd = "postsuper -h ALL"

elif menuItem == 12:
    #
    # Unhold all messages
    #

    if QMessageBox.information( None, self.__tr("Confirm!"),
           self.__tr("Confirm set all messages to delivery (unhold)?"), self.__tr("&Yes"), self.__tr("&No")) != 0:
        self.console(self.__tr("unhold request was aborted."), self.CONSOLE_INFO)
        return True
    cmd = "postsuper -H ALL"

elif menuItem == 13:
    #
    # Requeue all messages
    #

    if QMessageBox.information( None, self.__tr("Confirm!"),
           self.__tr("Confirm transport requeue for all messages?"), self.__tr("&Yes"), self.__tr("&No")) != 0:
        self.console(self.__tr("transport requeue request was aborted."), self.CONSOLE_INFO)
        return True
    cmd = "postsuper -r ALL"

elif menuItem == 14:
    #
    # Delete all messages
    #

    if QMessageBox.information( None, self.__tr("Confirm!"),
           self.__tr("Confirme all messages deletion request?"), self.__tr("&Yes"), self.__tr("&No")) != 0:
        self.console(self.__tr("deletion request was aborted."), self.CONSOLE_INFO)
        return True
    cmd = "postsuper -d ALL"

elif menuItem == 20:
    #
    # Auto-search
    #
    if self.queue_search_auto:
        self.lvQueueMenu2.changeItem(20, QIconSet(QPixmap()), self.__tr('On &typing'))
        self.pQueueSearch.setEnabled(True)
        self.queue_search_auto = False
    else:
        self.lvQueueMenu2.changeItem(20, QIconSet(self.icons.get("ok.png")), self.__tr('On &typing'))
        self.pQueueSearch.setEnabled(False)
        self.queue_search_auto = True

elif menuItem == 21:
    #
    # Search Option: sender
    #
    self.queue_search_mode = 0
    self.lvQueueMenu2.changeItem(21, QIconSet(self.icons.get("ok.png")), self.__tr('&Sender'))
    self.lvQueueMenu2.changeItem(22, QIconSet(QPixmap()), self.__tr('&Recipient'))
    self.lvQueueMenu2.changeItem(23, QIconSet(QPixmap()), self.__tr('S&ender/recipient'))

elif menuItem == 22:
    #
    # Search Option: recipient
    #
    self.queue_search_mode = 1
    self.lvQueueMenu2.changeItem(21, QIconSet(QPixmap()), self.__tr('&Sender'))
    self.lvQueueMenu2.changeItem(22, QIconSet(self.icons.get("ok.png")), self.__tr('&Recipient'))
    self.lvQueueMenu2.changeItem(23, QIconSet(QPixmap()), self.__tr('S&ender/recipient'))

elif menuItem == 23:
    #
    # Search Option: all
    #
    self.queue_search_mode = 2
    self.lvQueueMenu2.changeItem(21, QIconSet(QPixmap()), self.__tr('&Sender'))
    self.lvQueueMenu2.changeItem(22, QIconSet(QPixmap()), self.__tr('&Recipient'))
    self.lvQueueMenu2.changeItem(23, QIconSet(self.icons.get("ok.png")), self.__tr('S&ender/recipient'))

if menuItem > 10 and menuItem < 15:
    if self.ssh_exec(cmd) and menuItem == 14:
        self.lvQueue.clear()

if menuItem > 20 and menuItem < 24:
    self.queue_search()

}

void dKorreio::ldap_import_csv_findFile() {

dlg = QFileDialog(os.getenv("HOME"))
dlg.setCaption(self.__tr("Korreio: load CSV file..."))
dlg.setMode(QFileDialog.ExistingFile)
dlg.exec_loop()
file = dlg.selectedFile()
self.iLdapImportCSVfile.setText(file)

self.rbLdapImportCSVviewmodeCSVfile.setChecked(True)
self.ldap_import_csv_viewMode()

}

void dKorreio::ldap_import_csv_viewMode() {

self.pLdapImportCSV.setEnabled(False)

#
# Ldap Import View Mode LDIF Template
#

if self.rbLdapImportCSVviewmodeLDIFtemplate.isChecked():
    self.wsLdapImportCSV.raiseWidget(1)

#
# Ldap Import View Mode LDIF Preview
#

elif self.rbLdapImportCSVviewmodeLDIFpreview.isChecked():
    self.wsLdapImportCSV.raiseWidget(2)
    self.pLdapImportCSV.setEnabled(True)
    self.teLdapImportCSVldifPreview.clear()
    CSVfile = self.ldap_import_csv_loadFile()
    if CSVfile:
        dnAttr = Q2uni(self.cbLdapImportCSVdn.currentText())
        re_dn = re.compile(ur'^%s: ?(.*)' % dnAttr)
        CSVseparator = Q2utf(self.iLdapImportCSVseparator.text())
        template = Q2uni(self.teLdapImportCSVldifTemplate.text())
        ldif = u""
        for line in CSVfile:
            i = 1
            ldifpartial = template
            for value in line.split(CSVseparator):
                ldifpartial = re.sub(u"\$\{%i\}" % i, value, ldifpartial)
                i += 1
            for x in ldifpartial.split(u"\n"):
                match = re_dn.match(x)
                if match:
                    dn = u"dn: %s=%s,%s" % (dnAttr, match.group(1), self.ldap_current_dn())
                    if ldif:
                        ldif = u"%s\n%s\n%s" % (ldif, dn, ldifpartial)
                    else:
                        ldif = u"%s\n%s" % (dn, ldifpartial)
                    break

        self.teLdapImportCSVldifPreview.setText(ldif)

#
# Ldap Import View Mode CSV File
#

elif self.rbLdapImportCSVviewmodeCSVfile.isChecked():
    self.wsLdapImportCSV.raiseWidget(0)

    cols = self.lvLdapImportCSV.columns()
    for col in range(cols-1,-1,-1):
        self.lvLdapImportCSV.removeColumn(col)

    CSVfile = self.ldap_import_csv_loadFile()
    if CSVfile:
        CSVseparator = Q2utf(self.iLdapImportCSVseparator.text())
        for field in range(1, len(CSVfile[0].split(CSVseparator)) + 1):
            self.lvLdapImportCSV.addColumn("%i" % field)
        
        for line in CSVfile:
            item = QListViewItem(self.lvLdapImportCSV)
            i = 0
            for field in line.split(CSVseparator):
                item.setText(i, field)
                i += 1
    item = self.lvLdapImportCSV.firstChild()
    if item is not None:
        self.lvLdapImportCSV.setCurrentItem(item)
        item.setSelected(True)

}

void dKorreio::ldap_import_csv_loadFile() {

if not self.iLdapImportCSVfile.text():
    self.console(self.__tr("select a CSV file."), self.CONSOLE_WARN)
    return []

file = Q2utf(self.iLdapImportCSVfile.text())

try:
    f = open(file, 'r')
    CSVfile = f.read()
    f.close()
except IOError, e:
    self.console(self.__tr("can't read file '%1' (%2).").arg(self.iLdapImportCSVfile.text()).arg(e), self.CONSOLE_ERR)
    return []

CSVfileList = CSVfile.split("\n")
CSVseparator = Q2utf(self.iLdapImportCSVseparator.text())
if not CSVfileList[-1]:
    CSVfileList.pop()

nSep = len(CSVfileList[0].split(CSVseparator))
i = 0
for line in CSVfileList:
    if len(line.split(CSVseparator)) != nSep:
        i += 1

if i != 0:
    self.console(self.__tr("Lines with more or less fields: '%1'.").arg("%i" % i), self.CONSOLE_WARN)

return CSVfileList

}

void dKorreio::ldap_import_csv_dn() {

self.cbLdapImportCSVdn.clear()
for line in Q2uni(self.teLdapImportCSVldifTemplate.text()).split(u'\n'):
    attr = line.split(u':')
    if attr[0] and attr[0].lower() != 'objectclass':
        self.cbLdapImportCSVdn.insertItem(attr[0])

}

void dKorreio::ldap_import_csv_acceptLDIF() {

self.wsLdap.raiseWidget(9)
self.teLdapImportLDIF.setText(self.teLdapImportCSVldifPreview.text())

}

void dKorreio::ldap_import_ldif_apply() {

ldif = Q2utf(self.teLdapImportLDIF.text())

ldifList = ldif.split("\n")
#for line in ldifList

}

void dKorreio::imap_import_findFile() {

file = QSelectFile(self.__tr("Korreio: open file..."), QFileDialog.ExistingFile)
self.iImapImportFile.setText(file)

}

void dKorreio::imap_import_apply() {

file = Q2utf(self.iImapImportFile.text())
if not file:
    self.console(self.__tr("select a file."))
    return True

try:
    f = open(file, "r")
    content = f.read()
    f.close()
except IOError, e:
    self.console(self.__tr("can't read file '%1' (%2).").arg(self.iLdapImportCSVfile.text()).arg(e), self.CONSOLE_ERR)
    return False

imap = self.imap_connect()

def defaults():
    if self.iConfImapQuotaMbytes.text().ascii():
        imap.sq(mbPath, (int(self.iConfImapQuotaMbytes.text().ascii()) * 1024))
    if self.rbImapMailboxMode.isChecked():
        item = self.lvConfImapFolders.firstChild()
        while item is not None:
            __imap_add("%s%s%s" % (mbPath, imap.SEP, item.text(0).ascii()))
            if item.text(1).ascii() and item.text(1).ascii() != "0":
                imap.setannotation("%s%s%s" % (mbPath, imap.SEP, item.text(0).ascii()), "/vendor/cmu/cyrus-imapd/expire", item.text(1).ascii())
            if item.text(2).ascii():
                imap.sam("%s%s%s" % (mbPath, imap.SEP, item.text(0).ascii()), "anyone", "p")
            item = item.nextSibling()


for line in content.split("\n"):
    if line.strip():
        if self.rbImapImportUidMail.isChecked():
            imap.cm("user%s%s" % (imap.SEP, line))

}

void dKorreio::imap_import_option() {

if self.rbImapImportUidMail.isChecked():
    self.cImapImportUidMaildefaultFolders.setEnabled(True)
else:
    self.cImapImportUidMaildefaultFolders.setEnabled(False)

if self.rbImapImportKIF.isChecked():
    self.cImapImportKIFmailbox.setEnabled(True)
    self.cImapImportKIFquota.setEnabled(True)
    self.cImapImportKIFacl.setEnabled(True)
    self.cImapImportKIFannotation.setEnabled(True)
else:
    self.cImapImportKIFmailbox.setEnabled(False)
    self.cImapImportKIFquota.setEnabled(False)
    self.cImapImportKIFacl.setEnabled(False)
    self.cImapImportKIFannotation.setEnabled(False)

}
