import time
start = time.time()
import numpy as np

def test_function(x):
    #return np.exp(-(x**2))
    return np.sqrt(1-x**2)

def simpson_integration(lim_inf,lim_sup,nDivisiones):
    delta = (lim_sup-lim_inf)/nDivisiones
    suma = 0
    i = 0
    while(i < nDivisiones):
        suma = suma + test_function(lim_inf+delta*i) + 2*test_function(lim_inf+delta*(i+1/2))
        i = i + 1
    integral = delta*(test_function(lim_sup)+2*suma-test_function(lim_inf))/6
    return integral

#integral = simpson_integration(-1000,1000,100000)
#approx_pi = integral**2

approx_pi = 4*simpson_integration(0,1,100000)

print("Aproximacion de pi = %.30f" % approx_pi)
end = time.time()
print("El tiempo gastado fue:", end-start, "segundos")
