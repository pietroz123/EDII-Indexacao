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

//todo: INSERIR_NOVO_PRODUTO            1
//todo: REMOVER_PRODUTO                 2
//todo: MODIFICAR_DESCONTO              3
//todo: BUSCAR_PRODUTOS                 4
//todo: LISTAR_PRODUTOS                 5
//todo: LIBERAR_ESPACO                  6
//todo: IMPRIMIR_INDICES_SECUNDARIOS    8
//todo: FINALIZAR                       9

#define NREGISTROS (strlen(ARQUIVO) / TAM_REGISTRO)


/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>


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
int NCAT = 0;

/* ==========================================================================
 * ========================= PROTÓTIPOS DAS FUNÇÕES =========================
 * ========================================================================== */

/* Recebe do usuário uma string simulando o arquivo completo e retorna o número de registros. */
int carregar_arquivo();

/* Exibe o Produto */
int exibir_registro(int rrn, char com_desconto);

/* Recupera do arquivo o registro com o rrn informado e retorna os dados na struct Produto */
Produto recuperar_registro(int rrn);

/* (Re)faz o índice respectivo */
//todo: ordenar os índices
void criar_iprimary(Ip *indice_primario, int* nregistros);
void criar_iproduct(Is *indice_produto, int* nregistros);
void criar_ibrand(Is *indice_marca, int* nregistros);
void criar_icategory(Ir *indice_categoria, int* nregistros); //todo
void criar_iprice(Isf *indice_preco, int* nregistros);       //todo

void refaz_iprimary(Ip *indice_primario, int* nregistros);
void refaz_iproduct(Is *indice_produto, int* nregistros);
void refaz_ibrand(Is *indice_marca, int* nregistros);
void refaz_icategory(Ir *indice_categoria, int* nregistros);
void refaz_iprice(Isf *indice_preco, int* nregistros);   


/* Realiza os scanfs na struct Produto */
void ler_entrada(char* registro, Produto *novo);

/* Rotina para impressao de indice secundario */
void imprimirSecundario(Is* iproduct, Is* ibrand, Ir* icategory, Isf *iprice, int nregistros, int ncat);


/****** INTERAÇÃO COM O USUÁRIO ******/

// (1) INSERCAO 
void inserir(Ip *iprimary, Is* iproduct, Is* ibrand, Ir* icategory, Isf *iprice);

// (2) ALTERAÇÃO
//todo

// (3) REMOÇÃO
//todo

// (4) BUSCAR PRODUTOS - Busca pelo produto e retorna o RRN
int buscarProdutos(Ip *iprimary, Is *iproduct, Ir *icategory, Is *ibrand);

// (5) LISTAGEM
void listarProdutos(Ip *iprimary, Ir *icategory, Is *ibrand, Isf *iprice, int nregistros);

// (6) LIBERAR ESPAÇO
//todo

// (7) IMPRIMIR ARQUIVO DE DADOS
//!Já implementado

// (8) IMPRIMIR INDICES SECUNDARIOS
//!Já implementado

// (9) FINALIZAR
//todo



/****** FUNÇÕES AUXILIARES ******/
int nREG();


/* ==========================================================================
 * ============================ FUNÇÃO PRINCIPAL ============================
 * =============================== NÃO ALTERAR ============================== */
