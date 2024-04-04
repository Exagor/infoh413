#Script to do the paired t-test and the wilcoxon signed rank test over the relative deviation
results <- read.csv("raw_results/results_vnd.csv", header = TRUE)

results_vnd1 <- subset(results, permutation == "VND1")
results_vnd2 <- subset(results, permutation == "VND2")

t.test(results_vnd1$relative_deviation, results_vnd2$relative_deviation, paired = TRUE)
wilcox.test(results_vnd1$relative_deviation, results_vnd2$relative_deviation, paired = TRUE)