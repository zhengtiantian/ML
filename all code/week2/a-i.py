import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# Read data1
data = open("data")
dataArr = str(data.read()).split('\n')

#Divide the data1 into x, y arrays of 1 and xy arrays of -1 according to -1 and 1
px,py,nx,ny = [],[],[],[]
for d in dataArr[1:]:
    nums = d.split(',')
    if nums[2] == '1':
        px.append(float(nums[0]))
        py.append(float(nums[1]))
    else:
        nx.append(float(nums[0]))
        ny.append(float(nums[1]))

#Draw a picture
fig,square = plt.subplots(figsize=(7, 7))
square.scatter(px,py,s=10,c='r',marker ='o',label = '1')
square.scatter(nx,ny,s=10,c='b',marker ='x',label = '-1')
square.legend()
square.set_xlabel("x-axis")
square.set_ylabel("y-axis")
plt.show()