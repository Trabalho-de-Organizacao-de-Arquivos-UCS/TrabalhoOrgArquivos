#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// #include <Windows.h>

struct twitter{

	int id_twitter;
	char usuario[20];
	char mensagem[280];
	char data[8];
	char pais[20];
	char hashtag[200];


};

// char* data_file = "c:\\temp\\OrgArq\\file.dat";
// char* index_file = "c:\\temp\\OrgArq\\index.dat";
// char* index_user_file = "c:\\temp\\OrgArq\\index_user.dat";

char* data_file = "C:\\temp\\OrgArq\\file.dat";
char* index_file = "C:\\temp\\OrgArq\\index.dat";
char* index_user_file = "C:\\temp\\OrgArq\\index_user.dat";

struct index **memory_index = NULL;
long int memory_index_size = 0;

struct usuario_index **memory_user_index = NULL;
long int memory_user_index_size = 0;

struct index{
	char hashtag[200];
	long int pos_in_file;

};

struct usuario_index{
	char usuario[20];
	long int pos_in_file;

};

char *strsep(char **stringp, const char *delim) {
    char *rv = *stringp;
    if (rv) {
        *stringp += strcspn(*stringp, delim);
        if (**stringp)
            *(*stringp)++ = '\0';
        else
            *stringp = 0; }
    return rv;
}

char* createDir(){
	// if(!CreateDirectory("c:\\temp\\OrgArq", NULL)) {
    //     return NULL;
  	// }
  	// else{
  	// 	FILE *file;
  		
  	// 	if(file = fopen(data_file,"wb+")){
	// 	 return data_file;
	// 	}
	// 	else{
	// 		 return NULL;
	// 	}
	// }
	return NULL;
}

void search_memory(struct twitter *tw, char *hashtag ){
	int i, count = 0;
	bool flag = false;
	FILE *file;

	char *ocorrencia[200];

	file = fopen(data_file,"rb");

	for(i = 0; i < memory_index_size; i++) {
		*ocorrencia = strstr(memory_index[i]->hashtag, hashtag);
		if(*ocorrencia != NULL){
			fseek(file, memory_index[i]->pos_in_file - sizeof(struct twitter), SEEK_SET);
			fread(tw, sizeof(struct twitter), 1, file);

			printf("MATCHED POS:%ld {%s}|{%s}\n", memory_index[i]->pos_in_file, memory_index[i]->hashtag, hashtag);
			printf(" ID: %d \n USER: %s \n HASHTAGS: %s \n MENSAGEM: %s",tw->id_twitter,tw->usuario, tw->hashtag,tw->mensagem);
			printf("\n--------------------------------------------------------------------------------------------------------------------------------\n");
			count++;
		}
	}

	printf("%d registros em memória\n", count);
}

void search_file(struct twitter *tw, char *hashtag ){
	struct index *aux_index = (struct index *)malloc(sizeof(struct index));
	int pos;
	int count = 0;
	bool flag = false;
	FILE *file, *index;

	char *ocorrencia[200];

	file = fopen(data_file,"rb");

	if(index = fopen(index_file,"rb")){
		while(!feof(index)){
			fread(aux_index, sizeof(struct index), 1, index);
			*ocorrencia = strstr(aux_index->hashtag, hashtag);
			if(*ocorrencia != NULL){
				fseek(file, aux_index->pos_in_file - sizeof(struct twitter), SEEK_SET);
				fread(tw, sizeof(struct twitter), 1, file);

				printf("MATCHED POS:%ld {%s}|{%s}\n", aux_index->pos_in_file, aux_index->hashtag, hashtag);
				printf(" ID: %d \n USER: %s \n HASHTAGS: %s \n MENSAGEM: %s",tw->id_twitter,tw->usuario, tw->hashtag,tw->mensagem);
				printf("\n--------------------------------------------------------------------------------------------------------------------------------\n");
				count++;
			}

		}
		printf("%d registros em arquivo\n", count);

	}
}


