import numpy as np
import pylab as plt

u = np.genfromtxt("conveccion.csv",delimiter=',')

n_puntos_t = u.shape[0]
n_puntos_x = u.shape[1]
xfin = 2
x = np.linspace(0,xfin,n_puntos_x)

fig1, ax1 = plt.subplots(2,1)
ax1[0].set_xlabel(r'$x$ (UA)', size = 9)
ax1[1].set_xlabel(r'$x$ (UA)', size = 9)
ax1[0].set_ylabel(r'$u(x,0)$', size = 9)
ax1[1].set_ylabel(r'$u(x,0.3)$', size = 9)
ax1[0].tick_params(labelsize = 8)
ax1[1].tick_params(labelsize = 8)
ax1[0].plot(x, u[0,:], 'r', linewidth = 1)
ax1[1].plot(x, u[n_puntos_t-1,:], 'b', linewidth = 1)
fig1.savefig('conveccion.pdf',dpi=300)
plt.close()
