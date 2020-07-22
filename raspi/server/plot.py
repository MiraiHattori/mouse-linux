#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np

while True:
  xs = []
  ys = []
  x = 0
  y = 0
  with open("pattern.txt", "r") as f:
    for line in f:
      l = line.split(',')
      x += int(l[0])
      y -= int(l[1])
      xs.append(x)
      ys.append(y)
  fig = plt.figure(0)
  ax = fig.add_subplot(1, 1, 1)
  plt.plot(xs, ys)
  ax.set_aspect('equal')
  plt.show()
