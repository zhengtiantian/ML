import numpy as np
from sklearn.linear_model import Ridge
from sklearn.model_selection import cross_val_score
from sklearn.preprocessing import PolynomialFeatures
import matplotlib.pyplot as plt

#Code function is the same as (ii)(a)
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
Xtrain_poly = PolynomialFeatures(degree=5).fit_transform(X_train)


alpha = 0.01
alphas,means,stds = [],[],[]
while alpha<0.4:
    RidgeR = Ridge(alpha=alpha)
    scores = cross_val_score(RidgeR, Xtrain_poly, y_train, cv=5)
    alphas.append(alpha)
    means.append(scores.mean())
    stds.append(scores.std())
    alpha = alpha + 0.01

plt.figure(figsize=(12,5))
plt.title('Ridge Regression error bar')
plt.xlabel('alpha')
plt.ylabel('mean accuracy')
plt.errorbar(alphas,means,stds,mfc='red')
plt.show()