void bynary_search(struct twitter *tw,char *hashtag){
	struct index *aux_index = (struct index *)malloc(sizeof(struct index));
	int pos;
	int count = 0;
	bool flag = false;
	FILE *file, *index;

	char *ocorrencia[200];

	file = fopen(data_file,"rb");
	int cc = 0;
	
	// fclose(index);
	if(index = fopen(index_file,"rb")){
		
		fseek(index, 0 , SEEK_END);
		int tmp = ftell(index) / sizeof(struct index);
		int total_registros =  abs(tmp);
		rewind(index);
		
		bool exit = false;
		int meio,inicio,fim,posindex,ppp;
	 
	 	inicio = 0;
	 	fim = total_registros - 1;
		
		while(inicio <= fim && exit == false){
			
			meio = (inicio+fim) / 2;
			fseek(index, sizeof(struct index) * meio, SEEK_SET);
			fread(aux_index, sizeof(struct index), 1, index);
			
			int cmp = strcmp(aux_index->hashtag,hashtag);
			
			switch(cmp){
				
				case 0:
					ppp = meio;
					while(!exit && !feof(index)){
						ppp--;
						fseek(index, sizeof(struct index) * ppp, SEEK_SET);
						fread(aux_index, sizeof(struct index), 1, index);
						
						if(strcmp(aux_index->hashtag,hashtag) != 0){
							
							while(!exit && !feof(index)){
								ppp++;
								fseek(index, sizeof(struct index) * ppp, SEEK_SET);
								fread(aux_index, sizeof(struct index), 1, index);
								if(strcmp(aux_index->hashtag,hashtag) != 0 || ppp == total_registros){
									exit = true;
								}else{
									 posindex = ftell(index);
									 
									 fseek(file, aux_index->pos_in_file - sizeof(struct twitter), SEEK_SET);
								     fread(tw, sizeof(struct twitter), 1, file);
					
									 printf("MATCHED POS:%ld {%s}|{%s}\n", aux_index->pos_in_file, aux_index->hashtag, hashtag);
									 printf(" ID: %d \n USER: %s \n HASHTAGS: %s \n MENSAGEM: %s",tw->id_twitter,tw->usuario, tw->hashtag,tw->mensagem);
									 printf("\n--------------------------------------------------------------------------------------------------------------------------------\n");
									 count++;
							
								}
							}
						}
					}
					
				
				break;
				
				case 1:
					fim = meio - 1;
				break;
				
				case -1:
					inicio = meio + 1;
				break;
				
			}
		
		}
		

	}
}


void search(struct twitter *tw, char *hashtag ) {
	if(memory_index_size) search_memory(tw, hashtag);
	else search_file(tw, hashtag);
}


void indexar_hashtags(FILE *indexf, char* hashtags, long int pos_in_file) {
	struct index *aux_index = (struct index *)malloc(sizeof(struct index));
	char *r;

	aux_index->pos_in_file = pos_in_file;
	r = strdup(hashtags);

    char *tok = r, *end = r;

    while (tok != NULL) {
        strsep(&end, "#");

        if(strcmp(tok, "")) {
			strcpy(aux_index->hashtag, tok);
			fwrite(aux_index,sizeof(struct index), 1, indexf);
			printf("NOVO REGISTRO INSERIDO NO INDEX (%ld): %s \n",aux_index->pos_in_file, aux_index->hashtag);
		}

        tok = end;
    }

    free(r);
}


void indexar(struct twitter *tw, struct index *ix){
	FILE *file, *index;
	int posicao_index = 0;
	if(file = fopen(data_file,"rb")){

		while(!feof(file)){

			fread(tw, sizeof(struct twitter), 1, file);

			if(posicao_index) {
				index = fopen(index_file,"ab");
			} else {
				index = fopen(index_file,"wb");
				posicao_index++;
			}

			if(index) indexar_hashtags(index, tw->hashtag, ftell(file));
			
			fclose(index);
		}

	}

}



void indexarByUser(struct twitter *tw, struct usuario_index *ix){
	FILE *file, *index;
	struct usuario_index *aux_index = (struct usuario_index *)malloc(sizeof(struct usuario_index));
	int posicao_index = 0;
	if(file = fopen(data_file,"rb")){

		while(!feof(file)){

			fread(tw, sizeof(struct twitter), 1, file);

			if(posicao_index) {
				index = fopen(index_user_file,"ab");
			} else {
				index = fopen(index_user_file,"wb");
				posicao_index++;
			}

			if(index){
				if(strlen(tw->usuario) <= 20){
					aux_index->pos_in_file = ftell(file);
					strcpy(aux_index->usuario, tw->usuario);
					fwrite(aux_index,sizeof(struct usuario_index),1,index);
					// printf("NOVO REGISTRO INSERIDO NO INDEX: %s \n",aux_index->usuario);
				}

			}

			fclose(index);

		}

	}

}

void indexar_em_memoria(struct twitter *tw, struct index *ix) {
	FILE *file, *index;
	struct index *aux_index = (struct index *)malloc(sizeof(struct index));

	file = fopen(data_file,"rb");
	if(!file) return;

	index = fopen(index_file,"rb");

	if(!index) {
		indexar(tw, ix);
		index = fopen(index_file,"rb");
	}

	fseek(index, 0L, SEEK_END);
	memory_index_size = ftell(index) / sizeof(struct index);

	printf("Hashtags carregadas %ld\n", memory_index_size);

	memory_index = (struct index **) malloc(memory_index_size * sizeof(struct index *));

	int i;
	fseek(index, 0L, SEEK_SET);

	for(i = 0; i < memory_index_size; i++) {
		if(feof(index)) {
			printf("end of index before expected\n");
			memory_index_size = i + 1;
		}

		memory_index[i] = (struct index *) malloc(sizeof(struct index));
		fread(memory_index[i], sizeof(struct index), 1, index);
	}

	printf("Index carregado em memória com sucesso!");
}

