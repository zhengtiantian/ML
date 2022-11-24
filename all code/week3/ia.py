import numpy as np
import matplotlib.pyplot as plt

#Get data1 from file
data = str(open("data").read()).split('\n')
dataset = []
for d in data[1:]:
    nums = d.split(',')
    dxy = []
    dxy.append(float(nums[0]))
    dxy.append(float(nums[1]))
    dxy.append(float(nums[2]))
    dataset.append(dxy)
dataset = np.array(dataset)
x = dataset[:, 0]
y = dataset[:, 1]
z = dataset[:, 2]

#Draw a 3D scatter plot
fig = plt.figure(figsize=(12, 12))
ax = fig.add_subplot(projection='3d')
ax.scatter(dataset[:,0],dataset[:,1],dataset[:,2])
ax.set_xlabel('x-axis')
ax.set_ylabel('y-axis')
ax.set_zlabel('z-axis')
ax.set_title('3D scatter plot' )
plt.show()
