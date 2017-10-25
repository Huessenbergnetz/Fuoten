# Fuoten
ownCloud / Nextcloud News App client for [Sailfish OS](https://sailfishos.org/) (other platforms maybe later)
that uses [libfuoten](https://github.com/Huessenbergnetz/libfuoten).

## Contributing
### Translations
Translation is done on Transifex. If you want to help translating Fuoten, please consider also to help translating libfuoten and
BT SFOS Components, as both are used by Fuoten.
* [Fuoten](https://www.transifex.com/buschtrommel/fuoten)
* [libfuoten](https://www.transifex.com/buschtrommel/libfuoten)
* [BT SFOS Components](https://www.transifex.com/buschtrommel/bt-sfos-components)

## Sailfish OS building instructions
To build Fuoten for Sailfish OS, you need the [Sailfish OS Application SDK](https://sailfishos.org/wiki/Application_SDK).

At first you have to clone this repository including all submodules (libfuoten, BT SFOS Components).

    git clone --recursive https://github.com/Buschtrommel/Fuoten.git
    
Now open the *Fuoten.pro* project file in the SDK and configure your build targets. For every build target
you have to specify the following additional *qmake arguments* (can be done on the Projects page of the Qt Creator).

    CONFIG+=sfos CONFIG+=no_install_dev_files INSTALL_LIB_DIR=/usr/share/harbour-fuoten/lib INSTALL_TRANSLATIONS_DIR=/usr/share/harbour-fuoten/l10n BTSC_APP_NAME=fuoten
    
To create the translations, run the following script inside the Fuoten repository base directory:

    ./releasel10n.sh
    
Now you are ready to build and run Fuoten on your Sailfish OS device or in the emulator.

## License
Fuoten - ownCloud/Nextcloud News App Client<br>
Copyright (C) 2016-2017 Matthias Fehring<br>
https://github.com/Huessenbergnetz/Fuoten<br>
<br>
This program is free software: you can redistribute it and/or modify<br>
it under the terms of the GNU General Public License as published by<br>
the Free Software Foundation, either version 3 of the License, or<br>
(at your option) any later version.<br>
<br>
This program is distributed in the hope that it will be useful,<br>
but WITHOUT ANY WARRANTY; without even the implied warranty of<br>
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the<br>
GNU General Public License for more details.<br>
<br>
You should have received a copy of the GNU General Public License<br>
along with this program.  If not, see <http://www.gnu.org/licenses/>.

