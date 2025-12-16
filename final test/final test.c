#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define FILENAME "students.dat"

// 1. Student 구조체 정의
typedef struct {
    char name[12];
    int kor;
    int eng;
    int math;
    int total;
    float avg;
    int rank;
} Student;

// 학생 데이터 배열과 현재 학생 수를 전역 변수처럼 사용합니다.
Student students[MAX_STUDENTS];
int student_count = 0;

/* 총점, 평균 계산 */
void CalcScores() {
    for (int i = 0; i < student_count; i++) {
        students[i].total = students[i].kor + students[i].eng + students[i].math;
        students[i].avg = students[i].total / 3.0f;
    }
}

/* 등수 계산 */
void CalcRank() {
    CalcScores(); // 등수 계산 전에 총점/평균을 먼저 계산

    for (int i = 0; i < student_count; i++) {
        students[i].rank = 1;
        for (int j = 0; j < student_count; j++) {
            if (students[i].total < students[j].total) {
                students[i].rank++;
            }
        }
    }
}

/* 1. .dat 파일에서 데이터 읽기 (i) */
void ReadFromFile() {
    FILE* fp = NULL;
    // 파일을 이진 읽기 모드("rb")로 엽니다.
    if (fopen_s(&fp, FILENAME, "rb") != 0 || fp == NULL) {
        printf(" 파일(%s)을 열 수 없습니다. 새로 시작합니다.\n", FILENAME);
        student_count = 0;
        return;
    }

    // 파일에서 Student 구조체 배열의 크기만큼 읽어옵니다.
    // 읽어온 항목의 개수가 student_count가 됩니다.
    student_count = fread(students, sizeof(Student), MAX_STUDENTS, fp);

    fclose(fp);
    printf(" %s 파일에서 %d명의 학생 데이터를 성공적으로 읽었습니다.\n", FILENAME, student_count);
}

/* 2. 추가 학생 정보 입력 (ii) */
void InputStudent() {
    if (student_count >= MAX_STUDENTS) {
        printf(" 더 이상 학생을 추가할 수 없습니다. (최대 %d명)\n", MAX_STUDENTS);
        return;
    }

    Student s;

    printf("--- [%d]번째 학생 정보 입력 ---\n", student_count + 1);

    printf("이름 (최대 11글자): ");
    scanf_s("%s", s.name, (unsigned)_countof(s.name));

    printf("국어 점수 (0~100): ");
    scanf_s("%d", &s.kor);

    printf("영어 점수 (0~100): ");
    scanf_s("%d", &s.eng);

    printf("수학 점수 (0~100): ");
    scanf_s("%d", &s.math);

    // 초기값 설정
    s.total = 0;
    s.avg = 0.0f;
    s.rank = 0;

    // 배열에 저장 (요구사항: 배열에 저장)
    students[student_count] = s;
    student_count++;

    printf(" 학생 정보가 성공적으로 입력되었습니다.\n");
}

/* 3. .dat 파일 저장 (iii) */
void SaveToFile() {
    if (student_count == 0) {
        printf(" 저장할 학생 데이터가 없습니다.\n");
        return;
    }

    FILE* fp = NULL;
    // 파일을 이진 쓰기 모드("wb")로 엽니다.
    if (fopen_s(&fp, FILENAME, "wb") != 0 || fp == NULL) {
        printf(" 파일(%s)을 저장용으로 열 수 없습니다.\n", FILENAME);
        return;
    }

    // 배열에 있는 student_count 개수만큼의 Student 구조체를 파일에 씁니다.
    fwrite(students, sizeof(Student), student_count, fp);

    fclose(fp);
    printf(" 현재 %d명의 학생 데이터가 %s 파일에 성공적으로 저장되었습니다.\n", student_count, FILENAME);
}

/* 4. 성적 확인 (평균 계산 등) (iv) */
void PrintStudents() {
    if (student_count == 0) {
        printf(" 출력할 학생 데이터가 없습니다. (1번 또는 2번 메뉴를 통해 입력하세요)\n");
        return;
    }

    CalcRank(); // 출력 전에 총점, 평균, 등수 계산

    printf("\n\n=== 학생 성적표 ===\n");
    printf("이름          국어  영어  수학  총점  평균   등수\n");
    printf("------------------------------------------------\n");

    for (int i = 0; i < student_count; i++) {
        printf("%-12s %3d  %3d  %3d %5d %5.1f %3d등\n",
            students[i].name,
            students[i].kor,
            students[i].eng,
            students[i].math,
            students[i].total,
            students[i].avg,
            students[i].rank);
    }
    printf("------------------------------------------------\n");
}


int main(void) {
    int menu;

    printf(">> 학생 성적 관리 프로그램 <<\n");

    while (1) {
        printf("\n[Menu]\n");
        printf("1. .dat 파일에서 데이터 읽기\n");
        printf("2. 추가 학생 정보 입력\n");
        printf("3. .dat 파일 저장\n");
        printf("4. 성적 확인 (평균 계산 등)\n");
        printf("5. 종료\n");
        printf("---------------------------\n");
        printf("선택(1~5): ");

        if (scanf_s("%d", &menu) != 1) {
            // 잘못된 입력(문자 등) 처리
            printf(" 잘못된 입력입니다. 메뉴를 다시 선택해 주세요.\n");
            while (getchar() != '\n'); // 입력 버퍼 비우기
            continue;
        }

        switch (menu) {
        case 1:
            ReadFromFile();
            break;
        case 2:
            InputStudent();
            break;
        case 3:
            SaveToFile();
            break;
        case 4:
            PrintStudents();
            break;
        case 5:
            printf("프로그램을 종료합니다.\n");
            // 요구사항 v. 프로그램 종료
            return 0;
        default:
            printf(" 잘못된 선택입니다. 1~5 사이의 숫자를 입력해 주세요.\n");
            break;
        }
    }
    
    return 0;
}