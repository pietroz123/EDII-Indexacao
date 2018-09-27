/* ==========================================================================
 * Universidade Federal de São Carlos - Campus Sorocaba
 * Disciplina: Estruturas de Dados 2
 * Prof. Tiago A. de Almeida
 *
 * Trabalho 01
 *
 * RA: 743588
 * Aluno: Pietro Zuntini Bonfim
 * ========================================================================== */

//!  implicit declaration of function 'gerarChave' [-Wimplicit-function-declaration] gerarChave(&j);
//!  undefined reference to `gerarChave'

//todo: INSERIR_NOVO_PRODUTO            1
//todo: REMOVER_PRODUTO                 2
//todo: MODIFICAR_DESCONTO              3
//todo: BUSCAR_PRODUTOS                 4
//todo: LISTAR_PRODUTOS                 5
//todo: LIBERAR_ESPACO                  6
//todo: IMPRIMIR_DADOS                  7
//todo: IMPRIMIR_INDICES_SECUNDARIOS    8
//todo: FINALIZAR                       9



/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


/* Tamanho dos campos dos registros */
#define TAM_PRIMARY_KEY	11
#define TAM_NOME 		51
#define TAM_MARCA 		51
#define TAM_DATA 		11
#define TAM_ANO 		3
#define TAM_PRECO 		8
#define TAM_DESCONTO 	4
#define TAM_CATEGORIA 	51


#define TAM_REGISTRO 	192
#define MAX_REGISTROS 	1000
#define MAX_CATEGORIAS 	30
#define TAM_ARQUIVO (MAX_REGISTROS * TAM_REGISTRO + 1)


/* Saídas para o usuario */
#define OPCAO_INVALIDA 				"Opcao invalida!\n"
#define MEMORIA_INSUFICIENTE 		"Memoria insuficiente!"
#define REGISTRO_N_ENCONTRADO 		"Registro(s) nao encontrado!\n"
#define CAMPO_INVALIDO 				"Campo invalido! Informe novamente.\n"
#define ERRO_PK_REPETIDA			"ERRO: Ja existe um registro com a chave primaria: %s.\n"
#define ARQUIVO_VAZIO 				"Arquivo vazio!"
#define INICIO_BUSCA 		 		"**********************BUSCAR**********************\n"
#define INICIO_LISTAGEM  			"**********************LISTAR**********************\n"
#define INICIO_ALTERACAO 			"**********************ALTERAR*********************\n"
#define INICIO_EXCLUSAO  			"**********************EXCLUIR*********************\n"
#define INICIO_ARQUIVO  			"**********************ARQUIVO*********************\n"
#define INICIO_ARQUIVO_SECUNDARIO	"*****************ARQUIVO SECUNDARIO****************\n"
#define SUCESSO  				 	"OPERACAO REALIZADA COM SUCESSO!\n"
#define FALHA 					 	"FALHA AO REALIZAR OPERACAO!\n"

/* Opções Menu */
#define INSERIR_NOVO_PRODUTO            1
#define REMOVER_PRODUTO                 2
#define MODIFICAR_DESCONTO              3
#define BUSCAR_PRODUTOS                 4
#define LISTAR_PRODUTOS                 5
#define LIBERAR_ESPACO                  6
#define IMPRIMIR_DADOS                  7
#define IMPRIMIR_INDICES_SECUNDARIOS    8
#define FINALIZAR                       9


/* Registro do Produto */
typedef struct produto {
	char pk[TAM_PRIMARY_KEY];           /* N1N2+M1M2+DDMM+AL */
	char nome[TAM_NOME];                /* Nome do Produto ou Modelo */
	char marca[TAM_MARCA];              /* Nome da Empresa */
	char data[TAM_DATA];	            /* Data de Registro do Produto: DD/MM/AAAA */
	char ano[TAM_ANO];                  /* Ano de Lançamento: (últimos dois digitos) */
	char preco[TAM_PRECO];              /* Preço-base: 4 dígitos + 2 casas decimais => DDDD.DD */
	char desconto[TAM_DESCONTO];        /* Desconto sobre o Preço-base (em porcentagem): 3 dígitos => 040 (40% de desconto) */
	char categoria[TAM_CATEGORIA];      /* Categorias separadas por '|' */
} Produto;


