#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <math.h>



//COSNTANTES UTILIZADAS

//Conclusão de funções
#define SUCESSO 0
#define JOGO_CONCLUIDO 00

#define APRESENTATION 3 //Repetições do caractere '#' utilizado para preencher o menu
#define NUM_MINIMO_BOMBA 1
#define INCREMENTA_BOMBA 1
#define INICIALIZA_CAMPO 0
#define INCREMENTA_EIXO 1
#define NUM_MINIMO_EIXO 1
#define NUM_MAXIMO_EIXO 256

//Tratamento de ERROS
#define VALOR_NEGATIVO_EIXO -1
#define MEMORIA_NAO_ALOCADA -2



//Menu de apresentação
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

//Struct para Lista encadeada que varrerá as posições buscando o melhor caminho.
typedef struct setGoodWay_s {
	uint8_t** setPositionInit;
	struct setGoodWay_t* setPositionNext;
} setGoodWay;

//Renomeando o ponteiro para próxima struct
typedef struct setGoodWay_t goodWayNext;

//Função para realizar a busca do melhor caminho
void showMeAGoodWay(uint8_t** fieldOfBomb) {
	/*
	  Passo 1 - Receber o endereço da matriz em uma posição inicial
	  Passo 2 - Verificar se a posição da matriz com +/- 1 no X e +/- 1 no Y possuem valores <= 1 
	  *** Como fazer o passo 2? O endereço pra próxima posição de matriz só pode pegar 1 por vez, nesse caso, teria que passar o da
	      +1 no X e verificar e ir seguind 1 caminho por vez?
	  Passo 3 - Caso encontre um caminho com valor > 1 voltar para posição anterior e buscar uma nova rota
	*/
	
	setGoodWay goodWay;
	goodWay.setPositionInit = fieldOfBomb;

	//Como incrementar as unidades nos eixos X e Y ?
	if (goodWay.setPositionInit ){
	}
	
}


//Função para aloca as linhas da matriz
uint8_t** setMemoryMatrix(uint8_t setAxis_Y, uint8_t setAxis_X){
	//Variável utilizada
	uint8_t i,j;

	//Alocando a memória necessária para o eixo X
	uint8_t ** setMemoryMatrix = (uint8_t**)malloc(setAxis_Y * sizeof(uint8_t));

	if (setMemoryMatrix == NULL) {
		perror("ERROR: Impossível alocar memoria");
	}
	else
	{
		for (i = 0; i < setAxis_Y; i++) {
			//Alocando um y colunas para cada linha
			setMemoryMatrix[i] = (uint8_t*)malloc(setAxis_X * sizeof(uint8_t));
			if (setMemoryMatrix[i] == NULL) {
				perror("ERROR: Impossível alocar memoria");
			}
		}

		//Inicializar a matriz com o número de contador zerado
		for (i = 0; i < setAxis_Y; i++) {
			for (j = 0; j < setAxis_X; j++) {
				setMemoryMatrix[i][j] = INICIALIZA_CAMPO;
			}
		}

	}
	return setMemoryMatrix;
}

//Função para alocar a matriz da bomba
uint16_t* setMemoryBomb(uint16_t setMaxBomb) {
	//Alocando memória
	uint16_t * setMemoryMaxBomb = (uint16_t*)malloc(setMaxBomb * sizeof(uint16_t));
	
	//Teste de alocação
	if (setMemoryMaxBomb == NULL) {
		perror("ERROR: Memoria nao alocada");
	}

	return setMemoryMaxBomb;
}

//Função para atribuição de bombas
//Passo 1 - Sortear os números e alocar nos vetores
uint16_t* setMineBomb(uint16_t * memoryVetBomb, uint16_t getNumberBomb, uint16_t numberMaxBomb) {
	//Variável utilizada
	uint16_t setNumberBomb;
	uint8_t i;

	//Atribuindo as posições da bomba
	for (i = 0; i < getNumberBomb; i++) {
		setNumberBomb = rand() % numberMaxBomb;
		memoryVetBomb[i] = setNumberBomb;
	}
	return memoryVetBomb;
}

