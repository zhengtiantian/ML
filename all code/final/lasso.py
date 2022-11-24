import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
from sklearn.linear_model import Lasso
from sklearn.model_selection import cross_val_score

data = pd.read_csv("station21_11_p10_mean.csv")


X_train = data.iloc[:, 2:13]
y_train = data.iloc[:, 13]
X_train = np.array(X_train)
y_train = np.array(y_train)

alpha = 0.0001
alphas=[]
maes=[]
while alpha<1.005:
    model = Lasso(alpha=alpha)
    model.fit(X_train, y_train)
    scores = -cross_val_score(model, X_train, y_train, cv=5, scoring='neg_mean_absolute_error')
    print(scores.mean())
    alphas.append(alpha)
    maes.append(scores.mean())
    alpha = alpha+0.01
    print(alpha)


fig = plt.figure(figsize=(6, 4))
ax = fig.add_subplot()
ax.plot(alphas,maes,color='blue',linewidth=2.0)
ax.set_xlabel('alpha')
ax.set_ylabel('Mean Absolute Error')
plt.show()
