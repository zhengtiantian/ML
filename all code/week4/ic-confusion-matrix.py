import numpy as np
from sklearn.dummy import DummyClassifier
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import confusion_matrix
from sklearn.model_selection import train_test_split
from sklearn.neighbors import KNeighborsClassifier
from sklearn.preprocessing import PolynomialFeatures

data = str(open("data2").read()).split('\n')
dataset = []
px, py, nx, ny = [], [], [], []
for d in data[1:]:
    nums = d.split(',')
    dxy = []
    dxy.append(float(nums[0]))
    dxy.append(float(nums[1]))
    dxy.append(float(nums[2]))
    dataset.append(dxy)
    if nums[2] == '1':
        px.append(float(nums[0]))
        py.append(float(nums[1]))
    else:
        nx.append(float(nums[0]))
        ny.append(float(nums[1]))
dataset = np.array(dataset)

train = dataset.take([0, 1], axis=1)
target = dataset[:, 2]

X_train, X_test, y_train, y_test = train_test_split(train, target)
knn = KNeighborsClassifier(n_neighbors=13)
knn.fit(X_train, y_train)
pred = knn.predict(X_test)
knn_m = confusion_matrix(y_test, pred)
print('confusion matrix：', knn_m, sep='\n')

poly_features = PolynomialFeatures(degree=1)
poly_train = poly_features.fit_transform(X_train)
LR = LogisticRegression(penalty='l2', C=1)
LR.fit(poly_train, y_train)
poly_test = poly_features.fit_transform(X_test)
poly_pred = LR.predict(poly_test)
lr_m = confusion_matrix(y_test, poly_pred)
print('confusion matrix：', lr_m, sep='\n')

dummy = DummyClassifier(strategy="uniform").fit(X_train, y_train)
dummy_pred = dummy.predict(X_test)
lr_m = confusion_matrix(y_test, dummy_pred)
print('confusion matrix：', lr_m, sep='\n')

