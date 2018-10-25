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
#define ARQUIVO_VAZIO                 "Arquivo vazio!\n"
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
 
/* Struct para lista invertida */
typedef struct reverse_index{
  char cat[TAM_CATEGORIA];
  ll* lista;
} Ir;
 
/*----- GLOBAL -----*/
char ARQUIVO[TAM_ARQUIVO];
 
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
void insere_iprimary(Ip *indice_primario, int* nregistros);
void insere_iproduct(Is *indice_produto, int* nregistros);
void insere_ibrand(Is *indice_marca, int* nregistros);
void insere_icategory(Ir *indice_categoria, int* nregistros, int *ncat);
void insere_iprice(Isf *indice_preco, int* nregistros);
 
void criar_iprimary(Ip *indice_primario, int* nregistros);
void criar_iproduct(Is *indice_produto, int* nregistros);
void criar_ibrand(Is *indice_marca, int* nregistros);
void criar_icategory(Ir *indice_categoria, int* nregistros, int *ncat);
void criar_iprice(Isf *indice_preco, int* nregistros);   
 
 
/* Realiza os scanfs na struct Produto */
void ler_entrada(char* registro, Produto *novo);
 
/* Rotina para impressao de indice secundario */
void imprimir_secundario(Is* iproduct, Is* ibrand, Ir* icategory, Isf *iprice, int nregistros, int ncat);
 
 
/****** INTERAÇÃO COM O USUÁRIO ******/
 
// (1) INSERCAO 
void inserir(Ip *iprimary, Is* iproduct, Is* ibrand, Ir* icategory, Isf *iprice, int *nreg, int *ncat);
void inserir_lista(ll **primeiro, char *pk);
 
// (2) ALTERAÇÃO
void alterar(int rrn, char *novoDesconto, Isf *iprice, int nregistros);
 
// (3) REMOÇÃO
void remover(Ip *indicePri, Ip *iprimary);
 
// (4) BUSCAR PRODUTOS - Busca pelo produto e retorna o RRN
void buscar_produtos(Ip *iprimary, Is *iproduct, Ir *icategory, Is *ibrand, int nregistros, int ncat);
int bSearch(Is *a, int inicio, int fim, char chave[]);
int bsearch_inferior(Is *a, int inicio, int fim, char chave[]);
int bsearch_superior(Is *a, int inicio, int fim, char chave[]);
int buscar_lista(ll **primeiro, char *pk);
 
 
// (5) LISTAGEM
void listar_produtos(Ip *iprimary, Ir *icategory, Is *ibrand, Isf *iprice, int nregistros, int ncat);
 
// (6) LIBERAR ESPAÇO
int liberar_espaco(int *nregistros);
 
// (7) IMPRIMIR ARQUIVO DE DADOS
//!Já implementado
 
// (8) IMPRIMIR INDICES SECUNDARIOS
//!Já implementado
 
