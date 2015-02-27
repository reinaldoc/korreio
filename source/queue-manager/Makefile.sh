#!/bin/bash

case "$1" in
	compile)
		find . -name '*.pyc' -exec rm -f {} \;

		echo '*.pyc' > .gitignore
		for interface in view/*qrc 
		do
			target=$(sed 's/.qrc/_rc.py/' <<< $interface)
			pyrcc4 $interface > $target
			echo $target >> .gitignore
		done

		for interface in view/*.ui; do
			target=$(sed 's/ui/py/' <<< $interface)
			pyuic4 $interface > $target
			echo $target >> .gitignore
		done
	;;
	release)
		if [ "$0" != "./Makefile.sh" ] ; then
			echo "This command must be used from workdir"
			exit
		fi

		$0 compile

		if [ -n "$2" ]; then
			CITY="-$2"
		fi
		YEAR=$(date +%Y)
		GIT_HASH_COMMIT=$(git rev-list HEAD --count)
		VERSION="v$YEAR-$GIT_HASH_COMMIT$CITY"
		#sed -i 's/project stage = development/project stage = production/' je-connect.conf
		#mv dao/ConfigDAO.py dao/ConfigDAO.py.tmp
		#cat dao/ConfigDAO.py.tmp |sed -e "s/^version =.*/version = \"$VERSION\"/" > dao/ConfigDAO.py
		cd ..
		tar czf /tmp/queue-manager-$VERSION.tar.gz queue-manager/ --exclude=Makefile.sh --exclude=test.py --exclude=*.pyc --exclude=*.ui --exclude=.gitignore --exclude=ConfigDAO.py.tmp
		if [ $? -eq 0 ] ; then
			echo "/tmp/queue-manager-$VERSION.tar.gz created."
		else
			echo "Failed create a release"
		fi
		#find ./je-connect -iregex '.*\.\(py\|png\|qrc\)' | sed 's/^\./\/opt/' > /tmp/itnm1JECN.txt
		cd - >/dev/null
		#mv -f dao/ConfigDAO.py.tmp dao/ConfigDAO.py
		#sed -i 's/project stage = production/project stage = development/' je-connect.conf
	;;
	*)
		echo "Sintax: $0 [ compile | release ]"
	;;
esac
