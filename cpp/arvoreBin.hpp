/**************************************************************/
/* José Eduardo R. M. Peres y Peres       No. USP:8945332     */
/* Exercicio-Programa EP1                                     */
/* Curso MAC323 - Prof. Carlos Eduardo Ferreira	              */
/*                                                            */
/* Compilador: GNU GCC Compiler                               */
/**************************************************************/

/*
Arquivo arvoreBin.hpp
Contem interface e implementacao de arvoreBin.
*/

#ifndef ARVOREBIN_HPP
#define ARVOREBIN_HPP

using namespace std;
#include <iostream> /* cout */
#include <cctype> /* tolower */
#include <fstream> /* ifstream, open, get */
#include <cstring> /* strcmp, strlen */
#include "Par.hpp"

/*
arvoreBin:
Definicao da classe arvoreBin.
*/
template <class Chave, class Item>
class arvoreBin
{
    private:
	typedef struct node{
	    Par<Chave,Item> P;
	    struct node *esq;
	    struct node *dir;
	    struct node *pai;
	}Node;
	typedef Node* Root;
	Root Raiz;

	char dummy_ch = '\0';
	Chave CHAVE_VAZIO = &dummy_ch;
	int dummy_val = 0;
	Item ITEM_VAZIO = &dummy_val;

	/*funcoes auxiliares*/
	int conta_R( Root );
	void Node_Destructor_R( Root );
	bool achouBin( Chave , Root* );
	void debug_print_all();
	void debug_print_R( Root );
    public:
	arvoreBin( string );
	~arvoreBin();

	void insere( Chave , Item );
	Item devolve( Chave );
	void remove( Chave );
	int rank( Chave );
	Chave seleciona( int );
};

/*
arvoreBin():
Construtor; recebe o nome de um arquivo como
argumento; le do arquivo dado, inserindo
as palavras na arvore binaria.
*/
template <class Chave, class Item>
arvoreBin<Chave,Item>::arvoreBin( string file_name ):
Raiz(){
    int i;
    char a, buff[50];
    ifstream ptr;
    Raiz = nullptr;
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
    debug_print_all();
}

/*
~arvoreBin():
Destrutor; libera a memória alocada para a ABB.
*/
template <class Chave, class Item>
arvoreBin<Chave,Item>::~arvoreBin()
{
    Node_Destructor_R( Raiz );
}

/*
Node_Destructor_R():
Recebe como entrada uma ABB; libera a memoria
recursivamente de suas sub-arvores esquerda e
direita, e depois da propria raiz (pos-ordem).
*/
template <class Chave, class Item>
void arvoreBin<Chave,Item>::Node_Destructor_R( Root ptr )
{
    if( ptr->esq != nullptr)
	Node_Destructor_R( ptr->esq );
    if( ptr->dir != nullptr)
	Node_Destructor_R( ptr->dir );
    delete [] ptr->P.getCh();
    delete ptr;
}

/*
debug_print_all():
Funcao para auxiliar na depuracao; imprime todas
as chaves e itens.
*/
template <class Chave, class Item>
void arvoreBin<Chave,Item>::debug_print_all(){
    if( Raiz != nullptr )
	debug_print_R( Raiz );
}

/*
debug_print_R():
Funcao para auxiliar na depuracao; imprime
recursivamente o conteudo das sub-arvores
e do proprio No, na sequencia in-ordem.
*/
template <class Chave, class Item>
void arvoreBin<Chave,Item>::debug_print_R( Root ptr ){
    if( ptr->esq != nullptr )
	debug_print_R( ptr->esq );
    cout << ptr->P.getCh() << "\t";
    cout << *(ptr->P.getVal()) << endl;
    if( ptr->dir != nullptr )
	debug_print_R( ptr->dir );
}

/*
insere():
Recebe uma par Chave-Item e o insere na ABB; se
a chave já está na ABB, apenas insere o novo valor
naquela posicao. Se a chave ainda nao esta na ABB,
cria uma nova celula na posicao correta da ABB
para armazenar os novos valores.
*/
template <class Chave, class Item>
void arvoreBin<Chave,Item>::insere( Chave chave, Item valor )
{
    int auxi;
    if( Raiz == nullptr ){
	auxi = strlen(chave);
	Chave ch_aux = new char[auxi+1];
	for( int j=0 ; j<=auxi ; j++ )
	    ch_aux[j] = chave[j];
	Raiz = new Node;
	Raiz->P.setCh( ch_aux );
	Raiz->P.setVal( valor );
	Raiz->esq = nullptr;
	Raiz->dir = nullptr;
	Raiz->pai = nullptr;
	return;
    }
    Root ptr, aux;
    if( achouBin( chave , &ptr ) )
	ptr->P.setVal( valor );
    else{
	auxi = strlen(chave);
	Chave ch_aux = new char[auxi+1];
	for( int j=0 ; j<=auxi ; j++ )
	    ch_aux[j] = chave[j];
	aux = new Node;
	aux->P.setCh( ch_aux );
	aux->P.setVal( valor );
	aux->esq = nullptr;
	aux->dir = nullptr;
	aux->pai = ptr;
	auxi = strcmp( chave , ptr->P.getCh() );
	if( auxi < 0)
	    ptr->esq = aux;
	else
	    ptr->dir = aux;
    }
}

