import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import PolynomialFeatures
from sklearn.linear_model import Lasso
from sklearn.dummy import DummyRegressor
from sklearn.metrics import mean_squared_error

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

x = dataset[:, 0]
y = dataset[:, 1]
z = dataset[:, 2]
X_train = dataset.take([0, 1], axis=1)
y_train = dataset[:, 2]

times = 100
modelSq, dummySq = 0, 0
for i in range(0,times):
    Xtrain, Xtest, ytrain, ytest = train_test_split(X_train, y_train, test_size=0.2)
    Xtrain_poly = PolynomialFeatures(degree=5).fit_transform(Xtrain)
    Xtest_poly = PolynomialFeatures(degree=5).fit_transform(Xtest)
    lassoModel = Lasso(alpha=0.01).fit(Xtrain_poly, ytrain)
    dummy = DummyRegressor(strategy='mean').fit(Xtrain_poly, ytrain)
    ypred = lassoModel.predict(Xtest_poly)
    ydummy = dummy.predict(Xtest_poly)
    modelSq1 = mean_squared_error(ytest, ypred)
    dummySq1 = mean_squared_error(ytest, ydummy)
    print(modelSq1)
    print(dummySq1)
    modelSq = modelSq+modelSq1
    dummySq = dummySq+dummySq1

print('square error: Lasso regression with alpha=10:'+str(modelSq/times)+' dummy:'+str(dummySq/times))
