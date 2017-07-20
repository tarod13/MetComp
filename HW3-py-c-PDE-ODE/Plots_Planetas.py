# Se importan las librerias necesarias
import numpy as np
import pylab as plt
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.animation as animation

# Funcion que inicializa la animacion, mostrando cada uno de los planetas en sus
# posiciones iniciales.
def init():  
    texto1.set_text(r'$t=0.00$ y')
    texto2.set_text(r'$t=0.00$ y')
    for i in range(0,n_astros):
        astro[i].set_data(np.array([x[0,i], y[0,i]]))
        astro[i].set_3d_properties(z[0,i], 'z')
    for i in range(0,5):
        astro[i+n_astros].set_data(np.array([x[0,i], y[0,i]]))
        astro[i+n_astros].set_3d_properties(z[0,i], 'z')
    return astro
# Funcion que actualiz la posicion de los planetas en la animacion.
def animate(t): 
    t1 = int(t*60) 
    t2 = int(t) 
    tiempo1 = t*3/5
    tiempo2 = t/100 
    texto1.set_text((r'$t=$''%.2f 'r'y'  % tiempo1))
    texto2.set_text((r'$t=$''%.2f 'r'y' % tiempo2))
    for i in range(0,n_astros):        
        astro[i].set_data(np.array([x[t1,i], y[t1,i]]))
        astro[i].set_3d_properties(z[t1,i], 'z')
    for i in range(0,5):        
        astro[i+n_astros].set_data(np.array([x[t2,i], y[t2,i]]))
        astro[i+n_astros].set_3d_properties(z[t2,i], 'z')
    return astro    
# Se cargan los datos generados con el archivo Planetas.c.
posiciones = np.genfromtxt("trayectorias.csv",delimiter=',',skip_header=1)

# Se definen las constantes necesarias.
n_puntos = posiciones.shape[0]/3
n_astros = posiciones.shape[1]

# Se guardan los radios de los astros relativos a la tierra y se modifican para que las diferencias no sea tan grandes
radios = [109/17, 0.3829, 0.9499, 1.0, 0.533, 11.209/2, 9.4492/2, 4.007/2, 3.883/2, 0.1868*6]

# Se guardan en arreglos distintos las coordenadas x, y, z, de las posiciones de los astros.
id_x = np.array([3*i for i in range(0,np.int64(n_puntos))])
x = posiciones[id_x,:]
y = posiciones[id_x+1,:]
z = posiciones[id_x+2,:]

# Se crea un arreglo con colores similares a los de los astros.
colores = ['#ffce00','#7e7e7e','#f6ee91','#4682b4','#d2b48c','#f4a460','#eedd82','#afeeee','#87cefa','#d6ae7a']

# Se genera una grafica 3D de las trayectorias recorridas por los astros. Se modifica el
# tamanio de algunas de las orbitas para poder observar mas claramente las 9 orbitas.
fig1 = plt.figure(figsize=(12,6))
ax11 = fig1.add_subplot(121, projection='3d')
ax12 = fig1.add_subplot(122, projection='3d')
ax11.set_xlabel(r'$x$ (AU)', size = 9)
ax12.set_xlabel(r'$x$ (AU)', size = 9)
ax11.set_ylabel(r'$y$ (AU)', size = 9)
ax12.set_ylabel(r'$y$ (AU)', size = 9)
ax11.set_zlabel(r'$z$ (AU)', size = 9)
ax12.set_zlabel(r'$z$ (AU)', size = 9)
ax11.set_title(r'Órbitas planetarias del sistema solar', size = 9) 
ax12.set_title(r'Órbitas de los 4 primeros planetas del sistema solar', size = 9) 
ax11.tick_params(labelsize = 8)
ax12.tick_params(labelsize = 8)
for i in range(0,n_astros):
    ax11.plot(x[:,i], y[:,i], z[:,i], colores[i], linewidth = 0.016*i**2)    
for i in range(0,5):
    ax12.plot(x[:,i], y[:,i], z[:,i], colores[i], linewidth = 0.016*i**2)    
fig1.savefig('Planetas.jpeg',dpi=300)
plt.close()

# Se genera una animacion 3D de las trayectorias recorridas por los astros.
fig2 = plt.figure(figsize=(12,6))
astro = [[] for i in range(0,n_astros+5)]
ax21 = fig2.add_subplot(121, projection='3d')
ax22 = fig2.add_subplot(122, projection='3d')
ax21.set_xlabel(r'$x$ (AU)', size = 9)
ax22.set_xlabel(r'$x$ (AU)', size = 9)
ax21.set_ylabel(r'$y$ (AU)', size = 9)
ax22.set_ylabel(r'$y$ (AU)', size = 9)
ax21.set_zlabel(r'$z$ (AU)', size = 9)
ax22.set_zlabel(r'$z$ (AU)', size = 9)
ax21.set_title(r'Simulación del sistema solar con sus 8 planetas y Plutón', size = 10) 
ax22.set_title(r'Simulación del sistema solar con los 4 primeros planetas', size = 10) 
ax21.tick_params(labelsize = 8)
ax22.tick_params(labelsize = 8)
for i in range(0,n_astros):
    if i != 0:
    	ax21.plot(x[:,i], y[:,i], z[:,i], 'k', linewidth=0.004*2**np.amin((i,7)))
    astro[i], = ax21.plot([], [], [], color = colores[i], marker = 'o', markersize = radios[i])
for i in range(0,5):
    if i != 0:
    	ax22.plot(x[:,i], y[:,i], z[:,i], 'k', linewidth=0.00025*2**i)    
    	astro[i+n_astros], = ax22.plot([], [], [], color = colores[i], marker = 'o', markersize = radios[i]*5)
    else:
    	astro[i+n_astros], = ax22.plot([], [], [], color = colores[i], marker = 'o', markersize = radios[i]*2)
texto1 = ax21.text(-0.16, 0, 0.92,[], horizontalalignment='center', verticalalignment='center',
			transform = ax21.transAxes, bbox=dict(facecolor='none', edgecolor='red'))
texto2 = ax22.text(-0.16, 0, 0.92,[], horizontalalignment='center', verticalalignment='center',
			transform = ax22.transAxes, bbox=dict(facecolor='none', edgecolor='red'))
animacion_sisSolar = animation.FuncAnimation(fig2, animate, init_func=init, 
                               frames=int(n_puntos*63/75/60), interval=75, blit=True)
print("Guardando animacion... (Puede tardar un par de minutos)")                               
animacion_sisSolar.save('Planetas.mp4',dpi=300)
plt.close()
print("Proceso terminado")
