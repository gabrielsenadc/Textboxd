make compress:
	gcc -g -o compacta mcompress.c compress.c bitmap.c -lm

make decompress:
	gcc -g -o descompacta mdecompress.c compress.c bitmap.c -lm