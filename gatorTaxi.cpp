// {
//     Created by: Chintan Acharya,
//     Date: 6 April 2023
// }

#include <iostream>
#include <vector>
#include <fstream>
#include "minHeap.h"
#include "RBT.h"

using namespace std;

// A RIDE IS INDENTIFIED BY THE FOLLOWING TRIPLET:
// rideNumber: UNIQUE INTEGER IDENTIFIER FOR EACH RIDE
// rideCost: THE ESTIMATED COST (IN INTEGER DOLLARS) FOR THE RIDE
// tripDuration: THE TOTAL TIME (IN INTEGER MINUTES) NEEDED TO GET FROM PICKIP TO DESTINATION
struct Ride
{
    int rideNumber;
    int rideCost;
    int tripDuration;
    MinHeapNode *heapNode;
    NodePtr RBTNode;

    bool operator<(const Ride &other) const
    {
        if (rideCost == other.rideCost)
        {
            return tripDuration < other.tripDuration;
        }
        return rideCost < other.rideCost;
    }
};

class GatorTaxi
{
public:
    vector<int> ride_to_node_;
    unordered_map<int, Ride> mp;
    RBTree rbt_;
    MinHeap heap_;
    ofstream myfile;

    // INSERTS INTO THE RB-TREE AND MINHEAP.
    // ALSO CHECKS FOR DUPLICATE RIDE NUMBER
    bool Insert(int rideNumber, int rideCost, int tripDuration)
    {
        // CREATE A NEW RIDE AND ADD IT TO BOTH THE MIN-HEAP AND RBT
        auto it = rbt_.searchTree(rideNumber);
        if (it->rideNumber == 0)
        {
            MinHeapNode mh = MinHeapNode(rideNumber, rideCost, tripDuration);
            heap_.insert(mh);
            NodePtr RBTNode = rbt_.insert(rideNumber, rideCost, tripDuration);
            Ride ride = {rideNumber, rideCost, tripDuration, &mh, RBTNode};
            ride_to_node_.push_back(rideNumber);
            mp.insert({rideNumber, ride});
            return true;
        }
        else
        {
            myfile << "Duplicate Ride Number";
            return false;
        }
    }

    // CANCEL RIDE - DELETES NODES FROM BOTH DATA STRUCTURES
    void CancelRide(int rideNumber)
    {
        bool found = false;

        for (int i = 0; i < ride_to_node_.size(); i++)
        {
            if (ride_to_node_[i] == rideNumber)
            {
                found = true;
            }
        }

        if (found)
        {
            heap_.deleteHeapNode(rideNumber);
            rbt_.deleteNode(rideNumber);
            ride_to_node_.erase(std::remove(ride_to_node_.begin(), ride_to_node_.end(), rideNumber), ride_to_node_.end());
            mp.erase(rideNumber);
        }
    }

    // UPDATES TRIP DURATION
    void UpdateTrip(int rideNumber, int newTripDuration)
    {

        bool found = false;

        for (int i = 0; i < ride_to_node_.size(); i++)
        {
            if (ride_to_node_[i] == rideNumber)
            {
                found = true;
            }
        }

        if (found)
        {
            Ride ride = mp[rideNumber];
            if (newTripDuration > 2 * ride.tripDuration)
            {
                // IF THE NEW TRIP IS MORE THAN DOUBLE THE EXISTING DURATION, CANCEL THE RIDE
                CancelRide(rideNumber);
            }
            else if (newTripDuration > ride.tripDuration)
            {
                // IF THE NEW TRIP IS MORE THAN EXISTING DURATION BUT LESS THAN DOUBLE, UPDATE THE RIDE WITH RIDE COST PENALTY
                int newRideCost = ride.rideCost + 10;
                MinHeapNode mh = MinHeapNode(rideNumber, newRideCost, newTripDuration);
                heap_.deleteHeapNode(rideNumber);
                heap_.insert(mh);
                rbt_.deleteNode(rideNumber);
                NodePtr newNode = rbt_.insert(rideNumber, newRideCost, newTripDuration);
                Ride newRide = {rideNumber, newRideCost, newTripDuration, &mh, newNode};
                mp[rideNumber] = newRide;
            }
        }
        else
        {
            myfile << "No Ride with this number";
        }
    }

