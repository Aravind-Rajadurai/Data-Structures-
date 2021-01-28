// FILE: DPQueue.cpp
// IMPLEMENTS: p_queue (see DPQueue.h for documentation.)
//
// INVARIANT for the p_queue class:
//   1. The number of items in the p_queue is stored in the member
//      variable used.
//   2. The items themselves are stored in a dynamic array (partially
//      filled in general) organized to follow the usual heap storage
//      rules.
//      2.1 The member variable heap stores the starting address
//          of the array (i.e., heap is the array's name). Thus,
//          the items in the p_queue are stored in the elements
//          heap[0] through heap[used - 1].
//      2.2 The member variable capacity stores the current size of
//          the dynamic array (i.e., capacity is the maximum number
//          of items the array currently can accommodate).
//          NOTE: The size of the dynamic array (thus capacity) can
//                be resized up or down where needed or appropriate
//                by calling resize(...).
// NOTE: Private helper functions are implemented at the bottom of
// this file along with their precondition/postcondition contracts.

#include <cassert>   // provides assert function
#include <iostream>  // provides cin, cout
#include <iomanip>   // provides setw
#include <cmath>     // provides log2
#include "DPQueue.h"

using namespace std;

namespace CS3358_SP2019_A7
{
    // EXTRA MEMBER FUNCTIONS FOR DEBUG PRINTING
    void p_queue::print_tree(const char message[], size_type i) const
    // Pre:  (none)
    // Post: If the message is non-empty, it has first been written to
    //       cout. After that, the portion of the heap with root at
    //       node i has been written to the screen. Each node's data
    //       is indented 4*d, where d is the depth of the node.
    //       NOTE: The default argument for message is the empty string,
    //             and the default argument for i is zero. For example,
    //             to print the entire tree of a p_queue p, with a
    //             message of "The tree:", you can call:
    //                p.print_tree("The tree:");
    //             This call uses the default argument i=0, which prints
    //             the whole tree.
    {
        const char NO_MESSAGE[] = "";
        size_type depth;
        
        if (message[0] != '\0')
            cout << message << endl;
        
        if (i >= used)
            cout << "(EMPTY)" << endl;
        else
        {
            depth = size_type( log( double(i+1) ) / log(2.0) + 0.1 );
            if (2*i + 2 < used)
                print_tree(NO_MESSAGE, 2*i + 2);
            cout << setw(depth*3) << "";
            cout << heap[i].data;
            cout << '(' << heap[i].priority << ')' << endl;
            if (2*i + 1 < used)
                print_tree(NO_MESSAGE, 2*i + 1);
        }
    }
    
    void p_queue::print_array(const char message[]) const
    // Pre:  (none)
    // Post: If the message is non-empty, it has first been written to
    //       cout. After that, the contents of the array representing
    //       the current heap has been written to cout in one line with
    //       values separated one from another with a space.
    //       NOTE: The default argument for message is the empty string.
    {
        if (message[0] != '\0')
            cout << message << endl;
        
        if (used == 0)
            cout << "(EMPTY)" << endl;
        else
            for (size_type i = 0; i < used; i++)
                cout << heap[i].data << ' ';
    }
    
    // CONSTRUCTORS AND DESTRUCTOR
    
    p_queue::p_queue(size_type initial_capacity) : capacity(initial_capacity), used(0)
    {
        if (capacity == 0)
        {
            capacity = DEFAULT_CAPACITY; // Sets cap to 1 if user entered 0
        }
        heap = new ItemType[capacity]; // Initializing the heap
        
    }
    
    p_queue::p_queue(const p_queue& src) : capacity(src.capacity), used(src.used)
    {
        heap = new ItemType[capacity]; // Initializing the new heap
        
        for (size_type i = 0; i < used; ++i) // Copying the heap
        {
            heap[i] = src.heap[i];
        }
        
    }
    
    p_queue::~p_queue()
    {
        delete [] heap;
        
        heap = 0;
        
    }
    
    // MODIFICATION MEMBER FUNCTIONS
    p_queue& p_queue::operator=(const p_queue& rhs)
    {
        if (this != &rhs) // Trapping self-assignment
        {
            resize(rhs.capacity); // Resizing array to fit any new data
        }
        used = rhs.used;
        
        for (size_type i = 0; i < used; ++i)
        {
            heap[i] = rhs.heap[i];
        }
        return *this;
    }
    
    void p_queue::push(const value_type& entry, size_type priority)
    {
        if (used == capacity) // Case where resize is required
        {
            resize(capacity * 2);
        }
        size_type newEntryIndex = used;
        heap[newEntryIndex].data = entry;
        heap[newEntryIndex].priority = priority; // Adding data at the end of the tree
        
        ++used; // Incrementing used to indicate the added data
        
        while(newEntryIndex != 0 && heap[newEntryIndex].priority > parent_priority(newEntryIndex))
        {
            swap_with_parent(newEntryIndex); // Swapping parent when new priority is >
            newEntryIndex = parent_index(newEntryIndex); // Staying with new entry }
        }
    }
    
