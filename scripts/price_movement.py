import matplotlib.pyplot as plt

# Read price data from file
with open('price_movement.txt', 'r') as f:
    prices = [float(line.strip()) for line in f if line.strip()]

# Plot the price movement
plt.figure(figsize=(12, 6))
plt.plot(prices, linewidth=2)
plt.xlabel('Time')
plt.ylabel('Price')
plt.title('Price Movement Over Time')
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig('price_movement.png', dpi=300)