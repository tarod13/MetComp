#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define t_sim 2048
#define n_astros 10
#define n_dim 3
#define runge_kutta_n 4
#define n_columnas 8

const float G = 1.99350e-44;

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

void leer_archivo(SistemaSolar* apuntador_sisSolar, char* nombre_archivo){
	FILE *archivo = fopen(nombre_archivo, "r");	
	int max_long = 1024;
	char linea[max_long];
	int astro_id = 0;
	
	printf("Leyendo archivo\n");
	while (fgets(linea, max_long, archivo)){	         
        for(int columna = 0; columna < n_columnas; columna++){   
        	//strdup copia al string linea y lo guarda en memoria, retornando la ubicacion de la copia
        	char* copia_linea = strdup(linea);         
		    char* dato = obtener_columna(copia_linea, columna);
		    switch (columna){
		    	case 0: strcpy((*apuntador_sisSolar).astros[astro_id].nombre, dato); break;
		    	case 1: (*apuntador_sisSolar).astros[astro_id].masa = strtod(dato,NULL); break;
		    	case 2: 
		    	case 3: 
		    	case 4: (*apuntador_sisSolar).astros[astro_id].posicion[columna-2][0] = strtod(dato,NULL); break;
		    	case 5: 
		    	case 6: 
		    	case 7: (*apuntador_sisSolar).astros[astro_id].velocidad[columna-5][0] = strtod(dato,NULL); break;		    	
		    }
		    free(copia_linea);		    		    	    
    	}    	    	
    	astro_id++;
    	printf("\n");		
    }
    
    fclose(archivo);    
}

float** calculo_velocidad(SistemaSolar* apuntador_sisSolar, float** dv, int t){
	float** vel = (float **)malloc(n_dim * sizeof(float*));
	for(int m=0;m<n_dim;m++) vel[m] = (float *)malloc(n_astros * sizeof(float));
	
	for(int i=0;i<n_dim;i++)
		for(int j=0;j<n_astros;j++) 	
			vel[i][j] = (*apuntador_sisSolar).astros[j].velocidad[i][t] + dv[i][j];	
	return vel;
}

float** calculo_aceleracion(SistemaSolar* apuntador_sisSolar, float** dx, int t){
	float** pos = (float **)malloc(n_dim * sizeof(float*));
	for(int m=0;m<n_dim;m++) pos[m] = (float *)malloc(n_astros * sizeof(float));
	float** acel = (float **)malloc(n_dim * sizeof(float*));
	for(int m=0;m<n_dim;m++) acel[m] = (float *)malloc(n_astros * sizeof(float));
	
	for(int i=0;i<n_dim;i++)
		for(int j=0;j<n_astros;j++){
			pos[i][j] = (*apuntador_sisSolar).astros[j].posicion[i][t] + dx[i][j];
			acel[i][j] = 0.0;
		}
	for(int j=0;j<n_astros;j++)		
		for(int k=0;k<n_astros;k++)
			if(j!=k){
				float r_jk[n_dim] = {pos[0][k]-pos[0][j],pos[1][k]-pos[1][j],pos[2][k]-pos[2][j]};
				float d = sqrt(r_jk[0]*r_jk[0] + r_jk[1]*r_jk[1] + r_jk[2]*r_jk[2]);
				float c = G*(*apuntador_sisSolar).astros[k].masa/(d*d*d);
				for(int h=0;h<n_dim;h++) 
					acel[h][j] += c*r_jk[h];			
			}
		
	return acel; 
}

void runge_kutta_4(SistemaSolar* apuntador_sisSolar, float dt){
	for(int t=1;t<t_sim;t++){
		float** dx = (float **)malloc(n_dim * sizeof(float*));
		for(int m=0;m<n_dim;m++) dx[m] = (float *)malloc(n_astros * sizeof(float));
		float** dv = (float **)malloc(n_dim * sizeof(float*));
		for(int m=0;m<n_dim;m++) dv[m] = (float *)malloc(n_astros * sizeof(float));
		float k1[runge_kutta_n][n_dim][n_astros], k2[runge_kutta_n][n_dim][n_astros];
				
		for(int i=0;i<n_astros;i++)
			for(int h=0;h<n_dim;h++){			
				dx[h][i] = 0.0;
				dv[h][i] = 0.0;
			}
			
		float **vel, **acel;
		
		for(int m=0;m<runge_kutta_n;m++){
			vel = calculo_velocidad(apuntador_sisSolar, dv, t-1);
			acel = calculo_aceleracion(apuntador_sisSolar, dx, t-1);
			float coef;
			switch(m){
				case 0: coef = 1.0/2.0; break;
				case 1: coef = 1.0/2.0; break;
				case 2: coef = 1.0; break;
				case 3: coef = 0.0; break;
			}			 
			for(int i=0;i<n_astros;i++)
				for(int h=0;h<n_dim;h++){
					k1[m][h][i] = dt*vel[h][i];
					k2[m][h][i] = dt*acel[h][i];
					dx[h][i] = coef*k1[m][h][i];
					dv[h][i] = coef*k2[m][h][i]; 
				}
		} 
		
		for(int i=0;i<n_astros;i++)
			for(int h=0;h<n_dim;h++){
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
	
	char nombre_archivo[30] = "coordinates.csv";
	SistemaSolar sisSolar;
	float dt = 0.01;
	
	leer_archivo(&sisSolar, nombre_archivo);
    runge_kutta_4(&sisSolar, dt);
    
    for(int astro_id = 0; astro_id < n_astros; astro_id++) 
    	for(int t=0;t<4;t++){
    		printf("La trayectoria del astro %d es: [%f, %f, %f]\t", astro_id, sisSolar.astros[astro_id].posicion[0][t],
    	 	sisSolar.astros[astro_id].posicion[1][t], sisSolar.astros[astro_id].posicion[2][t]);
    		printf("\n");
    	}
    	
    return 0;
}
