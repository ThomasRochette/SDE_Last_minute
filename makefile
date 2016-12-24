CC = gcc
flag = -Wall
fph = struct.h
exec = utilisateur agence affichage


all : $(exec)

%: %.c $(fph)
	$(CC) $(flag) $< -o $@
clean:
	rm -rf *.o

cleanAll: clean
	rm -rf programme
