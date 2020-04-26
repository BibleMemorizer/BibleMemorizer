Name:           biblememorizer
Version:        0.6.2
Release:        1%{?dist}
Summary:        Tool to help memorizing Scripture

Group:          Applications/Text
License:        MIT/X11/GPL
URL:            https://biblememorizer.github.io/
Source0:        biblememorizer-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  qt-devel sword-devel

%description
BibleMemorizer is a program to help with memorizing Scripture.  It 
allows you to create files with lists of verses you want to  memorize,
including the text of the verse and any categories you create.

%prep
%setup -q


%build
%configure
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
make install INSTALL_ROOT=$RPM_BUILD_ROOT


%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
%doc AUTHORS CREDITS ChangeLog LICENSE.GPL LICENSE.MIT README TODO
%{_bindir}/%{name}
/usr/share/apps/%{name}

%changelog
