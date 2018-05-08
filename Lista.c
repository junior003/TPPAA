#include "Cabecalho.h"

//Funcao responsavel por criar a cabeca da lista
void Cria_Lista(Lista * P)
{
	P->c = (Celula*)malloc(sizeof(Celula));
	P->c->prox = NULL;
	P->Num = 0;
}
// funcao responsavel por inserir uma caixa x na posicao apos a celula cl
void insere_Lista(Caixa x, Celula *cl)
{
	Celula *nova;
	nova = (Celula*)malloc(sizeof(Celula));
	nova->cx = x;
	nova->prox = cl->prox;
	cl->prox = nova;
}

//funcao responsavel por mostra na tela quais as caixas escolhidas na solucao encontrada
void print_Lista(Celula *cl) {
	Celula *p;
	printf("Valor das caixas empilhadas:\n");
	for (p = cl->prox; p != NULL; p = p->prox)
	{
		printf("ID: %d A:%d L:%d P:%d V:%d R:%d \n", p->cx.id, p->cx.altu, p->cx.larg, p->cx.prof, p->cx.valor, p->cx.rot);
	}
}
void delete_Lista(Celula *cl) {
	Celula *p,*aux;
	p = cl;
	while(p!=NULL)
	{
		aux = p;
		p = p->prox;
		free(aux);
	}
}



