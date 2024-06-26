/**************************************************************/
/* José Eduardo R. M. Peres y Peres       No. USP:8945332     */
/* Exercicio-Programa EP1                                     */
/* Curso MAC323 - Prof. Carlos Eduardo Ferreira	              */
/*                                                            */
/* Compilador: GNU GCC Compiler                               */
/**************************************************************/

/*
Arquivo vetorDes.hpp
Contem interface e implementacao de vetorDes.
*/

#ifndef VETORDES_HPP
#define VETORDES_HPP

using namespace std;
#include <iostream> /* cout */
#include <cctype> /* tolower */
#include <fstream> /* ifstream, open, get */
#include <cstring> /* strcmp, strlen */
#include "Par.hpp"

/*
vetorDes:
Definicao da classe vetorDes.
*/
template <class Chave, class Item>
class vetorDes
{
    private:
	int size;
	Par<Chave,Item> *P;/*vetor*/
	int ultimo;/*primeira posicao vazia*/

	char dummy_ch = '\0';
	Chave CHAVE_VAZIO = &dummy_ch;
	int dummy_val = 0;
	Item ITEM_VAZIO = &dummy_val;

	/*funcoes auxiliares*/
	void resize();
	void debug_print_all();
	void qS( Par<Chave,Item> ** , int , int , int );
	int separa( Par<Chave,Item> ** , int , int );
    public:
	vetorDes( string );
	~vetorDes();

	void insere( Chave , Item );
	Item devolve( Chave );
	void remove( Chave );
	int rank( Chave );
	Chave seleciona( int );
};

