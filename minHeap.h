// {
//     CREATED BY: CHINTAN ACHARYA
//     DATE: 6 APRIL 2023
// }

#include <vector>

using namespace std;

// STORES THE DATA OF THE HEAP NODE
class MinHeapNode
{
public:
    int rideNumber;
    int rideCost;
    int tripDuration;
    MinHeapNode(int rn, int rc, int td) : rideNumber(rn), rideCost(rc), tripDuration(td) {}
};

class MinHeap
{
    // DEFINING THE BASIC HEAP FUNCTIONS
private:
    vector<MinHeapNode> heap;
    int left(int i) { return 2 * i + 1; }
    int right(int i) { return 2 * i + 2; }
    int parent(int i) { return (i - 1) / 2; }
    void swap(MinHeapNode &a, MinHeapNode &b)
    {
        MinHeapNode temp = a;
        a = b;
        b = temp;
    }

public:
    // INSERTS INTO HEAP
    void insert(MinHeapNode &node)
    {
        heap.push_back(node);
        int i = heap.size() - 1;
        while (i > 0 && heap[i].rideCost < heap[parent(i)].rideCost ||
               (heap[i].rideCost == heap[parent(i)].rideCost && heap[i].tripDuration < heap[parent(i)].tripDuration))
        {
            swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    // EXTRACT MIN FROM THE HEAP
    MinHeapNode extractMin()
    {
        MinHeapNode root = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        int i = 0;
        while (left(i) < heap.size())
        {
            int j = left(i);
            if (right(i) < heap.size() &&
                (heap[right(i)].rideCost < heap[j].rideCost ||
                 (heap[right(i)].rideCost == heap[j].rideCost && heap[right(i)].tripDuration < heap[j].tripDuration)))
            {
                j = right(i);
            }
            if (heap[i].rideCost < heap[j].rideCost ||
                (heap[i].rideCost == heap[j].rideCost && heap[i].tripDuration <= heap[j].tripDuration))
            {
                break;
            }
            swap(heap[i], heap[j]);
            i = j;
        }
        return root;
    }

    // RETURNS THE MIN NODE
    MinHeapNode getMin()
    {
        return heap[0];
    }

    // RETURNS IS EMPTY
    bool isEmpty()
    {
        return heap.size() == 0;
    }

    // DELETE HEAP NODE BY RIDENUMBER
    void deleteHeapNode(int rideNumber)
    {
        int index = -1;
        for (int i = 0; i < heap.size(); i++)
        {
            if (heap[i].rideNumber == rideNumber)
            {
                index = i;
                break;
            }
        }
        if (index == -1)
        {
            return;
        }
        heap[index] = heap.back();
        heap.pop_back();
        int i = index;
        while (i > 0 && heap[i].rideCost < heap[parent(i)].rideCost ||
               (heap[i].rideCost == heap[parent(i)].rideCost && heap[i].tripDuration < heap[parent(i)].tripDuration))
        {
            swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
        while (left(i) < heap.size())
        {
            int j = left(i);
            if (right(i) < heap.size() &&
                (heap[right(i)].rideCost < heap[j].rideCost ||
                 (heap[right(i)].rideCost == heap[j].rideCost && heap[right(i)].tripDuration < heap[j].tripDuration)))
            {
                j = right(i);
            }
            if (heap[i].rideCost < heap[j].rideCost ||
                (heap[i].rideCost == heap[j].rideCost && heap[i].tripDuration <= heap[j].tripDuration))
            {
                break;
            }
            swap(heap[i], heap[j]);
            i = j;
        }
    }
};
