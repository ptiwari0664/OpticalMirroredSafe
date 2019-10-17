# OpticalMirroredSafe

Safe Ltd. is a company that manufactures high-quality safes. Its latest invention is an optical closure mechanism that uses a laser beam passing through a rectangular grid with several mirrors. 

When the laser is activated, a beam enters the top row of the grid horizontally from the left. The beam is reflected by every mirror that it hits. Each mirror has a 45 degree diagonal orientation, either / or \. If the beam exits the bottom row of the grid horizontally to the right, it is detected and the safe opens (see the left side of the figure above). Otherwise, the safe remains closed and an alarm is raised.

Each safe has a missing mirror, which prevents the laser beam from traveling successfully through the grid (see the right side of the figure above). The safe has a mechanism that enables the user to drop a single mirror into any empty grid cell. A legitimate user knows the correct position and orientation of the missing mirror (/ in row 4 column 3 above) and can thus open the safe. Without this knowledge, the user has to guess correctly, which can be difficult for safes with large grids.

Programmer's job is to determine if particular safes are actually secure. A secure safe does not open right away without inserting a mirror, and there is at least one valid location and orientation for the missing mirror. There may indeed be multiple such locations and orientations.


## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

The files rover_input.txt and rover_output.txt should be at home location. If this is not the case, then the program will not read the files and will output a bad file name and terminates. 

### Input
Each test case describes a single safe and starts with a line containing four integer numbers r, c, m, and n. The mechanisms grid has r rows and c columns.
Each of the next m lines contains two integer numbers ri and ci(1 ≤ ri ≤ r and 1 ≤ ci ≤ c) specifying that there is a / mirror in row ri column ci . The following n lines specify the positions of the \ mirrors in the same way. The m + n positions of the mirrors are pairwise distinct.






## Authors

* **Puneet Tiwari**
