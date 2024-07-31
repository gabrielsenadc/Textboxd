make compress:
	gcc -g -o compacta mcompress.c compress.c bitmap.c -lm

make decompress:
	gcc -g -o descompacta mdecompress.c compress.c bitmap.c -lm

make runcompress:
	gcc -g -o compacta mcompress.c compress.c bitmap.c -lm
	valgrind --leak-check=full ./compacta $(f)

make rundecompress:
	gcc -g -o descompacta mdecompress.c compress.c bitmap.c -lm
	valgrind --leak-check=full ./descompacta $(f)
