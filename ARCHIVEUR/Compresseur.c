#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

typedef struct{
int pere, fd, fg;
float freq;
}Noeud;

float frequence[256]; //Notre tableau ou on stocke la fréquence de chaque caractères.

Noeud arbre[511];  // Notre arbre binaire qui va nous permettre de coder nos caractères.

char *codes[256]; // Stock le code de nos caractère encodé

//La fonction itoa vu en TD
char* itoa(int nombre){
    int nbc = 0;
    int inter = nombre;
    do{
        inter /= 10;
        nbc++;
    } while(inter != 0);
    char* chaine =  malloc ((nbc+1)*sizeof(char));
    chaine[nbc] = '\0';
    inter=nombre;
    int i=0;
    do{
        chaine[nbc-i-1] = (inter - 10*(inter/10))+48;
        inter = inter/10;
        i++;
    } while(inter != 0);
    return chaine;
}
//ajout char pour le parcours
char* ajoutChar(char *s, char c) {
        int len = strlen(s);
        char* a = (char*)malloc(len + 2);
        for (int i = 0; i<len; i++) {
        a[i] = s[i];
        }
        a[len] = c;
        a[len+1] = '\0';
        return a;
        }

// recherche la racine de notre arbre binaire
int rechercheRacine(Noeud* arbre){
  int resultat = 0;
  float frequence = 0.0;
  for (int i=0 ; i <511 ; i++){
    if (arbre[i].freq > frequence){
      resultat = i;
      frequence = arbre[i].freq;
    }
  }
  return resultat;
}
//Donne la longueur moyenne d'un code pour un caracteres.

//Fonction qui calcule la fréquence de chaque caractère dans notre fichier source.
void calculFrequences(char* source){
  FILE* fichier;
  float longueur=0.0;
  fichier = fopen(source,"r");
  int caractere_actuel=fgetc(fichier);

  for(int i=0; i<256; i++){
    frequence[i] = 0.0;
  }


  while(caractere_actuel != EOF){
    frequence[caractere_actuel]++;
    caractere_actuel=fgetc(fichier);
    longueur++;
  }

  for(int i=0; i< 256; i++){
    frequence[i]=(frequence[i])/(longueur);
  }

  for(int i=0; i< 256; i++){
    //if (frequence[i]!=0){
      //printf("%c apparait avec une frequence de %f\n", i, frequence[i]);
    //}
  }
  fclose(fichier);
}
//Fonction de construction de l'arbre binaire.
void BuildTree(){

  for(int i=0; i<511; i++){
    arbre[i].pere=-1;
    arbre[i].fg=-1;
    arbre[i].fd=-1;
    arbre[i].freq=0.0;
  }

  for (int i=0; i<256; i++){
    arbre[i].freq = frequence[i];
  }

  int compteurs=0;
  int indice=256;

  int toto, titi;
  float freqmin1, freqmin2;

  for (int i =0; i<256; i++){
    if(arbre[i].freq !=0.0){
      compteurs++;
    }
  }

  while (compteurs>1){

    toto=-1;
    titi=-1;

    freqmin1=2.0;
    freqmin2=2.0;

    for(int i=0; i<indice; i++){
      //On vérifie que le Noeud a une frequence non nulle et pas de père
      if (arbre[i].pere == -1 && arbre[i].freq > 0.0){
	//On vérifie si le Noeud a une fréquence plus petite que les deux fréquences minimum enregistrée, et on fait en sorte d'avoir toujours freqmin1 < freqmin2
	if (arbre[i].freq < freqmin2){
	  if (arbre[i].freq > freqmin1){
	    // On sait que la fréquence du Noeud est entre freqmin2 et freqmin 1
	    freqmin2 = arbre[i].freq;
	    titi=i;
	  }
	  else{
	    // On sait que la fréquence du Noeud est inférieure aux deux fréquences
	    freqmin2=freqmin1;
	    freqmin1=arbre[i].freq;
	    titi=toto;
	    toto=i;
	  }
	}
      }
    }

    arbre[toto].pere = indice;
    arbre[titi].pere = indice;
    arbre[indice].fg = toto;
    arbre[indice].fd = titi;
    arbre[indice].freq = freqmin1 + freqmin2;

    indice++;
    compteurs--;
  }
//Affichage...
}

