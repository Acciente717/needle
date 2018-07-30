import os
import sys
import wrapper.disassemble as disas
import wrapper.slave as slave
import queue
import pandas as pd
from optparse import OptionParser

usage = "usage: %prog [options] binary_dir" 
parser = OptionParser(usage=usage)
parser.add_option("-t", "--thread", dest="threadnum",  
                  help="designate thread number", metavar="THREAD",
                  action="store", type="int", default=1)
parser.add_option("-o", "--output", dest="output",
                 help="designate output file path", metavar="FILE",
                 action="store", type="string")
(options, args) = parser.parse_args()


if len(args) != 1:
    parser.print_help()
    exit(1)
threadnum = options.threadnum
binary_path = args[0]

if options.output is not None:
    opath, oname = os.path.split(options.output)
    if not os.access(opath, os.F_OK):
        print("Fatal error! Cannot open output file path!")
        exit(1)
    if not os.access(opath, os.W_OK):
        print("Permission denied! Cannot open output file to write!")
        exit(1)
    if not os.path.exists(options.output):
        os.mknod(options.output)
    else:
        if not os.access(options.output, os.W_OK):
            print("Permission denied! Cannot open output file to write!")
            exit(1)


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

if options.output is None:
    print(res_lst)
else:
    csv = pd.Series(res_lst)
    csv.to_csv(options.output)
