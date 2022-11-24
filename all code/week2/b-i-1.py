import numpy as np
import matplotlib.pyplot as plt
from sklearn.metrics import classification_report
from sklearn.svm import LinearSVC
from sklearn.model_selection import train_test_split

# LinearSVC related classes
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

# Training model
LinearSVC = LinearSVC(penalty='l2', loss='squared_hinge', dual=True, C=100, fit_intercept=True, max_iter=1000)
LinearSVC.fit(X_train, y_train)

# Prediction data1 set
y_pred = LinearSVC.predict(data)
y_pred = list(y_pred)
y_test_pred = LinearSVC.predict(X_test)

# Print classification report
print(classification_report(y_test, y_test_pred))

# Draw a dot plot
ppx, ppy, pnx, pny = [], [], [], []
for i, v in enumerate(data):
    if y_pred[i] == 1:
        ppx.append(v[0])
        ppy.append(v[1])
    else:
        pnx.append(v[0])
        pny.append(v[1])

fig, square = plt.subplots(figsize=(7, 7))
square.scatter(px, py, s=30, c='r', marker='o', label='1')
square.scatter(nx, ny, s=30, c='b', marker='x', label='-1')
square.scatter(ppx, ppy, s=10, c='g', marker='o', label='pre 1')
square.scatter(pnx, pny, s=10, c='y', marker='x', label='pre -1')

# Draw the decision boundary
x1_plot = np.linspace(-1, 1, 200)
x2_plot = (-LinearSVC.coef_[0][0] * x1_plot - LinearSVC.intercept_[0]) / LinearSVC.coef_[0][1]
plt.plot(x1_plot, x2_plot)

# Draw the soft margin and hardmargin of LinearSVC
# w0*x0 + w1*x1 + b = 1
h_up_y = -LinearSVC.coef_[0][0] / LinearSVC.coef_[0][1] * x1_plot - LinearSVC.intercept_[0] / LinearSVC.coef_[0][
    1] + 1 / LinearSVC.coef_[0][1]
# w0*x0 + w1*x1 + b = -1
h_down_y = -LinearSVC.coef_[0][0] / LinearSVC.coef_[0][1] * x1_plot - LinearSVC.intercept_[0] / LinearSVC.coef_[0][
    1] - 1 / LinearSVC.coef_[0][1]
up_index = (h_up_y >= -1) & (h_up_y <= 1)
down_index = (h_down_y >= -1) & (h_down_y <= 1)
plt.plot(x1_plot[up_index], h_up_y[up_index], color='black')
plt.plot(x1_plot[down_index], h_down_y[down_index], color='black')
# w0*x0 + w1*x1 + b = 1-
h_up_y = -LinearSVC.coef_[0][0] / LinearSVC.coef_[0][1] * x1_plot - LinearSVC.intercept_[0] / LinearSVC.coef_[0][
    1] + 0.5 / LinearSVC.coef_[0][1]
# w0*x0 + w1*x1 + b = -1
h_down_y = -LinearSVC.coef_[0][0] / LinearSVC.coef_[0][1] * x1_plot - LinearSVC.intercept_[0] / LinearSVC.coef_[0][
    1] - 0.5 / LinearSVC.coef_[0][1]
up_index = (h_up_y >= -1) & (h_up_y <= 1)
down_index = (h_down_y >= -1) & (h_down_y <= 1)
plt.plot(x1_plot[up_index], h_up_y[up_index], color='black', linestyle='--')
plt.plot(x1_plot[down_index], h_down_y[down_index], color='black', linestyle='--')

square.legend()
plt.title("LinearSVC with C=0.001")
square.set_xlabel("x-axis")
square.set_ylabel("y-axis")
plt.show()
