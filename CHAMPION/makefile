all: cliente arbitro jogo

cliente: cliente.o
	@ gcc cliente.o -o cliente -lpthread
	@ echo "executável do cliente - criado"

arbitro: arbitro.o
	@ gcc arbitro.o -o arbitro -lpthread
	@ echo "executável do arbitro - criado"

jogo: jogos/g_001 jogos/g_002 jogos/g_003

jogos/g_001: jogos/g_001.o
	@ gcc jogos/g_001.o -o jogos/g_001
	@ echo "executável do g_001 - criado"

jogos/g_002: jogos/g_002.o
	@ gcc jogos/g_002.o -o jogos/g_002
	@ echo "executável do g_002 - criado"

jogos/g_003: jogos/g_003.o
	@ gcc jogos/g_003.o -o jogos/g_003
	@ echo "executável do g_003 - criado"

cliente.o: cliente.c 
	@ gcc cliente.c -c
	@ echo "objeto do cliente - criado"

arbitro.o: arbitro.c
	@ gcc arbitro.c -c
	@ echo "objeto do arbitro - criado" 

jogos/g_001.o: jogos/g_001.c
	@ cd jogos && gcc g_001.c -c
	@ echo "objeto do g_001 - criado"

jogos/g_002.o: jogos/g_002.c
	@ cd jogos && gcc g_002.c -c
	@ echo "objeto do g_002 - criado"

jogos/g_003.o: jogos/g_003.c
	@ cd jogos && gcc g_003.c -c
	@ echo "objeto do g_003 - criado"

clean_o:
	@ rm *.o
	@ rm jogos/*.o
	@ echo "objetos - removidos"

clean:
	@ rm *.o
	@ rm jogos/*.o
	@ rm cliente
	@ rm arbitro
	@ rm jogos/g_001
	@ rm jogos/g_002
	@ rm jogos/g_003
	@ echo "objetos e executáveis - removidos"