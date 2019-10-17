#!/bin/bash

BASENAME=harbour-fuoten
ICONSDIR=icons
INKSCAPE=`which inkscape`
ZOPFLIPNG=`which zopflipng`

if [ ! -x $INKSCAPE ]
then
    echo "Can not find inkscape executable"
    exit 1
fi

SVGFILE=$BASENAME.svg

if [ ! -r $SVGFILE ]
then
    echo "Can not find SVG source file!"
    exit 1
fi

if [ ! -x $ZOPFLIPNG ]
then
    echo "zopflipng not found. Disabling PNG compression using zopfli."
fi

for SIZE in 86 108 128 150 172
do
    SIZEDIR=$ICONSDIR/${SIZE}x${SIZE}
    FULLPATH=$SIZEDIR/$BASENAME.png

    if [ ! -r $FULLPATH ]
    then

        if [ ! -d $SIZEDIR ]
        then
            mkdir -p $SIZEDIR
        fi

        FNAME=$(mktemp)
        $INKSCAPE -z -e $FNAME -w $SIZE  -h $SIZE $SVGFILE &> /dev/null
        if [ -x $ZOPFLIPNG ]
        then
            $ZOPFLIPNG -y --iterations=500 --filters=01234mepb --lossy_transparent $FNAME $FULLPATH
            rm $FNAME
        else
            mv $FNAME $FULLPATH
        fi
    else
        echo "$FULLPATH already exists"
    fi
done

# if [ ! -r $BASENAME.png ]
# then
#     if [ -r $ICONSDIR/86x86/$BASENAME.png ]
#     then
#         cp $ICONSDIR/86x86/$BASENAME.png $BASENAME.png
#     else
#         echo "Can not find 86x86 icon!"
#         exit 1
#     fi
# fi
