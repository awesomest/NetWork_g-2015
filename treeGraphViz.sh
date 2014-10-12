c++ rand.cpp 
./a.out > randOut0
c++ graphDrawing.cpp
./a.out < randOut0
dot -Tpdf network.dot -o network0.pdf; open network0.pdf
