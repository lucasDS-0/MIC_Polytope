#Experimento 3 -  Comportamiento de la memoria
#----------------------------

import random, math
import pandas as pd
import numpy as np
from scipy import stats
import sys
import subprocess
import os
import glob

NOMBRE_EXPERIMENTO = 'comportamiento_memoria'
MODIFIED_PATH = './'+ NOMBRE_EXPERIMENTO +'/'
OUTPUT_PATH = MODIFIED_PATH + 'output/'
INSTANCIAS_PATH_IN = 'original/in/*'
INSTANCIAS_PATH_OUT = 'original/out/'

cantidad_corridas= 20
columnas = ["metodo",
            "n",
            "mG",
            "mH",
            "impacto",
            "tiempo",
            "error",
            "maxItSinMejora",
            "aspiracion"];

filas = [];
resultados = [];


def make():
    os.system('mkdir'+ NOMBRE_EXPERIMENTO)
    print("Compilando..")
    os.system('make clean -C ../src')
    os.system('make -C ../src')
    print("Building...")

def read(path_instancia):
    with open(path_instancia, "r") as f:
        return f.read();



def correr_experimento(metodo, parametros, instancia,archivo_instancia):
    input = parametros + " " + instancia
    process = subprocess.Popen(["../build/tp2", metodo, input], stderr=subprocess.PIPE, stdout=subprocess.PIPE, stdin=subprocess.PIPE, universal_newlines = True)

    process.stdin.write(input+"\n")
    process.stdin.flush()

    exit_code = process.wait()

    if exit_code is not 0: raise(f"Hubo un error en la experimentacion para el metodo: {metodo} con la instancia {archivo_instancia}.")


    out = list(map(float,process.stderr.read().strip().split("\n")))


    process.stdin.close();
    process.stdout.close();
    process.stderr.close();
    return out




def run(metodo, parametros, maxsinmejora, aspiracion):
    print('Corriendo...', metodo)
    print('Instancias en: ', INSTANCIAS_PATH_IN)

    for instancia_path in glob.glob(INSTANCIAS_PATH_IN):
        print(instancia_path)
        out_path= instancia_path.split("/")[-1].split(".")[0]
        esperado = read(INSTANCIAS_PATH_OUT + out_path +".out").split(" ")[0]
        esperado = float(esperado)

        instancia = read(instancia_path)
        data = instancia.split();
        tiempos = []
        impactos = []

        for i in range(0,cantidad_corridas):
            #resu = [impacto, tiempo]
            resu = correr_experimento(metodo, parametros, instancia, instancia_path);

            tiempos.append(resu[1]);
            impactos.append(resu[0]);

        tiempo = np.median(tiempos);
        impacto_promedio = np.mean(impactos)

        error=(esperado - impacto_promedio)/esperado

        filas.append([metodo, data[0], data[1], data[2], impacto_promedio, tiempo, error, maxsinmejora, aspiracion]);

    path = f"resultados/resultados-{NOMBRE_EXPERIMENTO}.csv"
    print('Escribiendo resultados en: %s', path)

    resultado = pd.DataFrame(filas, columns=columnas);
    resultado.to_csv(path, index=False, header=True)


# Start experiment
#make()
#heuristica, max_it, max_it_sin_mejoras, max_cant_colores, tipo_vecindad, memory_size, vecindad_size, aspiracion, num_factor

for i in range(0,3,2):   
    for j in range(0,3):
        parametros =f"0 1 {i} 2 -1 -1 -1 {j} 0"
        run('MH1',parametros,i,j)
