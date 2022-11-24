import csv
import random

import numpy as np
import pandas as pd

with open('station5_10_p60.csv', 'r') as csvfile:
    reader = csv.reader(csvfile)

    a = 0
    sum = 0
    for i, row in enumerate(reader):
        if i != 0:
            ran = random.randint(0, 40)
            sum = sum + abs(ran - int(row[-1]))
            a = a + 1

    print(sum/a)
