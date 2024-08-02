import logging
import argparse
import re
from PIL import Image, ImageDraw
from util import read_subtrees, LANGUAGE, run_python_script, run_executable
from util import python_script_path, python_output_file, cpp_output_file, executable_path

if __name__ == "__main__":
    logging.basicConfig()

    argparser = argparse.ArgumentParser(description="Construct the straight skeleton of a polygon. The polygon is to be given as a counter-clockwise series of vertices specified by their coordinates: see the example files for the exact format.")
    argparser.add_argument('polygon_file', metavar="<polygon-file>", type=argparse.FileType('r'), help="text file describing the polygon ('-' for standard input)")
    argparser.add_argument('--verbose', '--v', action="store_true", default=False, help="Show construction of the skeleton")
    argparser.add_argument('--log', dest="loglevel", choices=['DEBUG', 'INFO', 'WARNING', 'ERROR'], default='WARNING', help="Set log level")
    args = argparser.parse_args()

    polygon_line_pat = re.compile(r"\s*(?P<coord_x>\d+(\.\d+)?)\s*,\s*(?P<coord_y>\d+(\.\d+)?)\s*(#.*)?")

    contours = []
    poly = []
    for line in args.polygon_file:
        line = line.strip()
        if not line or line.startswith('#'):
            continue

        if line.startswith('-'):
            contours.append(poly)
            poly = []
            continue

        match = polygon_line_pat.match(line)
        poly.append((float(match.group("coord_x")), float(match.group("coord_y"))))

    if not args.polygon_file.isatty():
        args.polygon_file.close()

    contours.append(poly)
    poly = contours[0]
    holes = contours[1:] if len(contours) > 0 else None
    bbox_end_x = int(max(poly, key=lambda x: x[0])[0]+20)
    bbox_end_y = int(max(poly, key=lambda x: x[1])[1]+20)

     # Run the Python script and capture its output
    run_python_script(python_script_path, ['--log', 'DEBUG', args.polygon_file.name], python_output_file)

    # Run the executable and capture its output
    run_executable(executable_path, args.polygon_file.name, cpp_output_file)

    files = [('out_cpp.txt', LANGUAGE.CPP), ('out_python.txt', LANGUAGE.PYTHON)]
    for file, lang in files:
        im = Image.new("RGB", (bbox_end_x, bbox_end_y), "white")
        draw = ImageDraw.Draw(im)


        for contour in contours:
            for point, next in zip(contour, contour[1:]+contour[:1]):
                draw.line((point[0], point[1], next[0], next[1]), fill=0)

        skeleton = read_subtrees(file, lang)
        
        print(f"{lang} output:")
        for res in skeleton:
            print(res)
        draw.text((0, 0), str(lang), fill="black")
        for arc in skeleton:
            for sink in arc['sinks']:
                draw.line((arc['source'][0], arc['source'][1], sink[0], sink[1]), fill="red")
        im.show(title=str(lang))
