//#include<string>
#include<vector>
using namespace std;

void move(int disk,string First,string Last);
void Tower_Of_Hanoi(int disk , string Begin , string End , string Intermediate );
void Step_Count(int steps);
void Staircase_Problem();
void Hanoi_Problem();
bool compare1(struct node a , struct node b);
bool compare2(struct node a , struct node b);
vector<struct node> maxCount_Desc(vector<struct node> &list);
vector<struct node> maxCount_Asc(vector<struct node> &list);
void Circus_Problem();
void Triomino();
void get_matrix();
void remove_a_cell();
void insert_values_using_triomino();
void print_matrix();
void partition(int start_row , int start_column , int end_row , int end_column);
void place_triomino(int p, int q, int start_row, int start_column); 
