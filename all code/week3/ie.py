import numpy as np
from scipy.interpolate import griddata
import matplotlib.pyplot as plt
from sklearn.preprocessing import PolynomialFeatures
from sklearn.linear_model import Ridge

#Code function is the same as (i)(b-c)
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

X_train = dataset.take([0, 1], axis=1)
y_train = dataset[:, 2]

polyFeatures = PolynomialFeatures(degree=5)
print(polyFeatures.get_feature_names)
X_Poly = polyFeatures.fit_transform(X_train)
print(polyFeatures.get_feature_names())
ridgeR = Ridge(alpha=0.001)
ridgeR.fit(X_Poly, y_train)

Xtest = []
grid = np.linspace(-1, 1)
for i in grid:
    for j in grid:
        Xtest.append([i, j])
Xtest = np.array(Xtest)
Xtest_poly = polyFeatures.fit_transform(Xtest)

X_pre = ridgeR.predict(Xtest_poly)

Xtest_x = Xtest[:, 0]
Xtest_y = Xtest[:, 1]

xi = np.linspace(min(x), max(x))
yi = np.linspace(min(y), max(y))
X, Y = np.meshgrid(xi, yi)
Z = griddata((x, y), z, (X, Y), 'nearest')

xitest = np.linspace(min(Xtest_x), max(Xtest_x))
yitest = np.linspace(min(Xtest_y), max(Xtest_y))
Xitest, Yitest = np.meshgrid(xitest, yitest)
Z_test = griddata((Xtest_x, Xtest_y), X_pre, (Xitest, Yitest), 'nearest')

fig = plt.figure(figsize=(12, 12))
ax = fig.add_subplot(projection='3d')

ax.scatter(dataset[:, 0], dataset[:, 1], dataset[:, 2], label='training point')
Curve1 = ax.plot_surface(Xitest, Yitest, Z_test, rstride=1, cstride=1, color='r', label='predict with alpha=0.001', alpha=0.5)
Curve1._facecolors2d = Curve1._facecolor3d
Curve1._edgecolors2d = Curve1._edgecolor3d

ridgeR2 = Ridge(alpha=0.1)
ridgeR2.fit(X_Poly, y_train)
X_pre2 = ridgeR2.predict(Xtest_poly)
Z_test2 = griddata((Xtest_x, Xtest_y), X_pre2, (Xitest, Yitest), 'nearest')
Curve2 = ax.plot_surface(Xitest, Yitest, Z_test2, rstride=1, cstride=1, color='b', label='predict with alpha=0.1', alpha=0.5)
Curve2._facecolors2d = Curve2._facecolor3d
Curve2._edgecolors2d = Curve2._edgecolor3d

ridgeR3 = Ridge(alpha=1)
ridgeR3.fit(X_Poly, y_train)
X_pre3 = ridgeR3.predict(Xtest_poly)
Z_test3 = griddata((Xtest_x, Xtest_y), X_pre3, (Xitest, Yitest), 'nearest')
Curve3 = ax.plot_surface(Xitest, Yitest, Z_test3, rstride=1, cstride=1, color='g', label='predict with alpha=1', alpha=0.5)
Curve3._facecolors2d = Curve3._facecolor3d
Curve3._edgecolors2d = Curve3._edgecolor3d

ax.legend()
ax.set_xlabel('x-axis')
ax.set_ylabel('y-axis')
ax.set_zlabel('z-axis')
ax.set_title('The predicted surface varies with different alpha')
plt.show()
