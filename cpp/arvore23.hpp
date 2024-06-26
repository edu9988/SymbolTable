/**************************************************************/
/* José Eduardo R. M. Peres y Peres       No. USP:8945332     */
/* Exercicio-Programa EP1                                     */
/* Curso MAC323 - Prof. Carlos Eduardo Ferreira	              */
/*                                                            */
/* Compilador: GNU GCC Compiler                               */
/**************************************************************/

/*
Arquivo arvore23.hpp
Contem interface e implementacao de arvore23.
*/

/*
remove() baseada em texto de autoria de Prof. Lyn Turbak,
Wellesley College;
https://www.cs.princeton.edu/~dpw/courses/cos326-12/ass/2-3-trees.pdf
(13/06/2020)
*/

#ifndef ARVORE23_HPP
#define ARVORE23_HPP

using namespace std;
#include <iostream> /* cout */
#include <cctype> /* tolower */
#include <fstream> /* ifstream, open, get */
#include <cstring> /* strcmp, strlen */
#include "Par.hpp"

/*
arvore23:
Definicao da classe arvore23.
*/
template <class Chave, class Item>
class arvore23
{
    private:
	typedef struct node{
	    Par<Chave,Item> P1;
	    struct node *esq;
	    struct node *dir;
	    struct node *pai;
	    bool tresNo;
	    Par<Chave,Item> P2;
	    struct node *meio;
	    bool quatroNo;
	    Par<Chave,Item> P3;
	    struct node *meio2;
	}Node;
	typedef Node* Tree;
	Tree Raiz;

	char dummy_ch = '\0';
	Chave CHAVE_VAZIO = &dummy_ch;
	int dummy_val = 0;
	Item ITEM_VAZIO = &dummy_val;

	/*funcoes auxiliares*/
	void sobe_do_meio( Node* , int );
	void copy_Par_3to1( Node* , Node* );
	void copy_Par_2to3( Node* );
	void copy_Par_1to1( Node* , Node* );
	void copy_Par_2to2( Node* , Node* );
	void copy_Par_1to2( Node* , Node* );
	void copy_Par_2to1( Node* , Node* );
	void copia_VAZIO( Node* , int );
	void split_root();
	void split_4No( Node* );
	void add_Par( Node* , Chave , Item );
	Node *novo_2No();
	int conta_R( Tree );
	void Node_Destructor_R( Node* );
	int achouBin( Chave , Tree* );
	void debug_print_R( Node* );
	void insere_Aux( Node* , Chave , Item );

    public:
	arvore23( string );
	~arvore23();

	void insere( Chave , Item );
	Item devolve( Chave );
	void remove( Chave );
	int rank( Chave );
	Chave seleciona( int );
};

/*
arvore23():
Construtor; recebe o nome de um arquivo como
argumento; le do arquivo dado, inserindo
as palavras na arvore23.
*/
template <class Chave, class Item>
arvore23<Chave,Item>::arvore23( string file_name ):
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
}

/*
~arvore23():
Destrutor; libera a memória alocada para a arvore23.
*/
template <class Chave, class Item>
arvore23<Chave,Item>::~arvore23()
{
    Node_Destructor_R( Raiz );
}

/*
Node_Destructor_R():
Recebe como entrada uma arvore23; libera a memoria
recursivamente de suas sub-arvores esquerda e
direita, e depois da propria raiz (pos-ordem).
*/
template <class Chave, class Item>
void arvore23<Chave,Item>::Node_Destructor_R( Node* ptr )
{
    if( ptr == nullptr )
	return;
    Node_Destructor_R( ptr->esq );
    delete [] ptr->P1.getCh();
    if( ptr->tresNo ){
	Node_Destructor_R( ptr->meio );
	delete [] ptr->P2.getCh();
    }
    Node_Destructor_R( ptr->dir );
    delete ptr;
}

