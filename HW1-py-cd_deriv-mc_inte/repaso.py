import numpy as np
import pylab as plt

estados_ignorados = np.seterr(divide='ignore')

class carga:
    def __init__(self, magnitud, posicion):
        self.mag = magnitud
        self.pos = posicion
    def potencial(self, posicion_x,posicion_y):
        dist_x = self.pos[0] - posicion_x
        dist_y = self.pos[1] - posicion_y
        dist = np.sqrt(dist_x**2 + dist_y**2)
        infMat = np.sign(self.mag)*float('Inf')*np.ones(posicion_x.shape)
        potencial = np.where(dist==0.0, infMat, kmod*self.mag/dist)
        return(potencial)

def derivacion_cd_2D(puntos,evaluaciones):
    tam = len(puntos)
    dif_puntos = puntos[2:tam] - puntos[0:tam-2]
    dif_eval_x = evaluaciones[1:tam-1, 2:tam] - evaluaciones[1:tam-1, 0:tam-2]
    dif_eval_y = evaluaciones[2:tam, 1:tam-1] - evaluaciones[0:tam-2, 1:tam-1]
    dif_puntos_x, dif_puntos_y = np.meshgrid(dif_puntos,dif_puntos)
    derivadas_x = dif_eval_x/dif_puntos_x
    derivadas_y = dif_eval_y/dif_puntos_y
    return(derivadas_x,derivadas_y)

def funcion(x):
    return(np.sum(x)**3)

def integracion_MC(lim_inf,lim_sup,n_puntos,f_max,dim):
    puntos_dentro = 0
    for n in range(0,n_puntos):
        punto_ran = (lim_sup-lim_inf)*np.random.random(size=(1,dim))
        f_ran = f_max*np.random.random()
        f = funcion(punto_ran)
        if(f > f_ran):
            puntos_dentro = puntos_dentro + 1
    return f_max*(lim_sup-lim_inf)**dim*puntos_dentro/n_puntos

def integral_promediada_MC(lim_inf,lim_sup,n_puntos,f_max,dim, nrep):
    suma = 0
    for i in range(0,nrep):
        integral_aprox = integracion_MC(lim_inf,lim_sup,n_puntos,f_max,dim)
        suma = suma + integral_aprox
    return suma/nrep

k = 8.987e9 
e = 1.602176e-19
nm = 1e-9
kmod = k*e**2/nm**3
npuntos = 201
q = 1.0
lado = 1.0
        
carga_1 = carga(q,np.array([lado/2,lado/2]))
carga_2 = carga(-q,np.array([-lado/2,lado/2]))
carga_3 = carga(q,np.array([-lado/2,-lado/2]))
carga_4 = carga(-q,np.array([lado/2,-lado/2]))

x = np.linspace(-lado,lado,npuntos)
xgrid, ygrid = np.meshgrid(x,x)
xE = x[1:len(x)-1]
xgridE, ygridE = np.meshgrid(xE,xE)

V_1 = carga_1.potencial(xgrid, ygrid)
V_2 = carga_2.potencial(xgrid, ygrid)
V_3 = carga_3.potencial(xgrid, ygrid)
V_4 = carga_4.potencial(xgrid, ygrid)
V_total = V_1 + V_2 + V_3 + V_4

Ex, Ey = np.negative(derivacion_cd_2D(x,V_total))

Vfin = V_total[np.isfinite(V_total)]
colormap = plt.cm.gist_rainbow
colormap.set_over('k')
colormap.set_under('w')
fig1, ax1 = plt.subplots()
im1 = ax1.pcolormesh(xgrid, ygrid, V_total, cmap = colormap, vmin = Vfin.min(), vmax = Vfin.max())
ax1.streamplot(xgridE, ygridE, Ex, Ey, linewidth = 0.5, cmap = plt.cm.autumn)
fig1.colorbar(im1, extend = 'both', label = r'$V$ (nN''\u2022'r'nm/e)')
fig1.gca().set_aspect('equal')
plt.xlabel(r'$x$ (nm)')
plt.ylabel(r'$y$ (nm)')
fig1.savefig('cargas_test.pdf')

print('Proceso de cargas terminado.\n')

limInf = 0.0
limSup = 2.0
nPuntos = 8192
Dim = 10
fMax = 20.0**3
nRep = 20
minExp = 1
maxExp = 13
integral_analitica = 1126400

aprox_integral = integral_promediada_MC(limInf,limSup,nPuntos,fMax,Dim,nRep)
print('La integral obtenida con %s puntos, tras %s repeticiones es: %s\n' % (nPuntos,nRep,aprox_integral))

N = np.array([2**i for i in range(minExp,maxExp+1)])
integral_variando_N = np.array([integral_promediada_MC(limInf,limSup,n,fMax,Dim,nRep) for n in N])
raiz_inversa_N = 1/np.sqrt(N)
errores = np.absolute(integral_analitica-integral_variando_N)/integral_analitica

fig2, ax2 = plt.subplots()
ax2.plot(N, integral_variando_N)
ax2.set_xscale('log', basex = 2)
ax2.ticklabel_format(style = 'sci', axis = 'y')
ax2.yaxis.major.formatter.set_powerlimits((0,0)) 
ax2.grid(linestyle='--', linewidth=0.25)
plt.xlabel(r'N')
plt.ylabel(r'Integral numérica obtenida por MC y promediada %s veces' % nRep)
fig2.savefig('num_integral_test.pdf')

fig3, ax3 = plt.subplots()
ax3.plot(raiz_inversa_N, errores)
ax3.grid(linestyle = '--', linewidth = 0.25)
plt.xlabel(r'$N^{-1/2}$')
plt.ylabel(r'Eror relativo entre integral numérica e integral analítica')
fig3.savefig('err_integral_test.pdf')

print('Proceso de integracion terminado.\n')

