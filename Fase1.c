#include <stdio.h>
#include <stdlib.h>
#include <time.h>


char comando[60];
char comandoTotal[128];
int diagonal=0;
int op =0;


/**DECLARACION DE METODOS**/
void quitarSaltoLinea();
void Analizador(char *linea);
void mkdisk(char *token);
//todos los demas comandos que piden

/**METODO PRINCIPAL MAIN**/
int main()
{

    int q=0;
    for(q=0;q<128;q++)
    {
        comandoTotal[q] = NULL;
    }

    while (strcasecmp(comando, "exit") != 0)
    {

        /**lo que realizo en este for es limpiar mi variable y evitar que tenga basura**/
        int b=0;
        for(b=0;b<128;b++)
        {
            comando[b] = NULL;
        }

        if(diagonal == 0)
        {
            printf("root@201313720#: ");
        }

        fgets(&comando,120,stdin);
        quitarSaltoLinea();
        //aca voy a quitar lo que son los espacios en blanco para que no me afecte
        //lo que voy a realizar mas adelante..........
    }

 return 0;

}

void quitarSaltoLinea()
{

    int contador = 0;
    int c=0;

       while(comando[contador] != NULL)
       {
           contador++;
       }
       /**esta linea de codigo nos dice que va a agarrar la ultima letra
       que viene en este caso seria n y la pone en null**/
       comando[contador-1] = NULL;
       /**aca lo que hace es agarrar la diganal y sustituirla por un espacio en blanco**/
       if (comando[contador-2] == '\\'){
           comando[contador-2] = ' ';
           diagonal = 1;
       }else{
           diagonal = 0;
       }
}

void Analizador(char *linea)
{

    char *token;
    token = strtok(linea, "$"); /*Separamos por espacios*/

    if (strcasecmp(token, "mkdisk") == 0){
       //aca voy a mandar a llamar a mi metodo de mkdisk
       //mkdisk(token);
       printf("Token: %s",token);
       printf("Linea: %s",linea);
    }
}
