%define _topdir /home/sxis/workspace/hgNDLP/file_daemon/rpm/fdaemond
%define _tmppath /var/tmp

Name: fdaemond
Version: 1.1.1
Release: 1
Summary: Document compiler
BuildRequires: file_discover-devel
BuildRequires: jsoncpp-devel
BuildRequires: libfile_parse-devel
BuildRequires: ndlphelper-devel

Requires: ndlphelper
Requires: file_discover

Group: Application/Productivity
License: SXIS
Source0: %{name}.tar.gz

%description
This is for fdaemond of NDLP.

%prep
pwd
tar xf ../SOURCES/fdaemond.tar.gz

%build
make
	
%install
make DESDIR=$RPM_BUILD_ROOT install

%post
systemctl enable fdaemond
systemctl start fdaemond

%files
/sbin/fdaemond

%defattr(644,root,root,-)
/etc/systemd/system/fdaemond.service
/etc/file_daemon/*

