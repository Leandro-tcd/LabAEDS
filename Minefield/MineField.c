#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <math.h>



//COSNTANTES UTILIZADAS

//Conclus�o de fun��es
#define SUCESSO 0
#define JOGO_CONCLUIDO 00

#define APRESENTATION 3 //Repeti��es do caractere '#' utilizado para preencher o menu
#define NUM_MINIMO_BOMBA 1
#define INCREMENTA_BOMBA 1
#define INICIALIZA_CAMPO 0
#define INCREMENTA_EIXO 1
#define NUM_MINIMO_EIXO 1
#define NUM_MAXIMO_EIXO 256

//Tratamento de ERROS
#define VALOR_NEGATIVO_EIXO -1
#define MEMORIA_NAO_ALOCADA -2



//Menu de apresenta��o
void apresentation(void) {
	uint8_t i;
	for (i = 0; i < APRESENTATION; i++) {
		printf("|#####################################################################################################################|\n");
	}
	printf("|#############################################       MINE       ######################################################|\n");
	printf("|#############################################       FIELD      ######################################################|\n");
	for (i = 0; i < APRESENTATION; i++) {
		printf("|#####################################################################################################################|\n");
	}
	printf("\n \n");
	printf("%66s", "PRESS [1] TO PLAY \n");
	printf("%66s %s", "PRESS [0] TO EXIT \n", "\n");
}

//Struct para Lista encadeada que varrer� as posi��es buscando o melhor caminho.
typedef struct setGoodWay_s {
	uint8_t** setPositionInit;
	struct setGoodWay_t* setPositionNext;
} setGoodWay;

//Renomeando o ponteiro para pr�xima struct
typedef struct setGoodWay_t goodWayNext;

//Fun��o para realizar a busca do melhor caminho
void showMeAGoodWay(uint8_t** fieldOfBomb) {
	/*
	  Passo 1 - Receber o endere�o da matriz em uma posi��o inicial
	  Passo 2 - Verificar se a posi��o da matriz com +/- 1 no X e +/- 1 no Y possuem valores <= 1 
	  *** Como fazer o passo 2? O endere�o pra pr�xima posi��o de matriz s� pode pegar 1 por vez, nesse caso, teria que passar o da
	      +1 no X e verificar e ir seguind 1 caminho por vez?
	  Passo 3 - Caso encontre um caminho com valor > 1 voltar para posi��o anterior e buscar uma nova rota
	*/
	
	setGoodWay goodWay;
	goodWay.setPositionInit = fieldOfBomb;

	//Como incrementar as unidades nos eixos X e Y ?
	if (goodWay.setPositionInit ){
	}
	
}


//Fun��o para aloca as linhas da matriz
uint8_t** setMemoryMatrix(uint8_t setAxis_Y, uint8_t setAxis_X){
	//Vari�vel utilizada
	uint8_t i,j;

	//Alocando a mem�ria necess�ria para o eixo X
	uint8_t ** setMemoryMatrix = (uint8_t**)malloc(setAxis_Y * sizeof(uint8_t));

	if (setMemoryMatrix == NULL) {
		perror("ERROR: Imposs�vel alocar memoria");
	}
	else
	{
		for (i = 0; i < setAxis_Y; i++) {
			//Alocando um y colunas para cada linha
			setMemoryMatrix[i] = (uint8_t*)malloc(setAxis_X * sizeof(uint8_t));
			if (setMemoryMatrix[i] == NULL) {
				perror("ERROR: Imposs�vel alocar memoria");
			}
		}

		//Inicializar a matriz com o n�mero de contador zerado
		for (i = 0; i < setAxis_Y; i++) {
			for (j = 0; j < setAxis_X; j++) {
				setMemoryMatrix[i][j] = INICIALIZA_CAMPO;
			}
		}

	}
	return setMemoryMatrix;
}

