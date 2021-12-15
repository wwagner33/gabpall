
## o que precisa para compilar?

1) OPENMPI:
sudo apt-get update
sudo apt-get install openmpi-bin
echo "export PATH=\$PATH:\$HOME/opt/openmpi/bin" >> $HOME/.bashrc
echo "export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:\$HOME/opt/openmpi/lib" \ >> $HOME/.bashrc
source ~/.bashrc
Depois testa: mpicc --version

2) Bison:
sudo apt install bison

3) Flex
sudo apt install flex

4) Chama o make:
Entra na pasta do projeto e digita make

# O que modifiquei:
Coloquei um include da stdlib.h no main.c e na linha 236 do grammar.y
inseri o string.h e stdlib.h em todos os arquivos .c
Removi os cabeçalhos malloc.h que havia nos .c gedit print.c&

Renomeei o prog2.gm para prog1.gm

# O que o MAKE faz:

rm -f scan.c 
lex  -t scan.l > scan.c
cc -g   -c -o scan.o scan.c
cc -g   -c -o main.o main.c
cc -g   -c -o parser.o parser.c
cc -g   -c -o misc.o misc.c
cc -g   -c -o print.o print.c
cc -g   -c -o check.o check.c
cc -g   -c -o vars.o vars.c
cc -g   -c -o code.o code.c
cc -g   -c -o printcode.o printcode.c
cc -g   -c -o pattern.o pattern.c
mpicc -g grammar.o scan.o main.o parser.o misc.o print.o check.o vars.o code.o printcode.o pattern.o  -o gm
rm scan.c

