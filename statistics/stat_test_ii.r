#Script to do the paired t-test and the wilcoxon signed rank test over the relative deviation
results_best <- read.csv("raw_results/results_best2.csv", header = TRUE)
results_first <- read.csv("raw_results/results_first2.csv", header = TRUE)
size_inst <- 150

results_ex_rd_f <- subset(results_first, permutation == "exchange" & initial == "random" & size == size_inst)
results_ex_cw_f <- subset(results_first, permutation == "exchange" & initial == "CW" & size == size_inst)
results_in_rd_f <- subset(results_first, permutation == "insert" & initial == "random" & size == size_inst)
results_in_cw_f <- subset(results_first, permutation == "insert" & initial == "CW" & size == size_inst)
results_tr_rd_f <- subset(results_first, permutation == "transpose" & initial == "random" & size == size_inst)
results_tr_cw_f <- subset(results_first, permutation == "transpose" & initial == "CW" & size == size_inst)

results_ex_rd_b <- subset(results_best, permutation == "exchange" & initial == "random" & size == size_inst)
results_ex_cw_b <- subset(results_best, permutation == "exchange" & initial == "CW" & size == size_inst)
results_in_rd_b <- subset(results_best, permutation == "insert" & initial == "random" & size == size_inst)
results_in_cw_b <- subset(results_best, permutation == "insert" & initial == "CW" & size == size_inst)
results_tr_rd_b <- subset(results_best, permutation == "transpose" & initial == "random" & size == size_inst)
results_tr_cw_b <- subset(results_best, permutation == "transpose" & initial == "CW" & size == size_inst)

# Create an empty data frame to store the results
stat_results <- data.frame()

# Loop through each subset and perform t-test and wilcox.test
subsets <- c("ex_rd_f", "ex_cw_f", "in_rd_f", "in_cw_f", "tr_rd_f", "tr_cw_f", "ex_rd_b", "ex_cw_b", "in_rd_b", "in_cw_b", "tr_rd_b", "tr_cw_b")

for (subset in subsets) {
    subset_results <- get(paste0("results_", subset))
     for (subset2 in subsets){
        subset_results2 <- get(paste0("results_", subset2))
        t_test_result <- t.test(subset_results$relative_deviation, subset_results2$relative_deviation, paired = TRUE)
        wilcox_test_result <- wilcox.test(subset_results$relative_deviation, subset_results2$relative_deviation, paired = TRUE)
    
        # Append the results to the stat_results data frame
        stat_results <- rbind(stat_results, data.frame(
            Subset1 = subset,
            Subset2 = subset2,
            T_Test_P_Value = t_test_result$p.value,
            Wilcox_Test_P_Value = wilcox_test_result$p.value
        ))
    }
   
}

# Write the results to a CSV file
write.csv(stat_results, file = "stat_results/stat_tests.csv", row.names = FALSE)

# t.test(results_in_cw_f$relative_deviation, results_in_cw_b$relative_deviation, paired = TRUE)
# wilcox.test(results_in_cw_f$relative_deviation, results_in_cw_b$relative_deviation, paired = TRUE)