/*----- Registros dos Índices -----*/

/* Struct para índice Primário */
typedef struct primary_index{
  char pk[TAM_PRIMARY_KEY];
  int rrn;
} Ip;

/* Struct para índice secundário */
typedef struct secundary_index{
  char pk[TAM_PRIMARY_KEY];
  char string[TAM_NOME];
} Is;

/* Struct para índice secundário de preços */
typedef struct secundary_index_of_final_price{
  float price;
  char pk[TAM_PRIMARY_KEY];
} Isf;

/* Lista ligada para o Índice abaixo*/
typedef struct linked_list{
  char pk[TAM_PRIMARY_KEY];
  struct linked_list *prox;
} ll;

/* Struct para lista invertida */   //? Apenas para icategory
typedef struct reverse_index{
  char cat[TAM_CATEGORIA];
  ll* lista;
} Ir;

/*----- GLOBAL -----*/
char ARQUIVO[TAM_ARQUIVO];

/* ==========================================================================
 * ========================= PROTÓTIPOS DAS FUNÇÕES =========================
 * ========================================================================== */

/* Recebe do usuário uma string simulando o arquivo completo e retorna o número
 * de registros. */
int carregar_arquivo();

/* Exibe o Produto */
int exibir_registro(int rrn, char com_desconto);

/* Recupera do arquivo o registro com o rrn informado
 *  e retorna os dados na struct Produto */
Produto recuperar_registro(int rrn);

/* (Re)faz o índice respectivo */
void criar_iprimary(Ip *indice_primario, int* nregistros);
void criar_iproduct(Is *indice_produto, int* nregistros);       //todo
// void criar_ibrand(Is *indice_marca, int* nregistros);        //todo
// void criar_icategory(Ir *indice_categoria, int* nregistros); //todo
// void criar_iprice(Isf *indice_preco, int* nregistros);       //todo

/* Realiza os scanfs na struct Produto */
void ler_entrada(char* registro, Produto *novo);

/* Rotina para impressao de indice secundario */
void imprimirSecundario(Is* iproduct, Is* ibrand, Ir* icategory, Isf *iprice, int nregistros, int ncat);


/* ==========================================================================
 * ============================ FUNÇÃO PRINCIPAL ============================
 * =============================== NÃO ALTERAR ============================== */
