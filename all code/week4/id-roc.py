import numpy as np
from sklearn.dummy import DummyClassifier
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import confusion_matrix, roc_auc_score, roc_curve
from sklearn.model_selection import train_test_split
from sklearn.neighbors import KNeighborsClassifier
from sklearn.preprocessing import PolynomialFeatures
import matplotlib.pyplot as plt

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

poly_features = PolynomialFeatures(degree=1)
poly_train = poly_features.fit_transform(X_train)
LR = LogisticRegression(penalty='l2', C=1)
LR.fit(poly_train, y_train)

knn = KNeighborsClassifier(n_neighbors=13)
knn.fit(X_train, y_train)

dummy = DummyClassifier(strategy="uniform").fit(X_train, y_train)
# dummy_pred = dummy.predict(X_test)


logit_roc_auc = roc_auc_score(y_test, LR.predict(poly_features.fit_transform(X_test)))
y_prob = LR.predict_proba(poly_features.fit_transform(X_test))[:, 1]
fpr_lr, tpr_lr, threshold_lr = roc_curve(y_test, y_prob)


dummy_roc_auc = roc_auc_score(y_test, dummy.predict(X_test))
y_prob_dummy = dummy.predict_proba(X_test)[:, 1]
fpr_lr_dummy, tpr_lr_dummy, threshold_lr_dummy = roc_curve(y_test, y_prob_dummy)


knn_roc_auc = roc_auc_score(y_test, knn.predict(X_test))
y_prob_knn = knn.predict_proba(X_test)[:, 1]
print(y_prob_knn)
fpr_lr_knn, tpr_lr_knn, threshold_lr_knn = roc_curve(y_test, y_prob_knn)

plt.figure()
fig,ax = plt.subplots(figsize=(7,7))
ax.plot(fpr_lr, tpr_lr, label='Logistic Regression (area = %0.2f)' % logit_roc_auc)
ax.plot(fpr_lr_dummy, tpr_lr_dummy, label='Dummy Classifier (area = %0.2f)' % dummy_roc_auc)
ax.plot(fpr_lr_knn, tpr_lr_knn, label='knn (area = %0.2f)' % knn_roc_auc)
# ax.plot([0, 1], [0, 1], 'r--')
plt.xlim([0.0, 1.0])
plt.ylim([0.0, 1.05])
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Receiver operating characteristic')
plt.legend(loc="lower right")
plt.show()