CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -fPIC
PTHREAD = -pthread
RTLIB = -lrt
RPATH = -Wl,-rpath,'$$ORIGIN'

LIB_LOCAL = libclaves.so
LIB_PROXY = libproxyclaves.so

CLIENTE1_MQ = cliente1_mq
CLIENTE2_MQ = cliente2_mq
CLIENTE3_MQ = cliente3_mq
CLIENTE4_MQ = cliente4_mq
SERVIDOR_MQ = servidor_mq

OBJ_LOCAL = storage.o claves.o
OBJ_PROXY = proxy-mq.o
OBJ_SERVER = servidor-mq.o

OBJ_APP1 = app_cliente1.o
OBJ_APP2 = app_cliente2.o
OBJ_APP3 = app_cliente3.o
OBJ_APP4 = app_cliente4.o

.PHONY: all clean

all: $(LIB_LOCAL) $(LIB_PROXY) $(SERVIDOR_MQ) $(CLIENTE1_MQ) $(CLIENTE2_MQ) $(CLIENTE3_MQ) $(CLIENTE4_MQ)

$(LIB_LOCAL): $(OBJ_LOCAL)
	$(CC) -shared -o $(LIB_LOCAL) $(OBJ_LOCAL) $(PTHREAD)

$(LIB_PROXY): $(OBJ_PROXY)
	$(CC) -shared -o $(LIB_PROXY) $(OBJ_PROXY) $(RTLIB)

$(SERVIDOR_MQ): servidor-mq.o $(LIB_LOCAL)
	$(CC) -o $(SERVIDOR_MQ) servidor-mq.o -L. -lclaves $(PTHREAD) $(RTLIB) $(RPATH)

$(CLIENTE1_MQ): app_cliente1.o $(LIB_PROXY)
	$(CC) -o $(CLIENTE1_MQ) app_cliente1.o -L. -lproxyclaves $(RTLIB) $(RPATH)

$(CLIENTE2_MQ): app_cliente2.o $(LIB_PROXY)
	$(CC) -o $(CLIENTE2_MQ) app_cliente2.o -L. -lproxyclaves $(RTLIB) $(RPATH)

$(CLIENTE3_MQ): app_cliente3.o $(LIB_PROXY)
	$(CC) -o $(CLIENTE3_MQ) app_cliente3.o -L. -lproxyclaves $(RTLIB) $(RPATH)

$(CLIENTE4_MQ): app_cliente4.o $(LIB_PROXY)
	$(CC) -o $(CLIENTE4_MQ) app_cliente4.o -L. -lproxyclaves $(RTLIB) $(RPATH)

storage.o: storage.c storage.h claves.h
	$(CC) $(CFLAGS) -c storage.c -o storage.o

claves.o: claves.c claves.h storage.h
	$(CC) $(CFLAGS) -c claves.c -o claves.o

proxy-mq.o: proxy-mq.c claves.h mensajes.h
	$(CC) $(CFLAGS) -c proxy-mq.c -o proxy-mq.o

servidor-mq.o: servidor-mq.c claves.h mensajes.h
	$(CC) $(CFLAGS) -c servidor-mq.c -o servidor-mq.o

app_cliente1.o: app_cliente1.c claves.h
	$(CC) $(CFLAGS) -c app_cliente1.c -o app_cliente1.o

app_cliente2.o: app_cliente2.c claves.h
	$(CC) $(CFLAGS) -c app_cliente2.c -o app_cliente2.o

app_cliente3.o: app_cliente3.c claves.h
	$(CC) $(CFLAGS) -c app_cliente3.c -o app_cliente3.o

app_cliente4.o: app_cliente4.c claves.h
	$(CC) $(CFLAGS) -c app_cliente4.c -o app_cliente4.o

clean:
	rm -f *.o *.so $(CLIENTE1_MQ) $(CLIENTE2_MQ) $(CLIENTE3_MQ) $(CLIENTE4_MQ) $(SERVIDOR_MQ)