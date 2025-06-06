#  Student Management System (C Language)

This is a simple **Student Management System** written in C that allows a lecturer to manage student records and allows students to log in and view their grades.

## Features

### Lecturer Functions:
- Secure login for lecturer
- Add new student records
- Display all student data
- Search student by ID
- Enroll students into subjects (Math, English, Science)
- Enter and update marks for enrolled students
- Save/load student data to/from a binary file (`students.dat`)

### Student Portal:
- Secure student login using ID and password
- View personal information and enrollment status
- See marks for enrolled subjects (if active)
- View average score and grade classification (A/B/C/F)

## Data Storage
All student data is stored in a binary file called `students.dat`. The file includes:
- Basic student info
- Enrollments
- Marks
- Login credentials

## How to Run
1. Compile using a C compiler:
   ```bash
   gcc student_management.c -o student_mgmt
  
## Default Lecturer Login
    Username: Kivunge
    Password: Adm@123
## Notes
-Student ID must be unique.
-Passwords and data are not encrypted — this is a console-based learning project.
-Designed for educational use only
