#include <iostream>
#include <cstdio>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <thread>
//#define ran(x) (rand()%x)
using namespace std;

bool gameOver = false;

int ran(int x, int y){
	int a;
	if (y<x){
		int t;
		t = x;
		x = y;
		y = t;
	}
	a = rand() % (y - x + 1) + x;
	if(a>0)return a;
	else return -a;
}

void getKeyboardHit(int* n,bool* gameover){
	char c;
	int d;
	for (;;){
		if (*gameover)return;
		if (_kbhit()){
			c = _getch();
			switch (c)
			{
			case 'w':d = 1; break;
			case 's':d = 2; break;
			case 'a':d = 3; break;
			case 'd':d = 4; break;
			default:
				break;
			}
			if ((d == 1 && *n == 2) || (d == 2 && *n == 1));
			else if ((d == 3 && *n == 4) || (d == 4 && *n == 3));
			else *n = d;
		}
	}
	
}

class MAP{
public:
	int x;//��ͼ����
	int y;//��ͼ����
	vector<vector<int> > numMap;//�洢��ͼ�Ķ�ά����
	MAP(int width, int height);//���캯��
	void creatMap(char c);
private:
	int x1, y1, x2, y2, box_num;
	bool getPos(int* x_out, int* y_out, int left_x, int right_x, int up_y, int down_y);//��Ҫ������һ�������
	void makeBox();//����һ������
	void printMap();//����Ļ�����MAP��ͼ��
};

//MAP�Ĺ��캯��
MAP::MAP(int height, int width){
	x = height;
	y = width;
	x1 = 0;
	x2 = 0;
	y1 = 0;
	y2 = 0;
	box_num = 2;
	vector<vector<int> > temp_map(x);//��ʱ��ͼ��λ����
	//��ʼ�����飬���0
	for (int i = 0; i < x; i++){
		for (int j = 0; j < y; j++){
			temp_map[i].push_back(0);
		}
	}
	for (int j = 0; j < width; j++)//�����±߿�
	{
		temp_map[0][j] = 1;
		temp_map[height - 1][j] = 1;
	}
	for (int i = 0; i<height; i++)//�����ұ߿�
	{
		temp_map[i][0] = 1;
		temp_map[i][width - 1] = 1;
	}
	//����ʱ����ת�Ƶ�ʵ��������
	numMap.swap(temp_map);
}

//��ӡMAP
void MAP::printMap(){
	system("cls");
	for(int i = 0;i<x;i++){
		for(int j = 0; j < y;j++){
			if(numMap[i][j]<=0)cout<<" ";
			else cout<<"#";
		}
		cout<<endl;
	}
}

//�������һ������
void MAP::makeBox(){
	srand((int)time(0));
	bool TooClose = true;
	//ȡ�ڶ�����ʧ��ʱ����ȡ��һ���㣨��1 λ��̫ƫƧ��
	while (TooClose){
		TooClose = false;
		do{
			getPos(&x1, &y1, 3, x - 4, 3, y - 4);
		} while (!getPos(&x2, &y2, x1 + 3, x1 + 10, y1 + 3, y1 + 10));
		for (int i = x1 - 3; i < x2 + 3; i++){
			for (int j = y1 - 3; j < y2 + 3; j++){
				if (numMap[i][j] > 0)TooClose = true;
			}
		}
	}
	for (int i = x1; i <= x2; i++){
		for (int j = y1; j <= y2; j++){
			numMap[i][j] = box_num;
		}
	}
	box_num++;
}
//��[left_x , right_x],[up_y , down_y] �����������������(x_out , y_out)
bool MAP::getPos(int* x_out, int* y_out, int left_x, int right_x, int up_y, int down_y){
	if (right_x > x || down_y > y)return false;
	bool AllZero = false;
	int temp_x, temp_y;
	do{
		AllZero = true;
		temp_x = ran(left_x, right_x);
		temp_y = ran(up_y, down_y);
		for (int i = 0; i < 5; i++){
			for (int j = 0; j < 5; j++){
				if (numMap[temp_x - 2 + i][temp_y - 2 + j] > 0)AllZero = false;
			}
		}
	} while (!AllZero);
	*x_out = temp_x;
	*y_out = temp_y;
	return true;
}
//�������������ͼ
void MAP::creatMap(char c){
	int NumOfBox;
	if (c == 'a')NumOfBox = 1;
	else if (c == 'b')NumOfBox = 3;
	for (int i = 0; i < NumOfBox; i++){
		makeBox();
	}
	system("cls");
	printMap();
}

