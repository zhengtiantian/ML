import numpy as np
from sklearn.model_selection import cross_val_score
from sklearn.neighbors import KNeighborsClassifier
import matplotlib.pyplot as plt

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

Ks, acc_means, acc_stds, f1_means, f1_stds = [], [], [], [], []
for K in range(1,30):
    knn = KNeighborsClassifier(n_neighbors=K)
    scores = cross_val_score(knn, train, target, cv=10)
    Ks.append(K)
    acc_means.append(scores.mean())
    acc_stds.append(scores.std())

plt.figure()
fig, ax = plt.subplots(figsize=(12, 5))
ax.errorbar(Ks, acc_means, acc_stds, mfc='red', color="r", label='accuracy', alpha=0.5)
plt.errorbar(Ks, acc_means, acc_stds, mfc='blue', color="b",
             label='The vertical line is the standard deviation', alpha=0.0)
plt.title('KNN error bar with different K' )
plt.xlabel('K')
plt.ylabel('mean accuracy')
plt.legend(loc=4)
plt.show()