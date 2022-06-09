from pathlib import Path
import os

diff_combo = Path('./diff/combotest')
diff_combo_test_list = [p for p in diff_combo.glob('./*')]
diff_combo_test_list = sorted(diff_combo_test_list, key=lambda x: int(x.name))

for p in diff_combo_test_list:
    print(f"test {p.name}: ")
    print("--------------------------------")
    t_out_path = p/'t.out'
    m_out_path = p/'m.out'
    os.system(f'diff {t_out_path} {m_out_path}')
    print()