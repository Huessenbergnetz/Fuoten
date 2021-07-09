#!/bin/bash

LRELEASE=`which lrelease-qt5`
if [ ! -x $LRELEASE ]
then
    LRELEASE=`which lrelease`
fi

if [ ! -x $LRELEASE ]
then
    echo "Can neither find lrelease nor lrelease-qt5 that is needed for creating translation files"
    exit 1
fi

CAIROSVG=`which cairosvg`
if [ ! -x $CAIROSVG ]
then
    echo "Can not find CairoSVG that is needed to generate icons"
    exit 1
fi

BC=`which bc`
if [ ! -x $BC ]
then
    echo "Can not find bc that is needed to generate icons"
    exit 1
fi

echo "Creating Fuoten translations:"
./releasel10n.sh

pushd libfuoten > /dev/null
echo ""
echo "Creating libfuoten translations:"
./scripts/releasel10n.sh
popd > /dev/null

pushd HBN_SFOS_Components > /dev/null
echo ""
echo "Creating HBN SFOS Components translations:"
./releasel10n.sh
popd > /dev/null

pushd sailfishos > /dev/null
echo ""
echo "Creating SailfishOS App Launcher icons:"
./createAppIcons.sh

pushd icons > /dev/null
echo ""
echo "Creating SailfishOS application icons:"
./createIcons.sh

popd > /dev/null
popd > /dev/null

pushd HBN_SFOS_Components/images > /dev/null
echo ""
echo "Creating HBN SFOS Components application icons:"
./createIcons.sh
popd > /dev/null
