Summary:	 A troll works to free his people from an evil ruler in an adventure game.
Name:		troll
Version:	0.5.2
Release:	1
Group:		Amusements/Games
License:	GPLv2+
Vendor:		Dennis Payne <dulsi@identicalsoftware.com>
Url:		http://www.identicalsoftware.com/troll/
Source0:		http://www.identicalsoftware.com/troll/%{name}-%{version}.tgz
BuildRequires: SDL-devel
BuildRequires: SDL_image-devel
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
make -f Makefile.sdl %{?_smp_mflags}
cd ../src
make -f Makefile.sdl LIB_DIR=%{_lib} %{?_smp_mflags}
cd ../util
make -f Makefile.sdl %{?_smp_mflags}


%install
cd src
make -f Makefile.sdl prefix=%{buildroot} LIB_DIR=%{_lib} install
cd ../util
make -f Makefile.sdl prefix=%{buildroot} install

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
* Sun May 24 2015 Dennis Payne <dulsi@identicalsoftware.com> - 0.5.2-1
- New version of Troll Bridge released.
