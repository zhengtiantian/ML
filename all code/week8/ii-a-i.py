from keras.losses import mean_squared_error
from sklearn.dummy import DummyRegressor
from sklearn.metrics import accuracy_score, classification_report
from tensorflow import keras

num_classes = 10
input_shape = (32, 32, 3)

(x_train, y_train), (x_test, y_test) = keras.datasets.cifar10.load_data()
n = 5000
x_train = x_train[1:n]
y_train = y_train[1:n]

x_train = x_train.astype("float32") / 255
x_test = x_test.astype("float32") / 255

# y_train = keras.utils.to_categorical(y_train, num_classes)
# y_test = keras.utils.to_categorical(y_test, num_classes)

dummy = DummyRegressor(strategy='median').fit(x_train, y_train)
ydummy = dummy.predict(x_test)
print(classification_report(y_test, ydummy))