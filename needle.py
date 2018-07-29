import os
import sys
import wrapper.disassemble as disas
import wrapper.slave as slave
import queue

binary_path = sys.argv[1]
try:
    bin_lst = os.listdir(binary_path)
except FileNotFoundError:
    print("Fatal error! Cannot open binary code directory!")
    exit(1)

disas_lst = [(binary, disas.filtered_disassembly(os.path.join(binary_path, binary)))
             for binary in bin_lst]

job_queue = queue.Queue()
for i in range(len(bin_lst)):
    for j in range(len(bin_lst)):
        if i != j:
            job_queue.put((i, j))

res_lst = slave.multithread_bincmp(4, job_queue, disas_lst)

print(res_lst)
