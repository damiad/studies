# Task: MAT
Matryca

Lab for Algorithms and Data Structures, Lab 1 (Warm-up). Memory Limit: 128 MB. November 2, 2022, 23:59:59.

## Description

Bajtek Printing Company (BPC) has received a large order to produce striped wallpapers, which are the season's hit in interior design. Each wallpaper consists of n equally wide colored vertical stripes. BPC is responsible for designing and printing the wallpapers. The client specified the colors of some stripes on the wallpaper. For the remaining stripes, BPC has full freedom.

To print the wallpapers, BPC uses printing plates that can print a certain number of consecutive stripes on the wallpaper. The plate has specified colors for each printed stripe and may be shorter than the entire wallpaper. If the plate consists of k stripes, it is applied in all n−k+1 possible positions where its stripes overlap with the wallpaper's stripes, printing all the stripes on the plate each time. This means that one stripe on the wallpaper can be printed multiple times. If a stripe is printed with different colors, its final color will be a mixture of those colors.

Regardless of their aesthetic sense, BPC employees want to design the shortest possible plate that allows them to print the entire wallpaper. They must keep in mind that for the stripes specified by the client, they must use a pure color without any other color mixture. In other words, whenever the plate overlaps with such a stripe, the color of the stripe on the plate must exactly match the one specified by the client.

## Input

The input consists of a single line containing a string composed of capital letters of the Latin alphabet and asterisks (*), representing the desired appearance of the wallpaper. Each letter denotes a different color of a stripe, while asterisks denote stripes whose color has not been specified by the client. The length of the string, n, satisfies 1 ≤ n ≤ 1,000,000.

## Output

Your program should print a single line containing a single integer, k: the minimum length of the plate that allows printing the desired wallpaper.

## Example

For the input:

```
A*B*B*A
```

the correct output is:

```
6
```

## Constraints

- 1 ≤ n ≤ 1,000,000 (length of the string representing the desired wallpaper)

---
