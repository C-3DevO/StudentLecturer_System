#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_USERS 1000
#define FILENAME "students.dat"
#define MAX_TIMES 3

struct Student{
    char name[100];
    int ID;
    char status[20];
    float Math,Eng,Sci;
    char password[20];
    char course[20];
    int entrollMath;
    int entrollEng;
    int entrollSci;
    int authenticated;
    float GPA_average;
    char grade[10];
};

struct Lecturer{
    char username[30];
    char password[30];
    const char* u_name;
    const char* loginpass;
    int authenticated;

};

void AddStudents(struct Student s[],int *n);
void Display(struct Student s[],int n);
void SearchStudent(struct Student s[],int n);
void EntrollStudent(struct Student s[],int n);
int loadUsers(struct Student s[]);
void SaveUsers(struct Student s[],int n);
int Login_Lec(struct Lecturer *lec);
void EnterMarks(struct Student s[],int n);
int Login_Stu(struct Student s[], int n);
int Calc_average(struct Student s[], int index);
void Grading(struct Student s[], int i);
void Display_Entrollment(struct Student s[], int i);
void Print_grade(struct Student s[], int i);

int main()
{
    struct Student s[MAX_USERS];
    struct Lecturer lec;
    lec.u_name = "Kivunge";
    lec.loginpass = "Adm@123";
    lec.authenticated = 0;
    int n = loadUsers(s);
    int user_choice;
    do{
     printf("\n==== WELCOME ====\n");
     printf("1: Student\n");
     printf("2: Lecturer\n");
     printf("3: Exit\n");
     printf("Choice: ");
     scanf("%d", &user_choice);
     getchar();

     switch(user_choice){
        case 1:{
        int i = Login_Stu(s, n);
        if(i !=-1){
            int std_choice;
            do{
                printf("\n==== SELECT OPERATION ====\n");
                printf("1: View Entrolment\n");
                printf("2: View Grades\n");
                printf("3: Exit\n");
                printf("***********\n");
                printf("Choice:");
                scanf("%d",&std_choice);
                getchar();

                switch(std_choice){
                case 1:{
                  Display_Entrollment(s,i);
                  break;
                }

                case 2:{
                  Print_grade(s,i);
                  break;
                }

                case 3:{
                  printf("Returning to Homepage ......\n");
                  break;
                }
                default:
                 printf("Invalid Input");
                 break;
                }
            } while(std_choice !=3);
        }
        break;
     }
     case 2:{
     if(Login_Lec(&lec)){
       int choice;
       do{
       printf("\n==== WELCOME ====\n");
       printf("1: Add Students\n");
       printf("2: Display\n");
       printf("3: Search Student\n");
       printf("4: Entrol Student\n");
       printf("5: Enter Marks\n");
       printf("6: Exit\n");
       printf("***********\n");
       printf("Choice:");
       scanf("%d",&choice);

       switch(choice){
       case 1:{
        AddStudents(s,&n);
        SaveUsers(s,n);
        break;
        }
       case 2:{
        Display(s,n);
        SaveUsers(s,n);
        break;
        }
       case 3:{
        SearchStudent(s,n);
        break;
        }
       case 4: {
        EntrollStudent(s,n);
        SaveUsers(s,n);
        break;
        }
       case 5: {
        EnterMarks(s,n);
        SaveUsers(s,n);
        break;
        }
       case 6:{
        SaveUsers(s,n);
        printf("Exiting Program ......\n");
        break;
        }
       default:
        printf("Invalid Input");
        break;
        }

     } while(choice !=6);
       }else {
       printf("Contact Admin!!!!!!!!!!");
       }

       }
      case 3: {
       printf("Exiting Program ......\n");
       break;

      }
      default:
        printf("Invalid Input");
        break;
       }

      } while(user_choice !=3);
    return 0;
}
void AddStudents(struct Student s[],int *n){
  int new_n;
  printf("How Many Student Details do you want to enter! ");
  scanf("%d",&new_n);
  getchar();

  for(int i=0;i<new_n && *n< MAX_USERS;i++,(*n)++){
    printf("Student %d Details!\n",*n+1);
    printf("NAME:");
    fgets(s[*n].name, sizeof(s[*n].name),stdin);
    s[*n].name[strcspn(s[*n].name,"\n")] = 0;

    int validID = 0;
     while (!validID) {
        printf("STUDENT_ID: ");
        scanf("%d", &s[*n].ID);
        getchar();

        validID = 1; // Here I Assume it's valid
        for (int j = 0; j < *n; j++) {
           if (s[j].ID == s[*n].ID) {
             printf("Student ID already exists! Please re-enter a unique ID.\n");
             validID = 0;
             break;
          }
          }
        }

    printf("STATUS (Active/Inactive):");
    fgets(s[*n].status, sizeof(s[*n].status),stdin);
    s[*n].status[strcspn(s[*n].status,"\n")] = 0;

    // Set default status to Inactive if not provided
        if (strlen(s[*n].status) == 0) {
            strcpy(s[*n].status, "Inactive");
        }

    printf("PASSWORD: ");
    fgets(s[*n].password, sizeof(s[*n].password),stdin);
    s[*n].password[strcspn(s[*n].password,"\n")] = 0;

    printf("COURSE: ");
    fgets(s[*n].course, sizeof(s[*n].course),stdin);
    s[*n].course[strcspn(s[*n].course,"\n")] = 0;

    // Initializing subject scores to 0.0
    s[*n].Math = s[*n].Eng = s[*n].Sci = 0.0;
    s[*n].entrollMath = s[*n].entrollEng = s[*n].entrollSci = 0;
    s[*n].GPA_average = 0.0;
    strcpy(s[*n].grade, "PENDING");
  }
  if (*n >= MAX_USERS) {
     printf("Maximum Student limit reached.\n");
     return;
    }
}
void SaveUsers(struct Student s[],int n){
    FILE *fp = fopen(FILENAME, "wb");
    if(fp == NULL){
        printf("Error Opening File!\n");
        return;
    }
fwrite(&n,sizeof(int),1,fp);
fwrite(s,sizeof(struct Student),n,fp);
fclose(fp);
}
int loadUsers(struct Student s[]) {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        return 0; // No existing file — start fresh
    }
    int n;
    fread(&n, sizeof(int), 1, fp);
    fread(s, sizeof(struct Student), n, fp);
    fclose(fp);
    return n;
}
void Display(struct Student s[], int n) {
    printf("\n--- Student Records ---\n");
    printf("+--------------------+------------+--------------+----------+---------+---------+---------+---------+\n");
    printf("| %-20s | %-12s | %-12s | %-10s | %-9s | %-9s | %-9s |\n","NAME","ID","STATUS","COURSE","MATH","ENG","SCI");
    for (int i = 0; i < n; i++) {
     printf("| %-20s | %-12d | %-12s | %-10s | %-9.2f | %-9.2f | %-9.2f |\n",s[i].name,s[i].ID,s[i].status,s[i].course,s[i].Math, s[i].Eng, s[i].Sci);
    }
    printf("+--------------------+------------+--------------+----------+---------+---------+---------+---------+\n");
}
void SearchStudent(struct Student s[],int n){
  int searchID;
  int found = 0;
  printf("Please Input Student ID to Search: ");
  scanf("%d",&searchID);
  getchar();// Clear the newline character from the buffer
  for(int i=0; i<n;i++){
    if(s[i].ID == searchID){
    printf("\n--- Student Found ---\n");
    printf("+--------------------+------------+--------------+----------+---------+---------+---------+---------+\n");
    printf("| %-20s | %-12s | %-12s | %-10s | %-9s | %-9s | %-9s |\n","NAME","ID","STATUS","COURSE","MATH","ENG","SCI");
    printf("| %-20s | %-12d | %-12s | %-10s | %-9.2f | %-9.2f | %-9.2f |\n",s[i].name,s[i].ID,s[i].status,s[i].course,s[i].Math, s[i].Eng, s[i].Sci);
    printf("+--------------------+------------+--------------+----------+---------+---------+---------+---------+\n");
    found = 1;
    }

  }
  if(!found){
    printf("Student ID not found..\n");
  }

}
void EntrollStudent(struct Student s[],int n){
    int searchID;
    int found = 0;
    printf("Please Input Student ID to Search: ");
    scanf("%d",&searchID);
    getchar();// Clear the newline character from the buffer

    for(int i = 0; i<n;i++){
        if(s[i].ID == searchID){
           found = 1;
           printf("\n--- Student Found ---\n");
           printf("+--------------------+------------+--------------+----------+---------+---------+---------+---------+\n");
           printf("| %-20s | %-12s | %-12s | %-10s | %-9s | %-9s | %-9s |\n","NAME","ID","STATUS","COURSE","MATH","ENG","SCI");
           printf("| %-20s | %-12d | %-12s | %-10s | %-9.2f | %-9.2f | %-9.2f |\n",s[i].name,s[i].ID,s[i].status,s[i].course,s[i].Math, s[i].Eng, s[i].Sci);
           printf("+--------------------+------------+--------------+----------+---------+---------+---------+---------+\n");
           printf("Do you wish to continue Entrolling the student:(1 = yes, 0 = no): ");
           int choice;
           scanf("%d", &choice);
           if(choice == 1){
             printf("Enroll in Math (1 = yes, 0 = no): ");
             scanf("%d",&s[i].entrollMath);
             printf("Enroll in Eng (1 = yes, 0 = no): ");
             scanf("%d",&s[i].entrollEng);
             printf("Enroll in Sci (1 = yes, 0 = no): ");
             scanf("%d",&s[i].entrollSci);
             getchar();// Clear the newline character from the buffer

             strcpy(s[i].status,"Active");
             printf("\nEnrollment updated successfully for:\n");
             printf("Student Name: %s\n", s[i].name);
             printf("Student ID  : %d\n", s[i].ID);
             printf("Status  : %s\n", s[i].status);

      } else {
             printf("Enrollment cancelled.\n");
      }
      return;// Exit after processing the matching student
     }
    }
    if(!found) {
      printf("Student not found..\n");
    }
}
int Login_Lec(struct Lecturer *lec){
  int trials = 0;
  char username[30];
  char password[30];
  while(trials<MAX_TIMES){
    printf("Enter your Username: ");
    fgets(username, sizeof(username),stdin);
    username[strcspn(username,"\n")] = 0;
    printf("Enter your Password: ");
    fgets(password, sizeof(password),stdin);
    password[strcspn(password,"\n")] = 0;
    if((strcmp(username,lec->u_name)==0 && strcmp(password,lec->loginpass)==0)){
      printf("Login Successful!\n");
      lec->authenticated = 1;
      return 1;
    }else{
    trials++;
    printf("\nIncorrect Login Details!!.....You have %d trials remaining\n",MAX_TIMES-trials);
  }
  }
    printf("Too many failed attempts. Access denied.\n");
    lec->authenticated = 0;
    return 0;
}

