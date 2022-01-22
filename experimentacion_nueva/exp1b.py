#!/usr/bin/python3

import os
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from instancias import *
from lib import *

PATH_EJECUTABLE = "../build/tp2"

n = 12
m_min = 0
m_max = 24


def generar_combinaciones_aristas():
    """Genera todas las combinaciones posibles de mG y mH"""
    comb_aristas = []
    for i in range(m_min, m_max+1):
        for j in range(m_min, m_max+1):
            comb_aristas.append((i, j))
    return comb_aristas


def generar_instancias(comb_aristas):
    instancias = []
    for params in comb_aristas:
        for i in range(50):
            file_path = f"exp1b/n{n}_mG{params[0]}_mH{params[1]}_{i}"
            instancias.append({
                "id": i,
                "n": n,
                "mG": params[0],
                "mH": params[1],
                "file": file_path
            })
            generar_instancia(n, params[0], params[1], f"{file_path}.in")
    return instancias


def run_exp1b(instancias):
    data = []
    for instancia in instancias:
        resultado_h1 = run(instancia["file"], "H1", PATH_EJECUTABLE)
        data.append({**instancia, **resultado_h1, "heuristica": "H1"})
        resultado_h2 = run(instancia["file"], "H2", PATH_EJECUTABLE)
        data.append({**instancia, **resultado_h2, "heuristica": "H2"})
    return data


def guardar_datos_impacto():
    with open('datos_impacto.txt', 'w') as f:
        f.writelines(str(datos_impacto))


comb_aristas = generar_combinaciones_aristas()
instancias = generar_instancias(comb_aristas)
data = run_exp1b(instancias)
df = pd.DataFrame(data)
df.to_pickle('exp1b_df')
df = pd.read_pickle('exp1b_df')

datos_impacto_H1 = [[-1 for i in range(m_min, m_max+1)] for i in range(m_min, m_max+1)]
datos_impacto_H2 = [[-1 for i in range(m_min, m_max+1)] for i in range(m_min, m_max+1)]
datos_impacto_error = [[-1 for i in range(m_min, m_max+1)] for i in range(m_min, m_max+1)]
datos_impacto = []
# mG -> filas
# mH -> columnas
for m in comb_aristas:
    data_instancias = df.loc[df['file'].apply(lambda x: True if f"exp1b/n12_mG{m[0]}_mH{m[1]}_" in x else False)]
    data_instancias_H1 = data_instancias.loc[data_instancias['heuristica'] == 'H1']
    promedio_impacto_H1 = data_instancias_H1.mean()['impacto']
    data_instancias_H2 = data_instancias.loc[data_instancias['heuristica'] == 'H2']
    promedio_impacto_H2 = data_instancias_H2.mean()['impacto']
    datos_impacto.append({
        'impacto_H1': promedio_impacto_H1,
        'impacto_H2': promedio_impacto_H2,
        'mG': m[0],
        'mH': m[1],
    })
    datos_impacto_H1[m[0]][m[1]] = promedio_impacto_H1
    datos_impacto_H2[m[0]][m[1]] = promedio_impacto_H2
    if promedio_impacto_H2 > 0:
        datos_impacto_error[m[0]][m[1]] = (promedio_impacto_H2 - promedio_impacto_H1) / promedio_impacto_H2 * 100
    else:
        datos_impacto_error[m[0]][m[1]] = 0

datos_impacto_error_bin = [[None for i in range(m_min, m_max+1)] for i in range(m_min, m_max+1)]
for i in range(0, 25):
    for j in range(0, 25):
        if datos_impacto_error[i][j] < 0:
            datos_impacto_error_bin[i][j] = 1
        else:
            datos_impacto_error_bin[i][j] = 2


# guardar_datos_impacto()

# Calcular promedio para cada id

def heatmap(data, heuristica, annot=False):
    sns.set(font_scale=0.5)
    ticks = [i for i in range(0, 25)]
    fig = sns.heatmap(data, yticklabels=ticks, xticklabels=ticks, cbar=False, annot=annot)
    fig.invert_yaxis()
    plt.savefig(f"exp1b_{heuristica}")
    plt.xlabel("mH")
    plt.ylabel("mG") 
    fig.set_xticklabels(fig.get_xticklabels(), rotation=90) 
    plt.close()


index_values = [f"mG={i}" for i in range(0, 25)]
column_values = [f"mH={i}" for i in range(0, 25)] 

print(datos_impacto_error)
# creating the dataframe 
df_error = pd.DataFrame(data = datos_impacto_error,  
                  index = index_values,  
                  columns = column_values)

print(df_error)
heatmap(datos_impacto_H1, "H1")
heatmap(datos_impacto_H2, "H2")
heatmap(datos_impacto_error, "error", annot=True)
heatmap(datos_impacto_error_bin, "errorbin")
