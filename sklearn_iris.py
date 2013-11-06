from sklearn import datasets                   # data
from sklearn.naive_bayes import GaussianNB     # classifier object
import numpy as np                             # Python's matrix library

# iris dataset: 50 examples of setosa, versicolor, virginica in sequence
iris = datasets.load_iris()
data = iris.data
target = iris.target

# split off first 10 examples of each kind -> test set
train_i = np.array(range(10, 50) + range(60, 100) + range(140, 150))
test_i = np.array(range(10) + range(50, 60) + range(100, 110))
train_data, train_target = data[train_i], target[train_i]
test_data, test_target = data[test_i], target[test_i]

# learn on the train set, predict classes for the test set
gnb = GaussianNB()
gnb.fit(train_data, train_target)
pred = gnb.predict(test_data)
print("Number of mislabeled points : %d" % (test_target != pred).sum())

