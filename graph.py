import pandas as pd
import matplotlib.pyplot as plt

df_fcfs = pd.read_csv("FCFS.csv")
df_sjf = pd.read_csv("SJF.csv")

fig, axes = plt.subplots(1, 2, figsize=(12, 5))

# FCFS
df_fcfs.plot(
    x="Algorithm",
    y=["Average Waiting Time", "Average Turnaround Time"],
    kind="bar",
    ax=axes[0],
)
axes[0].set_title("FCFS")
axes[0].set_ylabel("Time (units)")
axes[0].set_xlabel("Algorithm")
axes[0].grid(True)

# SJF
df_sjf.plot(
    x="Algorithm",
    y=["Average Waiting Time", "Average Turnaround Time"],
    kind="bar",
    ax=axes[1],
)
axes[1].set_title("SJF")
axes[1].set_xlabel("Algorithm")
axes[1].grid(True)

plt.suptitle("CPU Scheduling Comparison")
plt.tight_layout()
plt.show()

