#!/bin/bash

lrelease-qt5 -idbased translations/fuoten.ts

for LANG in de en_GB en_US
do
lrelease-qt5 -idbased translations/fuoten_$LANG.ts
done
