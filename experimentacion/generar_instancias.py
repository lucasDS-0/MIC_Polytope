import random, math
import pandas as pd
import numpy as np
import os

MIN_NODOS = 30
MAX_NODOS = 100
CANT_INSTANCIAS_N= 2

def generate_edges (n,m):
    aristas = []
    aristasUsadas = [ [ False for i in range(n) ] for j in range(n) ]
    for i in range (0,m):
        edge = np.random.randint(1,n, size=2)
        aristasUsadas [edge[0]-1][edge[1]-1] = True
        aristasUsadas [edge[1]-1][edge[0]-1] = True
    for i in range (0,n):
        for j in range (i+1,n):
            if aristasUsadas[i][j]:
                aristas.append([i+1, j+1])
    return aristas

def save_instance(instance_name, n, mG, mH, aristasG, aristasH):
    with open(F"instancias/{instance_name}.txt", "w") as f:
        print(n, mG, mH, file=f)
        for arista in aristasG:
            print(arista[0], arista[1], file=f, end="\n")
        for arista in aristasH:
            print(arista[0], arista[1], file=f, end="\n")
           
def save_index(instances):
    with open(F"instancias/instances.txt", "w") as f:
        for instance in instances:
            print(instance, file=f)

filas_indice = []


print("Borrando instancias anteriores..")
if os.path.exists('instancias'):
    os.system('rm -rf instancias')

print("Generando instancias..")
if not os.path.exists('instancias'):
    os.makedirs('instancias')

print("Genera carpeta ralo..")
if not os.path.exists('instancias/ralo'):
    os.makedirs('instancias/ralo')

print("Genera carpeta denso..")
if not os.path.exists('instancias/denso'):
    os.makedirs('instancias/denso')


print(F"Con {MIN_NODOS} nodos hasta {MAX_NODOS}. \nPor valor se generan {CANT_INSTANCIAS_N}  instancias")

for n in range(MIN_NODOS, MAX_NODOS):
    for i in range(0, CANT_INSTANCIAS_N):
        mGralo = np.random.randint(1,n, size=1)[0]
        mGdenso = np.random.randint(n*n/2,n*n,size=1)[0]
        mH = np.random.randint(1,n*n, size=1)[0]

        aristasGralo = generate_edges(n, mGralo)
        aristasGdenso = generate_edges(n, mGdenso)
        aristasH = generate_edges (n, mH)
        mGralo = len(aristasGralo)
        mGdenso = len(aristasGdenso)
        mH = len(aristasH)

        save_instance(F"ralo/instancia-{n}-nodos-Gralo-{i}", n, mGralo, mH, aristasGralo, aristasH)
        save_instance(F"denso/instancia-{n}-nodos-Gdenso-{i}", n, mGdenso, mH, aristasGdenso, aristasH)
	
        filas_indice.append([ n, mGralo, mH, F"ralo/instancia-{n}-nodos-Gralo-{i}.txt"])
        #filas_indice.append([ n, mGdenso, mH, F"instancia-{n}-nodos-Gdenso-{i}.txt"])
pd.DataFrame(filas_indice, columns=["n","mG","mH", "archivo"]).to_csv("indice-instancias.csv", index=False, header=True)

print("Terminado")
3
