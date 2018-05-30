# gravity-sim

simulation of celestial bodies: gravity, collision, and merging

### compile simulation

in root folder:
```
g++ main.cpp -w -lGL -lGLU -lglut -o sol
```

### compile GUI

in qt folder:
```
qmake -project && qmake qt.pro && make
```
