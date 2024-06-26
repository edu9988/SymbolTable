/**************************************************************/
/* José Eduardo R. M. Peres y Peres       No. USP:8945332     */
/* Exercicio-Programa EP1                                     */
/* Curso MAC323 - Prof. Carlos Eduardo Ferreira	              */
/*                                                            */
/* Compilador: GNU GCC Compiler                               */
/**************************************************************/

/*
Arquivo treap.hpp
Contem interface e implementacao de treap.
*/

/*
Merge() baseada em funcao de mesmo nome encontrada em
https://cp-algorithms.com/data_structures/treap.html
(13/06/2020)
*/

#ifndef TREAP_HPP
#define TREAP_HPP

using namespace std;
#include <iostream> /* cout */
#include <ctime> /*time()*/
#include <cctype> /* tolower */
#include <fstream> /* ifstream, open, get */
#include <cstring> /* strcmp, strlen */
#include "Par.hpp"

/*
treap:
Definicao da classe treap.
*/
template <class Chave, class Item>
class treap
{
    private:
	typedef struct node{
	    Par<Chave,Item> P;
	    int prior;
	    struct node *esq;
	    struct node *dir;
	    struct node *pai;
	}Node;
	typedef Node* Tree;
	Tree Raiz;

	char dummy_ch = '\0';
	Chave CHAVE_VAZIO = &dummy_ch;
	int dummy_val = 0;
	Item ITEM_VAZIO = &dummy_val;

	/*funcoes auxiliares*/
	void rotate_right( Node* );
	void rotate_left( Node* );
	void sobe_heap( Node* );
	int conta_R( Tree );
	void Node_Destructor_R( Node* );
	bool achouBin( Chave , Tree* );
	void debug_print_all();
	void debug_print_R( Node* );
	Tree Merge( Tree , Tree );
    public:
	treap( string );
	~treap();

	void insere( Chave , Item );
	Item devolve( Chave );
	void remove( Chave );
	int rank( Chave );
	Chave seleciona( int );
};

