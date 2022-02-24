#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int MAX_KEY;
int MIN_KEY;
int MID;
struct bnode {
    struct bnode **child;
    struct bnode *parent;
    int size;
    int *key;
};
struct bnode *broot;

void binit(int N) {
    MAX_KEY = N - 1;
    MIN_KEY = (N%2) ? N/2+1 : N/2;
    MID = N/2;
    broot = NULL;
}
struct bnode *bCreate() {
    struct bnode *n = (struct bnode *)malloc(sizeof(struct bnode));
    n->parent = NULL;
    n->size = 0;
    //比MAX_KEY多一个，方便先插入，后判断是否需要分裂
    n->key = (int *)calloc((MAX_KEY+1), sizeof(int));
    n->child = (struct bnode **)calloc((MAX_KEY+2), sizeof(struct bnode *));
    return n;
}
struct bnode *bSearch(int key, struct bnode *n) {
    if (!n) return NULL;
    if (key<n->key[0]) return bSearch(key, n->child[0]);
    for (int i = 0;i < n->size;i++) {
        if (n->key[i]==key) return n;
        else if (key<n->key[i+1]) return bSearch(key, n->child[i+1]);
    }
    return bSearch(key, n->child[n->size]);
}
void bSplit(struct bnode *n) {
    struct bnode *p = NULL, *l = NULL;
    while (n->size>MAX_KEY) {
        int len = n->size;
        l = bCreate();
        //把n的右边一半复制给l
        memcpy(l->key, n->key+MID+1, (len-MID-1) * sizeof(int));
        memcpy(l->child, n->child+MID+1, (len-MID) * sizeof(struct bnode *));
        l->size = len-MID-1;
        n->size = MID;
        l->parent = n->parent;
        p = n->parent;
        //没有父节点，则总共一个节点，新建一个根节点
        if (!p) {
            p = bCreate();
            broot = p;
            p->child[0] = n;
            p->child[1] = l;
            n->parent = p;
            l->parent = p;
            p->key[0] = n->key[MID];
            p->size = 1;
        } else {
            int i;
            //把父节点在MID右边的往右移一位
            for (i = p->size;i > 0;i--) {
                if (n->key[MID]<p->key[i-1]) {
                    p->key[i] = p->key[i-1];
                    p->child[i+1] = p->child[i];
                } else break;
            }
            p->key[i] = n->key[MID];
            p->child[i+1] = l;
            l->parent = p;
            p->size++;
        }
        //把n复制的部分设置为0
        memset(n->key+MID, 0, (len-MID) * sizeof(int));
        memset(n->child+MID+1, 0, (len-MID) * sizeof(struct bnode *));
        for (int i = 0;i <= l->size;i++) if (l->child[i]) l->child[i]->parent = l;
        //父节点多了一个元素，从父节点处开始递归
        n = p;
    }
}
void bInsert(int key) {
    //根节点为空时，创建一个节点
    if (broot==NULL) {
        struct bnode *n = bCreate();
        n->size = 1;
        n->key[0] = key;
        n->parent = NULL;
        broot = n;
        return;
    }
    struct bnode *q = broot;
    int i;
    //找到要插入的位置
    while (q) {
        //找到插入值所在的区间
        for (i = 0;i < q->size;i++) {
            if (key==q->key[i]) return;
            else if (key<q->key[i]) break;
        }
        //往下找相对应的子节点
        if (q->child[i]) q = q->child[i];
        else break;
    }
    //i和i之后的元素往后移一位，i为key值
    for (int j = q->size;j > i;j--) q->key[j] = q->key[j-1];
    q->key[i] = key;
    q->size++;
    //如果插入后元素个数超出上限，进行分裂
    if (q->size>MAX_KEY) bSplit(q);
}
void bMerge(struct bnode *n);
void bDoMerge(struct bnode *l, struct bnode *r, int mid) {
    struct bnode *p = l->parent;
    //合并父节点的那个元素
    l->key[l->size++] = p->key[mid];
    //合并右边的元素
    memcpy(l->key+l->size, r->key, r->size * sizeof(int));
    memcpy(l->child+l->size, r->child, (r->size+1) * sizeof(struct bnode *));
    int i;
    for (i = 0;i <= r->size;i++) if (r->child[i]) r->child[i]->parent = l;
    l->size += r->size;
    //处理父节点键值和孩子的序号
    for (i = mid;i <= p->size-1;i++) {
        p->key[i] = p->key[i+1];
        p->child[i+1] = p->child[i+2];
    }
    p->key[i] = 0;
    p->child[i+1] = NULL;
    p->size--;
    free((void *)r->key);
    free((void *)r->child);
    free(r);
    //如果父节点也小于最小要求了，递归合并
    if (p->size<MIN_KEY) bMerge(p);
}
void bMerge(struct bnode *n) {
    int i, mid;
    struct bnode *p = n->parent;
    struct bnode *l = NULL, *r = NULL;
    //如果n是根节点
    if (!p) {
        //如果根节点删完了
        if (n->size==0) {
            //如果有那一个孩子，就让孩子成为新的根节点
            if (n->child[0]) {
                broot = n->child[0];
                n->child[0]->parent = NULL;
            } else broot = NULL;//没有孩子就全没了
            free((void *)n->key);
            free((void *)n->child);
            free(n);
        }
        //如果根节点没删完
        return;
    }
    //如果n不是根节点，找到其在父节点的位置
    for (i = 0;i <= p->size;i++) if (n==p->child[i]) break;
    if (i>p->size) return;//没找到要合并的节点
    //如果该节点位于其父节点最右边，找左兄弟
    if (i==p->size) {
        mid = i - 1;
        l = p->child[mid];
        //左兄弟也没有多余的节点，直接合并
        if (n->size+l->size+1<=MAX_KEY) return bDoMerge(l, n, mid);
        if (l->size==1) return;//叶子结点
        //左兄弟有多余的节点，拿一个过来
        for (int j = n->size;j > 0;j--) {
            n->key[j] = n->key[j-1];
            n->child[j+1] = n->child[j];
        }
        n->child[1] = n->child[0];
        n->key[0] = p->key[mid];
        n->size++;
        n->child[0] = l->child[l->size];
        if (l->child[l->size]) l->child[l->size]->parent = n;
        p->key[mid] = l->key[l->size-1];
        l->key[l->size-1] = 0;
        l->child[l->size] = NULL;
        l->size--;
        return;
    }
    //找右兄弟
    mid = i;
    r = p->child[mid+1];
    //右兄弟也没有多余的节点，直接合并
    if (n->size+r->size+1<=MAX_KEY) return bDoMerge(n, r, mid);
    if (r->size==1) return;//叶子结点
    //右兄弟有多余的节点，拿一个过来
    n->size++;
    n->key[n->size-1] = p->key[mid];
    n->child[n->size] = r->child[0];
    if (r->child[0]) r->child[0]->parent = n;
    p->key[mid] = r->key[0];
    for (int j = 0;j < r->size;j++) {
        r->key[j] = r->key[j+1];
        r->child[j] = r->child[j+1];
    }
    r->child[r->size] = NULL;
    r->size--;
}
void bDoDelete (struct bnode *n, int i) {
    struct bnode *q = n;
    struct bnode *child = q->child[i];
    //寻找直接后继
    while (child) {
        n = child;
        child = n->child[child->size];
    }
    //有直接后继，删除直接后继
    if (q!=n) q->key[i] = n->key[n->size-1];
    //无直接后继，i和i之后的左移一位
    else for (int j = i;j < q->size;j++) q->key[j] = q->key[j+1];
    n->key[n->size-1] = 0;
    n->size--;
    //删除后元素个数小于最小要求，进行合并
    if (n->size<MIN_KEY) bMerge(n);
}
void bDelete(int key) {
    struct bnode *n = broot;
    int i;
    //找要删除的节点
    while (n) {
        for (i = 0;i < n->size;i++) {
            //找到了
            if (key==n->key[i]) {
                bDoDelete(n, i);
                return;
            } else if (key<n->key[i]) break;//没找到
        }
        n = n->child[i];
    }
    return;
}
void bpreorder(struct bnode *n) {
        if(n == NULL) return;
        printf("(");
        printf("%d", n->key[0]);
        for(int i = 1; i < n->size; i++) printf(", %d", n->key[i]);
        printf(")");
        for(int i = 0; i <= n->size; i++) bpreorder(n->child[i]);
}

int main() {
    binit(3);
    int n;
    printf("Please enter the number of operations you want to perform:");
    scanf("%d", &n);
    while (n--) {
        int opcode, key;
        printf("Please enter the opcode and value(%d sets of data left):", n+1);
        scanf("%d%d", &opcode, &key);
        struct bnode *q;
        switch (opcode) {
        case 1:
            q = bSearch(key, broot);
            if (q) printf("Found!\n");
            else printf("Not found!\n");
            break;
        case 2:
            bInsert(key);
            bpreorder(broot);
            printf("\n");
            break;
        case 3:
            bDelete(key);
            bpreorder(broot);
            printf("\n");
            break;
        default:
            printf("Wrong opcode!\n");
            n++;
            break;
        }
    }
}