from random import *
import sys

seed()

FileName = input(str("Digite el nombre del archivo a guardar: "))
print("\n\n1: GT1\n2: SP5")
problemName = input(str("\n\nDigite la opcion del problema: "))

problem = ""

if (problemName == "1"):
    problem = problem + "GT1"
elif (problemName == "2"):
    problem = problem + "SP5"
else:
    print("\n\nNo es una opcion valida, saliendo.")
    sys.exit()

try:
    N = int(input("Digite N: "))
    M = int(input("Digite M: "))
    K = int(input("Digite K: "))
except:
    print("\n\nNo es un numero valido, saliendo.")
    sys.exit()

problem = problem + "\n" + str(N) + " " + str(M) + " " + str(K) + "\n"

C = range(N)
result = []

if (problemName == "1"):
    for i in range (M):
        result.append(sample(C, 2))
else:
    for i in range (M):
        result.append(sample(C, randint(0, N)))

for i in range(M):
    for j in range(len(result[i])):
        problem = problem + str(result[i][j]) + " "
    problem = problem + "\n"
    
print("\n" + problem)

FileName = FileName + ".txt"

OutFile = open(FileName, 'w')
OutFile.write(problem)
OutFile.close()

sys.exit()
