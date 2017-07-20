# Se importan las librerias necesarias
import numpy as np
import pylab as plt
import matplotlib.animation as ani

# Se asume que la velocidad inicial de la onda es 0, por lo que aproximando la derivada con la diferencia central
# se obtiene que la onda es la misma el instante -1 y el instante 1. De esta manera la funcion calcula la onda en
# el instante 1 aplicando la igualdad en la ecuacion de diferencias que resulta de discretizar la ecuacion de onda.
# Parametros:	z0 - matriz con el valor de la onda en el instante 0 
# Salidas: 		z1 - matriz con el valor de la onda en el instante 1  
def evolucion_inicial_onda(z0):
    z1 = np.zeros(z0.shape)
    z1[1:n_puntos_x-1,1:n_puntos_y-1] = (1-gamma_x**2-gamma_y**2)*z0[1:n_puntos_x-1,1:n_puntos_y-1] \
        + gamma_x**2/2*(z0[2:n_puntos_x,1:n_puntos_y-1]+z0[0:n_puntos_x-2,1:n_puntos_y-1]) \
        + gamma_y**2/2*(z0[1:n_puntos_x-1,2:n_puntos_y]+z0[1:n_puntos_x-1,0:n_puntos_y-2]) 
    return z1

# Esta funcion aplica la ecuacion de diferencias que resulta de discretizar la ecuacion de onda.
# Parametros: 	z0 - matriz con el valor de la onda en el instante t-2, 
#				z1 - matriz con el valor de la onda en el instante t-1,	 
# Salidas: 		z2 - matriz con el valor de la onda en el momento t  
def evolucion_onda(z0,z1):
    z2 = np.zeros(z0.shape)
    z2[1:n_puntos_x-1,1:n_puntos_y-1] = 2*(1-gamma_x**2-gamma_y**2)*z1[1:n_puntos_x-1,1:n_puntos_y-1] \
        + gamma_x**2*(z1[2:n_puntos_x,1:n_puntos_y-1]+z1[0:n_puntos_x-2,1:n_puntos_y-1]) \
        + gamma_y**2*(z1[1:n_puntos_x-1,2:n_puntos_y]+z1[1:n_puntos_x-1,0:n_puntos_y-2]) \
        - z0[1:n_puntos_x-1,1:n_puntos_y-1]   
    return z2

