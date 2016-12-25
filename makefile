CC = gcc
flag = -Wall
head = struct.h
exec = utilisateur agence affichage


all : $(exec)

%: %.c $(head)
	$(CC) $(flag) $< -o $@
clean:
	rm -rf $(exec)
