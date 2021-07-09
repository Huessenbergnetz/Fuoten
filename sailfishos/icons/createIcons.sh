#!/bin/bash

SCALES="1 1.25 1.5 1.75 2"
SIZES="32 64 96"

CAIROSVG=`which cairosvg`
export ZOPFLIPNG=`which zopflipng`
PARALLEL=`which parallel`
BC=`which bc`

if [ ! -x $CAIROSVG ]
then
    echo "Can not find CairoSVG executable"
    exit 1
fi

if [ ! -x $BC ]
then
    echo "Can not find bc executable"
fi

if [ ! -x $ZOPFLIPNG ]
then
    echo "zopflipng not found. Disabling PNG compression using zopfli."
fi

if [ ! -x $PARALLEL ]
then
    echo "GNU parallel not found. It is recommended for speeding up the icon creation."
fi

for SCALE in $SCALES
do
    SCALEDIR="z$SCALE"

    if [ ! -d $SCALEDIR ]
    then
        echo "Creating directory $SCALEDIR"
        mkdir $SCALEDIR
    fi
done

processSvg() {
    SCALE=$1
    OUTSIZE=$2
    SVGFILE=$3

    SCALEDIR="z$SCALE"

    FILENAME=$(basename -- "$SVGFILE")
    BASENAME="${FILENAME%.*}"

    if [[ ($BASENAME == "lock-"* || $BASENAME == "cover-"*) && $OUTSIZE != 32 ]]
    then
        exit 0
    fi

    SIZE=`echo "$SCALE*$OUTSIZE" | bc`
    SIZE=`LC_ALL=C printf '%.*f' 0 $SIZE`

    if [[ $BASENAME == "lock-"* || $BASENAME == "cover-"* ]]
    then
        FNAME=icon-$BASENAME.png
    else
        if [ $OUTSIZE -eq 32 ]
        then
            FNAME=icon-s-$BASENAME.png
        fi

        if [ $OUTSIZE -eq 64 ]
        then
            FNAME=icon-m-$BASENAME.png
        fi

        if [ $OUTSIZE -eq 96 ]
        then
            FNAME=icon-l-$BASENAME.png
        fi
    fi

    if [ -r $SCALEDIR/$FNAME -a -s $SCALEDIR/$FNAME ]
    then
        echo "$SCALEDIR/$FNAME already exists. Doing nothing."
    else
        echo "Creating $SCALEDIR/$FNAME (${SIZE}x${SIZE})"
        TMPFILE=$(mktemp)

        cairosvg -f png --output-width $SIZE --output-height $SIZE -o $TMPFILE $SVGFILE
        if [ -x $ZOPFLIPNG ]
        then
            zopflipng -y --iterations=500 --filters=01234mepb --lossy_transparent $TMPFILE $SCALEDIR/$FNAME
            rm $TMPFILE
        else
            mv $TMPFILE $SCALEDIR/$FNAME
        fi
    fi
}
export -f processSvg


if [ "$1" == "" ]; then

    SVGFILES=`ls src/*.svg`

    if [ -x $PARALLEL ]
    then
        parallel processSvg ::: $SCALES ::: $SIZES ::: $SVGFILES
    else
        for SCALE in $SCALES
        do
            for SIZE in $SIZES
            do
                for SVGFILE in $SVGFILES
                do
                    processSvg $SCALE $SIZE $SVGFILE
                done
            done
        done
    fi

else

    if [ -x $PARALLEL ]
    then
        parallel processSvg ::: $SCALES ::: $SIZES ::: $1
    else
        for SCALE in $SCALES
        do
            for SIZE in $SIZES
            do
                processSvg $SCALE $SIZE $1
            done
        done
    fi
fi