    void p_queue::pop()
    {
        heap[0] = heap[used-1]; // Replacing first value with last
        --used; // Decrementing used to indicate loss of an element
        
        if(used == 0) // Special case when popping last item
        {
            return;
        }
        size_type newEntryIndex = 0; // Creating an index to follow the new entry
        
        while(!is_leaf(newEntryIndex) && big_child_priority(newEntryIndex) > heap[newEntryIndex].priority)
        {
            newEntryIndex = big_child_index(newEntryIndex); // Moving index to child
            
            swap_with_parent(newEntryIndex); // Swapping the larger priority up }
        }
    }
    
    // CONSTANT MEMBER FUNCTIONS
    
    p_queue::size_type p_queue::size() const
    {
        return used; // Used reflectes the number of items in the p_queue
    }
    
    bool p_queue::empty() const
    {
        return(used == 0); // Empty if used is 0
    }
    
    p_queue::value_type p_queue::front() const
    {
        assert (size() > 0); // Asserting precondition
        
        return(heap[0].data); // Returning the top of the queue
        
    }
    
    // PRIVATE HELPER FUNCTIONS
    void p_queue::resize(size_type new_capacity)
    // Pre:  (none)
    // Post: The size of the dynamic array pointed to by heap (thus
    //       the capacity of the p_queue) has been resized up or down
    //       to new_capacity, but never less than used (to prevent
    //       loss of existing data).
    //       NOTE: All existing items in the p_queue are preserved and
    //             used remains unchanged.
    {
        ItemType* tempHeap = 0; // Temp store of heap
        size_type tempCap = new_capacity;
        
        if (used == 0 && tempCap == used)
        {
            delete [] heap;
            tempCap = 1;
            heap = new ItemType[tempCap]; //Allocating array at 1 element
        }
        
        else if (tempCap > used)
        {
            tempHeap = heap; // Storing heap
            heap = new ItemType[tempCap]; //Allocating new array
            for (size_type i = 0; i < used; ++i)
            {
                heap[i] = tempHeap[i]; // Adding data back into heap
                
            }
            
            delete [] tempHeap;
            
        }
        
        else // tempCap <= used
        {
            tempCap = used; // Temp cap is set to used b/c cap cannot be less than used
            tempHeap = heap; // Storing heap
            heap = new ItemType[tempCap]; // Allocating new array
            for (size_type i = 0; i < used; i++)
            {
                heap[i] = tempHeap[i]; // Adding data back into heap
                
            }
            
            delete [] tempHeap;
            
        }
        capacity = tempCap; // Reflecting change in capacity
        
    }
    
    bool p_queue::is_leaf(size_type i) const
    // Pre:  (i < used)
    // Post: If the item at heap[i] has no children, true has been
    //       returned, otherwise false has been returned.
    {
        assert(i < used); // Asserting precondition
        return !((i*2 + 1) < used);
    }
    
    p_queue::size_type
    p_queue::parent_index(size_type i) const
    // Pre:  (i > 0) && (i < used)
    // Post: The index of "the parent of the item at heap[i]" has
    //       been returned.
    {
        assert(i > 0);
        assert(i < used); // Asserting preconditions
        return((i-1)/2); // (i-1)/2 is the parent location
    }
    
    p_queue::size_type
    p_queue::parent_priority(size_type i) const
    // Pre:  (i > 0) && (i < used)
    // Post: The priority of "the parent of the item at heap[i]" has
    //       been returned.
    {
        assert(i > 0);
        assert(i < used); // Asserting preconditions
        return(heap[parent_index(i)].priority);
    }
    
    p_queue::size_type
    p_queue::big_child_index(size_type i) const
    // Pre:  is_leaf(i) returns false
    // Post: The index of "the bigger child of the item at heap[i]"
    //       has been returned.
    //       (The bigger child is the one whose priority is no smaller
    //       than that of the other child, if there is one.)
    {
        assert(!is_leaf(i)); // Asserting precondition
        size_type left = 2 * i + 1;
        size_type right = 2 * i + 2; // Holders for left and right children
        if (right < used && heap[right].priority > heap[left].priority)
        {
            return right; // Right is used and priority is > left
        }
        else
        {
            return left; // Right either doesnt exist or priority < left
        }
        
    }
    
    p_queue::size_type
    p_queue::big_child_priority(size_type i) const
    // Pre:  is_leaf(i) returns false
    // Post: The priority of "the bigger child of the item at heap[i]"
    //       has been returned.
    //       (The bigger child is the one whose priority is no smaller
    //       than that of the other child, if there is one.)
    {
        assert(!is_leaf(i)); // Asserting precondition
        return(heap[big_child_index(i)].priority); // Calls other helper function
        
    }
    void p_queue::swap_with_parent(size_type i)
    // Pre:  (i > 0) && (i < used)
    // Post: The item at heap[i] has been swapped with its parent.
    {
        assert(i > 0 && i < used); // Asserting preconditions
        
        ItemType tempItem = heap[i]; // Storing item
        heap[i] = heap[parent_index(i)]; // Inserting parent in child
        heap[parent_index(i)] = tempItem; // Inserting child in parent
    }
}
