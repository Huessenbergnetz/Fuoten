# Fuoten
[Nextcloud News App](https://apps.nextcloud.com/apps/news) client for [Sailfish OS](https://sailfishos.org/)
(other platforms maybe later) that uses [libfuoten](https://github.com/Huessenbergnetz/libfuoten).

## Contributing
Thanks for your interest in contributing! There are many ways to contribute to this project.
Get started [here](https://github.com/Huessenbergnetz/Fuoten/blob/master/CONTRIBUTING.md).

## Sailfish OS building instructions
To build Fuoten for Sailfish OS, you need the [Sailfish OS Application SDK](https://sailfishos.org/wiki/Application_SDK)
and some other tools.

### Required tools
* [Sailfish OS Application SDK](https://sailfishos.org/wiki/Application_SDK) (required to build the code)
* [git](https://git-scm.com/) (required to get the code)
* [CMake](https://cmake.org/) (required to build the code)
* [lrelease](https://doc.qt.io/qt-5/linguist-manager.html) (required to generate the translations)
* [librsvg-tools](https://wiki.gnome.org/Projects/LibRsvg) (required to generate the icons)
* [bc](http://www.gnu.org/software/bc/bc.html) (required to generate the icons)

When using the Sailfish OS Application SDK, all requirements should be pulled in automatically.

### Get the code
At first you have to clone this repository including all submodules (libfuoten, HBN SFOS Cmake, HBN SFOS Components, ADVobfuscator).

    git clone --recursive https://github.com/Huessenbergnetz/Fuoten.git

### Configure the project
Open the *CMakeLists.txt* project file in the SDK and configure your build targets. For every build target
you have to specify the following additional *CMake* (can be done on the Projects page of the Qt Creator).
**Note:** The AES256_KEY used here is only an example. Please use your own 32 character (32 byte) long key.

    FUOTEN_AES256_KEY=AbCdEfGhIjKlMnOpQrStUvWxYz012345

#### Configuration options
| Option                         | Description
| ------------------------------ | ---------------
| `FUOTENAES256_KEY`             | 32 charachter (32 byte) long encryption key used for AES256 encryption of the password, if you change this key, passwords stored with old keys are not readable anymore

### Build the code
Now you are ready to build and run Fuoten on your Sailfish OS device or in the emulator.

## License
```
Fuoten - ownCloud/Nextcloud News App Client
Copyright (C) 2016-2022 Hüssenbergnetz/Matthias Fehring
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
