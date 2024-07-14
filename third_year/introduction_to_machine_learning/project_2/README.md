# Introduction to Machine Learning 2024 Project II

### Data Description

Organs like the pancreas consist of various types of tissues, which in turn consist of various cell types. Within the pancreas, we can distinguish cells specific to this organ, such as alpha and beta cells, as well as cells associated with blood supply or the immune system.

The data in this task come from multimodal single-cell RNA sequencing (scRNA-seq). Using scRNA-seq allows studying samples at high resolution and separating different types of cells. It is possible, among other things, to compare pathological cells from cancer patients with healthy cells. In multimodal scRNA-seq technology, we get two types of readings for each cell:
- RNA transcript counts corresponding to gene expression (activity) in a given cell;
- The amount of surface proteins, which is directly related to the type of cell.

The result of the scRNA-seq experiment is matrices where each cell is assigned an RNA signal from thousands of genes (in our task X) and a signal from several dozen surface proteins (in our task, we chose a single protein CD361 for simplicity, y).

According to the central dogma of biology, we know that genetic information flows from RNA to proteins. Thus, we should expect a correlation between the amount of protein and the expression of the gene that encodes that protein. For technical and biological reasons, this relationship often degenerates. The problem in this task is to predict the surface protein signal based on gene expression. Predicting protein abundance is crucial for most publicly available datasets for which only the RNA matrix is available. Analyzing gene expression and protein abundance signals significantly facilitates the process of identifying and naming cells in a sample.

The data were collected from the bone marrow of human donors. The collected cells are mostly immune system cells. Proper identification of T lymphocytes based on both types of readings in such a dataset could be the basis for developing targeted cancer therapies (for the curious: CAR T cell therapy).

### Data Retrieval

On the course's Moodle page, there is a link to a folder with data for each lab group. Since each group works on data from a different experiment, results may vary between groups. The data is compressed and saved in .csv format. Three files will be provided:
- `X_train.csv` and `X_test.csv`, containing RNA matrices. Each row corresponds to a cell, the column to a gene, and the values are expression levels. The columns of these matrices are our explanatory variables.
- `y_train.csv`, corresponding to the amount of a certain type of surface protein in the cells (those covered by the data in `X_train.csv`). This is our dependent variable.

In the rest of the description, data from the `X_train.csv` and `y_train.csv` files will be called training data, and data from the `X_test.csv` file will be called test data.

### Submission Instructions

On the course Moodle page, in the section dedicated to Task 2 solutions, you should submit the following files:
- A report in a Jupyter notebook (.ipynb), implementing the described tasks (file name template: `[StudentID] report.ipynb`, e.g., `123456 report.ipynb`).
- The report should be structured in a way that naturally guides the reader through the solution of the individual tasks.
- Predictions on the test data (see task 4) in the form of a .csv file, containing a column `Id` with observation numbers and a column `Expected` with predicted values (file name template: `[StudentID] prediction.csv`, e.g., `123456 prediction.csv`).

Make sure to double-check that the names of the submitted files comply with the required templates and that the .csv file is correctly prepared (two appropriately named columns, the correct number of rows corresponding to the test set).

### Grading

The entire project is worth 30 points. The maximum number of points for each of the following subpoints is given in parentheses. The evaluation of tasks 1 to 4 will consider:
- The implementation of the presented instructions,
- The quality of the report, i.e., logical structure, visualizations, readability of the text, description of results, and explanations of the undertaken actions,
- The quality of the used code. It should be readable and reproducible.

Additional details about the grading criteria can be obtained from your lab instructor.

**Note:** Tasks submitted after the deadline will not be graded and will receive 0 points.

## Task Descriptions

### 1. Exploration (7 points)
(a) Check how many observations and variables are in the training and test data. Examine the types of variables and, if deemed appropriate, make the necessary conversions before further analysis. Ensure the data is complete.
(b) Examine the empirical distribution of the dependent variable (present some basic statistics, include a histogram or density estimator plot in the analysis).
(c) Select the 250 most correlated explanatory variables with the dependent variable. Calculate the correlation for each pair of these variables. Illustrate the result with a heatmap.

**Note:** The described variable selection is only for this subpoint; the analysis described in the following tasks should be conducted on the full training dataset.

### 2. ElasticNet (7 points)
The first model to train is ElasticNet, which has special cases of ridge regression and lasso.
(a) In the report, provide information about the ElasticNet model, explaining the parameters it estimates, the optimized function, and the hyperparameters it depends on. For which values of hyperparameters do we get ridge regression, and for which lasso?
(b) Define a grid of hyperparameters based on at least three values of each hyperparameter. Ensure the grid includes configurations corresponding to ridge regression and lasso. Use cross-validation to select the appropriate hyperparameters (the number of subsets used in cross-validation should be decided and justified by the student).
(c) Provide the training and validation error of the model (the result should be averaged over all subsets distinguished in cross-validation).

### 3. Random Forests (8 points)
In this part of the project, a Random Forest model should be trained and compared with the previously created ElasticNet model.
(a) Among the many hyperparameters characterizing the Random Forest model, choose three different ones. Define a three-dimensional grid of searched hyperparameter combinations and use cross-validation to select their optimal values (in the context of the prediction). The data partition used in cross-validation should be the same as for ElasticNet.
(b) Provide a tabular summary of the results obtained by the methods in cross-validation in both considered models. (This comparison is the reason we want to use the same partitions). Determine which model seems best to you (justify your choice). Include a basic reference model that assigns the arithmetic mean of the dependent variable to any values of the explanatory variables.

### 4. Prediction on the Test Set (8 points)
This part of the project is open-ended. Based on the training data, fit a model of your choice, and then use it to predict the dependent variable values in the test set. The choice and construction of the model, as well as the motivations behind this choice, should be described in the report. The generated predictions should be sent to the instructor in a separate file, formatted as described earlier. The number of points obtained will depend on the prediction quality, measured by the Root Mean Squared Error (RMSE).

Scoring Details:
- (1 point) – for an error lower than the one from the described basic reference model.
- (2 points) – for an error lower than the ElasticNet model trained by the lab instructors.
- (5 points) – this bonus is calculated according to the formula \(12⌊10F_b(e)^3⌋\), where \(e\) is the student's test error prediction, \(F_b\) is the empirical cumulative distribution function of all submitted prediction errors in the student's lab group, and \(⌊·⌋\) is the floor function.