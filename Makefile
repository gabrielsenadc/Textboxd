make compress:
	gcc -o compacta mcompress.c compress.c bitmap.c -lm

make decompress:
	gcc -o descompacta mdecompress.c compress.c bitmap.c -lm