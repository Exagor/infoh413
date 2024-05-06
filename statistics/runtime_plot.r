data1 <- read.csv("raw_results/runtimes/runtime_ILS_N-be75eec_150.csv")
data2 <- read.csv("raw_results/runtimes/runtime_meme_N-be75eec_150.csv")

best_cost <- 3482828
data1 <- cbind(data1,relative_deviation = ((best_cost - data1$Cost) / best_cost) * 100)
data2 <- cbind(data2,relative_deviation = ((best_cost - data2$Cost) / best_cost) * 100)



# Start the PNG device.
png(filename = "figures/runtime_plot.png", width = 800, height = 800)

# Plot the Cost vs Time for data1
plot(data1$Time, data1$relative_deviation, type = "l", col = "#00ff5e", 
    xlab = "run-time [CPU sec]",
    ylab = "Relative deviation (%)",
    ylim = c(0, 1.5),
    main = "",
    first.panel=grid(lty=2))
legend("topright", legend=c("ILS","MA"), lty=1, col=c("#00ff5e","#ff0000"), bty="n",inset = c(0, 0.05),cex=1.2)

# Add the Cost vs Time for data2 to the existing plot
lines(data2$Time, data2$relative_deviation, col = "#ff0000")




invisible(dev.off()) #close png