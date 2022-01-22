#!/usr/bin/python3

import pandas as pd
import seaborn as sns
import numpy as np
import matplotlib.pyplot as plt

from lib import *


PATH_EJECUTABLE = "../build/tp2"


def obtener_optimo(path_instancia):
	with open(f"{path_instancia}.out", "r") as f:
		contenido = f.readlines()
	return contenido[0].strip()


def obtener_datos(heuristica):
	data = []
	for i in range(3, 16):
		path_instancia = f"instancias/CMI_n{i*2}"
		optimo = obtener_optimo(path_instancia)
		resultado = run(path_instancia, heuristica, PATH_EJECUTABLE)
		data.append((resultado["n"], resultado["mG"], resultado["mH"], resultado["impacto"], int(optimo)))
	return pd.DataFrame(data, columns=["n", "mG", "mH", "impacto_obtenido", "impacto_optimo"])


def grafico(dataframe):
    with sns.axes_style("darkgrid"):
    	fig = sns.scatterplot(data=dataframe, x='instancia', y='error H1', color="green")
    	fig = sns.scatterplot(data=dataframe, x='instancia', y='error H2', color="purple")
    	fig.set_xticks(range(6,30)[::2])
    	fig.set(xlabel='Instancia', ylabel='Error (%)')
    	fig.legend(labels=['H1','H2'])
    plt.savefig(f"exp1")
    plt.close()


data_h1 = obtener_datos("H1")
data_h2 = obtener_datos("H2")
data_h1["error"] = data_h1.apply(lambda row: (row.impacto_optimo - row.impacto_obtenido) / row.impacto_optimo * 100, axis = 1) 
data_h2["error"] = data_h2.apply(lambda row: (row.impacto_optimo - row.impacto_obtenido) / row.impacto_optimo * 100, axis = 1) 

data = [data_h1["error"], data_h2["error"], data_h1["n"]]
headers = ["error H1", "error H2", "instancia"]
error_data = pd.concat(data, axis=1, keys=headers)

print(error_data)
grafico(error_data)
