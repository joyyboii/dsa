#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct rope rope;
struct rope {
    int length;
    char *str;
    rope *left, *right;
};

rope *new_node(char *str, int len) {
    rope *n = (rope*)malloc(sizeof(rope));
    n->str = strdup(str);
    n->length = len;
    n->left = n->right = NULL;
    return n;
}

rope *concat(rope *a, rope *b) {
    rope *n = malloc(sizeof(rope));
    if (n == NULL) {
        // Handle memory allocation failure
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    n->left = a;
    n->right = b;
    n->str = NULL;
    n->length = (a ? a->length : 0) + (b ? b->length : 0);
    return n;
}


void split(rope *n, int pos, rope **left, rope **right) {
    if (n == NULL) {
        *left = NULL;
        *right = NULL;
        return;
    }
    if (pos <= 0 || pos >= n->length) {
        *left = n;
        *right = NULL;
        return;
    }
    if (n->left == NULL) {
        *left = n;
        *right = NULL;
        return;
    }
    if (pos <= n->left->length) {
        split(n->left, pos, left, &(n->left));
        *right = n;
    } else {
        split(n->right, pos - n->left->length, &(n->right), right);
        *left = n;
    }
    n->length = (n->left ? n->left->length : 0) + (n->right ? n->right->length : 0);
}


void insert(rope **n, int pos, rope *sub) {
    rope *left, *right;
    split(*n, pos, &left, &right);
    *n = concat(left, sub);
    *n = concat(*n, right);
}

void delete(rope **n, int start, int end) {
    rope *left, *mid, *right;
    split(*n, start, &left, &mid);
    split(mid, end - start + 1, &mid, &right);
    *n = concat(left, right);
}

void display(rope *n) {
    if (n == NULL) return;
    if (n->str) {
        printf("%.*s", n->length, n->str);
        return;
    }
    display(n->left);
    display(n->right);
}

int main() {
    rope *r, *sub, *left, *mid, *right;
    int len=0, start=0, end=0, pos=0, ch = 0;
    char str[100];
    while (ch<8) {
        printf("\n 1.Create\t 2.Concat\t 3.Split\t4.Insert\t5.Delete\t6.Display\t7.Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1:
                printf("Enter string: ");
                scanf("%s", str);
                len = strlen(str);
                r = new_node(str, len);
                break;
            case 2:
                if (r == NULL) {
                    printf("No rope exists!\n");
                    break;
                }
                printf("Enter right rope string: ");
                scanf("%s", str);
                len = strlen(str);
                sub = new_node(str, len);
                r = concat(r, sub);
                break;
            case 3:
                if (r == NULL) {
                    printf("No rope exists!\n");
                    break;
                }
                printf("Enter start and end position: ");
                scanf("%d %d", &start, &end);
                split(r, start, &left, &mid);
                split(mid, end - start + 1, &mid, &right);
                printf("Left  : ");
                display(left);
                printf("\n");
                printf("Mid   : ");
                display(mid);
                printf("\n");
                printf("Right : ");
                display(right);
                printf("\n");
                break;
            case 4:
                if (r == NULL) {
                    printf("No rope exists!\n");
                    
                }
                printf("Enter start position and sub-rope string: ");
                scanf("%d %s", &pos, str);
                len = strlen(str);
                sub = new_node(str, len);
                insert(&r, pos, sub);
                break;
            case 5:
                if (r == NULL) {
                    printf("No rope exists!\n");
                    break;
                }
                printf("Enter start and end position: ");
                scanf("%d %d", &start, &end);
                delete(&r, start, end);
                break;
            case 6:
                if (r == NULL) {
                    printf("No rope exists!\n");
                    break;
                }
                printf("Rope  : ");
                display(r);
                printf("\n");
                break;
            case 7:
                exit(0);
            default:
                printf("Invalid command!\n");
                break;
        }
    }

    return 0;
}