void parcours(int i, char* code) {
        if(arbre[i].fg != -1 ) {
        parcours(arbre[i].fg,ajoutChar(code,'0'));
        parcours(arbre[i].fd,ajoutChar(code,'1'));
        }
  else { codes[i] = (char*)malloc(strlen(code)*sizeof(char));
        strcpy(codes[i],code);
  }
  }




void compressionFichier(char* file_name_to_read, char* file_name_to_write){
    FILE* FichierSource;
    FILE* FichierDest;
    FILE* FichierTemp;
    FichierSource = fopen(file_name_to_read,"r");
    FichierDest = fopen(file_name_to_write,"w");
    FichierTemp = fopen("fichierIntermediaire","w");

    unsigned char buffer[262];
    int caractereActuel=fgetc(FichierSource);
    int IndBuffer=0;
    int curseur;
    int CodedInt;
    int puissance;
    int nb_char = 0;

    int count_my_char = 0;
    int count_my_char_todo = 0;

    while(caractereActuel != EOF){

        //On écrit dans le buffer le code du caractère courant
        curseur=0;
        while(codes[caractereActuel][curseur] != '\0'){
          buffer[IndBuffer]=codes[caractereActuel][curseur];
          curseur++;
          IndBuffer++;
        }

        //Puis on vide le buffer autant que possible
        while(IndBuffer > 7){
          //On calcule d'abord la valeur de l'entier à écrire
          CodedInt=0;
            for(int i=0; i<8; i++){
                if (buffer[7-i]=='1'){
                    puissance = 1;
                    for(int j=0; j<i; j++){
                        puissance *= 2;
                    }
                CodedInt += puissance;
                }
            }
            //On écrit l'entier dans le fichier de destination
            fputc(CodedInt, FichierTemp);

            //On décale le buffer de 8 caractères
            for(int i=8 ; i<IndBuffer ; i++) {
                buffer[i-8]= buffer[i];
            }
            IndBuffer -= 8;
            nb_char++;
        }
        caractereActuel=fgetc(FichierSource);
        count_my_char++;
    }

    CodedInt = 0;
    for(int i=0; i<IndBuffer; i++){
        if (buffer[i]=='1'){
            puissance = 1;
            for(int j=7; j>i; j--){
                puissance *= 2;
            }
            CodedInt += puissance;
        }
    }
    fputc(CodedInt, FichierTemp);
    nb_char++;
    fclose(FichierTemp);

    //On ecrit dans le fichier resultat le nombre de caractères codés puis le nombre de bits a retenir sur le dernier octet, séparé par un ":"
    fputs(itoa(nb_char), FichierDest);
    fputs(":", FichierDest);
    fputc(IndBuffer, FichierDest);
    count_my_char_todo += strlen(itoa(nb_char)) + 2;

    //On ecrit l'index
    for(int i=0; i<256; i++){
        if (codes[i]!=NULL){
            fputc(i, FichierDest);
            fputc(strlen(codes[i]), FichierDest);
            fputs(codes[i], FichierDest);
            count_my_char_todo += strlen(codes[i]) + 2;
        }
    }
    fputs("::",FichierDest);
    count_my_char_todo += 2;
    //On recopie le compressage à la fin
    FichierTemp = fopen("fichierIntermediaire","r");
    caractereActuel=fgetc(FichierTemp);
    while(caractereActuel != EOF){
        fputc(caractereActuel, FichierDest);
        //printf("on a écrit %d dans le fichier\n",caractereActuel);
        count_my_char_todo ++;
        caractereActuel = fgetc(FichierTemp);
    }

    fclose(FichierTemp);
    fclose(FichierSource);
    fclose(FichierDest);
    remove("fichierIntermediaire");
}


int main(int argc, char ** argv) {
  if(argc != 3) {
system("clear");
printf("usage: ./compress [fichier_source] [fichier_destination]\n");
  }
  else {
    calculFrequences(argv[1]);
    for (int curseur = 0; curseur<256;curseur++) {
      codes[curseur] = NULL;
    }
    BuildTree();

    parcours(rechercheRacine(arbre),"");
  compressionFichier(argv[1], argv[2]);
    }//fin else
}//FIN
