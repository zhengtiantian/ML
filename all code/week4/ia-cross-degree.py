import numpy as np
from sklearn.linear_model import LogisticRegression
from sklearn.preprocessing import PolynomialFeatures, StandardScaler
from matplotlib.colors import ListedColormap
import matplotlib.pyplot as plt




# Get data1 from file
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

degree = 1
poly_train = PolynomialFeatures(degree=degree).fit_transform(train)
LR = LogisticRegression(penalty='l2', C=1)
LR.fit(poly_train,target)
fig, ax = plt.subplots(figsize=(7, 7))


axis = [-1, 1, -1, 1]
x0, x1 = np.meshgrid(
    np.linspace(axis[0], axis[1], int((axis[1] - axis[0]) * 100)).reshape(-1, 1),
    np.linspace(axis[2], axis[3], int((axis[3] - axis[2]) * 100)).reshape(-1, 1),
)
X_new = np.c_[x0.ravel(), x1.ravel()]
X_new = PolynomialFeatures(degree=degree).fit_transform(X_new)
y_predict = LR.predict(X_new)
zz = y_predict.reshape(x0.shape)

custom_cmap = ListedColormap(['#EF9A9A', '#FFF59D', '#90CAF9'])
ax.contourf(x0, x1, zz, linewidth=1, cmap=custom_cmap)
# plt.contour(x0, x1, zz, [0])
ax.scatter(px, py, s=10, c='b', marker='o', label='1')
ax.scatter(nx, ny, s=10, c='r', marker='x', label='-1')
ax.legend()
plt.title('Logistic Regression with C=' + str(degree))
ax.set_xlabel("x-axis")
ax.set_ylabel("y-axis")
plt.show()
