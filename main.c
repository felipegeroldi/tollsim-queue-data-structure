#include <stdio.h>
#include <conio2.h>
#include <ctype.h>
#include <Windows.h>
#include <time.h>

#include "fila.h"

#define cor_fundo BLACK
#define cor_letra WHITE
#define MAXCABINE 5

// Protótipos e sobrecargas
void put_on_xy(int x, int y, const char *charstring);
void put_on_xy(int x, int y, char letra);
void switch_cabine(int cabine_cod, TpCabine cabines[MAXCABINE]);
void desenhar_tela(void);
void desenhar_pedagio(int pos_ini, int ativo, int id);
void desenhar_menu(void);
void desenhar_divisor(void);
void executar(void);
void simulador(void);
int navegar(int &local);
int menu(void);
void tela_sobre(void);
void tela_instrucoes(void);
int ProcessarFila(TpFila &Fila, int tempo_proc);
void LimparExibicaoFila(TpCabine cabine);
void selecionar_botao(const char* botao, int coluna, int linha);

int main(void)
{
	srand(time(NULL));
    executar();
    
	return 0;
}

//  Executa uma opcao de acordo com o valor retornado pelo menu
void executar()
{
    int op=0;
    do
    {
        switch(op)
        {
            case 9:
                simulador();
                break;
            case 10:
            	tela_instrucoes();
            	break;
            case 11:
                tela_sobre();
                break;
        }

        op=menu();
    }while(op != 12);
}

void tela_instrucoes(void)
{
	clrscr();
	desenhar_tela();
	
	put_on_xy(10,5,"Instrucoes do simulador:");
    put_on_xy(10,6,"- Pressione ESC para encerrar e sair da simulacao");
    put_on_xy(10,7,"- Pressione o numero da cabine para a ativar ou desativar");
    getch();
}

//  Funcao para exibir a tela sobre
void tela_sobre(void)
{
    clrscr();
    desenhar_tela();
    put_on_xy(10,5,"Trabalho sobre a estrutura de dados Fila no 1o. Semestre de 2019");
    put_on_xy(10,6,"pelos alunos:");
    put_on_xy(10,7,"Felipe dos Santos Geroldi (RA: 26181105-3)");
    put_on_xy(10,8,"Gabriel Pellin Caetano (RA: 26181041-3)");

    put_on_xy(10,10,"Professores:");
    put_on_xy(10,11,"Leandro Luiz de Almeida");
    put_on_xy(10,12,"Robson Augusto Siscouto");
    getch();
}

