import numpy as np
from sklearn.linear_model import Lasso
from sklearn.model_selection import cross_val_score, train_test_split
from sklearn.preprocessing import PolynomialFeatures
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

#Get an array of x, y, z
x = dataset[:, 0]
y = dataset[:, 1]
z = dataset[:, 2]

#Generate training data1 and training target array
train = dataset.take([0, 1], axis=1)
target = dataset[:, 2]

#Add polynomial
Xtrain_poly = PolynomialFeatures(degree=5).fit_transform(train)

#Draw out the error bar
alpha = 0.001
alphas,means,stds = [],[],[]
while alpha<0.03:
    LassoR = Lasso(alpha=alpha)
    scores = cross_val_score(LassoR, Xtrain_poly, target, cv=5)
    alphas.append(alpha)
    means.append(scores.mean())
    stds.append(scores.std())
    alpha = alpha + 0.001

plt.figure(figsize=(12,5))
plt.title('Lasso Regression error bar')
plt.xlabel('alpha')
plt.ylabel('mean accuracy')
plt.errorbar(alphas,means,stds,mfc='red')
plt.show()