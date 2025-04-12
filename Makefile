app: 
	gcc -o bin/test1 deadlocks/deadlock1.c
	gcc -o bin/test2 deadlocks/deadlock2.c

clean:
	rm -rf bin/*
	cd build
	rm -rf *.o *.ko *.mod.* *.symvers *.order 

