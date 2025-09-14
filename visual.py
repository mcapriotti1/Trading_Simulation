import pandas as pd
import matplotlib.pyplot as plt

CSV_PATH = "portfolio_summary.csv"

def plotPortfolio(data):
    plt.figure(figsize=(12,6))
    for col in data.columns[2:]:
        plt.plot(data["tick"], data[col], alpha=0.5, label=col)  # label each line
    plt.xlabel("Tick")
    plt.ylabel("Portfolio Value")
    plt.title("Bots Portfolio over Time")
    plt.legend() 
    plt.show()

def seperatePlotPortfolio(data, folder):
  typeNames = ["ValueInvestor","TrendFollower","StopLoss","MarketMaker","Noise"]

  plt.figure(figsize=(12, 15))
  for i, bot_type in enumerate(typeNames):
      plt.subplot(len(typeNames), 1, i+1)
      plt.plot(data["tick"], data[f"best_{bot_type}"], label="Best", color="green")
      plt.plot(data["tick"], data[f"worst_{bot_type}"], label="Worst", color="red")
      plt.plot(data["tick"], data[f"avg_{bot_type}"], label="Average", color="blue")
      plt.title(bot_type)
      plt.xlabel("Tick")
      plt.ylabel("Portfolio Value")
      plt.legend()
  plt.tight_layout()
  plt.savefig(folder + "/bots")

def plotTickers(data, folder):
    plt.figure(figsize=(12,4))
    plt.plot(data["tick"], data["lastPrice"], label="Market Price")
    plt.xlabel("Tick")
    plt.ylabel("Price")
    plt.title("Market Price over Time")
    plt.legend()
    plt.savefig(folder + "/market")

def plotAll(data, file):
    seperatePlotPortfolio(data, folder)
    plotTickers(data, folder)

if __name__ == "__main__":
    df = pd.read_csv(CSV_PATH)
    folder = "plots/"
    plotAll(df, folder)