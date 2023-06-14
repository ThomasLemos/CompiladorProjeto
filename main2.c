#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the cell structure
typedef struct Cell
{
    struct Cell *pointer1;
    char type;
    struct Cell *pointer2;
} Cell;

// Define the heap size
#define HEAP_SIZE 100

// Define the heap as a global variable
Cell heap[HEAP_SIZE];

// Define the index of the next free cell in the heap
int next_free = 0;

// Function to allocate a new cell in the heap
Cell *allocate_cell(Cell *pointer1, char type, Cell *pointer2)
{
    if (next_free >= HEAP_SIZE)
    {
        printf("Heap overflow: No free cells available.\n");
        return NULL;
    }

    Cell *new_cell = &heap[next_free++];
    new_cell->pointer1 = pointer1;
    new_cell->type = type;
    new_cell->pointer2 = pointer2;

    return new_cell;
}

// Function to perform reduction based on cell types
Cell *reduce(Cell *cell)
{
    if (cell == NULL)
    {
        return NULL;
    }

    switch (cell->type)
    {
    case '@':
        // Perform reduction for '@' cells
        if (cell->pointer1 != NULL)
        {
            return reduce(cell->pointer1);
        }
        break;
    case 'K':
        // Perform reduction for 'K' cells
        if (cell->pointer1 != NULL && cell->pointer1->pointer1 != NULL)
        {
            return reduce(cell->pointer1->pointer1);
        }
        break;
    case 'S':
        // Perform reduction for 'S' cells
        if (cell->pointer1 != NULL && cell->pointer2 != NULL &&
            cell->pointer1->pointer1 != NULL && cell->pointer2->pointer1 != NULL)
        {
            Cell *a = reduce(cell->pointer1->pointer1);
            Cell *b = reduce(cell->pointer2->pointer1);
            Cell *c = reduce(cell->pointer2->pointer2);

            if (a != NULL && b != NULL && c != NULL)
            {
                Cell *ac = allocate_cell(a, ' ', c);
                Cell *bc = allocate_cell(b, ' ', c);
                return reduce(allocate_cell(ac, ' ', bc));
            }
        }
        break;
    default:
        printf("Unknown cell type: %c\n", cell->type);
        break;
    }

    return cell;
}

// Function to print all cells in the graph
void print_cells(Cell *root)
{
    if (root == NULL)
    {
        return;
    }

    printf("Cell: %p, Type: %c\n", root, root->type);

    print_cells(root->pointer1);
    print_cells(root->pointer2);
}

// Function to traverse the graph and perform reduction
Cell *traverse(Cell *root)
{
    if (root == NULL)
    {
        return NULL;
    }

    Cell *current = root;

    while (current != NULL)
    {
        current = reduce(current);

        if (current != NULL && current->pointer1 != NULL)
        {
            current = current->pointer1;
        }
        else
        {
            break;
        }
    }

    return current;
}

int main()
{
    // Create the graph based on the character sequence
    char sequence[] = "S(K(K))(KS)(SK)KK(SK)K";

    int len = strlen(sequence);
    int i = 0;

    Cell *root = NULL;
    Cell *current_cell = NULL;

    while (i < len)
    {
        char current_char = sequence[i];

        if (current_char == '(')
        {
            current_cell = allocate_cell(current_cell, ' ', NULL);
        }
        else if (current_char == ')')
        {
            if (current_cell != NULL && current_cell->pointer1 != NULL)
            {
                current_cell = current_cell->pointer1;
            }
        }
        else
        {
            current_cell = allocate_cell(current_cell, current_char, NULL);

            if (root == NULL)
            {
                root = current_cell;
            }
        }

        i++;
    }

    // Print all cells in the graph
    printf("Printing all cells in the graph:\n");
    print_cells(root);

    // Traverse the graph and perform reduction
    Cell *result = traverse(root);

    // Print the result
    if (result != NULL)
    {
        printf("Result: %c\n", result->type);
    }
    else
    {
        printf("No result.\n");
    }

    return 0;
}