class Snake{
public:
	//��ά�����ʾ�� body[0]Ϊͷ�����꣬(body[i][0],body[i][1])Ϊ��i�����������
	vector<vector<int> > body;
	vector<int> food;
	int direction;
	Snake(MAP* m);
	void printSnake();
	void addSnakeToMap(MAP* m);
	void moveSnake(MAP* m);
	void makeFood(MAP* m);
	
};
//���캯��
Snake::Snake(MAP* m){
	bool AllZero = false;
	int temp_x, temp_y;
	vector<vector<int> > temp_body(3);
	do{
		AllZero = true;
		temp_x = ran(3, m->x - 4);
		temp_y = ran(3, m->y - 4);
		for (int i = 0; i < 5; i++){
			for (int j = 0; j < 5; j++){
				if (m->numMap[temp_x - 2 + i][temp_y - 2 + j] > 0)AllZero = false;
			}
		}
	} while (!AllZero);
	//��ʼ����ͷ�͵�һ���������β
	temp_body[0].push_back(temp_x);
	temp_body[0].push_back(temp_y);
	temp_body[1].push_back(temp_x - 1);
	temp_body[1].push_back(temp_y);
	temp_body[2].push_back(temp_x - 2);
	temp_body[2].push_back(temp_y);
	body.swap(temp_body);
	//1�ϣ�2�£�3��4��
	direction = 2;
	//�������һ��ʳ��
	do{
		AllZero = true;
		temp_x = ran(3, m->x - 4);
		temp_y = ran(3, m->y - 4);
		for (int i = 0; i < 5; i++){
			for (int j = 0; j < 5; j++){
				if (m->numMap[temp_x - 2 + i][temp_y - 2 + j] != 0)AllZero = false;
			}
		}
	} while (!AllZero);
	food.push_back(temp_x);
	food.push_back(temp_y);
}
//���ߵ�������Ϣ���ݵ���ͼ������
void Snake::addSnakeToMap(MAP* m){
	//-1Ϊ��ʵ��-2Ϊ��ͷ��-3Ϊ����0Ϊ��β
	for (unsigned i = 0; i < body.size() - 1; i++){
		if (i == 0)m->numMap[body[0][0]][body[0][1]] = -2;
		else m->numMap[body[i][0]][body[i][1]] = -3;
	}
	m->numMap[food[0]][food[1]] = -1;
	m->numMap[body[body.size() - 1][0]][body[body.size() - 1][1]] = 0;
}
//�ߵ���һ��λ��
void Snake::moveSnake(MAP* m){
	vector<int> next_pos(2);
	switch (direction){
	case 1:next_pos[0] = body[0][0] - 1; next_pos[1] = body[0][1]; break;//����
	case 2:next_pos[0] = body[0][0] + 1; next_pos[1] = body[0][1]; break;//����
	case 3:next_pos[0] = body[0][0]; next_pos[1] = body[0][1] - 1; break;//����
	case 4:next_pos[0] = body[0][0]; next_pos[1] = body[0][1] + 1; break;//����
	}
	if (m->numMap[next_pos[0]][next_pos[1]] == 0)//ͷײ�������������괫�ݸ���
	{
		for (int i = body.size() - 1; i > 0; i--)body[i] = body[i - 1];
		body[0] = next_pos;
	}
	else if (m->numMap[next_pos[0]][next_pos[1]] == -3 || m->numMap[next_pos[0]][next_pos[1]] > 0)
		gameOver = true;//ͷײ�������ǽ����Ϸ����
	else if (m->numMap[next_pos[0]][next_pos[1]] == -1)//ͷײʳ�β������+1���ƶ�һ��
	{
		vector<int> temp_tail(2);
		temp_tail = body[body.size() - 1];
		for (int i = body.size() - 1; i > 0; i--)body[i] = body[i - 1];
		body[0] = next_pos;
		body.push_back(temp_tail);
		makeFood(m);
	}
}
//����ʳ��
void Snake::makeFood(MAP* m){
	int temp_x,temp_y;
	do{
		temp_x = ran(3, m->x - 4);
		temp_y = ran(3, m->y - 4);
	} while (m->numMap[temp_x][temp_y] != 0);
	food[0]=(temp_x);
	food[1]=(temp_y);
}