//  Funcao de execução do simulador
void simulador(void)
{
	clrscr();
	desenhar_tela();
	TpCabine cabine[MAXCABINE];
	TpVeiculo veic;
    int i, saiu, cabine_cod, proc_tempo=0; char tecla=0;
    int total_carros=0, total_motos=0, total_grandes=0, total_carros_sp=0,total_motos_sp=0,total_grandes_sp=0;
    int total_veic=0, total_tempo_atendimento=0;
    
    for(i=0; i<MAXCABINE-1; i++)
    {
    	cabine[i].id = i+1;
    	cabine[i].ativo = 1;
    	cabine[i].pos_cabine = 27+(i*12);
    	cabine[i].sem_parar = 0;
    	InicializarFila(cabine[i].FilaCarros);
    	desenhar_pedagio(cabine[i].pos_cabine,cabine[i].ativo,cabine[i].id);
    }
    cabine[i].id = i+1;
	cabine[i].ativo = 2;
	cabine[i].sem_parar = 1;
    cabine[i].pos_cabine = 27+(i*12);
    InicializarFila(cabine[i].FilaCarros);
    desenhar_pedagio(cabine[i].pos_cabine,cabine[i].ativo,cabine[i].id);
    
    desenhar_divisor();
    textcolor(GREEN);
    put_on_xy(2,2,"CABINES PADRAO");
    textcolor(WHITE);
    put_on_xy(2,3,"Total Carros:");
    put_on_xy(2,4,"Total Motos:");
    put_on_xy(2,5,"Total Veic. Grande:");
    
    textcolor(LIGHTBLUE);
    put_on_xy(2,7,"CABINES SEM PARAR");
    textcolor(WHITE);
    put_on_xy(2,8,"Total Carros:");
    put_on_xy(2,9,"Total Motos:");
    put_on_xy(2,10,"Total Veic. Grande:");
    
    put_on_xy(2,21,"Legenda:");
    put_on_xy(2,22,"Carro");put_on_xy(8,22,("%c",254));
    put_on_xy(2,23,"Moto");put_on_xy(7,23,("%c",30));
    put_on_xy(2,24,"Veic. Grande");put_on_xy(15,24,("%c",219));
    
    put_on_xy(2,13,"Tempo medio de espera: ");
    
    put_on_xy(2,19,"Tempo Total:");
    
    while(tecla != 27)
    {
    	while(!kbhit())
    	{
    		for(i=0; i<MAXCABINE; i++)
    		{
    			if(cabine[i].ativo > 0 && !FilaCheia(cabine[i].FilaCarros) && (rand()%2 +1)%2 == 0)
    			{
    				veic.cor = rand()%15 + 1;
    				if(cabine[i].sem_parar == 1)
    					veic.tempo_atendimento = 1;
    				else
    					veic.tempo_atendimento = rand()%10 + 1;
    				veic.tipo_veiculo = rand()%3 +1;
    				veic.tempo_entrada = proc_tempo;
    				total_veic++;
    				total_tempo_atendimento+=veic.tempo_atendimento;
    				
    				InserirNaFila(cabine[i].FilaCarros,veic);
    			}
    			if(!FilaVazia(cabine[i].FilaCarros))
    			{
    				saiu = ProcessarFila(cabine[i].FilaCarros, proc_tempo);
    				if(saiu == 1)
    				{
    					if(cabine[i].sem_parar == 1)
    						total_carros_sp+=1;
    					else
    						total_carros+=1;
    				}
    				else if(saiu == 2)
    				{
    					if(cabine[i].sem_parar == 1)
    						total_motos_sp+=1;
    					else
    						total_motos+=1;
    				}
    				else if(saiu == 3)
    				{
    					if(cabine[i].sem_parar == 1)
    						total_grandes_sp+=1;
    					else
    						total_grandes+=1;
    				}
    			}
    			LimparExibicaoFila(cabine[i]);
    			ExibirFilaCabine(cabine[i]);
    		}
    		proc_tempo++;
    		
    		gotoxy(15,19);printf("%d", proc_tempo);
    		
    		gotoxy(17,3);printf("%d", total_carros);
    		gotoxy(16,4);printf("%d", total_motos);
    		gotoxy(22,5);printf("%d", total_grandes);
    		
    		gotoxy(17,8);printf("%d", total_carros_sp);
    		gotoxy(16,9);printf("%d", total_motos_sp);
    		gotoxy(22,10);printf("%d", total_grandes_sp);
    		gotoxy(2,14);printf("%d", (total_tempo_atendimento/total_veic));
    		gotoxy(80,25);
    		Sleep(800);
    	}
    	tecla = getch();
    	cabine_cod = tecla - '0';
        if(cabine_cod > 0 && cabine_cod < MAXCABINE+1)
        {
            switch_cabine(cabine_cod, cabine);
        }
    }
    getch();
}

int ProcessarFila(TpFila &Fila, int tempo_proc)
{
	TpVeiculo aux = ExibirInicioFila(Fila);
	int saiu = 0;
	if(aux.tempo_atendimento <= tempo_proc-aux.tempo_entrada)
	{
		RetirarDaFila(Fila);
		saiu = 1;
	}
	if(saiu == 1)
		return aux.tipo_veiculo;
	else return 0;
}

void LimparExibicaoFila(TpCabine cabine)
{
	int i, pos_x = cabine.pos_cabine+2;
	for(i=5; i<25; i++)
	{
		put_on_xy(pos_x, i, " ");
	}
}

void switch_cabine(int cabine_cod, TpCabine cabines[MAXCABINE])
{
    cabine_cod-=1;
    if(cabine_cod < MAXCABINE && cabines[cabine_cod].ativo != 0)
    {
        cabines[cabine_cod].ativo = 0;
        desenhar_pedagio(cabines[cabine_cod].pos_cabine,cabines[cabine_cod].ativo,cabines[cabine_cod].id);
    }
    else if(cabine_cod == MAXCABINE-1 && cabines[cabine_cod].ativo == 0)
    {
        cabines[cabine_cod].ativo = 2;
        desenhar_pedagio(cabines[cabine_cod].pos_cabine,cabines[cabine_cod].ativo,cabines[cabine_cod].id);
    }
    else if(cabine_cod < MAXCABINE-1 && cabines[cabine_cod].ativo == 0)
        cabines[cabine_cod].ativo = 1;
        desenhar_pedagio(cabines[cabine_cod].pos_cabine,cabines[cabine_cod].ativo,cabines[cabine_cod].id);
}

