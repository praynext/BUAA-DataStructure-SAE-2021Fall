#include <stdio.h>
#include <stdlib.h>

struct avlnode {
    struct avlnode *lchild;
    struct avlnode *rchild;
    int value;
    int height;
};
struct avlnode *avlroot;

int getmax(int a, int b) {return (a>b)?a:b;}
int getheight(struct avlnode *n) {return (n==NULL)?0:n->height;}
void refresh(struct avlnode **n) {(*n)->height = getmax(getheight((*n)->lchild), getheight((*n)->rchild)) + 1;}
void avllrotate(struct avlnode **n) {
    struct avlnode *temp = (*n)->rchild;
    (*n)->rchild = temp->lchild;
    temp->lchild = (*n);
    refresh(n);
    refresh(&temp);
    (*n) = temp;
}
void avlrrotate(struct avlnode **n) {
    struct avlnode *temp = (*n)->lchild;
    (*n)->lchild = temp->rchild;
    temp->rchild = (*n);
    refresh(n);
    refresh(&temp);
    (*n) = temp;
}
void LL_rotate(struct avlnode **n) {avlrrotate(n);}
void LR_rotate(struct avlnode **n) {avllrotate(&((*n)->lchild)); avlrrotate(n);}
void RR_rotate(struct avlnode **n) {avllrotate(n);}
void RL_rotate(struct avlnode **n) {avlrrotate(&((*n)->rchild)); avllrotate(n);}
struct avlnode *avlSearch(int key, struct avlnode *n) {
    if (n==NULL) return NULL;
    if (key==n->value) return n;
    else if (key<n->value) return avlSearch(key, n->lchild);
    else return avlSearch(key, n->rchild);
}
struct avlnode *avlInsert(int key, struct avlnode *n) {
    if (n==NULL) {
        n = (struct avlnode *)malloc(sizeof(struct avlnode));
        n->lchild = n->rchild = NULL;
        n->value = key;
        n->height = 1;
        return n;
    }
    if (key==n->value) return n;
    else if (key<n->value) {
        n->lchild = avlInsert(key, n->lchild);
        //插入了左子树
        if (getheight(n->lchild)-getheight(n->rchild)==2) {
            //插入了左子树的左边
            if (key<=n->lchild->value) LL_rotate(&n);
            //插入了左子树的右边
            else LR_rotate(&n);
        }
        refresh(&n);
        return n;
    }
    else {
        n->rchild = avlInsert(key, n->rchild);
        //插入了右子树
        if (getheight(n->rchild)-getheight(n->lchild)==2) {
            //插入了右子树的右边
            if (key>=n->rchild->value) RR_rotate(&n);
            //插入了右子树的左边
            else RL_rotate(&n);
        }
        refresh(&n);
        return n;
    }
}
struct avlnode *avlDelete(int key, struct avlnode *n) {
    if (n==NULL) return NULL;
    //往左子树查找，删除后讨论右子树是否比左子树高2
    if (key<n->value) {
        n->lchild = avlDelete(key, n->lchild);
        //讨论右子树的左右子树谁更高来决定旋转方式
        if (getheight(n->rchild)-getheight(n->lchild)==2) {
            if (getheight(n->rchild->rchild)>=getheight(n->rchild->lchild)) RR_rotate(&n);
            else RL_rotate(&n);
        }
        refresh(&n);
        return n;
    }
    //往右子树查找，删除后讨论左子树是否比右子树高2
    else if (key>n->value) {
        n->rchild = avlDelete(key, n->rchild);
        //讨论左子树的左右子树谁更高来决定旋转方式
        if (getheight(n->lchild)-getheight(n->rchild)==2) {
            if (getheight(n->lchild->lchild)>=getheight(n->lchild->rchild)) LL_rotate(&n);
            else LR_rotate(&n);
        }
        refresh(&n);
        return n;
    }
    //找到了删除节点
    else {
        //删除的节点有左子树和右子树
        if (n->lchild && n->rchild) {
            //左子树更高，用左子树的最大值来代替
            if (getheight(n->lchild)>=getheight(n->rchild)) {
                struct avlnode *p = n->lchild;
                while (p->rchild) p = p->rchild;
                n->value = p->value;
                n->lchild = avlDelete(p->value, n->lchild);
                refresh(&n);
                return n;
            }
            //右子树更高，用右子树的最小值来代替
            else {
                struct avlnode *p = n->rchild;
                while (p->lchild) p = p->lchild;
                n->value = p->value;
                n->rchild = avlDelete(p->value, n->rchild);
                refresh(&n);
                return n;
            }
        }
        else {
            //用仅含有的那颗子树直接代替
            struct avlnode *temp;
            temp = n;
            if (n->lchild) n = n->lchild;
            else if (n->rchild) n = n->rchild;
            else n = NULL;
            free(temp);
            return n;
        }
    }
}
void avlpreorder(struct avlnode *n){
    if(n){
        printf("%d ",n->value);
        avlpreorder(n->lchild);
        avlpreorder(n->rchild);
    }
}

/*int main() {
    int n;
    printf("Please enter the number of operations you want to perform:");
    scanf("%d", &n);
    while (n--) {
        int opcode, key;
        printf("Please enter the opcode and value(%d sets of data left):", n+1);
        scanf("%d%d", &opcode, &key);
        struct avlnode *q;
        switch (opcode) {
        case 1:
            q = avlSearch(key, avlroot);
            if (q) printf("Found!\n");
            else printf("Not found!\n");
            break;
        case 2:
            avlroot = avlInsert(key, avlroot);
            avlpreorder(avlroot);
            printf("\n");
            break;
        case 3:
            avlroot = avlDelete(key, avlroot);
            avlpreorder(avlroot);
            printf("\n");
            break;
        default:
            printf("Wrong opcode!\n");
            n++;
            break;
        }
    }
}*/