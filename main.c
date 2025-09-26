#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Minha struct dos "nós" da pilha
typedef struct celula{
    char valor;
    struct celula *proximo;
}CELULA;

//Minha pilha que sempre aponta para o ultima celula que entro na pilha
typedef struct pilha{
    CELULA *topo;
}PILHA;

//Função que cria minha pilha dinamicamente na Heap e retorna um enereço para funçao main na stack
PILHA *criar_pilha(){
    PILHA *endereço_pilha_na_heap;
    endereço_pilha_na_heap = (PILHA*)malloc(sizeof(PILHA));
    endereço_pilha_na_heap->topo = NULL;//começa vazia olhando para nada
    return endereço_pilha_na_heap; //retorna o enereço para localizar a pilha(topo) na heap
}

//Função que empilha minhas celulas na pilha,recebe como parâmetro o endereço da pilha e um caracter
void empilhar(PILHA *pilha,char caracter){
    CELULA *elemento = (CELULA*) malloc(sizeof(CELULA));
    elemento->proximo = pilha->topo;
    pilha->topo = elemento;
    elemento->valor = caracter;
    return;
}

//Função que desempilha uma celula(a ultima celula que entrou na pilha) e retorna o valor que esta armazenado nela
char desempilhar(PILHA *pilha){ //Unico parâmetro é a minha pilha(topo)
    if(pilha->topo == NULL){
        //printf("A pilha esta vazia"); verifica se a pilha está vazia
        return -1;
    }
    CELULA *aux = pilha->topo;
    pilha->topo = pilha->topo->proximo;
    char salvar_caractere = aux->valor;
    free(aux); //apaga a celula para ela nao ficar peria na heap
    return salvar_caractere;
}

//Funçao que libera as células da pilha,ela vai ate o ultima celula da pilha e vai removendo de baixo para cima
void liberar_celulas(CELULA *celula){
    if(celula == NULL){//caso base,vai empilhando a função ate que celula aponte para null
        return;
    }
    liberar_celulas(celula->proximo);
    free(celula);
}

//Função para eu liberar a Pilha da heap
void liberar_pilha(PILHA *pilha){
    liberar_celulas(pilha->topo);
    free(pilha);
    return;
}


char *Transformar_infixa_para_posfixa(char *infixa){
    char *posfixa = (char*) malloc((strlen(infixa) + 1) * sizeof(char));//Alocando uma string do mesmo tamanho da entrada, que ira retornar na saída com a expreção formatada para fosfixa
    PILHA *p = criar_pilha(); //Criação da pilha

    int k = 0;//variável que vai controlar as posições da minha string posfixa
    for(int i=0; i < strlen(infixa); i++){
        char aux; //variável auxiliar que vai armazenar os operadores para comparação

        //Primeiro caso: se o caracter que está sendo comparado for um '(' então eu irei somente empilhar
        if(infixa[i] == '('){
            empilhar(p,infixa[i]);
        }

        //Segundo caso
        else if(infixa[i] == '-' || infixa[i] == '+'){
            aux = desempilhar(p);//desempilha o operador na pilha(para onde topo aponta)
            while(aux != -1 && aux != '('){ // se a procedencia for maior ou igual do que '+' e '-'
                posfixa[k] = aux;
                k++;                                                                    
                aux = desempilhar(p);
            }
            //se for menor,no caso '('
            if(aux == -1){
                empilhar(p,infixa[i]);  
            }
            else{
            empilhar(p,aux);
            empilhar(p,infixa[i]);
            }     
        }
        //Terceiro caso
        else if(infixa[i] == '*' || infixa[i] == '/'){
            aux = desempilhar(p);//desempilha o operador na pilha(para onde topo aponta)
            while(aux != -1  && aux != '(' && aux != '+' && aux != '-'){//se a procedência de quem estiver no topo da pilha for maior ou igual ao caracter de infixa[i] eu irei desempilhar este da pilha 
                posfixa[k] = aux;
                k++;
                aux = desempilhar(p);
            }
           if(aux == -1){
                empilhar(p,infixa[i]);  
            }
            else{
            empilhar(p,aux);
            empilhar(p,infixa[i]);
            }     
        }

        //Quarto caso
        else if(infixa[i] == '^'){
            aux = desempilhar(p);
            while(aux != -1 && aux != '(' && aux != '+' && aux != '-' && aux != '*' && aux != '/'){
               posfixa[k] = aux;
               k++;
               aux = desempilhar(p);
            }
            if(aux != -1){
                empilhar(p, aux);
            }
            else{
            empilhar(p,aux);
            empilhar(p,infixa[i]);
            }   
        }
                
        
        //Quinto caso
        else if(infixa[i] == ')'){ //desempilha até chegar no '('
            aux = desempilhar(p);
            while(aux != '(' ){
                posfixa[k] = aux;
                k++;
                aux = desempilhar(p);
            }
        }
        
        //Sexto caso
        else{
            posfixa[k] = infixa[i];
            k++;
        }
    }
    // se minha pilha estiver elementos ainda,deve desempilhar ate nao restar nada
    while(p->topo != NULL){
        posfixa[k]= desempilhar(p); 
        k++;
    }
    posfixa[k] = '\0';
    liberar_pilha(p);
    return posfixa;
}
int main(){
    char infixa[1000];
    scanf(" %[^\n]", infixa);
    char *posfixa = Transformar_infixa_para_posfixa(infixa);
    printf("%s\n",posfixa);
    free(posfixa);
    return 0;
}