//Fun��o para alocar a matriz da bomba
uint16_t* setMemoryBomb(uint16_t setMaxBomb) {
	//Alocando mem�ria
	uint16_t * setMemoryMaxBomb = (uint16_t*)malloc(setMaxBomb * sizeof(uint16_t));
	
	//Teste de aloca��o
	if (setMemoryMaxBomb == NULL) {
		perror("ERROR: Memoria nao alocada");
	}

	return setMemoryMaxBomb;
}

//Fun��o para atribui��o de bombas
//Passo 1 - Sortear os n�meros e alocar nos vetores
uint16_t* setMineBomb(uint16_t * memoryVetBomb, uint16_t getNumberBomb, uint16_t numberMaxBomb) {
	//Vari�vel utilizada
	uint16_t setNumberBomb;
	uint8_t i;

	//Atribuindo as posi��es da bomba
	for (i = 0; i < getNumberBomb; i++) {
		setNumberBomb = rand() % numberMaxBomb;
		memoryVetBomb[i] = setNumberBomb;
	}
	return memoryVetBomb;
}

//Fun��o para verificar se existem valores repetidos
//Passo 2 - Fazer a verifica��o dos n�meros
uint16_t* repeatValueBomb(uint16_t* memoryVetBomb, uint16_t getNumberBomb, uint16_t numberMaxBomb) {
	//Vari�vel utilizada
	uint16_t setNumberBomb;
	uint8_t i, j;
	
	//Processo de verifica��o
	for (i = 0; i < getNumberBomb; i++) {
		for (j = 0; j < i; j++) {   
			if (memoryVetBomb[i] == memoryVetBomb[j]) {
				setNumberBomb = rand() % numberMaxBomb;
				memoryVetBomb[j] = setNumberBomb;
				j = 0;
			}
		}
	}

	return memoryVetBomb;
}

