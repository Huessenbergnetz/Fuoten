#!/bin/bash
 
 
processSvg() {

    SVGFILE=$1
    FILENAME=`echo $SVGFILE | sed 's/.svg//'`

    echo "Processing $1"
    
    echo "Creating extra large icon"
    inkscape -z -e /tmp/icon-xl-$FILENAME.png -w 197 -h 197 $PWD/$SVGFILE
    convert /tmp/icon-xl-$FILENAME.png -gravity center -background '#ffffff00' -extent 256x256 $PWD/../icon-xl-$FILENAME.png
    rm /tmp/icon-xl-$FILENAME.png
    
    echo "Creating large icon"
    inkscape -z -e /tmp/icon-l-$FILENAME.png -w 74 -h 74 $PWD/$SVGFILE
    convert /tmp/icon-l-$FILENAME.png -gravity center -background '#ffffff00' -extent 96x96 $PWD/../icon-l-$FILENAME.png
    rm /tmp/icon-l-$FILENAME.png
    
    echo "Creating medium icon"
    inkscape -z -e /tmp/icon-m-$FILENAME.png -w 48 -h 48 $PWD/$SVGFILE
    convert /tmp/icon-m-$FILENAME.png -gravity center -background '#ffffff00' -extent 64x64 $PWD/../icon-m-$FILENAME.png
    rm /tmp/icon-m-$FILENAME.png
    
    echo "Creating small icon"
    inkscape -z -e /tmp/icon-s-$FILENAME.png -w 26 -h 26 $PWD/$SVGFILE
    convert /tmp/icon-s-$FILENAME.png -gravity center -background '#ffffff00' -extent 32x32 $PWD/../icon-s-$FILENAME.png
    rm /tmp/icon-s-$FILENAME.png

}

if [ "$1" == "" ]; then

    SVGFILES=`ls *.svg`
    
    for FILE in $SVGFILES
    do
        processSvg $FILE
    done

else 

    processSvg $1

fi
