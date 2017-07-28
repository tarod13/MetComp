# Se importan liberias necesarias
import numpy as np
import pylab as plt

def modelo(x,parametros):
    return(parametros[0]*x+parametros[1])

def funcion_likelihood(y,y_actual,y_propuesta):
    chi2_actual = np.sum((y-y_actual)**2)
    chi2_propuesta = np.sum((y-y_propuesta)**2)
    if chi2_actual > 1e10:
        chi2_actual*=1e-10
        chi2_propuesta*=1e-10
    l_actual = np.exp(-chi2_actual/2)
    l_propuesta = np.exp(-chi2_propuesta/2)
    return(l_actual,l_propuesta)

def metropolis_hastings(x,y,parametros):
    parametros_propuestos = np.random.normal(parametros,[1,1])
    y_estimada_actual = modelo(x,parametros)
    y_estimada_propuesta = modelo(x,parametros_propuestos)
    likelihoods = funcion_likelihood(y,y_estimada_actual,y_estimada_propuesta)
    alpha = likelihoods[1]/likelihoods[0]
    if alpha > 1:
        parametros_nuevos = parametros_propuestos
        likelihood_nuevos = likelihoods[1]
    else:
        beta = np.random.rand()
        if alpha > beta:
            parametros_nuevos = parametros_propuestos
            likelihood_nuevos = likelihoods[1]
        else:
            parametros_nuevos = parametros
            likelihood_nuevos = likelihoods[0]
    return(parametros_nuevos,likelihood_nuevos)

datos = np.genfromtxt('obs_data.dat', delimiter = ' ')
x_observado = datos[:,0]
y_observada = datos[:,1]

n_iteraciones = 100

parametros = [np.random.rand(2)]
likelihood_inicial,l = funcion_likelihood(y_observada,modelo(x_observado,parametros[0]),np.zeros(datos.shape[0]))
likelihoods = [likelihood_inicial]

for i in range(0,n_iteraciones):
    resultados = metropolis_hastings(x_observado,y_observada,parametros[i])
    parametros.append(resultados[0])
    likelihoods.append(resultados[1])

indice_max_lkhood = np.argmax(likelihoods)
parametros_max_lkhood = parametros[indice_max_lkhood]
y_estimada = modelo(x_observado,parametros_max_lkhood)

print("los parámetro encontrados son m = %f y b = %f" % (parametros_max_lkhood[0], parametros_max_lkhood[1]))

parametros = np.array(parametros)
likelihoods = np.array(likelihoods)

fig1, ax1 = plt.subplots()
ax1.scatter(parametros[:,0],-np.log(likelihoods))
ax1.set_xlabel(r'$\chi^2/2$',size=10)
ax1.set_ylabel(r'm',size=10)
fig1.savefig('Param_m.pdf',dpi=300)
plt.close()

fig2, ax2 = plt.subplots()
ax2.scatter(parametros[:,1],-np.log(likelihoods))
ax2.set_xlabel(r'$\chi^2/2$',size=10)
ax2.set_ylabel(r'm',size=10)
fig2.savefig('Param_b.pdf',dpi=300)
plt.close()

fig3, ax3 = plt.subplots()
ax3.scatter(x_observado,y_observada)
ax3.plot(x_observado,y_estimada)
ax3.set_xlabel(r'$x$',size=10)
ax3.set_ylabel(r'$y$',size=10)
fig3.savefig('Modelo.pdf',dpi=300)
plt.close()
