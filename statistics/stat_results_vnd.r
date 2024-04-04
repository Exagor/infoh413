#Script to make general statistics about raw data
results <- read.csv("raw_results/results_vnd.csv", header = TRUE)

algo <- "VND"
results_vnd1 <- subset(results, permutation == "VND1")
results_vnd2 <- subset(results, permutation == "VND2")

#Create new table
stat_results <- data.frame("algorithm" = character(), "average relative deviation" = numeric(), "average time" = numeric(), "size" = numeric())

aggregate_results <- aggregate(cbind(relative_deviation, time) ~ size, data = results_vnd1, FUN = mean)
aggregate_results$algorithm <- paste(algo, "1") #insert name of the algorithm
stat_results <- rbind(stat_results, aggregate_results) #insert the row in the table

aggregate_results <- aggregate(cbind(relative_deviation, time) ~ size, data = results_vnd2, FUN = mean)
aggregate_results$algorithm <- paste(algo, "2")
stat_results <- rbind(stat_results, aggregate_results)


# Formatting the column
names(stat_results)[names(stat_results) == "relative_deviation"] <- "average_relative_deviation"
names(stat_results)[names(stat_results) == "time"] <- "average_time"

column_order <- c("algorithm", "size", "average_relative_deviation", "average_time")

stat_results <- stat_results[, column_order]
#Save the table
write.csv(stat_results, file = "stat_results/stat_results_vnd.csv", row.names = FALSE)