# Se importan las librerias necesarias
import numpy as np
from matplotlib import mlab
import pylab as plt
from scipy import misc

# Se lee imagen
moonlanding = misc.imread("moonlanding.png")

# Se halla la transformada
ft_moonlanding = np.fft.fft2(moonlanding)
power = np.absolute(ft_moonlanding)**2

# Se filtra la transformada
max_d0 = ft_moonlanding.shape[0]
max_d1 = ft_moonlanding.shape[1]
ft_moonlanding_filtered = 255*np.ones(ft_moonlanding.shape,dtype = np.complex128)
ft_moonlanding_filtered[0:50,0:50] = ft_moonlanding[0:50,0:50]
ft_moonlanding_filtered[0:50,max_d1-51:max_d1-1] = ft_moonlanding[0:50,max_d1-51:max_d1-1]
ft_moonlanding_filtered[max_d0-51:max_d0-1,0:50] = ft_moonlanding[max_d0-51:max_d0-1,0:50]
ft_moonlanding_filtered[max_d0-51:max_d0-1,max_d1-51:max_d1-1] = ft_moonlanding[max_d0-51:max_d0-1,max_d1-51:max_d1-1]
power_filtered = np.absolute(ft_moonlanding_filtered)**2

# Se halla la imagen filtrada
moonlanding_filtered = np.real(np.fft.ifft2(ft_moonlanding_filtered))

# Se grafica
fig1, ax1 = plt.subplots(2,2)
ax1[0,0].imshow(moonlanding,cmap=plt.get_cmap('gray'), vmin=0, vmax=255)
im = ax1[0,1].imshow(power)
ax1[1,0].imshow(moonlanding_filtered, cmap=plt.get_cmap('gray'), vmin=0, vmax=255)
imf = ax1[1,1].imshow(power_filtered)
power_cut = 95.0
power_cut_f = 98.0
clipped_power = mlab.prctile(power.flatten(), power_cut)
clipped_power_f = mlab.prctile(power_filtered.flatten(), power_cut_f)
im.set_clim(0, clipped_power)
imf.set_clim(0, clipped_power_f)
plt.savefig('moon_landing.png')
