#Script to make general statistics about raw data
results <- read.csv("raw_results/results_ils.csv", header = TRUE)
algo <- "ILS"

#Create new table
stat_results <- data.frame(
    "algorithm" = character(), 
    "average relative deviation" = numeric(), 
    "average time" = numeric(), 
    "size" = numeric(), 
    "average generation" = numeric()
)

aggregate_results <- aggregate(
    cbind(relative_deviation, time, iterations) ~ size, 
    data = results, 
    FUN = mean
)
aggregate_results$algorithm <- algo #insert name of the algorithm
stat_results <- rbind(stat_results, aggregate_results) #insert the row in the table

names(stat_results)[names(stat_results) == "relative_deviation"] <- "average_relative_deviation"
names(stat_results)[names(stat_results) == "time"] <- "average_time"
names(stat_results)[names(stat_results) == "iterations"] <- "average_iterations"

column_order <- c(
    "algorithm", "size", "average_relative_deviation", "average_iterations","average_time"
)

stat_results <- stat_results[, column_order]

#Save the table
write.csv(
    stat_results, 
    file = "stat_results/stat_results_ils.csv", 
    row.names = FALSE
)