int main(){
    /* Arquivo */
	int carregarArquivo = 0, nregistros = 0, ncat = 0;
    
    //!DElETAR
    printf("Deseja carregar um arquivo?\n");

	scanf("%d%*c", &carregarArquivo); /* 1 (sim) | 0 (nao) */
    if (carregarArquivo)
		nregistros = carregar_arquivo();
    
    //!DELETAR
    //T:
    printf("nregistros: %d\n", nregistros);

	/* Índice primário */
	Ip *iprimary = (Ip *) malloc (MAX_REGISTROS * sizeof(Ip));  //? Aloca-se 1000 espaços na memória para 1000 registros de Ip
  	if (!iprimary) {
		perror(MEMORIA_INSUFICIENTE);
		exit(1);
	}
	criar_iprimary(iprimary, &nregistros);  //todo
    
    //!DELETAR
    printf("Teste no main\n");
    for (int i = 0; i < nregistros; i++) {
        printf("RRN: %d ||| PK: %s\n", iprimary[i].rrn, iprimary[i].pk);
    }

	/*Alocar e criar índices secundários*/ //todo

    // iproduct
    Is *iproduct = (Is*) malloc(MAX_REGISTROS * sizeof(Is));
    if (!iproduct) {
        perror(MEMORIA_INSUFICIENTE);
        exit(1);
    }
    criar_iproduct(iproduct, &nregistros);


	/* Execução do programa */
	int opcao = 0;
	while(1)
	{   
        printf("Digite uma opcao\n"); //!DELETAR
		scanf("%d%*c", &opcao);
		switch(opcao)
		{
			case INSERIR_NOVO_PRODUTO: // 1 //todo
				/*cadastro*/
			break;
			case REMOVER_PRODUTO: // 2 //todo
				/*alterar desconto*/
				printf(INICIO_ALTERACAO);
				/*
				if(alterar([args]))
					printf(SUCESSO);
				else
					printf(FALHA);
				*/
			break;
			case MODIFICAR_DESCONTO: // 3 //todo
				/*excluir produto*/
				printf(INICIO_EXCLUSAO);
				/*
				if(remover([args]))
					printf(SUCESSO);
				else
					printf(FALHA);
				*/
			break;
			case BUSCAR_PRODUTOS: // 4 //todo
				/*busca*/
				printf(INICIO_BUSCA );
                
                //T:
                Produto aux = recuperar_registro(0);
                printf("%s", aux.nome);

			break;
			case LISTAR_PRODUTOS: // 5 //todo
				/*listagens*/
				printf(INICIO_LISTAGEM);
			break;
			case LIBERAR_ESPACO: // 6 //todo
				/*libera espaço*/
			break;
			case IMPRIMIR_DADOS: // 7
				/*imprime o arquivo de dados*/
				printf(INICIO_ARQUIVO);
				printf("%s\n", ARQUIVO);
			break;
			case IMPRIMIR_INDICES_SECUNDARIOS: // 8 //todo
				/*imprime os índices secundários*/
				// imprimirSecundario(iproduct, ibrand, icategory, iprice, nregistros, ncat);
			break;
			case FINALIZAR: // 9 //todo
	      		/*Liberar memória e finalizar o programa */
				return 0;
			break;
			default:
				printf(OPCAO_INVALIDA);
			break;
		}
	}
	return 0;
}


//!DELETAR
void exibeProduto(Produto P) {
    printf("Nome: %s\n", P.nome);
    printf("Marca: %s\n", P.marca);
    printf("Data: %s\n", P.data);
    printf("Ano: %s\n", P.ano);
    printf("Preco: %s\n", P.preco);
    printf("Desconto: %s\n", P.desconto);
    printf("Categoria: %s\n", P.categoria);
    printf("PRIMARY KEY: %s\n", P.pk);
}


/* Exibe o Produto */
int exibir_registro(int rrn, char com_desconto)
{
	if(rrn<0)
		return 0;
	float preco;
	int desconto;
	Produto j = recuperar_registro(rrn);
  	char *cat, categorias[TAM_CATEGORIA];
	printf("%s\n", j.pk);
	printf("%s\n", j.nome);
	printf("%s\n", j.marca);
	printf("%s\n", j.data);
	if(!com_desconto)
	{
		printf("%s\n", j.preco);
		printf("%s\n", j.desconto);
	}
	else
	{
		sscanf(j.desconto,"%d",&desconto);
		sscanf(j.preco,"%f",&preco);
		preco = preco *  (100-desconto);
		preco = ((int) preco)/ (float) 100 ;
		printf("%07.2f\n",  preco);

	}
	strcpy(categorias, j.categoria);

	cat = strtok (categorias, "|");

	while(cat != NULL){
		printf("%s", cat);
		cat = strtok (NULL, "|");
		if(cat != NULL){
			printf(", ");
		}
	}

	printf("\n");

	return 1;
}


int carregar_arquivo()
{
    printf("carrega arquivo\n");
	scanf("%[^\n]s", ARQUIVO); //? O Tiago usa %[^\n]\n: o que isso significa?
	return strlen(ARQUIVO) / TAM_REGISTRO;
}


