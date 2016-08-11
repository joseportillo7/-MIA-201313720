#include <stdio.h>
#include <string.h>
#include <time.h>


/**ESTRUCTURAS**/
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


typedef struct ebr{
char part_status;
char *part_fit;
int part_start;
int part_size;
int part_next;
char part_name[16];
}EBR;


/**INICIALIZACION DE METODOS**/
void mkdisk(char* tokens);
void rmdisk(char* tokens);
void fdisk(char* tokens);
void mount(char* tokens);
void umount(char* tokens);
int UnMon(char id[]);
void Analizador(char *linea);
void CrearDisco(int c, char *ruta, MBR dato);
void EliminarDisco(char *nombre);
int CrearParticion(char* size, char* unit, char* path, char*  type, char* fit, char* name, int add);


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
    int diagonal = 0;//este sera para indicar que si viene una digonal para seguir escribiendo comandos

    int q=0;
    for(q=0;q<120;q++){
        comandoT[q] = NULL;
    }

    //mientras que no sea exit el comando que siga ejecutando
    while (strcasecmp(comando, "exit") != 0){


        int b=0;
        //limpio mi comando para evitar que tenga basura
        for(b=0;b<120;b++){
            comando[b] = NULL;
        }

        //si no hay diagonal que ponga la siguiente impresion
        if(diagonal == 0){
        printf("root@201313720#: ");
}

        //obtengo lo que estoy escribiendo en pantalla
        fgets(&comando,120,stdin);

/**Quitar el salto de linea para que al momento de poner la diagonal
   esta agarre tambien lo que viene en la siguiente linea**/
        int contador = 0;
        int c=0;
       while(comando[contador] != NULL){
           contador++;
        }
       comando[contador-1] = NULL;


       if (comando[contador-2] == '\\'){
           comando[contador-2] = ' ';
           diagonal = 1;
       }else{
           diagonal = 0;
       }


/**QUITANDO LOS ESPACIOS DE MAS***/
        int a=0;
        int valu = 0;
        int comillas = 0;
        for(a=0;a<120;a++){
            if(comando[a] == '-'){
                if((a-6)>0){
                    if(comando[a-1] == ':' && comando[a-2] == ':' &&comando[a-3] == 'd' && comando[a-4] == 'd' && comando[a-5] == 'a'&& comando[a-6] == '+'){
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

           }else{
               valu = 0;
           }
           if(comando[a] == ' '){

               comando[a]='$';
            valu = 1;
           }
           }

        }


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

        Analizador(&comandoT);

        int q=0;
        for(q=0;q<120;q++){
            comandoT[q] = NULL;
        }
    }

    }

    return 0;
}


/**METODO ANALIZADOR DE TODOS LOS COMANDOS**/
void Analizador(char *linea){


    char *token;
    token = strtok(linea, "$"); /*Separamos por espacios*/

    if (strcasecmp(token, "mkdisk") == 0){
       mkdisk(linea);
    }

    else if (strcasecmp(token, "rmdisk") == 0){
       rmdisk(linea);
    }
    else if (strcasecmp(token, "fdisk") == 0){
        fdisk(linea);
     }
    else if (strcasecmp(token, "mount") == 0){
        //mount(linea);
     }
    else if (strcasecmp(token, "unmount") == 0){
        //unmount(linea);
     }

}



