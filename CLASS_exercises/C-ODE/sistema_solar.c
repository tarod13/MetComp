#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char nombre_archivo[100] = "coordinates.csv";

struct Planeta {
	char 	nombre;
	float	masa;
   	float	posicion[3][1024];
   	float	velocidad[3][1024];
};

struct 	SistemaSolar {
   struct Planeta planetas[100];   
};

char* obtener_columna(char* linea, int numero_columna)
{
    char* token;
    //strtok divide al string linea y retorna el puntero del primer substring
    for (token = strtok(linea, ","); token && *token; token = strtok(NULL, ",\n"))	
    {
        if (!--numero_columna)
            return token;
    }
    return NULL;
}

int main(){
	
	struct SistemaSolar sistema_solar;
	
	FILE *archivo = fopen(nombre_archivo, "r");	
	printf("Leyendo...\n");
	char* linea[1024];
	for(int i = 1;i<9;i++){
		if(i == 1){

		    while (fgets(linea, 1024, archivo)){
			    //strdup copia al string linea y lo guarda en memoria, retornando la ubicacion de la copia
		        char* copia_linea = strdup(linea);
		        nombres_planetas = obtener_columna(copia_linea, 1);
		        free(copia_linea);
		    }
    
		    fclose(archivo);
		}
    }

    return 0;
}
