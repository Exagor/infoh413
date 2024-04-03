results <- read.csv("results/results_complete.csv", header = TRUE)

# Aggregate by permutation and calculate mean cost and time
permutation_aggregate <- aggregate(cbind(cost, time) ~ permutation, data = results, FUN = mean)
permutation_aggregate

# Aggregate by initial and calculate mean cost and time
initial_aggregate <- aggregate(cbind(cost, time) ~ initial, data = results, FUN = mean)
initial_aggregate
