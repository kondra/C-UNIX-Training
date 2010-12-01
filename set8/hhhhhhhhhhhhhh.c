#include 
#include 
#include 
enum {
    MAX_HASH = 65536,
    MAX_STR_LEN = 11,
    CRC_TABLE_LEN = 256,
    MAGIC_POLYNOM = 0xEDB88320UL,
    BITS = 8,
    ONES_MASK = 0xFFFFFFFFUL,
    ADD_ELEM = '+',
    DEL_ELEM = '-',
    FIND_ELEM = '?',
    END_OF_INPUT = '#'
};
struct hash_node {
    char str[MAX_STR_LEN];
    struct hash_node next_node;
};

/*int hash_func(char str)
{
    unsigned short crc = 0xFFFF;
    unsigned char i;
    char len = MAX_STR_LEN - 1;
    while (len--)
    {
        crc ^= str++ next_node){
        if (!strcmp(str, p->str)){
            return;
        }
        p = p->next_node;
    }
    p->next_node = malloc(sizeof(struct hash_node));
    p->next_node->next_node = NULL;
    strcpy(p->next_node->str, str);
    return;
}
void
del_elem(char *str, struct hash_node *hash_array[])
{
    int hash_val = hash_func(str);
    struct hash_node **p  = &(hash_array[hash_val]), 
                     **next_p = &(hash_array[hash_val]->next_node), 
                     *p_backup;
    if (!*p){
        return;
    }
    while(strcmp((*p)->str, str)){
        if ((*p)->next_node){
            p = next_p;
            next_p = &(*next_p)->next_node;
            continue;
        } else {
            return;
        }
    }
    p_backup = *p;
    *p = *next_p; 
    free(p_backup); 
}

int 
find_elem(char *str, struct hash_node *hash_array[])
{
    int hash_val = hash_func(str);
    struct hash_node *p = hash_array[hash_val];
    while (p){
        if (!strcmp(p->str, str)){
            return 1;
        }
        p = p->next_node;
    }
    return 0;
}
void 
parse_input(struct hash_node *hash_array[])
{
    char str[MAX_STR_LEN];
    char command, null;
    do {
        command = getchar();
        null = getchar();
        if (!fgets(str, MAX_STR_LEN, stdin)){
            return;
       }
        null = getchar();
        switch (command){
            case ADD_ELEM: 
                add_elem(str, hash_array);
                break;
            case DEL_ELEM:
                del_elem(str, hash_array);
                break;
            case FIND_ELEM:
                if (find_elem(str, hash_array)){
                    printf("YES\n");
                } else {
                    printf("NO\n");
                }
                break;
            default:
                return;
        }
    } while (command != END_OF_INPUT);
}
int main(void)
{
    int i;
    struct hash_node *arr[MAX_HASH];
    struct hash_node *p;
    p = malloc(sizeof(struct hash_node) * MAX_HASH);
    for (i = 0; i < MAX_HASH; i++){
        arr[i] = p + i;
    }
    parse_input(arr);
    return 0;
}

