# Se importan las librerias necesarias
import numpy as np
import pylab as plt
import pandas as pd
import numpy.linalg as lin

# Funcion que calcula el valor medio de una muestra, suponiendo que cada dato
# corresponde a un vector columna
# Parametros: muestra - matriz
# Salidas: media muestral como vector columna 
def media_muestral(muestra):
    suma = np.transpose(np.array([np.sum(muestra,axis=1)]))
    return suma/muestra.shape[1]

# Funcion que calcula la desviacion estandar de una muestra, suponiendo que cada dato
# corresponde a un vector columna
# Parametros: muestra - matriz
# Salidas: desviacion estandar como vector columna 
def desviacion_estandar(muestra):
    cuadrados = (muestra-media_muestral(muestra))**2
    suma = np.transpose(np.array([np.sum(cuadrados,axis=1)]))
    return np.sqrt(suma)/muestra.shape[1]

# Funcion que calcula la matriz de covarianza de una muestra de vectores,
# tal que se asume que cada vector está normalizado y que corresponde a 
# una columna de la matriz recibida. Para el calculo se suman los productos
# punto de los vectores columna con su transpuesta y despues se divide sobre
# la cantidad total de vectores menos 1
# Parametros: muestra - matriz cuyas columnas son vectores normalizados
# Salidas: matriz de covarianza muestral
def covarianza_muestral(muestra):
    suma = np.zeros([muestra.shape[0],muestra.shape[0]])
    for i in range(0,muestra.shape[1]):
        columna = np.transpose(np.array([muestra[:,i]]))
        suma = suma + np.dot(columna,np.transpose(columna))        
    return suma/(muestra.shape[1]-1)   
    
# Se lee el archivo con los datos a partir de la funcion read_csv
# de pandas, que es después convertido a una matriz de numpy
datosBM = pd.read_csv('DatosBancoMundial5.csv').as_matrix()
datosBM = np.float64(datosBM[:,4:])

# Se normalizan los datos restando la media y dividiendo sobre la varianza 
# de cada variable
media = media_muestral(datosBM)
desviacion = desviacion_estandar(datosBM)
datosBM_normalizados = (datosBM - media)/desviacion

# Se grafica cada variable contra la region o el grupo correspondiente
fig1, ax1 = plt.subplots(nrows = 5)
for i in range(0,5):
    ax1[i].plot(datosBM_normalizados[i,:], linewidth = 0.4)
    ax1[i].grid(linestyle = '--', linewidth = 0.3)
    ax1[i].tick_params(labelsize=4)
    plt.setp(ax1[i].get_xticklabels(), visible=False)
ax1[0].set_ylabel(r'Tasa de impuestos', fontsize=3)
ax1[1].set_ylabel(r'Costo start-up', fontsize=3)
ax1[2].set_ylabel(r'Desempleo femenino', fontsize=3)
ax1[3].set_ylabel(r'Desempleo masculino', fontsize=3)
ax1[4].set_ylabel(r'Razón hombres/mujeres,'"\n" r'trabajando', fontsize=3)
plt.setp(ax1[4].get_xticklabels(), visible=True)
fig1.savefig('ExploracionDatos.pdf')
plt.close()

# Se calcula la matriz de covarianza
matriz_covarianza = covarianza_muestral(datosBM_normalizados)

# Se hallan las componentes principales por medio de la funcion eig de la 
# libreria numpy.linalg y 
[val_propios,vec_propios] = lin.eig(matriz_covarianza)
cp1 = vec_propios[:,0]
cp2 = vec_propios[:,1]
val1 = val_propios[0]
val2 = val_propios[1]

# Se imprime en la terminal las dos primeras componentes
print('el componente principal es: [%.3f' % cp1[0], end="")
for i in range(1,5):
    print('; %.3f' % cp1[i], end="")
print('],\n el segundo componente principal es: [%.3f' % cp2[0], end="")
for i in range(1,5):
    print('; %.3f' % cp2[i], end="")
print(']')

# Se proyectan los datos sobre las dos componentes principales por medio del
# producto punto entre los datos y las componentes. Esto debido a que el producto
# punto con un vector unitario es equivalente a la proyeccion sobre este.
base = np.array([cp1,cp2])
datosBM_proyectados = np.dot(base,datosBM_normalizados)

# Se grafican los datos proyectados, teniendo como eje x al componente principal
# y como eje y al segundo
fig2, ax2 = plt.subplots()
ax2.scatter(datosBM_proyectados[0,:], datosBM_proyectados[1,:], s=2, c='g')
ax2.grid(linestyle = '--', linewidth = 0.4)
ax2.tick_params(labelsize=8)
ax2.set_xlabel(r'Primera componente principal', fontsize=8)
ax2.set_ylabel(r'Segunda componente principal', fontsize=8)
fig2.savefig('PCAdatos.pdf')
plt.close()

# Se grafican las variables originales en el sistema de referencia de los dos primeros
# componentes principales, teniendo en cuenta que estas variables corresponden a los 5
# ejes originales, y por tanto su proyeccion corresponde a las entradas de las
# componentes principales
fig3, ax3 = plt.subplots()
cmap = ['blue', 'red', 'green', 'yellow', 'black']
etiquetas = [("Variable "+str(i+1)) for i in range(0,5)]
variables = ['1', '2', '3', '4', '5']
p = []
for var in range(0,5):
    q, = ax3.plot(cp1[var], cp2[var], color = cmap[var], marker = 'o', markersize = 10, label=etiquetas[var])
    p.append(q)
ax3.grid(linestyle = '--', linewidth = 0.8)
ax3.tick_params(labelsize=8)
ax3.set_xlabel(r'Primera componente principal', fontsize=8)
ax3.set_ylabel(r'Segunda componente principal', fontsize=8)
plt.legend(handles = p)
fig3.savefig('PCAvariables.pdf')
plt.close()

# Se imprimen los grupos correlacionados
print("las variables que están correlacionadas son:")
print("1.Impuestos y 2.Costos de start-up; y")
print("3.Desempleo femenino y 4.Desempleo masculino")

# Se avisa cuando termina la ejecucion        
print('Proceso terminado.')
