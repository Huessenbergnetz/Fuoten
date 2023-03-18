# 
# Do NOT Edit the Auto-generated Part!
# Generated by: spectacle version 0.32
# 

Name:       harbour-fuoten

# >> macros
%define __provides_exclude_from ^%{_datadir}/.*$
%define __requires_exclude ^libFirfuoridaQt5|libFuotenQt5|libHbnSfosComponentsQt5.*$
# << macros

Summary:    ownCloud/Nextcloud News App Client
Version:    0.8.1
Release:    1
Group:      Applications/Internet
License:    GPL-3.0
URL:        https://github.com/Huessenbergnetz/Fuoten
Source0:    %{name}-%{version}.tar.bz2
Source100:  harbour-fuoten.yaml
Requires:   sailfishsilica-qt5 >= 0.10.9
Requires:   qt5-plugin-imageformat-gif
Requires:   qt5-plugin-imageformat-ico
Requires:   qt5-plugin-imageformat-jpeg
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5Sql)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(openssl)
BuildRequires:  pkgconfig(nemonotifications-qt5)
BuildRequires:  pkgconfig(sailfishsilica)
BuildRequires:  pkgconfig(sqlite3)
BuildRequires:  pkgconfig(dbus-1)
BuildRequires:  librsvg-tools
BuildRequires:  bc
BuildRequires:  qt5-qttools-linguist
BuildRequires:  desktop-file-utils
BuildRequires:  cmake

%description
Fuoten is a client for the ownCloud / Nextcloud News App, a cloud based
web feed reader.


%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
%cmake .
# << build pre


make %{?_smp_mflags}

# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
# << install pre
%make_install

# >> install post
# << install post

desktop-file-install --delete-original       \
  --dir %{buildroot}%{_datadir}/applications             \
   %{buildroot}%{_datadir}/applications/*.desktop

%files
%defattr(-,root,root,-)
%{_bindir}/%{name}
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
# >> files
%exclude %{_datadir}/%{name}/lib/cmake
%exclude %{_datadir}/%{name}/lib/pkgconfig
%exclude %{_includedir}/fuoten-qt5
# << files
