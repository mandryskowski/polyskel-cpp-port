import subprocess
import glob
from util import run_executable, run_python_script
from util import python_script_path, python_output_file, cpp_output_file, executable_path


fail = False

for file in glob.glob('../examples/*'):
    print(file)
    # Run the Python script and capture its output
    run_python_script(python_script_path, ['--log', 'DEBUG', file], python_output_file)

    # Run the executable and capture its output
    run_executable(executable_path, file, cpp_output_file)

    out = subprocess.run(['python3', 'compare.py'], check=False)
    if out.returncode != 0:
        fail = True

if fail:
    exit(1)