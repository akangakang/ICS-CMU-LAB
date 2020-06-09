/*
 * Explicit Free Lists + Segregated Free Lists + Segregated Fits
 * Explicit Free Lists: 
 *      a free block is consist of a header(4 bytes) + a pred pointer(8 bytes) + a succ pointer(8 bytes) + a footer(4 bytes)
 *      so a block is at least 24 bytes
 * 
 *      a allacate block is consist of at least a header and footer, but after free , it will 
 *      become a free block ,so it is also at least 24 bytes 
 * Segregated Free Lists: 
 *      partition the set of all possible bloc sizes into equivalence classes 
 *      {1} {2-3} {4-7} {8-15} {16-31} {32-63} {64-127} {128-255} .... {32768- }
 * Segregated Fits:
 *      each free list is associated with a size class
 *      because I can't use array, I use 16 static variables segregated_free_list(i) 
 *  
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8
#define ALIGN(size) ((((size) + (ALIGNMENT - 1)) / (ALIGNMENT)) * (ALIGNMENT))

// Basic constants and macros
#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1 << 12)
#define LIST_NUM 16

#define MAX(x, y) ((x) > (y) ? (x) : (y))

// Pack a size and allocated bit into a word
#define PACK(size, alloc) ((size) | (alloc))

// Read and write a word at address p
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

#define SET_PTR(p, ptr) (*(unsigned long *)(p) = (unsigned long)(ptr))

// Read the size and allocated fields from address p
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

// Given block ptr bp, compute address of its header and footer
#define HDRP(ptr) ((char *)(ptr)-WSIZE)
#define FTRP(ptr) ((char *)(ptr) + GET_SIZE(HDRP(ptr)) - DSIZE)

// Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(ptr) ((char *)(ptr) + GET_SIZE((char *)(ptr)-WSIZE))
#define PREV_BLKP(ptr) ((char *)(ptr)-GET_SIZE((char *)(ptr)-DSIZE))

// Given block ptr bp of a free block, compute the !address! of predecessor/successor pointer
// Given pointer to store the address of the pred pointer
/*
    ------------
    |__HEADER__|  ___ PRED_ADR(bp) 
    |___PRED___|
*/
#define PRED_ADR(ptr) ((char *)(ptr))
#define SUCC_ADR(ptr) ((char *)(ptr) + DSIZE)

// Given block ptr bp of a free block, get the pointer pointing to it's pred/succ
#define PRED(ptr) (*(char **)(ptr))
#define SUCC(ptr) (*(char **)(SUCC_ADR(ptr)))

static void *head_listsp;
static void *segregated_free_list0 = NULL;
static void *segregated_free_list1 = NULL;
static void *segregated_free_list2 = NULL;
static void *segregated_free_list3 = NULL;
static void *segregated_free_list4 = NULL;
static void *segregated_free_list5 = NULL;
static void *segregated_free_list6 = NULL;
static void *segregated_free_list7 = NULL;
static void *segregated_free_list8 = NULL;
static void *segregated_free_list9 = NULL;
static void *segregated_free_list10 = NULL;
static void *segregated_free_list11 = NULL;
static void *segregated_free_list12 = NULL;
static void *segregated_free_list13 = NULL;
static void *segregated_free_list14 = NULL;
static void *segregated_free_list15 = NULL;

/*
 * Given the id of the size class , return the address of fisrt pointer
 */
static void *segregated_free_list(int i)
{
    // printf("segregater_free_list %d \n", i);
    switch (i)
    {
    case 0:
        return &segregated_free_list0;
    case 1:
        return &segregated_free_list1;
    case 2:
        return &segregated_free_list2;
    case 3:
        return &segregated_free_list3;
    case 4:
        return &segregated_free_list4;
    case 5:
        return &segregated_free_list5;
    case 6:
        return &segregated_free_list6;
    case 7:
        return &segregated_free_list7;
    case 8:
        return &segregated_free_list8;
    case 9:
        return &segregated_free_list9;
    case 10:
        return &segregated_free_list10;
    case 11:
        return &segregated_free_list11;
    case 12:
        return &segregated_free_list12;
    case 13:
        return &segregated_free_list13;
    case 14:
        return &segregated_free_list14;
    case 15:
        return &segregated_free_list15;

    default:
        printf("false num\n");
        return NULL;
    }
}
static void *extend_heap(size_t size);
static void *coalesce(void *ptr);
static void *place(void *ptr, size_t size);
static void insert_node(void *ptr, size_t size);
static void delete_node(void *ptr);

