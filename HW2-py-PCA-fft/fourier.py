# Se importan las librerias necesarias
import numpy as np
import pylab as plt
import scipy.io.wavfile as wav

# Funcion que calcula un elemento de la base de la transformada de Fourier
def funcion_Z(exponente,N_puntos,signo):
    return np.exp(-np.sign(signo)*2*np.pi*1j*exponente/N_puntos)

# Funcion que invierte los bits del numero entero (o la secuencia de numeros enteros) recibido (s)
def inversion_de_bits(secuencia,potencia):
    secuencia_invertida = []
    for numero in secuencia:
        numero_invertido = 0
        for posicion in range(0,potencia):
            mascara = 2**posicion
            numero_invertido = numero_invertido + bool(numero & mascara)*2**(potencia-1-posicion)
        secuencia_invertida.append(numero_invertido)
    return np.array(secuencia_invertida)

# Funcion que rellena de ceros la secuencia recibida hasta obtener una longitud que es potencia de 2.
# Esto es para poder realizar el proceso de la fft, tal y como se implemento
def secuencia_potencia_de_2(secuencia):
    N = secuencia.shape[0]
    potencia_max = np.int64(np.ceil(np.log(N)/np.log(2.0)))
    secuencia_pot2 = np.copy(secuencia)	
    diferencia_pot2 = 2**potencia_max - N
    ceros_adicionales = np.zeros(diferencia_pot2)
    secuencia_pot2 = np.concatenate((secuencia_pot2,ceros_adicionales))
    return(secuencia_pot2,potencia_max)

# Funcion que sirve como paso intermedio para calcular la transformada de Fourier rapida. Se reciben
# como entrada dos secuencias y se retorna una sola que las combina por medio de la base de la transformada 
def fft_parcial(grupo1,grupo2,Z,potencia_max,etapa):
    indices_base_original = np.linspace(0,2**etapa-1,2**etapa)
    indices_base_potencia_max = 2**(potencia_max-(etapa+1))*indices_base_original
    Z_N_etapa = Z[np.int64(indices_base_potencia_max)]
    salida1 = grupo1 + grupo2*Z_N_etapa
    salida2 = grupo1 - grupo2*Z_N_etapa
    salida = np.concatenate((salida1,salida2))
    return salida

# Funcion que calcula la transformada de Fourier por medio del metodo radix-2, que utiliza secuencias con 
# longitudes potencias de 2. La explicacion del funcionamiento de este algoritmo se explica en la pagina:
# www.cmlab.csie.ntu.edu.tw/cml/dsp/training/coding/transform/fft.html    
def radix2_fft(secuencia,signo):
    secuencia_pot2, potencia_max = secuencia_potencia_de_2(secuencia)
    N = secuencia_pot2.shape[0]
    indices = np.int64(np.linspace(0,N-1,N))
    indices_invertidos = inversion_de_bits(indices,potencia_max)
    tf_secuencia = np.complex64(secuencia_pot2[indices_invertidos])
    Z = funcion_Z(np.int64(np.linspace(0,(N/2)-1,np.int64(N/2))),N,signo)
    for etapa in range(0,potencia_max):
        N_grupos = 2**(potencia_max-etapa-1)
        N_elementos_grupo = np.int64(N/N_grupos)
        for n_grupo in range(0,N_grupos):
            id_inf = N_elementos_grupo*n_grupo
            id_sup = N_elementos_grupo*(n_grupo+1)
            grupo = tf_secuencia[id_inf:id_sup]
            subgrupo1 = grupo[0:np.int64(N_elementos_grupo/2)]
            subgrupo2 = grupo[np.int64(N_elementos_grupo/2):N_elementos_grupo]            
            tf_secuencia[id_inf:id_sup] = fft_parcial(subgrupo1,subgrupo2,Z,potencia_max,etapa)
    if signo > 0:
        return tf_secuencia
    else:
        return tf_secuencia/N

# Funcion que elimina el pico con mayor valor de una secuencia en el espacio de frecuencias     
def filtro_amplitud_maxima(secuencia,radio):
    N = secuencia.shape[0]
    indice_max = np.argmax(np.absolute(secuencia)**2)
    indice_max_2 = np.int64(N - indice_max)
    secuencia_filtrada = np.copy(secuencia)
    secuencia_filtrada[indice_max-radio:indice_max+radio+1] = np.complex64(np.zeros(2*radio+1))   
    secuencia_filtrada[indice_max_2-radio:indice_max_2+radio+1] = np.complex64(np.zeros(2*radio+1))    
    return secuencia_filtrada

# Funcion que elimina las componentes relacionadas con frecuencias mayores a una frecuencia de corte dada, 
# supondiendo que la secuencia recibida se encuentra en el espacio de frecuencias  
def filtro_pasabajas(secuencia,frecuencias,f_corte):
    N = secuencia.shape[0]
    secuencia_filtrada = np.copy(secuencia)
    for i in range(0,N):
        if np.absolute(frecuencias[i]) > f_corte:
            secuencia_filtrada[i] = 0.0 + 0.0*1j
    return secuencia_filtrada	

# Se leen los archivos de audio, guardando sus tasa de muestreo fs, sus amplitudes y sus cantidades de puntos
fs_do, do, = wav.read("Do.wav")
fs_sol, sol,  = wav.read("Sol.wav")
N_do = np.int64(do.shape[0])
N_sol = np.int64(do.shape[0])