# Se asume que la onda satisface la condicion d phi/dn = 0, donde n hace referencia a la direccion normal al borde
# considerado (hay una reflexión perfecta de la onda en la direccion n). Esto genera una igualdad entre la onda en
# la posicion n-1 y n+1, lo que, de manera similar al caso de la funcion evolucion_inicial_onda, se reemplaza en la
# ecuacion de diferencias que resulta de discretizar la ecuacion de onda.
# Parametros: 	z0 - matriz con el valor de la onda en el instante t-2, 
#				z1 - matriz con el valor de la onda en el instante t-1,
#				z2 - matriz con el valor de la onda en el momento t	 
# Salidas: 		z_bordes - matriz con el valor de la onda en el momento t, con los bordes actualizados para tener
#				en cuenta la condicion de reflexion total      
def evolucion_bordes(z0,z1,z2):
    z_bordes = np.copy(z2)
    
    # Borde superior e inferior
    z_bordes[1:n_puntos_x-1,0] = 2*(1-gamma_x**2-gamma_y**2)*z1[1:n_puntos_x-1,0] \
        + gamma_x**2*(z1[2:n_puntos_x,0]+z1[0:n_puntos_x-2,0]) \
        + 2*gamma_y**2*z1[1:n_puntos_x-1,1] - z0[1:n_puntos_x-1,0]
    z_bordes[1:n_puntos_x-1,n_puntos_y-1] = 2*(1-gamma_x**2-gamma_y**2)*z1[1:n_puntos_x-1,n_puntos_y-1] \
        + gamma_x**2*(z1[2:n_puntos_x,n_puntos_y-1]+z1[0:n_puntos_x-2,n_puntos_y-1]) \
        + 2*gamma_y**2*z1[1:n_puntos_x-1,n_puntos_y-2] - z0[1:n_puntos_x-1,n_puntos_y-1]
        
    # Bordes superior e inferior de la barrera izquierda 
    z_bordes[1:x_perturbacion-2,y_barrera] = 2*(1-gamma_x**2-gamma_y**2)*z1[1:x_perturbacion-2,y_barrera] \
        + gamma_x**2*(z1[2:x_perturbacion-1,y_barrera]+z1[0:x_perturbacion-3,y_barrera]) \
        + 2*gamma_y**2*z1[1:x_perturbacion-2,y_barrera-1] - z0[1:x_perturbacion-2,y_barrera]
    z_bordes[1:x_perturbacion-2,y_barrera+1] = 2*(1-gamma_x**2-gamma_y**2)*z1[1:x_perturbacion-2,y_barrera+1] \
        + gamma_x**2*(z1[2:x_perturbacion-1,y_barrera+1]+z1[0:x_perturbacion-3,y_barrera+1]) \
        + 2*gamma_y**2*z1[1:x_perturbacion-2,y_barrera+2] - z0[1:x_perturbacion-2,y_barrera+1]
    
    # Extremos derechos de la barrera izquierda    
    z_bordes[x_perturbacion-2,y_barrera] = 2*(1-gamma_x**2-gamma_y**2)*z1[x_perturbacion-2,y_barrera] \
        + 2*gamma_x**2*z1[x_perturbacion-1,y_barrera] + 2*gamma_y**2*z1[x_perturbacion-2,y_barrera-1] \
        - z0[x_perturbacion-2,y_barrera]
    z_bordes[x_perturbacion-2,y_barrera+1] = 2*(1-gamma_x**2-gamma_y**2)*z1[x_perturbacion-2,y_barrera+1] \
        + 2*gamma_x**2*z1[x_perturbacion-1,y_barrera+1] + 2*gamma_y**2*z1[x_perturbacion-2,y_barrera+2] \
        - z0[x_perturbacion-2,y_barrera+1]
    
    # Bordes superior e inferior de la barrera derecha
    z_bordes[x_perturbacion+3:n_puntos_x-1,y_barrera] = \
        2*(1-gamma_x**2-gamma_y**2)*z1[x_perturbacion+3:n_puntos_x-1,y_barrera] \
        + gamma_x**2*(z1[x_perturbacion+4:n_puntos_x,y_barrera]+z1[x_perturbacion+2:n_puntos_x-2,y_barrera]) \
        + 2*gamma_y**2*z1[x_perturbacion+3:n_puntos_x-1,y_barrera-1] - z0[x_perturbacion+3:n_puntos_x-1,y_barrera]
    z_bordes[x_perturbacion+3:n_puntos_x-1,y_barrera+1] = \
        2*(1-gamma_x**2-gamma_y**2)*z1[x_perturbacion+3:n_puntos_x-1,y_barrera+1] \
        + gamma_x**2*(z1[x_perturbacion+4:n_puntos_x,y_barrera+1]+z1[x_perturbacion+2:n_puntos_x-2,y_barrera+1]) \
        + 2*gamma_y**2*z1[x_perturbacion+3:n_puntos_x-1,y_barrera+2] - z0[x_perturbacion+3:n_puntos_x-1,y_barrera+1]
    
    # Extremos izquierdos de la barrera izquierda
    z_bordes[x_perturbacion+2,y_barrera] = 2*(1-gamma_x**2-gamma_y**2)*z1[x_perturbacion+2,y_barrera] \
        + 2*gamma_x**2*z1[x_perturbacion+1,y_barrera] + 2*gamma_y**2*z1[x_perturbacion+2,y_barrera-1] \
        - z0[x_perturbacion+2,y_barrera]
    z_bordes[x_perturbacion+2,y_barrera+1] = 2*(1-gamma_x**2-gamma_y**2)*z1[x_perturbacion+2,y_barrera+1] \
        + 2*gamma_x**2*z1[x_perturbacion+1,y_barrera+1] + 2*gamma_y**2*z1[x_perturbacion+2,y_barrera+2] \
        - z0[x_perturbacion+2,y_barrera+1]
        
    # Bordes de los costados izquierdo y derecho    
    z_bordes[0,1:n_puntos_y-1] = 2*(1-gamma_x**2-gamma_y**2)*z1[0,1:n_puntos_y-1] \
        + gamma_y**2*(z1[0,2:n_puntos_y]+z1[0,0:n_puntos_y-2]) \
        + 2*gamma_x**2*z1[1,1:n_puntos_y-1] - z0[0,1:n_puntos_y-1]
    z_bordes[n_puntos_x-1,1:n_puntos_y-1] = 2*(1-gamma_x**2-gamma_y**2)*z1[n_puntos_x-1,1:n_puntos_y-1] \
        + gamma_y**2*(z1[n_puntos_x-1,2:n_puntos_y]+z1[n_puntos_x-1,0:n_puntos_y-2]) \
        + 2*gamma_x**2*z1[n_puntos_x-2,1:n_puntos_y-1] - z0[n_puntos_x-1,1:n_puntos_y-1]
    
    # Extremos izquierdos de la barrera izquierda
    z_bordes[0,y_barrera] = 2*(1-gamma_x**2-gamma_y**2)*z1[0,y_barrera] \
        + 2*gamma_y**2*z1[0,y_barrera-1] + 2*gamma_x**2*z1[1,y_barrera] - z0[0,y_barrera]
    z_bordes[n_puntos_x-1,y_barrera] = 2*(1-gamma_x**2-gamma_y**2)*z1[n_puntos_x-1,y_barrera] \
        + 2*gamma_y**2*z1[n_puntos_x-1,y_barrera-1] + 2*gamma_x**2*z1[n_puntos_x-2,y_barrera] \
        - z0[n_puntos_x-1,y_barrera]
        
    # Extremos derechos de la barrera derecha
    z_bordes[0,y_barrera+1] = 2*(1-gamma_x**2-gamma_y**2)*z1[0,y_barrera+1] \
        + 2*gamma_y**2*z1[0,y_barrera+2] + 2*gamma_x**2*z1[1,y_barrera+1] - z0[0,y_barrera+1]
    z_bordes[n_puntos_x-1,y_barrera+1] = 2*(1-gamma_x**2-gamma_y**2)*z1[n_puntos_x-1,y_barrera+1] \
        + 2*gamma_y**2*z1[n_puntos_x-1,y_barrera+2] + 2*gamma_x**2*z1[n_puntos_x-2,y_barrera+1] \
        - z0[n_puntos_x-1,y_barrera+1]
    
    return z_bordes

