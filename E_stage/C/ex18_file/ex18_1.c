#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/** Our old friend die from ex17. */
void die(const char *message)
{
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    exit(1);
}

// a typedef creates a fake type, in this
// case for a function pointer
typedef int (*compare_cb)(int a, int b);

/**
 * A classic bubble sort function that uses the
 * compare_cb to do the sorting.
 */
int *bubble_sort(int *numbers, int count, compare_cb cmp)
{
    int temp = 0;
    int i = 0;
    int j = 0;
    int *target = malloc(count * sizeof(int));

    if(!target) die("Memory error.");

    memcpy(target, numbers, count * sizeof(int));

    for(i = 0; i < count; i++) {
        for(j = 0; j < count - 1; j++) {
            if(cmp(target[j], target[j+1]) > 0) {
                temp = target[j+1];
                target[j+1] = target[j];
                target[j] = temp;
            }
        }
    }

    return target;
}
/**
 * Selection sort algorithm
 */
int *selection_sort(int *numbers, int count, compare_cb cmp)
{
    int i = 0;
    int j = 0;
    int min_idx = 0;
    int temp = 0;
    int *target = malloc(count * sizeof(int));

    if(!target) die("Memory error.");

    memcpy(target, numbers, count * sizeof(int));

    for(i = 0; i < count - 1; i++) {
        min_idx = i;
        for(j = i + 1; j < count; j++) {
            if(cmp(target[min_idx], target[j]) > 0) {
                min_idx = j;
            }
        }
        // Swap the found minimum element with the first element
        temp = target[min_idx];
        target[min_idx] = target[i];
        target[i] = temp;
    }

    return target;
}

int sorted_order(int a, int b)
{
    return a - b;
}

int reverse_order(int a, int b)
{
    return b - a;
}

int strange_order(int a, int b)
{
    if(a == 0 || b == 0) {
        return 0;
    } else {
        return a % b;
    }
}

// Typedef for sort functions
typedef int *(*sort_func)(int *numbers, int count, compare_cb cmp);

/**
 * Used to test that we are sorting things correctly
 * by doing the sort and printing it out.
 */
void test_sorting(int *numbers, int count, sort_func sort, compare_cb cmp, const char *sort_name, const char *order_name)
{
    int i = 0;
    int *sorted = sort(numbers, count, cmp);

    if(!sorted) die("Failed to sort as requested.");

    printf("%s with %s order: ", sort_name, order_name);
    for(i = 0; i < count; i++) {
        printf("%d ", sorted[i]);
    }
    printf("\n");

    free(sorted);
}

int main(int argc, char *argv[])
{
    if(argc < 2) die("USAGE: ex18 4 3 1 5 6");

    int count = argc - 1;
    int i = 0;
    char **inputs = argv + 1;

    int *numbers = malloc(count * sizeof(int));
    if(!numbers) die("Memory error.");

    for(i = 0; i < count; i++) {
        numbers[i] = atoi(inputs[i]);
    }

    printf("Original array: ");
    for(i = 0; i < count; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n\n");

    // Test bubble sort with different comparison functions
    test_sorting(numbers, count, bubble_sort, sorted_order, "Bubble sort", "sorted");
    test_sorting(numbers, count, bubble_sort, reverse_order, "Bubble sort", "reverse");
    test_sorting(numbers, count, bubble_sort, strange_order, "Bubble sort", "strange");
    
    printf("\n");
    
    // Test selection sort with different comparison functions
    test_sorting(numbers, count, selection_sort, sorted_order, "Selection sort", "sorted");
    test_sorting(numbers, count, selection_sort, reverse_order, "Selection sort", "reverse");
    test_sorting(numbers, count, selection_sort, strange_order, "Selection sort", "strange");

    free(numbers);

    return 0;
}