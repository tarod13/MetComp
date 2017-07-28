# Se importan las librerias necesarias
import numpy as np
import pylab as plt
from matplotlib.colors import ListedColormap

# Funcion que calcula longitud a partir del indice
def longitud(x):
    return((x/n_columnas - 0.5)*360.0)

# Funcion que calcula latitud a partir del indice
def latitud(x):
    return(-(x/n_filas - 0.5)*180.0)

# Funcion que halla las longitudes correspondientes a las latitutes, de tal
# manera que formen un circulo
def hallar_coordenadas_x(lat1,lat2,lon1):
    lat1 = lat1*np.pi/180
    lat2 = lat2*np.pi/180
    lon1 = lon1*np.pi/180
    semivseno_lat = (np.sin(np.absolute(lat1-lat2)/2))**2
    mul_cos = np.cos(lat1)*np.cos(lat2)
    #print(lat1,lat2,semivseno_lat,mul_cos),
    if(semivseno_rad-semivseno_lat < 0):
        delta_x = 0
    else:
        delta_x = 2*np.arcsin(np.sqrt((semivseno_rad-semivseno_lat)/mul_cos))
    x = np.array([lon1+delta_x,lon1-delta_x])*180/np.pi
    return(x)

def corregir_coordenadas_y(coordenadas_y, invertir):
    coordenadas_y_corr = np.copy(coordenadas_y)
    invertir_corr = np.copy(invertir)
    for i in range(0,n_puntos_circ):
        if(coordenadas_y_corr[i] >= 90):
            coordenadas_y_corr[i] = 90 - coordenadas_y_corr[i]%90
            invertir_corr[i] = 1
        else:
            if(coordenadas_y_corr[i] < -90):
                coordenadas_y_corr[i] = -90 + (-coordenadas_y_corr[i])%90
                invertir_corr[i] = 1
    return(coordenadas_y_corr, invertir_corr)

def corregir_coordenadas_x(coordenadas_x, invertir):
    coordenadas_x_corr = np.copy(coordenadas_x)
    for i in range(0,n_puntos_circ):
        for j in range(0,2):
            if(coordenadas_x_corr[i,j] >= 180):
                coordenadas_x_corr[i,j] = coordenadas_x_corr[i,j]%180
            else:
                if (coordenadas_x_corr[i,j] < -180):
                    coordenadas_x_corr[i,j] = 180 - (-coordenadas_x_corr[i,j])%180
    for i in range(0,n_puntos_circ):
        if(invertir[i]):
            for j in range(0,2):
                if(coordenadas_x_corr[i,j] >= 0):
                    coordenadas_x_corr[i,j] -= 180
                else:
                    coordenadas_x_corr[i,j] += 180
    return(coordenadas_x_corr)

# Se importan los datos
datos = np.genfromtxt("pmax.txt",delimiter=',')
mapa = np.genfromtxt("map_data.txt",delimiter=' ')
mapa = np.ones(mapa.shape) - mapa

# Se definen las constantes
corte_i = 0
corte_d = 0
n_filas = mapa.shape[0]
n_columnas = mapa.shape[1]-corte_d-corte_i
n_puntos_circ = 10000
R_tierra = 6371

# Coordenadas y radio del punto mas distante
x = datos[0];
y = datos[1];
r = datos[2];

semivseno_rad = (np.sin(r/(2*R_tierra)))**2
lon = (x/n_columnas - 0.5)*360.0
lat = -(y/n_filas - 0.5)*180.0

# Se calcula el circulo, teniendo en cuenta distancias ortodromicas
coordenadas_y = lat+np.linspace(-r,r,n_puntos_circ)*(180/(R_tierra*np.pi))
invertir = np.zeros(n_puntos_circ)
coordenadas_y_corr, invertir_corr = corregir_coordenadas_y(coordenadas_y, invertir)
coordenadas_x = np.zeros((n_puntos_circ,2))
for i in range(0,n_puntos_circ):
    coordenadas_x[i,:] = hallar_coordenadas_x(lat,coordenadas_y_corr[i],lon)
coordenadas_x_corr = corregir_coordenadas_x(coordenadas_x, invertir)

# Se grafica
rgba_array = plt.cm.ocean(np.linspace(0,1,num=20,endpoint=True))
extract_rgba_array_255 = rgba_array[1:10,0:3]
color_map = ListedColormap(extract_rgba_array_255)
fig1, ax1 = plt.subplots()
ax1.set_xlabel(r'Longitud', size = 9)
ax1.set_ylabel(r'Latitud', size = 9)
ax1.tick_params(labelsize = 8)
ax1.imshow(mapa[:,corte_i:n_columnas], cmap = color_map , extent=[-180,180,-90,90])
ax1.scatter(lon, lat, c='r', s = 5)
ax1.scatter(coordenadas_x_corr[:,0],coordenadas_y_corr,c='r',s=2)
ax1.scatter(coordenadas_x_corr[:,1],coordenadas_y_corr,c='r',s=2)
fig1.savefig('PuntoNemo.pdf',dpi=300)
plt.close()