//Função para verificar se existem valores repetidos
//Passo 2 - Fazer a verificação dos números
uint16_t* repeatValueBomb(uint16_t* memoryVetBomb, uint16_t getNumberBomb, uint16_t numberMaxBomb) {
	//Variável utilizada
	uint16_t setNumberBomb;
	uint8_t i, j;
	
	//Processo de verificação
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

//Função Play: Todas as chamadas de funções de suporte e desenvolvimento do game estão aqui
int clickToPlay(void) {
	//Variáveis
	uint8_t getAxis_Y, getAxis_X, i, j, numberMaxBomb, positionBomb_X, positionBomb_Y, countBomb =0;
	uint8_t** memoryMatrix;
	uint16_t* memoryVetBomb;
	uint16_t getNumberBomb;

	//Solicitação de entrada para o eixo X
	do {
		printf("Por favor, informe no logo abaixo o numero de linhas que o campo deve ter [VALOR MAXIMO = 256]\n");
		printf("Numero de linhas: ");
		scanf("%hhd", &getAxis_Y);

		//Tratamento de erro: Possibilidade de entrada inválida. Teste de valor
		if (getAxis_Y < NUM_MINIMO_EIXO || getAxis_Y > NUM_MAXIMO_EIXO) {
			printf("ERROR: VALOR INVALIDO, POR FAVOR, TENTE NOVAMENTE\n");
		}
	} while (getAxis_Y > NUM_MAXIMO_EIXO || getAxis_Y < NUM_MINIMO_EIXO);

	//Solicitação de entrada para o eixo Y
	do{
		printf("Agora, digite a quantidade de colunas que [VALOR MAXIMO = 256]\n");
		printf("Numero de Colunas:");
		scanf("%hhd", &getAxis_X);
		
		//Tratamento de erro: Possibilidade de entrada inválida. Teste de valor
		if (getAxis_X < NUM_MINIMO_EIXO || getAxis_X > NUM_MAXIMO_EIXO) {
			printf("ERROR: VALOR INVALIDO, POR FAVOR, TENTE NOVAMENTE\n");
		}
	} while (getAxis_X > NUM_MAXIMO_EIXO || getAxis_X < NUM_MINIMO_EIXO);

	//Calculando o número máximo de bombas que o usuário pode implementar no campo X por Y
	numberMaxBomb = getAxis_Y * getAxis_X;

	//Solicitando e testando o número de bombas que devem ser atribuídos para a matriz
	do {
		printf("Por favor, informe a quantidade de bombas que deseja distribuir no campo, leve em consideração as seguintes informações \n");
		printf("[VALOR MINIMO %d]\n", NUM_MINIMO_BOMBA);
		printf("[VALOR MAXIMO %hhd]\n", (numberMaxBomb-1));
		printf("Numero de bombas:");
		scanf("%hd", &getNumberBomb);
	} while ((getNumberBomb >= numberMaxBomb) || (getNumberBomb < NUM_MINIMO_BOMBA));

	//Alocando o espaço para as bombas
	memoryVetBomb = setMemoryBomb(getNumberBomb);
	
	//Setando as localizações iniciais das bombas
	memoryVetBomb = setMineBomb(memoryVetBomb, getNumberBomb, numberMaxBomb);
	
	//Bloco de verificação para localização da bomba (double check)
	//Utilização de double check para garantir que nenhum número seja repetido
	memoryVetBomb = repeatValueBomb(memoryVetBomb, getNumberBomb, numberMaxBomb); //Check 1
	memoryVetBomb = repeatValueBomb(memoryVetBomb, getNumberBomb, numberMaxBomb); //Check 2
	
	//Alocando espaço para memória da matriz
	memoryMatrix = setMemoryMatrix(getAxis_Y, getAxis_X);

	//Distribuição das bombas
	for (i = 0; i < getNumberBomb; i++) {
		positionBomb_X = memoryVetBomb[i] / getAxis_X; //A posição em Y, ou seja, a linha que deve ser escrita é == ao número inteiro da divisão.
		positionBomb_Y = memoryVetBomb[i] % getAxis_X; //A posição em X, ou seja, a coluna que deve ser escrita é == ao resto da divisão.
		memoryMatrix[positionBomb_X][positionBomb_Y] = (uint8_t) '*';
	}

	/////////////////////////////////////EXISTE UM POSSÍVEL ERRO DE LÓGICA NAS LINHAS A SEGUIR////////////////////////////////////////////////////
	/////////////////////////////////////////////         CORRIGIR         ///////////////////////////////////////////////////////////////////////
	//OBS: As linhas a seguir podem ser simplificadas com loop, no entanto, para melhor visualização, os casos foram individualmente linha por linha
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
				printf("%2hd \t", memoryMatrix[i][j]);// Se for vazio ou próximo de bomba imprimir como número
			}
		}
			printf("\n");
	}

	return JOGO_CONCLUIDO;
}

int main(int argc, char** argv) {
	uint8_t getOption; //Variável utilizada
	
	//Inicializando o rand
	srand((uint16_t)time(NULL));
	
	do {

		apresentation(); //Menu de apresentação do game
		printf("%55s", "PRESS:"); //Solicitação de entrada
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

	//Se chegou aqui é por que deu certo
	return SUCESSO;
}