import numpy as np

def inversion_de_bits(secuencia):
	secuencia_invertida = []
	for numero in secuencia:
		numero_invertido = 0
		for posicion in range(0,3):
			mascara = 2**posicion
			numero_invertido = numero_invertido + bool(numero & mascara)*2**(2-posicion)
		secuencia_invertida.append(numero_invertido)
	return np.array(secuencia_invertida)

a = np.int16(np.linspace(0,8-1,8))
print(a)

b = inversion_de_bits(a)
print(b)
