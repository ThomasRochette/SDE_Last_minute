CC = gcc
flag = -Wall
head = struct.h fonction.h ecrivain.h
exec = utilisateur agence affichage tirage


all : $(exec)

%: %.c $(head)
	$(CC) $(flag) $< -o $@
clean:
	rm -rf $(exec)
