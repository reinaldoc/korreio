
default:
	clear
	rm *~ >/dev/null 2>&1 || true
	pyuic korreiolib.ui > korreiolib.py

	sed -i 's/HEADERS/SOURCES/g' korreio.pro
	pylupdate -noobsolete korreio.pro
	for i in lang/*.ts; do lrelease $${i} -qm $${i%%.*}.qm ; done
	mv lang/*.qm ../release/template/inc/lib/lang/

	sed -i 's/def console(self,a0,a1):/def console(self,a0,a1=None,a2=True):/' korreiolib.py
	sed -i 's/def ldap_menu(self):/def ldap_menu(self,a0,a1):/' korreiolib.py
	sed -i 's/def ldap_menu_attr(self):/def ldap_menu_attr(self,a0,a1):/' korreiolib.py
	sed -i 's/def ldap_query(self,a0):/def ldap_query(self,a0,a1=None,a2=None,a3=None):/' korreiolib.py
	sed -i 's/def imap_menu(self):/def imap_menu(self,a0,a1):/' korreiolib.py
	sed -i 's/def imap_partition_menu(self):/def imap_partition_menu(self,a0,a1):/' korreiolib.py
	sed -i 's/def sieve_menu(self):/def sieve_menu(self,a0,a1):/' korreiolib.py
	sed -i 's/def sieve_menu_template(self):/def sieve_menu_template(self,a0,a1):/' korreiolib.py
	sed -i 's/def sieve_template_show(self):/def sieve_template_show(self,a0):/' korreiolib.py
	sed -i 's/def services_menu(self):/def services_menu(self,a0,a1):/' korreiolib.py
	sed -i 's/def queue_menu(self):/def queue_menu(self,a0,a1):/' korreiolib.py
	sed -i 's/def imap_acl_clicked(self):/def imap_acl_clicked(self,a0):/' korreiolib.py
	sed -i 's/def imap_annotation_user_get(self):/def imap_annotation_user_get(self,a0=None,a1=None):/' korreiolib.py
	sed -i 's/def imap_mailbox_clicked(self):/def imap_mailbox_clicked(self,a0=None,a1=None):/' korreiolib.py
	sed -i 's/self.teSieveScript = QTextEdit/self.teSieveScript = QTextEditSieveScript/' korreiolib.py
	sed -i 's/def __init__(self,parent = None,name = None,fl = 0):/def __init__(self,parent = None,name = None,fl = 0,korreioConfigDict = {}):\n        self.korreioConfigDict = korreioConfigDict/' korreiolib.py
	cat QTextEditSieveScript.py >> korreiolib.py
	cat ldapcache.py >> korreiolib.py
	cat QLdapCache.py >> korreiolib.py
	cat QObjects.py >> korreiolib.py
	
	mv korreiolib.py ../release/template/inc/lib/
	
	../release/template/korreio.py

linguist:
	clear
	pyuic korreiolib.ui > korreiolib.py
	sed -i 's/HEADERS/SOURCES/g' korreio.pro
	pylupdate -noobsolete korreio.pro
	linguist-qt3 lang/korreio-pt_BR.ts

designer:
	designer-qt3 korreiolib.ui
