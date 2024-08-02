import re, subprocess
from enum import Enum

# Paths to the Python script and the executable
python_script_path = 'reference/demo.py'  # Replace with the actual path
executable_path = '../demo'   # Replace with the actual path

# Output files
python_output_file = 'out_python.txt'
cpp_output_file = 'out_cpp.txt'

def parse_cpp_line(line):
    pattern = r"Subtree from \(([\d.]+), ([\d.]+)\) with height ([\d.]+) and sinks \[ ([\d., ()]+) \]"
    match = re.match(pattern, line)
    if match:
        source = (round(float(match.group(1)), 2), round(float(match.group(2)), 2))
        height = round(float(match.group(3)), 2)
        sinks = [tuple(map(lambda x: round(float(x), 2), sink.strip().strip("()").split(", "))) for sink in match.group(4).split(") (")]
        return {"source": source, "height": height, "sinks": sinks}
    return None

def parse_python_line(line):
    pattern = r"Subtree\(source=\(([\d.]+), ([\d.]+)\), height=([\d.]+), sinks=\[([\d., ()]+)\]\)"
    match = re.match(pattern, line)
    if match:
        source = (round(float(match.group(1)), 2), round(float(match.group(2)), 2))
        height = round(float(match.group(3)), 2)
        sinks = [tuple(map(lambda x: round(float(x), 2), sink.strip().strip("()").split(", "))) for sink in match.group(4).split("), (")]
        return {"source": source, "height": height, "sinks": sinks}
    return None


class LANGUAGE(Enum):
    CPP = 0
    PYTHON = 1

def read_subtrees(file_path, language: LANGUAGE):
    with open(file_path, 'r') as file:
        lines = file.readlines()
    subtrees = []
    for line in lines:
        line = line.strip()
        if line.startswith("Subtree"):
            parsed_line = parse_cpp_line(line) if language == LANGUAGE.CPP else parse_python_line(line)
            if parsed_line:
                subtrees.append(parsed_line)
    return subtrees

def run_python_script(script_path, args, output_file):
    try:
        result = subprocess.run(['python3', script_path] + args, capture_output=True, text=True, check=True)
        with open(output_file, 'w') as file:
            file.write(result.stderr)
            file.write(result.stdout)
    except subprocess.CalledProcessError as e:
        print(f"Error running {script_path}: {e}")

def run_executable(executable_path, arg, output_file):
    try:
        result = subprocess.run([executable_path, arg], capture_output=True, text=True, check=True)
        with open(output_file, 'w') as file:
            file.write(result.stdout)
    except subprocess.CalledProcessError as e:
        print(f"Error running {executable_path}: {e}")