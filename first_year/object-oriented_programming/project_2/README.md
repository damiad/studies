# Bajt Trade Task

The goal of this task is to create a market simulation involving Agents whose objective is to acquire as many diamonds as possible. Two primary types of Agents are identified: Workers and Speculators. For any doubts, please refer to the Assumptions section.

## Simulation Flow

The simulation will be conducted in turns, also known as days. The simulation begins with turn number 1. Each turn consists of the following stages, executed in the specified order. Each stage is detailed in its respective section:

1) Workers decide whether to spend the day learning or working.
a) Workers engage in learning. After learning, the day ends (no further points below refer to it).
b) Workers engage in work.
i) Workers produce items based on their strategies.
ii) Workers list their produced items for sale on the market. Workers' sale offers do not include prices; they only specify the quantity and type of items for sale.
iii) Workers list purchase offers. Similarly, purchase offers do not include prices; they only specify the type and quantity of items desired.

2) Speculators enter the market with their purchase and sale offers. Their offers already include prices, in addition to the quantity and type of items.

3) The market matches purchase offers with sale offers. Only offers from Workers and Speculators are matched, thus determining the transaction prices. As a result, Workers do not sell/buy items to/from other Workers, and the same applies to Speculators.

4) After executing the matched transactions, the market buys unsold products from Workers. Unfulfilled offers from Speculators (both purchase and sale) and Workers (purchase) are cleared from the market's memory. Therefore, each turn starts with an empty set of offers.

5) At the end of the day, Workers consume their items: food, clothing, tools, and used computer programs.

It is worth noting that a learning Agent does not consume any items at the end of the turn, including food.

## Workers

Each Worker begins the simulation with a fixed number of products (loaded from input). Additionally, each Worker is assigned a base productivity vector, which determines the number of units of each product the Worker can produce in one turn. The values in this vector are always multiples of 100. During the simulation, this value will be modified by various bonuses (expressed as percentages, e.g., +10%, +20%, -1%, -150%), ensuring that Workers always produce a whole number of items per turn. If the resulting production, after applying bonuses (which can be negative), is negative, no items are produced. For example, if the base production of tools is 200 and we have the following bonuses: -10%, 20%, 40%, the production in a given turn would be 200 + 200 * (20 + 40 - 10)% = 200 + 100 = 300.

In each turn, a Worker can choose to either learn or work. Learning involves advancing in the current career path or switching career paths. Working involves production, sale, and purchase of items. If a Worker chooses to work, the Worker also consumes (loses) items as follows:

- Consumes 100 units of food. If the Worker has fewer units, the consumed amount is equal to what they have, but it is treated as if they hadn't eaten that day.
- Consumes all their tools.
- Adds one day of wear to a maximum of 100 items of clothing. The remaining clothing items (if any) remain unchanged. An item is considered completely worn out if it has been worn for the same number of days as its durability. If a Worker has fewer than 100 clothing items, one day of wear is added to all items they have.
- Consumes the computer programs used for production on that day. The remaining programs remain unchanged.

When does a Worker learn, and when do they work?

Each Worker follows one of the following strategies, which determine their actions for the day:

- "Hard Worker" never learns and always works.
- "Thrifty" learns only when they have more than a certain number of diamonds, where the threshold is a strategy parameter.
- "Student" learns whenever they can afford to buy 100 * stock units of food, calculated using the average of the average prices over the last n days, where stock and n are strategy parameters.
- "Periodic" learns every period_of_learning days, where period_of_learning is a strategy parameter. On other days, they work. For example, with a period_of_learning of 10, learning occurs on days 10, 20, 30, and so on.
- "Random" works with a probability of 1 - 1 / (simulation_day + 3) and learns with a probability of 1 / (simulation_day + 3).

How does a Worker learn?

A Worker can learn in one of two ways: by advancing in their current career path or by switching career paths. When changing their career path, a Worker retains their level from previous career paths. Although they can only use production bonuses from their current career path, if they switch back to a career they had previously, they start at the level they achieved before. For example, if a Worker was a level 3 Farmer and became a Miner, a career they had not pursued before, they would start as a level 1 Miner. Later, if they switch back to being a Farmer, they will resume at level 3 (instead of level 1).

Workers have two strategies for career path changes:

- "Conservative" never changes their career path.
- "Revolutionary" changes their career path once every 7 days. They calculate n as max(1, their id modulo the total number of Workers), where the total number of Workers is a strategy parameter. On the designated day, they switch to the next career path available in their strategy's list.

The simulation continues for a specified number of turns, and the final results are evaluated based on the number of diamonds each Agent has acquired.

## Assumptions

The simulation assumes the following:

- The market operates efficiently, with instant matching of buy and sell offers.
- Agents make rational decisions based on their strategies and available information.
- Prices are determined solely by supply and demand in the market.
- The simulation does not account for external factors such as randomness or external events.

## Conclusion

The Bajt Trade task involves creating a market simulation where Workers and Speculators interact to acquire diamonds. By implementing the specified rules and strategies, the simulation will demonstrate how different approaches can lead to varying levels of success for the Agents.

The provided guidelines outline the core mechanics of the simulation, including the roles and actions of the Agents, the market dynamics, and the learning and working strategies of the Workers. By following these instructions, you can develop a comprehensive and functional simulation that reflects the dynamics of a competitive market.
