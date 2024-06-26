/**************************************************************/
/* José Eduardo R. M. Peres y Peres       No. USP:8945332     */
/* Exercicio-Programa EP1                                     */
/* Curso MAC323 - Prof. Carlos Eduardo Ferreira	              */
/*                                                            */
/* Compilador: GNU GCC Compiler                               */
/**************************************************************/

/*
Arquivo arvoreRN.hpp
Contem interface e implementacao de arvoreRN.
*/

#ifndef ARVORERN_HPP
#define ARVORERN_HPP

using namespace std;
#include <iostream> /* cout */
#include <cctype> /* tolower */
#include <fstream> /* ifstream, open, get */
#include <cstring> /* strcmp, strlen */
#include "Par.hpp"

/*
arvoreRN:
Definicao da classe arvoreRN.
*/
template <class Chave, class Item>
class arvoreRN
{
    private:
	typedef struct node{
	    Par<Chave,Item> P;
	    bool red;
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
	int conta_R( Tree );
	void Node_Destructor_R( Node* );
	bool achouBin( Chave , Tree* );
	void debug_print_R( Node* );
	int debug_count_R( Node* );
	Tree Merge( Tree , Tree );
	void copy_From_To( Tree , Tree );
	Tree Menor_Folha_Dir( Tree );
	Tree Maior_Folha_Esq( Tree );
	bool is_left_child( Tree );
	bool has_red_child( Tree );
	Tree get_Brother( Tree , bool );
	Tree get_Closest_Nephew( Tree , bool );
    public:
	arvoreRN( string );
	~arvoreRN();

	void insere( Chave , Item );
	Item devolve( Chave );
	void remove( Chave );
	int rank( Chave );
	Chave seleciona( int );
};

/*
arvoreRN():
Construtor; recebe o nome de um arquivo como
argumento; le do arquivo dado, inserindo
as palavras na arvoreRN.
*/
template <class Chave, class Item>
arvoreRN<Chave,Item>::arvoreRN( string file_name ):
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
~arvoreRN():
Destrutor; libera a memória alocada para a arvoreRN.
*/
template <class Chave, class Item>
arvoreRN<Chave,Item>::~arvoreRN()
{
    Node_Destructor_R( Raiz );
}

/*
Node_Destructor_R():
Recebe como entrada uma arvoreRN; libera a memoria
recursivamente de suas sub-arvores esquerda e
direita, e depois da propria raiz (pos-ordem).
*/
template <class Chave, class Item>
void arvoreRN<Chave,Item>::Node_Destructor_R( Node* ptr )
{
    if( ptr->esq != nullptr)
	Node_Destructor_R( ptr->esq );
    if( ptr->dir != nullptr)
	Node_Destructor_R( ptr->dir );
    delete [] ptr->P.getCh();
    delete ptr;
}

/*
debug_print_R():
Funcao para auxiliar na depuracao; imprime
recursivamente o conteudo das sub-arvores
e do proprio No, na sequencia in-ordem.
*/
template <class Chave, class Item>
void arvoreRN<Chave,Item>::debug_print_R( Node* ptr ){
    if( ptr->esq != nullptr )
	debug_print_R( ptr->esq );
    cout << "\t" << ptr->P.getCh() << " ";
    cout << *(ptr->P.getVal()) << " ";
    if( ptr->red )
	cout << "Red\n";
    else
	cout << "Black\n";
    if( ptr->dir != nullptr )
	debug_print_R( ptr->dir );
}

/*
debug_count_R():
Funcao para auxiliar na depuracao;
Conta quantos elementos ha na arvore.
*/
template <class Chave, class Item>
int arvoreRN<Chave,Item>::debug_count_R( Node* ptr ){
    if( ptr == nullptr )
	return 0;
    else
	return debug_count_R( ptr->esq ) + 1 + debug_count_R( ptr->dir );
}

/*
insere():
Recebe um par Chave-Item e o insere na arvoreRN; se
a chave já está na arvoreRN, apenas insere o novo valor
naquela posicao. Se a chave ainda nao esta na arvoreRN,
cria uma nova celula na posicao correta da arvoreRN
para armazenar os novos valores.
*/
template <class Chave, class Item>
void arvoreRN<Chave,Item>::insere( Chave chave, Item valor )
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
	Raiz->pai = nullptr;
	Raiz->esq = nullptr;
	Raiz->dir = nullptr;
	Raiz->red = false; /* raiz preta! */
	return;
    }
    Node *avo, *tio, *ptr, *novo;
    bool pai_filho_dir, novo_filho_dir;
    if( achouBin( chave , &ptr ) )
	ptr->P.setVal( valor );
    else{/* nao achou */
	auxi = strlen(chave);
	Chave ch_aux = new char[auxi+1];
	for( int j=0 ; j<=auxi ; j++ )
	    ch_aux[j] = chave[j];
	novo = new Node;
	novo->P.setCh( ch_aux );
	novo->P.setVal( valor );
	novo->red = true;
	novo->esq = nullptr;
	novo->dir = nullptr;
	novo->pai = ptr;
	auxi = strcmp( chave , ptr->P.getCh() );
	if( auxi < 0){
	    ptr->esq = novo;
	    novo_filho_dir = false;
	}
	else{
	    ptr->dir = novo;
	    novo_filho_dir = true;
	}

	while( novo->red && ptr->red ){/* pai vermelho */
	    avo = ptr->pai;
	    if( !avo ){/* nao tem avo */
		ptr->red = false;
		return;
	    }
	    if( ptr == avo->esq ){
		tio = avo->dir;
		pai_filho_dir = false;
	    }
	    else{/* ptr == avo->dir */
		tio = avo->esq;
		pai_filho_dir = true;
	    }
	    if( !tio || !(tio->red) ){/* tio preto ou nao tem tio */
		if( pai_filho_dir && !novo_filho_dir ){
		    avo->dir = novo;
		    novo->pai = avo;
		    ptr->pai = novo;
		    ptr->esq = novo->dir;
		    if( ptr->esq )
			ptr->esq->pai = ptr;
		    novo->dir = ptr;
		    novo = ptr;
		    ptr = ptr->pai;
		    novo_filho_dir = true;
		}
		else if( !pai_filho_dir && novo_filho_dir ){
		    avo->esq = novo;
		    novo->pai = avo;
		    ptr->pai = novo;
		    ptr->dir = novo->esq;
		    if( ptr->dir )
			ptr->dir->pai = ptr;
		    novo->esq = ptr;
		    novo = ptr;
		    ptr = ptr->pai;
		    novo_filho_dir = false;
		}
		if( avo->pai ){
		    if( avo == avo->pai->esq ){
			avo->pai->esq = ptr;
		    }
		    else{/* avo == avo->pai->dir */
			avo->pai->dir = ptr;
		    }
		}
		else
		    Raiz = ptr;
		ptr->pai = avo->pai;
		avo->pai = ptr;
		if( pai_filho_dir && novo_filho_dir ){
		    if( ptr->esq )
			ptr->esq->pai = avo;
		    avo->dir = ptr->esq;
		    ptr->esq = avo;
		}
		else{/* !pai_filho_dir && !novo_filho_dir */
		    if( ptr->dir )
			ptr->dir->pai = avo;
		    avo->esq = ptr->dir;
		    ptr->dir = avo;
		}
		avo->red = true;
		ptr->red = false;
		return;
	    }
	    /* tio vermelho */
	    avo->red = true;
	    tio->red = false;
	    ptr->red = false;
	    ptr = avo->pai;
	    if( !ptr )
		return;
	    novo = avo;
	    if( novo == ptr->esq )
		novo_filho_dir = false;
	    else/* novo == ptr->dir */
		novo_filho_dir = true;
	}
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
bool arvoreRN<Chave,Item>::achouBin( Chave chave , Tree *ret )
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
Item presente na arvoreRN.
*/
template <class Chave, class Item>
Item arvoreRN<Chave,Item>::devolve( Chave chave )
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
a chave nao esta na arvoreRN, nao faz nada.
*/
template <class Chave, class Item>
void arvoreRN<Chave,Item>::remove( Chave chave ){
    Tree ptr, irmao, sobrinho_perto, aux;
    if( achouBin( chave , &ptr ) ){
	delete [] ptr->P.getCh();
	ptr = Menor_Folha_Dir( ptr );
	ptr = Maior_Folha_Esq( ptr );/* para o caso de so' ter filho esq */
	/* agora estou numa folha a ser removida */
	if( ptr->pai == nullptr ){/* nao tem pai */
	    Raiz = nullptr;
	    delete ptr;
	}
	else{/* tem pai */
	    if( ptr->red ){
		/* folha vermelha, caso 1 */
		if( is_left_child( ptr ) )
		    ptr->pai->esq = nullptr;
		else
		    ptr->pai->dir = nullptr;
		delete ptr;
	    }
	    else{/* folha preta, caso 2 */
		bool filho_esq, duplo_preto = false;
		do{
		    filho_esq = is_left_child( ptr );
		    irmao = get_Brother( ptr , filho_esq );
		    if( irmao->red ){
			/* irmao vermelho, caso 2.2 */
			irmao->red = false;
			ptr->pai->red = true;
			if( filho_esq )
			    rotate_left( ptr->pai );
			else
			    rotate_right( ptr->pai );
			irmao = get_Brother( ptr , filho_esq );
			/* agora irmao preto, pode ir para 2.1a, 2.3 ou 2.4 */
		    }
		    if( has_red_child( irmao ) ){
			/* tem sobrinho vermelho */
			/* casos 2.3 e 2.4 */
			sobrinho_perto = get_Closest_Nephew( irmao , filho_esq );
			if( sobrinho_perto && sobrinho_perto->red ){
			    /* caso 2.3 */
			    irmao->red = true;
			    sobrinho_perto->red = false;
			    if( filho_esq ){
				rotate_right( irmao );
				irmao = sobrinho_perto;
				sobrinho_perto = irmao->esq;
			    }
			    else{
				rotate_left( irmao );
				irmao = sobrinho_perto;
				sobrinho_perto = irmao->dir;
			    }
			}
			/* caso 2.4 */
			if( ptr->pai->red )/* troca cores */
			    irmao->red = true;
			ptr->pai->red = false;
			if( filho_esq ){
			    rotate_left( ptr->pai );
			    irmao->dir->red = false;
			}
			else{
			    rotate_right( ptr->pai );
			    irmao->esq->red = false;
			}
			if( duplo_preto )/* DP */
			    duplo_preto = false;
			else{/* e' folha */
			    if( filho_esq )
				ptr->pai->esq = nullptr;
			    else
				ptr->pai->dir = nullptr;
			    delete ptr;
			}
		    }/* end if (tem sobrinho vermelho) */
		    else{/* nao tem sobrinho vermelho, caso 2.1  */
			irmao->red = true;
			aux = ptr->pai;
			if( aux->red ){/* pai vermelho */
			    aux->red = false;
			    if( duplo_preto )/* DP */
				duplo_preto = false;
			    else{/* e' folha */
				if( filho_esq )
				    aux->esq = nullptr;
				else
				    aux->dir = nullptr;
				delete ptr;
			    }
			}/* end if (aux->red) */
			else{/* pai preto */
			    if( !duplo_preto ){/* folha */
				duplo_preto = true;
				if( filho_esq )
				    aux->esq = nullptr;
				else
				    aux->dir = nullptr;
				delete ptr;
			    }
			    ptr = aux;
			    if( ptr->pai == nullptr )/* nao tem pai */
				duplo_preto = false;
			}
		    }/* end else (caso 2.1) */
		}while( duplo_preto );
	    }/* end else (folha preta) */
	}/* end else (tem pai) */
    }
    else/*nao achou*/
	cout << "Nao ";
}

/*
get_Closest_Nephew():
Devolve um apontador para o No sobrinho
mais proximo de A;
Precisa receber o parametro left_child
com valor True se A e' filho esquerdo,
False caso contrario.
*/
template <class Chave, class Item>
typename arvoreRN<Chave,Item>::Tree
arvoreRN<Chave,Item>::get_Closest_Nephew( Tree A , bool left_child )
{
    if( left_child )
	return A->esq;
    else
	return A->dir;
}

/*
get_Brother():
Devolve um apontador para o No irmao de A;
Precisa receber o parametro left_child
com valor True se A e' filho esquerdo,
False caso contrario.
*/
template <class Chave, class Item>
typename arvoreRN<Chave,Item>::Tree
arvoreRN<Chave,Item>::get_Brother( Tree A , bool left_child )
{
    if( left_child )
	return A->pai->dir;
    else
	return A->pai->esq;
}

/*
has_red_child():
Devolve True se a sub-arvore esquerda de A
e' vermelha; devolve True se a sub-arvore
direita de A e' vermelha; devolve False
caso contrario.
*/
template <class Chave, class Item>
bool arvoreRN<Chave,Item>::has_red_child( Tree A )
{
    if( A->esq && A->esq->red )
	return true;
    else if( A->dir && A->dir->red )
	return true;
    else
	return false;
}

/*
is_left_child():
Devolve True se A e' filho esquerdo, False
caso contrario. Nao deve ser chamada quando
A->pai == nullptr.
*/
template <class Chave, class Item>
bool arvoreRN<Chave,Item>::is_left_child( Tree A )
{
    if( A == A->pai->esq )
	return true;
    else
	return false;
}

/*
Maior_Folha_Esq():
Retorna a maior folha na sub-arvore esquerda
de A. Se a sub-arvore esquerda de A e'
nullptr, devolve nullptr.
*/
template <class Chave, class Item>
typename arvoreRN<Chave,Item>::Tree
arvoreRN<Chave,Item>::Maior_Folha_Esq( Tree A )
{
    Tree folha;
    while( A->esq ){
	folha = A->esq;
	while( folha->dir )
	    folha = folha->dir;
	copy_From_To( folha , A );
	A = folha;
    }
    return A;
}

/*
Menor_Folha_Dir():
Retorna a menor folha na sub-arvore direita
de A. Se a sub-arvore direita de A e'
nullptr, devolve nullptr.
*/
template <class Chave, class Item>
typename arvoreRN<Chave,Item>::Tree
arvoreRN<Chave,Item>::Menor_Folha_Dir( Tree A )
{
    Tree folha;
    while( A->dir ){
	folha = A->dir;
	while( folha->esq )
	    folha = folha->esq;
	copy_From_To( folha , A );
	A = folha;
    }
    return A;
}

/*
copy_From_To():
Copia chave e valor do No apontado por Orig para
o No apontado por Dest. Se algum dos dois e'
nullptr, nao faz nada.
*/
template <class Chave, class Item>
void arvoreRN<Chave,Item>::copy_From_To( Tree Orig , Tree Dest ){
	if( !Orig || !Dest ){
	    cerr << "Invalid Copy (nullptr Orig or Dest)\n";
	    return;
	}
	Dest->P.setCh( Orig->P.getCh() );
	Dest->P.setVal( Orig->P.getVal() );
}

/*
rank():
Devolve quantos objetos ha' na lista com Chave
menor ou igual ao valor de entrada. Se a lista
esta vazia, devolve 0; se a chave nao esta na
lista, devolve o rank que teria se estivesse.
*/
template <class Chave, class Item>
int arvoreRN<Chave,Item>::rank( Chave chave ){
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
	else{/* aux > 0 */
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
int arvoreRN<Chave,Item>::conta_R( Tree r ){
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
Chave arvoreRN<Chave,Item>::seleciona( int k ){
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
	else{/* count + aux < k */
	    count += 1 + aux;
	    ptr = ptr->dir;
	}
    }
    return CHAVE_VAZIO;
}

/*
rotate_right():
Recebe um apontador para no e rotaciona para
a direita sub-arvore que tem como raiz esse no.
*/
template <class Chave, class Item>
void arvoreRN<Chave,Item>::rotate_right( Node* ptr ){
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
void arvoreRN<Chave,Item>::rotate_left( Node* ptr ){
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

#endif
