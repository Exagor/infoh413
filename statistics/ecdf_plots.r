data1 <- read.csv("raw_results/results_meme.csv")

# Compute the empirical cumulative distribution function (ECDF)
ecdf_data <- ecdf(data1$relative_deviation)

png(filename = "figures/quality_distrib_meme.png")

# Plot the ECDF
plot(ecdf_data,main="",
     xlab = "Relative deviation (%)",
     ylab = "Cumulative frequency",
     xlim=c(0, 1),
     do.points = FALSE, verticals = TRUE, lty=1,
     panel.first = grid(lty = 2))
legend("topleft", legend=c("250s"), lty=1, col="black", bty="n",inset = c(0, 0.05))
grid()
# Close the current PNG device
invisible(dev.off())


data2 <- read.csv("raw_results/results_ils.csv")
# Compute the empirical cumulative distribution function (ECDF)
ecdf_data2 <- ecdf(data2$relative_deviation)
png(filename = "figures/quality_distrib_ils.png")
# Plot the ECDF
plot(ecdf_data2,main="",
     xlab = "Relative deviation (%)",
     ylab = "Cumulative frequency",
     xlim=c(0, 1),
     do.points = FALSE, verticals = TRUE, lty=1,
     panel.first = grid(lty = 2))
legend("topleft", legend=c("250s"), lty=1, col="black", bty="n",inset = c(0, 0.05))
grid()
# Close the current PNG device
invisible(dev.off())