/*
 * Extends the heap with a new free block 
 */
static void *extend_heap(size_t size)
{
    void *ptr;

    size = ALIGN(size);

    if ((ptr = mem_sbrk(size)) == (void *)-1)
        return NULL;

    /* Initial free block header/footer and the epilogue headers */
    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));

    PUT(HDRP(NEXT_BLKP(ptr)), PACK(0, 1));

    /* insert the free block into the free list */
    // printf("extend_heap to insert_node \n");
    insert_node(ptr, size);

    return coalesce(ptr);
}

/*
 * Insert a node into the free list 
 */
static void insert_node(void *ptr, size_t size)
{
    int listnumber = 0;

    while ((listnumber < LIST_NUM - 1) && (size > 1))
    {
        // printf("insert_node while 1 : %d \n", num);

        size >>= 1;
        listnumber++;
    }
    /* find the suitable position to insert -- from small to big*/
    void *search_ptr = (void *)*(unsigned long *)segregated_free_list(listnumber);
    void *insert_ptr = NULL;

    while ((search_ptr != NULL) && (size > GET_SIZE(HDRP(search_ptr))))
    {
        insert_ptr = search_ptr;
        search_ptr = PRED(search_ptr);
    }
    /* when break , insert_p is the pred block of the new node , search_p is the next block of the new node*/

    /* the new node is inserted in the middle */
    if (search_ptr != NULL && insert_ptr != NULL)
    {

        SET_PTR(PRED_ADR(ptr), search_ptr);
        SET_PTR(SUCC_ADR(search_ptr), ptr);
        SET_PTR(SUCC_ADR(ptr), insert_ptr);
        SET_PTR(PRED_ADR(insert_ptr), ptr);
        return;
    }

    /* the new node should be inserted as the first node*/
    if (search_ptr != NULL && insert_ptr == NULL)
    {
        SET_PTR(PRED_ADR(ptr), search_ptr);
        SET_PTR(SUCC_ADR(search_ptr), ptr);
        SET_PTR(SUCC_ADR(ptr), NULL);
        SET_PTR(segregated_free_list(listnumber), ptr);
        return;
    }

    /* the new node is inserted as the last */
    if (search_ptr == NULL && insert_ptr != NULL)
    {
        SET_PTR(PRED_ADR(ptr), NULL);
        SET_PTR(SUCC_ADR(ptr), insert_ptr);
        SET_PTR(PRED_ADR(insert_ptr), ptr);
        return;
    }
    if (search_ptr == NULL && insert_ptr == NULL)
    {
        /* if it's the first node */
        SET_PTR(PRED_ADR(ptr), NULL);
        SET_PTR(SUCC_ADR(ptr), NULL);
        SET_PTR(segregated_free_list(listnumber), ptr);
    }
}

/*
 * Delete a node from the free list
 */
static void delete_node(void *ptr)
{
    int listnumber = 0;
    size_t size = GET_SIZE(HDRP(ptr));

    while ((listnumber < LIST_NUM - 1) && (size > 1))
    {
        size >>= 1;
        listnumber++;
    }

    /* the middle */
    if (PRED(ptr) != NULL && SUCC(ptr) != NULL)
    {
        SET_PTR(SUCC_ADR(PRED(ptr)), SUCC(ptr));
        SET_PTR(PRED_ADR(SUCC(ptr)), PRED(ptr));
        return;
    }
    /* the last */
    if (PRED(ptr) != NULL && SUCC(ptr) == NULL)
    {
        SET_PTR(SUCC_ADR(PRED(ptr)), NULL);
        SET_PTR(segregated_free_list(listnumber), PRED(ptr));
        return;
    }

    /* the first */
    if (PRED(ptr) == NULL && SUCC(ptr) != NULL)
    {
        SET_PTR(PRED_ADR(SUCC(ptr)), NULL);
        return;
    }
    /* the only one */
    if (PRED(ptr) == NULL && SUCC(ptr) == NULL)
    {

        SET_PTR(segregated_free_list(listnumber), NULL);
        return;
    }
}