// (9) FINALIZAR
void desalocar_estruturas(Ip *iprimary, Is* iproduct, Is* ibrand, Ir* icategory, Isf *iprice, int *ncat);
void liberar_lista(ll **primeiro);
 
 
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
    char pk[TAM_PRIMARY_KEY];
    Ip *indicePri;
 
 
    scanf("%d%*c", &carregarArquivo); /* 1 (sim) | 0 (nao) */
    if (carregarArquivo)
        nregistros = carregar_arquivo();
 
 
    /* Índice primário */
    Ip *iprimary = (Ip *) malloc (MAX_REGISTROS * sizeof(Ip));
      if (!iprimary) {
        perror(MEMORIA_INSUFICIENTE);
        exit(1);
    }
    criar_iprimary(iprimary, &nregistros);
 
 
    /* Alocar e criar índices secundários */
 
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
 
    /****** icategory ******/
    Ir *icategory = (Ir*) malloc(MAX_CATEGORIAS * sizeof(Ir));
    if (!icategory) {
        perror(MEMORIA_INSUFICIENTE);
        exit(1);
    }
    criar_icategory(icategory, &nregistros, &ncat);
 
    /****** iprice ******/
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
            case INSERIR_NOVO_PRODUTO: // 1
                /*cadastro*/
                inserir(iprimary, iproduct, ibrand, icategory, iprice, &nregistros, &ncat);
            
            break;
 
            
            case MODIFICAR_DESCONTO: // 2
                /*alterar desconto*/
                printf(INICIO_ALTERACAO);
                
                scanf("%[^\n]s", pk);
 
                // Busca se existe a chave primária
                indicePri = (Ip*) bsearch(pk, iprimary, NREGISTROS, sizeof(Ip), comparacao_iprimary_PK);
                if (indicePri == NULL) {
                    printf(REGISTRO_N_ENCONTRADO);
                    printf(FALHA);
                    break;
                }
 
                char novoDesconto[TAM_DESCONTO];
                getchar();
                scanf("%[^\n]s", novoDesconto);
 
                // Verificar se o novo desconto é válido    
                while (strcmp(novoDesconto, "100") > 0 || strcmp(novoDesconto, "000") <= 0) {
                    printf(CAMPO_INVALIDO);
                    getchar();
                    scanf("%[^\n]s", novoDesconto);
                }
 
 
                alterar(indicePri->rrn, novoDesconto, iprice, nregistros);
                printf(SUCESSO);
                
            break;
            
            case REMOVER_PRODUTO: // 3
                /*excluir produto*/
                printf(INICIO_EXCLUSAO);
 
                scanf("%[^\n]s", pk);
                getchar();
 
                // Busca se existe a chave primária
                indicePri = (Ip*) bsearch(pk, iprimary, NREGISTROS, sizeof(Ip), comparacao_iprimary_PK);
                if (indicePri == NULL || indicePri->rrn == -1) {
                    printf(REGISTRO_N_ENCONTRADO);
                    printf(FALHA);
                    break;
                }
 
                remover(indicePri, iprimary);
                printf(SUCESSO);
                
            break;
 
            case BUSCAR_PRODUTOS: // 4
                /*busca*/
                printf(INICIO_BUSCA);
                buscar_produtos(iprimary, iproduct, icategory, ibrand, nregistros, ncat);
                
            break;
            
            case LISTAR_PRODUTOS: // 5
                /*listagens*/
                printf(INICIO_LISTAGEM);
                listar_produtos(iprimary, icategory, ibrand, iprice, nregistros, ncat);
 
            break;
            
            case LIBERAR_ESPACO: // 6
                /*libera espaço*/
                nregistros = liberar_espaco(&nregistros);
                desalocar_estruturas(iprimary, iproduct, ibrand, icategory, iprice, &ncat);

                ncat = 0;

                iprimary = (Ip *) malloc (MAX_REGISTROS * sizeof(Ip));
                if (!iprimary) {
                    perror(MEMORIA_INSUFICIENTE);
                    exit(1);
                }
                criar_iprimary(iprimary, &nregistros);
            
                iproduct = (Is*) malloc(MAX_REGISTROS * sizeof(Is));
                if (!iproduct) {
                    perror(MEMORIA_INSUFICIENTE);
                    exit(1);
                }
                criar_iproduct(iproduct, &nregistros);
            
                ibrand = (Is*) malloc(MAX_REGISTROS * sizeof(Is));
                if (!ibrand) {
                    perror(MEMORIA_INSUFICIENTE);
                    exit(1);
                }
                criar_ibrand(ibrand, &nregistros);
            
                icategory = (Ir*) malloc(MAX_CATEGORIAS * sizeof(Ir));
                if (!icategory) {
                    perror(MEMORIA_INSUFICIENTE);
                    exit(1);
                }
                criar_icategory(icategory, &nregistros, &ncat);
            
                iprice = (Isf*) malloc(MAX_REGISTROS * sizeof(Isf));
                if (!iprice) {
                    perror(MEMORIA_INSUFICIENTE);
                    exit(1);
                }
                criar_iprice(iprice, &nregistros);



            break;
            
            case IMPRIMIR_DADOS: // 7
                /*imprime o arquivo de dados*/
                if (strlen(ARQUIVO) == 0)
                    printf(ARQUIVO_VAZIO);
                else {
                    printf(INICIO_ARQUIVO);
                    printf("%s\n", ARQUIVO);
                }

            break;
            
            case IMPRIMIR_INDICES_SECUNDARIOS: // 8
                /*imprime os índices secundários*/
                imprimir_secundario(iproduct, ibrand, icategory, iprice, nregistros, ncat);

            break;
            
            case FINALIZAR: // 9
                /*Liberar memória e finalizar o programa */
                desalocar_estruturas(iprimary, iproduct, ibrand, icategory, iprice, &ncat);

                return 0;
            break;
 
            //!DELETAR
            // Imprime indice primario 
            case 11: 
                printf("%lu\n", nregistros);
                for (int i = 0; i < nregistros; i++)
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
    scanf("%[^\n]%*c", ARQUIVO);
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
void imprimir_secundario(Is* iproduct, Is* ibrand, Ir* icategory, Isf *iprice, int nregistros, int ncat){
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
    if (strcmp((*(Ip*)a).pk, (*(Ip*)b).pk) == 0) 
        return 0;
    else if (strcmp((*(Ip*)a).pk, (*(Ip*)b).pk) > 0)
        return 1;
    else
        return -1;
}
int comparacao_iproduct_NOME(const void *a, const void *b) {
    // Em caso de empate (nomes iguais), ordena pela PK
    if (strcmp((*(Is*)a).string, (*(Is*)b).string) == 0) {
        if (strcmp((*(Is*)a).pk, (*(Is*)b).pk) == 0)
            return 0;
        else if (strcmp((*(Is*)a).pk, (*(Is*)b).pk) > 0)
            return 1;
        else
            return -1;
    }
 
    if (strcmp((*(Is*)a).string, (*(Is*)b).string) == 0)
        return 0;
    else if (strcmp((*(Is*)a).string, (*(Is*)b).string) > 0)
        return 1;
    else
        return -1;
}
int comparacao_ibrand_MARCA(const void *a, const void *b) {
    // Em caso de empate (marcas iguais), ordena pela PK
    if (strcmp((*(Is*)a).string, (*(Is*)b).string) == 0) {
        if (strcmp((*(Is*)a).pk, (*(Is*)b).pk) == 0)
            return 0;
        else if (strcmp((*(Is*)a).pk, (*(Is*)b).pk) > 0)
            return 1;
        else
            return -1;
    }
 
    if (strcmp((*(Is*)a).string, (*(Is*)b).string) == 0)
        return 0;
    else if (strcmp((*(Is*)a).string, (*(Is*)b).string) > 0)
        return 1;
    else
        return -1;
}
int comparacao_icategory_CAT(const void *a, const void *b) {
    if (strcmp((*(Ir*)a).cat, (*(Ir*)b).cat) == 0)
        return 0;
    else if (strcmp((*(Ir*)a).cat, (*(Ir*)b).cat) > 0)
        return 1;
    else
        return -1;
}
int comparacao_iprice_PRECO(const void *a, const void *b) {
    // Em caso de empate (preços iguais), ordena pela PK
    if ((*(Isf*)a).price == (*(Isf*)b).price) {
        if (strcmp((*(Isf*)a).pk , (*(Isf*)b).pk) == 0)
            return 0;
        else if (strcmp((*(Isf*)a).pk , (*(Isf*)b).pk) > 0)
            return 1;
        else
            return -1;
    }
 
    if ((*(Isf*)a).price < (*(Isf*)b).price)
        return -1;
    else if ((*(Isf*)a).price > (*(Isf*)b).price)
        return 1;
    else if ((*(Isf*)a).price == (*(Isf*)b).price)
        return 0;
}
 
