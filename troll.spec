Summary:	 A troll works to free his people from an evil ruler in an adventure game.
Name:		troll
Version:	0.5.4
Release:	1
Group:		Amusements/Games
License:	GPLv2+
Vendor:		Dennis Payne <dulsi@identicalsoftware.com>
Url:		http://www.identicalsoftware.com/troll/
Source0:		http://www.identicalsoftware.com/troll/%{name}-%{version}.tgz
BuildRequires: boost-devel
BuildRequires: SDL2-devel
BuildRequires: SDL2_image-devel
BuildRequires: SDL2_mixer-devel
BuildRequires: desktop-file-utils


%description
Troll Bridge is an adventure game in the style of the original Legend of
Zelda.

%package	devel
Summary:	The development environment for Troll Bridge adventures.
Group:		Amusements/Games
Requires:	%{name} = %{version}

%description	devel
The development tools needed to construct new adventures using the Troll
Bridge game engine.

%prep
%setup -q

%build
cd identical
make %{?_smp_mflags}
cd ../src
make LIB_DIR=%{_lib} %{?_smp_mflags}
cd ../util
make %{?_smp_mflags}


%install
cd src
make prefix=%{buildroot} LIB_DIR=%{_lib} install
cd ../util
make prefix=%{buildroot} install

%posttrans
/usr/bin/gtk-update-icon-cache %{_datadir}/icons/hicolor &>/dev/null || :

%files
%doc README LICENSE
%{_bindir}/troll
%{_libdir}/troll
%{_datadir}/troll
%{_datadir}/icons/hicolor/*/apps/troll.png
%{_datadir}/applications/troll.desktop

%files devel
%{_bindir}/createmap
%{_bindir}/pcx2text
%{_bindir}/text2pal
%{_bindir}/text2pcx
%{_bindir}/screenedit
%{_bindir}/pcx2zsp
%{_bindir}/splitpcx
%{_mandir}/man1/*
%{_includedir}/troll

%changelog
* Sun Aug 28 2016 Dennis Payne <dulsi@identicalsoftware.com> - 0.5.4-1
- New version of Troll Bridge released.

* Sun May 24 2015 Dennis Payne <dulsi@identicalsoftware.com> - 0.5.2-1
- New version of Troll Bridge released.
