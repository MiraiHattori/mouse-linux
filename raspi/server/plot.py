#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np

while True:
  xs = [0]
  ys = [0]
  x = 0
  y = 0
  cnt = 0
  with open("pattern.txt", "r") as f:
    for line in f:
      l = line.split(',')
      cnt+=1
      x += int(l[0])
      y -= int(l[1])
      if (cnt % 5 == 0):
          xs.append(x)
          ys.append(y)
  fig = plt.figure(0)
  ax = fig.add_subplot(1, 1, 1)
  plt.plot(xs, ys, '.-')
  ax.set_aspect('equal')
  plt.show()
