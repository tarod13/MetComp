# Se importan las librerias necesarias
import numpy as np
import pylab as plt
import numpy.linalg as lin

# Lectura del archivo
temp = np.genfromtxt('room-temperature.csv', delimiter=',', skip_header = 1, usecols = (1,2,3,4))

# Grafica de la temperatura en cada esquina del cuarto
fig1, ax1 = plt.subplots(nrows = 4,figsize=(14, 8))
ax1[0].plot(temp[:,0])
ax1[1].plot(temp[:,1])
ax1[2].plot(temp[:,2])
ax1[3].plot(temp[:,3])
ax1[0].set_ylabel(r'temperatura')
ax1[0].set_title(r'Front-Left')
ax1[1].set_ylabel(r'temperatura')
ax1[1].set_title(r'Front-Right')
ax1[2].set_ylabel(r'temperatura')
ax1[2].set_title(r'Back-Left')
ax1[3].set_xlabel(r'tiempo')                    
ax1[3].set_ylabel(r'temperatura')
ax1[3].set_title(r'Back-Right')
ax1[0].grid(linestyle = '--', linewidth = 0.25)
ax1[1].grid(linestyle = '--', linewidth = 0.25)
ax1[2].grid(linestyle = '--', linewidth = 0.25)
ax1[3].grid(linestyle = '--', linewidth = 0.25)
plt.savefig('temp.png')
plt.close()

# Se centran y normalizan los datos de temperatura
temp_cent_norm = np.zeros(temp.shape)
for column in range(0,4):
    mean = np.mean(temp[:,column])
    dev = np.cov(temp[:,column])
    temp_cent_norm[:,column] = (temp[:,column] - mean)/np.sqrt(dev)

# Calculo de la matriz de covarianza
matriz_cov_temp = np.cov(temp_cent_norm.T)
print('La matriz de covarianza es:\n %s \n' % matriz_cov_temp)

# Calculo de las componentes principales
[val_propios,vec_propios] = lin.eig(matriz_cov_temp)
cp1 = vec_propios[:,0]
cp2 = vec_propios[:,1]
val1 = val_propios[0]
val2 = val_propios[1]
print('La primera componente principal es %s con valor %.3f' % (cp1,val1))
print('La segunda componente principal es %s con valor %.3f\n' % (cp2,val2))

# Calculo del aporte de las componentes principales cp1 y cp2
suma_var = np.sum(val_propios)
aporte_cp1 = val1/suma_var*100.0
aporte_cp2 = val2/suma_var*100.0
print('La primera componente principal explica el %.2f %% de la varianza.' % aporte_cp1)
print('La segunda componente principal explica el %.2f %% de la varianza.\n' % aporte_cp2)

Npuntos = 200
pendiente_1_FRFL = cp1[1]/cp1[0]
pendiente_2_FRFL = cp2[1]/cp2[0]
min_FL = temp_cent_norm[:,0].min()
max_FL = temp_cent_norm[:,0].max()
eje_FL = np.linspace(min_FL-0.1,max_FL+0.1,Npuntos)
cp1_linea_FRFL = pendiente_1_FRFL*eje_FL
cp2_linea_FRFL = pendiente_2_FRFL*eje_FL
pendiente_1_BLFL = cp1[2]/cp1[0]
pendiente_2_BLFL = cp2[2]/cp2[0]
cp1_linea_BLFL = pendiente_1_BLFL*eje_FL
cp2_linea_BLFL = pendiente_2_BLFL*eje_FL

fig2, ax2 = plt.subplots()
ax2.scatter(temp_cent_norm[:,0],temp_cent_norm[:,1])
linea_cp1_FRFL, = ax2.plot(eje_FL,cp1_linea_FRFL,'r', label='Primera componente principal')
linea_cp2_FRFL, = ax2.plot(eje_FL,cp2_linea_FRFL, 'y', label='Segunda componente principal')
ax2.set_xlim([min_FL-0.1, max_FL+0.1])
ax2.set_xlabel('Temperatura en la esquina fontral izquierda del cuarto')
ax2.set_ylabel('Temperatura en la esquina frontal derecha del cuarto')
ax2.grid(linestyle = '--', linewidth = 0.25)
plt.legend(handles=[linea_cp1_FRFL, linea_cp2_FRFL])
plt.savefig('pca_fr_fl.pdf')
plt.close()

fig3, ax3 = plt.subplots()
ax3.scatter(temp_cent_norm[:,0],temp_cent_norm[:,2])
linea_cp1_BLFL, = ax3.plot(eje_FL,cp1_linea_BLFL,'r', label='Primera componente principal')
linea_cp2_BLFL, = ax3.plot(eje_FL,cp2_linea_BLFL, 'y', label='Segunda componente principal')
ax3.set_xlim([min_FL-0.1, max_FL+0.1])
ax3.set_xlabel('Temperatura en la esquina fontral izquierda del cuarto')
ax3.set_ylabel('Temperatura en la esquina posterior izquierda del cuarto')
ax3.grid(linestyle = '--', linewidth = 0.25)
plt.legend(handles=[linea_cp1_BLFL, linea_cp2_BLFL])
plt.savefig('pca_bl_fl.pdf')
plt.close()
