/**************************************************************/
/* José Eduardo R. M. Peres y Peres       No. USP:8945332     */
/* Exercicio-Programa EP1                                     */
/* Curso MAC323 - Prof. Carlos Eduardo Ferreira	              */
/*                                                            */
/* Compilador: GNU GCC Compiler                               */
/**************************************************************/

/*
Arquivo hashTable.hpp
Contem interface e implementacao de hashTable.
*/

#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

using namespace std;
#include <iostream> /* cout */
#include <cctype> /* tolower */
#include <fstream> /* ifstream, open, get */
#include <cstring> /* strcmp, strlen */
#include "Par.hpp"
#include "listaOrd.hpp"

/*
hashTable:
Definicao da classe hashTable.
*/
template <class Chave, class Item>
class hashTable
{
    private:
	int size;
	listaOrd<Chave,Item> *L;

	char dummy_ch = '\0';
	Chave CHAVE_VAZIO = &dummy_ch;
	int dummy_val = 0;
	Item ITEM_VAZIO = &dummy_val;

	/*funcoes auxiliares*/
	bool achou( Chave );
	void debug_print_all();
	int hash( Chave );
    public:
	hashTable( string );
	~hashTable();

	void insere( Chave , Item );
	Item devolve( Chave );
	void remove( Chave );
	int rank( Chave );
	Chave seleciona( int );
};

/*
hashTable():
Construtor; recebe o nome de um arquivo como
argumento; le do arquivo dado, inserindo
as palavras na hashTable.
*/
template <class Chave, class Item>
hashTable<Chave,Item>::hashTable( string file_name ):
size(997){
    int i;
    char a, buff[50];
    ifstream ptr;
    L = new listaOrd<Chave,Item>[size];
    Item auxi = new int;
    ptr.open( file_name , ios_base::in );
    i = 0;
    while( !ptr.eof() ){
	ptr.get( a );
	if( ignorable(a) ){
	    if( i > 0 ){
		buff[i] = '\0';
		*auxi = *devolve( buff );
		*auxi += 1;
		insere( buff , auxi );
		i = 0;
	    }
	}
	else
	    buff[i++] = tolower(a);
    }
    delete auxi;
    ptr.close();
}

/*
~hashTable():
Destrutor; libera a memória alocada para a hashTable.
*/
template <class Chave, class Item>
hashTable<Chave,Item>::~hashTable()
{
    delete [] L;
}

/*
debug_print_all():
*/
template <class Chave, class Item>
void hashTable<Chave,Item>::debug_print_all(){
    for( int i=0 ; i<size ; i++ ){
	//printf( "============L[%3d]:============\n" , i );
	L[i].debug_print_all();
    }
}

/*
hash():
*/
template <class Chave, class Item>
int hashTable<Chave,Item>::hash( Chave A ){
    int L, i, sum;
    sum = 0;
    L = strlen( A );
    for( i=0 ; i<L ; i++ ){
	sum += (A[i] - 'a')*19;
    }
    sum = sum % size;
    return sum;
}

/*
insere():
*/
template <class Chave, class Item>
void hashTable<Chave,Item>::insere( Chave chave, Item valor )
{
    int hash_value = hash( chave );
    L[ hash_value ].insere( chave, valor );
}

/*
achou():
*/
template <class Chave, class Item>
bool hashTable<Chave,Item>::achou( Chave chave )
{
    int hash_value, *aux;
    hash_value = hash( chave );
    *aux = L[ hash_value ].devolve( chave );
    if( *aux > 0 )
	return true;
    else
	return false;
}

/*
devolve():
Recebe uma Chave e retorna o valor de seu
Item presente na hashTable.
*/
template <class Chave, class Item>
Item hashTable<Chave,Item>::devolve( Chave chave )
{
    int hash_value = hash( chave );
    return L[ hash_value ].devolve( chave );
}

/*
remove():
Remove uma celula com determinada chave; se
a chave nao esta na hashTable, nao faz nada.
*/
template <class Chave, class Item>
void hashTable<Chave,Item>::remove( Chave chave ){
    int hash_value = hash( chave );
    L[ hash_value ].remove( chave );
}

/*
rank():
Devolve quantos objetos ha' na lista com Chave
menor ou igual ao valor de entrada. Se a lista
esta vazia, devolve 0; se a chave nao esta na
lista, devolve o rank que teria se estivesse.
*/
template <class Chave, class Item>
int hashTable<Chave,Item>::rank( Chave chave ){
    int i, sum = 0;
    for( i=0 ; i<size ; i++ )
	sum += L[i].rank( chave );
    return sum;
}

/*
seleciona():
Devolve a chave de rank k. Se ha menos que
k+1 elementos na lista, devolve CHAVE_VAZIO.
*/
template <class Chave, class Item>
Chave hashTable<Chave,Item>::seleciona( int k ){
    int j, aux_int;
    Chave Aux_Key;
    for( int i=0 ; i<size ; i++ ){
	j = 0;
	do{
	    Aux_Key = L[i].seleciona( j );
	    if( !strcmp( Aux_Key , "\0" ) )
		break;
	    aux_int = this->rank( Aux_Key );
	    if( aux_int == k ){
		return Aux_Key;
	    }
	    j++;
	}while( aux_int < k );
    }
    return CHAVE_VAZIO;
}

#endif
