#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char nombre_archivo[100] = "coordinates.csv";

typedef struct{	
	char 	nombre[15];
	float	masa;
   	float	posicion[3][1024];
   	float	velocidad[3][1024];   	
} Astro;

typedef struct{
	Astro astros[10];   
} SistemaSolar;

char* obtener_columna(char* linea, int columna){    
    //strtok divide al string linea y retorna el puntero del primer substring
    char* token = strtok(linea, ",");
    while(columna){
    	token = strtok(NULL, ",\n");	
    	columna--;
    }
    return token;
}

int main(){
	
	SistemaSolar sisSolar;
	
	FILE *archivo = fopen(nombre_archivo, "r");	
	printf("Leyendo...\n");
	char linea[1024];
	int astro_id = 0;
	while (fgets(linea, 1024, archivo)){	         
        for(int columna = 0; columna < 8; columna++){   
        	//strdup copia al string linea y lo guarda en memoria, retornando la ubicacion de la copia
        	char* copia_linea = strdup(linea);         
		    char* dato = obtener_columna(copia_linea, columna);
		    printf("%s\t", dato);		    		    		    		    
		    switch (columna){
		    	case 0: strcpy( sisSolar.astros[astro_id].nombre, dato); break;
		    	case 1: sisSolar.astros[astro_id].masa = strtod(dato,NULL); break;
		    	case 2: 
		    	case 3: 
		    	case 4: sisSolar.astros[astro_id].posicion[columna-2][0] = strtod(dato,NULL); break;
		    	case 5: 
		    	case 6: 
		    	case 7: sisSolar.astros[astro_id].velocidad[columna-5][0] = strtod(dato,NULL); break;		    	
		    }
		    free(copia_linea);		    		    	    
    	}    	    	
    	astro_id++;
    	printf("\n");		
    }
    
    fclose(archivo);
    
    for(astro_id = 0; astro_id < 9; astro_id++){ 
    	printf("El astro %d es: %s,\n su masa es: %f\n, su posicion es: [%f, %f, %f]\n y su velocidad es: [%f, %f, %f]\n\n",
    	 astro_id, sisSolar.astros[astro_id].nombre, sisSolar.astros[astro_id].masa, sisSolar.astros[astro_id].posicion[0][0], 
    	 sisSolar.astros[astro_id].posicion[1][0], sisSolar.astros[astro_id].posicion[2][0],sisSolar.astros[astro_id].velocidad[0][0], 
    	 sisSolar.astros[astro_id].velocidad[1][0], sisSolar.astros[astro_id].velocidad[2][0]);
	}
	
    return 0;
}
