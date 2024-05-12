FROM artemisbeta/geant4

# RUN git clone https://github.com/jonni2/Geant4-Tokamak.git

COPY . /home/Geant4_Tokamak

WORKDIR /home/Geant4_Tokamak

RUN source /usr/local/share/geant4/install/4.11.1/bin/geant4.sh

RUN mkdir build

WORKDIR /home/Geant4_Tokamak/build

RUN cmake ..

RUN make

# CMD ./Tokamak_Breeding.exe

# CMD 
