from pathlib import Path
import os
import subprocess
from colorama import Fore, Back, Style, init
import string
import random
from typing import Callable
import time


os.system('rm -rf ./diff')

dofile_dir_path = Path('./dofile')
diff_dir_path = (dofile_dir_path.parent/'diff')
diff_dir_path.mkdir(parents=True)
ref_p2_p = Path('./ref/p2Run-linux')
my_p2_p = Path('./p2Run')

def run_p(program_path, df_path):
    o = ""
    if isinstance(program_path, Path):
        program_path = program_path.absolute()
    with open(df_path, 'r') as r:
        p = subprocess.Popen([program_path], stdin=r, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        o, _ = p.communicate()
    return o

# unittest
print("unittest")
print("--------------------------------")
for p in dofile_dir_path.glob('./unit/*'):
    print(p.name, ":")
    diff_unittest_path = diff_dir_path/'unittest'/p.name
    diff_unittest_path.mkdir(parents=True)
    for df in p.glob('./*.dofile'):
        print("  test: ", df.name)
        teacher_o = run_p(ref_p2_p, df)
        my_o = run_p(my_p2_p, df)
        if teacher_o==my_o:
            print(Fore.GREEN+"    -> pass"+Style.RESET_ALL)
        else:
            print(Fore.RED+"    -> diff!!!"+Style.RESET_ALL)
            with open(diff_unittest_path/f'{df.name}_t.o', 'x') as w:
                w.write(teacher_o.decode())
            with open(diff_unittest_path/f'{df.name}_m.o', 'x') as w:
                w.write(my_o.decode())
    print()

# combotest
def ADD():
    key_length = random.randint(1, 10)
    key = ''.join(random.choice(string.ascii_letters+string.digits+'_') for i in range(key_length))
    value = random.randint(-10000, 10000)
    return f"ADD {key} {value}"

cmd_dict = {
    'ADD': ADD,
    'SUM': 'SUM',
    'AVE': 'AVE',
    'MAX': 'MAX',
    'MIN': 'MIN',
    'PRINT': 'PRINT'
}

print("combotest")
print("--------------------------------")
diff_combo_dir = diff_dir_path/'combotest'
diff_combo_dir.mkdir(parents=True)

def create_combo_dofile(length):
    with open('combo.dofile', 'w') as w:
        w.write('test1.json'+'\n')
        for i in range(length):
            cmd = cmd_dict[random.choice(list(cmd_dict.keys()))]
            if isinstance(cmd, Callable):
                cmd = cmd()
            w.write(cmd+'\n')
        w.write('PRINT'+'\n')
        w.write('EXIT'+'\n')

for i in range(100):
    print(f"  test {i}: ")
    length = random.randint(10, 1000)
    create_combo_dofile(length)
    teacher_o = run_p(ref_p2_p, 'combo.dofile')
    my_o = run_p(my_p2_p, 'combo.dofile')
    if teacher_o==my_o:
        print(Fore.GREEN+"    -> pass"+Style.RESET_ALL)
    else:
        print(Fore.RED+"    -> diff!!!"+Style.RESET_ALL)
        test_out_dir = (diff_combo_dir/f'{i}')
        test_out_dir.mkdir(parents=True)
        Path('combo.dofile').rename(target=test_out_dir/'combo.dofile')
        with open(test_out_dir/'t.out', 'x') as w:
            w.write(teacher_o.decode())
        with open(test_out_dir/'m.out', 'x') as w:
            w.write(my_o.decode())

Path('combo.dofile').unlink()

# read: time
# TODO: