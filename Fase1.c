#include <stdio.h>
#include <string.h>
#include <time.h>



typedef struct Particion{

char *part_status;
char *part_type;
char *part_fit;
int part_start;
int part_size;
char part_name[16];
}Partition;




typedef struct mbr{
int mbr_tamanio;
time_t mbr_fecha_creacion;
int mbr_disk_signature;
Partition part[4];
}MBR;


void mkdisk(char* tokens);
void Analizador(char *linea);
void CrearDisco(int c, char *ruta, MBR dato);

/**METODO PRINCIPAL MAIN**/
int main(void)
{

   MBR disc;

   disc.mbr_fecha_creacion = 100;
   disc.mbr_tamanio = 200;
   disc.mbr_disk_signature = 20;

   int p=0;
   for(p=0;p<4;p++){
       disc.part[p].part_status = '0';
       disc.part[p].part_start = 0;
       disc.part[p].part_size = 0;

   }





    char comando1[200];

    char comando[120];
    char comandoT[120];
    int diagonal = 0;

    int q=0;
    for(q=0;q<120;q++){
        comandoT[q] = NULL;
    }

    while (strcasecmp(comando, "exit") != 0){


        int b=0;
        for(b=0;b<120;b++){
            comando[b] = NULL;
        }

        if(diagonal == 0){
        printf("root@201313720#: ");
}

     // AnalizarLinea("mount-path=/home/Jonnychaavez/Documentos/perra/mijito3.txt -name=ParticionL1");

        fgets(&comando,120,stdin);//scanf("%d",&d);

//QUITANDO SALTO DE LINEA
        int contador = 0;
        int c=0;
       while(comando[contador] != NULL){
           contador++;
        }
       comando[contador-1] = NULL;

       //  printf("Tomala: %s\n", &comando[contador-2]);
       if (comando[contador-2] == '\\'){
           comando[contador-2] = ' ';
           diagonal = 1;
       }else{
           diagonal = 0;
       }
      // printf("son %d \n", contador);

//QUITANDO LOS ESPACIOS DE MAS
        int a=0;
        int valu = 0;
        int comillas = 0;
        for(a=0;a<120;a++){
            if(comando[a] == '-'){
                if((a-4)>0){
                    if(comando[a-1] == ':' && comando[a-2] == ':' &&comando[a-3] == 'd' && comando[a-4] == 'd' && comando[a-5] == 'a'){
                        comando[a] = '%';
                    }
            }
            }

            if(comando[a] == '\"'){
                if(comillas == 0){
                    comillas = 1;
                    valu = 0;
                }else{
                    comillas = 0;
                }

                int b=0;
                for(b=a;b<119;b++){
                    comando[b] = comando[b+1];
                }
            }

            if (comillas == 0){
           if(valu == 1 && comando[a] == ' '){
           // printf("aqui hay doble");
            int b=0;
            for(b=a;b<119;b++){
                comando[b] = comando[b+1];
            }
            a--;
          //  printf("vale verga %s \n", &comando);
           }else{
               valu = 0;
           }
           if(comando[a] == ' '){
                //printf("verguis:\n");
               comando[a]='$';
            valu = 1;
           }
           }

        }

 // printf("Tomala: %d\n", diagonal);

    if (diagonal == 1){
       int contador2 = 0;
       while(comandoT[contador2] != NULL){
           contador2++;
        }
       int q=0;
       for(q=0;q<contador;q++){
           comandoT[contador2] = comando[q];
           contador2++;
       }


    }else{

        int contador2 = 0;
        while(comandoT[contador2] != NULL){
            contador2++;
         }
        int w=0;
        for(w=0;w<contador;w++){
            comandoT[contador2] = comando[w];
            contador2++;
        }
       // printf("Tomala: %s\n", &comandoT);
        Analizador(&comandoT);

        int q=0;
        for(q=0;q<120;q++){
            comandoT[q] = NULL;
        }
    }
//    AnalizarLinea(&comando);

    }

    return 0;
}


