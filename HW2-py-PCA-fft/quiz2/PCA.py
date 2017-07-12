import numpy as np
import pylab as plt
import numpy.linalg as lin

temperatura = np.genfromtxt('room-temperature.csv',delimiter=',',skip_header=1,usecols=[1,2,3,4])

temperatura_normalizada = (temperatura - np.mean(temperatura,axis=0))/np.std(temperatura,axis = 0)
matriz_covarianza = np.cov(temperatura_normalizada.T)

valores_p, vectores_p = lin.eig(matriz_covarianza)
PC1 = vectores_p[:,0]
PC2 = vectores_p[:,1]
print("PC1 es: [ %.3f" % PC1[0])
for i in range(1,PC1.shape[0]):
      print(", %.3f" % PC1[i])
print(" ] y PC2 es: [ %.3f" % PC2[0])
for i in range(1,PC2.shape[0]):
      print(", %.3f" % PC2[i])
      
fig1, ax1 = plt.subplots()
colormap = ['yellow','red','green','blue']
etiquetas = ['Frontal izquierdo','Frontal derecho','Posterior izquierdo','Posterior derecho']
for i in range(0,4):
    ax1.scatter(PC1[i],PC2[i], c = colormap[i], s = 10, label = etiquetas[i])
ax1.set_xlabel('Primera componente')
ax1.set_ylabel('Segunda componente')
ax1.legend(etiquetas)
fig1.savefig('Agrupaciones.pdf')
plt.close()
