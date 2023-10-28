# Neutron production and interaction in an ITER-like tokamak
This project simulates, by using the software Geant4, the production of neutrons in a very simplified tokamak model inspired from ITER.
The tokamak is designed as a series of toroidal and spherical layers which exemplify the layers and materials used in ITER, for example the $`1^{st}`$ wall and the central solenoid.
The neutrons are produced by a toroidal source in the center of the tokamak and have an energy of 14.1 MeV.


To build the project:
```
mkdir build && cd build
cmake ..
make
./project run.mac
```

## References
Mohammad Mehdi Nasseri, "Three Dimensional Simulation of ITER Machine by Using Geant4", Journal of Fusion Energy (2016). [Link to article](https://link.springer.com/article/10.1007/s10894-016-0083-0)