/*
debug_print_R():
Funcao para auxiliar na depuracao; imprime
recursivamente o conteudo das sub-arvores
e do proprio No, na sequencia in-ordem.
*/
template <class Chave, class Item>
void arvore23<Chave,Item>::debug_print_R( Node* ptr ){
    if( ptr == nullptr )
	return;
    debug_print_R( ptr->esq );
    cout << ptr->P1.getCh() << "\t";
    cout << *(ptr->P1.getVal()) << endl;
    if( ptr->tresNo ){
	debug_print_R( ptr->meio );
	cout << ptr->P2.getCh() << "\t";
	cout << *(ptr->P2.getVal()) << endl;
    }
    debug_print_R( ptr->dir );
}

/*
insere():
Recebe um par Chave-Item e o insere na arvore23; se
a chave já está na arvore23, apenas insere o novo valor
naquela posicao. Se a chave ainda nao esta na arvore23,
cria uma nova celula na posicao correta da arvore23
para armazenar os novos valores.
*/
template <class Chave, class Item>
void arvore23<Chave,Item>::insere( Chave chave, Item valor )
{
    int auxi;
    Chave ch_aux;
    if( Raiz == nullptr ){
	auxi = strlen(chave);
	ch_aux = new char[auxi+1];
	for( int j=0 ; j<=auxi ; j++ )
	    ch_aux[j] = chave[j];
	Raiz = novo_2No();
	Raiz->P1.setCh( ch_aux );
	Raiz->P1.setVal( valor );
	return;
    }
    Node *ptr;
    auxi = achouBin( chave , &ptr );
    if( auxi == 1 )
	ptr->P1.setVal( valor );
    else if( auxi == 2 )
	ptr->P2.setVal( valor );
    else{/* nao achou */
	auxi = strlen(chave);
	ch_aux = new char[auxi+1];
	for( int j=0 ; j<=auxi ; j++ )
	    ch_aux[j] = chave[j];
	insere_Aux( ptr , ch_aux , valor );
    }
}

/*
novo_2No():
Cria uma celula que representa um no;
inicializa as variaveis apropriadas
e devolve o endereco do no.
*/
template <class Chave, class Item>
typename arvore23<Chave,Item>::Node*
arvore23<Chave,Item>::novo_2No(){
    Node *ptr = new Node;
    ptr->esq = nullptr;
    ptr->dir = nullptr;
    ptr->pai = nullptr;
    ptr->tresNo = false;
    ptr->P2.setCh( CHAVE_VAZIO );
    ptr->P2.setVal( ITEM_VAZIO );
    ptr->meio = nullptr;
    ptr->quatroNo = false;
    ptr->P3.setCh( CHAVE_VAZIO );
    ptr->P3.setVal( ITEM_VAZIO );
    ptr->meio2 = nullptr;
    return ptr;
}

/*
insere_Aux():
Funcao auxiliar, criada para melhor organizacao da
funcao insere(); Recebe um apontador para no e um par
Chave-Valor; adiciona o par na celula recebida, e
caso a celula resultante seja 4No, a "explode" da
maneira apropriada, iterativamente, enquanto for
preciso.
*/
template <class Chave, class Item>
void arvore23<Chave,Item>::insere_Aux( Node *ptr , Chave chave, Item valor )
{
    add_Par( ptr , chave , valor );
    while( ptr->quatroNo ){
	if( ptr->pai == nullptr ){/* nao tem pai */
	    split_root();
	    return;
	}
	else/* tem pai */
	    split_4No( ptr );
	ptr = ptr->pai;
    }
}