//�ֲ����£���ӡ����
void Snake::printSnake(){
	//���ع��
	CONSOLE_CURSOR_INFO cursor_info;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
	cursor_info.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
	COORD pos;
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	pos.Y = food[0];
	pos.X = food[1];
	SetConsoleCursorPosition(hOutput, pos);
	cout << "*";
	pos.Y = body[0][0];
	pos.X = body[0][1];
	SetConsoleCursorPosition(hOutput,pos);
	cout<<"@";
	for(unsigned i = 1 ;i < body.size() - 1;i++){
		pos.Y = body[i][0];
		pos.X = body[i][1];
		SetConsoleCursorPosition(hOutput,pos);
		cout<<"0";
	}
	pos.Y = body[body.size()-1][0];
	pos.X = body[body.size()-1][1];
	SetConsoleCursorPosition(hOutput,pos);
	cout<<" ";
	pos.Y = 20;
	pos.X = 90;
	SetConsoleCursorPosition(hOutput,pos);
	cout << "�÷֣�" << body.size() - 3;
}
//��Ϸѭ��
int gameLoop(char lever){
	MAP M(40, 80);
	M.creatMap(lever);
	Snake s(&M);
	s.direction = 2;
	s.printSnake();
	thread th_getInput = thread(getKeyboardHit, &s.direction, &gameOver);
	bool fastest = false;
	do{
		s.moveSnake(&M);
		s.addSnakeToMap(&M);
		s.printSnake();
		if (fastest)Sleep(50);
		else{
			Sleep(300 - 10 * (s.body.size() - 3));
			if ((300 - 10 * (s.body.size() - 3)) < 50)fastest = true;
		}
	} while (!gameOver);
	th_getInput.join();
	return s.body.size() - 3;
}
//��Ϸ��ʼ
char gameStart(){
	char s;
	cout << "��ӭ����̰������Ϸ!" << endl;
	cout << "�������ֻ��Ǹ����أ�\n���������룺a\n���������룺b" << endl;
	cin >> s;
	while (!(s == 'a' || s == 'b'))
	{
		cout << "�ԣ����֣������ü��̿��Ը���ճ��^v^" << endl;
		cout << "����������һ�Σ�";
		cin >> s;
		cout << s;
	}
	cout<<"��Ŷ����Ϸ3����Զ���ʼ������׼������"<<endl;
	Sleep(1000);
	cout << "3..." << endl;
	Sleep(1000);
	cout << "2..." << endl;
	Sleep(1000);
	cout << "1..." << endl;
	Sleep(1000);
	system("cls");
	return s;
}
//��Ϸ����
bool gameEnd(int g){
	system("cls");
	cout<<"��Ϸ����������û�ˣ�"<<endl;
	if (g <= 20)cout << "������յ÷�ֻ��" << g << "�֣��̣�" << endl;
	else if (g <= 70)cout << "������յ÷�Ϊ" << g << "�֣�һ��һ������" << endl;
	else if (g <= 100)cout << "������յ÷�Ϊ" << g << "�֣���Ӵ������Ŷ��" << endl;
	else if (g <= 150)cout << "������յ÷־�Ȼ��" << g << "�֣��������" << endl;
	else cout << "������յ÷־�Ȼ��" << g << "�֣�����ô�ã������ж����ģ�" << endl;
	Sleep(3000);
	cout << "�Ƿ�ʼ��һ�֣�[Y/N]��";
	char c;
	cin >> c;
	return (c == 'y' || c == 'Y');
}
int main(){
	system("mode con cols=120 lines=50");
	srand((int)time(0));
	char lever;
	int grade;
	while (true) {
		system("cls");
		lever = gameStart();
		grade = gameLoop(lever);
		if (!gameEnd(grade))break;
		else gameOver = false;
	}
	return 0;
}