# Se halla la transformada de Fourier de los audios por medio del algoritmo fft 
do_tf = radix2_fft(do,1)
N_do_tf = do_tf.shape[0]
do_tf_shift = np.concatenate((do_tf[np.int64(N_do_tf/2):N_do_tf],do_tf[0:np.int64(N_do_tf/2)]))
do_tf_frec = np.linspace(-N_do_tf/2,N_do_tf/2-1,np.int64(N_do_tf))*fs_do/N_do_tf
sol_tf = radix2_fft(sol,1)
N_sol_tf = sol_tf.shape[0]
sol_tf_shift = np.concatenate((sol_tf[np.int64(N_sol_tf/2):N_sol_tf],sol_tf[0:np.int64(N_sol_tf/2)]))
sol_tf_frec = np.linspace(-N_sol_tf/2,N_sol_tf/2-1,np.int64(N_sol_tf))*fs_sol/N_sol_tf

# Se filtra la transformada hallada para la nota Do por medio de las funciones definidas
radio_filtrado = 80
frecuencia_corte = 1000
do_tf_amax = filtro_amplitud_maxima(do_tf,radio_filtrado)
do_tf_amax_shift = np.concatenate((do_tf_amax[np.int64(N_do_tf/2):N_do_tf],do_tf_amax[0:np.int64(N_do_tf/2)]))
do_tf_frec_shift = np.concatenate((do_tf_frec[np.int64(N_do_tf/2):N_do_tf],do_tf_frec[0:np.int64(N_do_tf/2)]))
do_tf_pb = filtro_pasabajas(do_tf,do_tf_frec_shift,frecuencia_corte)
do_tf_pb_shift = np.concatenate((do_tf_pb[np.int64(N_do_tf/2):N_do_tf],do_tf_pb[0:np.int64(N_do_tf/2)]))

# Se grafican las magnitudes al cuadrado de las transformadas de la nota Do y las dos resultantes al filtrar esta
fig1, ax1 = plt.subplots(nrows = 3)
ax1[0].plot(do_tf_frec,np.absolute(do_tf_shift)**2,'r',linewidth=1)
ax1[1].plot(do_tf_frec,np.absolute(do_tf_amax_shift)**2,'violet',linewidth=1)
ax1[2].plot(do_tf_frec,np.absolute(do_tf_pb_shift)**2,'b',linewidth=1)
ax1[0].set_ylabel(r'|Transformada nota Do|$^2$',size=5)
ax1[1].set_ylabel(r'|Transformada nota Do''\n' r'sin máximo|$^2$',size=5)
ax1[2].set_ylabel(r'|Transformada nota Do''\n' r'con filtro pasabajas|$^2$',size=5)
ax1[2].set_xlabel(r'Frecuencia (Hz)',size=6)
plt.setp(ax1[0].get_xticklabels(), visible=False)
plt.setp(ax1[1].get_xticklabels(), visible=False)
for i in range(0,3):
    ax1[i].set_xlim(-4000,4000)
    ax1[i].tick_params(labelsize=6)
    tx = ax1[i].yaxis.get_offset_text()
    tx.set_fontsize(6)
fig1.savefig('DoFiltros.pdf')
plt.close()

# Se modifica la frecuencia fundamental de la nota Do para que su frecuencia coincida con la de la nota Do
f_fundamental_do = 260
f_fundamental_sol = 391
fs_doSol = fs_do*f_fundamental_sol/f_fundamental_do
doSol_tf_frec = np.linspace(-N_do_tf/2,N_do_tf/2-1,np.int64(N_do_tf))*fs_doSol/N_do_tf

# Se grafican las magnitudes al cuadrado de las transformadas de la nota Do, con su frecuencia fundamental modificada,
# y de la nota Sol
fig2, ax2 = plt.subplots(nrows = 2)
ax2[0].plot(doSol_tf_frec,np.absolute(do_tf_shift)**2,'r',linewidth=1)
ax2[1].plot(sol_tf_frec,np.absolute(sol_tf_shift)**2,'b',linewidth=1)
ax2[0].set_ylabel(r'|Transformada nota Do''\n' r'con frecuencia artificial|$^2$',size=6)
ax2[1].set_ylabel(r'|Transformada nota Sol|$^2$',size=6)
ax2[1].set_xlabel(r'Frecuencia (Hz)',size=7)
plt.setp(ax2[0].get_xticklabels(), visible=False)
for i in range(0,2):
    ax2[i].set_xlim(-4000,4000)
    ax2[i].tick_params(labelsize=6)
    tx = ax2[i].yaxis.get_offset_text()
    tx.set_fontsize(6)
fig2.savefig('DoSol.pdf')
plt.close()

# Se aplica la transformada inversa a cada una de las versiones de la nota Do para obtenerlas como funciones en el tiempo
do_amax = np.real(radix2_fft(do_tf_amax,-1))[0:N_do]
do_pb = np.real(radix2_fft(do_tf_pb,-1))[0:N_do]

# Se guardan las versiones de la nota Do como funciones en el tiempo en archivos de audio
wav.write("Do_pico.wav",np.int64(fs_do),np.int16(do_amax))
wav.write("Do_pasabajos.wav",np.int64(fs_do),np.int16(do_pb))
wav.write("DoSol.wav",np.int64(fs_doSol),do)

# Se avisa cuando termina la ejecucion        
print('Proceso terminado.')