/*
split_4No():
Funcao auxiliar, criada para melhor organizacao
da funcao insere(); atua quando um no' vira 4No,
necessitando "explodir" e mandar o elemento do
meio para cima, acertando todos os ponteiros
envolvidos.
*/
template <class Chave, class Item>
void arvore23<Chave,Item>::split_4No( Node *ptr ){
    Node *aux;
    aux = novo_2No();
    copy_Par_3to1( ptr , aux );
    aux->dir = ptr->dir;
    aux->esq = ptr->meio2;
    aux->pai = ptr->pai;
    if( aux->dir != nullptr )
	aux->dir->pai = aux;
    if( aux->esq != nullptr )
	aux->esq->pai = aux;
    copia_VAZIO( ptr , 3 );
    ptr->dir = ptr->meio;
    ptr->meio2 = nullptr;
    ptr->meio = nullptr;
    ptr->quatroNo = false;
    ptr->tresNo = false;
    if( ptr->pai->tresNo ){
	ptr->pai->quatroNo = true;
	if( ptr->pai->dir == ptr ){/*filho direito*/
	    sobe_do_meio( ptr , 3 );
	    ptr->pai->dir = aux;
	    ptr->pai->meio2 = ptr;
	}
	else if( ptr->pai->esq == ptr ){/*filho esquerdo*/
	    sobe_do_meio( ptr , 1 );
	    ptr->pai->meio2 = ptr->pai->meio;
	    ptr->pai->meio = aux;
	}
	else{/*filho do meio*/
	    sobe_do_meio( ptr , 2 );
	    ptr->pai->meio2 = aux;
	}
    }
    else{/* ptr->pai->tresNo == false */
	ptr->pai->tresNo = true;
	if( ptr->pai->dir == ptr ){/*filho direito*/
	    sobe_do_meio( ptr , 2 );
	    ptr->pai->dir = aux;
	    ptr->pai->meio = ptr;
	}
	else{/*filho esquerdo*/
	    sobe_do_meio( ptr , 1 );
	    ptr->pai->meio = aux;
	}
    }
    copia_VAZIO( ptr , 2 );
}

/*
split_root():
Funcao auxiliar, criada para melhor organizacao
da funcao insere(); atua quando a Raiz vira 4No,
necessitando "explodi-la" e manter apenas o
elemento do meio na Raiz, acertando todos os
ponteiros envolvidos.
*/
template <class Chave, class Item>
void arvore23<Chave,Item>::split_root(){
    Node *ptr, *aux;
    ptr = novo_2No();
    copy_Par_3to1( Raiz , ptr );
    copia_VAZIO( Raiz , 3 );
    ptr->esq = Raiz->meio2;
    Raiz->meio2 = nullptr;
    Raiz->quatroNo = false;
    ptr->dir = Raiz->dir;
    Raiz->dir = Raiz->meio;
    Raiz->meio = nullptr;
    if( ptr->esq != nullptr )
	ptr->esq->pai = ptr;
    if( ptr->dir != nullptr )
	ptr->dir->pai = ptr;
    aux = novo_2No();
    copy_Par_2to1( Raiz , aux );
    copia_VAZIO( Raiz , 2 );
    Raiz->tresNo = false;
    aux->esq = Raiz;
    Raiz->pai = aux;
    aux->dir = ptr;
    ptr->pai = aux;
    Raiz = aux;
}

/*
sobe_do_meio():
Funcao auxiliar, criada para melhor organizacao
da funcao insere(); atua quando um 4No e'
explodido e seu elemento do meio inserido na
celula pai.
*/
template <class Chave, class Item>
void arvore23<Chave,Item>::sobe_do_meio( Node *ptr , int i )
{
    Node *aux = ptr->pai;
    if( i == 1 ){
	copy_Par_2to3( aux );
	copy_Par_1to2( aux , aux );
	aux->P1.setCh( ptr->P2.getCh() );
	aux->P1.setVal( ptr->P2.getVal() );
    }
    else if( i == 2 ){
	copy_Par_2to3( aux );
	aux->P2.setCh( ptr->P2.getCh() );
	aux->P2.setVal( ptr->P2.getVal() );
    }
    else{/* i == 3 */
	aux->P3.setCh( ptr->P2.getCh() );
	aux->P3.setVal( ptr->P2.getVal() );
    }
}