int Login_Stu(struct Student s[], int n){
    int id, trials = 0;
    char password[20];

    while(trials < MAX_TIMES){
        printf("Enter your Student ID: ");
        scanf("%d", &id);
        getchar();
        printf("Enter your Password: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = 0;

        for(int i = 0; i < n; i++){
            if(s[i].ID == id && strcmp(s[i].password, password) == 0){
                printf("Login Successful! Welcome %s\n", s[i].name);
                return i;
            }
        }

        trials++;
        printf("Incorrect login. Attempts left: %d\n", MAX_TIMES - trials);
    }

    printf("Too many failed attempts.\n");
    return -1;
}

void EnterMarks(struct Student s[],int n){
   int searchID;
   int found = 0;
   printf("Please Input Student ID to Search: ");
   scanf("%d",&searchID);
   getchar();// Clear the newline character from the buffer

   for(int i = 0; i<n; i++){
    if(s[i].ID == searchID){
        found = 1;
        printf("\n--- Student Found ---\n");
        printf("+-------------------+------------+------------+-----------+\n");
        printf("| %-20s | %-12s | %-12s | %-10s |\n","NAME","ID","STATUS","COURSE");
        printf("| %-20s | %-12d | %-12s | %-10s |\n", s[i].name, s[i].ID,s[i].status,s[i].course);
        printf("+-------------------+------------+------------+-----------+\n");
        printf("Do you wish to continue Updating the student Marks:(1 = yes, 0 = no): ");
        int choice;
        scanf("%d", &choice);
        if(choice == 1){
            if(s[i].entrollMath == 1){
            printf("Enter Mathematics Marks: ");
            scanf("%f",&s[i].Math);
            if (s[i].Math < 0 || s[i].Math > 100) {
            printf("Invalid Math mark. Must be between 0 and 100.\n");
            return;
            }
            }

            if(s[i].entrollEng == 1){
            printf("Enter English Marks: ");
            scanf("%f",&s[i].Eng);
            if (s[i].Eng < 0 || s[i].Eng > 100) {
            printf("Invalid Eng mark. Must be between 0 and 100.\n");
            return;
            }
            }

            if(s[i].entrollSci == 1){
            printf("Enter Science Marks: ");
            scanf("%f",&s[i].Sci);
            if (s[i].Sci < 0 || s[i].Sci > 100) {
            printf("Invalid Eng mark. Must be between 0 and 100.\n");
            return;
            }
            }

            getchar();
            Calc_average(s, i);
            Grading(s, i);
            //Printing the updated Marks
            printf("\n--- Student Found ---\n");
            printf("+--------------------+------------+--------------+----------+---------+---------+---------+---------+\n");
            printf("| %-20s | %-12s | %-12s | %-10s | %-9s | %-9s | %-9s |\n","NAME","ID","STATUS","COURSE","MATH","ENG","SCI");
            printf("| %-20s | %-12d | %-12s | %-10s | %-9.2f | %-9.2f | %-9.2f |\n",s[i].name,s[i].ID,s[i].status,s[i].course,s[i].Math, s[i].Eng, s[i].Sci);
            printf("+--------------------+------------+--------------+----------+---------+---------+---------+---------+\n");

        } else{
            printf("Marks Update cancelled.\n");
        }
        return;
    }
   }
   if(!found) {
      printf("Student not found..\n");
    }

}
int Calc_average(struct Student s[], int index){
    int subjects = 0;
    float sum = 0.0;

    if (s[index].entrollMath) {
        sum += s[index].Math;
        subjects++;
    }
    if (s[index].entrollEng) {
        sum += s[index].Eng;
        subjects++;
    }
    if (s[index].entrollSci) {
        sum += s[index].Sci;
        subjects++;
    }

    if (subjects > 0) {
        s[index].GPA_average = sum / subjects;
    } else {
        s[index].GPA_average = 0.0;
    }

    return s[index].GPA_average;
}
void Grading(struct Student s[], int i){
    if(s[i].GPA_average>=70){
        strcpy(s[i].grade, "A");
    }else if(s[i].GPA_average>=60 && s[i].GPA_average<70){
        strcpy(s[i].grade, "B");
    }else if(s[i].GPA_average>=50 && s[i].GPA_average<60){
        strcpy(s[i].grade, "C");
    }else if(s[i].GPA_average>=40 && s[i].GPA_average<50){
        strcpy(s[i].grade, "D");
    }else {
        strcpy(s[i].grade, "F");
    }

}
void Display_Entrollment(struct Student s[], int i){
        printf("\n--- Student Details ---\n");
        printf("+----------------+----------+--------------+----------+\n");
        printf("| %-14s | %-8s | %-12s | %-8s |\n","NAME","ID","STATUS","COURSE");
        printf("| %-14s | %-8d | %-12s | %-8s |\n",s[i].name,s[i].ID,s[i].status,s[i].course);
        printf("+----------------+----------+--------------+----------+\n");
        printf("Enrolled Subjects:\n");
         if (s[i].entrollMath) printf(" - Math\n");
         if (s[i].entrollEng)  printf(" - English\n");
         if (s[i].entrollSci)  printf(" - Science\n");

         if (!s[i].entrollMath && !s[i].entrollEng && !s[i].entrollSci)
           printf(" - None\n");
}
void Print_grade(struct Student s[], int i) {
    printf("+------------+------------+------------+---------+---------+---------+-------------+--------+\n");
    printf("| %-10s | %-10s | %-10s | %-7s | %-7s | %-7s | %-11s | %-6s |\n",
           "ID", "STATUS", "COURSE", "MATH", "ENG", "SCI", "GPA_Avg", "GRADE");
    printf("+------------+------------+------------+---------+---------+---------+-------------+--------+\n");
    printf("| %-10d | %-10s | %-10s | %-7.2f | %-7.2f | %-7.2f | %-11.2f | %-6s |\n",
           s[i].ID, s[i].status, s[i].course, s[i].Math, s[i].Eng, s[i].Sci, s[i].GPA_average, s[i].grade);
    printf("+------------+------------+------------+---------+---------+---------+-------------+--------+\n");
}
