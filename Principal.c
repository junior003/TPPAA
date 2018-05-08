/*
*Universidade Federal de Ouro Preto
*Departamento de Computação
*Ciência da Computação - Projeto e Análise de Algoritmos
*
*
*Autor: Joaquim Alves Viana Júnior - 14.2.4338
*E-mail: jojuvi3@gmail.com
*
*/

#include "Cabecalho.h"
#include "Funcoes.c"

int main(int argc, char * argv[])
{
	clock_t inicio = clock(), fim;
	double segundos;

	int Arq_entrada = 1,rec,Arq_saida = 2;
	int lucro_obt1=0;
	Caixa c;

	Leitura(argv[Arq_entrada]);
	Lista P;
	Cria_Lista(&P);
	lucro_obt1 = Empilha(&P);

	printf("Lucro maximo obtido: %d", lucro_obt1);
	Saida(&P, lucro_obt1,argv[Arq_saida]);
	Termina(&P);
	fim = clock();
	segundos = ((double)fim - inicio) / ((double)CLOCKS_PER_SEC);
	printf("\nTEMPO DE EXECUCAO: %.3f\n", segundos);
	

	return 0;
}