/*
add_Par():
Funcao auxiliar, insere um Par Chave-Valor numa
celula que representa um No; Apenas insere o par,
deslocando demais pares que estejam no caminho, e
acertando os ponteiros.
*/
template <class Chave, class Item>
void arvore23<Chave,Item>::add_Par( Node* ptr , Chave chave , Item valor )
{
    int aux;
    if( ptr->tresNo ){
	ptr->quatroNo = true;
	aux = strcmp( chave , ptr->P1.getCh() );
	if( aux < 0 ){
	    copy_Par_2to3( ptr );
	    copy_Par_1to2( ptr , ptr );
	    ptr->meio2 = ptr->meio;
	    ptr->meio = ptr->esq;
	    ptr->P1.setCh( chave );
	    ptr->P1.setVal( valor );
	}
	else{/* aux > 0 */
	    aux = strcmp( chave , ptr->P2.getCh() );
	    if( aux < 0 ){
		copy_Par_2to3( ptr );
		ptr->P2.setCh( chave );
		ptr->P2.setVal( valor );
	    }
	    else{/* aux > 0 */
		ptr->P3.setCh( chave );
		ptr->P3.setVal( valor );
	    }
	}
    }
    else{/* ptr->tresNo == false */
	ptr->tresNo = true;
	aux = strcmp( chave , ptr->P1.getCh() );
	if( aux < 0 ){
	    copy_Par_1to2( ptr , ptr );
	    ptr->P1.setCh( chave );
	    ptr->P1.setVal( valor );
	}
	else{/* aux > 0 */
	    ptr->P2.setCh( chave );
	    ptr->P2.setVal( valor );
	}
    }
}

/*
copia_VAZIO():
Insere a chave CHAVE_VAZIO e item ITEM_VAZIO
no Di-esimo par do Node apontado por D.
*/
template <class Chave, class Item>
void arvore23<Chave,Item>::copia_VAZIO( Node* D , int Di )
{
	if( Di == 1 ){
	    D->P1.setCh( CHAVE_VAZIO );
	    D->P1.setVal( ITEM_VAZIO );
	}
	if( Di == 2 ){
	    D->P2.setCh( CHAVE_VAZIO );
	    D->P2.setVal( ITEM_VAZIO );
	}
	if( Di == 3 ){
	    D->P3.setCh( CHAVE_VAZIO );
	    D->P3.setVal( ITEM_VAZIO );
	}
}

/*
copy_Par_2to1():
Copia o Par 2 do No' O(origem) para o
par 1 do No' D(destino);
*/
template <class Chave, class Item>
void arvore23<Chave,Item>::copy_Par_2to1( Node* O , Node* D )
{
    if( O == nullptr || D == nullptr )
	return;
    D->P1.setCh( O->P2.getCh() );
    D->P1.setVal( O->P2.getVal() );
}

/*
copy_Par_1to2():
Copia o Par 1 do No' O(origem) para o
par 2 do No' D(destino);
*/
template <class Chave, class Item>
void arvore23<Chave,Item>::copy_Par_1to2( Node* O , Node* D )
{
    if( O == nullptr || D == nullptr )
	return;
    D->P2.setCh( O->P1.getCh() );
    D->P2.setVal( O->P1.getVal() );
}

/*
copy_Par_2to2():
Copia o Par 2 do No' O(origem) para o
par 2 do No' D(destino);
*/
template <class Chave, class Item>
void arvore23<Chave,Item>::copy_Par_2to2( Node* O , Node* D )
{
    if( O == nullptr || D == nullptr )
	return;
    D->P2.setCh( O->P2.getCh() );
    D->P2.setVal( O->P2.getVal() );
}

/*
copy_Par_1to1():
Copia o Par 1 do No' O(origem) para o
par 1 do No' D(destino);
*/
template <class Chave, class Item>
void arvore23<Chave,Item>::copy_Par_1to1( Node* O , Node* D )
{
    if( O == nullptr || D == nullptr )
	return;
    D->P1.setCh( O->P1.getCh() );
    D->P1.setVal( O->P1.getVal() );
}

/*
copy_Par_2to3():
Copia o Par 2 do No' O(origem) para o
par 3 do No' D(destino);
*/
template <class Chave, class Item>
void arvore23<Chave,Item>::copy_Par_2to3( Node* ptr )
{
    if( ptr == nullptr )
	return;
    ptr->P3.setCh( ptr->P2.getCh() );
    ptr->P3.setVal( ptr->P2.getVal() );
}

/*
copy_Par_3to1():
Copia o Par 3 do No' O(origem) para o
par 1 do No' D(destino);
*/
template <class Chave, class Item>
void arvore23<Chave,Item>::copy_Par_3to1( Node* O , Node* D )
{
    if( O == nullptr || D == nullptr )
	return;
    D->P1.setCh( O->P3.getCh() );
    D->P1.setVal( O->P3.getVal() );
}

