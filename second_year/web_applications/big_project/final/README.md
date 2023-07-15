# Final part: Browser-Side Automation

## Requirements for Completion
Opened: Monday, April 26, 2021
Required by: Monday, May 29, 2023

In this iteration of the application, we will extend it by adding new functionalities and improving its reliability.

## New Features of the Solution
1. Implement infrastructure for user login. It is not necessary to provide a "signup" functionality. Adding new users can be done through the application's administrative interface. Files and directories added by a specific user must be stored in the database as belonging to that user.

2. Enhance user experience by avoiding page reloads for file operations and menu interactions.

3. Enable manual section creation where the content includes the selected file fragment. If the section separation model is chosen with line precision, the section starts from the line where the selection begins and ends at the line where the selection ends.

4. In case of incorrect file compilation, display the information output by the compiler in the "Code Fragment" area. The information should include hints indicating the relevant line number in the source code. Similar hints should be provided for the *.asm file. Help the user by allowing them to click on a line number hint in the "Code Fragment" area – regardless of the compilation success or failure – to highlight the corresponding line in the source code. The highlighting should make the line visible in the code window and indicate it by changing its graphical properties (e.g., color, font properties) or by changing its location (e.g., the line becomes the first visible line in the code window). After clicking on the hint, the graphical highlighting should disappear.

5. When the compiler finishes successfully, the "Code Fragment" area should display the content of the *.asm file. However, sometimes the sections in this file may be very long. Implement the option to: (a) hide and show the content of all sections, and (b) hide and show the content of individual sections. Section headers should always remain visible.

## Scoring
- Adding users (through the administrative panel or by other means) - 1 point
- User login functionality - 1 point
- File operations without page reloads - 1 point
- Menu interactions without page reloads - 1 point
- Manual section creation for files - 1 point
- Highlighting source code line after clicking on a line number hint in the "Code Fragment" section - 1 point
- Hiding and showing section contents - 1 point

## Improving Application Reliability
1. Implement tests for the server-side part of the application. The tests should cover the models, views, and forms used in the application.

## Scoring
- Tests for all models - 1 point
- Tests for all views - 1 point
- Tests for all forms (if the application does not use forms, the tests for views should include functions to check the correctness of data sent from the application client; in such a case, up to 2 points can be awarded for the views) - 1 point