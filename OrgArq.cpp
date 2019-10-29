#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
struct twitter{
	
	int id_twitter;
	char usuario[20];
	char mensagem[280];
	char data[8];
	char pais[20];
	char hashtag[200];
	
	
};


struct index{
	char hashtag[200];
	long int pos_in_file;

};

struct usuario_index{
	char usuario[20];
	long int pos_in_file;

};

char* createDir(){
	if(!CreateDirectory("c:\\temp\\OrgArq", NULL)) {
        return NULL;
  	}
  	else{
  		FILE *file;
  		
  		if(file = fopen("c:\\temp\\OrgArq\\file.dat","wb+")){
		 return "c:\\temp\\OrgArq\\file.dat";
		}
		else{
			 return NULL;
		}
	}
    
}


void search(struct twitter *tw, char *hashtag ){
	struct index *aux_index = (struct index *)malloc(sizeof(struct index));
	int pos;
	int count = 0;
	bool flag = false;
	FILE *file, *index;
	
	char *ocorrencia[200];
	
	file = fopen("c:\\temp\\OrgArq\\file.dat","rb");
	
	
	
	if(index = fopen("c:\\temp\\OrgArq\\index.dat","rb")){

		while(!feof(index)){
			fread(aux_index, sizeof(struct index), 1, index);
			*ocorrencia = strstr(aux_index->hashtag, hashtag);
			if(*ocorrencia != NULL){
				fseek(file, aux_index->pos_in_file - sizeof(*tw),SEEK_SET);
				fread(tw, sizeof(struct twitter), 1, file);
				
				printf(" ID: %d \n USER: %s \n HASHTAGS: %s \n MENSAGEM: %s",tw->id_twitter,tw->usuario, tw->hashtag,tw->mensagem); 
				printf("\n--------------------------------------------------------------------------------------------------------------------------------\n");
				count++;
			}
	
		
		}
		printf("%d registros\n", count);

	}
	
	

	
	
}




void indexar(struct twitter *tw, struct index *ix){
	FILE *file, *index;
	struct index *aux_index = (struct index *)malloc(sizeof(struct index));
	int posicao_index = 0;
	if(file = fopen("c:\\temp\\OrgArq\\file.dat","rb")){
	
		while(!feof(file)){
			
			fread(tw, sizeof(struct twitter), 1, file);
		 
			if(index = fopen("c:\\temp\\OrgArq\\index.dat","ab")){
					aux_index->pos_in_file = ftell(file);
					strcpy(aux_index->hashtag, tw->hashtag);
					fwrite(aux_index,sizeof(struct index),1,index);
					printf("NOVO REGISTRO INSERIDO NO INDEX: %s \n",aux_index->hashtag);
			
			}
			
			fclose(index);
			
		}
		
	}
	
	

}


void indexarByUser(struct twitter *tw, struct usuario_index *ix){
	FILE *file, *index;
	struct usuario_index *aux_index = (struct usuario_index *)malloc(sizeof(struct usuario_index));
	int posicao_index = 0;
	if(file = fopen("c:\\temp\\OrgArq\\file.dat","rb")){
	
		while(!feof(file)){
			
			fread(tw, sizeof(struct twitter), 1, file);
		 
			if(index = fopen("c:\\temp\\OrgArq\\index_user.dat","ab")){
				if(strlen(tw->usuario) <= 20){				
					aux_index->pos_in_file = ftell(file);
					strcpy(aux_index->usuario, tw->usuario);
					fwrite(aux_index,sizeof(struct usuario_index),1,index);
					printf("NOVO REGISTRO INSERIDO NO INDEX: %s \n",aux_index->usuario);
				}
			
			}
			
			fclose(index);
			
		}
		
	}
	
	

}

void displayAll(struct twitter *tw){

	FILE *file;
	int count = 0;
	if(file = fopen("c:\\temp\\OrgArq\\file.dat","rb")){

		while(!feof(file)){
			fread(tw, sizeof(struct twitter), 1, file);
					
			printf(" ID: %d \n USER: %s \n HASHTAGS: %s \n MENSAGEM: %s",tw->id_twitter,tw->usuario, tw->hashtag,tw->mensagem); 
			printf("\n--------------------------------------------------------------------------------------------------------------------------------\n");
			count++;
		}
		printf("%d registros\n", count);

	}
}



void LaunchMenu(){
	char *dir = createDir(); 
	FILE *file, *index;
	
	struct twitter *t_object = (struct twitter*) malloc(sizeof(struct twitter));
	struct index *t_index = (struct index *)malloc(sizeof(struct index));
	
    struct usuario_index *u_index = (struct usuario_index *)malloc(sizeof(struct usuario_index));
	
	char menu[500] = "\nSelecione a opcao abaixo\n1- Listar todos os registros\n2- Indexar Arquivo (HashTag)\n3- Indexar Arquivo (Usuario)\n4- Pesquisar Hashtag\nInforme a opcao: ";
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
				search(t_object,term);
				
				printf("\n%s\n",menu);
			break;
			case 0:
				exit = true;
			break;
		}
	}
	
	
	
}


main(){
	
	
	
	
LaunchMenu();
//	
	
 //indexar(t_object,t_index);
  

	
	
	
}
