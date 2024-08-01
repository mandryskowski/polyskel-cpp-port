from util import read_subtrees, LANGUAGE

def compare_numbers(num1, num2, tolerance=0.01):
    return abs(num1 - num2) <= tolerance

def compare_subtrees(subtrees1, subtrees2, tolerance=0.01):
    if len(subtrees1) != len(subtrees2):
        return False
    for subtree1, subtree2 in zip(subtrees1, subtrees2):
        if (compare_numbers(subtree1["source"][0], subtree2["source"][0], tolerance) and
            compare_numbers(subtree1["source"][1], subtree2["source"][1], tolerance) and
            compare_numbers(subtree1["height"], subtree2["height"], tolerance) and
            all(any(compare_numbers(s1[0], s2[0], tolerance) and compare_numbers(s1[1], s2[1], tolerance) for s2 in subtree2["sinks"]) for s1 in subtree1["sinks"]) and
            len(subtree1["sinks"]) == len(subtree2["sinks"])):
            continue
        
        print("Subtrees are different: cpp", subtree1, "!= python", subtree2)
        return False
        
    return True

# Read and parse both files
subtrees1 = read_subtrees('out_cpp.txt', LANGUAGE.CPP)
subtrees2 = read_subtrees('out_python.txt', LANGUAGE.PYTHON)

# Compare the subtrees
are_same = compare_subtrees(subtrees1, subtrees2)
print(f"The files have the same subtrees: {are_same}")
if not are_same:
    exit(1)
