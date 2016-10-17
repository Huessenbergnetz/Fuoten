#!/bin/bash

lrelease-qt5 -idbased translations/fuoten.ts

for LANG in de
do
lrelease-qt5 -idbased translations/fuoten_$LANG.ts
done
