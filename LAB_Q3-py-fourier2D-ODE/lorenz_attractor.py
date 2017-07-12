# Se importan las librerias necesarias
import numpy as np
import pylab as plt
from mpl_toolkits.mplot3d import Axes3D

# Funcion que calcula el vector (dx/dt,dy/dt,dz/dt)
def derivative(x_t):
    xdot = np.array([sigma*(x_t[1]-x_t[0]), x_t[0]*(rho - x_t[2])-x_t[1], x_t[0]*x_t[1]-beta*x_t[2]])
    return xdot
    
# Funcion que determina la solucion del sistema por medio del metodo de Euler
def euler(time,nvar,x0):
    x = np.zeros((nvar,time.shape[1]))
    x[:,0] = x0
    for i in range(1,time.shape[1]):
        x[:,i] = x[:,i-1] + derivative(x[:,i-1])*(time[0,i]-time[0,i-1])
    return x
   
# Se definen las constantes necesarias    
x0 = 1.0
y0 = 1.0
z0 = 1.0
sigma = 10.0
rho = 28.0
beta = 8.0/3.0
n_var = 3
t0 = 0
tend = 40
n_puntos = np.int64((tend-t0)/0.001)

# Se define el vector de tiempo y se halla la dinamica del sistema como una matriz de 3 filas,
# donde la primera corresponde a x, la seguda a y, y la tercera a z 
t = np.array([np.linspace(t0,tend,n_puntos)])
x = euler(t,n_var,np.array([x0,y0,z0]))

# Se grafica cada variable por separado en 2d
fig1, ax1 = plt.subplots(nrows = 3)
ax1[0].plot(t[0,:],x[0,:], 'g')
ax1[1].plot(t[0,:],x[1,:], 'b')
ax1[2].plot(t[0,:],x[2,:], 'violet')
for j in range(0,3):
    ax1[j].grid(linestyle = '--', linewidth = 0.25)
    ax1[j].tick_params(labelsize=5)
ax1[0].set_ylabel(r'$x$', fontsize = 6)
ax1[1].set_ylabel(r'$y$', fontsize = 6)
ax1[2].set_ylabel(r'$z$', fontsize = 6)
ax1[2].set_xlabel(r'$t$', fontsize = 6)
plt.savefig('lorenz2D.png')
plt.close()

# Se hace la grafica 3D de todas las variables
fig2 = plt.figure()
ax2 = fig2.add_subplot(111, projection='3d')
ax2.plot(x[0,:], x[1,:], x[2,:])
plt.savefig('lorenz.png')
plt.close()
