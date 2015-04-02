edflib
======

A C++ implementation of the EDF file format toolkit.

Included are a linux build script, NetBeans project, and a XCode project with a simple read test codeset.

## cmake
```bash
cd build
cmake ..
make
```

## Xcode
Choose schema and run
- edflib (build static lib)
- test (small example application)
- Unit Tests

Application inputs for Xcode can be changed by editing the schemas.

Inputs for the test application and unit tests are:
- ./testapp ../sample.edf <1..37>
- ./units -d yes -r console -- ../sample.edf
