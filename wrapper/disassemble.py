import subprocess

def extract_functions(lines):
    """Extract functions from raw disassembly text."""
    
    line_num = len(lines)
    idx = 0
    functions = []

    while idx < line_num:

        line = lines[idx]

        if "<" in line and ">:" in line:

            func_name_start = line.find("<") + 1
            func_name_end = line.find(">")
            func_name = line[func_name_start:func_name_end]
            idx += 1
            line = lines[idx]

            function = []
            while line != "":

                try:
                    instruction = line.split("\t")[2]
                    opcode = instruction.split(" ")[0]
                    function.append(opcode)
                except IndexError:
                    pass

                idx += 1
                if idx >= line_num:
                    break
                line = lines[idx]

            functions.append((func_name, function))

        idx += 1

    return functions


def filter_functions(functions):
    """Filter out library functions."""

    return functions


def generate_bincmp_input(functions):
    """Generate input stream for bincmp."""

    s = ""
    for func_name, function in functions:
        s += "!begin_func " + func_name + "\n"
        s += "\n".join(function) + "\n"
        s += "~end_func\n"
    return s


def disassembly_lines(binary_path):
    """Use subprocess call of objdump to disassembly binary code."""
    
    process_objdump = subprocess.Popen("objdump -d " + binary_path, 
                                       stdout=subprocess.PIPE, shell=True)
    retval, _ = process_objdump.communicate()
    process_objdump.wait()
    lines = retval.decode("utf-8").split("\n")
    
    return lines


def filtered_disassembly(binary_path):
    """
    Generate C++ friendly disassembly code with library functions
    filtered out.
    """
    
    lines = disassembly_lines(binary_path)
    functions = extract_functions(lines)
    functions = filter_functions(functions)
    res = generate_bincmp_input(functions)
    
    return res

