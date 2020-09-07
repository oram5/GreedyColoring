//VS-17
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> 

typedef struct point
{
	int min, max;
}Point;

typedef struct edges
{
	Point data;
	int edg_loc;
	struct edges *next;
}Edges;

typedef struct interval
{
	Point data;
	Edges * Ed;
	int degree;
	int color;
}Interval;

void GreedyColoring();
void scanArray(Interval *arr, int size);
void merge(Interval *arr, int l, int m, int r);
void mergeSort(Interval *arr, int l, int r);
void ConEdges(Interval *arr, int size);
void ColorCods(Interval *arr, int size);
void insert(Edges **Head, Point data);
void ComplementArr(Interval *arr, Interval *comparr, int size);
void printArray(Interval *arr, int size);
void Printcolor(Interval *arr, int size);

void main()
{
	GreedyColoring();
	system("pause");
}

void GreedyColoring()
{
	int k;
	printf("Please input k\n");
	scanf_s("%d", &k);
	Interval *arr = (Interval*)calloc(k, sizeof(Interval));
	Interval *comparr = (Interval*)calloc(k, sizeof(Interval));
	scanArray(arr, k);
	printArray(arr, k);
	mergeSort(arr, 0, k - 1);
	ConEdges(arr, k);
	ColorCods(arr, k);
	ComplementArr(arr, comparr, k);
	Printcolor(arr, k);
}

void insert(Edges **Head, Point data)
{
	Edges * NewEd = (Edges*)malloc(sizeof(Edges));
	NewEd->data = data;
	NewEd->next = *Head;
	*Head = NewEd;
}

void Printcolor(Interval *arr, int size)
{
	int i, j, t, count = 1, flag = 0;
	for (i = 0; i < size; i++) if (arr[i].color > count)count = arr[i].color;
	printf("Optional Coloring: ");
	for (i = 1; i <= count; i++)
	{
		printf("{");
		for (j = 0; j < size; j++)
		{
			flag = 0;
			if (arr[j].color == i)
			{
				printf("[%d,%d]", arr[j].data.min, arr[j].data.max);
				for (t = j + 1; t < size; t++) {
					if (arr[t].color == i) {
						flag = 1;
						break;
					}
				}
				if (flag == 1) printf(",");
			}
		}
		printf("} = %d", i);
		if (i < count) printf(", ");
		else printf("\n");
	}
}

void ComplementArr(Interval *arr, Interval *comparr, int size)
{
	int i, j, flag, edges = 0, maxdeg = 0, mindeg = 0;
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++){
			Edges *temp = arr[i].Ed;
			flag = 0;
			if (j != i) {
				if (temp == NULL) {
					insert(&comparr[i].Ed, arr[j].data);
					comparr[i].degree++;
					edges++;
				}
				else{
					while (temp) {
						if ((temp->data.max == arr[j].data.max) && (temp->data.min == arr[j].data.min)){
							flag = 0;
							break;
						}
						else{
							flag = 1;
							temp = temp->next;
						}
					}
				}
			}
			if (flag) {
				insert(&comparr[i].Ed, arr[j].data);
				comparr[i].degree++;
				edges++;
			}
			if (maxdeg < comparr[i].degree)
				maxdeg = comparr[i].degree;
		}
	}
	mindeg = maxdeg;
	for (i = 0; i < size; i++) {
		if (mindeg > comparr[i].degree)
			mindeg = comparr[i].degree;
	}
	printf("G's Complement Edges = %d\nMaximum Degree of G's Complement = %d\nMinimum Degree of G's Complement = %d\n", edges / 2, maxdeg, mindeg);
}

void ConEdges(Interval *arr, int size)
{
	int i, j, flag = 0, edges = 0, maxdeg = 0, mindeg = 0;
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			if (i != j) {
				if ((arr[i].data.min >= arr[j].data.min && arr[i].data.min <= arr[j].data.max) || (arr[j].data.min >= arr[i].data.min && arr[j].data.min <= arr[i].data.max)) {
					insert(&arr[i].Ed, arr[j].data);
					arr[i].degree++;
					arr[i].Ed->edg_loc = j;
					flag = 1;
					edges++;
				}
			}
		}
		if (maxdeg < arr[i].degree) maxdeg = arr[i].degree;
	}
	mindeg = maxdeg;
	for (i = 0; i < size; i++) {
		if (mindeg > arr[i].degree)
			mindeg = arr[i].degree;
	}
	printf("G Edges = %d\nMaximum Degree of G = %d\nMinimum Degree of G = %d\n", edges / 2, maxdeg, mindeg);
}

void ColorCods(Interval * arr, int size)
{
	int i, j, flag = 1, *AssistArr;
	AssistArr = (int*)malloc(sizeof(int) * size);
	for (i = 0; i < size; i++) AssistArr[i] = 1;
	arr[0].color = 1;
	for (i = 1; i < size; i++) {
		Edges *temp = arr[i].Ed;
		while (temp) {
			if (arr[temp->edg_loc].color != 0) AssistArr[arr[temp->edg_loc].color - 1] = 0;
			temp = temp->next;
		}
		for (j = 0; j < size; j++) {
			if (AssistArr[j] == 1) {
				arr[i].color = j + 1;
				if (j + 1 > flag) flag = j + 1;
				break;
			}
		}
		for (j = 0; j < size; j++) AssistArr[j] = 1;
	}
	printf("Chromatic Number of G = %d\n", flag);
}

void scanArray(Interval *arr, int size)
{
	system("cls");
	int i;
	printf("You will now be asked to insert a family of %d intervals:\n", size);
	for (i = 0; i < size; i++) {
		printf("%dth Interval: ", i + 1);
		scanf_s("%d %d", &arr[i].data.min, &arr[i].data.max);
	}
}

void printArray(Interval *arr, int size)
{
	int i;
	printf("The Intervals family is:\n");
	for (i = 0; i < size; i++) {
		printf("[%d,%d]", arr[i].data.min, arr[i].data.max);
		if (i < size - 1) printf(",");
	}
	printf("\n");
}

void merge(Interval *arr, int l, int m, int r)
{
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;
	Interval *L = (Interval*)calloc(n1, sizeof(Interval));
	Interval *R = (Interval*)calloc(n2, sizeof(Interval));
	for (i = 0; i < n1; i++) L[i] = arr[l + i];
	for (j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
	i = 0;
	j = 0;
	k = l;
	while (i < n1 && j < n2) {
		if (L[i].data.min <= R[j].data.min) {
			arr[k] = L[i];
			i++;
		}
		else {
			arr[k] = R[j];
			j++;
		}
		k++;
	}
	while (i < n1) {
		arr[k] = L[i];
		i++;
		k++;
	}
	while (j < n2) {
		arr[k] = R[j];
		j++;
		k++;
	}
	free(L);
	free(R);
}

void mergeSort(Interval *arr, int l, int r)
{
	if (l < r) {
		int m = l + (r - l) / 2;
		mergeSort(arr, l, m);
		mergeSort(arr, m + 1, r);
		merge(arr, l, m, r);
	}
}