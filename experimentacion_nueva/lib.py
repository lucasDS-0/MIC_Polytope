import subprocess
import json


def run(path_instancia, heuristica, path_ejecutable):
	cmd = f"cat {path_instancia}.in | {path_ejecutable} {heuristica} --json"
	ps = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
	output = ps.communicate()[0]
	print(output.decode())
	return json.loads(output.decode())
