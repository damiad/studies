### Task 3

Please construct a classifier based on a deep neural network for the CIFAR-10 dataset.

To construct the classifier, use the CIFAR-10 training dataset, and use the CIFAR-10 test dataset as a validation set to evaluate the accuracy of the current model after each epoch of the training procedure.

The solution should be a .ipynb file with Python code that loads the data, performs any necessary data transformations, trains the described classifier, and generates accuracy plots for both the training and validation data after each training epoch. The training procedure should utilize "patient early stopping" and save the parameters of the best model to a file. After training, the saved model should be loaded from the file and applied to the validation data to calculate and display the achieved accuracy for that data.

The trained model should achieve an accuracy of at least 70% on the validation data.

***Tips:***
- Consider the appropriate "input shape" for the model.
- The model should be a deep neural network, but its architecture is left to your discretion.