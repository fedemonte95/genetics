import abc
from abc import ABCMeta
import random
from random import randint


global archivo
global nombre
global linea
global arco
global arcos
global boolarcos
arcos = ''
boolarcos = ''

random.seed()

class problema:
    __metaclass__ = ABCMeta
    
    @abc.abstractmethod
    def readProblema(self, pNameFile):
        global archivo
        global nombre
        archivo = open(pNameFile,'r')
        nombre = archivo.readline()
    def geneSize(self):
        global linea
        '''tamaño del gen'''
        linea= archivo.readline()
        linea = linea.replace("\n", "")
        linea = linea.split(' ')
        genSize = linea[0]
        aristas = linea[1]
        recubrimiento = linea[2]
        print (genSize)
    def fitnessGT1(self, gen):
        '''Calculo del fitness'''
        i = 0
        global arco
        global arcos
        global boolarcos
        cantCeros = 0
        valFitness = 0
        if(len(arcos)== 0):
            while (i <int(linea[1])):
                arco = archivo.readline().replace("\n", ";")
                arcos = arcos + arco
                i += 1
            arcos = arcos.split(';')
            boolarcos = ['']* len(arcos)
        i = 0
        j = 0
        while (i < len(gen)):
            if(gen[i] == '1'):
                while(j<len(arcos)):
                    if str(i +1) in arcos[j]:
                        boolarcos[j] = '1'
                    j += 1
                j = 0
            else:
                cantCeros += 1
            i += 1
        i = 0
        while(i < len(boolarcos)):
            if(boolarcos[i] == '') :
               cantCeros -= 1
            i += 1
        valFitness = cantCeros;
        return(valFitness)

    def fitnessSP5(self, gen):
        result = 0
        for i in range(len(gen)):
            if(gen[i] == '1'):
                result = result + 1
        return (result)

    def name(self):
        '''Nombre del problema'''
        print (nombre)
    def mutar(self, phijo):
        gen = ''
        pos = 0
        i = 0 
        if (self.pMutacion >=randint(1,100)):
            pos = randint(0,(len(phijo)-1))
            while(i< len(phijo)):
                if(i==pos):
                    if (phijo[i]=='0'):
                        gen += '1'
                    else:
                        gen += '0'
                else:
                    gen += phijo[i]
                i += 1
            print('Muto')
        else:
            print('No muto')
            gen = phijo
        print('Gen')
        print(gen)
    def cruces(self, padreA, padreB, cantPuntos ):
        punto = 0
        hijoA=''
        hijoB=''
        divisiones = len(padreA)// (cantPuntos + 1)
        flag = 0
        while(divisiones<=len(padreA)):
            if(flag == 0):
                hijoA += padreA[punto: punto + divisiones]
                hijoB += padreB[punto: punto + divisiones]
                flag = 1
            else:
                hijoA += padreB[punto:divisiones]
                hijoB += padreA[punto:divisiones]
                flag = 0
            punto = divisiones
            divisiones = divisiones + punto + ((len(padreA)-divisiones)%(len(padreA)// (cantPuntos + 1)))
        return(hijoA,hijoB)
    def seleccionAzar(self, poblacion):
        azar = poblacion[randint(0, len(poblacion)-1)]
        return(azar)
    def seleccionTorneo(self, poblacion):
        torneo = sorted(poblacion)
        return (torneo[-1])
    def seleccionRuleta(self, poblacion):
        poblacionSorted = sorted(poblacion)
        ruleta = poblacionSorted[randint(len(poblacion)//2, len(poblacion)-1)]
        return(ruleta)
        

class Algoritmo(problema):
    def __init__(self, pPolitica, pNumCruces, pMutacion, pTamPoblacion):
        self.pPolitica = pPolitica
        self.pNumCruces = pNumCruces
        self.pMutacion = pMutacion
        self.pTamPoblacion = pTamPoblacion
        self.poblacion = ['']*pTamPoblacion
        self.generacion = 1
        
    def resetPoblacion(self, pTampoblacion):
        while len(self.poblacion) > 0 : self.poblacion.pop()
        self.poblacion = ['']*self.pTamPoblacion
        i = 0
        j = 0
        while (i < len(self.poblacion)):
            while (j<int(linea[0])):
                self.poblacion[i] = self.poblacion[i] + str(randint(0,1))
                j += 1
            j = 0
            i += 1
        print ('La poblacion se reinicio')
        return(self.poblacion)
    def readPoblacion(self, pNameFile):
        poblacionTxt = open(pNameFile,'r')
        nombre = poblacionTxt.readline()
        cantidadGenes = poblacionTxt.readline()
        genes = poblacionTxt.readline()
    def writePoblacion(self):
        poblacionTxt = open('poblacion.txt', 'w+')
        poblacionTxt.write(nombre + '\n')
        poblacionTxt.write(pTampoblacion + '\n')
        while(i<len(poblacion)):
            poblacionTxt.write(self.poblacion[i] +';')
            i ++ 1    
    def generacion(self):
        self.generacion
        fitness(generacion)
        print (self.generacion)
    def getBest(self, boolfitness):
        print('El mejor es: ')
        i = 0
        best = -(len(boolarcos)-1)
        while(i<len(boolfitness)):
            if(best < int(boolfitness[i])):
                best = boolfitness[i]
            i += 1
        print(best)

'''
NameFile = input(str("Digite el nombre del archivo: "))
NameFile += ".txt"
problem = problema()
problem.readProblema(NameFile)
problem.name()
problem.geneSize()
algo = Algoritmo(1,1,5,100)
pobla = algo.resetPoblacion(100)
print(pobla)
print('azarUno')
padreA = algo.seleccionAzar(pobla)
print(padreA)
print('azarDos')
padreB = algo.seleccionAzar(pobla)
print(padreB)
print('hijos')
a,b = algo.cruces(padreA, padreB)
print(a)
print(b)
algo.mutar(a)
fitn = algo.fitness(a)
print('A')
print(fitn)
fitn = algo.fitness(b)
print('B')
print(fitn)
boolfitness = [0]*len(pobla)
i = 0
while(i<len(boolfitness)):
    boolfitness[i]= algo.fitness(pobla[i])
    i +=1
print(boolfitness)
algo.getBest(boolfitness)
'''

NameFile = input(str("Digite el nombre del archivo: "))
politica = int(input("Digite la politica: "))
numCruces = int(input("Digite el numero de cruces: "))
mutacion = int(input("Digite el porcentaje de mutacion: "))
tamanoPoblacion = int(input("Digite el tamano de la poblacion: "))

problem = problema()
problem.readProblema(NameFile)
problem.geneSize()
algoritmo = Algoritmo(politica,numCruces,mutacion,tamanoPoblacion)
pobla = algoritmo.resetPoblacion(100)

if (politica == 1):
    padreA = algoritmo.seleccionAzar(pobla)
    padreB = algoritmo.seleccionAzar(pobla)
    
elif (politica == 2):
    padreA = algoritmo.seleccionTorneo(pobla)
    padreB = algoritmo.seleccionTorneo(pobla)
    
elif (politica == 3):
    padreA = algoritmo.seleccionRuleta(pobla)
    padreB = algoritmo.seleccionRuleta(pobla)

a,b = algoritmo.cruces(padreA, padreB, numCruces)
algoritmo.mutar(a)

if (nombre == "GT1"):
    fitna = algoritmo.fitnessGT1(a)
    fitnb = algoritmo.fitnessGT1(b)
else:
    fitna = algoritmo.fitnessSP5(a)
    fitnb = algoritmo.fitnessSP5(b)

print("\nFitness A = " + str (fitna) + ("\n\n"))
print("\nFitness B = " + str (fitnb) + ("\n\n"))

boolfitness = [0]*len(pobla)
i = 0
if (nombre == "GT1"):
    while(i<len(boolfitness)):
        boolfitness[i]= algoritmo.fitnessGT1(pobla[i])
        i +=1
else:
    while(i<len(boolfitness)):
        boolfitness[i]= algoritmo.fitnessSP5(pobla[i])
        i +=1
print(boolfitness)
algoritmo.getBest(boolfitness)
