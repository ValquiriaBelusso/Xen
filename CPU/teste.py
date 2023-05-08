import subprocess
import time
import json
import os

num_cpus = 1 # inicia com uma CPU alocada

# lista que irá armazenar todos os registros das CPU
lista_total_cpus = []
for j in range(num_cpus):
    lista_total_cpus.append([])

# lista que irá conter os ultimos 5 instantes
lista_cpus = []
for j in range(num_cpus):
    lista_cpus.append([])

while True:
  try:
    print(os.system("date +'%T'"))

    for n in range(0, len(lista_total_cpus)):

      a = subprocess.run(['mpstat', '-P', 'ALL', '-o', 'JSON'], check=True, capture_output=True, text=True).stdout
      json_obj = json.loads(a)

      cpu_no = (json_obj.get('sysstat').get('hosts')[0].get('statistics')[0].get('cpu-load')[num_cpus].get('idle'))
      lista_cpus[n].append(cpu_no)
      lista_total_cpus[n].append(cpu_no)

      if(len(lista_cpus[n]) < 5):
        continue

      elif (len(lista_cpus[n]) > 5):
        lista_cpus[n].pop(0) # remove o primeiro elemento da lista temporaria

      media = sum(lista_cpus[n])/5

      print("media:", media)
      print(f"CPU {n}: {lista_cpus[n]}\n")

      if (media <= 20):
        print("Alocando nova CPU... \n")
        os.system("cd /home/teste/Downloads/CPU")
        os.system("./cpu.sh")
        num_cpus += 1
        
        lista_total_cpus.append([])
        for k in range(0, len(lista_total_cpus[n])): # popula com 0 para fins de registro
          lista_total_cpus[num_cpus-1].append(0)
        lista_cpus.append([])

    time.sleep(1)

  except KeyboardInterrupt:
    print("Programa interrompido pelo usuário.")
    break

  except Exception as e:
    print(f"Erro: {e}")
    break
