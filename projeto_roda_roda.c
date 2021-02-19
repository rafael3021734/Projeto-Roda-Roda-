/*Bibliotecas*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Structs*/
typedef struct{
	char pista[17];
	int qtd;
	char vetpalavras[3][17];
} Roda;

typedef struct {
	float valor[30];
	int contador;
} Premio;

 /*Variáveis globais*/
Roda roda[15];
Premio premios;
char plvrs_ocultas[3][17];

int ler_arquivos(void); // função que lê e armazena conteudo dos arquivos pistas e premios
int inicia_jogada(int); // função que cria a string plvrs_ocultas
int verif_letra (int); //função que verifica cada letra digitada
int verif_palavra (int); //função que compara as palavras digitadas na rodada final
void imprime_placar (int,float[], int); // função que imprime o imprime_placar

/*Main*/
int main()
{
	int i=0; 
	int j;
	int qtd_pistas; //armazena quantidade de pistas salvas na variável roda
	int qtd_letras; //contador de letras na(s) palavra(s) sorteada(s)
	int acerto; //para descobrir se houve acertos na rodada
	int qtd_acertos=0; //acumula a quantidade de letras que foi acertada
	int contagem_final; //ajuda a contar o numero de rodadas para a etapa final (quando faltam menos de 3 letras)
	int vencedor=-1; //se torna o i (0, 1 ou 2) do jogador que vence 
	int k=0;
	int sorteio; //variável que armazena qual pista foi sorteada na rodada
	float jogador[3]= {0,0,0}; //placar dos jogadores zerado a princípio 
	float valor; //valor sorteado na roleta na rodada
	
	qtd_pistas = ler_arquivos(); //recebe total de pistas lidas no arquivo
	
	srand(time(NULL));
	sorteio = rand()% qtd_pistas; //sorteia 1 entre total de pistas
	qtd_letras = inicia_jogada(sorteio); // retorna quantos caracteres existem no total das palavras

	while ( qtd_letras > qtd_acertos + 3 )
	{ 
		imprime_placar(sorteio, jogador,vencedor);	
		printf("\nEh a vez do jogador %d. RODA A RODA! ");
	  	fflush(stdin);
		getchar();
		srand(time(NULL));
		valor = premios.valor[rand()%premios.contador]; //sorteia um premio no vetor de premios

		if (valor == 0)
		{
			printf("\nJOGADOR %d PASSOU A VEZ!\n", i+1);
			i = (i+1)%3; //vai para próximo jogador
      		printf("\nA vez sera passada para o jogador %d.\n\nPressione qualquer tecla.",i+1);
      		fflush(stdin);
			getch(); //espera o usuario clicar enter
		}
	
		else if(valor == 0.01f){
			printf("\nJOGADOR %d PERDEU TUDO!\n", i+1);
			jogador[i] = 0; //zerar placar do jogador ativo
			i = (i+1)%3; //vai para próximo jogador
			printf("\nA vez sera passada para o jogador %d.\n\nPressione qualquer tecla.", i+1);
			fflush(stdin);
			getch(); 
		}
		else { 
			printf("Valendo R$%.2f. ", valor);			
			acerto = verif_letra (sorteio);  //recebe a quantidade de acertos da função que verifica cada letra
      
			if (acerto>0) {
				jogador[i] = jogador[i] + valor*acerto;
				qtd_acertos += acerto; //armazena total de caracteres descobertos
				if ( qtd_letras - qtd_acertos == 0 ){ //jogador venceu antes do esperado, antes da rodada com as palavras inteiras
							vencedor = i;
							system("cls");
							imprime_placar ( sorteio, jogador, vencedor );
							printf("\nPressione qualquer tecla para fechar.");
							fflush(stdin);
							getch();
							return 0;
				}
			}
			else
			{
        		i = (i+1)%3;//vai para próximo jogador
        		printf("\nA vez sera passada para o jogador %d.\n\nPressione qualquer tecla.", i+1);
        		fflush(stdin);
				getch(); //espera o usuario clicar em enter
			}
		}
      	system("cls"); //limpa a tela
	}
	contagem_final = 3;

	while ( vencedor==-1 && contagem_final>0 ){ 
	imprime_placar(sorteio,jogador,vencedor);	
	printf("\nJogador %d, RODA A RODA!",i+1);
	fflush(stdin);
	getchar();
	srand(time(NULL));
	valor = premios.valor[rand()%premios.contador];
	printf("\nSoh falta(m) %d letra(s)!\n", qtd_letras-qtd_acertos);
	printf("Voce tem 5 segundos para pensar e depois digitar a palavra.");
	fflush(stdin);
	for(j=5;j!=0; j--){ //contagem regressiva
		printf("%d...", j);
		Sleep(1000);
	} 
	printf("\n\nValendo R$ %.2f.\n", valor+jogador[i]);
	if (verif_palavra(sorteio) == 1){ //jogador ganhou
		vencedor = i;
		jogador[i] += valor;
      	system("cls");
		imprime_placar (sorteio, jogador, vencedor );
		printf("\nPressione qualquer tecla para fechar.");
		fflush(stdin);
		getch();
		return 1;
	}
	else
	{
		i = (i+1)%3;
		contagem_final--; //conta quantas vezes jogaram a rodada com as palavras inteiras
	}
    system("cls");
	}
	
	imprime_placar (sorteio, jogador, vencedor );
	printf("\nNinguem acertou! Fim de jogo.\n");
	
printf("\nPressione enter para fechar.");
fflush(stdin);
getch();			
return 2;
}

