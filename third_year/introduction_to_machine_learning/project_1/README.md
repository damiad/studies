# Introduction to Machine Learning 2024 Project I

The goal of the task is to perform a statistical analysis of the data located in the file `dane_projekt1.csv`.

**Data:**
These are simulated data describing a segment of survey results on consumer habits among residents of the fictional region of Bajtocja. The survey was conducted on a representative sample. The data may include random errors. Below is an explanation of the variable names used in the study:

- `id` – observation identifier, does not contain additional information
- `waga` – respondent's weight (in kg)
- `wzrost` – respondent's height (in cm)
- `plec` – respondent's gender as per identity document (1 – "female", 2 – "male")
- `dzieci` – number of children under the care of the respondent (in persons)
- `wiek` – respondent's age (in years)
- `dochod` – respondent's declared income in the surveyed month (in bajtalars)
- `oszczednosci` – respondent's declared savings in the surveyed month (in bajtalars, negative values indicate total expenditures exceeded income)
- `jednoos` – household status (1 – "single-person household", 0 – "multi-person household")
- `miejsce` – size of the locality where the respondent lives (1 – "up to 10,000 inhabitants", 2 – "10,000 to 100,000 inhabitants", 3 – "over 100,000 inhabitants")
- `wydatki_zyw` – respondent's declared expenses on food in the surveyed month (in bajtalars)

**Result:**
The outcome should be a report in a Jupyter notebook (.ipynb). The report and comments must be sufficient to understand and reproduce the steps taken without the need to read your code. Any significant modifications to the dataset (e.g., deleting records, modifying and introducing new variables) must be justified and described.

**Task 1: Load Data, Examine, and Summarize**

- Number of observations, discussion on data structure: quantitative vs. qualitative variables, presence of missing data.
- Presentation and commentary on valid frequency tables or descriptive statistics for variables in the dataset.
- Presentation and commentary on variable distributions, especially visually comparing them with the normal distribution using histograms, quantile-quantile plots, etc.

**Task 2: Explore Relationships**

- Check for dependencies between quantitative variables, calculate and present a heatmap of valid correlation coefficients.
- Examine the relationship between qualitative variables, commenting on statistical significance.

**Task 3: Summarize Data with Plots**

- Scatter plots for all quantitative variables against `wydatki_zyw`.
- Box plot for a selected quantitative variable, segmented by respondents' place of residence.
- Stacked bar chart for respondent gender and whether they lead a single-person household.

**Task 4: Confidence Intervals**

- Calculate two-sided confidence intervals at a confidence level of 99% for `wiek` (age) for mean, standard deviation, and quartiles.
- Provide assumptions used and comment on their validity.

**Task 5: Socioeconomic Classes**

- Discuss and compare food expenditure diversity across socioeconomic classes.

**Task 6: Statistical Tests**

- Perform statistical tests to answer specified research questions.
- Verify an additional hypothesis regarding conformity to a specific parametric distribution.

**Task 7: Regression Analysis**

- Estimate an initial model with all variables from the original dataset (excluding `id`), interpret R2 and significance tests.
- Check if the initial model meets assumptions of Classical Linear Regression Model (CLRM).
- Check for multicollinearity and outliers in the initial model.
- Improve the model to meet CLRM assumptions and interpret significant coefficients.
