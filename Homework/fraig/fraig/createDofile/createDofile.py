import pdb
import sys
import argparse
from pathlib import Path
import logging
import os

def main(args):

    with open(args.aagFile, 'r') as f:
        lines = f.readlines()
    header = lines[0].split(" ")
    
    numOfPoPi = 0
    for line in lines[1:]:
        if len(line.split(" ")) == 1:
            numOfPoPi += 1
        else:
            break
    M = int(header[1])
    numOfPi = int(header[2])
    numOfPo = numOfPoPi - numOfPi
    numOfAnd = int(header[5])

    with open(args.aagFile+".dofile", "w") as f:
        f.write("cirr {}\n".format(args.aagFile))
        f.write("usage\n")
        f.write("cirp -s\n")
        f.write("cirp -n\n")
        f.write("cirp -pi\n")
        f.write("cirp -po\n")
        f.write("cirp -fl\n")

        # PI & CONST0: 0 ~ numOfPi
        for gid in range(numOfPi+1):
            f.write("cirg {}\n".format(str(gid)))
            for level in range(30):
                f.write("cirg {} -fanin {}\n".format(gid, level))
                f.write("cirg {} -fanout {}\n".format(gid, level))
        # PO: M+1 ~ M+numOfPo
        for gid in range(M+1, M+numOfPo+1):
            f.write("cirg {}\n".format(str(gid)))
            for level in range(30):
                f.write("cirg {} -fanin {}\n".format(gid, level))
                f.write("cirg {} -fanout {}\n".format(gid, level))
        
        # AIG: numOfPi+1 ~ numOfPi+numOfAnd
        for gid in range(numOfPi+1, numOfPi+numOfAnd+1):
            f.write("cirg {}\n".format(str(gid)))
            for level in range(30):
                f.write("cirg {} -fanin {}\n".format(gid, level))
                f.write("cirg {} -fanout {}\n".format(gid, level))

        f.write("usage\n")
        f.write("q -f\n")

    os.system("./ref/fraig -f {} > ref.log 2>&1".format(args.aagFile+".dofile"))
    os.system("./fraig -f {} > my.log 2>&1".format(args.aagFile+".dofile"))
    os.system("diff my.log ref.log")



def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('aagFile', type=str)
    args = parser.parse_args()

    return args

if __name__ == "__main__":
    logging.basicConfig(
        format='%(asctime)s | %(levelname)s | %(name)s: %(message)s',
        level=logging.INFO, datefmt='%m-%d %H:%M:%S'
    )
    args = parse_args()
    main(args)