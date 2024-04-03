results <- read.csv("results/results_best.csv", header = TRUE)

# Aggregate by permutation and calculate mean cost and time for size = 250
permutation_aggregate_250 <- aggregate(cbind(relative_deviation, time) ~ permutation, data = subset(results, size == 250), FUN = mean)
print("Permutation aggregate for size = 250")
permutation_aggregate_250

# Aggregate by initial and calculate mean cost and time for size = 250
initial_aggregate_250 <- aggregate(cbind(relative_deviation, time) ~ initial, data = subset(results, size == 250), FUN = mean)
initial_aggregate_250

# Aggregate by permutation and calculate mean cost and time for size = 150
permutation_aggregate_150 <- aggregate(cbind(relative_deviation, time) ~ permutation, data = subset(results, size == 150), FUN = mean)
print("Permutation aggregate for size = 250")
permutation_aggregate_150

# Aggregate by initial and calculate mean cost and time for size = 150
initial_aggregate_150 <- aggregate(cbind(relative_deviation, time) ~ initial, data = subset(results, size == 150), FUN = mean)
initial_aggregate_150