//Fun��o Play: Todas as chamadas de fun��es de suporte e desenvolvimento do game est�o aqui
int clickToPlay(void) {
	//Vari�veis
	uint8_t getAxis_Y, getAxis_X, i, j, numberMaxBomb, positionBomb_X, positionBomb_Y, countBomb =0;
	uint8_t** memoryMatrix;
	uint16_t* memoryVetBomb;
	uint16_t getNumberBomb;

	//Solicita��o de entrada para o eixo X
	do {
		printf("Por favor, informe no logo abaixo o numero de linhas que o campo deve ter [VALOR MAXIMO = 256]\n");
		printf("Numero de linhas: ");
		scanf("%hhd", &getAxis_Y);

		//Tratamento de erro: Possibilidade de entrada inv�lida. Teste de valor
		if (getAxis_Y < NUM_MINIMO_EIXO || getAxis_Y > NUM_MAXIMO_EIXO) {
			printf("ERROR: VALOR INVALIDO, POR FAVOR, TENTE NOVAMENTE\n");
		}
	} while (getAxis_Y > NUM_MAXIMO_EIXO || getAxis_Y < NUM_MINIMO_EIXO);

	//Solicita��o de entrada para o eixo Y
	do{
		printf("Agora, digite a quantidade de colunas que [VALOR MAXIMO = 256]\n");
		printf("Numero de Colunas:");
		scanf("%hhd", &getAxis_X);
		
		//Tratamento de erro: Possibilidade de entrada inv�lida. Teste de valor
		if (getAxis_X < NUM_MINIMO_EIXO || getAxis_X > NUM_MAXIMO_EIXO) {
			printf("ERROR: VALOR INVALIDO, POR FAVOR, TENTE NOVAMENTE\n");
		}
	} while (getAxis_X > NUM_MAXIMO_EIXO || getAxis_X < NUM_MINIMO_EIXO);

	//Calculando o n�mero m�ximo de bombas que o usu�rio pode implementar no campo X por Y
	numberMaxBomb = getAxis_Y * getAxis_X;

	//Solicitando e testando o n�mero de bombas que devem ser atribu�dos para a matriz
	do {
		printf("Por favor, informe a quantidade de bombas que deseja distribuir no campo, leve em considera��o as seguintes informa��es \n");
		printf("[VALOR MINIMO %d]\n", NUM_MINIMO_BOMBA);
		printf("[VALOR MAXIMO %hhd]\n", (numberMaxBomb-1));
		printf("Numero de bombas:");
		scanf("%hd", &getNumberBomb);
	} while ((getNumberBomb >= numberMaxBomb) || (getNumberBomb < NUM_MINIMO_BOMBA));

	//Alocando o espa�o para as bombas
	memoryVetBomb = setMemoryBomb(getNumberBomb);
	
	//Setando as localiza��es iniciais das bombas
	memoryVetBomb = setMineBomb(memoryVetBomb, getNumberBomb, numberMaxBomb);
	
	//Bloco de verifica��o para localiza��o da bomba (double check)
	//Utiliza��o de double check para garantir que nenhum n�mero seja repetido
	memoryVetBomb = repeatValueBomb(memoryVetBomb, getNumberBomb, numberMaxBomb); //Check 1
	memoryVetBomb = repeatValueBomb(memoryVetBomb, getNumberBomb, numberMaxBomb); //Check 2
	
	//Alocando espa�o para mem�ria da matriz
	memoryMatrix = setMemoryMatrix(getAxis_Y, getAxis_X);

	//Distribui��o das bombas
	for (i = 0; i < getNumberBomb; i++) {
		positionBomb_X = memoryVetBomb[i] / getAxis_X; //A posi��o em Y, ou seja, a linha que deve ser escrita � == ao n�mero inteiro da divis�o.
		positionBomb_Y = memoryVetBomb[i] % getAxis_X; //A posi��o em X, ou seja, a coluna que deve ser escrita � == ao resto da divis�o.
		memoryMatrix[positionBomb_X][positionBomb_Y] = (uint8_t) '*';
	}

	/////////////////////////////////////EXISTE UM POSS�VEL ERRO DE L�GICA NAS LINHAS A SEGUIR////////////////////////////////////////////////////
	/////////////////////////////////////////////         CORRIGIR         ///////////////////////////////////////////////////////////////////////
	//OBS: As linhas a seguir podem ser simplificadas com loop, no entanto, para melhor visualiza��o, os casos foram individualmente linha por linha
	for (i = 0; i < getAxis_Y; i++) {
		for (j = 0; j < getAxis_X; j++) {

			//Trata casos na extremidade inicial onde i++,j++
			if (memoryMatrix[i][j] == '*' && i == 0 && j == 0) {
				if (memoryMatrix[i++][j] != '*') {
					memoryMatrix[i++][j]++;
				}
				if (memoryMatrix[i][j++] != '*'){
					memoryMatrix[i][j++]++;
				}
				if (memoryMatrix[i++][j++] != '*') {
					memoryMatrix[i++][j++]++;
				}
			}

			//Trata casos nas extremidades finais onde j--, i--
			else if (memoryMatrix[i][j] == '*' && i == (getAxis_Y-1) && j == (getAxis_X-1)) {
				if (memoryMatrix[i][j--] != '*') {
					memoryMatrix[i][j--] ++;
				}
				if (memoryMatrix[i--][j--] != '*') {
					memoryMatrix[i--][j--]++;
				}
				if (memoryMatrix[i--][j] != '*') {
					memoryMatrix[i--][j] ++;
				}
			}

			//Trata as possibilidades de bombas na primeira coluna
			else if (memoryMatrix[i][j] == '*' && j == 0 && i != (getAxis_Y - 1) && i > 0) {
				if (memoryMatrix[i++][j] != '*') {
					memoryMatrix[i++][j]++;
				}
				if (memoryMatrix[i++][j++] != '*') {
					memoryMatrix[i++][j++]++;
				}
				if (memoryMatrix[i][j++] != '*') {
					memoryMatrix[i][j++]++;
				}
				if (memoryMatrix[i--][j] != '*') {
					memoryMatrix[i--][j]++;
				}
				if (memoryMatrix[i--][j++] != '*') {
					memoryMatrix[i--][j++]++;
				}
			}

			//Trata as possibilidades de bombas na ultima coluna
			else if (memoryMatrix[i][j] == '*' && j == (getAxis_X - 1) && i != (getAxis_Y - 1) && i > 0) {
				if (memoryMatrix[i][j--] != '*') {
					memoryMatrix[i][j--]++;
				}
				if (memoryMatrix[i--][j] != '*') {
					memoryMatrix[i--][j]++;
				}
				if (memoryMatrix[i--][j--] != '*') {
					memoryMatrix[i--][j--]++;
				}
				if (memoryMatrix[i++][j] != '*') {
					memoryMatrix[i++][j]++;
				}
				if (memoryMatrix[i++][j--] != '*') {
					memoryMatrix[i++][j--]++;
				}
			}

			//Trata as possibilidades de bomba na primeira linha 
			else if (memoryMatrix[i][j] == '*' && i == 0 && j > 0 && j > getAxis_X) {
				if (memoryMatrix[i][j++] != '*') {
					memoryMatrix[i][j++]++;
				}
				if (memoryMatrix[i++][j] != '*') {
					memoryMatrix[i++][j]++;
				}
				if (memoryMatrix[i++][j++] != '*') {
					memoryMatrix[i++][j++]++;
				}
				if (memoryMatrix[i++][j--] != '*') {
					memoryMatrix[i++][j--]++;
				}
				if (memoryMatrix[i][j--] != '*') {
					memoryMatrix[i][j--]++;
				}
			}

			//Trata as possibilidades de bomba na ultima linha 
			else if (memoryMatrix[i][j] == '*' && i == (getAxis_Y - 1) && j > 0 && j > getAxis_X) {
				if (memoryMatrix[i][j++] != '*') {
					memoryMatrix[i][j++]++;
				}
				if (memoryMatrix[i--][j] != '*') {
					memoryMatrix[i--][j]++;
				}
				if (memoryMatrix[i--][j++] != '*') {
					memoryMatrix[i--][j++]++;
				}
				if (memoryMatrix[i--][j--] != '*') {
					memoryMatrix[i--][j--]++;
				}
				if (memoryMatrix[i][j--] != '*') {
					memoryMatrix[i][j--]++;
				}
			}

		}
	}

	/////////////////////////////////////////////         CORRIGIR         ///////////////////////////////////////////////////////////////////////
	
	//Imprimir a matriz	
	for (i = 0; i < getAxis_Y; i++) {
		for (j = 0; j < getAxis_X; j++) {
			if (memoryMatrix[i][j] == '*') {
				printf("%2c \t", memoryMatrix[i][j]);// Se for bomba imprimir como caracter
			}
			else {
				printf("%2hd \t", memoryMatrix[i][j]);// Se for vazio ou pr�ximo de bomba imprimir como n�mero
			}
		}
			printf("\n");
	}

	return JOGO_CONCLUIDO;
}

int main(int argc, char** argv) {
	uint8_t getOption; //Vari�vel utilizada
	
	//Inicializando o rand
	srand((uint16_t)time(NULL));
	
	do {

		apresentation(); //Menu de apresenta��o do game
		printf("%55s", "PRESS:"); //Solicita��o de entrada
		scanf("%hhd", &getOption);
		printf("\n");

		switch (getOption) {
			case 0: { 
				break; 
			} 
			case 1: {
				clickToPlay();
				break;
			}
			default: {
				printf("WARNING!! ENTRADA INVALIDA, POR FAVOR, TENTE NOVAMENTE PRESSIONANDO [1] PARA JOGAR OU [0] PARA SAIR. \n \n");
				break;
			}
		}
	} while (getOption != 0);

	//Se chegou aqui � por que deu certo
	return SUCESSO;
}