#Script to do the paired t-test and the wilcoxon signed rank test over the relative deviation
results_best <- read.csv("raw_results/results_best.csv", header = TRUE)
results_first <- read.csv("raw_results/results_first.csv", header = TRUE)
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

# list_results <- list(results_ex_rd, results_ex_cw, results_in_rd, results_in_cw, results_tr_rd, results_tr_cw)

# for (i in 1:length(list_results)) {
#   for (j in 1:length(list_results)) {
#     if (i != j) {
#       print(paste("Paired t-test between", list_results[[i]]$permutation[1], list_results[[i]]$initial[1], "and", list_results[[j]]$permutation[1], list_results[[j]]$initial[1]))
#       print(t.test(list_results[[i]]$relative_deviation, list_results[[j]]$relative_deviation, paired = TRUE))
#       print(paste("Wilcoxon signed rank test between", list_results[[i]]$permutation[1], list_results[[i]]$initial[1], "and", list_results[[j]]$permutation[1], list_results[[j]]$initial[1]))
#       print(wilcox.test(list_results[[i]]$relative_deviation, list_results[[j]]$relative_deviation, paired = TRUE))
#     }
#   }
# }

t.test(results_in_cw_f$relative_deviation, results_in_cw_b$relative_deviation, paired = TRUE)
wilcox.test(results_in_cw_f$relative_deviation, results_in_cw_b$relative_deviation, paired = TRUE)