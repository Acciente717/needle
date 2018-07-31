import threading
import queue
import subprocess


def worker(params, job_queue, res_lst, disas_lst):
    """Worker in thread pool."""

    alpha = params["alpha"]
    beta = params["beta"]
    omega = params["omega"]
    
    while True:
        try:
            idx1, idx2 = job_queue.get_nowait()
            bin_name1, disas1 = disas_lst[idx1]
            bin_name2, disas2 = disas_lst[idx2]
            bincmp_input = str(alpha) + " " + str(beta) + "\n" + disas1 + "#\n" + disas2 + "#\n"
            bincmp_process = subprocess.Popen("./bincmp/bincmp", stdin=subprocess.PIPE,
                                             stdout=subprocess.PIPE)
            distance, _ = bincmp_process.communicate(bytes(bincmp_input, encoding="utf-8"))
            distance = float(distance)
            bincmp_process.wait()
            res_lst.append((bin_name1, bin_name2, distance))
            
        except queue.Empty:
            break
        
    return


def multithread_bincmp(params, thread_num, job_queue, disas_lst):
    """Call bincmp with multithread to get pair-wise distance between binaries."""
    
    res_lst = []
    thread_pool = []
    for i in range(thread_num):
        thread = threading.Thread(target=worker, args=(params, job_queue, res_lst, disas_lst))
        thread.start()
        thread_pool.append(thread)
    for thread in thread_pool:
        thread.join()
        
    return res_lst
