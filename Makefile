
a.out: main.o create_test_bmp.o
		gcc main.o create_test_bmp.o

main.o: main.c header.h
		gcc -c main.c

create_test_bmp.o: create_test_bmp.c header.h
		gcc -c create_test_bmp.c 
clean: 
	rm -f *.o a.out