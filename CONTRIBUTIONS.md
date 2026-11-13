# Contribution Report

**Project:** Smart Campus Navigation and Facility Analytics System
**Course:** CSCE 2110: Foundations of Data Structures\
**Last Updated:** 11-08-2026

---

## Summary Table
| Team Member | Component(s) | Specific Tasks Completed | Testing/Debugging | 
|-------------|--------------|--------------------------|-------------------|
| Matthew |• Utils.h<br>• Utils.cpp<br>• main.cpp<br>• Facility.cpp/Facility.h<br>• HashTable.h/HashTable.cpp<br>• FacilityDatabase.h/FacilityDatabase.cpp<br>• Screen.h/ Screen.cpp | • Facility records <br> • Custom hash table <br> • ID lookup <br> • Original text helpers, and shared menu integration <br> • Implement Screen clearer <br> • Fixed bugs and deleted unused functions <br> • Implement clearScreen() and pauseScreen() <br> • Add the buffer clearer in main.cpp| Local integration results are recorded in test_results.txt; final combined CELL testing is pending. <br> Fix the bugs "enter a number from 0 to 9" menu error. Replace the old logic with a simpler input check that clears the input buffer and accepts only numbers from 1 to 18. |

| Alka | • CampusGraph.h<br>• CampusGraph.cpp<br>• BFS Reachability<br>• README.md<br>• test_results.txt | • Developed the initial Campus Graph using adjacency lists.<br>• Implemented functions for adding buildings and connections.<br>• Worked on graph display, graph statistics, and BFS reachability.<br>• Collaborated with Sankalp on Campus Graph and BFS development.<br>• Collaborated with team members on graph integration with main.cpp.<br>• Reviewed Campus Graph code and checked header and implementation consistency.<br>• Prepared and updated README.md with project overview, features, compilation instructions, team contributions, and repository history.<br>• Committed and pushed project changes using GitHub branches and pull requests.<br>• Completed Member B testing on UNT CELL18.<br>• Documented all six test results in test_results.txt and pushed them to the alka-testing branch. | • Successfully compiled and ran the C++ project on UNT CELL18.<br>• Campus Graph Display — PASS (7 buildings and 6 connections).<br>• Graph Counts and Degrees — PASS (all degrees correct).<br>• BFS from Library — PASS (6 of 7 buildings reachable with correct distances).<br>• BFS from Observatory — PASS (1 of 7 reachable, distance 0).<br>• Invalid Building Name — PASS (error displayed without crashing).<br>• Case-Insensitive BFS — PASS (lowercase library recognized correctly).<br>• Verified program returned to the main menu after each test.<br>• **Final Result: 6/6 tests passed on UNT CELL18.** |

| Sankalp | CampusGraph; DepartmentNode; DepartmentTree; shared Utils | Building and connection loaders; graph/BFS; department loading, add, search, hierarchy and preorder; short comments and input helpers | Department options are integrated. Current local integration checks are in test_results.txt; earlier 71 module and 10 demo checks were for a previous revision. Final combined CELL testing is pending. |

[`GitHub Contributions`](https://github.com/MatthewHtang/campus-nav-analytics/commits/main/)

---

- Repository: https://github.com/MatthewHtang/campus-nav-analytics.git

Local integration fixes (2026-11-08): exit handling, category spelling, hash-loop warnings, documentation, and repeatable M1 checks were prepared with Codex assistance. Earlier individual CELL claims above are retained as reported, not rerun by this tool. Final ZIP matching, collaborator access, and final CELL execution still need team verification.