# Funcion que define la matriz inicial en la animacion de la onda    
def init():  
    z.set_data(onda[:,:,0].T)
    texto.set_text('t=0.00')
    return [z]
    
# Funcion que define la matriz en el instante t en la animacion de la onda     
def animate(t):  
    tiempo = t*t_fin/n_puntos_t
    z.set_data(onda[:,:,t].T)
    texto.set_text(('t=%.2f' % tiempo))    
    return [z] 

# Se definen las constantes necesarias
print("Inicializando el sistema...")    
n_puntos_x = 301
n_puntos_y = 300
n_puntos_t = 600
n_pasos = 12000
t_fin = 60
profundidad = -2
c = 1
lado = 30
dx = lado/(n_puntos_x-1)
dy = lado/(n_puntos_y-1)
dt = t_fin/n_pasos
gamma_x = c*dt/dx
gamma_y = c*dt/dy
x_perturbacion = np.int64((n_puntos_x-1)/2)
y_perturbacion = np.int64(n_puntos_y/3)-1
y_barrera = 2*np.int64(n_puntos_y/3)-1

# Se generan las condiciones iniciales de la onda, es decir, en los instantes 0 y 1
onda = np.zeros((n_puntos_x,n_puntos_y,n_puntos_t+2))
onda_temp = np.zeros((n_puntos_x,n_puntos_y,3))
onda[x_perturbacion,y_perturbacion,0] = -0.5
onda[:,:,1] = evolucion_inicial_onda(onda[:,:,0])
onda_temp[:,:,0] = np.copy(onda[:,:,0])
onda_temp[:,:,1] = np.copy(onda[:,:,1])

