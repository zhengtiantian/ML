import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
from sklearn.metrics import mean_absolute_error
from sklearn.model_selection import KFold
from sklearn.neighbors import KNeighborsRegressor

data = pd.read_csv("7k_data.csv")

X_train = data.iloc[:, 0:26]
y_train = data.iloc[:, 26]
X_train = np.array(X_train)
y_train = np.array(y_train)


k = 1

ks = []
means = []
while k < 70:
    kf = KFold(n_splits=5, shuffle=True)
    ks.append(k)
    i = 1
    model = KNeighborsRegressor(n_neighbors=k)
    sum = 0
    for train_index, test_index in kf.split(X_train):
        X_fold = X_train[train_index]
        y_fold = y_train[train_index]
        model.fit(X_fold, y_fold)
        # print(np.sum(np.diff(X_fold)) / len(X_fold))
        y_test_fold = X_train[test_index]
        # print(np.sum(np.diff(y_test_fold)) / len(y_test_fold))
        y_pre = model.predict(X_train[test_index])
        # print("C=" + str(C) + ",K=" + str(i) + ',' + str(mean_absolute_error(y_train[test_index], y_pre)))
        sum = sum + mean_absolute_error(y_train[test_index], y_pre)
        i = i + 1
    print("k=" + str(k) + "mean:" + str(sum / 5))
    means.append(sum/5)
    k = k + 1


fig = plt.figure(figsize=(6, 4))
ax = fig.add_subplot()
ax.plot(ks,means,color='blue',linewidth=2.0)
ax.set_xlabel('k')
ax.set_ylabel('Mean Absolute Error')
plt.show()

