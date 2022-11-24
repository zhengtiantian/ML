import math
import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LogisticRegression as LR
from sklearn.metrics import classification_report, roc_auc_score, roc_curve
from sklearn.model_selection import train_test_split

# Logistic regression 4 features
# Read data1
dataText = open("newdata.txt")
dataArr = str(dataText.read()).split('\n')

# Divide the data1 into x, y arrays of 1 and xy arrays of -1 according to -1 and 1
px, py, nx, ny = [], [], [], []
data, target = [], []
for d in dataArr:
    nums = d.split(',')
    print(nums)
    dxy = []
    dxy.append(float(nums[0]))
    dxy.append(float(nums[1]))
    dxy.append(float(nums[2]))
    dxy.append(float(nums[3]))
    data.append(dxy)
    target.append(float(nums[4]))
    if nums[4] == '1':
        px.append(float(nums[0]))
        py.append(float(nums[1]))
    else:
        nx.append(float(nums[0]))
        ny.append(float(nums[1]))

# Divide the data1 set into a training set and a test set
X_train, X_test, y_train, y_test = train_test_split(data, target)

# Training model
lr = LR(penalty='l2', max_iter=1000, tol=0.0001)
lr.fit(X_train, y_train)

# Prediction data1 set
y_pred = lr.predict(data)

# Print classification report
print(classification_report(y_test, lr.predict(X_test)))
fig, square = plt.subplots(figsize=(7, 7))

# Get accuracy and other data1
y_prob = lr.predict_proba(X_test)[:,1]
logit_roc_auc = roc_auc_score(y_test, lr.predict(X_test))
fpr_lr,tpr_lr,threshold_lr = roc_curve(y_test,y_prob)

# Draw the ROC curve graph
plt.figure()
plt.plot(fpr_lr, tpr_lr, label='Logistic Regression with 4 features(area = %0.2f)' % logit_roc_auc)
plt.plot([0, 1], [0, 1],'r--')
plt.xlim([0.0, 1.0])
plt.ylim([0.0, 1.05])
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Receiver operating characteristic')
plt.legend(loc="lower right")
plt.show()

# Draw a dot plot
y_pred = list(y_pred)
ppx, ppy, pnx, pny = [], [], [], []
for i, v in enumerate(data):
    if y_pred[i] == 1:
        ppx.append(v[0])
        ppy.append(v[1])
    else:
        pnx.append(v[0])
        pny.append(v[1])
square.scatter(px, py, s=30, c='r', marker='o', label='1')
square.scatter(nx, ny, s=30, c='b', marker='x', label='-1')
square.scatter(ppx, ppy, s=10, c='g', marker='o', label='pre 1')
square.scatter(pnx, pny, s=10, c='y', marker='x', label='pre -1')

# Draw the decision boundary
x1_plot = np.linspace(-1, 1, 200)
x2_plot=[]
for i,v in enumerate(x1_plot):
    a = (-lr.coef_[0][0]*v-lr.coef_[0][2]*math.pow(v,2)) / lr.coef_[0][1]
    x2_plot.append(a)
x2_plot = np.array(x2_plot)
plt.plot(x1_plot, x2_plot)

square.legend()
plt.title("Logistic Regression with 4 features")
square.set_xlabel("x-axis")
square.set_ylabel("y-axis")
plt.show()