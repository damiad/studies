# Part 2: Server-Side Application

## Requirements for Completion
Opened: Sunday, April 11, 2021
Required by: Wednesday, May 10, 2023

In this phase of application development, you need to create a data model, appropriate page templates with placeholders, and a link-button user interface for the application.

## Data Model
Create a data model with the following entities:

1. Directory - Entities of this type should store information about files and other directories. Besides the relationships with other entities, they should have the following fields:
   - Name
   - Optional description
   - Creation date
   - Owner
   - Accessibility flag (false when the directory is deleted, initially set to true)
   - Date of changing the accessibility flag
   - Date of the last content modification

2. File - Entities of this type should store source code of programs. The source code is divided into meaningful sections. Besides the relationships with other entities, these entities should have the following fields:
   - Name
   - Optional description
   - Creation date
   - Owner
   - Accessibility flag (false when the file is deleted, initially set to true)
   - Date of changing the accessibility flag
   - Date of the last content modification

3. File Section - Entities of this type contain meaningful parts of a file or comments. The structure of the section is complex as it may have subsections. Besides the relationships with other entities, these entities should have the following fields:
   - Optional name
   - Optional description
   - Creation date
   - Section start
   - Section end
   - Section type
   - Section status
   - Status data
   - Content

Note: There are two options for defining the start and end of a section. In the first option, the section always starts at the beginning of a line and ends at the end of a line. The start and end points are represented by line numbers. In the second option, the start and end points can be anywhere. In this case, they are represented by two values: line number and character position in the line. You can choose either of these models.

4. Section Type - This entity determines the type of content in a section. Categories define how the section is handled by the application. The minimum set of categories includes: procedure, comment, compiler directive (#define, #pragma, etc.), variable declarations, and assembler insertions.

5. Section Status - This entity defines the status of a section. Examples of status are: compiling with warnings, compiling without warnings, not compiling.

6. Status Data - Additional data related to the status, e.g., compilation error, line number for warnings, etc.

7. User - This entity defines the application's user. Besides the relationships with other entities, these entities should have the following fields:
   - Name
   - Login
   - Password

The above entities and their fields may use English names.

The described model should be implemented with a user interface that aligns with the application's functionality.

## Scoring for Entities
The following scoring scheme will be used for implementing individual entities:

- User - 1 point
- Entities related to files and directories - 1 point
- Entities related to sections - 1 point

## Link-Button Interface and Page Templates
The application should provide the following features, similar to the SDCC compiler.

1. The "File Selection" section should display the available directory structure from the database. This section should be implemented as a page template (or templates) populated with data from the database.

2. The application should allow adding a file to the database and dividing it into sections based on the categories mentioned above. This process should happen partially automatically, but users should also have the option to specify sections manually, including adding, marking fragments of the file as sections, and merging existing sections into a single section. These operations should be accessible from one of the menus in the menu bar. Note that full functionality with JavaScript is not required; server-side partial implementation is sufficient.

3. The application should allow adding a directory to the database. This operation should be accessible from one of the menus in the menu bar.

4. The application should allow "deleting" a file or directory. However, files and directories should not be permanently removed from the database. Instead, they should be marked as unavailable using the accessibility flag. This operation should be accessible from one of the menus in the menu bar and should be associated with the appropriate file or directory.

5. The "Code Fragment" section should display the contents of the filename.asm file obtained from the following command:
   ```
   # sdcc -S <filename>.c
   ```
   The output from the filename.asm file should be divided into ranges separated by dashed lines. Hovering over a section should highlight it. Separate highlighting should be applied to section headers and contents. Displaying this section should be ensured through an appropriate page template (or templates).

6. At the bottom of the screen, there should be four tabs with the following contents:
   - The first tab (titled "STANDARD") should indicate the compatibility standard for the compiler (at least C89, C99, C11). Users should be able to choose one of them. After selection, all compiler executions performed by the application should follow the chosen standard.
   - The second tab (titled "OPTIMIZATIONS") should contain a list of available optimization types (at least 3). Users should be able to specify the chosen set of optimizations. After selection, all compiler executions performed by the application should generate code according to the chosen optimizations.
   - The third tab (titled "PROCESSOR") should contain a list of available SDCC processor architectures (at least MCS51, Z80, and STM8). Users should be able to choose one of them. After selection, all compiler executions performed by the application should be specific to the chosen processor.
   - The fourth tab (titled "DEPENDENT") should contain a list of compiler options that are dependent on the processor. For each selected processor, there should be the possibility to choose three specific options related to that processor (e.g., for the MCS51 processor, there may be an option to choose the target program model: small, medium, large, or huge model programs).

7. The menu should include an option allowing the user to perform a full compilation of the currently selected file. The process should update the "Code Fragment" section's content and provide the option to save the compilation result on the local disk.

