# Se importan librerias necesarias
import numpy as np
import pylab as plt

# Se ignonan las advertencias relacionadas con divisiones por 0
estados_ignorados = np.seterr(divide='ignore')

# Se define la clase carga
# Atributos: mag - magnitud de la carga, pos - posicion (x,y)
# Metodos: potencial - potencial generado por la carga en un punto (x,y)
class carga:
    def __init__(self, magnitud, posicion):
        self.mag = magnitud
        self.pos = posicion
    def potencial(self,pos2_x,pos2_y):
        dist_x = self.pos[0]*np.ones((len(pos2_y),len(pos2_x))) - pos2_x
        dist_y = self.pos[1]*np.ones((len(pos2_y),len(pos2_x))) - pos2_y
        dist = np.sqrt(dist_x**2+dist_y**2)
        infMat = self.mag*float('inf')*np.ones((len(pos2_y),len(pos2_x)))        
        V_carga = np.where(dist == 0.0,infMat,np.divide(kmod*self.mag,dist))        
        return V_carga

# Se define la funcion que halla el gradiente de un campo escalar 2D (matriz cuadrada n x n) por medio del
# metodo central differentiation (se toma la diferencia del punto siguiente y el anterior al punto evaluado)
# Parametros: puntos - intervalo del eje a ser derivado, 
#             evaluaciones - valores que toma la funcion en el rectangulo
#             limitado por el intervalo especificado
# Salidas: derivada_x - matriz cuadrada n-1 x n-1 con la componente x del gradiente del campo evaluado
#          derivada_y - matriz cuadrada n-1 x n-1 con la componente y del gradiente del campo evaluado
def derivacion_cd_2D(puntos,evaluaciones):
    diferencias_puntos = puntos[2:len(puntos)]-puntos[0:len(puntos)-2]
    diferencias_f_x = evaluaciones[1:len(puntos)-1,2:len(puntos)]-evaluaciones[1:len(puntos)-1,0:len(puntos)-2]
    diferencias_f_y = evaluaciones[2:len(puntos),1:len(puntos)-1]-evaluaciones[0:len(puntos)-2,1:len(puntos)-1]
    dif_puntos_x, dif_puntos_y = np.meshgrid(diferencias_puntos,diferencias_puntos)     
    derivadas_x = diferencias_f_x/dif_puntos_x
    derivadas_y = diferencias_f_y/dif_puntos_y    
    return(derivadas_x, derivadas_y)

#Se definen las constantes necesarias
k = 8.987e9             # Constante de Coulomb en N.m^2.C^-2
e = 1.602176e-19        # Magnitud de la carga de un electron en C
nm = 1e-9               # Longitud de un nm en m
kmod = k*e**2/nm**3     # Constante de Coulomb en nN.nm^2.e^-2
magnitud = 1.0          # Magnitud de las cargas del problema en e
lado = 1.0              # Longitud relevante del problema en nm
npuntos = 201           # Cantidad de puntos en los que se divide una dimension 

# Se crean las 4 cargas del problema como instancias de la clase carga
carga_1 = carga(magnitud,np.array([lado/2,lado/2]))
carga_2 = carga(-magnitud,np.array([-lado/2,lado/2]))
carga_3 = carga(magnitud,np.array([-lado/2,-lado/2]))
carga_4 = carga(-magnitud,np.array([lado/2,-lado/2])) 

# Se crean los ejes y mallas para la evaluacion del campo y el potencial
x = np.linspace(-lado,lado,npuntos)         # Eje x para la eval. del potencial
xE = np.copy(x[1:npuntos-1])                # Eje x para la eval. del campo
xgrid, ygrid = np.meshgrid(x,x)             # Malla para la eval. del potencial
xgridE, ygridE = np.meshgrid(xE,xE)         # Malla para la eval. del campo

# Se evalua el potencial creado por cada carga en la malla y el potencial 
# total a partir del teorema de superposicion
V_carga_1 = np.copy(carga_1.potencial(xgrid,ygrid))
V_carga_2 = np.copy(carga_2.potencial(xgrid,ygrid))
V_carga_3 = np.copy(carga_3.potencial(xgrid,ygrid))
V_carga_4 = np.copy(carga_4.potencial(xgrid,ygrid))
Vtotal = V_carga_1 + V_carga_2 + V_carga_3 + V_carga_4

# Se calcula el campo electrico a partir del la derivacion central difference del potencial
Ex, Ey = np.negative(derivacion_cd_2D(x,Vtotal))

# Se genera la grafica del campo y el potencial en el espacio especificado
Vfin = Vtotal[np.isfinite(Vtotal)]                #Se separan los valores finitos del potencial
color_map = plt.cm.gist_rainbow         # Se define la escala de colores
color_map.set_over('k', 1)              # Se define el color para +infinito
color_map.set_under('w', 1)             # Se define el color para -infinito
fig1, ax1 = plt.subplots()
im1 = ax1.pcolormesh(xgrid,ygrid,Vtotal, cmap=color_map,vmin=Vfin.min(), vmax=Vfin.max())
ax1.streamplot(xgridE, ygridE,Ex, Ey, linewidth=0.5, cmap=plt.cm.autumn)   
# Se genera la escala de colores correspondiente al potencial
fig1.colorbar(im1, extend = 'both',label = r'$V$' ' (nN\u2022nm/e)')
# Se ajustan los ejes para que tengan el mismo tamanyo
fig1.gca().set_aspect('equal', adjustable='box')
plt.xlabel(r'$x$' ' (nm)')                    
plt.ylabel(r'$y$' ' (nm)')                    
fig1.savefig('cargas.pdf')

# Se avisa cuando termina la ejecucion        
print('Proceso terminado.')      