void insere_iprimary(Ip *indice_primario, int* nregistros) {
 
    if (*nregistros == 0)
        return;
 
    Produto J = recuperar_registro(*nregistros-1);
    strcpy(indice_primario[*nregistros-1].pk, J.pk);
    indice_primario[*nregistros-1].rrn = *nregistros-1;
 
 
    /* Ordenado pela chave primária */
    qsort(indice_primario, *nregistros, sizeof(Ip), comparacao_iprimary_PK);
 
}
 
void insere_iproduct(Is *indice_produto, int* nregistros) {
 
    if (*nregistros == 0)
        return;
 
    Produto J = recuperar_registro(*nregistros-1);
    strcpy(indice_produto[*nregistros-1].pk, J.pk);
    strcpy(indice_produto[*nregistros-1].string, J.nome);
 
 
    /* Ordenado pelo nome do produto ou modelo e, em caso de empate, pelo código */
    qsort(indice_produto, *nregistros, sizeof(Is), comparacao_iproduct_NOME);
 
}
 
void insere_ibrand(Is *indice_marca, int* nregistros) {
 
    if (*nregistros == 0)
        return;
 
    Produto J = recuperar_registro(*nregistros-1);
    strcpy(indice_marca[*nregistros-1].string, J.marca);
    strcpy(indice_marca[*nregistros-1].pk, J.pk);
 
 
    /* Ordenado pela marca e, em caso empate, pelo código */
    qsort(indice_marca, *nregistros, sizeof(Is), comparacao_ibrand_MARCA);
 
}
 
