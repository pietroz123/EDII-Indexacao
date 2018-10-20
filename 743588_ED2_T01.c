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
// #include <math.h>
 
 
/* Tamanho dos campos dos registros */
#define TAM_PRIMARY_KEY    11
#define TAM_NOME         51
#define TAM_MARCA         51
#define TAM_DATA         11
#define TAM_ANO         3
#define TAM_PRECO         8
#define TAM_DESCONTO     4
#define TAM_CATEGORIA     51
 
 
#define TAM_REGISTRO     192
#define MAX_REGISTROS     1000
#define MAX_CATEGORIAS     30
#define TAM_ARQUIVO (MAX_REGISTROS * TAM_REGISTRO + 1)
 
 
/* Saídas para o usuario */
#define OPCAO_INVALIDA                 "Opcao invalida!\n"
#define MEMORIA_INSUFICIENTE         "Memoria insuficiente!"
#define REGISTRO_N_ENCONTRADO         "Registro(s) nao encontrado!\n"
#define CAMPO_INVALIDO                 "Campo invalido! Informe novamente.\n"
#define ERRO_PK_REPETIDA            "ERRO: Ja existe um registro com a chave primaria: %s.\n"
#define ARQUIVO_VAZIO                 "Arquivo vazio!"
#define INICIO_BUSCA                  "**********************BUSCAR**********************\n"
#define INICIO_LISTAGEM              "**********************LISTAR**********************\n"
#define INICIO_ALTERACAO             "**********************ALTERAR*********************\n"
#define INICIO_EXCLUSAO              "**********************EXCLUIR*********************\n"
#define INICIO_ARQUIVO              "**********************ARQUIVO*********************\n"
#define INICIO_ARQUIVO_SECUNDARIO    "*****************ARQUIVO SECUNDARIO****************\n"
#define SUCESSO                       "OPERACAO REALIZADA COM SUCESSO!\n"
#define FALHA                          "FALHA AO REALIZAR OPERACAO!\n"
 
/* Opções Menu */
#define INSERIR_NOVO_PRODUTO            1
#define MODIFICAR_DESCONTO              2
#define REMOVER_PRODUTO                 3
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
    char data[TAM_DATA];                /* Data de Registro do Produto: DD/MM/AAAA */
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
void inserir(Ip *iprimary, Is* iproduct, Is* ibrand, Ir* icategory, Isf *iprice, int nregistros);
void inserir_lista(ll **primeiro, char *pk);
 
// (2) ALTERAÇÃO
//todo
void alterar(int rrn, char *novoDesconto, Isf *iprice);
 
// (3) REMOÇÃO
//todo
 
