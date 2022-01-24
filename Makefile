app: main.o encode.o decode.o
	gcc -o app main.o encode.o decode.o

main.o: main.c encode.h decode.h
	gcc -c main.c

encode.o: encode.c encode.h
	gcc -c encode.c

decode.o: decode.c decode.h
	gcc -c decode.c

clean:
	rm *.o
