### Task 2

Please construct a classifier based on logistic regression for data related to the quality of radar signal reflected by the ionosphere, where the quality of the reflected signal is described by a nominal attribute with two possible values: 'b' (bad) and 'g' (good). The model should predict the quality of the radar signal reflection based on the values of all 34 continuous attributes.

A detailed description of the data is available on the website: [ionosphere dataset](https://archive.ics.uci.edu/ml/datasets/ionosphere), and the data files can be accessed at [ionosphere.data](https://archive.ics.uci.edu/ml/machine-learning-databases/ionosphere/ionosphere.data). The specific information about this dataset is stored in the file [ionosphere.names](https://archive.ics.uci.edu/ml/machine-learning-databases/ionosphere/ionosphere.names).

To construct the classifier, use the training set, and to evaluate its performance, use the test set. These sets should be created by dividing the data into parts containing 80% and 20% of all examples, respectively. These parts should have the same ratio of the number of observations from class 'b' to the number of observations from class 'g' as the full dataset.

The solution should be a .ipynb file with Python code that loads the data, trains the described classifier, and evaluates its predictive capabilities. The evaluation of the classifier's predictive capabilities should include a confusion matrix, accuracy score, ROC curve, and the area under the ROC curve (AUC) calculated based on the test data.

##### Additional guidelines:

1) You can load the data from the file using [numpy.genfromtxt](https://numpy.org/doc/stable/reference/generated/numpy.genfromtxt.html).

2) Replace the classes 'b' (bad) and 'g' (good) with numerical values, 0 and 1, respectively.