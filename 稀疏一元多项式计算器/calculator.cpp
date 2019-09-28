#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#pragma warning(disable:4996)
#define OK 1
#define ERROR 0
#define OVERFLOW -2
typedef int status;
using namespace std;

struct unit {
	double coef;
	int exp;
	struct unit *next;
};
typedef struct unit unit;
typedef struct {
	unit *polynomial;
}Line;
typedef struct {
	Line polynomials[18];
}Storage;

void InitStorage(Storage &S) {
	int i;
	for (i = 0; i < 18; i++) S.polynomials[i].polynomial = NULL;
	return;
}

void CreatPoly(Line &L) {
	L.polynomial = new unit[1];
	L.polynomial->coef = 0;
	L.polynomial->exp = 0;
	L.polynomial->next = NULL;
	return;
}

void DeletePoly(Line &L) {
	unit *prior, *inferior;
	if (!L.polynomial) return;
	prior = L.polynomial->next;
	inferior = L.polynomial;
	if (prior) {
		delete[] inferior;
		inferior = prior;
		prior = prior->next;
	}
	delete[] inferior;
	L.polynomial = NULL;
	return;
}

void ClearPoly(Line &L) {
	DeletePoly(L);
	CreatPoly(L);
	return;
}

status InputPoly(Storage &S, int x) {
	double coef;
	int exp;
	unit *p, *q, *comp, *temp;
	if (x < 0 || x > 9) return OVERFLOW;
	p = S.polynomials[x].polynomial;
	//DeletePoly(S.polynomials[x]);
	ClearPoly(S.polynomials[x]);
	scanf_s("%lf %d", &coef, &exp);
	while (1) {
		if (coef < 0.00000001&&coef>-0.00000001) {
			if (exp == 0)break;
			else goto a;
		}
		if (exp<0) goto a;
		comp = S.polynomials[x].polynomial;
		q = new unit[1];
		q->coef = coef;
		q->exp = exp;
		while (comp)
		{
			if (!comp->next) {
				comp->next = q;
				q->next = NULL;
				break;
			}
			if (exp == comp->next->exp) {
				if (comp->next->coef + coef == 0) {
					temp = comp->next;
					comp->next = temp->next;
					free(temp);
				}
				else comp->next->coef = comp->next->coef + coef;
				break;
			}
			if (exp > comp->next->exp) {
				temp = comp->next;
				comp->next = q;
				q->next = temp;
				break;
			}
			else comp = comp->next;
		}
	a:scanf_s("%lf %d", &coef, &exp);
	}
	return OK;
}

status PrintPoly(Storage S, int x) {
	unit *p;
	if (x < 0 || x > 9) { printf("Overflow\n"); return OVERFLOW; }
	p = S.polynomials[x].polynomial;
	if (!p) { printf("NULL\n"); return ERROR; }
	if (!p->next) { printf("%.4lf\n",0); return OK; }
	p = p->next;
	while (p) {
		printf("%.4lf", p->coef);
		if (p->exp != 0) {
			if (p->exp == 1)printf("x");
			else printf("x^%d", p->exp);
		}
		if (p->next) { 
			if(p->next->coef>0)printf("+");
		}
		p = p->next;
	}
	printf("\n");
	return OK;
}

status CopyPoly(Storage &S, int a, int b) {//将式a复制到b处
	unit* p, *q,*temp;
	if (!S.polynomials[a].polynomial) { printf("Poly Not Exist\n"); return OVERFLOW; }
	if (a == b) return OK;
	ClearPoly(S.polynomials[b]);
	p = S.polynomials[a].polynomial->next;
	q = S.polynomials[b].polynomial;
	while (p) {
		temp = new unit[1];
		temp->coef = p->coef;
		temp->exp = p->exp;
		temp->next = NULL;
		q->next = temp;
		p = p->next;
		q = q->next;
	}
	return OK;
}