/*Funções*/
int ler_arquivos(void){	
	int i, j=0;
	int qtd_temp;
	char pista_temp[17], palavras_temp[3][17];

	FILE *arq_pistas;
	FILE *arq_premios;
	arq_pistas = fopen("palavras.dat", "rb");
	if (arq_pistas == NULL){
		printf("\nERRO AO ABRIR O ARQUIVO\n"); //teste de erro
		printf("\nPressione qualquer tecla para fechar.");
		fflush(stdin);
		getch();
 		exit(0);
	}
	while ( !feof (arq_pistas) )
	{
		if (fscanf(arq_pistas,"%s %d", pista_temp, &qtd_temp) != 2 && !feof(arq_pistas) ) 
		//primeiro faz a leitura do nome e da quantidade de palavras e armazena nas variaveis temporarias
		//caso não consiga realizar essa ação (fscanf != 2) e não seja o final do arquivo, exibe a mensagem de erro e sai do programa
		{ 
			printf("\nERRO NO FORMATO DO ARQUIVO!\n");
			printf("\nPressione qualquer tecla para fechar.");
			fflush(stdin);
			getch();
			exit(1);
    	}
    	for (i = 0; i < qtd_temp; i++){
			if (fscanf(arq_pistas, "%s", palavras_temp[i]) != 1 && !feof(arq_pistas) )  
		// agora faz a leitura de cada uma das palavras da linha. o máximo de palavras é o armazenado na variavel qtd_temp
			{
				printf("\nERRO AO ABRIR O ARQUIVO!\n");
				printf("\nPressione qualquer tecla para fechar.");
				fflush(stdin);
				getch();
 				exit(2);
      		}
    	}
    	//finalmente, armazena os valores da pista, quantidade e palavras das variaveis temporárias na struct global do jogo
    	strcpy(roda[j].pista, pista_temp); 
		roda[j].qtd = qtd_temp; 
		for (i = 0; i < qtd_temp; i++)
			strcpy(roda[j].vetpalavras[i], palavras_temp[i]); 
		j++; //atualiza j para a proxima palavra ser amarzenada na posição seguinte do vetor
	}
	fclose(arq_pistas);
	
	arq_premios = fopen("premios.dat", "rb"); // arq_premios é a variavel que retorna a abertura binária do premios.dat 
	i=0;
	if(arq_premios == NULL) { 
		printf("ERRO AO ABRIR O ARQUIVO!\n");
		printf("\nPressione qualquer tecla para fechar.");
		fflush(stdin);
		getch();
		exit(3);
		} // teste de erro
	while ( !feof(arq_premios) && i < 30 ){ //percorre os prêmios registrados no premios.dat com um limite de 30 (tamanho definido na struct)
		if ( !feof(arq_premios) ) fscanf(arq_premios, "%f\n", &premios.valor[i]); //lê cada número como float e armazena na variável global premios
		i++; //contador de premios
		
	}
	premios.contador = i; //armazena o contador para poder usar no sorteio dos premios
		
	fclose(arq_premios); //fechar o arquivo
	 
	return j; //devolve a quantidade total de pistas  
	
	}

