import os
import sys
import wrapper.disassemble as disas
import wrapper.slave as slave
import queue
from optparse import OptionParser

usage = "usage: %prog [options] binary_dir" 
parser = OptionParser(usage=usage)
parser.add_option("-t", "--thread", dest="threadnum",  
                  help="designate thread number", metavar="THREAD",
                  action="store", type="int", default=1)
(options, args) = parser.parse_args()


if len(args) != 1:
    parser.print_help()
    exit(1)
threadnum = options.threadnum
binary_path = args[0]


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

res_lst = slave.multithread_bincmp(threadnum, job_queue, disas_lst)
res_lst.sort()

print(res_lst)