status AddPoly(Storage &S, int a, int b, int c) {//将式a于式b相加，存到c处
	unit* posa, *posb, *posc, *temp;
	if (!S.polynomials[a].polynomial || !S.polynomials[b].polynomial) { printf_s("NOT EXIST\n"); return ERROR; }//检查是否存在
	CreatPoly(S.polynomials[10]);
	posa = S.polynomials[a].polynomial->next;
	posb = S.polynomials[b].polynomial->next;
	posc = S.polynomials[10].polynomial;
	while (posa&&posb) {
		if (posa->exp > posb->exp) {
			temp = new unit[1];
			temp->coef = posa->coef;
			temp->exp = posa->exp;
			temp->next = NULL;
			posc->next = temp;
			posc = posc->next;
			posa = posa->next;
			continue;
		}
		if (posa->exp < posb->exp) {
			temp = new unit[1];
			temp->coef = posb->coef;
			temp->exp = posb->exp;
			temp->next = NULL;
			posc->next = temp;
			posc = posc->next;
			posb = posb->next;
			continue;
		}
		else {
			if (posa->coef + posb->coef >= 0.00000001|| posa->coef + posb->coef<=-0.00000001) {
				temp = new unit[1];
				temp->coef = posa->coef + posb->coef;
				temp->exp = posa->exp;
				temp->next = NULL;
				posc->next = temp;
				posc = posc->next;
			}
			posa = posa->next;
			posb = posb->next;
			continue;
		}
	}
	if (!posa) {
		while (posb) {
			temp = new unit[1];
			temp->coef = posb->coef;
			temp->exp = posb->exp;
			temp->next = NULL;
			posc->next = temp;
			posc = posc->next;
			posb = posb->next;
		}
	}
	if (!posb) {
		while (posa) {
			temp = new unit[1];
			temp->coef = posa->coef;
			temp->exp = posa->exp;
			temp->next = NULL;
			posc->next = temp;
			posc = posc->next;
			posa = posa->next;
		}
	}
	CopyPoly(S, 10, c);
	DeletePoly(S.polynomials[10]);
	return OK;
}

status MinusPoly(Storage &S, int a, int b, int c) {//多项式a减去多项式b，存到c处
	unit *posb;
	if (!S.polynomials[a].polynomial || !S.polynomials[b].polynomial) { printf_s("NOT EXIST\n"); return ERROR; }//检查是否存在
	posb = S.polynomials[b].polynomial;
	while (posb) {//将多项式b反号
		posb->coef = 0 - posb->coef;
		posb = posb->next;
	}
	AddPoly(S, a, b, c);
	if (b != c) {//把多项式b的系数再调回来
		while (posb) {
			posb->coef = 0 - posb->coef;
			posb = posb->next;
		}
	}
	return OK;
}

status MultiplyPoly(Storage &S, int a, int b, int c) {//多项式a乘多项式b，存到c处
	unit* posa, *posb, *temp,*comp,*temp2;
	if (!S.polynomials[a].polynomial || !S.polynomials[b].polynomial) { printf_s("NOT EXIST\n"); return ERROR; }//检查是否存在
	posa = S.polynomials[a].polynomial->next;
	posb = S.polynomials[b].polynomial->next;
	CreatPoly(S.polynomials[10]);

	while (posa) {
		while (posb) {
			temp = new unit[1];
			temp->coef = posa->coef*posb->coef;
			temp->exp = posa->exp+posb->exp;
			temp->next = NULL;
			comp = S.polynomials[10].polynomial;
			while (comp)
			{
				if (!comp->next) {
					comp->next = temp;
					break;
				}
				if (temp->exp == comp->next->exp) {
					if (comp->next->coef + temp->coef < 0.00000001&&comp->next->coef + temp->coef>-0.00000001) {
						temp2 = comp->next;
						comp->next = temp2->next;
						free(temp2);
					}
					else { comp->next->coef = comp->next->coef + temp->coef; free(temp); }
					break;
				}
				if (temp->exp > comp->next->exp) {
					temp2 = comp->next;
					comp->next = temp;
					temp->next = temp2;
					break;
				}
				else comp = comp->next;
			}
			posb = posb->next;
		}
		posa = posa->next;
		posb = S.polynomials[b].polynomial->next;//将posb置回原位
	}
	CopyPoly(S, 10, c);
	DeletePoly(S.polynomials[10]);
	return OK;
}

status DividePolyAndMoluloOperationPoly(Storage &S, int a, int b, int c,int d) {//多项式a除以多项式b，存到c处,取模存到d处
	unit* posa, *posb, *temp, *store, *temp2;
	double prop;
	int adexp;
	if (!S.polynomials[a].polynomial || !S.polynomials[b].polynomial) { printf_s("NOT EXIST\n"); return ERROR; }//检查是否存在
	if (!S.polynomials[b].polynomial->next) { printf_s("Can't Divide 0\n"); return OVERFLOW; }
	CreatPoly(S.polynomials[11]);
	CreatPoly(S.polynomials[12]);
	CreatPoly(S.polynomials[13]);//用于储存除法结果
	CopyPoly(S, a, 11);
	CopyPoly(S, b, 12);
	posa = S.polynomials[11].polynomial->next;
	posb = S.polynomials[12].polynomial->next;
	store = S.polynomials[13].polynomial;
	while (posa&&posa->exp >= posb->exp){
		temp = new unit[1];
		temp->coef= posa->coef / posb->coef;
		temp->exp = posa->exp - posb->exp;
		temp->next = NULL;

		prop = posa->coef / posb->coef;//计算比例
		adexp= posa->exp - posb->exp;

		store->next = temp;//给储存链表13添加节点
		store = store->next;

		while (posb) {
			posb->coef =prop*posb->coef;
			posb->exp = posb->exp + adexp;
			posb = posb->next;
		}//倍乘b
		MinusPoly(S, 11, 12,11);
		CopyPoly(S, b, 12);
		posb = S.polynomials[12].polynomial->next;//让链a的复制品减去首位

		posa = S.polynomials[11].polynomial->next;
	}
	CopyPoly(S, 13, c);
	CopyPoly(S, 11, d);
	DeletePoly(S.polynomials[11]);
	DeletePoly(S.polynomials[12]);
	DeletePoly(S.polynomials[13]);
	return OK;
}

