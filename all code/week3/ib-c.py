import numpy as np
from scipy.interpolate import griddata
import matplotlib.pyplot as plt
from sklearn.preprocessing import PolynomialFeatures
from sklearn.linear_model import Lasso

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

#Get an array of x, y, z
x = dataset[:, 0]
y = dataset[:, 1]
z = dataset[:, 2]

#Generate training data1 and training target array
X_train = dataset.take([0, 1], axis=1)
y_train = dataset[:, 2]

#Add polynomial
polyFeatures = PolynomialFeatures(degree=5)
# print(polyFeatures.get_feature_names)
X_Poly = polyFeatures.fit_transform(X_train)
print(polyFeatures.get_feature_names())

#Generate Lasso regression model and train the model
LassoR = Lasso(alpha=0.001)
LassoR.fit(X_Poly, y_train)

#Generate test set and add polynomial
Xtest = []
grid = np.linspace(-1, 1)
for i in grid:
    for j in grid:
        Xtest.append([i, j])
Xtest = np.array(Xtest)
Xtest_poly = polyFeatures.fit_transform(Xtest)

#Use the test set to predict and get the target
X_pre = LassoR.predict(Xtest_poly)

Xtest_x = Xtest[:, 0]
Xtest_y = Xtest[:, 1]

#Generate plot_surface data1
xi = np.linspace(min(x), max(x))
yi = np.linspace(min(y), max(y))
X, Y = np.meshgrid(xi, yi)
Z = griddata((x, y), z, (X, Y), 'nearest')
xitest = np.linspace(min(Xtest_x), max(Xtest_x))
yitest = np.linspace(min(Xtest_y), max(Xtest_y))
Xitest, Yitest = np.meshgrid(xitest, yitest)
Z_test = griddata((Xtest_x, Xtest_y), X_pre, (Xitest, Yitest), 'nearest')

#Create 3D canvas
fig = plt.figure(figsize=(12, 12))
ax = fig.add_subplot(projection='3d')

#Draw the first 3D surface
ax.scatter(dataset[:, 0], dataset[:, 1], dataset[:, 2], label='training point')
Curve1 = ax.plot_surface(Xitest, Yitest, Z_test, rstride=1, cstride=1, color='r', label='predict with alpha=0.001', alpha=0.5)
Curve1._facecolors2d = Curve1._facecolor3d
Curve1._edgecolors2d = Curve1._edgecolor3d

#Generate the second model and draw the surface
LassoR2 = Lasso(alpha=0.1)
LassoR2.fit(X_Poly, y_train)
X_pre2 = LassoR2.predict(Xtest_poly)
Z_test2 = griddata((Xtest_x, Xtest_y), X_pre2, (Xitest, Yitest), 'nearest')
Curve2 = ax.plot_surface(Xitest, Yitest, Z_test2, rstride=1, cstride=1, color='b', label='predict with alpha=0.1', alpha=0.5)
Curve2._facecolors2d = Curve2._facecolor3d
Curve2._edgecolors2d = Curve2._edgecolor3d

#Generate the third model and draw the surface
LassoR3 = Lasso(alpha=1)
LassoR3.fit(X_Poly, y_train)
X_pre3 = LassoR3.predict(Xtest_poly)
Z_test3 = griddata((Xtest_x, Xtest_y), X_pre3, (Xitest, Yitest), 'nearest')
Curve3 = ax.plot_surface(Xitest, Yitest, Z_test3, rstride=1, cstride=1, color='g', label='predict with alpha=1', alpha=0.5)
Curve3._facecolors2d = Curve3._facecolor3d
Curve3._edgecolors2d = Curve3._edgecolor3d

#Set picture parameters and draw 3D pictures flatly
ax.legend()
ax.set_xlabel('x-axis')
ax.set_ylabel('y-axis')
ax.set_zlabel('z-axis')
ax.set_title('The predicted surface varies with different alpha')
plt.show()
