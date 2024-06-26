/**************************************************************/
/* José Eduardo R. M. Peres y Peres       No. USP:8945332     */
/* Exercicio-Programa EP1                                     */
/* Curso MAC323 - Prof. Carlos Eduardo Ferreira	              */
/*                                                            */
/* Compilador: GNU GCC Compiler                               */
/**************************************************************/

/*
Arquivo listaDes.hpp
Contem interface e implementacao de listaDes.
*/

#ifndef LISTADES_HPP
#define LISTADES_HPP

using namespace std;
#include <iostream> /* cout */
#include <cctype> /* tolower */
#include <fstream> /* ifstream, open, get */
#include <cstring> /* strcmp, strlen */
#include "Par.hpp"

/*
listaDes:
Definicao da classe listaDes.
*/
template <class Chave, class Item>
class listaDes
{
    private:
	typedef struct celula{
	    Par<Chave,Item> P;
	    struct celula *ant;
	    struct celula *prox;
	}Celula;
	typedef Celula* Cell;
	Cell Head;
	bool achou( Chave , Cell* );

	char dummy_ch = '\0';
	Chave CHAVE_VAZIO = &dummy_ch;
	int dummy_val = 0;
	Item ITEM_VAZIO = &dummy_val;
	void debug_print_all();
	void qS( Cell * , int , int , int );
	int separa( Cell * , int , int );
    public:
	listaDes( string );
	~listaDes();
	void insere( Chave , Item );
	Item devolve( Chave );
	void remove( Chave );
	int rank( Chave );
	Chave seleciona( int );
};

/*
listaDes():
Construtor; Aloca a celula cabeca de lista, le do
arquivo de entrada inserindo as palavras na lista
na ordem de entrada.
*/
template <class Chave, class Item>
listaDes<Chave,Item>::listaDes( string file_name ):
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
~listaDes():
Destrutor; libera a memória alocada para a lista.
*/
template <class Chave, class Item>
listaDes<Chave,Item>::~listaDes()
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
void listaDes<Chave,Item>::debug_print_all(){
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
cria uma nova celula e a insere no inicio (primeira
posicao apos a cabeca)da lista.
*/
template <class Chave, class Item>
void listaDes<Chave,Item>::insere( Chave chave, Item valor )
{
    int len;
    Cell ptr;
    if( achou( chave , &ptr ) )
	ptr->P.setVal( valor );
    else{
	len = strlen(chave);
	Chave ch_aux = new char[len+1];
	for( int j=0 ; j<=len ; j++ )
	    ch_aux[j] = chave[j];
	ptr = new Celula;
	ptr->P.setCh( ch_aux );
	ptr->P.setVal( valor );
	ptr->ant = Head;
	ptr->prox = Head->prox;
	if( Head->prox != nullptr )
	    Head->prox->ant = ptr;
	Head->prox = ptr;
    }
}

/*
achou():
Funcao auxiliar; procura uma celula com determinada
chave e devolve verdadeiro se for encontrada, falso
caso contrario; devolve tambem no ponteiro ret,
endereco da celula encontrada(caso tenha sido).
*/
template <class Chave, class Item>
bool listaDes<Chave,Item>::achou( Chave chave , Cell *ret )
{
    Cell ptr = Head->prox;
    *ret = nullptr;
    if( ptr == nullptr )
	return false;
    while( ptr != nullptr ){
	if( !strcmp( chave , ptr->P.getCh() ) ){
	    *ret = ptr;
	    return true;
	}
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
Item listaDes<Chave,Item>::devolve( Chave chave )
{
    Cell ptr;
    if( achou( chave , &ptr ) )
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
void listaDes<Chave,Item>::remove( Chave chave ){
    Cell ptr;
    if( achou( chave , &ptr ) ){
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
int listaDes<Chave,Item>::rank( Chave chave ){
    int count = 0;
    Cell ptr = Head->prox;
    while( ptr != nullptr ){
	if( strcmp( ptr->P.getCh() , chave ) < 0 )
	    count++;
	ptr = ptr->prox;
    }
    return count;
}

/*
seleciona():
Devolve a chave de rank k. Se ha menos que
k+1 elementos na lista, devolve CHAVE_VAZIO.
*/
template <class Chave, class Item>
Chave listaDes<Chave,Item>::seleciona( int k ){
    int i, n;
    Cell ptr, *apontadores;
    if( k < 0 )
	return CHAVE_VAZIO;
    n = 0;
    ptr = Head->prox;
    while( ptr != nullptr ){
	n++;
	ptr = ptr->prox;
    }
    if( n <= k )
	return CHAVE_VAZIO;
    apontadores = new Cell[n];
    ptr = Head->prox;
    for( i=0 ; i<n ; i++ ){
	apontadores[i] = ptr;
	ptr = ptr->prox;
    }
    qS( apontadores , k , 0 , n-1 );
    ptr = apontadores[k];
    delete [] apontadores;
    return ptr->P.getCh();
}

/*
qS():
*/
template <class Chave, class Item>
void listaDes<Chave,Item>::qS( Cell *ponteiro , int alvo , int p , int r ){
    if( p <= r ){
	int q = separa( ponteiro , p , r );
	if( q < alvo )
	    qS( ponteiro , alvo , q+1 , r );
	else if( q > alvo )
	    qS( ponteiro , alvo , p , q-1 );
    }
}

/*
separa():
*/
template <class Chave, class Item>
int listaDes<Chave,Item>::separa( Cell *ponteiro , int p , int r ){
    int i, j, dif;
    Cell ptr, aux, troca;
    ptr = ponteiro[r];
    for( j=p-1 , i=p ; i<=r ; i++ ){
	aux = ponteiro[i];
	dif = strcmp( aux->P.getCh() , ptr->P.getCh() );
	if( dif <= 0 ){
	    troca = ponteiro[++j];
	    ponteiro[j] = ponteiro[i];
	    ponteiro[i] = troca;
	}
    }
    return j;
}

#endif