status GetValue(Storage S, int poly, double xvalue, double &yvalue) {
	unit *pos;
	if (poly < 0 || poly > 9) return OVERFLOW;
	yvalue = 0;
	if (!S.polynomials[poly].polynomial) { printf_s("NOT EXIST\n"); return ERROR; }//检查是否存在
	pos = S.polynomials[poly].polynomial;
	while (pos) {
		yvalue += pos->coef*pow(xvalue, pos->exp);
		pos = pos->next;
	}
	return OK;
}

status DifferentialPoly(Storage &S, int a, int b) {//将多项式a微分，存到b处
	unit *posa, *posb,*temp;
	if (!S.polynomials[a].polynomial) { printf_s("NOT EXIST\n"); return ERROR; }//检查是否存在
	CreatPoly(S.polynomials[10]);
	posa = S.polynomials[a].polynomial->next;
	posb = S.polynomials[10].polynomial;
	while (posa && posa->exp>0){
		temp = new unit[1];
		temp->coef = posa->coef*posa->exp;
		temp->exp = posa->exp - 1;
		temp->next = NULL;

		posb->next = temp;
		posb = posb->next;
		posa = posa->next;
	}
	CopyPoly(S, 10, b);
	DeletePoly(S.polynomials[10]);
	return OK;
}

status IndefiniteIntegralPoly(Storage &S,int a,int b) {//将式a不定积分，存到b处
	unit *posa, *posb, *temp;
	if (!S.polynomials[a].polynomial) { printf_s("NOT EXIST\n"); return ERROR; }//检查是否存在
	CreatPoly(S.polynomials[10]);
	posa = S.polynomials[a].polynomial->next;
	posb = S.polynomials[10].polynomial;
	while (posa) {
		temp = new unit[1];
		temp->coef = posa->coef/(posa->exp+1);
		temp->exp = posa->exp + 1;
		temp->next = NULL;

		posb->next = temp;
		posb = posb->next;
		posa = posa->next;
	}
	CopyPoly(S, 10, b);
	DeletePoly(S.polynomials[10]);
	return OK;
}

status DefiniteIntegralPoly(Storage &S,int a, double start,double end, double &value) {//将式a定积分，存到b处
	double startval, endval;
	IndefiniteIntegralPoly(S, a, 10);
	GetValue(S, a, start, startval);
	GetValue(S, a, end, endval);
	value = endval - startval;
	return OK;
}

status DividePoly(Storage &S,int a,int b,int c) {//多项式a除以多项式b，存到c处
	int Status;
	Status=DividePolyAndMoluloOperationPoly(S, a, b, c, 11);
	if (Status)return OK;
	else return ERROR;
}

status MoluloOperationPoly(Storage &S,int a,int b,int c) {//多项式a模多项式b，存到c处
	int Status;
	Status = DividePolyAndMoluloOperationPoly(S, a, b, 13, c);
	if (Status)return OK;
	else return ERROR;
}

status GreatestCommonPoly(Storage &S, int a, int b, int c) {//将多项式a与b的最大公约数存到c处
	unit *p, *q;
	CreatPoly(S.polynomials[14]);
	CreatPoly(S.polynomials[15]);
	CopyPoly(S, a, 14);
	CopyPoly(S, b, 15);
	p = S.polynomials[14].polynomial;
	q = S.polynomials[15].polynomial;
	while (p->next&&q->next){
		MoluloOperationPoly(S, 14, 15, 14);
		p = S.polynomials[14].polynomial;
		if (p->next == NULL) break;
		MoluloOperationPoly(S, 15, 14, 15);
		q = S.polynomials[15].polynomial;
	}
	if (p->next == NULL) CopyPoly(S, 15, c);
	if (q->next == NULL) CopyPoly(S, 14, c);
	DeletePoly(S.polynomials[14]);
	DeletePoly(S.polynomials[15]);
	return OK;
}

status LeastCommonPoly(Storage &S, int a, int b, int c) {//将多项式a与b的最小公倍数存到c处
	unit *p, *q;
	CreatPoly(S.polynomials[16]);
	CreatPoly(S.polynomials[17]);
	GreatestCommonPoly(S, a, b, 16);
	MultiplyPoly(S, a, b, 17);
	DividePoly(S, 17, 16, 17);
	CopyPoly(S, 17, c);
	DeletePoly(S.polynomials[16]);
	DeletePoly(S.polynomials[17]);
	return OK;
}

