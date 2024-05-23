FROM artemisbeta/geant4

WORKDIR /home

COPY . /home/Geant4_Tokamak

# When inside the docker virtual machine type:
# source /usr/local/share/geant4/install/4.11.1/bin/geant4.sh