void CrearDisco(int c, char *ruta, MBR dato){


  int ejec=0;
        char path1[130];
        char ayuda[130];
        int bitInicio = 0;
        for(ejec=0;ejec<130;ejec++){
            ayuda[ejec] ='\0';
        }
        strcpy(path1, ruta);
        int i=0;
        for(i=0;i<130;i++){
            if(path1[i]=='/'){
                ayuda[i]=path1[i];
                char *hola= (char*)malloc(150);
                strcpy(hola,"mkdir ");
                strcat(hola,"\"");
                strcat(hola,ayuda);
                strcat(hola,"\"");
               // printf("direccion: %s\n",hola);
                system(hola);
                free(hola);
                ejec=i;
            }
            ayuda[i]=path1[i];
            if(path1[i]=='\0'){

                break;
            }
        }

    MBR *dato2;
int cant = 0;
int canti = 0;

cant = c;
char aux = NULL;
char rutita[50];

canti = cant - sizeof(MBR);

strcpy(rutita,ruta);
FILE *archivo = fopen(ruta, "wb+");
fwrite(&dato, sizeof(dato),1,archivo);
int a =0;

for(a=0;a<canti;a++){
    aux = NULL;
    fwrite(&aux,sizeof(char),1,archivo);
}
fclose(archivo);

  printf("\n-----------------------Disco creado------------------------\n");
}



void Analizador(char *linea){


    char *token;
    token = strtok(linea, "$"); /*Separamos por espacios*/

    if (strcasecmp(token, "mkdisk") == 0){
       mkdisk(linea);
    }

    else if (strcasecmp(token, "rmdisk") == 0){
       //rmdisck(linea);
    }
    else if (strcasecmp(token, "fdisk") == 0){
        //fdisk(linea);
     }
    else if (strcasecmp(token, "mount") == 0){
        //mount(linea);
     }
    else if (strcasecmp(token, "unmount") == 0){
        //unmount(linea);
     }

}


void mkdisk(char* tokens){


    char* vector[40];
    int p=0;
    for(p=0;p<40;p++){
        vector[p]=NULL;
    }
    p=0;
    while(tokens!= NULL){
        tokens = strtok(NULL,"$");
        vector[p]= tokens;
        p++;
    }

    char* path= NULL;
    char* unit= NULL;
    char* size= NULL;
    char* nombre = NULL;
    char* extension=NULL;
    int sizeN = 0;

    int valpath = 0;
    int valunit = 0;
    int valsize = 0;
    int valname = 0;
    int valextension=0;
    int validacionT = 0;

   p = 0;
    while(vector[p]!= NULL){
        char* type;

       type = strtok(vector[p],"::");

       if (strcasecmp(type, "-size") == 0){
           size = strtok(NULL,"::");
           valsize = 1;
       }else if(strcasecmp(type, "+unit") == 0){
           unit = strtok(NULL,"::");
           valunit = 1;
       }else if(strcasecmp(type, "-path") == 0){
           path = strtok(NULL,"::");
           valpath = 1;
       }else if(strcasecmp(type, "-name") == 0){
           nombre = strtok(NULL,"::");
           valname = 1;
       }else{
               printf("\nESTE PARAMETRO NO ES VALIDO!!\n");
       }
        p++;
    }
//parseando size
    if (valsize == 1){
    sizeN = (int) strtol(size, NULL,10);
    }
    if (sizeN<1){
        validacionT = 1;
    }

    extension = strtok(nombre,".");
    extension = strtok(NULL,".");

    if(strcasecmp(extension,"dsk")==0)
    {
        valextension=1;
    }
    else
    {
        valextension=0;
    }
    //verificando si vienen los datos

    if (valpath == 1 && valsize == 1 && valname == 1 && valextension==1){
        if (valunit == 1){
            if (strcasecmp(unit, "k") == 0){
            sizeN = sizeN * 1024;

            }else if (strcasecmp(unit, "m") == 0){
            sizeN = sizeN * 1024 * 1024;

            }else{
        //error
            }

        //si no se crea ningun unit como k o m por defecto traera m
        }else{
             sizeN = sizeN * 1024* 1024;
        }


            MBR disc;

            disc.mbr_fecha_creacion = 100;
            disc.mbr_tamanio = sizeN;
            disc.mbr_disk_signature = 20;

            int p=0;
            for(p=0;p<4;p++){
                disc.part[p].part_status = '0';
                disc.part[p].part_start = 0;
                disc.part[p].part_size = 0;

            }


   char * str3 = (char *) malloc(1 + strlen(path)+ strlen(nombre));
   strcpy(str3,path);
   strcat(str3,nombre);

   CrearDisco(sizeN,str3,disc);

    }else{
        printf("Hace falta un atributo, para realizar con exito la operacion!!\n");
    }
}
