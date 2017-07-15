#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define G 9

int t_sim = 2048;
int n_astros = 10;
int n_dim = 3;

typedef struct{	
	char 	nombre[15];
	float	masa;
   	float	posicion[n_dim][t_sim];
   	float	velocidad[n_dim][t_sim];   	
} Astro;

typedef struct{
	Astro astros[n_astros];   
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

float** calculo_velocidad(SistemaSolar* apuntador_sisSolar, float** dv, int t){
	float** vel = (float **)malloc(n_dim * sizeof(float*));
	for(int m=0;m<n_dim;m++) vel[m] = (float *)malloc(n_astros * sizeof(float));
	
	for(int i=0:i<3;i++)
		for(int j=0;j<10;j++)			
			vel[i][j] = (*apuntador_sisSolar).astros[j].velocidad[i][t] + dv[i][j];	
	return vel;
}

float** calculo_aceleracion(SistemaSolar* apuntador_sisSolar, float** dx, int t){
	float** pos = (float **)malloc(n_dim * sizeof(float*));
	for(int m=0;m<n_dim;m++) pos[m] = (float *)malloc(n_astros * sizeof(float));
	float** acel = (float **)malloc(n_dim * sizeof(float*));
	for(int m=0;m<n_dim;m++) acel[m] = (float *)malloc(n_astros * sizeof(float));
	
	for(int i=0:i<3;i++)
		for(int j=0;j<10;j++){
			pos[i][j] = (*apuntador_sisSolar).astros[j].posicion[i][t] + dx[i][j];
			acel[i][j] = 0.0;
		}
	for(int j=0;j<10;j++){		
		for(int k=0;k<10;k++){
			float r_jk[3] = {pos[0][k]-pos[0][j],pos[1][k]-pos[1][j],pos[2][k]-pos[2][j]}
			float d = sqrt(r_jk[0]*r_jk[0] + r_jk[1]*r_jk[1] + r_jk[2]*r_jk[2])
			float c = G*(*apuntador_sisSolar).astros[k].masa/(d*d*d)
			for(int h=0;h<3;h++)
				acel[h][j] += c*r_jk[h];			
		}	
	}	
	return acel; 
}

void runge_kutta_4(SistemaSolar* apuntador_sisSolar, float dt){
	for(int t=1;t<t_sim;t++){
		float** dx = (float **)malloc(n_dim * sizeof(float*));
		for(int m=0;m<n_dim;m++) dx[m] = (float *)malloc(n_astros * sizeof(float));
		float** dv = (float **)malloc(n_dim * sizeof(float*));
		for(int m=0;m<n_dim;m++) dv[m] = (float *)malloc(n_astros * sizeof(float));
		float k1[4][n_dim][n_astros], k2[4][n_dim][n_astros];
				
		for(int i=0:i<10;i++)
			for(int h=0;h<3;h++){			
				dx[h][i] = 0.0;
				dv[h][i] = 0.0;
			}
			
		float** vel, acel;
		
		for(int m=0;m<4;m++){
			vel = calculo_velocidad(apuntador_sisSolar, dv, t-1);
			acel = calculo_aceleracion(apuntador_sisSolar, dx, t-1);
			float coef;
			switch(m){
				case 0: coef = 1.0/2.0; break;
				case 1: coef = 1.0/2.0; break;
				case 2: coef = 1.0; break;
				case 3: coef = 0.0; break;
			}			 
			for(int i=0;i<10;i++)
				for(int h=0;h<3;h++){
					k1[m][h][i] = dt*vel[h][i];
					k2[m][h][i] = dt*acel[h][i];
					dx[h][i] = coef*k1[m][h][i];
					dv[h][i] = coef*k2[m][h][i]; 
				}
		} 
		
		for(int i=0;i<10;i++)
			for(int h=0;h<3;h++){
				(*apuntador_sisSolar).astros[i].posicion[h][t] = 
					(*apuntador_sisSolar).astros[i].posicion[h][t-1] + 
					(k1[0][h][i]+2*k1[1][h][i]+2*k1[2][h][i]+k1[3][h][i])/6.0;
				(*apuntador_sisSolar).astros[i].velocidad[h][t] = 
					(*apuntador_sisSolar).astros[i].velocidad[h][t-1] + 
					(k2[0][h][i]+2*k2[1][h][i]+2*k2[2][h][i]+k1[3][h][i])/6.0;
			}				
	}
}

int main(){
	
	char nombre_archivo[100] = "coordinates.csv";
	SistemaSolar sisSolar;
	FILE *archivo = fopen(nombre_archivo, "r");	
	printf("Leyendo...\n");
	char linea[1024];
	int astro_id = 0;
	float dt = 0.01;
	
	while (fgets(linea, 1024, archivo)){	         
        for(int columna = 0; columna < 8; columna++){   
        	//strdup copia al string linea y lo guarda en memoria, retornando la ubicacion de la copia
        	char* copia_linea = strdup(linea);         
		    char* dato = obtener_columna(copia_linea, columna);
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