    // PRINT SINGLE RIDE
    void Print(int rideNumber)
    {
        auto it = rbt_.searchTree(rideNumber);
        if (it->rideNumber != 0)
        {
            myfile << "(" << it->rideNumber << ", " << it->rideCost << ", " << it->tripDuration << ")" << endl;
        }
        else
        {
            myfile << "(0, 0, 0)" << endl;
        }
    }

    // PRINT RIDES BETWEEN RANGE RIDENUMBER1 AND RIDENUMBER2
    void Print(int rideNumber1, int rideNumber2)
    {
        vector<NodePtr> rides = rbt_.rangeSearch(rideNumber1, rideNumber2);
        string out = "";
        for (auto &ride : rides)
        {
            out += "(" + to_string(ride->rideNumber) + ", " + to_string(ride->rideCost) + ", " + to_string(ride->tripDuration) + "),";
        }
        if (rides.size() == 0)
        {
            myfile << "(0, 0, 0)";
        }
        else
        {
            myfile << out.substr(0, out.length() - 1);
        }
        myfile << endl;
    }

    // GETS THE RIDE WITH THE SMALLEST RIDE COST
    // USES MIN-HEAP ATTRIBUTES
    void GetNextRide()
    {
        int min = INT_MAX;
        Ride minRide;
        if (ride_to_node_.size() > 0)
        {
            MinHeapNode min = heap_.getMin();
            myfile << "(" << min.rideNumber << ", " << min.rideCost << ", " << min.tripDuration << ")" << endl;
            mp.erase(min.rideNumber);
            CancelRide(min.rideNumber);
        }
        else
        {
            myfile << "No active ride requests" << endl;
        }
    }
};

int main()
{

    std::vector<std::string> v;
    std::vector<std::string> ans;
    std::string line;
    ifstream myfile("input.txt");
    GatorTaxi gt;

    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            v.push_back(line);
        }
        myfile.close();
    }

    gt.myfile.open("output.txt");
    for (auto line : v)
    {
        string operation = "";
        int rideNumber, rideCost, tripDuration, newDuration, ride1 = -1, ride2 = -1;
        for (int i = 0; i < line.length(); i++)
        {
            if (line[i] == '(')
            {
                break;
            }
            else
            {
                operation += line[i];
            }
        }

        if (operation == "Insert")
        {
            int count = 0;
            string temp = "";
            for (int i = 7; i < line.length(); i++)
            {
                if (line[i] == ',')
                {
                    count += 1;
                    if (count == 1)
                    {
                        rideNumber = stoi(temp);
                    }
                    else if (count == 2)
                    {
                        rideCost = stoi(temp);
                    }
                    temp = "";
                }
                else if (line[i] == ')')
                {
                    tripDuration = stoi(temp);
                    temp = "";
                }
                else
                {
                    temp += line[i];
                }
            }
            bool res = gt.Insert(rideNumber, rideCost, tripDuration);
            if (!res)
            {
                break;
            }
        }
        else if (operation == "CancelRide")
        {
            string temp = "";
            for (int i = 11; i < line.length(); i++)
            {
                if (line[i] == ')')
                {
                    break;
                }
                else
                {
                    temp += line[i];
                }
            }
            rideNumber = stoi(temp);
            gt.CancelRide(rideNumber);
        }
        else if (operation == "UpdateTrip")
        {

            string temp = "";
            for (int i = 11; i < line.length(); i++)
            {
                if (line[i] == ',')
                {

                    rideNumber = stoi(temp);
                    temp = "";
                }
                else if (line[i] == ')')
                {
                    newDuration = stoi(temp);
                    temp = "";
                }
                else
                {
                    temp += line[i];
                }
            }
            gt.UpdateTrip(rideNumber, newDuration);
        }
        else if (operation == "Print")
        {
            string temp = "";
            for (int i = 6; i < line.length(); i++)
            {
                if (line[i] == ',')
                {
                    ride1 = stoi(temp);
                    temp = "";
                }
                else if (line[i] == ')')
                {
                    if (ride1 == -1)
                    {
                        ride1 = stoi(temp);
                        temp = "";
                    }
                    else
                    {
                        ride2 = stoi(temp);
                        temp = "";
                    }
                }
                else
                {
                    temp += line[i];
                }
            }
            if (ride2 == -1)
            {
                gt.Print(ride1);
            }
            else
            {
                gt.Print(ride1, ride2);
            }
        }
        else if (operation == "GetNextRide")
        {
            gt.GetNextRide();
        }
    }
    gt.myfile.close();
    return 0;
}