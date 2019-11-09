//#include<string>
//#include<vector>
using namespace std;

class Module_4
{

public:

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

void get_matrix();
void remove_a_cell();
void partition(int x1,int y1,int x2,int y2,int a,int b);
void place_triomino(int x,int y,int s,int t,int p,int q);
void insert_values_using_triomino();
void print_matrix();
};
