#!/bin/bash

lrelease-qt5 -idbased translations/fuoten.ts

for LANG in da de en_GB en_US fr nl sv zh_CN
do
lrelease-qt5 -idbased translations/fuoten_$LANG.ts
done
