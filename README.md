# Needle
An implementation of the Needle Algorithm proposed by Yanyan Jiang and Chang Xu in *Needle: Detecting Code Plagiarism on Student Submissions*, in SIGCSE China, 2018.

## Installation and Building
```
git clone https://github.com/Acciente717/needle.git
cd needle
make
```

## Usage
Compile all testing source code with -O2 or higher level optimization turned on, and put all binary files in a folder.

Run the following command.
```
python needle.py [options] binary_dir
```

## Options
#### --thread, -t
Designating the number of running threads. [default 1]
#### --output, -o
Designating output csv file path. If not set, results will be printed to *stdout*.
#### --alpha, -a
Parameter for *bincmp* kernel, which controls an anttenuation rate. See the paper for details. [default 10.0]
#### --beta, -b
Parameter for *bincmp* kernel, which controls a bias. See the paper for details. [default 0.5]
#### --omega, -w
Parameter for *bincmp* kernel, which controls the size of sliding window in a function. Setting to -1 will lead to using full function instead of a block of instructions. See the paper for details. [default -1]