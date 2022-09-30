CC=gcc
FILES=main.c updates/webhook.c
OUT=tgpihole

all:
	$(CC) $(FILES) -o $(OUT)

clean:
	rm -f $(OUT)