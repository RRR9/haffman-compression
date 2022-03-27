struct AVLNode {
    AVLNode *left;
    AVLNode *right;
    uchar height;
    uchar symbol;
    char *code;
};

static AVLNode * insert(AVLNode *, char *, char);
static void find(AVLNode *, char *, AVLNode **);
static void showTree(AVLNode *);
static int balance_factor(AVLNode *);
static AVLNode * count_height(AVLNode *);
static uchar height(AVLNode *);
static AVLNode * fill_node(AVLNode *, char *, char);
static AVLNode * balance(AVLNode *);
static AVLNode * rotate_left(AVLNode *);
static AVLNode * rotate_right(AVLNode *);
static AVLNode * copy_node(AVLNode *);

static int balance_factor(AVLNode *p) {
    return p ? height(p->left) - height(p->right) : 0;
}

static uchar height(AVLNode *p) {
    return p ? p->height : 0;
}

static AVLNode * count_height(AVLNode *p) {
    uchar hl = height(p->left);
    uchar hr = height(p->right);
    p->height = (hl > hr ? hl : hr) + 1;
    return p;
}

static AVLNode * fill_node(AVLNode *p, char *code, char symbol) {
    p = (AVLNode *)malloc(sizeof(AVLNode));
    p->code = (char *)malloc(strlen(code) + 1);
    strcpy(p->code, code);
    p->symbol = symbol;
    p->left = p->right = NULL;
    p->height = 1;
    return p;
}

static AVLNode * copy_node(AVLNode *p) {
    AVLNode *temp = (AVLNode *)malloc(sizeof(AVLNode));
    temp->code = p->code;
    temp->symbol = p->symbol;
    temp->height = p->height;
    temp->left = p->left;
    temp->right = p->right;
    return temp;
}

static AVLNode * rotate_left(AVLNode *p) {
    AVLNode *q = p->right;
    AVLNode *temp = copy_node(q);
    p->right = q->left;
    temp->left = p;
    count_height(p);
    count_height(temp);
    return temp;
}

static AVLNode * rotate_right(AVLNode *p) {
    AVLNode *q = p->left;
    AVLNode *temp = copy_node(q);
    p->left = q->right;
    temp->right = p;
    count_height(p);
    count_height(temp);
    return temp;
}

static AVLNode * balance(AVLNode *p) {
    p = count_height(p);
    if(balance_factor(p) == -2) {
        if(balance_factor(p->right) > 0) {
            p->right = rotate_right(p->right);
        }
        p = rotate_left(p);
    } else if(balance_factor(p) == 2) {
        if(balance_factor(p->left) < 0) {
            p->left = rotate_left(p->left);
        }
        p = rotate_right(p);
    }
    return p;
}

static void showTree(AVLNode *p) {
    if(p != NULL) {
        printf("%-20s  %c\n", p->code, p->symbol);
        showTree(p->left);
        showTree(p->right);
    }
}

static AVLNode * insert(AVLNode *p, char *code, char symbol) {
    if(p == NULL) {
        return fill_node(p, code, symbol);
    } else {
        if(strcmp(p->code, code) > 0) {
            p->left = insert(p->left, code, symbol);
        } else {
            p->right = insert(p->right, code, symbol);
        }
        return balance(p);
    }
}

static void find(AVLNode *p, char *code, AVLNode **temp) {
    if(p != NULL) {
        *temp = NULL;
        if(strcmp(p->code, code) == 0) {
            *temp = (AVLNode *)malloc(sizeof(AVLNode));
            (*temp)->code = p->code;
            //(*temp)->code = (char *)malloc(256);
            //strcpy((*temp)->code, code);
            (*temp)->symbol = p->symbol;
        } else if(strcmp(p->code, code) > 0) {
            find(p->left, code, temp);
        } else {
            find(p->right, code, temp);
        }
    }
}