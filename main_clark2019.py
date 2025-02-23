import matplotlib
matplotlib.use('Qt5Agg')

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Load the data from the tsv file
file_path = "GSE118614_Smart_cells.tsv"  # Replace with your file path

df = pd.read_csv(file_path, sep='\t')

# Check the first few rows of the dataframe to ensure it's loaded correctly
print(df.head())

# Get the number of unique clusters
num_clusters = df['CellType'].nunique()

# Generate a unique color palette with enough colors
# You can use 'husl' or any palette that gives a large number of distinct colors
palette = sns.color_palette("husl", num_clusters)  # Use a distinct color palette with enough colors

# Create a scatter plot using the tSNE coordinates and color by 'raw_cluster'
plt.figure(figsize=(10, 8))
sns.scatterplot(x='tSNE1_pos', y='tSNE2_pos', hue='CellType', data=df, palette=palette, s=100, edgecolor='w', alpha=0.7)

# Customize the plot
plt.title('tSNE Clusters', fontsize=16)
plt.xlabel('tSNE 1', fontsize=14)
plt.ylabel('tSNE 2', fontsize=14)

# Add a legend to indicate the clusters
plt.legend(title='Cluster', loc='upper right', fontsize=12)


# Display the plot
plt.show()
