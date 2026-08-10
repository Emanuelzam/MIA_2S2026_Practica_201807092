# compila el analizador de consola y el servidor web (API REST)
CXX = g++
CXXFLAGS = -std=c++17 -Wall
SRCS = analizador.cpp utils.cpp mkdisk.cpp rmdisk.cpp fdisk.cpp mount.cpp mkfs.cpp mkusr.cpp rmusr.cpp mkfile.cpp

analizador: main.cpp $(SRCS)
	$(CXX) $(CXXFLAGS) main.cpp $(SRCS) -o analizador

servidor: server.cpp $(SRCS)
	$(CXX) $(CXXFLAGS) server.cpp $(SRCS) -o servidor -lpthread

clean:
	rm -f analizador servidor

.PHONY: clean