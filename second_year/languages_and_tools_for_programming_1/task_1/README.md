# Song Chart Program

This program is designed to manage a song chart list. The songs eligible for voting are numbered from 1 to 99999999. The program reads input from the standard input, where each line contains a vote or a command to execute.

A valid vote is a non-empty list of song numbers within the range from 1 to MAX, where MAX is the maximum song number eligible for voting in the current chart list. A valid vote cannot include a song number that has been dropped from the chart list. In a valid vote, all song numbers must be distinct.

The program supports handling individual chart lists and provides summaries of the rankings. Both a chart list and a summary include a maximum of 7 highest-ranked songs.

If a song was listed in a previous chart list but didn't appear in the next chart list, it is dropped from the chart list, and no further votes can be cast for that song.

There are two commands:

1. `NEW MAX`

The `NEW` command closes the current chart list (if one is open) and starts a new chart list with the given MAX value. The MAX value must be within the range from 1 to 99999999 and must not be smaller than the MAX value of the previous (currently closing) vote. This command outputs the closing chart list to the standard output.

2. `TOP`

The `TOP` command outputs a summary of all previous chart lists to the standard output. The summary is determined by adding up the points obtained in each chart list. The song ranked first receives 7 points, the song ranked second receives 6 points, and so on until the song ranked seventh receives 1 point. This command does not interrupt the ongoing voting for the current chart list.

Both commands output at most 7 song numbers for the chart list or summary. For each song, it specifies the song number and the number of positions it has changed compared to the previous chart list or summary. If the song is new to the list or summary, it is represented by the '-' (hyphen) symbol.

Individual elements in the input lines are separated by whitespace, which can occur multiple times and can also appear at the beginning or end of a line. The program ignores empty lines and lines that consist solely of whitespace.

The program continuously checks for input errors. For each erroneous line, the program outputs the following diagnostic message to the standard error output:

`Error in line N: <original form of the erroneous line>`

where N denotes the line number. Lines are numbered starting from 1, including ignored and erroneous lines. Pay attention to the space after the colon and preserve the original form of the erroneous line in the error message.
