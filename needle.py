import os
import wrapper.disassemble as disas
import wrapper.slave as slave
import queue
import pandas as pd
from optparse import OptionParser

# Build options and arguments parser.
usage = "Usage: python %prog [options] binary_dir" 
parser = OptionParser(usage=usage)
parser.add_option("-t", "--thread", dest="threadnum",  
                  help="designate thread number", metavar="THREAD",
                  action="store", type="int", default=1)
parser.add_option("-o", "--output", dest="output",
                 help="designate output file path [default: stdout]"
                 , metavar="FILE", action="store", type="string")
parser.add_option("-a", "--alpha", dest="alpha",
                  help="parameter alpha for bincmp [default: 10.0]"
                  , metavar="ALPHA", action="store", type="float", default=10.0)
parser.add_option("-b", "--beta", dest="beta",
                  help="parameter beta for bincmp [default: 0.5]"
                  , metavar="BETA", action="store", type="float", default=0.5)
parser.add_option("-w", "--omega", dest="omega",
                  help="parameter omega for bincmp [default: -1]"
                  , metavar="OMEGA", action="store", type="int", default=-1)
(options, args) = parser.parse_args()


# Analyze options.
threadnum = options.threadnum
params = {"alpha" : options.alpha,
          "beta" : options.beta,
          "omega" : options.omega}


# Analyze arguments.
if len(args) != 1:
    parser.print_help()
    exit(1)
binary_path = args[0]


# Check availability of output path.
if options.output is not None:
    try:
        os.mknod(options.output)
    except FileNotFoundError:
        print("Fatal error! Output file path does not exist!")
        exit(1)
    except PermissionError:
        print("Permission denied! Cannot open output file to write!")
        exit(1)
    except FileExistsError:
        if not os.path.isfile(options.output):
            print("Fatal error! Output path does not point to a regular file.")
            exit(1)
        if not os.access(options.output, os.W_OK):
            print("Permission denied! Cannot open output file to write!")
            exit(1)
        print("Warning: output file exists!")
        while True:
            flag = input("Overwrite existing file? [y/n]: ")
            if flag == "y":
                break
            elif flag == "n":
                print("Operation abort!")
                exit(1)


# List binary files.
try:
    bin_lst = os.listdir(binary_path)
except FileNotFoundError:
    print("Fatal error! Binary file directory does not exist!")
    exit(1)
except PermissionError:
    print("Permission denied! Cannot open binary code directory!")
    exit(1)
except NotADirectoryError:
    print("Fatal error! \"" + binary_path + "\" is not a directory!")
    exit(1)


# Disassemble binary files.
disas_lst = [(binary, disas.filtered_disassembly(os.path.join(binary_path, binary)))
             for binary in bin_lst]


# Create job queue.
job_queue = queue.Queue()
for i in range(len(bin_lst)):
    for j in range(len(bin_lst)):
        if i != j:
            job_queue.put((i, j))


# Call bincmp kernel.
res_lst = slave.multithread_bincmp(params, threadnum, job_queue, disas_lst)


# Sort result by names of binary files.
res_lst.sort()


# Output results.
csv = pd.Series(res_lst)
if options.output is None:
    print(csv)
else:
    csv.to_csv(options.output)