void gerarChave(Produto *P) {
    strncat(P->pk, P->nome, 2);     // N1N2
    strncat(P->pk, P->marca, 2);    // M1M2

    char *dAux;                     // DDMM
    dAux = strtok(P->data, "/");    // DD
    strncat(P->pk, dAux, 2);
    dAux = strtok(NULL, "/");       // MM
    strncat(P->pk, dAux, 2);
    
    strncat(P->pk, P->ano, 2);      // AL
}
/* Recupera do arquivo o registro com o rrn
 * informado e retorna os dados na struct Produto */
Produto recuperar_registro(int rrn)
{
	char temp[193], *p;
	strncpy(temp, ARQUIVO + ((rrn)*192), 192);
	temp[192] = '\0';
	Produto j;
	p = strtok(temp,"@");
	strcpy(j.nome,p);
	p = strtok(NULL,"@");
	strcpy(j.marca,p);
	p = strtok(NULL,"@");
	strcpy(j.data,p);
	p = strtok(NULL,"@");
	strcpy(j.ano,p);
	p = strtok(NULL,"@");
	strcpy(j.preco,p);
	p = strtok(NULL,"@");
	strcpy(j.desconto,p);
	p = strtok(NULL,"@");
	strcpy(j.categoria,p);
    //!
	// gerarChave(&j); //todo
	return j;
}


/* Imprimir indices secundarios */
void imprimirSecundario(Is* iproduct, Is* ibrand, Ir* icategory, Isf *iprice, int nregistros, int ncat){
	int opPrint = 0;
	ll *aux;
	printf(INICIO_ARQUIVO_SECUNDARIO);
	scanf("%d", &opPrint);
	if(!nregistros)
		printf(ARQUIVO_VAZIO);
	switch (opPrint) {
		case 1: // IMPRIMIR_IS_NOMES
			for(int i = 0; i < nregistros; i++){
				printf("%s %s\n",iproduct[i].pk, iproduct[i].string);
			}
		break;
		case 2: // IMPRIMIR_IS_MARCAS
			for(int i = 0; i < nregistros; i++){
				printf("%s %s\n",ibrand[i].pk, ibrand[i].string);
			}
		break;
		case 3: // IMPRIMIR_IS_CATEGORIAS
			for(int i = 0; i < ncat; i++){
				printf("%s", icategory[i].cat);
				aux =  icategory[i].lista;
				while(aux != NULL){
					printf(" %s", aux->pk);
					aux = aux->prox;
				}
				printf("\n");
			}
		break;
		case 4: // IMPRIMIR_IS_PRECOS_COM_DESCONTOS
		for(int i = 0; i < nregistros; i++){
			printf("%s %.2f\n",iprice[i].pk, iprice[i].price);
		}
		break;
	}
}


/**********************************
        CRIAÇÃO DOS ÍNDICES
***********************************/


void criar_iprimary(Ip *indice_primario, int* nregistros) {

    for (int i = 0; i < *nregistros; i++) {
        indice_primario[i].rrn = 192 * i;
        Produto J = recuperar_registro(i);

        J.pk[0] = '\0';
        strncat(J.pk, J.nome, 2);           // N1N2
        strncat(J.pk, J.marca, 2);          // M1M2
        char *dAux;                         // DDMM
        dAux = strtok(J.data, "/");         // DD
        strncat(J.pk, dAux, 2);
        dAux = strtok(NULL, "/");           // MM
        strncat(J.pk, dAux, 2);
        strncat(J.pk, J.ano, 2);            // AL
        
        exibeProduto(J); //!DELETAR

        strcpy(indice_primario[i].pk, J.pk);
    }

    //!DELETAR
    for (int i = 0; i < *nregistros; i++) {
        printf("RRN: %d ||| PK: %s\n", indice_primario[i].rrn, indice_primario[i].pk);
    }
}

void criar_iproduct(Is *indice_produto, int* nregistros) {



}