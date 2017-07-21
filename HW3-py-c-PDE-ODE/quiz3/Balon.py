import numpy as np
import pylab as plt
from mpl_toolkits.mplot3d import Axes3D

def euler(trayectoria, t, dt):
    for i in range(0,4):
        if i < 2:
            trayectoria[t,i] = trayectoria[t-1,i] + dt*trayectoria[t-1,i+2]
        else:
            trayectoria[t,i] = trayectoria[t-1,i] + dt*g[i-2]
    return trayectoria
    
def leap_frog(trayectoria, tmin, dt):
    f = [False, False]
    for t in range(tmin,n_pasos):
        for i in range(0,4):
            if i < 2:
                pos = trayectoria[t-2,i] + 2*dt*trayectoria[t-1,i+2]
                if pos >= 0:
                    trayectoria[t,i] = np.copy(pos)
                else:
                    trayectoria[t,i] = 0.0
                    f[i] = True
            else:
                if f[i-2]:
                    trayectoria[t-1,i] = -0.8*trayectoria[t-1,i]
                    trayectoria[t,i] = trayectoria[t-1,i] + dt*g[i-2]
                    f[i-2] = False
                else:
                    trayectoria[t,i] = trayectoria[t-2,i] + 2*dt*g[i-2]
    return trayectoria
    
g = np.array([0, -9.81])
x0 = 0
y0 = 0
vx0 = 0
vy0 = 5
m = 0.5
tfin = 4
n_pasos = 40000
dx = 0.01
dy = 0.0001
x_inf = -0.1
x_sup = 0.1
y_inf = 0
y_sup = 10

trayectoria = np.zeros((n_pasos,4))
trayectoria[0,0] = x0
trayectoria[0,1] = y0
trayectoria[0,2] = vx0
trayectoria[0,3] = vy0

dt = tfin/n_pasos
trayectoria = euler(trayectoria,1,dt)
trayectoria = leap_frog(trayectoria,2,dt)

t = np.linspace(0,tfin,n_pasos)
fig1 = plt.figure()
ax1 = plt.subplot(111,projection='3d')
ax1.plot(t,trayectoria[:,0],trayectoria[:,1],'r')
ax1.set_xlabel(r'$t$',size = 9)
ax1.set_ylabel(r'$x$',size = 9)
ax1.set_zlabel(r'$y$',size = 9)
ax1.tick_params(labelsize=7)
fig1.savefig('posBalon.pdf',dpi=200)
plt.close()

fig2 = plt.figure()
ax2 = plt.subplot(111,projection='3d')
ax2.plot(t,trayectoria[:,2],trayectoria[:,3],'b')
ax2.set_xlabel(r'$t$',size = 9)
ax2.set_ylabel(r'$v_x$',size = 9)
ax2.set_zlabel(r'$v_y$',size = 9)
ax1.tick_params(labelsize=6)
fig2.savefig('velBalon.pdf',dpi=200)
plt.close()

h_max = trayectoria[:,1].max()
t_duracion = 1
print("la altura máxima alcanzada es %.2f metros" % h_max)
print("el tiempo que el balon permanece en el aire es: es %.2f segundos" % t_duracion)