//  Funcao principal de execucao do menu
int menu(void)
{
    int pos=9;

    clrscr();
    desenhar_tela();
    do
    {
        switch(pos)
        {
            case 9:
                selecionar_botao(" COMECAR ",35,9);
                break;
            case 10:
                selecionar_botao(" INSTRUCOES ",34,10);
                break;
            case 11:
                selecionar_botao(" SOBRE ",36,11);
                break;
            case 12:
                selecionar_botao(" ENCERRAR ",35,12);
                break;
        }
    }while(navegar(pos) > 0);

    return pos;
}

//  Função para movimentar o cursor entre os botões na tela
int navegar(int &local)
{
    char tecla;

    tecla = getch();
    if(tecla == -32)
    {
        tecla = getch();
        switch(tecla)
        {
            case 80:    // movimenta o cursor para baixo
                local++;
                break;
            case 72:    // movimenta o cursor para cima
                local--;
                break;
            default:
                break;
        }
    }

    if(tecla == 13)
        return -1;

    if(local > 12)
        local=9;
    if(local < 9)
        local=12;

    return 1;
}

//  Função para selecionar o botão na tela
void selecionar_botao(const char* botao, int coluna, int linha)
{
    desenhar_menu();
    textcolor(cor_fundo);
    textbackground(cor_letra);
    put_on_xy(coluna, linha, botao);
    textcolor(cor_letra);
    textbackground(cor_fundo);
}

//  Funcao para exibir uma string na tela, em determinada posicao
void put_on_xy(int x, int y, const char *charstring)
{
    gotoxy(x,y);
    puts(charstring);
}

//  Funcao sobrecarregada para exibir um caractere na tela, em determinada posicao
void put_on_xy(int x, int y, char letra)
{
    gotoxy(x,y);
    printf("%c", letra);
}

//  Funcao para desenhar as opcoes do menu na tela;
void desenhar_menu(void)
{
    put_on_xy(33,6,"CART Simulator");

    put_on_xy(35,9," Comecar ");
	put_on_xy(34,10," Instrucoes ");
    put_on_xy(36,11," Sobre ");
    put_on_xy(35,12," Encerrar ");
}

void desenhar_divisor(void)
{
    int i;
    put_on_xy(26,1,("%c",203));
    put_on_xy(26,25,("%c",202));
    for(i=2; i<25; i++)
    {
        put_on_xy(26,i,("%c",186));
    }
}

//  Funcao para desenhar as cabines do pedagio
// Parametro "ativo": 0 - Inativo, 1 - Ativo, 2 - Sem parar
void desenhar_pedagio(int pos_ini, int ativo, int id)
{
    int i, j;
    
    for(j=2; j<=7; j++)
    {
        put_on_xy(pos_ini,j,("%c",179));
        put_on_xy(pos_ini+4,j,("%c",179));
    }
    for(j=pos_ini+1; j<pos_ini+4; j++)
    {
        put_on_xy(j,2,("%c",196));
        put_on_xy(j,4,("%c",196));
    }

    put_on_xy(pos_ini,2,("%c",218));
    put_on_xy(pos_ini,4,("%c",195));
    put_on_xy(pos_ini+4,2,("%c",191));
    put_on_xy(pos_ini+4,4,("%c",180));
    textcolor(LIGHTGRAY);
	gotoxy(pos_ini+2,2);printf("%d", id);
	textcolor(WHITE);
    if(ativo == 1)
    {
        textcolor(GREEN);
        put_on_xy(pos_ini+2,3,("%c",24));
        textcolor(WHITE);
    }
    else if(ativo == 2)
    {
        textcolor(LIGHTBLUE);
        put_on_xy(pos_ini+2,3,("%c",127));
        textcolor(WHITE);
    }
    else
    {
        textcolor(RED);
        put_on_xy(pos_ini+2,3,"X");
        textcolor(WHITE);
    }

}

void desenhar_tela(void)
{
    int i;
	textcolor(cor_letra);
	textbackground(cor_fundo);
	clrscr();

    put_on_xy(1,1,("%c",201));
    put_on_xy(1,25,("%c",200));
    put_on_xy(80,1,("%c",187));
    put_on_xy(80,25,("%c",188));

	for(i=2; i<80; i++)
    {
        put_on_xy(i,1,("%c",205));
        put_on_xy(i,25,("%c",205));
    }

    for(i=2; i<25; i++)
    {
        put_on_xy(1,i,("%c",186));
        put_on_xy(80,i,("%c",186));
    }
}

