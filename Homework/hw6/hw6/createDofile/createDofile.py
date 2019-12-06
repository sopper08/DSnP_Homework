import ipdb
import sys
import argparse
from pathlib import Path
import logging
import os

def main(args):

    with open(args.aagFile, 'r') as f:
        l = f.readline()
    l = int(l.split(" ")[1])

    with open(args.aagFile+".dofile", "w") as f:
        f.write("cirr tests.fraig/cirp-n.aag\n")
        f.write("cirp -s\n")
        f.write("cirp -n\n")
        f.write("cirp -pi\n")
        f.write("cirp -po\n")
        f.write("cirp -fl\n")

        for i in range(l+30):
            f.write("cirg {}\n".format(str(i)))
            for j in range(7):
                f.write("cirg {} -fanin {}\n".format(i, j))
                f.write("cirg {} -fanout {}\n".format(i, j))

        f.write("q -f\n")

    os.system("./ref/cirTest-ref -f {} > ref.log 2>&1".format(args.aagFile+".dofile"))
    os.system("./cirTest -f {} > my.log 2>&1".format(args.aagFile+".dofile"))
    os.system("vimdiff my.log ref.log")



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
    with ipdb.launch_ipdb_on_exception():
        sys.breakpointhook = ipdb.set_trace
        args = parse_args()
        main(args)