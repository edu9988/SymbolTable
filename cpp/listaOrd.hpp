/**************************************************************/
/* José Eduardo R. M. Peres y Peres       No. USP:8945332     */
/* Exercicio-Programa EP1                                     */
/* Curso MAC323 - Prof. Carlos Eduardo Ferreira	              */
/*                                                            */
/* Compilador: GNU GCC Compiler                               */
/**************************************************************/

/*
Arquivo listaOrdenada.hpp
Contem interface e implementacao de listaOrd.
*/

#ifndef LISTAORD_HPP
#define LISTAORD_HPP

using namespace std;
#include <iostream> /* cout */
#include <cctype> /* tolower */
#include <fstream> /* ifstream, open, get */
#include <cstring> /* strcmp, strlen */
#include "Par.hpp"

/*
listaOrd:
Definicao da classe listaOrd.
*/
template <class Chave, class Item>
class listaOrd
{
    private:
	typedef struct celula{
	    Par<Chave,Item> P;
	    struct celula *ant;
	    struct celula *prox;
	}Celula;
	typedef Celula* Cell;
	Cell Head;/*cabeca da lista*/
	bool achouOrd( Chave , Cell* );

	char dummy_ch = '\0';
	Chave CHAVE_VAZIO = &dummy_ch;
	int dummy_val = 0;
	Item ITEM_VAZIO = &dummy_val;

	void debug_print_all();
    public:
	listaOrd( string );
	listaOrd();
	~listaOrd();

	void insere( Chave , Item );
	Item devolve( Chave );
	void remove( Chave );
	int rank( Chave );
	Chave seleciona( int );
};

/*
listaOrd():
*/
template <class Chave, class Item>
listaOrd<Chave,Item>::listaOrd():
Head( new Celula ){
    Head->ant = nullptr;
    Head->prox = nullptr;
    Head->P.setCh( CHAVE_VAZIO );
    Head->P.setVal( ITEM_VAZIO );
}

/*
listaOrd():
Construtor; Aloca a celula cabeca de lista, le do
arquivo de entrada inserindo as palavras na lista
de forma ordenada.
*/
template <class Chave, class Item>
listaOrd<Chave,Item>::listaOrd( string file_name ):
Head( new Celula ){
    int i;
    char a, buff[50];
    ifstream ptr;
    Head->ant = nullptr;
    Head->prox = nullptr;
    Head->P.setCh( CHAVE_VAZIO );
    Head->P.setVal( ITEM_VAZIO );
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
~listaOrd():
Destrutor; libera a memória alocada para a lista.
*/
template <class Chave, class Item>
listaOrd<Chave,Item>::~listaOrd()
{
    Cell ptr = Head->prox;
    while( ptr != nullptr ){
	delete [] ptr->P.getCh();
	ptr = ptr->prox;
	delete Head->prox;
	Head->prox = ptr;
    }
    delete Head;
}

/*
debug_print_all():
Funcao para auxiliar na depuracao; imprime todas
as chaves e itens com numeracao. A numeracao diz
quantos elementos ha na lista entre aquele e a 
cabeca.
*/
template <class Chave, class Item>
void listaOrd<Chave,Item>::debug_print_all(){
    int i = 0;
    Cell ptr = Head->prox;
    while( ptr != nullptr ){
	cout << i << "\t" << ptr->P.getCh(); 
	cout << "\t" << *ptr->P.getVal() << endl;
	ptr = ptr->prox;
	i++;
    }
}

/*
insere():
Recebe uma par Chave-Item e o insere na lista; se
a chave já está na lista, apenas insere o novo valor
naquela posicao. Se a chave ainda nao esta na lista,
cria uma nova celula na posicao correta da lista
para armazenar os novos valores.
*/
template <class Chave, class Item>
void listaOrd<Chave,Item>::insere( Chave chave, Item valor )
{
    int len;
    Cell ptr, aux;
    if( achouOrd( chave , &ptr ) )
	ptr->P.setVal( valor );
    else{
	len = strlen(chave);
	Chave ch_aux = new char[len+1];
	for( int j=0 ; j<=len ; j++ )
	    ch_aux[j] = chave[j];
	aux = new Celula;
	aux->P.setCh( ch_aux );
	aux->P.setVal( valor );
	aux->ant = ptr;
	aux->prox = ptr->prox;
	if( ptr->prox != nullptr )
	    ptr->prox->ant = aux;
	ptr->prox = aux;
    }
}

/*
achouOrd():
Funcao auxiliar; procura uma celula com determinada
chave e devolve verdadeiro se for encontrada, falso
caso contrario; devolve tambem no ponteiro ret,
endereco da celula encontrada(caso tenha sido), ou
endereco de onde deve ser inserida(se nao foi
encontrada).
*/
template <class Chave, class Item>
bool listaOrd<Chave,Item>::achouOrd( Chave chave , Cell *ret )
{
    int aux;
    Cell ptr = Head->prox;
    *ret = Head;
    if( ptr == nullptr )
	return false;
    while( ptr != nullptr ){
	aux = strcmp( chave, ptr->P.getCh() );
	if( aux < 0 )
	    return false;
	*ret = ptr;
	if( aux == 0 )
	    return true;
	ptr = ptr->prox;
    }
    return false;
}

/*
devolve():
Recebe um objeto da classe Chave; se a chave
está na lista, devolve seu Item; devolve
ITEM_VAZIO caso contrario.
*/
template <class Chave, class Item>
Item listaOrd<Chave,Item>::devolve( Chave chave )
{
    Cell ptr;
    if( achouOrd( chave , &ptr ) )
	return ptr->P.getVal();
    else
	return ITEM_VAZIO;
}

/*
remove():
Remove uma celula com determinada chave; se
a chave nao esta na lista, nao faz nada.
*/
template <class Chave, class Item>
void listaOrd<Chave,Item>::remove( Chave chave ){
    Cell ptr;
    if( achouOrd( chave , &ptr ) ){
	delete [] ptr->P.getCh();
	ptr->ant->prox = ptr->prox;
	if( ptr->prox != nullptr )
	    ptr->prox->ant = ptr->ant;
	delete ptr;
    }
}

/*
rank():
Devolve quantos objetos ha' na lista com Chave
menor ou igual ao valor de entrada. Se a lista
esta vazia, devolve 0; se a chave nao esta na
lista, devolve o rank que teria se estivesse.
*/
template <class Chave, class Item>
int listaOrd<Chave,Item>::rank( Chave chave ){
    int aux, count = 0;
    Cell ptr = Head->prox;
    if( ptr == nullptr )
	return 0;
    do{
	aux = strcmp( chave , ptr->P.getCh() );
	if( aux > 0 )
	    count++;
	else
	    break;
	ptr = ptr->prox;
    }while( ptr != nullptr );
    return count;
}

/*
seleciona():
Devolve a chave de rank k. Se ha menos que
k+1 elementos na lista, devolve CHAVE_VAZIO.
*/
template <class Chave, class Item>
Chave listaOrd<Chave,Item>::seleciona( int k ){
    int i;
    Cell ptr = Head->prox;
    if( ptr == nullptr )/*lista vazia*/
	return CHAVE_VAZIO;
    for( i=0 ; i<k ; i++ ){
	ptr = ptr->prox;
	if( ptr == nullptr )
	    return CHAVE_VAZIO;
    }
    return ptr->P.getCh();
}

#endif
