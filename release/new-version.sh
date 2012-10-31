#!/bin/bash

version="korreio-$(date +%Y%m%d)"

echo
echo "### Renomeando para $version ###"
cp -av template $version
find ./${version}/ -name *.pyc -exec rm {} \;
rm $version.tar.gz >/dev/null 2>&1

echo
echo "### Compactando $version.tar.gz ###"
tar cvzf $version.tar.gz $version/
rm -rf $version

echo
echo "### Copiando arquivos dev ###"
cp -av '../source' ${version}-dev

echo
echo "### Compactando ${version}-dev.tar.gz ###"
tar cvzf ${version}-dev.tar.gz ${version}-dev/
rm -rf ${version}-dev/
