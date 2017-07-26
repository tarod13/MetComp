import numpy as np
import pylab as plt

def funcion_estimada(t,coef):
    return coef[0]*V0*(1-np.exp(-t/(coef[1]*coef[0])))

def likelihood(q_obs,q_est):
    chi2 = np.sum(((q_obs-q_est)/np.std(q_obs))**2)
    l = np.exp(-chi2/2.0)
    l_min = 1e-50
    if l < l_min:
        return l_min
    else:
        return l

observaciones = np.genfromtxt('CircuitoRC.txt',delimiter=' ')

t = observaciones[:,0]
q_obs = observaciones[:,1]

V0 = 10
n_puntos = 20000
coef = [np.random.rand(2)]
coef[0][0]*=10
q_est = funcion_estimada(t,coef[0])
lklhood = [likelihood(q_obs,q_est)]

for punto in range(0,n_puntos):
    coef_nuevo = np.maximum(np.random.normal(coef[punto], [5,10]),np.array([1e-50,1e-50]))
    q_est_nuevo = funcion_estimada(t,coef_nuevo)
    lklhood_nuevo = likelihood(q_obs,q_est_nuevo)
    alpha = lklhood_nuevo/lklhood[punto]
    if alpha >= 1:
        coef.append(coef_nuevo)
        lklhood.append(lklhood_nuevo)
    elif alpha > np.random.rand():
        coef.append(coef_nuevo)
        lklhood.append(lklhood_nuevo)
    else:
        coef.append(coef[punto])
        lklhood.append(lklhood[punto])

max_id = np.argmax(lklhood)
coef_max = coef[max_id]
lklh_max = lklhood[max_id]
q_est_max = funcion_estimada(t,coef_max)

coef_array = np.array(coef)
R = coef_array[:,1]
C = coef_array[:,0]

fig1, ax1 = plt.subplots(1,2, sharey = True, figsize=(10, 5))
ax1[0].set_xlabel(r'Likelihood')
ax1[0].set_ylabel(r'R ($\Omega$)')
ax1[1].set_xlabel(r'Likelihood')
ax1[1].set_ylabel(r'C'' (F)')
ax1[0].scatter(lklhood,R,s=0.5)
ax1[1].scatter(lklhood,C,s=0.5)
fig1.savefig('RC_vs_likelihood.pdf')
plt.close()

fig2, ax2 = plt.subplots()
ax2.set_xlabel(r't'' (s)')
ax2.set_ylabel(r'q'' (C)')
ax2.scatter(t,q_obs,s=0.5)
ax2.plot(t,q_est_max)
plt.text(100,5,r'R = ''%.3f,'r' C = ''%.3f'r', Likelihood =''%.3f' % (coef_max[1],coef_max[0],lklh_max))
fig2.savefig('estimacion_circuito.pdf')
plt.close()
