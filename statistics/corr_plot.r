data1 <- read.csv("raw_results/results_meme.csv")
data2 <- read.csv("raw_results/results_ils.csv")

print(cor(data1$relative_deviation, data2$relative_deviation))
model <- lm(data2$relative_deviation ~ data1$relative_deviation)
se <- sqrt(sum(resid(model)^2) / model$df.residual) # Standard error of the regression

# Start the PNG device.
png(filename = "figures/corr_plot_meme_ils.png", width = 800, height = 800)
plot(data1$relative_deviation, data2$relative_deviation,
    xlab = "MA relative deviation (%)",
    ylab = "ILS relative deviation (%)",
    xlim = c(0, 1),
    ylim = c(0, 1),
    pch = 1,
    col = "blue",
    panel.first = grid(nx = 4, ny = 4, lty = 2))
legend("topleft", legend = c("Linear regression", "Standard error"),
       col = c("red", "#fbab00"), lty = 1:2, cex = 1.2)


# Print the parameters of the linear model
print(summary(model))
# Get the coefficients of the model
coefficients <- coef(model)
cat("Equation of the line: y =", coefficients[1], "+", coefficients[2], "* x\n")
cat("Standard error of the regression:", se, "\n")

abline(model, col = "red")
abline(a = coef(model)[1] - se, b = coef(model)[2], col = "#fbab00", lty = 2)
abline(a = coef(model)[1] + se, b = coef(model)[2], col = "#fbab00", lty = 2)

invisible(dev.off()) #close png