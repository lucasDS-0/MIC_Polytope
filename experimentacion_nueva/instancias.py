import typing as t
import numpy as np


def generar_grafo_random(n: int, m: int) -> t.List[t.Tuple[int, int]]:
	"""Genera grafo con m aristas y n vértices, sin aristas repetidas""" 
	aristas = set()
	while len(aristas) != m:
		e = np.random.randint(1, n, size=2)
		if e[0] != e[1]:
			aristas.add(frozenset(e))
	return [tuple(e) for e in aristas]


def guardar_grafo(n, g, h, path_archivo: str):
	lineas = [f"{n} {len(g)} {len(h)}\n"]
	for e in g:
		lineas.append(f"{e[0]} {e[1]}\n")
	for e in h:
		lineas.append(f"{e[0]} {e[1]}\n")
	with open(path_archivo, "w") as f:
		f.writelines(lineas)


def generar_instancia(n: int, mG: int, mH: int, path_archivo: str):
	g = generar_grafo_random(n, mG)
	h = generar_grafo_random(n, mH)
	guardar_grafo(n, g, h, path_archivo)