// (4) BUSCAR PRODUTOS - Busca pelo produto e retorna o RRN
void buscarProdutos(Ip *iprimary, Is *iproduct, Ir *icategory, Is *ibrand, int nregistros);
int bSearch(Is *a, int inicio, int fim, char chave[]);
int bSearchInferior(Is *a, int inicio, int fim, char chave[]);
int bSearchSuperior(Is *a, int inicio, int fim, char chave[]);
int buscar_lista(ll **primeiro, char *pk);
 
 
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
int comparacao_iprimary_PK(const void *a, const void *b);
int comparacao_iproduct_NOME(const void *a, const void *b);
int comparacao_ibrand_MARCA(const void *a, const void *b);
int comparacao_icategory_CAT(const void *a, const void *b);
int comparacao_iprice_PRECO(const void *a, const void *b);
 
 
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
    refaz_iprimary(iprimary, &nregistros);
 
 
    /* Alocar e criar índices secundários */ //todo
 
    /****** iproduct ******/
    Is *iproduct = (Is*) malloc(MAX_REGISTROS * sizeof(Is));
    if (!iproduct) {
        perror(MEMORIA_INSUFICIENTE);
        exit(1);
    }
    refaz_iproduct(iproduct, &nregistros);
 
    /****** ibrand ******/
    Is *ibrand = (Is*) malloc(MAX_REGISTROS * sizeof(Is));
    if (!ibrand) {
        perror(MEMORIA_INSUFICIENTE);
        exit(1);
    }
    refaz_ibrand(ibrand, &nregistros);
 
    /****** icategory ******/
    Ir *icategory = (Ir*) malloc(MAX_REGISTROS * sizeof(Ir));
    if (!icategory) {
        perror(MEMORIA_INSUFICIENTE);
        exit(1);
    }
    refaz_icategory(icategory, &nregistros); //todo
 
    /****** iprice ******/
    Isf *iprice = (Isf*) malloc(MAX_REGISTROS * sizeof(Isf));
    if (!iprice) {
        perror(MEMORIA_INSUFICIENTE);
        exit(1);
    }
    refaz_iprice(iprice, &nregistros);
 
 
    /* Execução do programa */
    int opcao = 0;
    while(1)
    {   
        scanf("%d%*c", &opcao);
        switch(opcao)
        {
            case INSERIR_NOVO_PRODUTO: // 1
                /*cadastro*/
                nregistros = NREGISTROS;
                inserir(iprimary, iproduct, ibrand, icategory, iprice, nregistros);
 
            break;
 
            
            case MODIFICAR_DESCONTO: // 2 //todo
                /*alterar desconto*/
                printf(INICIO_ALTERACAO);
                
                char pk[TAM_PRIMARY_KEY];
                scanf("%[^\n]s", pk);
 
                // Busca se existe a chave primária
                Ip *indicePri = (Ip*) bsearch(pk, iprimary, NREGISTROS, sizeof(Ip), comparacao_iprimary_PK);
                if (indicePri == NULL) {
                    printf(REGISTRO_N_ENCONTRADO);
                    printf(FALHA);
                    break;
                }
 
                char novoDesconto[TAM_DESCONTO];
                getchar();
                scanf("%[^\n]s", novoDesconto);
 
                // Verificar se o novo desconto é válido    
                while (strcmp(novoDesconto, "100") >= 0 || strcmp(novoDesconto, "000") <= 0) {
                    printf(CAMPO_INVALIDO);
                    getchar();
                    scanf("%[^\n]s", novoDesconto);
                }
 
 
                alterar(indicePri->rrn, novoDesconto, iprice);
                printf(SUCESSO);
                
            break;
            
            case REMOVER_PRODUTO: // 3 //todo
                /*excluir produto*/
                printf(INICIO_EXCLUSAO);
                
                // if(remover([args]))
                //     printf(SUCESSO);
                // else
                //     printf(FALHA);
                
            break;
 
            case BUSCAR_PRODUTOS: // 4
                /*busca*/
                printf(INICIO_BUSCA);
                nregistros = NREGISTROS;
                buscarProdutos(iprimary, iproduct, icategory, ibrand, nregistros);
                
            break;
            
            case LISTAR_PRODUTOS: // 5
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
    // Em caso de empate (marcas iguais), ordena pela PK
    if (strcmp((*(Is*)a).string, (*(Is*)b).string) == 0)
        return strcmp((*(Is*)a).pk, (*(Is*)b).pk);
 
    return strcmp((*(Is*)a).string, (*(Is*)b).string);
}
int comparacao_icategory_CAT(const void *a, const void *b) {
    return strcmp((*(Ir*)a).cat, (*(Ir*)b).cat);
}
int comparacao_iprice_PRECO(const void *a, const void *b) {
    // Em caso de empate (preços iguais), ordena pela PK
    if ((*(Isf*)a).price == (*(Isf*)b).price)
        return strcmp((*(Is*)a).pk, (*(Is*)b).pk);
 
    if ((*(Isf*)a).price < (*(Isf*)b).price)
        return -1;
    else if ((*(Isf*)a).price > (*(Isf*)b).price)
        return 1;
    else if ((*(Isf*)a).price == (*(Isf*)b).price)
        return 0;
}
 
void criar_iprimary(Ip *indice_primario, int* nregistros) {
 
    if (*nregistros == 0)
        return;
 
    Produto J = recuperar_registro(*nregistros-1);
    strcpy(indice_primario[*nregistros-1].pk, J.pk);
    indice_primario[*nregistros-1].rrn = *nregistros-1;
 
 
    /* Ordenado pela chave primária */
    qsort(indice_primario, *nregistros, sizeof(Ip), comparacao_iprimary_PK);
 
}
 
void criar_iproduct(Is *indice_produto, int* nregistros) {
 
    if (*nregistros == 0)
        return;
 
    Produto J = recuperar_registro(*nregistros-1);
    strcpy(indice_produto[*nregistros-1].pk, J.pk);
    strcpy(indice_produto[*nregistros-1].string, J.nome);
 
 
    /* Ordenado pelo nome do produto ou modelo e, em caso de empate, pelo código */
    qsort(indice_produto, *nregistros, sizeof(Is), comparacao_iproduct_NOME);
 
}
 
void criar_ibrand(Is *indice_marca, int* nregistros) {
 
    if (*nregistros == 0)
        return;
 
    Produto J = recuperar_registro(*nregistros-1);
    strcpy(indice_marca[*nregistros-1].string, J.marca);
    strcpy(indice_marca[*nregistros-1].pk, J.pk);
 
 
    /* Ordenado pela marca e, em caso empate, pelo código */
    qsort(indice_marca, *nregistros, sizeof(Is), comparacao_ibrand_MARCA);
 
}
 
void criar_icategory(Ir *indice_categoria, int* nregistros) {
 
    if (*nregistros == 0)
        return;
 
    // Cada indice_categoria[i] tem uma categoria: indice_categoria[i].cat
    // E uma lista ligada para todos as chaves primárias que contém aquela categoria: indice_categoria[i].lista
    
    Produto J = recuperar_registro(*nregistros-1);
    
    
    char *cat;
    cat = strtok(J.categoria, "|");
    while (cat != NULL) {
 
        char categoria[TAM_CATEGORIA];
        strcpy(categoria, cat);
 
        // Verifica se a categoria já existe
        Ir *indiceCat = (Ir*) bsearch(categoria, indice_categoria, NCAT, sizeof(Ir), comparacao_icategory_CAT);
        if (indiceCat != NULL) {
            // Achou categoria
            int indiceBusca = indiceCat - indice_categoria;
            inserir_lista(&(indice_categoria[indiceBusca].lista), J.pk);
        } else {
            // Não achou categoria
            strcpy(indice_categoria[NCAT].cat, categoria);
            NCAT++;
            inserir_lista(&(indice_categoria[NCAT-1].lista), J.pk);
 
 
            /* Ordenado pelos nomes das categorias e em seguida pelo código */
            qsort(indice_categoria, NCAT, sizeof(Ir), comparacao_icategory_CAT);
        }
 
 
        // Vai para a proxima categoria
        cat = strtok(NULL, "|");
    }
 
}
 
void criar_iprice(Isf *indice_preco, int* nregistros) {
 
    if (*nregistros == 0)
        return;
 
    Produto J = recuperar_registro(*nregistros-1);
    
    float preco;
    int desconto;
 
    strcpy(indice_preco[*nregistros-1].pk, J.pk);
 
    sscanf(J.desconto, "%d", &desconto);
    sscanf(J.preco, "%f", &preco);
 
    // Calculo do preço COM DESCONTO
    preco = preco * (100 - desconto);
    preco = ((int) preco) / (float) 100;
 
    indice_preco[*nregistros-1].price = preco;
 
 
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
void inserir(Ip *iprimary, Is* iproduct, Is* ibrand, Ir* icategory, Isf *iprice, int nregistros) {
 
    char temp[193];
    Produto I;
 
    // Lê os dados e os coloca na string temp
    ler_entrada(temp, &I);
    gerarChave(&I);
 
    // Verifica se existe chave primária igual
    if (bsearch(I.pk, iprimary, nregistros, sizeof(Ip), comparacao_iprimary_PK)) {
        printf(ERRO_PK_REPETIDA, I.pk);
        return;
    } else
        strcat(ARQUIVO, temp);
    
    nregistros++;
 
 
    // Cria o índice primário
    criar_iprimary(iprimary, &nregistros);
 
    // Cria o índice do produto
    criar_iproduct(iproduct, &nregistros);
 
    // Cria o indice da marca
    criar_ibrand(ibrand, &nregistros);
 
    // Cria o indice da categoria
    criar_icategory(icategory, &nregistros);
 
    // Cria o indice do preco
    criar_iprice(iprice, &nregistros);
 
}
 
 
/**** BUSCAR PRODUTOS ****/
 
int bSearch(Is *a, int inicio, int fim, char chave[]) {
 
    if (inicio > fim)
        return -1;
 
    int meio = (inicio + fim) / 2;
    if (strcmp(a[meio].string, chave) == 0)
        return meio;
    else {
        if (strcmp(a[meio].string, chave) > 0)
            meio = bSearch(a, inicio, meio-1, chave);
        else
            meio = bSearch(a, meio+1, fim, chave);
    }
    return meio;
    
}
 
int bSearchInferior(Is *a, int inicio, int fim, char chave[]) {
 
    if (fim < inicio)
        return inicio;
 
    int meio = (inicio + fim) / 2;
    
    
    if (strcmp(a[meio].string, chave) >= 0)
        meio = bSearchInferior(a, inicio, meio-1, chave);
    else
        meio = bSearchInferior(a, meio+1, fim, chave);
    
    return meio;
 
}
int bSearchSuperior(Is *a, int inicio, int fim, char chave[]) {
 
    if (fim < inicio)
        return fim;
 
    int meio = (inicio + fim) / 2;
    
    
    if (strcmp(a[meio].string, chave) > 0)
        meio = bSearchSuperior(a, inicio, meio-1, chave);
    else
        meio = bSearchSuperior(a, meio+1, fim, chave);
    
    return meio;
 
}
 
 
void buscarProdutos(Ip *iprimary, Is *iproduct, Ir *icategory, Is *ibrand, int nregistros) {
 
    int opcaoBusca;
    char chavePrimaria[TAM_PRIMARY_KEY];
    char nomeProduto[TAM_NOME];
    char marcaProduto[TAM_MARCA];
    char categoriaProduto[TAM_CATEGORIA];
    Ip *indicePri;
    Is *indiceProd;
    Is *indiceMarca;
    Ir *indiceCat;
 
    int indiceInferior, indiceSuperior, indiceBsearch;
 
    scanf("%d%*c", &opcaoBusca);
    switch (opcaoBusca) {
 
        // Busca por chave primária
        case 1:
 
            fgets(chavePrimaria, TAM_PRIMARY_KEY, stdin);
            indicePri = (Ip*) bsearch(chavePrimaria, iprimary, NREGISTROS, sizeof(Ip), comparacao_iprimary_PK);
            if (indicePri != NULL) {
                exibir_registro(indicePri->rrn, 0);
            } else {
                printf(REGISTRO_N_ENCONTRADO);
                return;
            }    
 
        break;
 
        // Busca por nome
        case 2:
 
            scanf("%[^\n]s", nomeProduto);
            getchar();
 
            indiceBsearch = bSearch(iproduct, 0, nregistros-1, nomeProduto);
            if (indiceBsearch != -1) {
                indiceInferior = bSearchInferior(iproduct, 0, nregistros-1, nomeProduto);
                indiceSuperior = bSearchSuperior(iproduct, 0, nregistros-1, nomeProduto);
            } else {
                printf(REGISTRO_N_ENCONTRADO);
                return;
            }
 
            // printf("inferior: %d\nsuperior: %d\n", indiceInferior, indiceSuperior); //!
 
 
            // Busca pelo RRN correspondente a partir da chave primária
            for (int i = indiceInferior; i <= indiceSuperior; i++) {
                indicePri = (Ip*) bsearch(iproduct[i].pk, iprimary, nregistros, sizeof(Ip), comparacao_iprimary_PK);
                if (indicePri) {
                    int RRN = indicePri->rrn;
                    exibir_registro(RRN, 0);
                    if (i != indiceSuperior)
                        printf("\n");
                }
            }
 
        break;
 
        // Busca por categoria e marca
        case 3: //todo
 
            // Busca pelo Produto de uma marca que está em uma categoria
 
            scanf("%[^\n]s", marcaProduto);
            getchar();
            scanf("%[^\n]s", categoriaProduto);
 
 
            int indiceBsearch = bSearch(ibrand, 0, nregistros-1, marcaProduto);
            if (indiceBsearch != -1) {
                indiceInferior = bSearchInferior(ibrand, 0, nregistros, marcaProduto);
                indiceSuperior = bSearchSuperior(ibrand, 0, nregistros, marcaProduto);
            } else {
                printf(REGISTRO_N_ENCONTRADO);
                return;
            }
 
            // printf("indiceInferior: %d\nindiceSuperior: %d\n", indiceInferior, indiceSuperior); //!
 
            indiceCat = (Ir*) bsearch(categoriaProduto, icategory, NCAT, sizeof(Ir), comparacao_icategory_CAT);
            if (indiceCat == NULL) {
                printf(REGISTRO_N_ENCONTRADO);
                return;
            }
 
            for (int i = indiceInferior; i <= indiceSuperior; i++) {
 
                strcpy(chavePrimaria, ibrand[i].pk);
                indicePri = (Ip*) bsearch(chavePrimaria, iprimary, nregistros, sizeof(Ip), comparacao_iprimary_PK);
                int resBuscaLista = buscar_lista(&(indiceCat->lista), chavePrimaria);
                if (resBuscaLista != -1) {
                    exibir_registro(indicePri->rrn, 0);
                    if (i != indiceSuperior)
                        printf("\n");
                }
 
            }
 
        break;
 
    }
 
}
 
 
/**** LISTAGEM ****/
 
void listarProdutos(Ip *iprimary, Ir *icategory, Is *ibrand, Isf *iprice, int nregistros) {
 
    char categoriaProduto[TAM_CATEGORIA];
 
    Ip *indicePri;
    Ir *indiceCat;
 
    int opcaoListagem;
    scanf("%d%*c", &opcaoListagem);
 
    if (nregistros == 0) {
        printf(ARQUIVO_VAZIO);
        return;
    }
 
    switch (opcaoListagem) {
 
        // Listagem por código
        case 1:
 
            for (int i = 0; i < nregistros; i++) {
                int RRN = iprimary[i].rrn;
                exibir_registro(RRN, 0);
                if (i != nregistros-1)
                    printf("\n");
            }
 
        break;
 
        // Listagem por categoria
        case 2:
 
            scanf("%[^\n]s", categoriaProduto);
 
            indiceCat = (Ir*) bsearch(categoriaProduto, icategory, NCAT, sizeof(Ir), comparacao_icategory_CAT);
            if (indiceCat) {
                ll *aux = indiceCat->lista;
                while (aux) {
                    indicePri = (Ip*) bsearch(aux->pk, iprimary, nregistros, sizeof(Ip), comparacao_iprimary_PK);
                    exibir_registro(indicePri->rrn, 0);
                    if (aux->prox)
                        printf("\n");
                    aux = aux->prox;
                }
            }
 
        break;
 
        // Listagem por marca
        case 3:
 
            for (int i = 0; i < nregistros; i++) {
 
                indicePri = (Ip*) bsearch(ibrand[i].pk, iprimary, nregistros, sizeof(Ip), comparacao_iprimary_PK);
                int RRN = indicePri->rrn;
                exibir_registro(RRN, 0);
                if (i != nregistros-1)
                    printf("\n");
 
            }
 
        break;
 
        // Listagem por preço com desconto aplicado
        case 4:
 
            for (int i = 0; i < nregistros; i++) {
 
                indicePri = (Ip*) bsearch(iprice[i].pk, iprimary, nregistros, sizeof(Ip), comparacao_iprimary_PK);
                int RRN = indicePri->rrn;
                exibir_registro(RRN, 1);
                if (i != nregistros-1)
                    printf("\n");
 
            }
 
        break;
 
    }
 
}
 
 
/**** ALTERAÇÃO DO DESCONTO ****/
 
int comparacao_iprice_PK(const void *a, const void *b) {
    return strcmp((*(Isf*)a).pk, (*(Isf*)b).pk);
}
 
void alterar(int rrn, char *novoDesconto, Isf *iprice) {
 
    char *p = ARQUIVO + 192*rrn;
    // printf("*p: %s\n", p); //!
 
 
    int i = 0; 
    int arr = 0;
    while (*p && arr < 5) {
        // printf("valor de p: %c\n", *p); //!
        if (*p == '@')
            arr++;
        p++;
    }
 
    // printf("valor final de p: %c\n", *p); //!
    
    // Altera no ARQUIVO de dados
     *p = novoDesconto[0];
    p++;
    *p = novoDesconto[1];
    p++;
    *p = novoDesconto[2];
 
 
    // Altera em iprice
    Produto J = recuperar_registro(rrn);
 
    for (int i = 0; i < NREGISTROS; i++) {
        if (strcmp(J.pk, iprice[i].pk) == 0) {
            float preco;
            int desconto;
    
            sscanf(novoDesconto, "%d", &desconto);
            sscanf(J.preco, "%f", &preco);
    
            // Calculo do preço COM DESCONTO
            preco = preco * (100 - desconto);
            preco = ((int) preco) / (float) 100;
    
            iprice[i].price = preco;
 
            break;
        }
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
 
    for (int i = 0; i < *nregistros; i++) {
 
        Produto J = recuperar_registro(i);
    
        char *cat;
        cat = strtok(J.categoria, "|");
        while (cat != NULL) {
    
            char categoria[TAM_CATEGORIA];
            strcpy(categoria, cat);
    
            // Verifica se a categoria já existe
            Ir *indiceCat = (Ir*) bsearch(categoria, indice_categoria, NCAT, sizeof(Ir), comparacao_icategory_CAT);
            if (indiceCat != NULL) {
                // Achou categoria
                int indiceBusca = indiceCat - indice_categoria;
                inserir_lista(&(indice_categoria[indiceBusca].lista), J.pk);
            } else {
                // Não achou categoria
                strcpy(indice_categoria[NCAT].cat, categoria);
                NCAT++;
                inserir_lista(&(indice_categoria[NCAT-1].lista), J.pk);
    
    
                /* Ordenado pelos nomes das categorias e em seguida pelo código */
                qsort(indice_categoria, NCAT, sizeof(Ir), comparacao_icategory_CAT);
            }
    
    
            // Vai para a proxima categoria
            cat = strtok(NULL, "|");
        }
 
 
    }
 
 
}
 
void refaz_iprice(Isf *indice_preco, int* nregistros) {
 
    for (int i = 0; i < *nregistros; i++) {
 
        Produto J = recuperar_registro(i);        
 
        float preco;
        int desconto;
 
        strcpy(indice_preco[i].pk, J.pk);
 
        sscanf(J.desconto, "%d", &desconto);
        sscanf(J.preco, "%f", &preco);
 
        // Calculo do preço COM DESCONTO
        preco = preco * (100 - desconto);
        preco = ((int) preco) / (float) 100;
 
        indice_preco[i].price = preco;
 
    }
 
    /* Ordenado pelo preço e, em caso empate, pelo código */
    qsort(indice_preco, *nregistros, sizeof(Isf), comparacao_iprice_PRECO);
 
}
 
 
/**** FUNÇÕES DA LISTA ENCADEADA ****/
 
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
 
// Busca por uma chave na lista
int buscar_lista(ll **primeiro, char *pk) {
 
    ll *aux = *primeiro;
    while (aux) {
        if (strcmp(aux->pk, pk) == 0)
            return 1;
        aux = aux->prox;
    }
    return -1;
 
 
}