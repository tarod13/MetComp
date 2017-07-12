# Se importan las librerias necesarias
import numpy as np
import pylab as plt

# Funcion que deriva por medio del metodo central difference
def derivada_cd_1D(puntos,evaluaciones):
    tam = puntos.shape[0]    
    dif_puntos = puntos[2:tam,:] - puntos[0:tam-2,:]
    dif_eval =  evaluaciones[2:tam,:] - evaluaciones[0:tam-2,:]
    return dif_eval/dif_puntos

# Se cargan los datos
datos = np.genfromtxt('pot.dat')
posicion = np.transpose(np.array([datos[:,0]]))
potencial = np.transpose(np.array([datos[:,1]]))

# Se calcula el campo
campo = np.negative(derivada_cd_1D(posicion,potencial))

# Se grafica la figura
fig1, ax1 = plt.subplots()
ax1.scatter(posicion[1:posicion.shape[0]-1], campo)
ax1.grid(linestyle = '--', linewidth = 0.25)
plt.xlabel(r'r')
plt.ylabel(r'E(r)')
fig1.savefig('campo.pdf')
