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
	char h[200];
	int pos;
	bool flag = false;
	FILE *file, *index;
	
	file = fopen("c:\\temp\\OrgArq\\file.dat","rb");
	index = fopen("c:\\temp\\OrgArq\\index.dat","rb");
	
	while(fread(&h, sizeof(h),1,index)){
		if(strcmp(h,hashtag) == 0){
			flag = true;
			break;
		}
	}
	if(flag){
		fseek(index,(-1) * sizeof(*tw),SEEK_CUR);
		pos = ftell(index);
		fseek(file, pos/ sizeof(h) * sizeof(*tw), SEEK_SET);
		fread(tw,sizeof(*tw),1,file);
	    printf(" ID: %d \n USER: %s \n HASHTAGS: %s \n MENSAGEM: %s",tw->id_twitter,tw->usuario, tw->hashtag,tw->mensagem); 
	}
	
}


void indexar(struct twitter *tw){
	FILE *file, *index;
	
	if(file = fopen("c:\\temp\\OrgArq\\file.dat","rb")){

		while(!feof(file)){
			fread(tw, sizeof(struct twitter), 1, file);
			
			if(index = fopen("c:\\temp\\OrgArq\\index.dat","ab")){
				fseek(index,0,SEEK_END);
				fwrite(&(tw->hashtag),sizeof(tw->hashtag),1,index);
				rewind(index);
			}
			
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
	
	struct twitter *t_object = (twitter*) malloc(sizeof(struct twitter));

	displayAll(t_object);
//	search(t_object,"#Joker#DC");
//  indexar(t_object);	
	
	
	
	
}
