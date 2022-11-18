This folder is represents the Geant4 simulation of neutron detector system which include two shielding (Lead and PE) materials, thermal neutron converter layer made of (6Li, 6LiF, 6Li2CO3, 6LiH) and Sensitive detector (LYSO or Si).

Quick instructions:
1- From build folder compile the Geant4 file using cmake ..  Then make
2- Change the macros path in main.mac file to your path.
3- Run the code using ./pstep (For visualisation window)  OR ./pstep main.mac (Without visualisation window, this is more quicker)
4- The number of events can be increased from main.mac file 
5- The radioactive source can be controlled from main.mac file. Currently, there are four sources are defined (Thermal neutron, AmBe, Sr90 and Gamma) sources. The source selection can be selected from main.mac file.
6- In order to change any material or geometry, should done through DetectorConstruction.cc in src folder.
7- In order to change the physics list, this can be done from pstep.cc file.  