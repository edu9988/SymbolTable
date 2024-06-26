/**************************************************************/
/* José Eduardo R. M. Peres y Peres       No. USP:8945332     */
/* Exercicio-Programa EP1                                     */
/* Curso MAC323 - Prof. Carlos Eduardo Ferreira	              */
/*                                                            */
/* Compilador: GNU GCC Compiler                               */
/**************************************************************/

/*
Arquivo vetorOrd.hpp
Contem interface e implementacao de vetorOrd.
*/

#ifndef VETORORD_HPP
#define VETORORD_HPP

using namespace std;
#include <iostream> /* cout */
#include <cctype> /* tolower */
#include <fstream> /* ifstream, open, get */
#include <cstring> /* strcmp, strlen */
#include "Par.hpp"

/*
vetorOrd:
Definicao da classe vetorOrd.
*/
template <class Chave, class Item>
class vetorOrd
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
	bool achouBin( Chave , int * );
	void deslocaOrd( int , bool );
	void debug_print_all();
    public:
	vetorOrd( string );
	~vetorOrd();

	void insere( Chave , Item );
	Item devolve( Chave );
	void remove( Chave );
	int rank( Chave );
	Chave seleciona( int );
};

/*
vetorOrd():
Construtor; aloca um vetor de tamanho inicial para 8 Pares
Chave-Item, le do arquivo de entrada inserindo as palavras
no vetor de forma ordenada -chamando resize caso preciso-
e inicializa as variaveis size e ultimo.
*/
template <class Chave, class Item>
vetorOrd<Chave,Item>::vetorOrd( string file_name ):
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
~vetorOrd():
Destrutor; libera a memória alocada para o vetor.
*/
template <class Chave, class Item>
vetorOrd<Chave,Item>::~vetorOrd()
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
void vetorOrd<Chave,Item>::debug_print_all(){
    for( int i=0 ; i<ultimo ; i++ ){
	cout << i << "\t" << P[i].getCh(); 
	cout << "\t" << *P[i].getVal() << endl;
    }
}

/*
achouBin():
Funcao auxiliar; procura uma celula com determinada
chave e devolve verdadeiro se for encontrada, falso
caso contrario; devolve tambem no int indice,
endereco da celula encontrada(caso tenha sido), ou
endereco de onde deve ser inserida(se nao foi
encontrada). Utiliza algoritmo de busca binaria.
*/
template <class Chave, class Item>
bool vetorOrd<Chave,Item>::achouBin( Chave chave, int *indice )
{
    if( ultimo == 0 ){
	*indice = 0;
	return false;
    }
    int aux, esq = 0, dir = ultimo-1, meio = (esq+dir)/2;
    while( dir >= esq ){
	aux = strcmp( chave , P[meio].getCh() );
	if( aux == 0 ){
	    *indice = meio;
	    return true;
	}
	else if( aux < 0 )
	    dir = meio-1;
	else
	    esq = meio+1;
	meio = (esq+dir)/2;
    }
    *indice = esq;
    return false;
}

/*
insere():
Recebe um par Chave-Item e o insere no vetor; se
a chave já está na lista, apenas insere o novo valor
naquela posicao; se a chave ainda nao esta na lista,
insere na posicao correta, deslocando os demais
elementos caso necessario.
*/
template <class Chave, class Item>
void vetorOrd<Chave,Item>::insere( Chave chave, Item valor )
{
    int i, len;
    if( ultimo == size ){
	resize();
    }
    if( achouBin( chave , &i ) )
	P[i].setVal( valor );
    else{
	len = strlen(chave);
	Chave ch_aux = new char[len+1];
	for( int j=0 ; j<=len ; j++ )
	    ch_aux[j] = chave[j];
	deslocaOrd( i , true );
	P[i].setCh( ch_aux );
	P[i].setVal( valor );
	ultimo++;
    }
}

/*
deslocaOrd():
Funcao auxiliar; se praFrente e' true, desloca os elementos
do vetor de indice i para i+1 se i >= j; caso contrario,
desloca de i para i-1 se i > j.
*/
template <class Chave, class Item>
void vetorOrd<Chave,Item>::deslocaOrd( int j , bool praFrente )
{
    int i;
    if( praFrente )
	for( i=ultimo ; i>j ; i-- ){
	    P[i].setCh( P[i-1].getCh() );
	    P[i].setVal( P[i-1].getVal() );
	}
    else
	for( i=j ; i<ultimo-1 ; i++ ){
	    P[i].setCh( P[i+1].getCh() );
	    P[i].setVal( P[i+1].getVal() );
	}
}

/*
resize():
Aloca um novo vetor com o dobro do tamanho, copia
o conteudo da anterior e libera sua memoria;
Atualiza a variavel size.
*/
template <class Chave, class Item>
void vetorOrd<Chave,Item>::resize()
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
Item vetorOrd<Chave,Item>::devolve( Chave chave )
{
    int i;
    if( achouBin( chave , &i ) )
	return P[i].getVal();
    else
	return ITEM_VAZIO;
}

/*
remove():
Remove do vetor um Par com determinada chave,
deslocando os demais elementos para preencher
o espaco vazio, caso necessario; se a chave
nao esta na lista, nao faz nada.
*/
template <class Chave, class Item>
void vetorOrd<Chave,Item>::remove( Chave chave ){
    int i;
    if( achouBin( chave , &i ) ){
	delete [] P[i].getCh();
	deslocaOrd( i , false );
	P[ultimo-1].setCh( CHAVE_VAZIO );
	P[ultimo-1].setVal( ITEM_VAZIO );
	ultimo--;
    }
}

/*
rank():
Devolve o indice do objeto com determinada chave;
se a chave nao esta na lista, devolve a posicao
na qual deveria ser inserida.
*/
template <class Chave, class Item>
int vetorOrd<Chave,Item>::rank( Chave chave ){
    int i;
    achouBin( chave , &i );
    return i;
}

/*
seleciona():
Devolve a chave de rank k. Se ha menos que k+1
elementos no vetor, devolve CHAVE_VAZIO.
*/
template <class Chave, class Item>
Chave vetorOrd<Chave,Item>::seleciona( int k ){
    if( k >= ultimo || k < 0 )
	return CHAVE_VAZIO;
    return P[k].getCh();
}

#endif
