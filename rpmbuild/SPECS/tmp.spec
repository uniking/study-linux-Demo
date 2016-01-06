%define _topdir /home/sxis/workspace/rpmbuild
%define _tmppath /var/tmp

Name: TestDoc
Version: 1.0
Release: 1
Summary: Document compiler

Group: Application/Productivity
License: KONG
Source0: hello.tar


%description
This is for test.

%prep
echo "to prep"
tar -xf ../SOURCES/hello.tar
	
%install
echo "to install"
cp ./hello.c $RPM_BUILD_ROOT/hello.c

%files
/hello.c

