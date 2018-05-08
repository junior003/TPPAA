#include "Cabecalho.h"
#include "mergesort.c"
#include "Lista.c"
//funcao referente a leitura de dados
void Leitura(const char* nome_arq)
{
	FILE *parq;
	int  i, aux, v;
	Caixa cx;
	int altu_aux = 0;
	int larg_aux = 0;
	int prof_aux = 0;
	
	parq = fopen(nome_arq, "r");

	if (parq != NULL)
	{
		fscanf(parq, "%d%d", &N, &H);
		//aloca o dobro de memoria para as caixas, para a rotacao 1 e para a rotacao 2
		caixas = (Caixa *)malloc((2 * N) * sizeof(Caixa)); 
		for (i = 0; i < 2 * N; i++)
		{
			fscanf(parq, "%d", &caixas[i].valor);
			caixas[i + 1].valor = caixas[i].valor;
			i++;
		}
		for (i = 0; i < 2 * N; i++)
		{
			fscanf(parq, "%d", &larg_aux);
			fscanf(parq, "%d", &altu_aux);
			fscanf(parq, "%d", &prof_aux);

			caixas[i].id = i;
			caixas[i].rot = 1;
			caixas[i].altu = altu_aux;
			caixas[i].larg = larg_aux;
			caixas[i].prof = prof_aux;

			caixas[i + 1].id = i;
			caixas[i + 1].rot = 2;
			caixas[i + 1].altu = larg_aux;
			caixas[i + 1].larg = altu_aux;
			caixas[i + 1].prof = prof_aux;
			i++;
		}
		//alocando memoria para as matrizes (2N+1)X(H+1)
		M_caixas_dup = (int **)malloc(((2 * N) + 1) * sizeof(int*));
		M_din = (Caixa **)malloc(((2 * N) + 1) * sizeof(Caixa*));
		for (i = 0; i < (2 * N) + 1; i++)
		{
			M_caixas_dup[i] = (int*)malloc((H + 1) * sizeof(int));
			M_din[i] = (Caixa*)malloc((H + 1) * sizeof(Caixa));
		}
	}
}
	
// Esta funcao busca em uma determinada coluna da matriz ate uma linha limite quais a melhor opcao de caixa
// que se pode colocar em cima sem ferir as restricoes
//O(N)
int buscaMax(int coluna, int ultima_linha)
{
	int max;
	int i,indice = 0;
	max = M_din[0][coluna].valor;
	for (i = 0; i < ultima_linha; i++)
	{
		if ((M_din[i][coluna].larg >= caixas[ultima_linha - 1].larg) && (M_din[i][coluna].prof >= caixas[ultima_linha - 1].prof))
		{
			if (M_din[i][coluna].valor >= max)
			{
				max = M_din[i][coluna].valor;
				indice = i;
			}
		}
	}
	return indice;
}
// Esta funcao pesquisa o maior valor em uma determinada coluna da matriz ate uma linha limite (utilizada para resgatar
// quais caixas foram empilhadas
//O(N)
int buscaMax_lista(int coluna, int ultima_linha)
{
	int max;
	int i,indice = 0;
	max = M_din[0][coluna].valor;
	for (i = 0; i < ultima_linha; i++)
	{
		if (M_din[i][coluna].valor >= max)
		{
			max = M_din[i][coluna].valor;
			indice = i;
		}
	}
	return indice;
}