void indexar_user_em_memoria(struct twitter *tw, struct usuario_index *ix) {
	FILE *file, *index;
	struct usuario_index *aux_index = (struct usuario_index *)malloc(sizeof(struct usuario_index));

	file = fopen(data_file,"rb");
	if(!file) return;

	index = fopen(index_user_file,"rb");

	if(!index) {
		indexarByUser(tw, ix);
		index = fopen(index_user_file,"rb");
	}

	memory_user_index_size = (fseek(index, 0, SEEK_END)+1) / sizeof(struct usuario_index);

	printf("Usuarios carregados %ld\n", memory_user_index_size);

	memory_user_index = (struct usuario_index **) malloc(memory_user_index_size * sizeof(struct usuario_index *));

	int i;
	fseek(index, 0L, SEEK_SET);

	for(i = 0; i < memory_user_index_size; i++) {
		if(feof(index)) {
			printf("end of index before expected\n");
			memory_user_index_size = i + 1;
		}

		memory_user_index[i] = (struct usuario_index *) malloc(sizeof(struct usuario_index));
		fread(memory_user_index[i], sizeof(struct usuario_index), 1, index);
	}

	printf("Index usuário carregado em memória com sucesso!");
}

int index_cmp_by_hashtag(const void *x, const void *y) 
{ 
	struct index *a=*((struct index**)x), *b=*((struct index**)y);
    return strcmp(a->hashtag, b->hashtag);
} 

void ordenar_index(struct twitter *tw, struct index *ix) {
	if(!memory_index_size) indexar_em_memoria(tw, ix);

	qsort(memory_index, memory_index_size, sizeof(struct index *), index_cmp_by_hashtag);
	printf("\nOrdenação finalizada.\nAtualizando arquivo de index...\n");

	int i;
	FILE *indexf = fopen(index_file, "wb");;

	for(i = 0; i < memory_index_size; i++) {	
		fwrite(memory_index[i], sizeof(struct index), 1, indexf);
		fseek(indexf, 0L, SEEK_END);
	}

	printf("memory_index_size: %d | %d\n", memory_index_size, i);
	printf("Arquivo de index atualizado com a ordenação.\n");
}

void print_index(){
	if(!memory_index_size) {
		printf("Index não carregado em memória\n");
		return;
	}

	int i;
	for(i = 0; i < memory_index_size; i++) {
		printf("#%s\n", memory_index[i]->hashtag);
	}
}

void displayAll(struct twitter *tw){

	FILE *file;
	int count = 0;
		printf("display all\n");
	if(file = fopen(data_file,"rb")){

		while(!feof(file)){
			fread(tw, sizeof(struct twitter), 1, file);

			printf(" ID: %d \n USER: %s \n HASHTAGS: %s \n MENSAGEM: %s",tw->id_twitter,tw->usuario, tw->hashtag,tw->mensagem);
			printf("\n--------------------------------------------------------------------------------------------------------------------------------\n");
			count++;
		}
		printf("%d registros\n", count);

	} else {
		printf("Ajustar configurações de caminha do arquivo!\n");
	}
}



void LaunchMenu(){
	char *dir = createDir();
	FILE *file, *index;

	struct twitter *t_object = (struct twitter*) malloc(sizeof(struct twitter));
	struct index *t_index = (struct index *)malloc(sizeof(struct index));

    struct usuario_index *u_index = (struct usuario_index *)malloc(sizeof(struct usuario_index));

	char menu[500] = "\nSelecione a opcao abaixo \
										\n1- Listar todos os registros \
										\n2- Indexar Arquivo (HashTag) \
										\n3- Indexar Arquivo (Usuario) \
										\n4- Pesquisar Hashtag \
										\n5- Carregar index em memória (HashTag) \
										\n6- Carregar index em memória (Usuario) \
										\n7- Ordena index em memória (Usuario) \
										\n8- Imprimir index em memória (Usuario) \
										\nInforme a opcao: ";

  printf("\n%s\n",menu);
	bool exit = false;
	while(!exit){


		int option;

		scanf("%d",&option);

		switch(option){
			case 1:
				displayAll(t_object);
			    printf("\n%s\n",menu);
			break;
			case 2:
				indexar(t_object,t_index);
				printf("\n%s\n",menu);
			break;
			case 3:
				indexarByUser(t_object,u_index);
				printf("\n%s\n",menu);
			break;
			case 4:
				char term[200];
				printf("\nInforme a hashtag:\n");
				scanf("%s",&term);
				bynary_search(t_object,term);

				printf("\n%s\n",menu);
			break;
			case 5:
				indexar_em_memoria(t_object, t_index);
				printf("\n%s\n",menu);
			break;
			case 6:
				indexar_user_em_memoria(t_object, u_index);
				printf("\n%s\n",menu);
			break;
			case 7:
				ordenar_index(t_object, t_index);
				printf("\n%s\n",menu);
			break;
			case 8:
				print_index();
				printf("\n%s\n",menu);
			break;
			case 0:
				exit = true;
			break;
		}
	}



}


main(){


//testet();
//indexarPrim();
LaunchMenu();
//

 //indexar(t_object,t_index);





}
