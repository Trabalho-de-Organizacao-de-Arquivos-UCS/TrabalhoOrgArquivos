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


main(){
	
	char *dir = createDir();
	FILE *file;
	
	struct twitter *t_object = (twitter*) malloc(sizeof(struct twitter));
/*	strcpy(t_object->usuario, "User1");
	strcpy(t_object->hashtag, "#Joker");
	strcpy(t_object->data, "17/09/2019");
	strcpy(t_object->mensagem, "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua");
	strcpy(t_object->pais, "Brazil");
	strcpy(t_object->usuario, "User1");
	
	if(file = fopen("c:\\temp\\OrgArq\\file.dat","ab")){
		 fwrite(t_object, sizeof(struct twitter), 1, file);
	}
	
	strcpy(t_object->usuario, "User2");
	strcpy(t_object->hashtag, "#Rambo");
	strcpy(t_object->data, "19/09/2019");
	strcpy(t_object->mensagem, "Consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua");
	strcpy(t_object->pais, "Brazil");
	strcpy(t_object->usuario, "User2");
	
	if(file = fopen("c:\\temp\\OrgArq\\file.dat","ab")){
		 fwrite(t_object, sizeof(struct twitter), 1, file);
	}*/
	
	if(file = fopen("c:\\temp\\OrgArq\\file.dat","rb")){

		while(!feof(file)){
			fread(t_object, sizeof(struct twitter), 1, file);
			printf("\n--------------------------------------------------------------------------------------------------------------------------------\n");
			printf(" ID: %d \n USER: %s \n HASHTAGS: %s \n MENSAGEM: %s",t_object->id_twitter,t_object->usuario, t_object->hashtag,t_object->mensagem); 
			printf("\n--------------------------------------------------------------------------------------------------------------------------------\n");
		}

		 
	}
	
	
	
}
