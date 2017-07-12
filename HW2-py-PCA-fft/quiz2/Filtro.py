import numpy as np
import pylab as plt
import scipy.io.wavfile as wav

def filtro_pasabandas(transformada,f_muestreo,f_corte,ff):
    N_f = transformada.shape[0]
    freq_f = np.linspace(-N_f/2,N_f/2-1,N_f)*f_muestreo/N_f
    filtro = np.complex64(np.zeros(N_f))
    for i in range(0,N_f):
        f = freq_f[i]
        if (np.absolute(f) > f_corte[0] and np.absolute(f) < f_corte[1]):
            filtro[i] = np.exp(-1j*2*np.pi*f/ff)            
    return(transformada * filtro, filtro)
    
fs, violin, = wav.read("violin.wav")
N = violin.shape[0]

tf_violin = np.fft.fft(violin)
tf_violin_shift = np.concatenate((tf_violin[np.int64(N/2):N],tf_violin[0:np.int64(N/2)]))
freq = np.linspace(-N/2,N/2-1,N)*fs/N

fig1, ax1 = plt.subplots()
ax1.plot(freq,np.absolute(tf_violin_shift)**2)
ax1.set_xlabel(r'Frecuencia (Hz)')
ax1.set_ylabel(r'|Transformada|$^2$')
ax1.set_xlim(-6000,6000)
fig1.savefig('Violin.pdf')
plt.close()

frecuencias_corte = np.array([1000,2000])
frecuencia_filtro = 50
tf_violin_filtro_pb, tf_filtro = filtro_pasabandas(tf_violin_shift,fs,frecuencias_corte,frecuencia_filtro)

fig2, ax2 = plt.subplots(nrows = 2)
ax2[0].plot(freq,np.absolute(tf_violin_shift)**2,'r')
ax2[1].plot(freq,np.absolute(tf_violin_filtro_pb)**2,'b')
ax2[1].set_xlabel(r'Frecuencia (Hz)')
ax2[0].set_ylabel(r'|Transformada|$^2$')
ax2[1].set_ylabel(r'|Transformada filtrada|$^2$')
ax2[0].set_xlim(-6000,6000)
ax2[1].set_xlim(-6000,6000)
fig2.savefig('ViolinFiltro.pdf')
plt.close()
