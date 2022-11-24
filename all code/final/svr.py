import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
from sklearn.model_selection import cross_val_score
from sklearn.svm import SVR

data = pd.read_csv("station21_12_p10.csv")

X_train = data.iloc[:, 2:14]
y_train = data.iloc[:, 14]
X_train = np.array(X_train)
y_train = np.array(y_train)

C = 0.00001
Cs=[]
maes=[]
for i in range(30):
    svr = SVR(C=C)
    print('start'+str(i+1))
    # svr.fit(X_train, y_train)
    scores = -cross_val_score(svr, X_train, y_train, cv=5, scoring='neg_mean_absolute_error')
    print('finish' + str(i + 1))
    print(scores.mean())
    print(C)
    Cs.append(C)
    maes.append(scores.mean())

    C = C + 0.00001



fig = plt.figure(figsize=(6, 4))
ax = fig.add_subplot()
ax.plot(Cs,maes,color='blue',linewidth=2.0)
ax.set_xlabel('alpha')
ax.set_ylabel('Mean Absolute Error')
plt.show()