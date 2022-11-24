import matplotlib.pyplot as plt
import pandas as pd
import time
import datetime

data = pd.read_csv("station5.csv")

time_list = list(data.iloc[:, 1])
y = list(data.iloc[:, 6])

x=[]
for i in time_list:
    timeArray = time.strptime(i, "%Y-%m-%d %H:%M:%S")
    x.append(int(time.mktime(timeArray)))

fig, square = plt.subplots(figsize=(21, 7))
square.scatter(x, y, s=10, c='b', marker='o', label='Bikes available')
square.legend()
square.set_xlabel("time stamp")
square.set_ylabel("bikes available")
plt.show()
