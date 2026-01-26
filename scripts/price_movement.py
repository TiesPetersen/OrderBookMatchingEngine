import matplotlib.pyplot as plt

prices = []
limit_orders = []

with open('price_movement.txt', 'r') as f:
    for line in f:
        parts = line.strip().split()
        if not parts:
            continue
        current_price = float(parts[0])
        prices.append(current_price)
        if len(parts) > 1:
            limit_orders.append((len(prices) - 1, float(parts[1]), current_price))

plt.figure(figsize=(12, 6))
plt.plot(prices, linewidth=1, label='Base Price', color='grey')

if limit_orders:
    above = [(i, lp) for i, lp, cp in limit_orders if lp > cp]
    below = [(i, lp) for i, lp, cp in limit_orders if lp <= cp]
    if above:
        xs, ys = zip(*above)
        plt.scatter(xs, ys, color='green', label='Limit order > current price', zorder=3, s=2)
    if below:
        xs, ys = zip(*below)
        plt.scatter(xs, ys, color='red', label='Limit order ≤ current price', zorder=3, s=2)

plt.xlabel('Tick')
plt.ylabel('Base Price')
plt.title('Limit Orders Generated + Base Price Movement')
plt.grid(True, alpha=0.3)
plt.legend()
plt.tight_layout()
plt.savefig('price_movement.png', dpi=300)