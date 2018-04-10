#include <stdlib.h>
#include <stdio.h>
#include<string.h>
#include<math.h>
char* codes[256];
typedef struct{
  int fd, fg, info;
}Noeud;
Noeud arbre[511];
unsigned int indice_lecture = 0;
unsigned int indice_ecriture = 1;
void BuildTree(){
  for (int j = 0; j < 511; j++) {
    arbre[j].fd = -1;
    arbre[j].fg = -1;
    arbre[j].info = -1;
  }
printf("DICTIONNAIRE :{");
  for (unsigned int i = 0; i < 256; i++) {
    if(codes[i] != NULL) {
      indice_lecture = 0;
      for(int c = 0; c < strlen(codes[i]); c++) {
        if(codes[i][c] == '0') {
          if(arbre[indice_lecture].fg == -1) {
            arbre[indice_lecture].fg = indice_ecriture;
            indice_ecriture++;
          }
          indice_lecture = arbre[indice_lecture].fg;
        }
        else {
          if(arbre[indice_lecture].fd == -1) {
            arbre[indice_lecture].fd = indice_ecriture;
            indice_ecriture++;
          }
          indice_lecture = arbre[indice_lecture].fd;
        }
      }
      arbre[indice_lecture].info = (unsigned char)i;
      printf(" %c ,",(unsigned char)i);
    }
  }
printf(" }\n");
}
int main(int argc, char**argv) {
  if(argc != 3) {
    printf("usage : ./decompress [fichier_source] [fichier_destination]\n");
  }
  else {
    printf("$$\\   $$\\ $$\\   $$\\ $$$$$$$$\\ $$$$$$$$\\ $$\\      $$\\  $$$$$$\\  $$\\   $$\\ \n");
    printf("$$ |  $$ |$$ |  $$ |$$  _____|$$  _____|$$$\\    $$$ |$$  __$$\\ $$$\\  $$ |\n");
    printf("$$ |  $$ |$$ |  $$ |$$ |      $$ |      $$$$\\  $$$$ |$$ /  $$ |$$$$\\ $$ |\n");
    printf("$$$$$$$$ |$$ |  $$ |$$$$$\\    $$$$$\\    $$\\$$\\$$ $$ |$$$$$$$$ |$$ $$\\$$ |\n");
    printf("$$  __$$ |$$ |  $$ |$$  __|   $$  __|   $$ \\$$$  $$ |$$  __$$ |$$ \\$$$$ |\n");
    printf("$$ |  $$ |$$ |  $$ |$$ |      $$ |      $$ |\\$  /$$ |$$ |  $$ |$$ |\\$$$ |\n");
    printf("$$ |  $$ |\\$$$$$$  |$$ |      $$ |      $$ | \\_/ $$ |$$ |  $$ |$$ | \\$$ |\n");
    printf("\\__|  \\__| \\______/ \\__|      \\__|      \\__|     \\__|\\__|  \\__|\\__|  \\__|\n");
    printf("------------------------------Fréquences---------------------------- :\n");

    FILE* fichier = NULL;
    int compt = 0;
    unsigned char caractereActuel;
    int indiceSave = 0;
    int check = 1;
    int decalage = 0;
    int nb_char = 0;
    for (int i = 0; i < 256; i++) {
      codes[i] = NULL;
    }
    fichier = fopen(argv[1], "r");
    if (fichier != NULL)
    {
/*----------------ON TROUVE LE NOMBRE DE CARACRERE DU FICHIER--------------------------*/
      caractereActuel = fgetc(fichier);
      while(caractereActuel != ':') {
        nb_char *= 10;
        nb_char += caractereActuel - '0';
        caractereActuel = fgetc(fichier);
      }
      printf("nombre de caractère dans le fichier : %d\n",nb_char);
/*-------------------ON TROUVE LE NOMBRE DE BIT A CONSERVER A LA FIN-------------------*/
      caractereActuel = fgetc(fichier);
      decalage = (unsigned int)caractereActuel;
      printf("nombre de bit à prendre en compte pour le dernier buffer : %d\n",decalage);
/*---------------------------------ENCODAGE DE 'CODES'---------------------------------*/
      printf("le caractereActuel est %c qui vaut %d\n",caractereActuel,(unsigned char)caractereActuel);
      caractereActuel = fgetc(fichier);
      while (check == 1) { //Tant que c'est pas le séparateur :
      if (caractereActuel == ':') {
        caractereActuel = fgetc(fichier);
        if (caractereActuel == ':') {
          check = 0;
        }
        else{
          indiceSave = (unsigned char)caractereActuel;
          caractereActuel = fgetc(fichier);
          compt = (unsigned char)caractereActuel;
          codes[indiceSave] = (char*)malloc(compt*sizeof(char));
          for (int i = 0; i < compt; ++i) {
            caractereActuel = fgetc(fichier);
            codes[indiceSave][i] = (unsigned int)caractereActuel;
          }
        }

      }

      else {
        indiceSave = (unsigned char)caractereActuel;
        //printf("%d = indice de codes \n",caractereActuel);
        caractereActuel = fgetc(fichier);
        compt = caractereActuel;
        //printf("%d est le compteur \n",compt);
        codes[indiceSave] = (char*)malloc(compt*sizeof(char));
        for (int i = 0; i < compt; ++i) {
          caractereActuel = fgetc(fichier);
          codes[indiceSave][i] = (unsigned int)caractereActuel;
          //printf("%c", codes[indiceSave][i]);
        }
      }
      caractereActuel = fgetc(fichier);
    }

    printf("Codes : \n");
    for (int i = 0; i < 256; i++) {
      if(codes[i] != NULL) {
        printf("le caractère %c est codé %s \n",(unsigned char)i,codes[i]);
      }
    }
/*------------------------CONTRUCTION DE L'ARBRE BINAIRE------------------------*/
  BuildTree();
/*---------------------DECOMPRESSION DE CHAQUE CARACTERE ASCII------------------*/
    int curseur = 1;
    unsigned int toconvert;
    int puissance;
    unsigned int finalconverted;
    int nb_bits;
    FILE * fichier_decomp = fopen(argv[2], "w");
    indice_lecture = 0;
    while(curseur <= nb_char) {

      if(curseur < nb_char) {   //on regarde si c'est pas le dernier caractère
        nb_bits = 8;
        puissance = 128;
      }
      else {
        nb_bits = decalage;
        puissance = 128 - pow(2,decalage-1); //sinon on calcule puissance en fonction du nb de bits à conserver
      }
      toconvert = (unsigned char)caractereActuel; //on prend en compte le ascii extended.
      for (int i = 0; i < nb_bits; i++ ) {
        //printf("nb bit = %d\n",nb_bits);
        if(toconvert >= puissance) {
          //printf("toconvert vaut %d il est supérieur à %d on va donc à droite\n", toconvert,puissance);
          toconvert = toconvert - puissance;
          puissance = puissance/2;
          indice_lecture = arbre[indice_lecture].fd;
          if(arbre[indice_lecture].info != -1) {
                finalconverted = (unsigned char)arbre[indice_lecture].info;
            //printf(".");
            fputc(finalconverted,fichier_decomp);
            indice_lecture = 0;
          }
        }
        else {
          //printf("toconvert vaut %d il est inférieur à %d on va donc à gauche\n", toconvert,puissance);
          indice_lecture = arbre[indice_lecture].fg;
          puissance = puissance/2;
          if(arbre[indice_lecture].info != -1) {
            finalconverted = arbre[indice_lecture].info;
            //printf(".");
            fputc(finalconverted,fichier_decomp);
            indice_lecture = 0;
          }
        }
      }
      curseur++;
      caractereActuel = fgetc(fichier);
    }
printf("\nLe fichier à été décompressé ! Il a été sauvegarder sous le nom de '%s'\n",argv[2]);

  }
  else {
    printf("Erreur lors de l'ouverture du fichier, avez vous les droits ? \n");
  }
}
}