/*
 * merge the continous free block
 */
static void *coalesce(void *ptr)
{
    _Bool is_prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(ptr)));
    _Bool is_next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(ptr)));
    size_t size = GET_SIZE(HDRP(ptr));

    /*
        CASE 1 : prev and next allocated
    */
    if (is_prev_alloc && is_next_alloc)
    {
        return ptr;
    }

    /*
        CASE 2 : prev allocated , next free
    */
    else if (is_prev_alloc && !is_next_alloc)
    {
        delete_node(ptr);
        delete_node(NEXT_BLKP(ptr));
        size += GET_SIZE(HDRP(NEXT_BLKP(ptr)));
        PUT(HDRP(ptr), PACK(size, 0));
        PUT(FTRP(ptr), PACK(size, 0));
    }
    /*
        CASE 3 : prev free next allocated
    */
    else if (!is_prev_alloc && is_next_alloc)
    {
        delete_node(ptr);
        delete_node(PREV_BLKP(ptr));
        size += GET_SIZE(HDRP(PREV_BLKP(ptr)));
        PUT(FTRP(ptr), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(ptr)), PACK(size, 0));
        ptr = PREV_BLKP(ptr);
    }
    /*
        CASE 4 : next and prev free   
   */
    else
    {
        delete_node(ptr);
        delete_node(PREV_BLKP(ptr));
        delete_node(NEXT_BLKP(ptr));
        size += GET_SIZE(HDRP(PREV_BLKP(ptr))) + GET_SIZE(HDRP(NEXT_BLKP(ptr)));
        PUT(HDRP(PREV_BLKP(ptr)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(ptr)), PACK(size, 0));
        ptr = PREV_BLKP(ptr);
    }

    // printf("coalesce to insert_node \n");
    insert_node(ptr, size);

    return ptr;
}

/*
 * Make a free block allocated
 */
static void *place(void *ptr, size_t size)
{
    size_t ptr_size = GET_SIZE(HDRP(ptr));

    size_t remaining = ptr_size - size;

    delete_node(ptr);

    if (remaining < DSIZE * 3)
    {
        PUT(HDRP(ptr), PACK(ptr_size, 1));
        PUT(FTRP(ptr), PACK(ptr_size, 1));
    }

    else if (size >= 88)
    {
        PUT(HDRP(ptr), PACK(remaining, 0));
        PUT(FTRP(ptr), PACK(remaining, 0));
        PUT(HDRP(NEXT_BLKP(ptr)), PACK(size, 1));
        PUT(FTRP(NEXT_BLKP(ptr)), PACK(size, 1));
        insert_node(ptr, remaining);
        return NEXT_BLKP(ptr);
    }

    else
    {
        PUT(HDRP(ptr), PACK(size, 1));
        PUT(FTRP(ptr), PACK(size, 1));
        PUT(HDRP(NEXT_BLKP(ptr)), PACK(remaining, 0));
        PUT(FTRP(NEXT_BLKP(ptr)), PACK(remaining, 0));
        insert_node(NEXT_BLKP(ptr), remaining);
    }
    return ptr;
}

/*
 * Creates a heap with an initial free block
 */
int mm_init(void)
{
    int listnumber;

    for (listnumber = 0; listnumber < LIST_NUM; listnumber++)
    {
        SET_PTR(segregated_free_list(listnumber), NULL);
    }

    if ((long)(head_listsp = mem_sbrk(4 * WSIZE)) == -1)
        return -1;

    PUT(head_listsp, 0);                            /* Aligment padding */
    PUT(head_listsp + (1 * WSIZE), PACK(DSIZE, 1)); /* Prologue header */
    PUT(head_listsp + (2 * WSIZE), PACK(DSIZE, 1)); /* Prologue footer */
    PUT(head_listsp + (3 * WSIZE), PACK(0, 1));     /* Epilogue header */
    head_listsp += (2 * WSIZE);

    /* Extend the empty heap woth a free block of CHUNKSIZE bytes */
    if (extend_heap(CHUNKSIZE) == NULL)
        return -1;

    return 0;
}

