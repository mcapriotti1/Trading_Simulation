import pandas as pd
import matplotlib.pyplot as plt

CSV_PATH = "portfolioSummary.csv"

def seperatePlotPortfolio(data, folder):
  typeNames = ["ValueInvestor","TrendFollower","StopLoss","MarketMaker","Noise"]

  plt.figure(figsize=(12, 15))
  for i, bot_type in enumerate(typeNames):
      plt.subplot(len(typeNames), 1, i+1)
      plt.plot(data["tick"], data[f"best_{bot_type}"], label="Best")
      plt.plot(data["tick"], data[f"worst_{bot_type}"], label="Worst")
      plt.plot(data["tick"], data[f"avg_{bot_type}"], label="Average")
      plt.title(bot_type)
      plt.xlabel("Tick")
      plt.ylabel("Portfolio Value")
      plt.legend()
  plt.tight_layout()
  plt.subplots_adjust(top=0.92)
  plt.tight_layout(rect=[0, 0.05, 1, 0.92])
  plt.subplots_adjust(hspace=1)
  plt.savefig(folder + "/bots.pdf")

def plotBotComparison(data, folder="plots"):
  typeNames = ["ValueInvestor","TrendFollower","StopLoss","MarketMaker","Noise"]
  
  fig, axes = plt.subplots(3, 1, figsize=(12, 15), sharex=True)

  # Best of each bot
  for bot_type in typeNames:
      axes[0].plot(data["tick"], data[f"best_{bot_type}"], label=bot_type)
  axes[0].set_ylabel("Portfolio Value")
  axes[0].set_title("Best Portfolio of Each Bot Type")
  axes[0].legend()

  # Average of each bot
  for bot_type in typeNames:
      axes[1].plot(data["tick"], data[f"avg_{bot_type}"], label=bot_type)
  axes[1].set_ylabel("Portfolio Value")
  axes[1].set_title("Average Portfolio of Each Bot Type")
  axes[1].legend()

  # Market price
  axes[2].plot(data["tick"], data["lastPrice"], label="Market Price", color="black")
  axes[2].set_xlabel("Tick")
  axes[2].set_ylabel("Price")
  axes[2].set_title("Market Price Over Time")
  axes[2].legend()

  plt.tight_layout()
  plt.savefig(f"{folder}/bot_comparison.pdf")
  plt.show()

def plotAll(data, file):
  seperatePlotPortfolio(data, folder)
  plotBotComparison(data, folder)

if __name__ == "__main__":
  df = pd.read_csv(CSV_PATH)
  folder = "plots/"
  plotAll(df, folder)