import numpy as np
import pylab as plt

x00 = 0.2
x10 = 0.0
k = 42
g = 9.81
mu = 0.15
m = 0.25
t0 = 0
tend = 3
n_puntos = 5001

t = np.array([np.linspace(t0,tend,n_puntos)])
print(t.shape)
x0 = np.zeros(t.shape)
x1 = np.zeros(t.shape)
x2 = np.zeros(t.shape)

x0[0,0] = x00
x1[0,0] = x10
x2[0,0] = -k/m*x00 + np.sign(x00)*mu*g

for i in range(1,t.shape[1]):
    x0[0,i] = x0[0,i-1] + x1[0,i-1]*(t[0,i]-t[0,i-1])
    x1[0,i] = x1[0,i-1] + x2[0,i-1]*(t[0,i]-t[0,i-1])
    x2[0,i] = -k/m*x0[0,i] + np.sign(x0[0,i])*mu*g

fig1, ax1 = plt.subplots(nrows = 3)
ax1[0].plot(t,x0)
ax1[1].plot(t,x1)
ax1[2].plot(t,x2) 
plt.savefig('ODE_resorte.pdf')
plt.close()
