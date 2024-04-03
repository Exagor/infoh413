# Read values from the file "best_known.txt"
best <- read.table("best_known/best_known.txt", header = FALSE, col.names = c("instance", "cost"))

# Read values from the file "results_ii.csv"
results <- read.csv("results/results_ii.csv", header = TRUE)

# Calculate the relative percentage deviation
common_indices <- match(results$instance, best$instance)
best_costs <- best$cost[common_indices] # Extract the best costs a number of times equal to the number of results
results_costs <- results$cost # Extract the results costs
# Calculate the relative deviation
results$relative_deviation <- ((best_costs - results_costs) / best_costs) * 100

# Print the updated data
print(results)