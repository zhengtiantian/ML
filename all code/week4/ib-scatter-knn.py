import numpy as np
from matplotlib.colors import ListedColormap
from sklearn.neighbors import KNeighborsClassifier
import matplotlib.pyplot as plt

data = str(open("data2").read()).split('\n')
dataset = []
px, py, nx, ny = [], [], [], []
for d in data[1:]:
    nums = d.split(',')
    dxy = []
    dxy.append(float(nums[0]))
    dxy.append(float(nums[1]))
    dxy.append(float(nums[2]))
    dataset.append(dxy)
    if nums[2] == '1':
        px.append(float(nums[0]))
        py.append(float(nums[1]))
    else:
        nx.append(float(nums[0]))
        ny.append(float(nums[1]))
dataset = np.array(dataset)

train = dataset.take([0, 1], axis=1)
target = dataset[:, 2]


K=13
knn = KNeighborsClassifier(n_neighbors=K)
knn.fit(train,target)

fig, ax = plt.subplots(figsize=(7, 7))
xp = np.linspace(-1, 1, 300)
yp = np.linspace(-1, 1, 300)
x1, y1 = np.meshgrid(xp, yp)
xy = np.c_[x1.ravel(), y1.ravel()]
y_pred = knn.predict(xy).reshape(x1.shape)
custom_cmap = ListedColormap(['#fafab0', '#9898ff', '#a0faa0'])
ax.contourf(x1, y1, y_pred, alpha=0.3, cmap=custom_cmap)
ax.scatter(px, py, s=10, c='b', marker='o', label='1')
ax.scatter(nx, ny, s=10, c='r', marker='x', label='-1')
plt.title('KNN Decision boundary with K=' +str(K))
ax.legend(loc=1)
ax.set_xlabel("x-axis")
ax.set_ylabel("y-axis")
plt.show()
