data10 <- read.csv("raw_results/results_meme_pop10.csv")
data20 <- read.csv("raw_results/results_meme_pop20.csv")
data30 <- read.csv("raw_results/results_meme_short.csv")
data50 <- read.csv("raw_results/results_meme_pop50.csv")

# Start the PNG device.
png(filename = "figures/pop_plot_meme.png", width = 800, height = 800)

# Calculate the average of the feature relative_deviation for each dataset
avg10 <- mean(data10$relative_deviation)
avg20 <- mean(data20$relative_deviation)
avg30 <- mean(data30$relative_deviation)
avg50 <- mean(data50$relative_deviation)

# Create a bar plot of the average relative_deviation
plot(c(10, 20, 30, 50),c(avg10, avg20, avg30, avg50), type = "b", col = "blue",
    xlab = "Population size",
    ylab = "Average relative deviation",
    main = "")

invisible(dev.off()) #close png

data1 <- read.csv("raw_results/results_ils_pert1.csv")
data8 <- read.csv("raw_results/results_ils_short.csv")
data20 <- read.csv("raw_results/results_ils_pert20.csv")

png(filename = "figures/pert_plot_ils.png", width = 800, height = 800)

avg1 <- mean(data1$relative_deviation)
avg8 <- mean(data8$relative_deviation)
avg20 <- mean(data20$relative_deviation)

plot(c(1, 8, 20),c(avg1, avg8, avg20), type = "b", col = "blue",
    xlab = "Perturbation number",
    ylab = "Average relative deviation",
    main = "")

invisible(dev.off()) #close png