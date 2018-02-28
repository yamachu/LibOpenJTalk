import re
import os
from pathlib import Path

proj_root = Path(os.path.dirname(os.path.abspath(__file__))).parent
target_dir = proj_root / 'library'

with open(target_dir / 'libopenjtalk.h') as f:
    funcs = [s.strip() for s in f if 'Open_JTalk_' in s]

func_name_re = re.compile(r'.*(Open_JTalk_.*)\(.*')

func_names = [func_name_re.search(s).group(1) for s in funcs]

with open(target_dir / 'open_jtalk.def', 'w') as f:
    f.write('LIBRARY open_jtalk\n')
    f.write('\n')
    f.write('EXPORTS\n')
    for i, s in enumerate(func_names, 1):
        f.write('\t{} @{}\n'.format(s, i))
