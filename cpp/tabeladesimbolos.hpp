/**************************************************************/
/* José Eduardo R. M. Peres y Peres       No. USP:8945332     */
/* Exercicio-Programa EP1                                     */
/* Curso MAC323 - Prof. Carlos Eduardo Ferreira	              */
/*                                                            */
/* Compilador: GNU GCC Compiler                               */
/**************************************************************/

/*
Arquivo tabeladesimbolos.hpp:
*/

#ifndef TABELADESIMBOLOS_HPP
#define TABELADESIMBOLOS_HPP

bool ignorable( char );

#include "vetorDes.hpp"
#include "vetorOrd.hpp"
#include "listaDes.hpp"
#include "listaOrd.hpp"
#include "arvoreBin.hpp"
#include "treap.hpp"
#include "arvore23.hpp"
#include "arvoreRN.hpp"
#include "hashTable.hpp"

/*
ignorable():
Recebe um char; devolve false se o char recebido e'
uma letra do alfabeto, true caso contrario.
*/
bool ignorable( char simbolo ){
    if( (simbolo >= 'A' && simbolo <= 'Z') ||
	(simbolo >= 'a' && simbolo <= 'z') )
	return false;
    else
	return true;
}

#endif