# Se calcula la onda en el periodo de tiempo especificado por n_pasos
t_guardado = 2
fondo = np.ones((n_puntos_x,n_puntos_y))*profundidad
print("Propagando onda... (Puede tardar un par de minutos)")
for t in range(2,n_pasos):
    tmod0 = (t-2)%3
    tmod1 = (t-1)%3
    tmod2 = t%3
    onda_temp[:,:,tmod2] = evolucion_onda(onda_temp[:,:,tmod0],onda_temp[:,:,tmod1])
    onda_temp[:,:,tmod2] = evolucion_bordes(onda_temp[:,:,tmod0],onda_temp[:,:,tmod1],onda_temp[:,:,tmod2])
    onda_temp[:,:,tmod2] = np.where(onda_temp[:,:,tmod2]>profundidad,onda_temp[:,:,tmod2],fondo)
    if(t%np.int64(n_pasos/n_puntos_t) == 0):
        onda[:,:,t_guardado] = onda_temp[:,:,tmod2]
        t_guardado += 1

# Se crean los ejes y la malla necesarias para generar las graficas 
x = np.linspace(0,lado,n_puntos_x)
y = np.linspace(0,lado,n_puntos_y)
xgrid, ygrid = np.meshgrid(x,y)
        
# Se generan las graficas con la onda en el tiempo 30 y el tiempo 60
color_map = plt.cm.hot
fig1, ax1 = plt.subplots()
im1 = ax1.imshow(onda[:,:,np.int64(n_puntos_t/2)].T, cmap=color_map, extent=[0,30,30,0])
fig1.colorbar(im1, label = r'$\Phi(x,y,t)$')
fig1.gca().set_aspect('equal', adjustable='box')
ax1.set_xlabel(r'$x$', size = 9)                    
ax1.set_ylabel(r'$y$', size = 9) 
texto1 = ax1.text(-0.16, 0.92,'t=30', horizontalalignment='center', verticalalignment='center',
     transform = ax1.transAxes, bbox=dict(facecolor='none', edgecolor='red'))
fig1.savefig('Onda30.jpeg',dpi=250)
plt.close()

fig2, ax2 = plt.subplots()
im2 = ax2.imshow(onda[:,:,n_puntos_t].T, cmap=color_map, extent=[0,30,30,0])
fig2.colorbar(im2, label = r'$\Phi(x,y,t)$')
fig2.gca().set_aspect('equal', adjustable='box')
ax2.set_xlabel(r'$x$', size = 9)                    
ax2.set_ylabel(r'$y$', size = 9)
texto2 = ax2.text(-0.16, 0.92,'t=60', horizontalalignment='center', verticalalignment='center',
     transform = ax2.transAxes, bbox=dict(facecolor='none', edgecolor='red'))
fig2.savefig('Onda60.jpeg',dpi=250)
plt.close()

# Se genera la animacion de la onda propagandose
fig3 = plt.figure()
ax3 = plt.axes(xlim=(0, 30), ylim=(30, 0))
ax3.set_xlabel(r'$x$', size = 9)
ax3.set_ylabel(r'$y$', size = 9)
ax3.set_title(r'Simulación de propagación de onda', size = 11)
ax3.tick_params(labelsize = 9)
z = ax3.imshow(onda[:,:,n_puntos_t-1].T, cmap=color_map, extent=[0,30,30,0])
texto = ax3.text(-0.16, 0.92,[], horizontalalignment='center', verticalalignment='center',
     transform = ax3.transAxes, bbox=dict(facecolor='none', edgecolor='red'))
fig3.colorbar(z, label = r'$\Phi(x,y,t)$')
animacion_onda = ani.FuncAnimation(fig3, animate, init_func=init,
                               frames=onda.shape[2], interval=75, blit=True)
print("Guardando animacion... (Puede tardar otro par de minutos)")
animacion_onda.save('Onda.mp4',dpi=250)
plt.close()
print("Proceso terminado")
