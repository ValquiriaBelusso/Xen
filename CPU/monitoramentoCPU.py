import subprocess
import json
import os
import time

cpusAlocadas = set()

def alocandoCpu(cpu_number, lista_total_cpus, lista_cpus):
    # Escreve no xenstore para marcar a CPU como online
    cmd = f"sudo xenstore-write -s /local/domain/3/cpu/{cpu_number + 1}/availability online"
    print(cmd)
    os.system(cmd)

    cpusAlocadas.add(cpu_number + 1)

    lista_total_cpus.append([cpu_number])
    for k in range(len(lista_total_cpus[cpu_number])):
        lista_total_cpus[cpu_number].append(0)
    lista_cpus.append([])

    # Ativa a CPU 
    cmd2 = f"echo 1 > /sys/devices/system/cpu/cpu{cpu_number + 1}/online"
    print(cmd2)
    os.system(cmd2)

    return cpu_number + 1


def desalocandoCpu(cpu_number, lista_total_cpus, lista_cpus):
    # Escreve no xenstore para marcar a CPU como offline
    cmd = f"sudo xenstore-write -s /local/domain/3/cpu/{cpu_number}/availability offline"
    print(cmd)
    os.system(cmd)
    
    #print("CPU number", cpu_number)

    #Desativa a CPU
    cmd2 = f"echo 0 > /sys/devices/system/cpu/cpu{cpu_number}/online"
    print(cmd2)
    os.system(cmd2)
    
    #Remove os dados da CPU das listas   
    for k in range(len(lista_total_cpus[cpu_number])):
        lista_total_cpus[cpu_number].pop()
    lista_cpus[cpu_number] = []
    
    cpusAlocadas.remove(cpu_number)

    return cpu_number


def monitor_cpu_usage():

    num_cpus = os.cpu_count()
    lista_total_cpus = [[] for _ in range(num_cpus)]
    lista_cpus = [[] for _ in range(num_cpus)]

    cpu_number = 0  # Inicia com a primeira CPU
    
    cpusAlocadas.add(cpu_number)

    while True:
        try:
            current_time = time.strftime('%H:%M:%S')
            print(current_time)

            for n in range(len(lista_total_cpus)):
                # Captura as estatísticas de uso da CPU
                a = subprocess.run(
                    ["mpstat", "-P", "ALL", "-o", "JSON", "1", "1"], capture_output=True).stdout
                json_obj = json.loads(a)

                # Obtém o valor de ociosidade da CPU atual
                cpu_no = json_obj.get('sysstat').get('hosts')[0].get(
                    'statistics')[0].get('cpu-load')[n].get('idle')
                lista_cpus[n].append(cpu_no)
                lista_total_cpus[n].append(cpu_no)
                
                

                if len(lista_cpus[n]) < 5:
                    continue
                elif len(lista_cpus[n]) > 5:
                    lista_cpus[n].pop(0)

                media = sum(lista_cpus[n]) / 5

                
                print(cpusAlocadas)
                
                print("n" , n)
                
                print("media:", media)
                print(f"CPU {n}: {lista_cpus[n]}")
                
                #print("tamanho lista cpus",len(lista_total_cpus))
                
                print("\n")

                if media <= 20 and n + 1 not in cpusAlocadas:
                    # Se a média de ociosidade estiver abaixo do limite e a CPU não estiver alocada, aloca a CPU
                    cpu_number = alocandoCpu(
                        n, lista_total_cpus, lista_cpus)

                if media >= 80 and n > 0:
                    # Se a média de ociosidade estiver acima do limite e a CPU estiver alocada, desaloca a CPU
                    print("entrou no desalocar")
                    cpu_number = desalocandoCpu(n, lista_total_cpus, lista_cpus)
                    lista_total_cpus.pop()
                           
       
        except KeyboardInterrupt:
            print("Programa interrompido pelo usuário.")
            break

        except Exception as e:
            print(f"Erro: {e}")
            break


monitor_cpu_usage()