void insere_icategory(Ir *indice_categoria, int* nregistros, int *ncat) {
 
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
        Ir *indiceCat = (Ir*) bsearch(categoria, indice_categoria, *ncat, sizeof(Ir), comparacao_icategory_CAT);
        if (indiceCat != NULL) {
            // Achou categoria
            int indiceBusca = indiceCat - indice_categoria;
            inserir_lista(&(indice_categoria[indiceBusca].lista), J.pk);
        } else {
            // Não achou categoria
            strcpy(indice_categoria[*ncat].cat, categoria);
            *ncat += 1;

            // Insere no começo da lista
            ll *novo = (ll*) malloc(sizeof(ll));
            strcpy(novo->pk, J.pk);
            novo->prox = NULL;
            indice_categoria[*ncat-1].lista = novo;
 
            /* Ordenado pelos nomes das categorias e em seguida pelo código */
            qsort(indice_categoria, *ncat, sizeof(Ir), comparacao_icategory_CAT);
        }
 
 
        // Vai para a proxima categoria
        cat = strtok(NULL, "|");
    }

 
}
 
void insere_iprice(Isf *indice_preco, int* nregistros) {
 
    if (*nregistros == 0)
        return;
 
    Produto J = recuperar_registro(*nregistros-1);
    
    float preco;
    int desconto;
 
    strcpy(indice_preco[*nregistros-1].pk, J.pk);
 
    sscanf(J.desconto, "%d", &desconto);
    sscanf(J.preco, "%f", &preco);
 
    // Calculo do preço COM DESCONTO
    preco = (preco * (100-desconto)) / 100.0;
    preco = preco * 100;
    preco = ((int) preco / (float) 100);
 
    indice_preco[*nregistros-1].price = preco;
 
 
    /* Ordenado pelo primeiramente pelo preço em ordem ascendente e, em seguida, pelo código */
    qsort(indice_preco, *nregistros, sizeof(Isf), comparacao_iprice_PRECO);
 
}
 
 
/**********************************
    INTERAÇÃO COM O USUÁRIO
***********************************/
 
 
/**** INSERCAO ****/
 
