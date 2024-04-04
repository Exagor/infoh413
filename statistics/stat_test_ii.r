#Script to do the paired t-test and the wilcoxon signed rank test over the relative deviation
results <- read.csv("results/results_best.csv", header = TRUE)
size_inst <- 150

results_ex_rd <- subset(results, permutation == "exchange" & initial == "random" & size == size_inst)
results_ex_cw <- subset(results, permutation == "exchange" & initial == "CW" & size == size_inst)
results_in_rd <- subset(results, permutation == "insert" & initial == "random" & size == size_inst)
results_in_cw <- subset(results, permutation == "insert" & initial == "CW" & size == size_inst)
results_tr_rd <- subset(results, permutation == "transpose" & initial == "random" & size == size_inst)
results_tr_cw <- subset(results, permutation == "transpose" & initial == "CW" & size == size_inst)

list_results <- list(results_ex_rd, results_ex_cw, results_in_rd, results_in_cw, results_tr_rd, results_tr_cw)

for (i in 1:length(list_results)) {
  for (j in 1:length(list_results)) {
    if (i != j) {
      print(paste("Paired t-test between", list_results[[i]]$permutation[1], list_results[[i]]$initial[1], "and", list_results[[j]]$permutation[1], list_results[[j]]$initial[1]))
      print(t.test(list_results[[i]]$relative_deviation, list_results[[j]]$relative_deviation, paired = TRUE))
      print(paste("Wilcoxon signed rank test between", list_results[[i]]$permutation[1], list_results[[i]]$initial[1], "and", list_results[[j]]$permutation[1], list_results[[j]]$initial[1]))
      print(wilcox.test(list_results[[i]]$relative_deviation, list_results[[j]]$relative_deviation, paired = TRUE))
    }
  }
}

# t.test(results_ex_rd$relative_deviation, results_ex_cw$relative_deviation, paired = TRUE)
# wilcox.test(results_ex_rd$relative_deviation, results_ex_cw$relative_deviation, paired = TRUE)