int inicia_jogada(int sorteio)
{ 
	int i, j, contagem=0, qtddLetras;
	for (i=0; i<roda[sorteio].qtd ;i++) { //percorre todas as palavras da pista sorteada
		qtddLetras = strlen(roda[sorteio].vetpalavras[i]);
		for (j=0;j<qtddLetras;j++){ //percorre todas as letras até o tamanho de cada palavra
			plvrs_ocultas[i][j]='_';
			contagem++; //conta quantos caracteres existem no total das palavras
		}
	}
	return contagem;
}

void imprime_placar (int sorteio, float dinheiro_do_jogador[], int vencedor ){
	int i, j;
	printf("\n==========================================================\n");
	printf("\n\t\tPista: %s\n\n\t\t", roda[sorteio].pista);
	for (i=0; i<roda[sorteio].qtd; i++) {
		for (j=0;j<strlen(roda[sorteio].vetpalavras[i]);j++){
			if ( vencedor == -1 ) //se for -1, ninguem venceu ainda
				printf(" %c ", plvrs_ocultas[i][j]);
			else //se não for -1, alguem venceu
				printf(" %c ", roda[sorteio].vetpalavras[i][j]); //imprime o vetor original
		}
		printf("\n\t\t");
	}	
	
	printf("\n==========================================================\n");
	printf("\nJogador 1\t\tJogador 2\t\tJogador 3\n");
	printf("==========\t\t==========\t\t==========\n");
	printf("R$%8.2f\t\tR$%8.2f\t\tR$%8.2f\n",dinheiro_do_jogador[0],dinheiro_do_jogador[1],dinheiro_do_jogador[2]);
	switch (vencedor){ //caso alguém vença o roda
		case 0: printf("\nVENCEDOR\n");
		break;
		case 1: printf("\n\t\t\tVENCEDOR\n");
		break;
		case 2: printf("\n\t\t\t\t\t\tVENCEDOR\n");
		break;
	}
	printf("\n==========================================================\n");
}

int verif_letra (int sorteio){
	int i, j, acertos=0, tam;
	char letra;
	do{ //recebe uma letra até o usuário digitar caracter válido
			printf("Digite uma letra: ");
			fflush(stdin);
			scanf("%c", &letra);
				
	} while ( letra < 'A' || ( letra > 'Z' && letra < 'a' ) || letra > 'z' ); 
			
	if ( letra >= 'a' && letra <= 'z') //se for minúscula
			letra =  letra - 32; //transforma em maiúscula
			
	for (i=0; i<roda[sorteio].qtd ; i++){ //percorre todas as palavras
		tam = strlen(roda[sorteio].vetpalavras[i]);
		for (j=0;j<tam;j++){ //percorre todas as letras até atingir o tamanho da palavra
    		if (plvrs_ocultas[i][j] == letra) 
				return 0; //se a letra já foi achada, retorna 0
			else if (roda[sorteio].vetpalavras[i][j] == letra ){ //confere letra por letra para ver qual é igual
				plvrs_ocultas[i][j] = letra; //mostra a letra correta na tela
				acertos++; //contagem de letras descobertas
			} 
		}
	}
  return acertos;
}

int verif_palavra (int sorteio) // funcao que compara a string da palavra final, palavra por palavra
{
	int i, verifica, acert=0;
	char palav[3][17];
	for ( i=0 ; i<roda[sorteio].qtd ; i++ ){
		printf("A palavra %d eh: ", i+1);
		fflush(stdin);
		gets(palav[i]);
		verifica = strcmp ( strupr(palav[i]),roda[sorteio].vetpalavras[i] );
		if ( verifica == 0 ) acert++;
	}
	if ( acert == roda[sorteio].qtd ) return 1;
	else return 0;
}
