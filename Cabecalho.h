#ifndef CABECALHO_H
#define CABECALHO_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//Estrutura que representa uma caixa:
typedef struct box
{
	int id;   //Identificador
	int rot;  //Rotação aplicada na caixa
	int altu; //Altura da caixa
	int larg; //Largura da caixa
	int prof; //Profundidade da caixa
	int valor;//Valor da caixa
}Caixa;

//Estrutura que representa uma celula da lista de caixas a serem empilhadas
typedef struct cel {
	Caixa cx; //Possui uma caixa
	struct cel *prox; // Ponteiro para a proxima celula
}Celula;
//Estrutura que representa uma lista encadeada feita de celulas
typedef struct lista
{
	Celula *c;// Possui uma celula
	int Num;
}Lista;

int **M_caixas_dup; //matriz que indica para cada posicao da matriz M_din, quantas caixas iguais se encontram na posicao
Caixa **M_din; //matriz que utiliza prog dinamica que armazena os valores subotimos para o calculo da solucao otima
Caixa *caixas; // vetor de caixas existentes
int H; //altura maxima da pilha 
int N; // numero de caixas

//funcao referente a entrada de dados, recebe o nome do arquivo de entrada como parametro
void Leitura(const char*);
void Saida(Lista *,int,const char*);
//funcao principal do empilhamento de caixas utilizando programacao dinamica botton-up;
int Empilha(Lista *);

//funcao responsavel por desalocar a memoria;
void Termina(Lista *);


#endif // !CABECALHO_H