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
			}
	
		
		}

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
					printf("NOVO REGISTRO CRIADO \n");
					fwrite(aux_index,sizeof(struct index),1,index);
					printf("NOVO REGISTRO INSERIDO NO INDEX: %s \n",aux_index->hashtag);
			
			}
			
			fclose(index);
			
		}
		
	}
	
	

}

void displayAll(struct twitter *tw){

	FILE *file;
	
	if(file = fopen("c:\\temp\\OrgArq\\file.dat","rb")){

		while(!feof(file)){
			fread(tw, sizeof(struct twitter), 1, file);
					
			printf(" ID: %d \n USER: %s \n HASHTAGS: %s \n MENSAGEM: %s",tw->id_twitter,tw->usuario, tw->hashtag,tw->mensagem); 
			printf("\n--------------------------------------------------------------------------------------------------------------------------------\n");
		
		}

	}
}



main(){
	
	char *dir = createDir(); 
	FILE *file, *index;
	
	struct twitter *t_object = (struct twitter*) malloc(sizeof(struct twitter));
	struct index *t_index = (struct index *)malloc(sizeof(struct index));

//	displayAll(t_object);
	search(t_object,"#TheRiseOfSkywalker");
 //indexar(t_object,t_index);
  

	
	
	
}
