#include "stdio.h"
#include "stdlib.h"
#define RED 0
#define BLACK 1

struct rbnode {
    struct rbnode *lchild;
    struct rbnode *rchild;
    struct rbnode *parent;
    int value;
    int color;
};
struct rbnode *rbroot;
struct rbnode *NIL;

struct rbnode *getgrandparent(struct rbnode *n) {return n->parent->parent;}
struct rbnode *getuncle(struct rbnode *n) {
    if (n->parent==NIL || getgrandparent(n)==NIL) return NIL;
    if (n->parent==getgrandparent(n)->lchild) return getgrandparent(n)->rchild;
    else if (n->parent==getgrandparent(n)->rchild) return getgrandparent(n)->lchild;
    else return NIL;
}
void rblrotate(struct rbnode *n) {
    struct rbnode *temp = n->rchild;
    n->rchild = temp->lchild;
    if (temp->lchild!=NIL) temp->lchild->parent = n;
    temp->parent = n->parent;
    if (n->parent==NIL) rbroot = temp;
    else {
        if (n==n->parent->lchild) n->parent->lchild = temp;
        else n->parent->rchild = temp;
    }
    temp->lchild = n;
    n->parent = temp;
}
void rbrrotate(struct rbnode *n) {
    struct rbnode *temp = n->lchild;
    n->lchild = temp->rchild;
    if (temp->rchild!=NIL) temp->rchild->parent = n;
    temp->parent = n->parent;
    if (n->parent==NIL) rbroot = temp;
    else {
        if (n==n->parent->lchild) n->parent->lchild = temp;
        else n->parent->rchild = temp;
    }
    temp->rchild = n;
    n->parent = temp;
}
struct rbnode *rbSearch(int key, struct rbnode *n) {
    if (n==NIL) return NULL;
    if (key==n->value) return n;
    else if (key<n->value) return rbSearch(key, n->lchild);
    else return rbSearch(key, n->rchild);
}
void rbFixInsert(struct rbnode *n) {
    struct rbnode *q = n;
    struct rbnode *p = q->parent;
    struct rbnode *g = getgrandparent(q);
    //考虑父节点为红色的情况，因为不能连续出现两个红色节点
    while (p->color==RED) {
        struct rbnode *u = getuncle(q);
        if (p==g->lchild) {
            //case1:叔叔节点为红色
            //solution:将叔叔节点和父节点设置为黑色，将祖父节点设置为红色，之后从祖父处开始递归
            if (u->color==RED) {
                u->color = BLACK;
                p->color = BLACK;
                g->color = RED;
                q = g;
                p = q->parent;
                g = getgrandparent(q);
                continue;
            }
            //case2:叔叔节点为黑色或NIL，当前节点为右孩子
            //solution:将父节点左旋一次，父节点就成了子节点的左孩子，再转换关注节点，转化为case3
            if (q==p->rchild) {
                rblrotate(p);
                struct rbnode *temp = p;
                p = q;
                q = temp;
            }
            //case3:叔叔节点为黑色或NIL，当前节点为左孩子
            //solution:将父节点设为黑色，祖父节点设为红色，再将祖父节点右旋一次
            p->color = BLACK;
            g->color = RED;
            rbrrotate(g);
        } else {
            //case1:叔叔节点为红色
            //solution:将叔叔节点和父节点设置为黑色，将祖父节点设置为红色，之后从祖父处开始递归
            if (u->color==RED) {
                u->color = BLACK;
                p->color = BLACK;
                g->color = RED;
                q = g;
                p = q->parent;
                g = getgrandparent(q);
                continue;
            }
            //case2:叔叔节点为黑色或NIL，当前节点为左孩子
            //solution:将父节点右旋一次，父节点就成了子节点的右孩子，再转换关注节点，转化为case3
            if (q==p->lchild) {
                rbrrotate(p);
                struct rbnode *temp = p;
                p = q;
                q = temp;
            }
            //case3:叔叔节点为黑色或NIL，当前节点为右孩子
            //solution:将父节点设为黑色，祖父节点设为红色，再将祖父节点左旋一次
            p->color = BLACK;
            g->color = RED;
            rblrotate(g);
        }
        p = q->parent;
        g = getgrandparent(q);
    }
    //过程中有可能将根节点设为红色，重新将根节点设为黑色，所有路径都会加一，不产生矛盾
    rbroot->color = BLACK;
}
void rbInsert(int key, struct rbnode *n) {
    struct rbnode *p = NIL;
    struct rbnode *q = n;
    //找到插入位置的叶子结点和其父节点
    while (q!=NIL) {
        p = q;
        if (key==q->value) return;
        else if (key<q->value) q = q->lchild;
        else q = q->rchild;
    }
    //将插入的节点设置为红色，之后从插入后的节点开始进行递归的修正
    q = (struct rbnode*)malloc(sizeof(struct rbnode));
    q->lchild = q->rchild = NIL;
    q->value = key;
    q->color = RED;
    q->parent = p;
    if (p==NIL) rbroot = q;
    else {
        if (q->value<p->value) p->lchild = q;
        else p->rchild = q;
    }
    rbFixInsert(q);
}
void rbFixDelete(struct rbnode *n, struct rbnode *p) {
    struct rbnode *brother;
    while (n->color==BLACK && n!=rbroot) {
        //n为p的左孩子
        if (n==p->lchild) {
            brother = p->rchild;
            //case1:黑+黑 & 兄弟节点为红色
            //solution:染色，左旋，转换成情况2
            if (brother->color==RED) {
                brother->color = BLACK;
                p->color = RED;
                rblrotate(p);
                brother = p->rchild;
            }
            //case2:黑+黑 & 兄弟节点为黑色，兄弟的子节点均为黑色
            //solution:染色，转换关注节点为其父节点，继续递归
            if (brother->lchild->color==BLACK && brother->rchild->color==BLACK) {
                brother->color = RED;
                n = p;
                p = n->parent;
                continue;
            }
            //case3:黑+黑 & 兄弟节点为黑色，兄弟的右孩子为黑，左孩子为红
            //solution:染色，右旋，转换成情况4
            if (brother->rchild->color==BLACK) {
                brother->lchild->color = BLACK;
                brother->color = RED;
                rbrrotate(brother);
                brother = p->rchild;
            }
            //case4:黑+黑 & 兄弟节点为黑色，兄弟的右孩子为红色
            //solution:将兄弟染成父节点的颜色，父节点染成黑色，右旋
            brother->color = p->color;
            p->color = BLACK;
            brother->rchild->color = BLACK;
            rblrotate(p);
            n = rbroot;
            break;
        } else {//n为p的右孩子
            brother = p->lchild;
            //case1:黑+黑 & 兄弟节点为红色
            //solution:染色，右旋，转换成情况2
            if (brother->color==RED) {
                brother->color = BLACK;
                p->color = RED;
                rbrrotate(p);
                brother = p->lchild;
            }
            //case2:黑+黑 & 兄弟节点为黑色，兄弟的子节点均为黑色
            //solution:染色，转换关注节点为其父节点，继续递归
            if (brother->lchild->color==BLACK && brother->rchild->color==BLACK) {
                brother->color = RED;
                n = p;
                p = n->parent;
                continue;
            }
            //case3:黑+黑 & 兄弟节点为黑色，兄弟的左孩子为黑，右孩子为红
            //solution:染色，左旋，转换成情况4
            if (brother->lchild->color==BLACK) {
                brother->rchild->color = BLACK;
                brother->color = RED;
                rblrotate(brother);
                brother = p->lchild;
            }
            //case4:黑+黑 & 兄弟节点为黑色，兄弟的左孩子为红色
            //solution:将兄弟染成父节点的颜色，父节点染成黑色，右旋
            brother->color = p->color;
            p->color = BLACK;
            brother->lchild->color = BLACK;
            rbrrotate(p);
            n = rbroot;
            break;
        }
    }
    //case0:处理后的根节点一定为黑+红
    //solution:将其染成黑色
    if (n!=NIL) n->color = BLACK;
}
void rbDelete(int key, struct rbnode *n) {
    if (n==NIL || n==NULL) return;
    struct rbnode *p, *child;
    struct rbnode *q = n;
    int color;
    //在红黑树中查找需要删除的节点
    while (q!=NIL) {
        p = q;
        if (key==q->value) break;
        if (key<q->value) q = q->lchild;
        else q = q->rchild;
    }
    //红黑树中没有要删除的节点
    if (q==NIL) return;
    //q有左子树和右子树,转为删除直接后继
    if (q->lchild!=NIL && q->rchild!=NIL) {
        //找到q的直接后继
        struct rbnode *s = q->rchild;
        while (s->lchild!=NIL) s = s->lchild;
        p = q->parent;
        if (p==NIL) rbroot = s;
        else {//更新父节点的子节点为q的直接后继
            if (q==p->lchild) p->lchild = s;
            else p->rchild = s;
        }
        //更新直接后继的父节点
        p = s->parent;
        child = s->rchild;
        color = s->color;
        //更新右孩子
        if (p==q) p = s;
        else {
            if (child!=NIL) child->parent = p;
            p->lchild = child;
            s->rchild = q->rchild;
            q->rchild->parent = s;
        }
        //更新左孩子
        s->parent = q->parent;
        s->lchild = q->lchild;
        q->lchild->parent = s;
        s->color = q->color;
        //如果删除节点为黑色，进行修正
        if (color==BLACK) rbFixDelete(child, p);
        free(q);
        return;
    }
    //q没有左子树或者右子树
    if (q->lchild!=NIL) child = q->lchild;
    else child = q->rchild;
    p = q->parent;
    color = q->color;
    //直接将q的孩子连到q的父节点上
    if (child!=NIL) child->parent = p;
    if (p!=NIL) {//考虑q是根节点的情况
        if (p->lchild==q) p->lchild = child;
        else p->rchild = child;
    } else rbroot = child;
    //如果删除节点为黑色，进行修正
    if (color==BLACK) rbFixDelete(child, p);
    free(q);
}
void rbpreorder(struct rbnode *n){
    if(n == NIL) return;
    printf("*%d is %s* ", n->value, n->color==RED?"RED":"BLACK");
    rbpreorder(n->lchild);
    rbpreorder(n->rchild);
}

/*int main() {
    NIL = (struct rbnode *)malloc(sizeof(struct rbnode));
    NIL->lchild = NIL->rchild = NIL->parent = NULL;
    NIL->color = BLACK;
    rbroot = NIL;
    int n;
    printf("Please enter the number of operations you want to perform:");
    scanf("%d", &n);
    while (n--) {
        int opcode, key;
        printf("Please enter the opcode and value(%d sets of data left):", n+1);
        scanf("%d%d", &opcode, &key);
        struct rbnode *q;
        switch (opcode) {
        case 1:
            q = rbSearch(key, rbroot);
            if (q) printf("Found! %d is %s.\n", q->value, q->color==RED?"RED":"BLACK");
            else printf("Not found!\n");
            break;
        case 2:
            rbInsert(key, rbroot);
            rbpreorder(rbroot);
            printf("\n");
            break;
        case 3:
            rbDelete(key, rbroot);
            rbpreorder(rbroot);
            printf("\n");
            break;
        default:
            printf("Wrong opcode!\n");
            n++;
            break;
        }
    }
}*/