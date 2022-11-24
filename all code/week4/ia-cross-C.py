import numpy as np
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import cross_val_score
from sklearn.preprocessing import PolynomialFeatures
from matplotlib.transforms import Affine2D
import matplotlib.pyplot as plt

# Get data1 from file
data = str(open("data2").read()).split('\n')
dataset = []
for d in data[1:]:
    nums = d.split(',')
    dxy = []
    dxy.append(float(nums[0]))
    dxy.append(float(nums[1]))
    dxy.append(float(nums[2]))
    dataset.append(dxy)
dataset = np.array(dataset)

train = dataset.take([0, 1], axis=1)
target = dataset[:, 2]

C = 0.001
# C = 0.1
# C = 1
# C = 10
# C = 1000
Cs, acc_means, acc_stds, f1_means, f1_stds = [], [], [], [], []
while C < 1.05:
    poly = PolynomialFeatures(degree=1)
    poly_train = poly.fit_transform(train)
    print(poly.get_feature_names())
    LR = LogisticRegression(penalty='l2', C=C, max_iter=10000, tol=0.0001)
    scores = cross_val_score(LR, poly_train, target, cv=10)
    # f1_score = cross_val_score(LR, poly_train, target, scoring='f1', cv=10)
    # print('C=' + str(C) + ' acc=' + str(scores.mean()) + ' std=' + str(scores.std()))
    Cs.append(C)
    acc_means.append(scores.mean())
    acc_stds.append(scores.std())
    # f1_means.append(f1_score.mean())
    # f1_stds.append(f1_score.std())
    C = C + 0.05

plt.figure()
fig, ax = plt.subplots(figsize=(12, 5))
plt.title('Logistic Regression error bar with C between 0.001 and 1')
plt.xlabel('C')
plt.ylabel('mean accuracy')
# trans1 = Affine2D().translate(-5, 0.0) + ax.transData
# trans2 = Affine2D().translate(0.0, 0.0) + ax.transData
plt.errorbar(Cs, acc_means, acc_stds, mfc='red', color="r",  label='accuracy', alpha=0.5)
# plt.errorbar(Cs, f1_means, f1_stds, mfc='blue', color="b", transform=trans2, label='f1_score', alpha=0.5)
plt.errorbar(Cs, acc_means, acc_stds, mfc='blue', color="b",
             label='The vertical line is the standard deviation', alpha=0.0)
plt.legend(loc=4)
plt.show()
