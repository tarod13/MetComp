import numpy as np
import pylab as plt

secret_mag = np.genfromtxt('magnitude.dat')
secret_ph = np.genfromtxt('phase.dat')

lenX = secret_mag.shape[0]
lenY = secret_mag.shape[1]
secret_coeff = np.array([[secret_mag[j,i]*np.exp(1j*secret_ph[j,i]) for i in range(0,lenY)] for j in range(0,lenX)])

secret = np.fft.fft2(secret_coeff)
plt.imshow(np.absolute(secret))
plt.savefig('secret.jpg')

