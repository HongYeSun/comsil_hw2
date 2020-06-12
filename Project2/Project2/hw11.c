#include <stdio.h>
#define MAX_TERMS 100
#define INSERT_MODE 0
#define DELETE_MODE 1
typedef struct treeNode *treePtr;
typedef struct treeNode {
	treePtr lchild;
	int key;
	treePtr rchild;
};

void inorder(treePtr);
void preorder(treePtr);
void insert(treePtr*, int);
treePtr modifiedSearch(treePtr, int, int);
void delete(treePtr*, int);
void rangeSearch(treePtr, int, int);
int getNodeNum(treePtr);

void main(void) {
	int i, j, n, m, A[MAX_TERMS], D[MAX_TERMS], k1, k2;
	treePtr tree = NULL;
	FILE *ip;

	ip = fopen("input2.txt", "r");
	/*�Է����� ������ ����� ������ n��, ���� ���� ������ m��, �����Ͽ���
	�� �������� D��, ���� ����� ������ k1, k2�� ����
	������� binary search tree�� ����*/
	if (!fscanf(ip, "%d\n", &n)) {
		printf("empty!\n");
		return;
	}

	for (i = 0; i < n; i++) {
		fscanf(ip, "%d\n", &A[i]);
		insert(&tree, A[i]);
	}
	inorder(tree);
	printf("\n");
	preorder(tree);
	printf("\n");

	/*������ binary search tree�� inorder�� preorder�� ����Ͽ� �ùٸ���
	Ʈ���� �����Ǿ����� Ȯ��
	delete�� �������� ������ binary search tree���� ������ ��, �ùٸ���
	�����Ǿ����� Ȯ���ϱ� ���� inorder�� preorder�� ���*/
	fscanf(ip, "%d\n", &m);
	if (m == 0) {
		printf("delete nothing\n");
	}
	else {
		for (j = 0; j < m; j++) {
			fscanf(ip, "%d\n", &D[j]);
			delete(&tree, D[j]);
			inorder(tree);
			printf("\n");
			preorder(tree);
			printf("\n");
		}
	}


	if (!fscanf(ip, "%d\n %d", &k1, &k2)) {
		printf("search nothing\n");
	}
	else {//������ Ʈ���� ��ȸ�ϸ� k1, k2 ������ ������ ������������ ���
		if (k1 > k2) {
			n = k1;
			k1 = k2;
			k2 = n;
		}
		rangeSearch(tree, k1, k2);
	}
}

void insert(treePtr* node, int num) {
	/*If num is in the tree pointed at by node do nothing;
	otherwise add a new node with data = num*/
	treePtr ptr, temp;
	temp = modifiedSearch(*node, num, INSERT_MODE);

	if (temp || !(*node)) {
		/*num is not in the tree*/
		ptr = (treePtr)malloc(sizeof(struct treeNode));
		ptr->key = num;
		ptr->lchild = ptr->rchild = NULL;
		if (*node) {	/*insert as child of temp*/
			if (num < temp->key)
				temp->lchild = ptr;
			else
				temp->rchild = ptr;
		}
		else
			*node = ptr;
	}
}

treePtr modifiedSearch(treePtr tree, int key, int mode) {
	/*If num is presented, return NULL.
	Otherwise, return a pointer to the last node
	of the tree that was encountered during the search.*/
	treePtr temp = tree;

	while (tree) {
		if (key == tree->key) {
			if (mode == INSERT_MODE)
				return NULL;
			else
				return temp;
		}
		if (key < tree->key) {
			temp = tree;
			tree = tree->lchild;
		}
		else {
			temp = tree;
			tree = tree->rchild;
		}
	}
	if (mode == INSERT_MODE)
		return temp;
	else
		return NULL;
}

void inorder(treePtr ptr) {
	if (ptr) {
		inorder(ptr->lchild);
		printf("%d ", ptr->key);
		inorder(ptr->rchild);
	}
}

void preorder(treePtr ptr) {
	if (ptr) {
		printf("%d ", ptr->key);
		preorder(ptr->lchild);
		preorder(ptr->rchild);
	}
}

void delete(treePtr* node, int num) {
	treePtr parent, temp;

	parent = modifiedSearch(*node, num, DELETE_MODE);
	if (parent == NULL) {
		printf("%d is not in the tree\n", num);
		return;
	}

	if (num < parent->key) {
		temp = parent->lchild;
	}
	else {
		temp = parent->rchild;
	}

	if (temp->lchild == NULL && temp->rchild == NULL) {
		if (num < parent->key) {
			parent->lchild = NULL;
		}
		else {
			parent->rchild = NULL;
		}
	}
	else if (temp->lchild != NULL && temp->rchild == NULL) {
		if (num < parent->key) {
			parent->lchild = temp->lchild;
		}
		else {
			parent->rchild = temp->lchild;
		}
	}
	else if (temp->lchild == NULL && temp->rchild != NULL) {
		if (num < parent->key) {
			parent->lchild = temp->rchild;
		}
		else {
			parent->rchild = temp->rchild;
		}
	}
	else {
		int lcount = getNodeNum(temp->lchild);
		int rcount = getNodeNum(temp->rchild);
		int ran;
		treePtr pleaf, leaf;
		//ran�� 0�̸� ���� ����Ʈ������
		//1�̸� ������ ����Ʈ������ ��� ��������
		if (lcount == rcount)
			ran = rand() % 2;
		else if (lcount > rcount)
			ran = 0;
		else
			ran = 1;

		if (ran == 0) {
			pleaf = temp;
			leaf = temp->lchild;
			while (leaf->rchild != NULL) {
				pleaf = leaf;
				leaf = leaf->rchild;
			}
			if (pleaf == temp) {
				pleaf->lchild = leaf->lchild;
			}
			else {
				pleaf->rchild = leaf->lchild;
			}
		}
		else {
			pleaf = temp;
			leaf = temp->rchild;
			while (leaf->lchild != NULL) {
				pleaf = leaf;
				leaf = leaf->lchild;
			}if (pleaf == temp) {
				pleaf->rchild = leaf->rchild;
			}
			else {
				pleaf->lchild = leaf->rchild;
			}
		}
		if (num < parent->key) {
			parent->lchild = leaf;
		}
		else {
			parent->rchild = leaf;
		}
		leaf->lchild = temp->lchild;
		leaf->rchild = temp->rchild;
	}
	free(temp);
}

int getNodeNum(treePtr node) {
	if (node == NULL) return 0;
	int result = 1;
	result += getNodeNum(node->lchild) + getNodeNum(node->rchild);
	return result;
}

void rangeSearch(treePtr tree, int k1, int k2) {
	if (tree) {
		rangeSearch(tree->lchild, k1, k2);
		if (tree->key >= k1 && tree->key <= k2) {
			printf("%d ", tree->key);
		}
		rangeSearch(tree->rchild, k1, k2);
	}
}