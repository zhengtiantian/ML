import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LogisticRegression as LR
from sklearn.model_selection import train_test_split
from sklearn.metrics import roc_curve, classification_report, roc_auc_score

# Read data1
dataText = open("data")
dataArr = str(dataText.read()).split('\n')

# Divide the data1 into x, y arrays of 1 and xy arrays of -1 according to -1 and 1
px, py, nx, ny = [], [], [], []
data, target = [], []
for d in dataArr[1:]:
    nums = d.split(',')
    dxy = []
    dxy.append(float(nums[0]))
    dxy.append(float(nums[1]))
    data.append(dxy)
    target.append(float(nums[2]))
    if nums[2] == '1':
        px.append(float(nums[0]))
        py.append(float(nums[1]))
    else:
        nx.append(float(nums[0]))
        ny.append(float(nums[1]))

# Divide the data1 set into a training set and a test set
X_train, X_test, y_train, y_test = train_test_split(data, target)
print(X_train)
print(y_train)
# Training model
lr = LR(penalty='l2', max_iter=1000, tol=0.0001)
lr.fit(X_train, y_train)

# Prediction data1 set
y_pred = lr.predict(data)
y_test_pred = lr.predict(X_test)

# Print classification report
print(classification_report(y_test, y_test_pred))
fig, square = plt.subplots(figsize=(7, 7))

# Get accuracy and other data1
y_prob = lr.predict_proba(X_test)[:, 1]
logit_roc_auc = roc_auc_score(y_test, lr.predict(X_test))
fpr_lr, tpr_lr, threshold_lr = roc_curve(y_test, y_prob)

# Draw the ROC curve graph
plt.figure()
plt.plot(fpr_lr, tpr_lr, label='Logistic Regression (area = %0.2f)' % logit_roc_auc)
plt.plot([0, 1], [0, 1], 'r--')
plt.xlim([0.0, 1.0])
plt.ylim([0.0, 1.05])
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Receiver operating characteristic')
plt.legend(loc="lower right")
plt.show()

# # Draw a dot plot
# y_pred = list(y_pred)
# ppx, ppy, pnx, pny = [], [], [], []
# for i, v in enumerate(data):
#     if y_pred[i] == 1:
#         ppx.append(v[0])
#         ppy.append(v[1])
#     else:
#         pnx.append(v[0])
#         pny.append(v[1])
# square.scatter(px, py, s=30, c='r', marker='o', label='1')
# square.scatter(nx, ny, s=30, c='b', marker='x', label='-1')
# square.scatter(ppx, ppy, s=10, c='g', marker='o', label='pre 1')
# square.scatter(pnx, pny, s=10, c='y', marker='x', label='pre -1')
#
# # Draw the decision boundary
# x1_plot = np.linspace(-1, 1, 100)
# x2_plot = (-lr.coef_[0][0] * x1_plot - lr.intercept_[0]) / lr.coef_[0][1]
# plt.plot(x1_plot, x2_plot)
#
# square.legend()
# plt.title("Logistic Regression")
# square.set_xlabel("x-axis")
# square.set_ylabel("y-axis")
# plt.show()