void ler_entrada(char* registro, Produto *novo) {
    fscanf(stdin, "%[^\n]s", novo->nome);
    getchar();
    fscanf(stdin, "%[^\n]s", novo->marca);
    getchar();
    fscanf(stdin, "%[^\n]s", novo->data);
    getchar();
    fscanf(stdin, "%[^\n]s", novo->ano);
    getchar();
    fscanf(stdin, "%[^\n]s", novo->preco);
    getchar();
    fscanf(stdin, "%[^\n]s", novo->desconto);
    getchar();
    fscanf(stdin, "%[^\n]s", novo->categoria);
    getchar();
    
    sprintf(registro, "%s@%s@%s@%s@%s@%s@%s@", novo->nome, novo->marca, novo->data, novo->ano, novo->preco, novo->desconto, novo->categoria);
    
    // Completa os espaços restantes com '#'
    int necessarios = 192 - strlen(registro);
    for (int i = 0; i < necessarios; i++)
        strcat(registro, "#");
}
void inserir(Ip *iprimary, Is* iproduct, Is* ibrand, Ir* icategory, Isf *iprice, int *nreg, int *ncat) {
 
    char temp[193];
    Produto I;
    
    // Lê os dados e os coloca na string temp
    ler_entrada(temp, &I);
    gerarChave(&I);
 
 
    // Verifica se existe chave primária igual
    Ip *indicePri = (Ip*) bsearch(I.pk, iprimary, *nreg, sizeof(Ip), comparacao_iprimary_PK);
    if (indicePri != NULL) {
        // Achou a chave
        /* Caso a chave a inserir esteja no ARQUIVO porém esteja marcada como removida */
        /* Achei que era assim */
        if (indicePri->rrn == -1) {

            // Coloca no final do ARQUIVO
            strcat(ARQUIVO, temp);

            // Atualiza o RRN
            indicePri->rrn = *nreg;

            return;
        
        }
        else {
            printf(ERRO_PK_REPETIDA, I.pk);
            return;
        }
    }

    // Insere no ARQUIVO de dados
    strcat(ARQUIVO, temp);
    
    // Incrementa o número de registros
    *nreg += 1;
 
    // Cria o índice primário
    insere_iprimary(iprimary, nreg);
 
    // Cria o índice do produto
    insere_iproduct(iproduct, nreg);
 
    // Cria o indice da marca
    insere_ibrand(ibrand, nreg);
 
    // Cria o indice da categoria
    insere_icategory(icategory, nreg, ncat);
 
    // Cria o indice do preco
    insere_iprice(iprice, nreg);
 
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
 
int bsearch_inferior(Is *a, int inicio, int fim, char chave[]) {
 
    if (fim < inicio)
        return inicio;
 
    int meio = (inicio + fim) / 2;
    
    
    if (strcmp(a[meio].string, chave) >= 0)
        meio = bsearch_inferior(a, inicio, meio-1, chave);
    else
        meio = bsearch_inferior(a, meio+1, fim, chave);
    
    return meio;
 
}
int bsearch_superior(Is *a, int inicio, int fim, char chave[]) {
 
    if (fim < inicio)
        return fim;
 
    int meio = (inicio + fim) / 2;
    
    
    if (strcmp(a[meio].string, chave) > 0)
        meio = bsearch_superior(a, inicio, meio-1, chave);
    else
        meio = bsearch_superior(a, meio+1, fim, chave);
    
    return meio;
 
}
 
 
void buscar_produtos(Ip *iprimary, Is *iproduct, Ir *icategory, Is *ibrand, int nregistros, int ncat) {
 
    int opcaoBusca;
    char chavePrimaria[TAM_PRIMARY_KEY];
    char nomeProduto[TAM_NOME];
    char marcaProduto[TAM_MARCA];
    char categoriaProduto[TAM_CATEGORIA];
    Ip *indicePri;
    Is *indiceProd;
    Is *indiceMarca;
    Ir *indiceCat;

    Ip vetorIp[MAX_REGISTROS];
    int j = 0;
 
    int indiceInferior, indiceSuperior, indiceBsearch;
 
    scanf("%d%*c", &opcaoBusca);
    switch (opcaoBusca) {
 
        // Busca por chave primária
        case 1:
 
            fgets(chavePrimaria, TAM_PRIMARY_KEY, stdin);
            indicePri = (Ip*) bsearch(chavePrimaria, iprimary, nregistros, sizeof(Ip), comparacao_iprimary_PK);
            if (indicePri != NULL && indicePri->rrn != -1) {
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
                indiceInferior = bsearch_inferior(iproduct, 0, nregistros-1, nomeProduto);
                indiceSuperior = bsearch_superior(iproduct, 0, nregistros-1, nomeProduto);
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
                    if (RRN != -1) {
                        int indice = indicePri - iprimary;
                        vetorIp[j] = iprimary[indice]; // Insere no vetor de indices primarios
                        j++;
                    }
                }
            }

            // Se não inseriu nenhum valor no vetor, retorna
            if (j == 0) {
                printf(REGISTRO_N_ENCONTRADO);
                return;
            }

            // Lista o vetor de índices primários
            for (int i = 0; i < j; i++) {
                int rrn = vetorIp[i].rrn;
                exibir_registro(rrn, 0);
                if (i != j-1)
                    printf("\n");
            }
 
        break;
 
        // Busca por categoria e marca
        case 3:
 
            // Busca pelo Produto de uma marca que está em uma categoria
 
            scanf("%[^\n]s", marcaProduto);
            getchar();
            scanf("%[^\n]s", categoriaProduto);
 
 
            int indiceBsearch = bSearch(ibrand, 0, nregistros-1, marcaProduto);
            if (indiceBsearch != -1) {
                indiceInferior = bsearch_inferior(ibrand, 0, nregistros, marcaProduto);
                indiceSuperior = bsearch_superior(ibrand, 0, nregistros, marcaProduto);
            } else {
                printf(REGISTRO_N_ENCONTRADO);
                return;
            }
 
            if (indiceSuperior == nregistros)
                indiceSuperior--;
            // printf("indiceInferior: %d\nindiceSuperior: %d\n", indiceInferior, indiceSuperior); //!
 
            // Verifica se existe a categoria requisitada
            indiceCat = (Ir*) bsearch(categoriaProduto, icategory, ncat, sizeof(Ir), comparacao_icategory_CAT);
            if (indiceCat == NULL) {
                printf(REGISTRO_N_ENCONTRADO);
                return;
            }

 
            for (int i = indiceInferior; i <= indiceSuperior; i++) {
 
                strcpy(chavePrimaria, ibrand[i].pk);
                indicePri = (Ip*) bsearch(chavePrimaria, iprimary, nregistros, sizeof(Ip), comparacao_iprimary_PK);
                
                // Se estiver marcado como removido, vai para a próxima iteração
                if (indicePri->rrn == -1)
                    continue;
                
                int resBuscaLista = buscar_lista(&(indiceCat->lista), chavePrimaria);
                if (resBuscaLista != -1) {
                    // Adiciona ao vetor de índices primários
                    int indice = indicePri - iprimary;
                    vetorIp[j] = iprimary[indice];
                    j++;
                }
 
            }
 
            // Se não adicionou nada ao vetor, retorna
            if (j == 0) {
                printf(REGISTRO_N_ENCONTRADO);
                return;
            }

            // Lista o vetor de índices primários
            for (int i = 0; i < j; i++) {
                int rrn = vetorIp[i].rrn;
                exibir_registro(rrn, 0);
                if (i != j-1)
                    printf("\n");
            }
 
        break;
 
    }
 
}
 
 
/**** LISTAGEM ****/
 
