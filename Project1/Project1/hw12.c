#include <stdio.h>
#include <stdlib.h>
#define MAX_VERTICES 30
#define MIN2(x,y) ((x) < (y) ? (x) : (y))
typedef struct node *node_pointer;
typedef struct Node *sptr;
typedef struct {
	int count;
	sptr link;
}STACK;
typedef struct Node {
	int e1, e2;
	sptr link;
}STACK_NODE;
typedef struct node {
	int vertex;
	node_pointer link;
};

node_pointer graph[MAX_VERTICES] = { NULL };
short int dfn[MAX_VERTICES];
short int low[MAX_VERTICES];
int num;
STACK* pStack;

void init();
void bicon(int, int);
node_pointer newNode(void);
void Exit(STACK*);
void Push(STACK*);
void Pop(STACK*, int*, int*);

void main(void) {
	//파일을 오픈하여 파일을 읽어 adjacency list에 저장한다.
	FILE *ip;
	node_pointer pNew, pCurr;
	int n, hn, var;
	ip = fopen("input.txt", "r");
	fscanf(ip, "%d\n", &n);

	for (int i = 0; i < n; i++) {
		hn = fgetc(ip);
		hn = hn - 48;
		while (1) {
			var = fgetc(ip);
			if (var == 10 || var == -1) {
				break;
			}
			else if (var == 32) {
				continue;
			}
			else {
				pNew = newNode();
				pNew->vertex = var - 48;
				pNew->link = NULL;
				if (graph[hn] == NULL) {
					graph[hn] = pNew;
					pCurr = pNew;
				}
				else {
					pCurr->link = pNew;
					pCurr = pNew;
				}
			}
		}

	}
	//stack 초기화
	pStack = (STACK*)malloc(sizeof(STACK));
	pStack->link = NULL;
	pStack->count = 0;

	init();
	bicon(3, -1);
	free(pStack);
}

void init(void) {
	int i;
	for (i = 0; i < MAX_VERTICES; i++) {
		dfn[i] = low[i] = -1;
	}
	num = 0;
}

void bicon(int u, int v) {
	//The function bicon assumes 
	//that the connected graph has at least two vertices.
	node_pointer ptr;
	int w, x, y;
	dfn[u] = low[u] = num++;
	for (ptr = graph[u]; ptr; ptr = ptr->link) {
		w = ptr->vertex;
		if (v != w && dfn[w] < dfn[u])
			Push(pStack, u, w); /*add edge to stack*/
		if (dfn[w] < 0) {/*w is an unvisited vertex*/
			bicon(w, u);
			low[u] = MIN2(low[u], low[w]);
			if (low[w] >= dfn[u]) {
				printf("New biconnected component: ");
				do { /*delete edge from stack*/
					Pop(pStack, &x, &y);
					printf(" <%d, %d>", x, y);
				} while (!((x == u) && (y == w)));
				printf("\n");
			}
		}
		else if (w != v) low[u] = MIN2(low[u], dfn[w]);
	}
}

node_pointer newNode(void) {
	node_pointer node;
	node = malloc(sizeof(struct node));
	return node;
}

void Push(STACK * pStack, int e1, int e2) {
	STACK_NODE *pNew;
	pNew = (STACK_NODE*)malloc(sizeof(STACK_NODE));

	pNew->link = pStack->link;
	pNew->e1 = e1;
	pNew->e2 = e2;
	pStack->link = pNew;
	pStack->count++;
}

void Pop(STACK* pStack, int *x, int *y) {
	STACK_NODE *temp;
	if (pStack->link) {
		temp = pStack->link;
		*x = temp->e1;
		*y = temp->e2;
		pStack->link = pStack->link->link;
		pStack->count--;
		free(temp);
	}
}