/*
    The mm_malloc routine returns a pointer to an allocated block payload 
    of at least size bytes
*/
void *mm_malloc(size_t size)
{
    if (size == 0)
        return NULL;

    if (size <= 2 * DSIZE)
    {
        size = 3 * DSIZE;
    }
    else
    {
        size = ALIGN(size + DSIZE);
    }

    int listnumber = 0;
    size_t searchsize = size;
    void *ptr = NULL;

    while (listnumber < LIST_NUM)
    {

        if (((searchsize <= 1) && ((void *)*(unsigned long *)segregated_free_list(listnumber) != NULL)))
        {
            ptr = (void *)*(unsigned long *)segregated_free_list(listnumber);

            while ((ptr != NULL) && ((size > GET_SIZE(HDRP(ptr)))))
            {
                ptr = PRED(ptr);
            }

            if (ptr != NULL)
                break;
        }

        searchsize >>= 1;
        listnumber++;
    }

    /* no free block  */
    if (ptr == NULL)
    {
        if ((ptr = extend_heap(MAX(size, CHUNKSIZE))) == NULL)
            return NULL;
    }

    ptr = place(ptr, size);

    return ptr;
}

/*
    The mm_free routine frees the block pointed to by ptr.
*/
void mm_free(void *ptr)
{
    size_t size = GET_SIZE(HDRP(ptr));

    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));

    insert_node(ptr, size);

    coalesce(ptr);
}

/*
    The mm_realloc routine returns a pointer to an allocated region of at 
    least size bytes with the following constraints.
*/
void *mm_realloc(void *ptr, size_t size)
{
    void *new_block = ptr;
    int remaining;

    if (size == 0)
        return NULL;

    if (size <= 2 * DSIZE)
    {
        size = 3 * DSIZE;
    }
    else
    {
        size = ALIGN(size + DSIZE);
    }

    if ((remaining = GET_SIZE(HDRP(ptr)) - size) >= 0)
    {
        return ptr;
    }

    else if (!GET_ALLOC(HDRP(NEXT_BLKP(ptr))) || !GET_SIZE(HDRP(NEXT_BLKP(ptr))))
    {

        if ((remaining = GET_SIZE(HDRP(ptr)) + GET_SIZE(HDRP(NEXT_BLKP(ptr))) - size) < 0)
        {
            if (extend_heap(MAX(-remaining, CHUNKSIZE)) == NULL)
                return NULL;
            remaining += MAX(-remaining, CHUNKSIZE);
        }

        delete_node(NEXT_BLKP(ptr));
        PUT(HDRP(ptr), PACK(size + remaining, 1));
        PUT(FTRP(ptr), PACK(size + remaining, 1));
    }

    else
    {
        new_block = mm_malloc(size);
        memcpy(new_block, ptr, GET_SIZE(HDRP(ptr)));
        mm_free(ptr);
    }

    return new_block;
}
/*
 * mm_check - a simple heap consistency checker. Return 1 if check succeed, 0 if fail.
 */
static int mm_check(void)
{
    char *bp;
    char *fp;

    // Check contiguous free blocks
    for (bp = head_listsp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp))
    {
        if (!(GET_ALLOC(HDRP(bp)) || GET_ALLOC(HDRP(NEXT_BLKP(bp)))))
        {
            fprintf(stderr, "contagious free blocks");
            return 0;
        }
    }

    // Check the allocate bit of free blocks in the free list
    for (int i = 0; i < LIST_NUM; i++)
    {
        void *p = (void *)*(unsigned long *)segregated_free_list(i);
        for (fp = p; NULL != fp; fp = PRED(fp))
        {
            if (GET_ALLOC(fp))
            {
                fprintf(stderr, "block in the free list marked as allocated.");
                return 0;
            }
        }
    }

    int count = 0;
    // Check whether there are any free block outside the free list
    for (bp = head_listsp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp))
    {
        if (!GET_ALLOC(HDRP(bp)))
        {
            count++;
        }
    }
    for (int i = 0; i < LIST_NUM; i++)
    {
        void *p = (void *)*(unsigned long *)segregated_free_list(i);
        for (fp = p; NULL != fp; fp = PRED(fp))
        {
            count--;
        }
    }
    if (0 != count)
    {
        fprintf(stderr, "numbers of free blocks in free list and heap are unmatched.");
        return 0;
    }

    return 1;
}