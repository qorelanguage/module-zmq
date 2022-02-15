%{?_datarootdir: %global mydatarootdir %_datarootdir}
%{!?_datarootdir: %global mydatarootdir %{buildroot}/usr/share}

%global module_dir %{_libdir}/qore-modules
%global user_module_dir %{mydatarootdir}/qore-modules/

Name:           qore-zmq-module
Version:        1.0.1
Release:        1
Summary:        Qorus Integration Engine - Qore zmq module
License:        MIT
Group:          Productivity/Networking/Other
Url:            https://qoretechnologies.com
Source:         qore-zmq-module-%{version}.tar.bz2
BuildRequires:  gcc-c++
%if 0%{?el7}
BuildRequires:  devtoolset-7-gcc-c++
%endif
BuildRequires:  cmake >= 3.12.4
BuildRequires:  qore-devel >= 1.0
Requires:       %{_bindir}/env
Requires:       qore >= 1.0
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
BuildRequires:  czmq-devel
Requires:       libzmq5 >= 4.2.3

%description
This package contains the zmq module for an ZeroMQ API for the Qore Programming Language.

%prep
%setup -q

%build
%if 0%{?el7}
# enable devtoolset7
. /opt/rh/devtoolset-7/enable
unset PATH
%endif
export CXXFLAGS="%{?optflags}"
cmake -DCMAKE_INSTALL_PREFIX=%{_prefix} -DCMAKE_BUILD_TYPE=RELWITHDEBINFO -DCMAKE_SKIP_RPATH=1 -DCMAKE_SKIP_INSTALL_RPATH=1 -DCMAKE_SKIP_BUILD_RPATH=1 -DCMAKE_PREFIX_PATH=${_prefix}/lib64/cmake/Qore .
make %{?_smp_mflags}

%install
make DESTDIR=%{buildroot} install %{?_smp_mflags}

%files
%{module_dir}

%changelog
* Tue Feb 15 2022 David Nichols <david@qore.org>
- updated to v1.0.1

* Wed Jan 27 2022 David Nichols <david@qore.org>
- initial 1.0.0 release

