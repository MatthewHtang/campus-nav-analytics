# Contribution Report

**Project:** Smart Campus Navigation and Facility Analytics System
**Course:** CSCE 2110: Foundations of Data Structures\
**Last Updated:** 10-26-2026

---

## Summary Table
| Team Member | Component(s) | Specific Tasks Completed | Testing/Debugging | 
|-------------|--------------|--------------------------|-------------------|
| Matthew |• Utils.h<br>• Utils.cpp<br>• main.cpp<br>• Facility.cpp/Facility.h<br>• HashTable.h/HashTable.cpp<br>• FacilityDatabase.h/FacilityDatabase.cpp |• Functions(Trim, Split, toUpper/toLower<br> equalsIgnoreCase)  |  |
| Alka | • CampusGraph.h<br>• CampusGraph.cpp<br>• BFS Reachability<br>• README.md<br>• test_results.txt | • Developed the initial Campus Graph using adjacency lists.<br>• Implemented functions for adding buildings and connections.<br>• Worked on graph display, graph statistics, and BFS reachability.<br>• Collaborated with Sankalp on Campus Graph and BFS development.<br>• Collaborated with team members on graph integration with main.cpp.<br>• Reviewed Campus Graph code and checked header and implementation consistency.<br>• Prepared and updated README.md with project overview, features, compilation instructions, team contributions, and repository history.<br>• Committed and pushed project changes using GitHub branches and pull requests.<br>• Completed Member B testing on UNT CELL18.<br>• Documented all six test results in test_results.txt and pushed them to the alka-testing branch. | • Successfully compiled and ran the C++ project on UNT CELL18.<br>• Campus Graph Display — PASS (7 buildings and 6 connections).<br>• Graph Counts and Degrees — PASS (all degrees correct).<br>• BFS from Library — PASS (6 of 7 buildings reachable with correct distances).<br>• BFS from Observatory — PASS (1 of 7 reachable, distance 0).<br>• Invalid Building Name — PASS (error displayed without crashing).<br>• Case-Insensitive BFS — PASS (lowercase library recognized correctly).<br>• Verified program returned to the main menu after each test.<br>• **Final Result: 6/6 tests passed on UNT CELL18.** |
| Sankalp | CampusGraph; DepartmentNode; DepartmentTree; shared Utils | Building and connection loaders; graph/BFS; department loading, add, search, hierarchy and preorder; short comments and input helpers | 71 module checks and 10 demo checks passed locally; see test_results.txt. Team-menu integration and CELL testing are still pending. |

[`GitHub Contributions`](https://github.com/MatthewHtang/campus-nav-analytics/commits/main/)

---

- Repository: https://github.com/MatthewHtang/campus-nav-analytics.git