/*
vetorDes():
Construtor; aloca um vetor de tamanho inicial para 8 Pares
Chave-Item, le do arquivo de entrada inserindo as palavras
no vetor na ordem que aparecem -chamando resize caso
necessario- e inicializa as variaveis size e ultimo.
*/
template <class Chave, class Item>
vetorDes<Chave,Item>::vetorDes( string file_name ):
size(8), P( new Par<Chave,Item>[8] ), ultimo(0){
    int i;
    char a, buff[50];
    ifstream ptr;
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
~vetorDes():
Destrutor; libera a memória alocada para o vetor.
*/
template <class Chave, class Item>
vetorDes<Chave,Item>::~vetorDes()
{
    for( int i=0 ; i<ultimo ; i++ ){
	delete [] P[i].getCh();
    }
    if( P != nullptr )
	delete [] P;
    else
	return;
}

/*
debug_print_all():
Funcao para auxiliar na depuracao; imprime todas
as chaves e itens com seu indice.
*/
template <class Chave, class Item>
void vetorDes<Chave,Item>::debug_print_all(){
    for( int i=0 ; i<ultimo ; i++ ){
	cout << i << "\t" << P[i].getCh(); 
	cout << "\t" << *(P[i].getVal()) << endl;
    }
}

/*
insere():
Recebe um par Chave-Item e o insere no vetor; se
a chave já está na lista, apenas insere o novo valor
naquela posicao; se a chave ainda nao esta na lista,
insere na posicao ultimo, incrementando esta
variavel.
*/
template <class Chave, class Item>
void vetorDes<Chave,Item>::insere( Chave chave, Item valor )
{
    int i, len;
    bool achou = false;
    if( ultimo == size ){
	resize();
    }
    for( i=0 ; i<ultimo ; i++ ){
	if( !strcmp( P[i].getCh(), chave ) ){
	    achou = true;
	    break;
	}
    }
    if( achou )
	P[i].setVal( valor );
    else{
	len = strlen(chave);
	Chave ch_aux = new char[len+1];
	for( i=0 ; i<=len ; i++ )
	    ch_aux[i] = chave[i];
	P[ultimo].setCh( ch_aux );
	P[ultimo].setVal( valor );
	ultimo++;
    }
}

/*
resize():
Aloca um novo vetor com o dobro do tamanho, copia
o conteudo da anterior e libera sua memoria;
Atualiza a variavel size.
*/
template <class Chave, class Item>
void vetorDes<Chave,Item>::resize()
{
    int i;
    size *= 2;
    Par<Chave,Item> *novoVetor = new Par<Chave,Item>[size];
    for( i=0 ; i<ultimo ; i++ ){
	novoVetor[i].setCh( P[i].getCh() );
	P[i].setCh( CHAVE_VAZIO );
	novoVetor[i].setVal( P[i].getVal() );
    }
    for( ; i<size ; i++)
	novoVetor[i].setVal( ITEM_VAZIO );
    delete [] P;
    P = novoVetor;
}

/*
devolve():
Recebe um objeto da classe Chave; se a chave
está na lista, devolve seu Item; devolve
ITEM_VAZIO caso contrario.
*/
template <class Chave, class Item>
Item vetorDes<Chave,Item>::devolve( Chave chave )
{
    int i;
    for( i=0 ; i<ultimo ; i++ )
	if( !strcmp( P[i].getCh() , chave ) )
	    break;
    if( i < ultimo )
	return P[i].getVal();
    else
	return ITEM_VAZIO;
}

/*
remove():
Remove do vetor um Par com determinada chave;
se o removido nao e' o ultimo, tapa o buraco
deslocando o ultimo para aquela posicao; se a
chave nao esta na lista, nao faz nada.
*/
template <class Chave, class Item>
void vetorDes<Chave,Item>::remove( Chave chave ){
    int i;
    for( i=0 ; i<ultimo ; i++ )
	if( !strcmp( P[i].getCh() , chave ) )
	    break;
    if( i<ultimo ){/*(achei)*/
	if( i < ultimo-1 ){/*tapa buraco*/
	    delete [] P[i].getCh();
	    P[i].setCh( P[ultimo-1].getCh() );
	    P[i].setVal( P[ultimo-1].getVal() );
	}
	else{
	    delete [] P[ultimo-1].getCh();
	}
	P[ultimo-1].setCh( CHAVE_VAZIO );
	P[ultimo-1].setVal( ITEM_VAZIO );
	ultimo--;
    }
}

/*
rank():
Devolve quantos objetos ha' no vetor com Chave
menor ou igual ao valor de entrada. A chave da
entrada nao precisa estar na lista.
*/
template <class Chave, class Item>
int vetorDes<Chave,Item>::rank( Chave chave ){
    int i, count = 0;
    for( i=0 ; i<ultimo ; i++ )
	if( strcmp( P[i].getCh() , chave ) < 0 )
	    count++;
    return count;
}

/*
seleciona():
Devolve a chave de rank k. Se ha menos que k+1
elementos no vetor, devolve CHAVE_VAZIO.
*/
template <class Chave, class Item>
Chave vetorDes<Chave,Item>::seleciona( int k ){
    int i;
    Par<Chave,Item> *ptr, **apontadores;
    if( k >= ultimo || k < 0 )
	return CHAVE_VAZIO;
    apontadores = new Par<Chave,Item>*[ultimo];
    for( i=0 ; i<ultimo ; i++ )
	apontadores[i] = &(P[i]);
    qS( apontadores , k , 0 , ultimo-1 );
    ptr = apontadores[k];
    delete [] apontadores;
    return ptr->getCh();
}

/*
qS():
*/
template <class Chave, class Item>
void vetorDes<Chave,Item>::qS( Par<Chave,Item> **ponteiro , int alvo , int p , int r ){
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
int vetorDes<Chave,Item>::separa( Par<Chave,Item> **ponteiro , int p , int r ){
    int i, j, dif;
    Par<Chave,Item> *troca;
    for( j=p-1 , i=p ; i<=r ; i++ ){
	dif = strcmp( ponteiro[i]->getCh() , ponteiro[r]->getCh() );
	if( dif <= 0 ){
	    troca = ponteiro[++j];
	    ponteiro[j] = ponteiro[i];
	    ponteiro[i] = troca;
	}
    }
    return j;
}

#endif
