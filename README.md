# Polyskel C++ port
This is a C++ port of Bottfy's [polyskel](https://github.com/Botffy/polyskel/tree/master) library.
Also, [celestialphineas's polyskel fork](https://github.com/celestialphineas/polyskel/tree/master) was used as a reference while porting.

## Why do we need polyskel for C++?
To put it simply, there is no good straight skeleton implementation in C++ with a permissive license.
CGAL has the best algorithm but it is licensed under GPL, which is a strong copyleft license.

### Issue with GPL
GPL is viral, which means that if you're using a GPL library, your fork will also have to be licensed under GPL (and everything that is using your GPL work, and everything that is using something using your GPL work, and so on). So everything using CGAL has to be open source which is annoying if you're developing a game. There is a commercial license, but indie developers are unlikely to be able to afford it.

This is not legal advice.

### GPL vs LGPL
LGPL is still a copyleft license, but softer.
Polyskel is licensed under LGPL which means you have to share all modifications to it under a LGPL license (so open source). But it doesn't "infect" the rest of your work so you don't have to make any of your code open source unless it's polyskel files.

This C++ port has to be licensed under LGPL because polyskel is LGPL.

This is not legal advice.

## Build
```
mkdir build
cd build
cmake ..
make
```

## Comparison with polyskel
```
cd build
cd test
python3 render_comparison.py <polygon file>
```
This will render the polygon + skeleton using both celestialphineas's polyskel and the C++ polyskel for comparison.
You might need to install dependencies with pip install.