/*
achouBin():
Funcao auxiliar, procura um No com determinada
chave e devolve verdadeiro se for encontrada, falso
caso contrario; tambem devolve no ponteiro ret,
endereco do No encontrado(caso tenha sido), ou
endereco de onde deve ser inserido(se nao foi
encontrado).
*/
template <class Chave, class Item>
bool arvoreBin<Chave,Item>::achouBin( Chave chave , Root *ret )
{
    int aux;
    Root ptr = Raiz;
    *ret = nullptr;
    if( ptr == nullptr )
	return false;
    while( ptr != nullptr ){
	*ret = ptr;
	aux = strcmp( chave , ptr->P.getCh() );
	if( aux == 0 )
	    return true;
	else if( aux < 0 )
	    ptr = ptr->esq;
	else
	    ptr = ptr->dir;
    }
    return false;
}

/*
devolve():
Recebe uma Chave e retorna o valor de seu
Item presente na ABB.
*/
template <class Chave, class Item>
Item arvoreBin<Chave,Item>::devolve( Chave chave )
{
    Root ptr;
    if( achouBin( chave , &ptr ) )
	return ptr->P.getVal();
    else
	return ITEM_VAZIO;
}

/*
remove():
Remove uma celula com determinada chave; se
a chave nao esta na ABB, nao faz nada.
*/
template <class Chave, class Item>
void arvoreBin<Chave,Item>::remove( Chave chave ){
    Root ptr, aux;
    if( achouBin( chave , &ptr ) ){
	if( ptr->pai == nullptr ){
	    if( ptr->esq == nullptr && ptr->dir == nullptr )
		Raiz = nullptr;
	    else if( ptr->esq != nullptr && ptr->dir == nullptr ){
		ptr->esq->pai = nullptr;
		Raiz = ptr->esq;
	    }
	    else if( ptr->esq == nullptr && ptr->dir != nullptr ){
		ptr->dir->pai = nullptr;
		Raiz = ptr->dir;
	    }
	    else{
		aux = ptr->esq;
		while( aux->dir != nullptr )
		    aux = aux->dir;
		ptr->dir->pai = aux;
		aux->dir = ptr->dir;
		Raiz = ptr->esq;
		ptr->esq->pai = nullptr;
	    }
	}
	else{/*pai != nullptr*/
	    if( ptr->esq == nullptr && ptr->dir == nullptr ){
		if( ptr->pai->esq == ptr )
		    ptr->pai->esq = nullptr;
		else
		    ptr->pai->dir = nullptr;
	    }
	    else if( ptr->esq != nullptr && ptr->dir == nullptr ){
		ptr->esq->pai = ptr->pai;
		if( ptr->pai->esq == ptr )
		    ptr->pai->esq = ptr->esq;
		else
		    ptr->pai->dir = ptr->esq;
	    }
	    else if( ptr->esq == nullptr && ptr->dir != nullptr ){
		ptr->dir->pai = ptr->pai;
		if( ptr->pai->esq == ptr )
		    ptr->pai->esq = ptr->dir;
		else
		    ptr->pai->dir = ptr->dir;
	    }
	    else{
		aux = ptr->esq;
		while( aux->dir != nullptr )
		    aux = aux->dir;
		aux->dir = ptr->dir;
		ptr->dir->pai = aux;
		ptr->esq->pai = ptr->pai;
		if( ptr->pai->esq == ptr )
		    ptr->pai->esq = ptr->esq;
		else
		    ptr->pai->dir = ptr->esq;
	    }
	}
	delete [] ptr->P.getCh();
	delete ptr;
    }
    else /*nao achou*/
	cout << "Nao ";
}

/*
rank():
Devolve quantos objetos ha' na lista com Chave
menor ou igual ao valor de entrada. Se a lista
esta vazia, devolve 0; se a chave nao esta na
lista, devolve o rank que teria se estivesse.
*/
template <class Chave, class Item>
int arvoreBin<Chave,Item>::rank( Chave chave ){
    if( Raiz == nullptr )
	return 0;
    int aux, count = 0;
    Root ptr = Raiz;
    while( ptr != nullptr ){
	aux = strcmp( chave , ptr->P.getCh() );
	if( aux == 0 )
	    return count + conta_R( ptr->esq );
	else if( aux < 0 )
	    ptr = ptr->esq;
	else{
	    count += 1 + conta_R( ptr->esq );
	    ptr = ptr->dir;
	}
    }
    return count;
}

/*
conta_R():
Funcao auxiliar; devolve quantos elementos ha
na subarvore de entrada; utiliza recursao.
*/
template <class Chave, class Item>
int arvoreBin<Chave,Item>::conta_R( Root r ){
    if( r == nullptr )
	return 0;
    return conta_R( r->esq ) +1 +conta_R( r->dir );
}

/*
seleciona():
Devolve a chave de rank k. Se ha menos que
k+1 elementos na lista, devolve CHAVE_VAZIO.
*/
template <class Chave, class Item>
Chave arvoreBin<Chave,Item>::seleciona( int k ){
    if( Raiz == nullptr )
	return CHAVE_VAZIO;
    int aux, count = 0;
    Root ptr = Raiz;
    while( ptr != nullptr ){
	aux = conta_R( ptr->esq );
	if( count + aux == k )
	    return ptr->P.getCh();
	else if( count + aux > k )
	    ptr = ptr->esq;
	else{
	    count += 1 + aux;
	    ptr = ptr->dir;
	}
    }
    return CHAVE_VAZIO;
}

#endif