status PowerPoly(Storage &S, int a,int b, int power) {//将多项式a进行power次方，存到b处，采用快速幂算法
	unit *temp;
	int i;
	CreatPoly(S.polynomials[16]);//存结果
	CreatPoly(S.polynomials[17]);//存base
	CopyPoly(S, a, 16);
	temp = new unit[1];
	temp->coef = 1;
	temp->exp = 0;
	temp->next = NULL;
	S.polynomials[16].polynomial->next = temp;//让第16号存储位置存放1.0000
	CopyPoly(S, a, 17);
	while (power != 0) {
		if (power & 1 != 0) {
			MultiplyPoly(S, 16, 17, 16);
		}
		MultiplyPoly(S, 17, 17, 17);
		power >>= 1;
	}
	CopyPoly(S, 16, b);
	DeletePoly(S.polynomials[16]);
	DeletePoly(S.polynomials[17]);
	return OK;
}

void PrintGUI() {
	printf("-------------MANU------------\n");
	printf("----------0.退出程序----------\n");
	printf("----------1.创建--------------\n");
	printf("----------2.显示--------------\n");
	printf("----------3.复制--------------\n");
	printf("----------4.求和--------------\n");
	printf("----------5.求差--------------\n");
	printf("----------6.求积--------------\n");
	printf("----------7.求值--------------\n");
	printf("----------8.销毁--------------\n");
	printf("----------9.清空--------------\n");
	printf("---------10.微分--------------\n");
	printf("---------11.不定积分----------\n");
	printf("---------12.定积分------------\n");
	printf("---------13.最大公约式--------\n");
	printf("---------14.最小公倍式--------\n");
	printf("---------15.除法--------------\n");
	printf("---------16.取模--------------\n");
	printf("---------17.乘方--------------\n");
	return;
}

void main() {
	Storage S;
	double xvalue,yvalue,startval,endval,res;
	int opernum,pos,pos2,pos3,power;
	InitStorage(S);
	while (1)
	{
		PrintGUI();
		scanf("%d", &opernum);
		switch (opernum) {
		case 0:
			exit(0);
		case 1:
			scanf("%d", &pos);
			CreatPoly(S.polynomials[pos]);
			InputPoly(S, pos);
			break;
		case 2:
			scanf("%d", &pos);
			PrintPoly(S, pos);
			break;
		case 3:
			scanf("%d %d", &pos,&pos2);
			CopyPoly(S, pos, pos2);
			break;
		case 4:
			scanf("%d %d %d", &pos, &pos2,&pos3);
			AddPoly(S, pos, pos2, pos3);
			break;
		case 5:
			scanf("%d %d %d", &pos, &pos2, &pos3);
			MinusPoly(S, pos, pos2, pos3);
			break;
		case 6:
			scanf("%d %d %d", &pos, &pos2, &pos3);
			MultiplyPoly(S, pos, pos2, pos3);
			break;
		case 7:
			scanf("%d", &pos);
			scanf("%lf", &xvalue);
			GetValue(S, pos, xvalue, yvalue);
			printf("%lf\n", yvalue);
			break;
		case 8:
			scanf("%d", &pos);
			DeletePoly(S.polynomials[pos]);
			break;
		case 9:
			scanf("%d", &pos);
			ClearPoly(S.polynomials[pos]);
			break;
		case 10:
			scanf("%d %d", &pos,&pos2);
			DifferentialPoly(S, pos, pos2);
			break;
		case 11:
			scanf("%d %d", &pos, &pos2);
			IndefiniteIntegralPoly(S, pos, pos2);
			break;
		case 12:
			scanf("%d %lf %lf", &pos,&startval,&endval);
			DefiniteIntegralPoly(S, pos, startval, endval, res);
			printf("%.4lf\n", res);
			break;
		case 13:
			scanf("%d %d %d", &pos, &pos2, &pos3);
			GreatestCommonPoly(S, pos, pos2, pos3);
			break;
		case 14:
			scanf("%d %d %d", &pos, &pos2, &pos3);
			LeastCommonPoly(S, pos, pos2, pos3);
			break;
		case 15:
			scanf("%d %d %d", &pos, &pos2, &pos3);
			DividePoly(S, pos, pos2, pos3);
			break;
		case 16:
			scanf("%d %d %d", &pos, &pos2, &pos3);
			MoluloOperationPoly(S, pos, pos2, pos3);
			break;
		case 17:
			scanf("%d %d %d", &pos, &pos2,&power);
			PowerPoly(S, pos, pos2, power);
			break;
		}
	}
}

//未完成：部分函数输入越界判断