/*
achouBin():
Funcao auxiliar, procura um No com determinada
chave e devolve verdadeiro se for encontrada, falso
caso contrario; tambem devolve no ponteiro ret
endereco do No encontrado(caso tenha sido), ou
endereco de onde deve ser inserido(se nao foi
encontrado).
*/
template <class Chave, class Item>
int arvore23<Chave,Item>::achouBin( Chave chave , Tree *ret )
{
    int aux;
    Node* ptr = Raiz;
    *ret = nullptr;
    if( ptr == nullptr )
	return 0;
    while( ptr != nullptr ){
	*ret = ptr;
	aux = strcmp( chave , ptr->P1.getCh() );
	if( aux == 0 )
	    return 1;
	else if( aux < 0 )
	    ptr = ptr->esq;
	else{/* aux > 0 */
	    if( ptr->tresNo ){
		aux = strcmp( chave , ptr->P2.getCh() );
		if( aux == 0 )
		    return 2;
		else if( aux < 0 )
		    ptr = ptr->meio;
		else/* aux > 0 */
		    ptr = ptr->dir;
	    }
	    else/* ptr->tresNo == false */
		ptr = ptr->dir;
	}
    }
    return 0;
}

/*
devolve():
Recebe uma Chave e retorna o valor de seu
Item presente na arvore23.
*/
template <class Chave, class Item>
Item arvore23<Chave,Item>::devolve( Chave chave )
{
    int achou;
    Node* ptr;
    achou = achouBin( chave , &ptr );
    if( achou == 1 )
	return ptr->P1.getVal();
    else if( achou == 2 )
	return ptr->P2.getVal();
    else/* nao achou */
	return ITEM_VAZIO;
}

