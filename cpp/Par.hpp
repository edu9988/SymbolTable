/**************************************************************/
/* José Eduardo R. M. Peres y Peres       No. USP:8945332     */
/* Exercicio-Programa EP1                                     */
/* Curso MAC323 - Prof. Carlos Eduardo Ferreira	              */
/*                                                            */
/* Compilador: GNU GCC Compiler                               */
/**************************************************************/

/*
Arquivo Par.hpp
Contem a classe Par
*/

#ifndef PAR_HPP
#define PAR_HPP

using namespace std;

template <class Chave, class Item>
class Par
{
    private:
	Chave ch;
	Item val;
    public:
	Par();
	~Par();
	void setCh( Chave );
	void setVal( Item );
	Chave getCh();
	Item getVal();
};

/*
Par():
Construtor; aloca um Par.
*/
template <class Chave, class Item>
Par<Chave,Item>::Par():ch(nullptr),val(new int){}

/*
~Par():
Destrutor; libera a memória alocada.
*/
template <class Chave, class Item>
Par<Chave,Item>::~Par(){
    delete val;
}
 
/*
setCh():
*/
template <class Chave, class Item>
void Par<Chave,Item>::setCh( Chave chave ){
    ch = chave;
}

/*
setVal():
*/
template <class Chave, class Item>
void Par<Chave,Item>::setVal( Item valor ){
    *val = *valor;
}

/*
getCh():
*/
template <class Chave, class Item>
Chave Par<Chave,Item>::getCh(){
    return ch;
}

/*
getVal():
*/
template <class Chave, class Item>
Item Par<Chave,Item>::getVal(){
    return val;
}

#endif
