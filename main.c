#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

//Argumento dos threads
typedef struct valores {
		int inicio;
		int fim;
		int intervalo;
    	int *vet_aux;
		int y;
		} argumento;

int* vetor;




void cria_heap(int *vet, int i, int f){
	int aux = vet[i];
	int j = i*2+1;
	while(j<=f){
		if (j<f){
			if(vet[j]<vet[j+1]){
				j=j+1;
			}
		}
	if (aux < vet[j]){
		vet[i] = vet[j];
		i = j;
		j = 2*i+1;
	}else{
		j=f+1;
	}
	}
	vet[i] = aux;
}

void heapsort (int *vet, int N){
	int i, aux;
	for (i=(N-1)/2; i >= 0; i--){
		cria_heap(vet, i, N-1);
	}
	for (i=N-1; i >= 0; i--){
		aux = vet[0];
		vet[0] = vet [i];
		vet [i] = aux;
		cria_heap(vet, 0, i-1);
	}
}
// impede que outros threads acessem a região crítica
pthread_mutex_t shallnotpass;
	

	void* threadX (void *pont) {
	int x = 0;
	int j = 0;		
		argumento* valores = (argumento *) pont;		

		pthread_mutex_lock(&shallnotpass);
	
	//Passa os valores do vetor para outro auxiliar
		for (x = valores->inicio ; x >= valores->inicio && x<=valores->fim ; x++) {
			valores->vet_aux[x] = vetor[x];

}
			heapsort (valores->vet_aux,valores->intervalo);	
//Retorna os valores após a organização			
		for ( j = valores->inicio; x >=valores->inicio && j<=valores->fim ; j++) {
		
			vetor[j] = valores->vet_aux[j];

}
	pthread_mutex_unlock (&shallnotpass);
	pthread_exit (NULL);
	
}


int main(){
	int x = 0;
	char nomeArqFinal[100];
	char nomeArqIni[100];
	int y = 0;
	int tamanhoVetor = 0;
	int NUM_THREADS = 0;
	int j = 0;
	int p;
	
	pthread_mutex_init(&shallnotpass, NULL);
	
	
	//Recebe a quantidade de ítens do vetor -> possibilitar a alocação dinamica de vetor
	printf("Digite o tamanho do vetor (quantos valores ele possui): ");
	scanf("%d", &tamanhoVetor);	
	
	//Aloca dinamicamente o vetor na memoria
	vetor = (int *) malloc(tamanhoVetor * sizeof(int));
	
	//Recebe o nome do aqruivo a ser lido
	printf("Informe o nome do arquivo de origem: ");
	scanf("%s", &nomeArqIni);
	

	
	//Recebe o nome do arquivo no qual o vetor organizado sera salvo
	printf("Informe o nome do arquivo de destino: ");
	scanf("%s", &nomeArqFinal);
	
	//Passa os dados do arquivo de origem para um vetor dinamicamente alocado
	FILE * pFile;
	char mystring [100000];
	int cont = 0;
	
	pFile = fopen (nomeArqIni , "r");
	
	while ( fgets (mystring , 100000 , pFile) != NULL ){
	
		vetor[cont] = atoi(mystring);
		cont++;
	}
	
	fclose (pFile);
		for (p = 0; p < tamanhoVetor; p++){

}
	
	//Recebe a quantidade de threads a serem criados
	printf ("Digite a quantidade de threads desejadado\n");
		scanf ("%d",&NUM_THREADS); 


		
	
		pthread_t threads[NUM_THREADS]; 
	argumento* argthread;
	
	//Cria a quantidade de threads recebido do usuario
 	for (y = 0; y < NUM_THREADS; y++) { 
			argthread = malloc(sizeof(argumento));
			argthread->vet_aux =malloc(sizeof(argumento)*argthread->intervalo);
			argthread->inicio = 0;
			argthread->intervalo = tamanhoVetor/NUM_THREADS;
			argthread->fim = argthread->inicio + argthread->intervalo;
			argthread->y = y;
	 		pthread_create (&threads[y], NULL, threadX, (void *) argthread);  //(trhead criado,null,função,parametro)
			argthread->fim = argthread->fim + argthread->intervalo;

}
	//Impede o programa de terminar até finalizar todas as threads
		for (y = 0; y < NUM_THREADS; y++) {
			pthread_join (threads[y], NULL);
}
	
	
	
	//Ordena os valores anteriormente pre-ordenados pelo algoritmo de HeapSort
		
		heapsort (vetor, tamanhoVetor);
	
	//Salva o vetor organizado no arquivo de nome nomeArqFinal
	FILE *fp;
	fp = fopen (nomeArqFinal, "w+");
	
	for (p = 0; p < tamanhoVetor; p++){
	fprintf(fp, "%d\n", vetor[p]);
	}	
	fclose(fp);
		pthread_mutex_destroy (&shallnotpass);
	return 0;
}

