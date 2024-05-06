data <- read.csv("raw_results/results_meme.csv")
data_short <- read.csv("raw_results/results_meme_short.csv")
data_middle <- read.csv("raw_results/results_meme_middle.csv")
# Compute the empirical cumulative distribution function (ECDF)
ecdf_data <- ecdf(data$relative_deviation)
ecdf_data_short <- ecdf(data_short$relative_deviation)
ecdf_data_middle <- ecdf(data_middle$relative_deviation)

png(filename = "figures/quality_distrib_meme.png")

# Plot the ECDF
plot(ecdf_data,main="",
     xlab = "Relative deviation (%)",
     ylab = "Cumulative frequency",
     xlim=c(0, 1.5),
     do.points = FALSE, verticals = TRUE, lty=1,
     panel.first = grid(lty = 2))
lines(ecdf_data_short, col = "#000000",do.points = FALSE, verticals = TRUE, lty=3)
lines(ecdf_data_middle, col = "#000000",do.points = FALSE, verticals = TRUE, lty=2)
legend("topleft", legend=c("250s","175s","100s"), lty=c(1,2,3), col=c("black","black","black"), bty="n",inset = c(0, 0.05))

# Close the current PNG device
invisible(dev.off())


data2 <- read.csv("raw_results/results_ils.csv")
data2_short <- read.csv("raw_results/results_ils_short.csv")
data2_middle <- read.csv("raw_results/results_ils_middle.csv")
# Compute the empirical cumulative distribution function (ECDF)
ecdf_data2 <- ecdf(data2$relative_deviation)
ecdf_data2_short <- ecdf(data2_short$relative_deviation)
ecdf_data2_middle <- ecdf(data2_middle$relative_deviation)
png(filename = "figures/quality_distrib_ils.png")
# Plot the ECDF
plot(ecdf_data2,main="",
     xlab = "Relative deviation (%)",
     ylab = "Cumulative frequency",
     xlim=c(0, 1.5),
     do.points = FALSE, verticals = TRUE, lty=1,
     panel.first = grid(lty = 2))
lines(ecdf_data2_short, col = "#000000",do.points = FALSE, verticals = TRUE, lty=3)
lines(ecdf_data2_middle, col = "#000000",do.points = FALSE, verticals = TRUE, lty=2)
legend("topleft", legend=c("250s","175s","100s"), lty=c(1,2,3), col=c("black","black","black"), bty="n",inset = c(0, 0.05))
# Close the current PNG device
invisible(dev.off())
