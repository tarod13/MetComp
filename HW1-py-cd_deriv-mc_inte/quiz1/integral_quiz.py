# Se importan la libreria necesaria 
import numpy as np

# Funcion que evalua la funcion a integrar
def funcion_a_integrar(x):
    return(np.sin(x))

# Integracion de Monte Carlo
def integracion_MC(limInf,limSup,npuntos,fmax):
    n_puntos_dentro = 0
    for i in range(0,npuntos):
        valor_ran = (limSup-limInf)*np.random.random()
        f_ran = fmax*np.random.random()
        f = funcion_a_integrar(valor_ran)
        if f_ran < f:
            n_puntos_dentro = n_puntos_dentro + 1    
    return fmax * (limSup - limInf) * n_puntos_dentro / npuntos

# Se promedian los resultados de la integracion Monte Carlos tras repetir el
# calculo nrep veces
def promedio_integracio_MC(limInf,limSup,npuntos,fmax,nrep):
    suma = 0
    for i in range(0,nrep):
        suma = suma + integracion_MC(limInf,limSup,npuntos,fmax)
    return suma/nrep

# Se definen las constantes necesarias
lim_inf = 0
lim_sup = np.pi
N = 10000
f_max = 1
n_rep = 20

# Se calcula la integral promedio y se imprime el resultado
aproximacion_integral = promedio_integracio_MC(lim_inf,lim_sup,N,f_max,n_rep)
print('el valor de la integral es %s' % aproximacion_integral)
