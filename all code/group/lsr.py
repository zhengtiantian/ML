import numpy as np
import pandas as pd
from sklearn.linear_model import Lasso
from sklearn.metrics import mean_absolute_error
from sklearn.model_selection import train_test_split, KFold, cross_val_score
from sklearn.svm import SVR

data = pd.read_csv("7k_data.csv")

X_train = data.iloc[:, 0:26]
y_train = data.iloc[:, 26]
X_train = np.array(X_train)
y_train = np.array(y_train)

# Xtrain, Xtest, ytrain, ytest = train_test_split(X_train, y_train, test_size=0.2)
alpha = 0.00001

while alpha < 1001:
    model = Lasso(alpha=alpha)
    scores = cross_val_score(model, X_train, y_train, scoring="neg_mean_absolute_error")
    print('alpha=' + str(alpha) + str(scores.mean()))
    # print("C=" + str(C) + ",K=" + str(i) + ',' + str(mean_absolute_error(y_train[test_index], y_pre)))
    alpha = alpha * 10

# model.fit(Xtrain, ytrain)
# y_pre = model.predict(Xtest)
# print(mean_absolute_error(ytest, y_pre))
