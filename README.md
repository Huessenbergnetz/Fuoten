# Fuoten
ownCloud / Nextcloud News App client for [Sailfish OS](https://sailfishos.org/) (other platforms maybe later)
that uses [libfuoten](https://github.com/Huessenbergnetz/libfuoten).

## Contributing
Thanks for your interest in contributing! There are many ways to contribute to this project. Get started [here](https://github.com/Huessenbergnetz/Fuoten/blob/master/CONTRIBUTINNG.md).

## Sailfish OS building instructions
To build Fuoten for Sailfish OS, you need the [Sailfish OS Application SDK](https://sailfishos.org/wiki/Application_SDK).

At first you have to clone this repository including all submodules (libfuoten, BT SFOS Components).

    git clone --recursive https://github.com/Buschtrommel/Fuoten.git
    
Now open the *Fuoten.pro* project file in the SDK and configure your build targets. For every build target
you have to specify the following additional *qmake arguments* (can be done on the Projects page of the Qt Creator).

    CONFIG+=sfos CONFIG+=no_install_dev_files INSTALL_LIB_DIR=/usr/share/harbour-fuoten/lib INSTALL_TRANSLATIONS_DIR=/usr/share/harbour-fuoten/l10n
    
To create the translations, run the following script inside the Fuoten repository base directory:

    ./releasel10n.sh
    
Now you are ready to build and run Fuoten on your Sailfish OS device or in the emulator.

## License
Fuoten - ownCloud/Nextcloud News App Client  
Copyright (C) 2016-2018 Hüssenbergnetz/Matthias Fehring
https://github.com/Huessenbergnetz/Fuoten

This program is free software: you can redistribute it and/or modify  
it under the terms of the GNU General Public License as published by  
the Free Software Foundation, either version 3 of the License, or  
(at your option) any later version.

This program is distributed in the hope that it will be useful,  
but WITHOUT ANY WARRANTY; without even the implied warranty of  
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the  
GNU General Public License for more details.

You should have received a copy of the GNU General Public License  
along with this program.  If not, see <http://www.gnu.org/licenses/>.

