#Script to do the paired t-test and the wilcoxon signed rank test over the relative deviation
results_meme <- read.csv("raw_results/results_meme.csv", header = TRUE)
results_ils <- read.csv("raw_results/results_ils.csv", header = TRUE)

wilcox.test(results_meme$relative_deviation, results_ils$relative_deviation, paired = TRUE)