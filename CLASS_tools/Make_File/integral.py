# Se importan librerias necesarias
import numpy as np
import pylab as plt

# Se define la funcion que evalua la funcion a integrar
# Parametros: punto - punto en el que se evalua la funcion
# Salidas: evaluacion del punto  
def funcion(x):
    return np.sum(x)**3

# Se define la funcion que realiza la integracion de Monte Carlo. En esta se genera aleatoriamente un
# punto en un espacio dim-dimensional y se evalua la funcion a integrar en el punto. Se genera ademas
# un numero aleatorio entre 0 y el maximo de la funcion a integrar (altura_max). Si este es menor que
# la evaluacion, se cuenta como un punto dentro del volumen dim+1-dimensional que se encuentra debajo
# de la superficie generada por la funcion. Finalmente se aproxima la integral a la razon entre el 
# numero de puntos contados dentro sobre la totalidad de los puntos, multiplicada por el volumen de 
# la celda dim+1-dimensional con altura altura_max y lados de dimension (lim_sup-lim_inf)   
# Parametros: lim_inf - limite inferior de la integral (se asume el mismo para todas las dimensiones)
#             lim_sup - limite superior de la integral (se asume el mismo para todas las dimensiones)
#             n_puntos - cantidad de puntos aleatorios generados
#             dim - dimension de cada punto aleatorio generado
#             altura_max - maximo que alcanza la funcion en los intervalos especificados
# Salidas: integral_aproximada
def integral_MC(lim_inf,lim_sup,n_puntos,dim,altura_max):   
    n_puntos_dentro = 0
    for i in range(0,n_puntos):
        rand_punto = (lim_sup-lim_inf)*np.random.random(size = (dim,1))
        rand_altura = altura_max*np.random.random()
        altura = funcion(rand_punto)
        if rand_altura < altura:
            n_puntos_dentro = n_puntos_dentro + 1            
    integral_aproximada = (lim_sup-lim_inf)**dim * altura_max * n_puntos_dentro / n_puntos
    return integral_aproximada

# Se define la funcion que calcula el promedio de los resultados obtenidos al integrar por medio del
# metodo de Monte Carlo n_rep veces   
# Parametros: lim_inf - limite inferior de la integral (se asume el mismo para todas las dimensiones)
#             lim_sup - limite superior de la integral (se asume el mismo para todas las dimensiones)
#             n_puntos - cantidad de puntos aleatorios generados
#             dim - dimension de cada punto aleatorio generado
#             altura_max - maximo que alcanza la funcion en los intervalos especificados
#             n_rep - numero de veces que se integra para obtener el promedio
# Salidas: promedio de n_rep aproximaciones de la integral a calcular 
def promedio_integral(lim_inf,lim_sup,n_puntos,dim,altura_max,n_rep):
    suma = 0
    for i in range(0,n_rep):
        aproximacion_integral = integral_MC(lim_inf,lim_sup,n_puntos,dim,altura_max)
        suma = suma + aproximacion_integral
    return suma/n_rep

# Se definen las constantes necesarias
limInf = 0                      # Limite inferior de las integrales
limSup = 2                      # Limite superior de las integrales
npuntos = 8192                  # Cantidad de puntos a utilizar con la integracion de Monte Carlo
Dim = 10                        # Dimension de la superficie sobre la que se integra
valorMax = 20**3                # Valor maximo que toma la funcion a integrar
nRep = 20                       # Veces que se repite el calculo de una integral para realizar su promedio
expInf = 1                      # Exponente que determina la cantidad mas pequeña de puntos con la que se evalua la integral
expSup = 13                     # Exponente que determina la cantidad mas grande de puntos con la que se evalua la integral
integral_analitica = 1126400    # Valor analitico de la integral a aproximar

# Se calcula la integral para npuntos y se imprime su valor
integral_numerica = promedio_integral(limInf, limSup, npuntos, Dim, valorMax, nRep)
print('La integral obtenida al promediar 20 resultados, tomando %s puntos es: %s\n' % (npuntos,integral_numerica))

# Se genera el vector de las cantidades de puntos con las que se calculara la integral y el vector 
# con las aproximaciones correspondientes
Npuntos = np.array([2**i for i in range(expInf,expSup+1)])
integralVariandoPuntos = np.array([promedio_integral(limInf, limSup, N, Dim, valorMax, nRep) for N in Npuntos])

# Se calculan los errores relativos de las integrales halladas
inversa_raiz_Npuntos = 1/np.sqrt(Npuntos)
errores = np.absolute(integral_analitica - integralVariandoPuntos)/integral_analitica

# Generacion de graficas
# Grafica 1 - Integral calculada en funcion del numero de puntos (N) en escala log-2
fig1, ax1 = plt.subplots()
ax1.plot(Npuntos,integralVariandoPuntos)
ax1.set_xscale('log',basex=2)
ax1.set_xlabel(r'Número de puntos $N$ utilizados para integrar')
ax1.set_ylabel(r'Promedio de la integral aproximada')
ax1.ticklabel_format(style='sci', axis='y')
ax1.yaxis.major.formatter.set_powerlimits((0,0)) 
ax1.grid(linestyle='--', linewidth=0.25)
fig1.savefig('num_integral.pdf')

# Grafica 2 - Error relativo entre la integral calculada y la integral analitica en funcion del
# inverso de la raiz de los numero de puntos (1/sqrt(N))
fig2, ax2 = plt.subplots()
ax2.plot(inversa_raiz_Npuntos,errores)
ax2.set_xlabel(r'$N^{-1/2}$')
ax2.set_ylabel(r'Error relativo de la integral numérica')
ax2.ticklabel_format(style='sci', axis='y')
ax2.grid(linestyle='--', linewidth=0.25)
fig2.savefig('err_integral.pdf')

# Se avisa cuando termina la ejecucion         
print('Proceso terminado.\n')  
