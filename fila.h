#define MAXFILA 10

struct TpVeiculo
{
	int tipo_veiculo, tempo_atendimento, tempo_entrada, cor;
};

struct TpFila
{
	TpVeiculo veiculo[MAXFILA];
	int inicio, fim, contador;
};

struct TpCabine
{
    int ativo, pos_cabine, sem_parar, id;
    TpFila FilaCarros;
};

void InicializarFila(TpFila &F)
{
	F.inicio = 0;
	F.fim = -1;
	F.contador = 0;
}

int FilaCheia(TpFila F)
{
	return (F.contador == MAXFILA-1);
}

int FilaVazia(TpFila F)
{
	return (F.contador == 0);
}

void InserirNaFila(TpFila &F, TpVeiculo V)
{
	if(F.fim == MAXFILA-1)
		F.fim = -1;

    F.veiculo[++F.fim] = V;
    F.contador++;
}

TpVeiculo ExibirInicioFila(TpFila F)
{
	return F.veiculo[F.inicio];
}

TpVeiculo ExibirFimFila(TpFila F)
{
	return F.veiculo[F.fim];
}

TpVeiculo RetirarDaFila(TpFila &F)
{
    TpVeiculo aux = F.veiculo[F.inicio++];

	if(F.inicio == MAXFILA)
        F.inicio = 0;
	F.contador--;
	return aux;
}

void ExibirFilaCabine(TpCabine C)
{
	TpVeiculo aux;
    int posicao_x = C.pos_cabine+2;
    int posicao_y = 5;
    while(!FilaVazia(C.FilaCarros))
    {
        gotoxy(posicao_x,posicao_y);
        aux = RetirarDaFila(C.FilaCarros);
        textcolor(aux.cor);
        if(aux.tipo_veiculo == 1) // carro
        {
        	printf("%c",254);
        }
        else if(aux.tipo_veiculo == 2) // moto
        {
        	printf("%c",30);
        }
        else // caminhao
        {
        	printf("%c",219);
        }
        posicao_y+=2;
    }
    textcolor(WHITE);
}