int main(){
    /* Arquivo */
	int carregarArquivo = 0, nregistros = 0, ncat = 0;
    Produto I;


	scanf("%d%*c", &carregarArquivo); /* 1 (sim) | 0 (nao) */
    if (carregarArquivo)
		nregistros = carregar_arquivo();


	/* Índice primário */
	Ip *iprimary = (Ip *) malloc (MAX_REGISTROS * sizeof(Ip));  //? Aloca-se 1000 espaços na memória para 1000 registros de Ip
  	if (!iprimary) {
		perror(MEMORIA_INSUFICIENTE);
		exit(1);
	}
	criar_iprimary(iprimary, &nregistros);


	/* Alocar e criar índices secundários */ //todo

    /****** iproduct ******/
    Is *iproduct = (Is*) malloc(MAX_REGISTROS * sizeof(Is));
    if (!iproduct) {
        perror(MEMORIA_INSUFICIENTE);
        exit(1);
    }
    criar_iproduct(iproduct, &nregistros);

    /****** ibrand ******/
    Is *ibrand = (Is*) malloc(MAX_REGISTROS * sizeof(Is));
    if (!ibrand) {
        perror(MEMORIA_INSUFICIENTE);
        exit(1);
    }
    criar_ibrand(ibrand, &nregistros);

    /****** icategory ******/ //todo
    Ir *icategory = (Ir*) malloc(MAX_REGISTROS * sizeof(Ir));
    if (!icategory) {
        perror(MEMORIA_INSUFICIENTE);
        exit(1);
    }
    criar_icategory(icategory, &nregistros);

    /****** iprice ******/ //todo
    Isf *iprice = (Isf*) malloc(MAX_REGISTROS * sizeof(Isf));
    if (!iprice) {
        perror(MEMORIA_INSUFICIENTE);
        exit(1);
    }
    criar_iprice(iprice, &nregistros);


	/* Execução do programa */
	int opcao = 0;
	while(1)
	{   
		scanf("%d%*c", &opcao);
		switch(opcao)
		{
			case INSERIR_NOVO_PRODUTO: // 1 //todo
				/*cadastro*/
                inserir(iprimary, iproduct, ibrand, icategory, iprice);

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
				printf(INICIO_BUSCA);

                int resultado = buscarProdutos(iprimary, iproduct, icategory, ibrand);
                if (resultado != -1) {
                    exibir_registro(resultado, 0);
                } else {
                    printf(REGISTRO_N_ENCONTRADO);
                }
				
			break;
			
            case LISTAR_PRODUTOS: // 5 //todo
				/*listagens*/
				printf(INICIO_LISTAGEM);

                nregistros = strlen(ARQUIVO) / TAM_REGISTRO;
                listarProdutos(iprimary, icategory, ibrand, iprice, nregistros);

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
                nregistros = strlen(ARQUIVO) / TAM_REGISTRO;
				ncat = NCAT;
				imprimirSecundario(iproduct, ibrand, icategory, iprice, nregistros, ncat);
			break;
			
            case FINALIZAR: // 9 //todo
	      		/*Liberar memória e finalizar o programa */
				return 0;
			break;

			//!DELETAR
			// Imprime indice primario 
			case 10: 
				printf("%d\n", NREGISTROS);
				for (int i = 0; i < NREGISTROS; i++)
					printf("%s %d\n", iprimary[i].pk, iprimary[i].rrn);
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

int nREG() {
    return strlen(ARQUIVO) / TAM_REGISTRO;
}


int carregar_arquivo()
{
	scanf("%[^\n]s", ARQUIVO);
	return strlen(ARQUIVO) / TAM_REGISTRO;
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

void gerarChave(Produto *P) {
    P->pk[0] = '\0';                // Garante que os dados serão concatenados corretamente na chave primária
    strncat(P->pk, P->nome, 2);     // N1N2 
    strncat(P->pk, P->marca, 2);    // M1M2

    char *dAux;                     // DDMM
    char dataAux[11];               // Cria uma string dataAux para não perder o valor da data original com o strtok
    strcpy(dataAux, P->data);
    dAux = strtok(dataAux, "/");    // DD
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
	gerarChave(&j);
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

int comparacao_iprimary_PK(const void *a, const void *b) {
    return strcmp((*(Ip*)a).pk, (*(Ip*)b).pk);
}
int comparacao_iproduct_NOME(const void *a, const void *b) {
	// Em caso de empate (nomes iguais), ordena pela PK
	if (strcmp((*(Is*)a).string, (*(Is*)b).string) == 0)
		return strcmp((*(Is*)a).pk, (*(Is*)b).pk);
    
	return strcmp((*(Is*)a).string, (*(Is*)b).string);
}
int comparacao_ibrand_MARCA(const void *a, const void *b) {
    return strcmp((*(Is*)a).string, (*(Is*)b).string);
}
int comparacao_icategory_CAT(const void *a, const void *b) {
    return strcmp((*(Ir*)a).cat, (*(Ir*)b).cat);
}
int comparacao_iprice_PRECO(const void *a, const void *b) {
    return (*(Isf*)a).price - (*(Isf*)b).price;
}

void criar_iprimary(Ip *indice_primario, int* nregistros) {

	if (NREGISTROS == 0)
		return;

	Produto J = recuperar_registro(NREGISTROS-1);
	strcpy(indice_primario[NREGISTROS-1].pk, J.pk);
	indice_primario[NREGISTROS-1].rrn = NREGISTROS-1;


    /* Ordenado pela chave primária */
    qsort(indice_primario, *nregistros, sizeof(Ip), comparacao_iprimary_PK);

}

void criar_iproduct(Is *indice_produto, int* nregistros) {

	if (NREGISTROS == 0)
		return;

	Produto J = recuperar_registro(NREGISTROS-1);
	strcpy(indice_produto[NREGISTROS-1].pk, J.pk);
	strcpy(indice_produto[NREGISTROS-1].string, J.nome);


    /* Ordenado pelo nome do produto ou modelo e, em caso de empate, pelo código */
    qsort(indice_produto, *nregistros, sizeof(Is), comparacao_iproduct_NOME);

}

void criar_ibrand(Is *indice_marca, int* nregistros) {

	if (NREGISTROS == 0)
		return;

	Produto J = recuperar_registro(NREGISTROS-1);
	strcpy(indice_marca[NREGISTROS-1].string, J.marca);
	strcpy(indice_marca[NREGISTROS-1].pk, J.pk);


    /* Ordenado pela marca e, em caso empate, pelo código */
    qsort(indice_marca, *nregistros, sizeof(Is), comparacao_ibrand_MARCA);

}

//todo
// Insere na lista ordenado
void inserir_lista(ll **primeiro, char *pk) {

	/* CASO LISTA VAZIA */
	if (*primeiro == NULL || strcmp((*primeiro)->pk, pk) > 0) {
		ll *novo = (ll*) malloc(sizeof(ll));
		strcpy(novo->pk, pk);
		novo->prox = *primeiro;
		*primeiro = novo;
		return;
	}

	ll *aux = *primeiro;
	
	// Percorre a lista até achar um valor maior
	while (aux->prox && strcmp(aux->prox->pk, pk) < 0)
		aux = aux->prox;

	// Se for igual, não insere repetido
	if (aux->prox != NULL && strcmp(aux->prox->pk, pk) == 0)
		return;

	ll *novo = (ll*) malloc(sizeof(ll));
	strcpy(novo->pk, pk);
	novo->prox = aux->prox;
	aux->prox = novo;

}

void criar_icategory(Ir *indice_categoria, int* nregistros) {

	if (NREGISTROS == 0)
		return;

	// Cada indice_categoria[i] tem uma categoria: indice_categoria[i].cat
	// E uma lista ligada para todos as chaves primárias que contém aquela categoria: indice_categoria[i].lista
    
	Produto J = recuperar_registro(NREGISTROS-1);
	
	
	char *cat;
	cat = strtok(J.categoria, "|");
	while (cat != NULL) {

		char categoria[TAM_CATEGORIA];
		strcpy(categoria, cat);

		// Verifica se a categoria já existe
		Ir *indiceCat = (Ir*) bsearch(categoria, indice_categoria, NCAT, sizeof(Ir), comparacao_icategory_CAT);
		if (indiceCat != NULL) {
			// Achou categoria
			// printf("ACHOU '%s'\n", categoria);
			int indiceBusca = indiceCat - indice_categoria;
			// printf("indiceBusca: %d\n", indiceBusca);
			inserir_lista(&(indice_categoria[indiceBusca].lista), J.pk);
		} else {
			// Não achou categoria
			// printf("NAO ACHOU '%s'\n", categoria);
			strcpy(indice_categoria[NCAT].cat, categoria);
			NCAT++;
			inserir_lista(&(indice_categoria[NCAT-1].lista), J.pk);


			/* Ordenado pelos nomes das categorias e em seguida pelo código */
			qsort(indice_categoria, NCAT, sizeof(Ir), comparacao_icategory_CAT);
		}


		// Vai para a proxima categoria
		cat = strtok(NULL, "|");
	}

	// /* Ordenado pelos nomes das categorias e em seguida pelo código */
	// qsort(indice_categoria, NCAT, sizeof(Ir), comparacao_icategory_CAT);


	// for (int i = 0; i < NCAT; i++) {
	// 	printf("%s %s\n", indice_categoria[i].cat, indice_categoria[i].lista->pk);
	// }

}

void criar_iprice(Isf *indice_preco, int* nregistros) {

	if (NREGISTROS == 0)
		return;

	Produto J = recuperar_registro(NREGISTROS-1);
	
	// printf("PK criar_iprice: %s\n", J.pk);
	strcpy(indice_preco[NREGISTROS-1].pk, J.pk);

	// Calculo do preço COM DESCONTO
	float preco = strtof(J.preco, NULL);
	// printf("preco: %f\n", preco);
	float precoArr = floor(10000 * preco) / 10000; //! undefined reference to `floor'
	// printf("preco arredondado: %f\n", precoArr);
	
	float desconto = strtof(J.desconto, NULL);
	// printf("desconto: %f\n", desconto);
	double precoComDesconto = preco * (100 - desconto) / 100;
	// printf("precoComDesconto: %f\n", precoComDesconto);
	indice_preco[NREGISTROS-1].price = precoComDesconto;


	/* Ordenado pelo primeiramente pelo preço em ordem ascendente e, em seguida, pelo código */
	qsort(indice_preco, *nregistros, sizeof(Isf), comparacao_iprice_PRECO);

}


/**********************************
    INTERAÇÃO COM O USUÁRIO
***********************************/


/**** INSERCAO ****/

void ler_entrada(char* registro, Produto *novo) {
    scanf("%[^\n]s", novo->nome);
    getchar();
    scanf("%[^\n]s", novo->marca);
    getchar();
    scanf("%[^\n]s", novo->data);
    getchar();
    scanf("%[^\n]s", novo->ano);
    getchar();
    scanf("%[^\n]s", novo->preco);
    getchar();
    scanf("%[^\n]s", novo->desconto);
    getchar();
    scanf("%[^\n]s", novo->categoria);
    getchar();
    
	sprintf(registro, "%s@%s@%s@%s@%s@%s@%s@", novo->nome, novo->marca, novo->data, novo->ano, novo->preco, novo->desconto, novo->categoria);
    
	// Completa os espaços restantes com '#'
    int necessarios = 192 - strlen(registro);
    for (int i = 0; i < necessarios; i++)
        strcat(registro, "#");
}
void inserir(Ip *iprimary, Is* iproduct, Is* ibrand, Ir* icategory, Isf *iprice) {

    char temp[193];
    Produto I;

    // Lê os dados e os coloca na string temp
    ler_entrada(temp, &I);
    gerarChave(&I);

    // Verifica se existe chave primária igual
    if (bsearch(I.pk, iprimary, NREGISTROS, sizeof(Ip), comparacao_iprimary_PK)) {
        printf(ERRO_PK_REPETIDA, I.pk);
        return;
    } else
        strcat(ARQUIVO, temp);


    int nreg = nREG();

	// Cria o índice primário
    criar_iprimary(iprimary, &nreg);

    // Cria o índice do produto
    criar_iproduct(iproduct, &nreg);

	// Cria o indice da marca
	criar_ibrand(ibrand, &nreg);

	// Cria o indice da categoria
	criar_icategory(icategory, &nreg); //todo

    // Cria o indice do preco
    criar_iprice(iprice, &nreg);

}


/**** BUSCAR PRODUTOS ****/

int buscarProdutos(Ip *iprimary, Is *iproduct, Ir *icategory, Is *ibrand) {

	int opcaoBusca;
	char chavePrimaria[TAM_PRIMARY_KEY];
    char nomeProduto[TAM_NOME];

	scanf("%d%*c", &opcaoBusca);
	switch (opcaoBusca) {

        // Busca por chave primária
		case 1:

			fgets(chavePrimaria, TAM_PRIMARY_KEY, stdin);
			Ip *indiceP = (Ip*) bsearch(chavePrimaria, iprimary, nREG(), sizeof(Ip), comparacao_iprimary_PK);
			if (indiceP != NULL) {
				return indiceP->rrn;
			} else
				return -1;
					
		break;

        // Busca por nome
		case 2: //todo

            scanf("%[^\n]s", nomeProduto);
			
			//!MUDAR PARA BSEARCH
			for (int i = 0; i < NREGISTROS; i++) {
				if (strcmp(iproduct[i].string, nomeProduto) == 0) {
					Ip *indicePri = (Ip*) bsearch(iproduct[i].pk, iprimary, NREGISTROS, sizeof(Ip), comparacao_iprimary_PK);
					return indicePri->rrn;
				}
			}
			return -1;

		break;

        // Busca por categoria e marca
		case 3: //todo

		break;

	}

}


/**** LISTAGEM ****/

void listarProdutos(Ip *iprimary, Ir *icategory, Is *ibrand, Isf *iprice, int nregistros) {

    int opcaoListagem;
	scanf("%d%*c", &opcaoListagem);


    switch (opcaoListagem) {

        // Listagem por código
        case 1:

            for (int i = 0; i < NREGISTROS; i++) {
                int RRN = iprimary[i].rrn;
                exibir_registro(RRN, 0);
                if (i != NREGISTROS-1)
                    printf("\n");
            }

        break;

        // Listagem por categoria
        case 2: //todo

        break;

        // Listagem por marca
        case 3: //todo

			for (int i = 0; i < NREGISTROS; i++) {

				Ip *indicePri = (Ip*) bsearch(ibrand[i].pk, iprimary, NREGISTROS, sizeof(Ip), comparacao_iprimary_PK);
				int RRN = indicePri->rrn;
				exibir_registro(RRN, 0);
                if (i != NREGISTROS-1)
                    printf("\n");

			}

        break;

        // Listagem por preço com desconto aplicado
        case 4: //todo

        break;

    }

}




/**** REFAZ TODOS OS INDICES ****/

void refaz_iprimary(Ip *indice_primario, int* nregistros) {

    for (int i = 0; i < *nregistros; i++) {
        indice_primario[i].rrn = i;
        Produto J = recuperar_registro(i);
        strcpy(indice_primario[i].pk, J.pk);
    }

    /* Ordenado pela chave primária */
    qsort(indice_primario, *nregistros, sizeof(Ip), comparacao_iprimary_PK);

}

void refaz_iproduct(Is *indice_produto, int* nregistros) {

    for (int i = 0; i < *nregistros; i++) {
        Produto J = recuperar_registro(i);
        strcpy(indice_produto[i].pk, J.pk);
        strcpy(indice_produto[i].string, J.nome);
    }

    /* Ordenado pelo nome do produto ou modelo e, em caso de empate, pelo código */
    qsort(indice_produto, *nregistros, sizeof(Is), comparacao_iproduct_NOME);

}

void refaz_ibrand(Is *indice_marca, int* nregistros) {

    for (int i = 0; i < *nregistros; i++) {
        Produto J = recuperar_registro(i);
        strcpy(indice_marca[i].string, J.marca);
        strcpy(indice_marca[i].pk, J.pk);
    }

    /* Ordenado pela marca e, em caso empate, pelo código */
    qsort(indice_marca, *nregistros, sizeof(Is), comparacao_ibrand_MARCA);

}

void refaz_icategory(Ir *indice_categoria, int* nregistros) {

    // Cada indice_categoria[i] tem uma categoria: indice_categoria[i].cat
	// E uma lista ligada para todos as chaves primárias que contém aquela categoria: indice_categoria[i].lista
	
	// Contador do vetor de categorias
	int j = 0;


	// Controla a iteracao entre os registros
    for (int i = 0; i < *nregistros; i++) {
        
		Produto J = recuperar_registro(i);
        
        
        char *cat;
        cat = strtok(J.categoria, "|");
        while (cat != NULL) {

            char categoria[TAM_CATEGORIA];
            strcpy(categoria, cat);
			
			//!MUDAR PARA BUSCA BINARIA
            // if (!existe_categoria(indice_categoria, categoria, j)) {
            //     printf("NAO ACHOU '%s'\n", cat);
			// 	strcpy(indice_categoria[j].cat, cat);
			// 	j++;
			// }

			Ir *indiceCat = (Ir*) bsearch(categoria, indice_categoria, j+1, sizeof(Ir), comparacao_icategory_CAT);
			if (indiceCat != NULL) {
				printf("ACHOU cat '%s'\n", indiceCat->cat);
			} else {
				printf("NAO ACHOU cat '%s'\n", categoria);
				printf("j antes ==== %d\n", j);
				strcpy(indice_categoria[j].cat, categoria);
				j++;
				printf("j depois ==== %d\n", j);
				qsort(indice_categoria, j, sizeof(Ir), comparacao_icategory_CAT);
			}



			// Vai para a proxima categoria
            cat = strtok(NULL, "|");
        }

		// qsort(indice_categoria, j, sizeof(Ir), comparacao_icategory_CAT);

    }

	for (int i = 0; i < j; i++)
		printf("%s\n", indice_categoria[i].cat);

	NCAT = j;

}

void refaz_iprice(Isf *indice_preco, int* nregistros) {



}