//O(H*N^2)
int Empilha(Lista *P)
{

	mergeSort(caixas, 0, 2 * N);
	int i, j, k;
	int repeticoes;
	int linha_max = 0, resto = 0;
	Caixa max;
	Caixa valor1, valor2;
	max.valor = 0;
	for (i = 0; i < (2 * N) + 1; i++) //inicializacao da matriz primeira linha e coluna = 0:
	{
		M_din[i][0].valor = 0;
		M_din[i][0].altu = -1;
		M_din[i][0].prof = -1;
		M_din[i][0].larg = -1;
		M_caixas_dup[i][0] = 0;
	}
	for (j = 0; j < H + 1; j++) 
	{
		M_din[0][j].valor = 0;
		M_din[0][j].altu = -1;
		M_din[0][j].prof = -1;
		M_din[0][j].larg = -1;
		M_caixas_dup[0][j] = 0;
	}
	//Os lacos seguintes percorrem toda a matriz calculando a partir de um resultado anterior a melhor solucao
	//baseado na altura e na quantidade de caixas possiveis de serem empilhadas nesta determinada altura.
	
	for (i = 1; i < (2 * N) + 1; i++)
	{
		for (j = 1; j < H + 1; j++)
		{
			M_caixas_dup[i][j] = 0; 
			M_din[i][j].valor = 0; 
			//Esta verificacao resulta em verdadeiro se a caixa que esta sendo analisada cabe dentro da altura estabelecida por 'j':
			if (caixas[i - 1].altu <= j)
			{
				//Este trecho abaixo contabiliza para a posicao analisada no momento, quantas copias da mesma caixa cabem
				//no maximo respeitando as restricoes:
				resto = j - caixas[i - 1].altu;
				while ((resto >= caixas[i - 1].altu) && ((M_caixas_dup[i][j] + 1) *caixas[i - 1].altu <= j))
				{
					M_caixas_dup[i][j]++; //registra na matriz de duplicatas quantas copias cabem no maximo nesta posicao
					resto -= caixas[i - 1].altu;
				}
				//Este laco analisa qual a melhor solucao encontrada se utilizarmos de 1 ate M_caixas_dup[i][j]+2 duplicatas da mesma
				//caixa em uma determinada solucao, este laco ocorre devido ao fato de que a melhor solucao nem sempre e feita de uma mesma
				//caixa, mesmo que ela seja a mais valiosa.
				for (k = 1; k < M_caixas_dup[i][j]+2; k++)
				{
					// A funcao buscaMax busca qual a melhor solucao ja calculada para completar o espaco que sobrou na posicao atual, verificando
					// se a pilha ficara consistente,recebe como parametro a coluna a ser inspecionada e uma linha limite.
					linha_max = buscaMax(j - (k*caixas[i - 1].altu), i);
					valor2.valor = M_din[linha_max][j - (k*caixas[i - 1].altu)].valor;
					valor2.larg = M_din[linha_max][j - (k*caixas[i - 1].altu)].larg;
					valor2.prof = M_din[linha_max][j - (k*caixas[i - 1].altu)].prof;
					if (valor2.valor != 0)
					{
						valor2.valor += k*caixas[i - 1].valor;//adiciona no acumulador valor2 o valor da caixa analisa vezes o numero de copias dela
						valor2.altu = caixas[i - 1].altu;
						valor2.larg = caixas[i - 1].larg;
						valor2.prof = caixas[i - 1].prof;

					}
					else // se o valor2.valor for 0 significa que somente a caixa da posicao atual pode ser posta na pilha
					{
						valor2 = caixas[i - 1];
						valor2.valor = k*caixas[i - 1].valor;
					}
					// esta verificacao serve para capturar quais os melhores valores encontrados utilizando diferentes numeros de copias
					//o maior valor e a solucao para o subproblema que se encontra na posicao analisada
					if (valor2.valor > max.valor)
					{
						max = valor2;
						repeticoes = k;
					}
				}
				valor2 = max;
				M_caixas_dup[i][j] = repeticoes;
				max.valor= 0;

			}
			else
			{
				valor2.valor = 0;
			}
			if (valor2.valor ==0)
			{
				M_din[i][j].valor = 0; // se nao couber a caixa na posicao o valor atribuido � zero
				M_din[i][j].altu = 0;
				M_din[i][j].larg = 0;
				M_din[i][j].prof = 0;
			}
			else
			{
				
				M_din[i][j].valor += valor2.valor; // se couber � acumulado o valor calculado das solucoes anteriores
				M_din[i][j].altu = valor2.altu;
				M_din[i][j].larg = valor2.larg;
				M_din[i][j].prof = valor2.prof;
			}	
		}
	}
	//fim do laco principal, com isso o maior valor da ultima coluna e a solucao otima encontrada

	//itens empilhados
	//Este trecho trata da parte da recuperacao de quais caixas foram empilhadas para se chegar na solucao otima
	//sendo elas adicionadas numa lista
	i = buscaMax_lista(H, (2 * N) + 1);
	j = H;
	while (j >= 0)
	{
		if (i < 1)
		{
			break;
		}
		if ((M_din[i][j].valor != M_din[i - 1][j].valor ))
		{
			if (M_caixas_dup[i][j] == 0)
			{
				
				insere_Lista(caixas[i - 1], P->c);
				P->Num++;
			}
			else
			{
				for (k = 0; k < M_caixas_dup[i][j]; k++)
				{
					insere_Lista(caixas[i - 1], P->c);
					P->Num++;
				}
			}
			j= j - caixas[i - 1].altu*(M_caixas_dup[i][j]);
			i= buscaMax_lista(j,i);
		}
		else
		{
			i--;
		}
	}
	//A funcao comentada abaixo serve para mostrar quais as caixas escolhidas, descomente-a para visualizar em tempo de execucao.
	//print_Lista(P.c);
	i = buscaMax_lista(H, (2*N)+1);
	return M_din[i][H].valor; //retorna o maior valor da ultima coluna

}

void Saida(Lista *P,int result,const char* nome)
{
	FILE *parq;
	Celula *cl = P->c;
	parq = fopen(nome, "w+");
	Celula *p;
	fprintf(parq, "%d\n", result);
	fprintf(parq, "%d\n", P->Num);
	for (p = cl->prox; p != NULL; p = p->prox)
	{
		fprintf(parq, "%d %d \n", p->cx.id, p->cx.rot);
	}
	fclose(parq);
}
//Funcao responsavel por desalocar memoria
void Termina(Lista * P)
{
	int i;
	for (i = 0; i < (2 * N) + 1; i++)
	{
		free(M_caixas_dup[i]);
		free(M_din[i]);
	}
	free(M_caixas_dup);
	free(M_din);
	free(caixas);
	delete_Lista(P->c);
}