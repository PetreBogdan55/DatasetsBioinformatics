import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from tqdm import tqdm

# Set the backend for matplotlib to be interactive
import matplotlib
matplotlib.use('Qt5Agg')

# Load the data from the TSV file in chunks with a progress bar
file_path = "GSE138002_Final_barcodes.tsv"  # Replace with your file path

# Initialize the progress bar
# Reading the file in chunks
chunk_size = 10000  # Adjust chunk size based on your available memory
chunks = pd.read_csv(file_path, sep=';', quotechar='"', chunksize=chunk_size)

# Create an empty DataFrame to hold the combined data
df = pd.DataFrame()

# Iterate through the chunks and load the data
for chunk in tqdm(chunks, desc="Loading data", unit="chunk"):
    df = pd.concat([df, chunk], ignore_index=True)

# Strip any leading/trailing spaces from column names
df.columns = df.columns.str.strip()

# Check the first few rows of the dataframe to ensure it's loaded correctly
print(df.head())

# Set up a Seaborn style for the plot
sns.set(style="whitegrid")

# Create a scatter plot using the UMAP coordinates and color by 'umap2_CellType'
plt.figure(figsize=(10, 8))

df_sample = df.sample(n=10000)  # Example: Use a subset of 10,000 rows

# Create the scatter plot manually using Matplotlib
plt.figure(figsize=(10, 8))
scatter = plt.scatter(
    x=df_sample['umap1_coord'],
    y=df_sample['umap2_coord'],
    c=df_sample['umap2_CellType'].astype('category').cat.codes,  # Use numeric codes for hue
    cmap='Set1',  # Specify the colormap
    s=100,
    edgecolor='w',
    alpha=0.7
)

# Add color bar
plt.colorbar(scatter, label='Cell Type')

# Customize the plot
plt.title('UMAP Clusters by Cell Type', fontsize=16)
plt.xlabel('UMAP 1', fontsize=14)
plt.ylabel('UMAP 2', fontsize=14)

# Display the plot
plt.show()

