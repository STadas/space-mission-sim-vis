# SpaceMissionSimVis
## About
SpaceMissionSimVis is an application that is meant to make producing PANGU spacecraft mission simulations easier by making the process more interactive and intuitive. It includes features like:
- launching and connecting to a PANGU server from the application for convenience (the user still needs to have their own PANGU server executable including the license)
- sending flight file commands to the server
- viewing the commands' results as images generated by the server
- displaying the progress of the flight file in an interactive:
  - 3D graph-like visualisation
  - playback interface

## Dependencies
The only real dependency is Qt 5.15.2, though a threads (and winsock2 for Windows) library should also be present on the system (which usually is by default).

## Building
```bash
cmake --build build --config Release
```

## Installing
```bash
cmake --install build
```

## Documentation
A user manual is available as a [pdf file](SpaceMissionSimVis_User_Manual.pdf).
Documentation for the source code can be generated with `doxygen`.

## License
Licensed under the [GNU General Public License v3.0](LICENSE).

---
Developed as part of my 4th year honors project in the University of Dundee.