void listar_produtos(Ip *iprimary, Ir *icategory, Is *ibrand, Isf *iprice, int nregistros, int ncat) {
 
    char categoriaProduto[TAM_CATEGORIA];
 
    /* Apontadores das buscas */
    Ip *indicePri;
    Ir *indiceCat;

    /* Vetor de índices primários */
    Ip vetorIp[MAX_REGISTROS];
    int j = 0; // Controla o índice do vetor de índices primários
 
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
                if (RRN != -1) {
                    vetorIp[j] = iprimary[i]; // insere no vetor de índices primários
                    j++;
                }
            }

            if (j == 0) {
                printf(REGISTRO_N_ENCONTRADO);
                return;
            }

            // Lista o vetor de índices primários
            for (int i = 0; i < j; i++) {
                int rrn = vetorIp[i].rrn;
                exibir_registro(rrn, 0);
                if (i != j-1)
                    printf("\n");
            }
 
        break;
 
        // Listagem por categoria
        case 2:
 
            scanf("%[^\n]s", categoriaProduto);
 
            indiceCat = (Ir*) bsearch(categoriaProduto, icategory, ncat, sizeof(Ir), comparacao_icategory_CAT);
            if (indiceCat) {
                ll *aux = indiceCat->lista;
                while (aux) {
                    indicePri = (Ip*) bsearch(aux->pk, iprimary, nregistros, sizeof(Ip), comparacao_iprimary_PK);
                    int indice = indicePri - iprimary;
                    if (indicePri->rrn != -1) {
                        vetorIp[j] = iprimary[indice]; // insere no vetor de índices primários
                        j++;                      
                    }
                    aux = aux->prox;
                }
            }

            // Lista o vetor de índices primários
            for (int i = 0; i < j; i++) {
                int rrn = vetorIp[i].rrn;
                exibir_registro(rrn, 0);
                if (i != j-1)
                    printf("\n");
            }
 
        break;
 
        // Listagem por marca
        case 3:
 
            for (int i = 0; i < nregistros; i++) {
 
                indicePri = (Ip*) bsearch(ibrand[i].pk, iprimary, nregistros, sizeof(Ip), comparacao_iprimary_PK);
                int RRN = indicePri->rrn;
                if (RRN != -1) {
                    int indice = indicePri - iprimary;
                    vetorIp[j] = iprimary[indice]; // insere no vetor de índices primários
                    j++;
                }
 
            }

            // Lista o vetor de índices primários
            for (int i = 0; i < j; i++) {
                int rrn = vetorIp[i].rrn;
                exibir_registro(rrn, 0);
                if (i != j-1)
                    printf("\n");
            }
 
        break;
 
        // Listagem por preço com desconto aplicado
        case 4:
 
            for (int i = 0; i < nregistros; i++) {
 
                indicePri = (Ip*) bsearch(iprice[i].pk, iprimary, nregistros, sizeof(Ip), comparacao_iprimary_PK);
                int RRN = indicePri->rrn;
                if (RRN != -1) {
                    int indice = indicePri - iprimary;
                    vetorIp[j] = iprimary[indice]; // insere no vetor de índices primários
                    j++;
                }
            }

            // Lista o vetor de índices primários
            for (int i = 0; i < j; i++) {
                int rrn = vetorIp[i].rrn;
                exibir_registro(rrn, 1);
                if (i != j-1)
                    printf("\n");
            }
 
        break;
 
    }
 
}
 
 
/**** ALTERAÇÃO DO DESCONTO ****/
 
