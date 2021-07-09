# Fuoten
ownCloud / Nextcloud News App client for [Sailfish OS](https://sailfishos.org/) (other platforms maybe later)
that uses [libfuoten](https://github.com/Huessenbergnetz/libfuoten).

## Contributing
Thanks for your interest in contributing! There are many ways to contribute to this project.
Get started [here](https://github.com/Huessenbergnetz/Fuoten/blob/master/CONTRIBUTING.md).

## Sailfish OS building instructions
To build Fuoten for Sailfish OS, you need the [Sailfish OS Application SDK](https://sailfishos.org/wiki/Application_SDK)
and some other tools.

### Required tools
* [Sailfish OS Application SDK](https://sailfishos.org/wiki/Application_SDK) (required to build the code)
* [git](https://git-scm.com/) (required to get the code)
* [bash](https://www.gnu.org/software/bash/) (required for the source code setup/init scripts)
* [lrelease](https://doc.qt.io/qt-5/linguist-manager.html) (required to generate the translations)
* [CairoSVG](https://cairosvg.org/) (required to generate the icons)
* [bc](http://www.gnu.org/software/bc/bc.html) (required to generate the icons)
* [parallel](https://www.gnu.org/software/parallel/) (optional to speed up icon creation)
* [zopflipng](https://github.com/google/zopfli) (optional to decrease icon sizes on disk)

### Get the code
At first you have to clone this repository including all submodules (libfuoten, BT SFOS Components, ADVobfuscator).

    git clone --recursive https://github.com/Huessenbergnetz/Fuoten.git

### Initialize the sources
Translation files and icons have to be generated separately. Switch to the source directory and run `init.sh`.
If you have *zopflipng* installed, it may take some time to create the icons.

    cd Fuoten
    ./init.sh

### Configure the project
Open the *Fuoten.pro* project file in the SDK and configure your build targets. For every build target
you have to specify the following additional *qmake arguments* (can be done on the Projects page of the Qt Creator).
**Note:** The AES256_KEY used here is only an example. Please use your own 32 character (32 byte) long key.

    CONFIG+=sfos CONFIG+=no_install_dev_files INSTALL_LIB_DIR=/usr/share/harbour-fuoten/lib INSTALL_TRANSLATIONS_DIR=/usr/share/harbour-fuoten/l10n AES256_KEY=AbCdEfGhIjKlMnOpQrStUvWxYz012345

#### Configuration options
| Option                         | Description
| ------------------------------ | ---------------
| `CONFIG+=sfos`                 | mandatory to build for SailfishOS
| `CONFIG+=no_install_dev_files` | disables the installation of the development files of libfuoten
| `INSTALL_LIB_DIR`              | target install directory for libfuoten, should be /usr/share/harbour-fuoten/lib
| `INSTALL_TRANSLATIONS_DIR`     | target install directory for translation files, should be /usr/share/harbour-fuoten/l10n
| `AES256_KEY`                   | 32 charachter (32 byte) long encryption key used for AES256 encryption of the password, if you change this key, passwords stored with old keys are not readable anymore

### Build the code
Now you are ready to build and run Fuoten on your Sailfish OS device or in the emulator.

## License
```
Fuoten - ownCloud/Nextcloud News App Client
Copyright (C) 2016-2019 Hüssenbergnetz/Matthias Fehring
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
```
