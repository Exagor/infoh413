results <- read.csv("results/results_first.csv", header = TRUE)

ii <- "First"
results_ex_rd <- subset(results, permutation == "exchange" & initial == "random")
results_ex_cw <- subset(results, permutation == "exchange" & initial == "CW")
results_in_rd <- subset(results, permutation == "insert" & initial == "random")
results_in_cw <- subset(results, permutation == "insert" & initial == "CW")
results_tr_rd <- subset(results, permutation == "transpose" & initial == "random")
results_tr_cw <- subset(results, permutation == "transpose" & initial == "CW")

stat_results <- data.frame("algorithm" = character(), "average relative deviation" = numeric(), "average time" = numeric(), "size" = numeric())


aggregate_results <- aggregate(cbind(relative_deviation, time) ~ size, data = results_ex_rd, FUN = mean)
aggregate_results$algorithm <- paste(ii, "exchange random")
stat_results <- rbind(stat_results, aggregate_results)

aggregate_results <- aggregate(cbind(relative_deviation, time) ~ size, data = results_ex_cw, FUN = mean)
aggregate_results$algorithm <- paste(ii, "exchange CW")
stat_results <- rbind(stat_results, aggregate_results)

aggregate_results <- aggregate(cbind(relative_deviation, time) ~ size, data = results_in_rd, FUN = mean)
aggregate_results$algorithm <- paste(ii, "insert random")
stat_results <- rbind(stat_results, aggregate_results)

aggregate_results <- aggregate(cbind(relative_deviation, time) ~ size, data = results_in_cw, FUN = mean)
aggregate_results$algorithm <- paste(ii, "insert CW")
stat_results <- rbind(stat_results, aggregate_results)

aggregate_results <- aggregate(cbind(relative_deviation, time) ~ size, data = results_tr_rd, FUN = mean)
aggregate_results$algorithm <- paste(ii, "transpose random")
stat_results <- rbind(stat_results, aggregate_results)

aggregate_results <- aggregate(cbind(relative_deviation, time) ~ size, data = results_tr_cw, FUN = mean)
aggregate_results$algorithm <- paste(ii, "transpose CW")
stat_results <- rbind(stat_results, aggregate_results)

# Formatting the column
names(stat_results)[names(stat_results) == "relative_deviation"] <- "average_relative_deviation"
names(stat_results)[names(stat_results) == "time"] <- "average_time"

column_order <- c("algorithm", "average_relative_deviation", "average_time", "size")

stat_results <- stat_results[, column_order]

write.csv(stat_results, file = "statistics/stat_results_ii.csv", row.names = FALSE)