void alterar(int rrn, char *novoDesconto, Isf *iprice, int nregistros) {
 
    char *p = ARQUIVO + 192*rrn;
 

    int i = 0; 
    int arr = 0;
    while (*p && arr < 5) {
        if (*p == '@')
            arr++;
        p++;
    }
 
    
    // Altera no ARQUIVO de dados
    *p = novoDesconto[0];
    p++;
    *p = novoDesconto[1];
    p++;
    *p = novoDesconto[2];
 
 
    // Altera em iprice
    Produto J = recuperar_registro(rrn);
 
    for (int i = 0; i < nregistros; i++) {
        if (strcmp(J.pk, iprice[i].pk) == 0) {
            float preco;
            int desconto;
    
            sscanf(novoDesconto, "%d", &desconto);
            sscanf(J.preco, "%f", &preco);
    
            // Calculo do preço COM DESCONTO
            preco = (preco * (100-desconto)) / 100.0;
            preco = preco * 100;
            preco = ((int) preco / (float) 100);
    
            iprice[i].price = preco;
 
            break;
        }
    }
 
    /* Re-ordena o índice de preços (iprice) */
    qsort(iprice, nregistros, sizeof(Isf), comparacao_iprice_PRECO);
 
}
 
 
/**** REMOÇÃO ****/
 
void remover(Ip *indicePri, Ip *iprimary) {
 
    char *p = ARQUIVO + 192 * (indicePri->rrn);
 
    // Coloca o marcador "*|" nas primeiras duas posições
    *p = '*';
    p++;
    *p = '|';
 
    // Modifica o RRN para -1
    indicePri->rrn = -1;
 
}
 
 
/**** REFAZ TODOS OS INDICES ****/
 
void criar_iprimary(Ip *indice_primario, int* nregistros) {
 
    for (int i = 0; i < *nregistros; i++) {
        indice_primario[i].rrn = i;
        Produto J = recuperar_registro(i);
        strcpy(indice_primario[i].pk, J.pk);
    }
 
    /* Ordenado pela chave primária */
    qsort(indice_primario, *nregistros, sizeof(Ip), comparacao_iprimary_PK);
 
}
 
void criar_iproduct(Is *indice_produto, int* nregistros) {
 
    for (int i = 0; i < *nregistros; i++) {
        Produto J = recuperar_registro(i);
        strcpy(indice_produto[i].pk, J.pk);
        strcpy(indice_produto[i].string, J.nome);
    }
 
    /* Ordenado pelo nome do produto ou modelo e, em caso de empate, pelo código */
    qsort(indice_produto, *nregistros, sizeof(Is), comparacao_iproduct_NOME);
 
}
 
