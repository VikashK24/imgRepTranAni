a.out: main.o create_test_bmp.o jarvis_bmp.o thor_bmp.o delay.o
	gcc main.o create_test_bmp.o jarvis_bmp.o thor_bmp.o delay.o

main.o: main.c header.h
	gcc -c main.c

create_test_bmp.o: create_test_bmp.c header.h
	gcc -c create_test_bmp.c 

jarvis_bmp.o: jarvis_bmp.c header.h
	gcc -c jarvis_bmp.c

thor_bmp.o: thor_bmp.c header.h
	gcc -c thor_bmp.c

delay.o: delay.s
	as delay.s -o delay.o

clean: 
	rm -f *.o a.out *.bmp
