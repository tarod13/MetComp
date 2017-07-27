import numpy as np
import pylab as plt

N = np.genfromtxt("MC.csv",delimiter=',')
Nrand = np.genfromtxt("MC_rand.csv",delimiter=',')
Nrand_av = np.average(Nrand,axis=0)

n_iteraciones = 10
n_puntos_t = N.shape[0]
tfin = 10
t = np.linspace(0,tfin,n_puntos_t)

fig1, ax1 = plt.subplots()
ax1.set_xlabel(r'$t$', size = 9)
ax1.set_ylabel(r'$N(t)$', size = 9)
ax1.tick_params(labelsize = 8)
ax1.plot(t, N, 'r', linewidth = 1)
for i in range(0,n_iteraciones):
    ax1.plot(t, Nrand[i,:], 'b', linewidth = 1)
fig1.savefig('MC.pdf',dpi=300)
plt.close()

fig2, ax2 = plt.subplots()
ax2.set_xlabel(r'$t$', size = 9)
ax2.set_ylabel(r'$N(t)$', size = 9)
ax2.tick_params(labelsize = 8)
ax2.plot(t, N, 'r', linewidth = 1)
ax2.plot(t, Nrand[0], 'b', linewidth = 1)
ax2.plot(t, Nrand_av, 'g', linewidth = 1)
fig2.savefig('MC_av.pdf',dpi=300)
plt.close()
