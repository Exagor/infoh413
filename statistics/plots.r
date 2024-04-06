# Load required packages
library(ggplot2)

# Read the CSV file
data <- read.csv("raw_results/results_best.csv")

# Calculate the relative deviation
results_ex_rd <- subset(data, permutation == "exchange" & initial == "random")

# Plot the distribution of the relative deviation
ggplot(results_ex_rd, aes(x = relative_deviation)) +
    geom_histogram(binwidth = 0.2, fill = "steelblue", color = "#000000") +
    labs(title = "Distribution of Relative Deviation",
             x = "Relative Deviation",
             y = "Frequency")