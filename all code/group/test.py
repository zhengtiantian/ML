import pandas as pd
data = pd.read_excel('NewAnalysis[1744].xlsx','7-15-pro(1)',index_col=0)
data.to_csv('data.csv')