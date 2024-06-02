COMP := g++
FLAGS := -o

compilar: makedir
	$(COMP) server.cpp $(FLAGS) ArchivosCompilados/servidor
	$(COMP) cliente.cpp $(FLAGS) ArchivosCompilados/cliente

makedir:
	mkdir -p ArchivosCompilados

clean:
	rm -f servidor cliente
	rm -r ArchivosCompilados

	
