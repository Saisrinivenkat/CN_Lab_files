CC = g++

# build: client.c
# 	$(CC) -c client.c


clean:
	ls | grep -v "\." | xargs rm
	rm tempCodeRunnerFile.c