/*
remove():
Remove uma celula com determinada chave; se
a chave nao esta na arvore23, nao faz nada.
Baseada em texto de autoria de Prof. Lyn Turbak,
Wellesley College, encontrado em
https://www.cs.princeton.edu/~dpw/courses/cos326-12/ass/2-3-trees.pdf
(13/06/2020)
*/
template <class Chave, class Item>
void arvore23<Chave,Item>::remove( Chave chave ){
    int aux_int;
    Node *ptr, *aux;
    aux_int = achouBin( chave , &ptr );
    if( aux_int == 0 ){ /*nao achou*/
	cout << "Nao ";
	return;
    }
    if( ptr->esq != nullptr ){/* not terminal-node */
	if( aux_int == 1 ){
	    delete [] ptr->P1.getCh();
	    aux = ptr->esq;
	    while( aux->dir != nullptr )
		aux = aux->dir;
	    if( aux->tresNo ){
		copy_Par_2to1( aux , ptr );
		copia_VAZIO( aux , 2 );
		aux->tresNo = false;
		return;
	    }
	    else/* aux->tresNo == false */
		copy_Par_1to1( aux , ptr );
	}
	else{/* aux_int == 2 */
	    delete [] ptr->P2.getCh();
	    aux = ptr->meio;
	    while( aux->dir != nullptr )
		aux = aux->dir;
	    if( aux->tresNo ){
		copy_Par_2to2( aux , ptr );
		copia_VAZIO( aux , 2 );
		aux->tresNo = false;
		return;
	    }
	    else/* aux->tresNo == false */
		copy_Par_1to2( aux , ptr );
	}
	copia_VAZIO( aux , 1 );
	ptr = aux;
    }
    else{/* terminal-node */
	if( ptr->tresNo ){
	    ptr->tresNo = false;
	    if( aux_int == 1){
		delete [] ptr->P1.getCh();
		copy_Par_2to1( ptr , ptr );
	    }
	    else/* aux_int == 2 */
		delete [] ptr->P2.getCh();
	    copia_VAZIO( ptr , 2 );
	    return;
	}
	else{/* ptr->tresNo == false */
	    delete [] ptr->P1.getCh();
	    copia_VAZIO( ptr , 1 );
	}
    }
    while( true ){/* Upward phase */
	if( ptr->pai == nullptr ){
	    Raiz = ptr->esq;
	    if( ptr->esq != nullptr )
		ptr->esq->pai = nullptr;
	    delete ptr;
	    return;
	}
	else{/* ptr->pai != nullptr */
	    aux = ptr->pai;
	    if( aux->tresNo ){/* pai tresNo */
		if( aux->esq == ptr ){/* filho esquerdo */
		    if( aux->meio->tresNo ){/* irmao 3no */
			copy_Par_1to1( aux , ptr );
			copy_Par_1to1( aux->meio , aux );
			aux = aux->meio;
			copy_Par_2to1( aux , aux );
			copia_VAZIO( aux , 2 );
			aux->tresNo = false;
			ptr->dir = aux->esq;
			if( ptr->dir != nullptr )
			    ptr->dir->pai = ptr;
			aux->esq = aux->meio;
		    }
		    else{/* irmao 2no */
			copy_Par_1to2( aux->meio , aux->meio );
			copy_Par_1to1( aux , aux->meio );
			copy_Par_2to1( aux , aux );
			copia_VAZIO( aux , 2 );
			aux->esq = aux->meio;
			aux->meio = nullptr;
			aux->tresNo = false;
			aux = aux->esq;
			aux->tresNo = true;
			aux->meio = aux->esq;
			aux->esq = ptr->esq;
			if( ptr->esq != nullptr )
			    aux->esq->pai = aux;
			delete ptr;
		    }
		}
		else if( aux->meio == ptr ){/* filho do meio */
		    if( aux->dir->tresNo ){/* irmao 3no */
			copy_Par_2to1( aux , ptr );
			copy_Par_1to2( aux->dir , aux );
			aux = aux->dir;
			copy_Par_2to1( aux , aux );
			copia_VAZIO( aux , 2 );
			aux->tresNo = false;
			ptr->dir = aux->esq;
			if( ptr->dir != nullptr )
			    ptr->dir->pai = ptr;
			aux->esq = aux->meio;
		    }
		    else{/* irmao 2no */
			copy_Par_1to2( aux->dir , aux->dir );
			copy_Par_2to1( aux , aux->dir );
			copia_VAZIO( aux , 2 );
			aux->meio = nullptr;
			aux->tresNo = false;
			aux = aux->dir;
			aux->tresNo = true;
			aux->meio = aux->esq;
			aux->esq = ptr->esq;
			if( aux->esq != nullptr )
			    aux->esq->pai = aux;
			delete ptr;
		    }
		}
		else{/* filho direito */
		    if( aux->meio->tresNo ){/* irmao 3no */
			copy_Par_2to1( aux , ptr );
			copy_Par_2to2( aux->meio , aux );
			aux = aux->meio;
			copia_VAZIO( aux , 2 );
			aux->tresNo = false;
			ptr->dir = ptr->esq;
			ptr->esq = aux->dir;
			if( ptr->esq != nullptr )
			    ptr->esq->pai = ptr;
			aux->dir = aux->meio;
			aux->meio = nullptr;
		    }
		    else{/* irmao 2no */
			copy_Par_2to2( aux , aux->meio );
			copia_VAZIO( aux , 2 );
			aux->dir = aux->meio;
			aux->meio = nullptr;
			aux->tresNo = false;
			aux = aux->dir;
			aux->tresNo = true;
			aux->meio = aux->dir;
			aux->dir = ptr->esq;
			if( aux->dir != nullptr )
			    aux->dir->pai = aux;
			delete ptr;
		    }
		}
		return;
	    }
	    else{/* pai doisNo */
		if( aux->esq == ptr ){/* filho esquerdo */
		    if( aux->dir->tresNo ){/* irmao 3no */
			copy_Par_1to1( aux , ptr );
			copy_Par_1to1( aux->dir , aux );
			copy_Par_2to1( aux->dir , aux->dir );
			aux = aux->dir;
			ptr->dir = aux->esq;
			if( ptr->dir != nullptr )
			    ptr->dir->pai = ptr;
			aux->esq = aux->meio;
			aux->tresNo = false;
			return;
		    }
		    else{/* irmao 2no */
			copy_Par_1to2( aux->dir , aux->dir );
			copy_Par_1to1( aux , aux->dir );
			copia_VAZIO( aux , 1 );
			aux->esq = aux->dir;
			aux->dir = nullptr;
			aux = aux->esq;
			aux->tresNo = true;
			aux->meio = aux->esq;
			aux->esq = ptr->esq;
			if( aux->esq != nullptr )
			    aux->esq->pai = aux;
			delete ptr;
			ptr = aux->pai;
		    }
		}
		else{/* filho direito */
		    if( aux->esq->tresNo ){/* irmao 3no */
			copy_Par_1to1( aux , ptr );
			copy_Par_2to1( aux->esq , aux );
			aux = aux->esq;
			ptr->dir = ptr->esq;
			ptr->esq = aux->dir;;
			if( ptr->esq != nullptr )
			    ptr->esq->pai = ptr;
			aux->dir = aux->meio;
			aux->tresNo = false;
			return;
		    }
		    else{/* irmao 2no */
			copy_Par_1to2( aux , aux->esq );
			copia_VAZIO( aux , 1 );
			aux->dir = nullptr;
			aux = aux->esq;
			aux->tresNo = true;
			aux->meio = aux->dir;
			aux->dir = ptr->esq;
			if( aux->dir != nullptr )
			    aux->dir->pai = aux;
			delete ptr;
			ptr = aux->pai;
		    }
		}
	    }
	}
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
int arvore23<Chave,Item>::rank( Chave chave ){
    if( Raiz == nullptr )
	return 0;
    int aux, count = 0;
    Node* ptr = Raiz;
    while( ptr != nullptr ){
	if( ptr->tresNo ){
	    aux = strcmp( chave , ptr->P1.getCh() );
	    if( aux == 0 )
		return count + conta_R( ptr->esq );
	    else if( aux < 0 )
		ptr = ptr->esq;
	    else{/* aux > 0 */
		count += 1 + conta_R( ptr->esq );
		aux = strcmp( chave , ptr->P2.getCh() );
		if( aux == 0 )
		    return count + conta_R( ptr->meio );
		else if( aux < 0 )
		    ptr = ptr->meio;
		else{/* aux > 0 */
		    count += 1 + conta_R( ptr->meio );
		    ptr = ptr->dir;
		}
	    }
	}
	else{/* ptr->tresNo == false */
	    aux = strcmp( chave , ptr->P1.getCh() );
	    if( aux == 0 )
		return count + conta_R( ptr->esq );
	    else if( aux < 0 )
		ptr = ptr->esq;
	    else{/* aux > 0 */
		count += 1 + conta_R( ptr->esq );
		ptr = ptr->dir;
	    }
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
int arvore23<Chave,Item>::conta_R( Tree r ){
    if( r == nullptr )
	return 0;
    if( r->tresNo ){
	return conta_R( r->esq ) + conta_R( r->meio )
	    +conta_R( r->dir ) +2;
    }
    else/* tresNo == false */
	return conta_R( r->esq ) +1 +conta_R( r->dir );
}

/*
seleciona():
Devolve a chave de rank k. Se ha menos que
k+1 elementos na lista, devolve CHAVE_VAZIO.
*/
template <class Chave, class Item>
Chave arvore23<Chave,Item>::seleciona( int k ){
    if( Raiz == nullptr )
	return CHAVE_VAZIO;
    int aux, count = 0;
    Node* ptr = Raiz;
    while( ptr != nullptr ){
	if( ptr->tresNo ){
	    aux = conta_R( ptr->esq );
	    if( count + aux == k )
		return ptr->P1.getCh();
	    else if( count + aux > k )
		ptr = ptr->esq;
	    else{/* count + aux < k */
		count += aux + 1;
		aux = conta_R( ptr->meio );
		if( count + aux == k )
		    return ptr->P2.getCh();
		else if( count + aux > k )
		    ptr = ptr->meio;
		else{/* count + aux < k */
		    count += aux + 1;
		    ptr = ptr->dir;
		}
	    }
	}
	else{/* ptr->tresNo == false */
	    aux = conta_R( ptr->esq );
	    if( count + aux == k )
		return ptr->P1.getCh();
	    else if( count + aux > k )
		ptr = ptr->esq;
	    else{/* count + aux < 0 */
		count += 1 + aux;
		ptr = ptr->dir;
	    }
	}
    }
    return CHAVE_VAZIO;
}

#endif