void criar_ibrand(Is *indice_marca, int* nregistros) {
 
    for (int i = 0; i < *nregistros; i++) {
        Produto J = recuperar_registro(i);
        strcpy(indice_marca[i].string, J.marca);
        strcpy(indice_marca[i].pk, J.pk);
    }
 
    /* Ordenado pela marca e, em caso empate, pelo código */
    qsort(indice_marca, *nregistros, sizeof(Is), comparacao_ibrand_MARCA);
 
}
 
void criar_icategory(Ir *indice_categoria, int* nregistros, int *ncat) {

    if (*nregistros == 0)
        return;

    for (int i = 1; i <= *nregistros; i++) {
 
        insere_icategory(indice_categoria, &i, ncat);
 
    }
 
 
}
 
void criar_iprice(Isf *indice_preco, int* nregistros) {
 
    for (int i = 0; i < *nregistros; i++) {
 
        Produto J = recuperar_registro(i);        
 
        float preco;
        int desconto;
 
        strcpy(indice_preco[i].pk, J.pk);
 
        sscanf(J.desconto, "%d", &desconto);
        sscanf(J.preco, "%f", &preco);
 
        // Calculo do preço COM DESCONTO
        preco = (preco * (100-desconto)) / 100.0;
        preco = preco * 100;
        preco = ((int) preco / (float) 100);
 
        indice_preco[i].price = preco;
 
    }
 
    /* Ordenado pelo preço e, em caso empate, pelo código */
    qsort(indice_preco, *nregistros, sizeof(Isf), comparacao_iprice_PRECO);
 
}

/**** LIBERAR ESPAÇO ****/

// Retorna o novo numero de registros
int liberar_espaco(int *nregistros) {

    int tam = 0;
    char *aux = (char*) malloc(TAM_ARQUIVO * sizeof(char));

    for (int i = 0; i < *nregistros; i++) {

        char temp[193];
        strncpy(temp, ARQUIVO + ((i)*192), 192);
        temp[192] = '\0';

        if (strncmp(temp, "*|", 2) != 0) {
            strcat(aux, temp);
            tam += 192;
        }

    }

    strcpy(ARQUIVO, aux);
    free(aux);

    ARQUIVO[tam] = '\0';

    // printf("strlen(ARQUIVO): %lu\n", strlen(ARQUIVO));

    return strlen(ARQUIVO) / TAM_REGISTRO;
}


/**** DASALOCAR ESTRUTURAS E LIBERAR MEMORIA ****/

void desalocar_estruturas(Ip *iprimary, Is* iproduct, Is* ibrand, Ir* icategory, Isf *iprice, int *ncat) {

    free(iprimary);
    free(iproduct);
    free(ibrand);
    free(iprice);

    for (int i = 0; i < *ncat; i++) {
        liberar_lista(&(icategory[i].lista));
    }

}

 
/**** FUNÇÕES DA LISTA ENCADEADA ****/
 
// Insere na lista ordenado
void inserir_lista(ll **primeiro, char *pk) {
 
    /* CASO LISTA VAZIA */
    if (strcmp((*primeiro)->pk, pk) > 0) {
        ll *novo = (ll*) malloc(sizeof(ll));
        if (novo) {
            strcpy(novo->pk, pk);
            novo->prox = *primeiro;
            *primeiro = novo;
            return;
        }
    }
 
    ll *aux = *primeiro;
    
    // Percorre a lista até achar um valor maior
    while (aux->prox && strcmp(aux->prox->pk, pk) < 0)
        aux = aux->prox;

    if (aux->prox && strcmp(aux->pk, pk) == 0)
        return;
        

    ll *novo = (ll*) malloc(sizeof(ll));
    if (novo) {
        strcpy(novo->pk, pk);
        if (aux->prox == NULL)
            novo->prox = NULL;
        else
            novo->prox = aux->prox;
        aux->prox = novo;
    }
 
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

// Libera a memoria das listas ligadas
void liberar_lista(ll **primeiro) {

    ll *atual = *primeiro;
    ll *temp;

    while (atual) {
        temp = atual;
        atual = atual->prox;
        free(temp);
    }

    *primeiro = NULL;

}