/**MKDISK**/
void mkdisk(char* tokens){


    char* vector[40];

    ///limpio mi vector
    int p=0;
    for(p=0;p<40;p++){
        vector[p]=NULL;
    }
    p=0;
    while(tokens!= NULL){
        tokens = strtok(NULL,"$");
        vector[p]= tokens;
        ///aca ya los estoy convirtiendo de esta forma
        ///-size::4-path::/home/jportillo/Escritorio/-name::h.dsk
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
   ///recorro lo que tiene mi vector y mientras no sea null
   ///que me vaya guardando en la variable type lo que trae tokens
    while(vector[p]!= NULL){
        char* type;

       ///aca hago un "split" que lo que venga antes de :: me lo guarde en la variable type
       type = strtok(vector[p],"::");
       ///aca lo delimito por los puntos dobles la cual quedarian asi
       ///-size -path -name

       if (strcasecmp(type, "-size") == 0){
           ///hago otro "split" que agarre
           size = strtok(NULL,"::");
           ///aca vendria siendo ::4 algo asi por lo tanto lo que agarraria seria solo el 4
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
           printf("\nESTE COMANDO NO EXISTE!!\n");
       }
        p++;
    }
//parseando size
    if (valsize == 1){

    ///esta es una forma de convertir de char a entero lleva tres parametros
    ///lo que viene en char el otro es null y la base en este caso es decimal
    sizeN = (int) strtol(size, NULL,10);
    }
    if (sizeN<1 || sizeN==0){
        validacionT = 1;
    }

    ///aca hago un split de la extension para que obtenga el dsk
    extension = strtok(nombre,".");
    extension = strtok(NULL,".");

    //verificando si vienen los datos
    if (valpath == 1 && valsize == 1 && valname == 1 /*&& valextension==1*/){
        if (valunit == 1){
            if (strcasecmp(unit, "k") == 0){
            sizeN = sizeN * 1024;

            }else if (strcasecmp(unit, "m") == 0){
            sizeN = sizeN * 1024 * 1024;

            }else{
        //error
            }

        ///si no se crea ningun unit como k o m por defecto traera m
        }else{
             sizeN = sizeN * 1024* 1024;
        }

        ///aca verifico que si el size es menor a uno o es igual a cero que me tire un error
        if(validacionT==1)
        {
            printf("\nError... el tamano del disco es cero o tiene un numero negativo...\n");
        }
        else if(!(strcasecmp(extension,"dsk")==0))
        {
            printf("\nError... la extension debe ser \"dsk\"\n");
        }else
        {
            MBR disc;

            disc.mbr_fecha_creacion = 100;
            disc.mbr_tamanio = sizeN;
            disc.mbr_disk_signature = 20;

            int p=0;
            for(p=0;p<4;p++){
                disc.part[p].part_status = '0';
                disc.part[p].part_start = 0;
                disc.part[p].part_size = 0;

            }//cierre del for

           ///concateno el nombre con la ruta
           char * str3 = (char *) malloc(1 + strlen(path)+ strlen(nombre) + strlen(extension));
           strcpy(str3,path);
           strcat(str3,nombre);
           strcat(str3,".");
           strcat(str3,extension);

           CrearDisco(sizeN,str3,disc);

        }

    }else{
        printf("Hace falta un atributo, para realizar con exito la operacion!!\n");
    }
}



/**RMDISK**/
void rmdisk(char* tokens){
    char* vector[20];
    int p=0;
    for(p=0;p<20;p++){
        vector[p]=NULL;
    }
    p=0;
    while(tokens!= NULL){
        tokens = strtok(NULL,"$");
        vector[p]= tokens;
        p++;
    }


    char* path= NULL;
    int valpath = 0;

    p = 0;
     while(vector[p]!= NULL){
         char* type;

        type = strtok(vector[p],"::");

        if (strcasecmp(type, "-path") == 0){
            path = strtok(NULL,":");
            valpath = 1;
        }else{
                   printf("\nESTE PARAMETRO NO ES VALIDO!!\n");
        }
        p++;
     }

      if (valpath == 1){

          char comando[120];


              int b=0;
              for(b=0;b<120;b++){
                  comando[b] = NULL;
              }

          printf("Esta seguro que quiere eliminar el disco? (Y/N):\n");

          fgets(&comando,120,stdin);//scanf("%d",&d);

          if(strcasecmp(comando, "n\n")==0){

          }else{
             EliminarDisco(path);
          }
            //printf("path %s \n", path);
          //crear disco
      }else{
          printf("Hace falta un atributo, para realizar con exito la operacion!!\n");
      }

}

/**FDISK**/
void fdisk(char *tokens)
{
    char* vector[20];

    int p=0;
    ///se limpiara el vector para evitar que tenga basura
    for(p=0; p<20;p++)
    {
        vector[p]=NULL;
    }

    p=0;
    while(tokens!=NULL)
    {
        tokens = strtok(NULL,"$");
        vector[p]=tokens;
        p++;
    }

    char* path= NULL;
    char* unit= "K"; ///se tomara en kilobytes por defecto en caso no venga ya que es opcional
    char* size= NULL;
    char* typee= "P";///se tomara que se creara una particion primaria ya que es opcional
    char* deletee= NULL;
    char* fit= "WF";///se tomara el peor ajuste
    char* name= NULL;
    char* add= NULL;




    int sizeN = 0;
    int addN=0;
    int tipito = 0;

    int valpath = 0;
    int valunit = 0;
    int valsize = 0;
    int valtype = 0;
    int valdelete = 0;
    int valfit = 0;
    int valname = 0;
    int valadd = 0;

    p = 0;
    while(vector[p]!= NULL){
        char* type;

       ///delimito por puntos dobles para obtener los siguientes comandos
       type = strtok(vector[p],"::");

       if (strcasecmp(type, "-size") == 0){
           size = strtok(NULL,"::");
           valsize = 1;
           //printf("ingresaste -size");
           sizeN = (int) strtol(size, NULL, 10);
           //printf("tamano: %d",sizeN);
       }else if(strcasecmp(type, "+unit") == 0){
           unit = strtok(NULL,"::");
           valunit = 1;
       }else if(strcasecmp(type, "-path") == 0){
           path = strtok(NULL,"::");
           valpath = 1;
       }else if(strcasecmp(type, "+type") == 0){
           typee = strtok(NULL,"::");
           valtype = 1;
       }else if(strcasecmp(type, "+delete") == 0){
           deletee = strtok(NULL,"::");
           valdelete = 1;
       }else if(strcasecmp(type, "+fit") == 0){
           fit = strtok(NULL,"::");
           valfit = 1;
       }else if(strcasecmp(type, "-name") == 0){
           name = strtok(NULL,"::");
           valname = 1;
       }else if(strcasecmp(type, "+add") == 0){
           add = strtok(NULL,"::");
           valadd = 1;
           //printf("parametro de agregar: %s",add);
           //addN =  (int) strtol(add, NULL, 10);
       }
       else{
                  printf("\nEL COMANDO NO EXISTE" ,",","DEBE INGRESAR LOS QUE SE LISTAN A CONTINUACION:\n\n",
                  "-size\n-path\n-name\n+type\n+delete\n+add\n+unit");
       }
        p++;
    }//cierre del while


    ///hare la comparacion de que si vienen los tres parametros obligatorios
    ///en este caso son size,name y path
    if(valpath==1 && valname==1 && valsize==1)
    {
        printf("\nya puedes crear la particion....\n");
        printf("Particion creada con:\n");
        printf("Nombre: %s",name);
        printf("\nSize: %d",sizeN);
        printf("\nPath: %s \n",path);

        int respuesta = CrearParticion(size,unit,path,typee,fit,name, valadd);

           if(respuesta==1){
               printf("%s\n","****************************No existe el disco****************************");
           }else if(respuesta==2){
               printf("%s\n", "****************************Nombre repetido****************************");
           }else if(respuesta==3){
               printf("%s\n","****************************Ya hay extendida****************************");
           }else if(respuesta==4){
               printf("%s\n","****************************Ya hay 3 primarias****************************");
           }else if(respuesta==5){
               printf("%s\n", "****************************Unidades Incorrecta****************************");
           }else if(respuesta==6){
               printf("%s\n", "****************************No hay espacio****************************");
           }else if(respuesta==7){
               printf("%s\n", "****************************No es tipo de Particion****************************");
           }else if(respuesta==8){
               printf("%s\n","****************************No existe el ajuste****************************");
           }else if(respuesta==9){
               printf("%s\n", "****************************No se puede Crear****************************");
           }else if(respuesta==10){
               printf("%s\n", "-----------------------Particion Creada con exito-----------------------");
           }else if (respuesta == 12){
                printf("%s\n", "****************************No existe particion Extendida****************************");
           }else if (respuesta == 11){
               printf("%s\n", "*****************Ya no se pueden crear mas particiones de este tipo****************************");
          }else if (respuesta == 13){
                printf("%s\n", "****************************No hay espacio para crear Logica****************************");
           }

    }else{
        printf("Hace falta un atributo, para realizar con exito la operacion!!\n");
    }





}

/**MOUNT**/
void mount(char* tokens){
    char* vector[20];
    int p=0;
    for(p=0;p<20;p++){
        vector[p]=NULL;
    }
    p=0;
    while(tokens!= NULL){
        tokens = strtok(NULL,"$");
        vector[p]= tokens;
        p++;
    }


    char* path= NULL;
    char* name= NULL;
    int valpath = 0;
    int valname = 0;


    p = 0;
     while(vector[p]!= NULL){
         char* type;

        type = strtok(vector[p],"::");

        if (strcasecmp(type, "-path") == 0){
            path = strtok(NULL,":");
            valpath = 1;
        }else if (strcasecmp(type, "-name") == 0){
            name = strtok(NULL,"::");
            valname = 1;
        }
        p++;
     }

      if (valpath == 1 && valname == 1){
          ///me falta hacer un metodo para que lo que me devuelva segun eso haga lo de montar en la particion
          ///int resultado= MonPart(path,name);
          if(resultado==1){
             // system("clear");
              printf("%s", "**************************No existe Disco**************************\n");
          }else if(resultado==2){
            //  system("clear");
              printf("%s","**************************No existe Particion**************************\n");
          }else if(resultado==3){
           //   system("clear");
              printf("%s","------------------------Particion Montada con Exito---------------------\n\n");

          }
          else if(resultado==4){
                      //  system("clear");
                        printf("%s","**************************Esta particion ya ha sido montada**************************\n");

                    }
          //montar
      }else{
          //error
      }

     // printf("path %s \n", path);
     // printf("nombre %s \n", name);

}
/**UMOUNT**/
void umount(char* tokens){

    char* vector[20];
    int p=0;
    for(p=0;p<20;p++){
        vector[p]=NULL;
    }
    p=0;
    while(tokens!= NULL){
        tokens = strtok(NULL,"$-");
        vector[p]= tokens;
        p++;
    }



    char* name= NULL;
    int valname = 0;


    p = 0;
     while(vector[p]!= NULL){
         char* type;

        type = strtok(vector[p],":");

         if (strcasecmp(type, "id") == 0){
            name = strtok(NULL,":");
            valname = 1;
        }
        p++;
     }

      if (valname == 1){
          int respuesta= UnMon(name);
          if(respuesta==1){
              printf("%s\n","**************************No existe ID**************************\n\n");
          }else if(respuesta==2){
              printf("%s\n","-------------------------Particion Desmontada------------------------\n\n");
          }
      }else{
          //error
      }

      printf("id %s \n", name);
}


int UnMon(char id[]){


    printf("\n\n~~~~~~~~~~~~~~~~~~~~~~~ Accion con %s ~~~~~~~~~~~~~~~~~~~~~~~~~~\n", id);
    int a=0;
    int encontrado=0;
    char path[100];
    char name[32];

    for(a = 0;a<40;a++){
        if(strcasecmp(id,montados[a].id)==0){

            montados[a].estado ='0';
            encontrado = 1;
            strcpy(path,montados[a].path);
            strcpy(name,montados[a].name);
        }
    }
    if(encontrado==0){
        return 1;
    }

    return 2;
}


/**CREARDISCO**/
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
//creo el archivo en la ruta especificada
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



/**ELIMINARDISCO**/
void EliminarDisco(char *nombre2){
    char pt[50];
    strcpy(pt,nombre2);

    printf("%s", nombre2);
    if(!remove(nombre2))
    {
       printf("\n---------------------Disco elimindo exitosamente!!----------------\n");

     }else{
        printf("\n****************************No existe disco a eliminar!!****************************\n");
     }
}


/**CREAR PARTICION**/
int CrearParticion(char* size, char* unit, char* path, char*  type, char* fit, char* name, int add)
{
    int tamanio= (int) strtol(size, NULL, 10);
    int bytes=0;

    printf("\n\n~~~~~~~~~~~~~~~~~~~~~~~ Creacion con %s ~~~~~~~~~~~~~~~~~~~~~~~~~~\n", name);

  //  printf("size tamaño  %d \n", tamanio);

    int tamanio_libre=0;
    FILE* disco;
    disco = fopen(path,"rb+");
    if(disco== NULL){
        return 1; //no existe disco
    }
    int particiones=0;

    MBR lectura;
    char fito[4];
    fseek(disco,0,SEEK_SET);
    fread(&lectura,sizeof(MBR),1,disco);
    char tipo[5];


    if(strcasecmp(unit,"k")==0){
        tamanio = tamanio*1024;
    }else if(strcasecmp(unit,"m")==0){
        tamanio = tamanio*1024*1024;
       } else if(strcasecmp(unit,"b")==0){

        }
    else {
        return 5; //unidades incorrectas
    }
    bytes = tamanio;
//    printf("size tamaño  %d \n", bytes);




    if(strcasecmp(type,"P")==0 || strcasecmp(type,"E")==0 || strcasecmp(type,"L")== 0){
        strcpy(tipo,type);
    }else {
        return 7;//no existe el tipo de particion
    }


    if(strcasecmp(fit,"B")==0 || strcasecmp(fit,"W")==0 || strcasecmp(fit,"F")==0){
        strcpy(fito,fit);
    }else{
        return 8;//no existe el ajuste
    }
}
