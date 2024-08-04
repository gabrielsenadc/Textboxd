/**
 * Trabalho 2 de Estrutura de Dados
 * Professora: Patrícia Dockhorn Costa
 * Alunos:
 * Eduardo Silva Guimarães
 * Gabriel Sena da Cunha
 */

#ifndef compress_h
#define compress_h

#include "bitmap.h"

/**
 * Compacta um arquivo, gerando um arquivo .comp com as informações compactadas.
 * @param file_name nome do arquivo a ser compactado.
 */
void compress(char * file_name);

/**
 * Descompacta um arquivo .comp, gerando o arquivo normal
 * @param file_name nome do arquivo a ser descompactado.
 */
void decompress(char * file_name);

#endif