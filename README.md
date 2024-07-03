RASPA3
======

This software is a general purpose classical simulation package. It has been developed at
the University of Amsterdam (Amsterdam, The Netherlands) during 2021/2024 in active collaboration
with Eindhoven University of Technology (Eindhoven, Netherlands), Delft University of
Technology (Delft, The Netherlands), and Northwestern University (Evanston, USA).

Authors
=======
Drs. Youri Ran, University of Amsterdam<br>
Drs. Shrinjay Shoarma, Delft University of Technology<br>
Drs. Zhao Li, Northwestern University<br>
Dr. David Dubbeldam, University of Amsterdam<br>
Prof. Sofia Calero,  Eindhoven University of Technology<br>
Prof. Thijs Vlugt, Delft University of Technology<br>
Prof. Randall Q. Snurr, Northwestern University

Contributors
============
Alvaro Vazquez Mayagoitia, Argonne National Lab, contribution to openmp-implementation discussion

Python
======
export PYTHONPATH=$PYTHONPATH:/usr/share/raspa3/lib:/usr/local/share/raspa3/lib<br>
cd python<br>
python3 script.py

Running
=======
cd examples/basic/1_mc_methane_in_box<br>
./run

Dependencies
============
- cmake 3.28 or higher<br>
- ninja 1.11 or higher<br>
- llvm-18 or higher<br>
- python3 and pybind11<br>
- blas and lapack (64-bit integers)

Ubuntu 24 
=========
apt-get install -y --no-install-recommends git ca-certificates cmake ninja-build<br>
apt-get install -y --no-install-recommends llvm lld clang clang-tools clang-tidy libc++-dev libc++abi-dev libomp-dev libclang-rt-dev<br>
apt-get install -y --no-install-recommends python3 pybind11-dev python3-pybind11 python3-dev<br>
apt-get install -y --no-install-recommends liblapack64-dev libblas64-dev<br>
cmake -B build --preset=linux-ubuntu-24<br>
ninja -C build<br>
ninja -C build install<br>
ctest --test-dir build/tests/raspakit-tests --verbose

Fedora 40
=========
dnf install -y wget git rpm-build<br>
dnf install -y llvm lld cmake clang clang-tools-extra ninja-build<br>
dnf install -y libomp-devel libcxx libcxxabi libcxx-devel libcxxabi-devel libcxx-static libcxxabi-static<br>
dnf install -y lapack-devel lapack64 blas64<br>
dnf install -y python3 python3-devel python3-pybind11<br>
dnf install -y pybind11-devel<br>
cmake -B --preset linux-fedora-40<br>
ninja -C build<br>
ninja -C build install<br>
ctest --test-dir build/tests/raspakit-tests --verbose

Presets
=======
cmake --list-presets<br>
Available configure presets:<br>
> "macos-intel"<br>
> "macos-intel-debug"<br>
> "macos-apple-silicon"<br>
> "macos-apple-silicon-debug"<br>
> "linux"<br>
> "linux-carbon"<br>
> "linux-opensuse-leap-15.2"<br>
> "linux-opensuse-leap-15.3"<br>
> "linux-opensuse-leap-15.4"<br>
> "linux-opensuse-leap-15.5"<br>
> "linux-opensuse-tumbleweed"<br>
> "linux-archlinux"<br>
> "linux-redhat-6"<br>
> "linux-redhat-7"<br>
> "linux-redhat-8"<br>
> "linux-redhat-9"<br>
> "linux-debian-12"<br>
> "linux-debian-11"<br>
> "linux-debian-10"<br>
> "linux-ubuntu-24"<br>
> "linux-ubuntu-22"<br>
> "linux-ubuntu-20"<br>
> "linux-fedora-35"<br>
> "linux-fedora-36"<br>
> "linux-fedora-37"<br>
> "linux-fedora-38"<br>
> "linux-fedora-39"<br>
> "linux-fedora-40"

