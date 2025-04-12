
obj-m := char_driver.o

KERNEL_DIR = /usr/src/linux-headers-$(shell uname -r)

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(shell pwd) modules
	mkdir -p build
	mv *.o *.ko *.mod.* *.symvers *.order build
	
app: 
	gcc -o bin/test1 deadlocks/deadlock1.c

clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order .* bin/* build
