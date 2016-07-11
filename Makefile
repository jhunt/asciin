all: asciin

asciin: asciin.c
	$(CC) -o $@ $<

clean:
	rm -f asciin

.PHONY: clean all
