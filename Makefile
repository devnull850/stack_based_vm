
all: vm asm mas

vm: vm.c
	gcc -Wall vm.c -o vm

asm: asm.c
	gcc -Wall asm.c -o asm

mas: mas.c
	gcc -Wall mas.c -o mas

.PHONY:
clean:
	rm vm asm mas
