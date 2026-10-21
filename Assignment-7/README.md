**Assignment-07: University Timetable Scheduling**

A university is facing challenges in scheduling exam timetables due to overlapping student
enrollments in multiple courses. To prevent clashes, the university needs to assign exam
slots efficiently, ensuring that no two exams taken by the same student are scheduled at the
same time.
To solve this, the university decides to model the problem as a Graph Coloring Problem,
where:
● Each course is represented as a vertex.
● An edge exists between two vertices if a student is enrolled in both courses.
● Each vertex (course) must be assigned a color (time slot) such that no two adjacent
vertices share the same color (no two exams with common students are scheduled in the
same slot).
As a scheduling system developer, you must:
1. Model the problem as a graph and implement a graph coloring algorithm (e.g., Greedy
Coloring or Backtracking).
2. Minimize the number of colors (exam slots) needed while ensuring conflict-free
scheduling.
3. Handle large datasets with thousands of courses and students, optimizing performance.
4. Compare the efficiency of Greedy Coloring, DSATUR, and Welsh-Powell algorithms
for better scheduling.
Extend the solution to include room allocation constraints where exams in the same slot should
fit within available classrooms.