/*
treap():
Construtor; recebe o nome de um arquivo como
argumento; le do arquivo dado, inserindo
as palavras na treap.
*/
template <class Chave, class Item>
treap<Chave,Item>::treap( string file_name ):
Raiz(){
    int i;
    char a, buff[50];
    ifstream ptr;
    Raiz = nullptr;
    Item auxi = new int;
    srand( time(NULL) );
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
~treap():
Destrutor; libera a memória alocada para a treap.
*/
template <class Chave, class Item>
treap<Chave,Item>::~treap()
{
    Node_Destructor_R( Raiz );
}

/*
Node_Destructor_R():
Recebe como entrada uma treap; libera a memoria
recursivamente de suas sub-arvores esquerda e
direita, e depois da propria raiz (pos-ordem).
*/
template <class Chave, class Item>
void treap<Chave,Item>::Node_Destructor_R( Node* ptr )
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
void treap<Chave,Item>::debug_print_all(){
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
void treap<Chave,Item>::debug_print_R( Node* ptr ){
    if( ptr->esq != nullptr )
	debug_print_R( ptr->esq );
    cout << ptr->prior << "\t";
    cout << ptr->P.getCh() << "\t";
    cout << *(ptr->P.getVal()) << endl;
    if( ptr->dir != nullptr )
	debug_print_R( ptr->dir );
}

/*
insere():
Recebe um par Chave-Item e o insere na treap; se
a chave já está na treap, apenas insere o novo valor
naquela posicao. Se a chave ainda nao esta na treap,
cria uma nova celula na posicao correta da treap
para armazenar os novos valores.
*/
template <class Chave, class Item>
void treap<Chave,Item>::insere( Chave chave, Item valor )
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
	Raiz->prior = rand();
	Raiz->esq = nullptr;
	Raiz->dir = nullptr;
	Raiz->pai = nullptr;
	return;
    }
    Node *ptr, *aux;
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
	aux->prior = rand();
	aux->esq = nullptr;
	aux->dir = nullptr;
	aux->pai = ptr;
	auxi = strcmp( chave , ptr->P.getCh() );
	if( auxi < 0)
	    ptr->esq = aux;
	else
	    ptr->dir = aux;
	while( aux->pai != nullptr &&
	    (aux->prior > aux->pai->prior) )
	    sobe_heap( aux );
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
bool treap<Chave,Item>::achouBin( Chave chave , Tree *ret )
{
    int aux;
    Node* ptr = Raiz;
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
Item presente na treap.
*/
template <class Chave, class Item>
Item treap<Chave,Item>::devolve( Chave chave )
{
    Node* ptr;
    if( achouBin( chave , &ptr ) )
	return ptr->P.getVal();
    else
	return ITEM_VAZIO;
}

/*
remove():
Remove uma celula com determinada chave; se
a chave nao esta na treap, nao faz nada.
*/
template <class Chave, class Item>
void treap<Chave,Item>::remove( Chave chave ){
    Node *ptr, *aux;
    if( achouBin( chave , &ptr ) ){
	aux = Merge( ptr->esq , ptr->dir );
	if( aux != nullptr )
	    aux->pai = ptr->pai;
	if( ptr->pai == nullptr )
	    Raiz = aux;
	else{/* nao e' raiz */
	    if( ptr->pai->esq == ptr )
		/* filho esquerdo */
		ptr->pai->esq = aux;
	    else/* filho direito*/
		ptr->pai->dir = aux;

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
int treap<Chave,Item>::rank( Chave chave ){
    if( Raiz == nullptr )
	return 0;
    int aux, count = 0;
    Node* ptr = Raiz;
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
int treap<Chave,Item>::conta_R( Tree r ){
    if( r == nullptr )
	return 0;
    return conta_R( r->esq) +1 +conta_R( r->dir );
}

/*
seleciona():
Devolve a chave de rank k. Se ha menos que
k+1 elementos na lista, devolve CHAVE_VAZIO.
*/
template <class Chave, class Item>
Chave treap<Chave,Item>::seleciona( int k ){
    if( Raiz == nullptr )
	return CHAVE_VAZIO;
    int aux, count = 0;
    Node* ptr = Raiz;
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

/*
sobe_heap():
Recebe um apontador para No e se ele e' filho
esquerdo, rotaciona a sub-arvore que tem como
raiz seu pai para a direita; caso contrario
rotaciona para a esquerda.
*/
template <class Chave, class Item>
void treap<Chave,Item>::sobe_heap( Node* ptr ){
    if( ptr->pai->esq == ptr )/*e' filho esquerdo*/
	rotate_right( ptr->pai );
    else if( ptr->pai->dir == ptr )/*e' filho direito*/
	rotate_left( ptr->pai );
}

/*
rotate_right():
Recebe um apontador para no e rotaciona para
a direita sub-arvore que tem como raiz esse no.
*/
template <class Chave, class Item>
void treap<Chave,Item>::rotate_right( Node* ptr ){
    Node* esq_backup = ptr->esq;
    ptr->esq->pai = ptr->pai;
    if( ptr->pai != nullptr ){
	if( ptr->pai->dir == ptr )
	    ptr->pai->dir = ptr->esq;
	else
	    ptr->pai->esq = ptr->esq;
    }
    else
	Raiz = ptr->esq;
    ptr->esq = ptr->esq->dir;
    if( ptr->esq != nullptr )
	ptr->esq->pai = ptr;
    esq_backup->dir = ptr;
    ptr->pai = esq_backup;
}

/*
rotate_left():
Recebe um apontador para no e rotaciona para
a esquerda sub-arvore que tem como raiz esse no.
*/
template <class Chave, class Item>
void treap<Chave,Item>::rotate_left( Node* ptr ){
    Node* dir_backup = ptr->dir;
    ptr->dir->pai = ptr->pai;
    if( ptr->pai != nullptr ){
	if( ptr->pai->dir == ptr )
	    ptr->pai->dir = ptr->dir;
	else
	    ptr->pai->esq = ptr->dir;
    }
    else
	Raiz = ptr->dir;
    ptr->dir = ptr->dir->esq;
    if( ptr->dir != nullptr )
	ptr->dir->pai = ptr;
    dir_backup->esq = ptr;
    ptr->pai = dir_backup;
}

/*
Merge():
Recebe duas treaps T1 e T2, com a condicao que
todos os elementos de T1 tenham chave menor
do que todos os elementos de T2; devolve uma
treap com a juncao de todos os elementos.
Baseada na funcao de mesmo nome em:
https://cp-algorithms.com/data_structures/treap.html
(13/06/2020)
*/
template <class Chave, class Item>
typename treap<Chave,Item>::Tree
treap<Chave,Item>::Merge( Tree T1 , Tree T2 )
{
    Tree ptr;
    if( T1 == nullptr ){
	if( T2 == nullptr)
	    return nullptr;
	else/* T2 != nullptr */
	    return T2;
    }
    else{/* T1 != nullptr */
	if( T2 == nullptr )
	    return T1;
    }
    if( T1->prior == T2->prior )
	T1->prior--;
    if( T1->prior > T2->prior ){
	ptr = T1;
	ptr->dir = Merge( ptr->dir , T2 );
	if( ptr->dir != nullptr )
	    ptr->dir->pai = ptr;
    }
    else{/* T1->prior < T2->prior */
	ptr = T2;
	ptr->esq = Merge( T1 , ptr->esq );
	if( ptr->esq != nullptr )
	    ptr->esq->pai = ptr;
    }
    return ptr;
}

#endif
