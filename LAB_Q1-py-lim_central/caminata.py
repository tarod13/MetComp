import numpy as np
import pylab as plt
from scipy.stats import norm

#Lectura del archivo
lanzamientos = np.loadtxt('datos_CAMINATA.txt')

#Generacion de histograma de la primera caminata
plt.hist(lanzamientos[0],bins=10)
plt.savefig('binomial.png')
plt.close()

#Construccion de arreglo con la cantidad total de pasos por caminata
pasos_dados = np.sum(lanzamientos,axis=1)

#Fit de la distribucion normal
[loc,scale] = norm.fit(pasos_dados)

#Generacion de histograma del total de datos de todas las caminatas
plt.hist(pasos_dados,normed=True,bins=50)
x_norm = np.linspace(3300,3700,len(pasos_dados))
y_norm = norm.pdf(x_norm,loc,scale)
plt.plot(x_norm,y_norm)
plt.savefig('normal.png')
plt.close()

#Determinacion de la probabilidad binomial 
#a partir del teorema del limite central
mu_binom = loc/1000.0
p_binom = mu_binom/10
print('La probabilidad de sacar una cara con esta moneda es : %s' % p_binom)
