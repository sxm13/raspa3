<h1 align="center">
  <br>
  <a href="https://github.com/iRASPA/raspa3"><img src="https://avatars.githubusercontent.com/u/46400041?s=200&v=4" alt="Markdownify" width="200"></a>
  <br>
  RASPA3
  <br>
</h1>

<h4 align="center">This software is a general purpose classical simulation package.</h4>

<p align="center">It has been developed at the University of Amsterdam (Amsterdam, The Netherlands) during 2022/2024 in active collaboration with Eindhoven University of Technology (Eindhoven, Netherlands), Delft University of Technology (Delft, The Netherlands), and Northwestern University (Evanston, USA).</p>

<p align="center">
  <a href="https://github.com/iRASPA/raspa3/releases">
<img alt="GitHub Actions Workflow Status" src="https://img.shields.io/github/actions/workflow/status/iRASPA/raspa3/github-actions-create-release.yml">
  </a>
  <a href="https://github.com/iRASPA/raspa3/issues"><img alt="GitHub Issues or Pull Requests" src="https://img.shields.io/github/issues/iRASPA/raspa3">
</a>
</p>

<p align="center">
  <a href="#authors">Authors</a> •
  <a href="#contributors">Contributors</a> •
  <a href="#running">Running</a> •
  <a href="#python">Python</a> •
  <a href="#dependencies">Dependencies</a> •
  <a href="#installation-guide">Installation Guide</a> •
</p>

# authors

Drs. Youri Ran, University of Amsterdam<br>
Drs. Shrinjay Sharma, Delft University of Technology<br>
Drs. Zhao Li, Northwestern University<br>
Dr. David Dubbeldam, University of Amsterdam<br>
Prof. Sofia Calero,  Eindhoven University of Technology<br>
Prof. Thijs Vlugt, Delft University of Technology<br>
Prof. Randall Q. Snurr, Northwestern University

# contributors

Alvaro Vazquez Mayagoitia, Argonne National Lab, contribution to openmp-implementation discussion<br>
Anserme, better README.md and packaging

# running

cd examples/basic/1_mc_methane_in_box<br>
./run

# dependencies

- cmake 3.28 or higher<br>
- ninja 1.11 or higher<br>
- llvm-18 or higher<br>
- python3 and pybind11<br>
- blas and lapack (64-bit integers)


# Installation Guide

<!-- TOC -->
* [Installation Guide](#installation-guide)
    * [Presets](#presets)
    * [Linux](#linux)
      * [Ubuntu 24](#ubuntu-24)
      * [Fedora 40](#fedora-40)
    * [macOS](#macos)
    * [Python](#python)
<!-- TOC -->

### presets

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

### linux

#### Ubuntu 24

apt-get install -y --no-install-recommends git ca-certificates cmake ninja-build<br>
apt-get install -y --no-install-recommends llvm lld clang clang-tools clang-tidy libc++-dev libc++abi-dev libomp-dev libclang-rt-dev<br>
apt-get install -y --no-install-recommends python3 pybind11-dev python3-pybind11 python3-dev<br>
apt-get install -y --no-install-recommends liblapack64-dev libblas64-dev<br>
cmake -B build --preset=linux-ubuntu-24<br>
ninja -C build<br>
ninja -C build install<br>
ctest --test-dir build/tests/raspakit-tests --verbose

#### Fedora 40

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

### macOS
brew install llvm ninja cmake doxygen graphviz lapack pybind11<br>
cmake -B --preset macos-apple-silicon<br>
(or cmake -B --preset macos-intel)<br>
ninja -C build<br>
ninja -C build install<br>
ctest --test-dir build/tests/raspakit-tests --verbose


### python

This package can also be built as a library for python. To build the python package the pip packaging system can be used. Note that due to compilation of the full package this might take a few minutes. To install, run the following command:

```bash
export CMAKE_ARGS="--preset=python"
pip install .
```

This will install the package to the current python environment.

We strongly advise users to use the CMakePresets.json preset for their given system. For building the python package with a given preset change the following line to reflect the given preset:

```
export CMAKE_ARGS="--preset=macos-apple-silicon"
pip install .
```

# How To Use

1. Set environment variables

* For bash

   ```bash
   echo 'export RASPA_DIR=/usr/share/raspa3' >> ~/.bashrc
   ```

* For zsh

   ```bash
   echo 'export RASPA_DIR=/usr/share/raspa3' >> ~/.zshrc
   ```

2. Run

   ```bash
   raspa
   ```
