//
//  main.cpp
//  Distribution
//
//  Created by YOUQingfei on 2/13/16.
//  Copyright © 2016 YOUQingfei. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

class Position
{
public:
    int x;
    int y;
};

class Product
{
public:
    int id;
    int weight;
    Product(int id, int weight):id(id), weight(weight){}
};

class Drone
{
public:
    int id;
    
};

class Warehouse
{
public:
    int id;
    Position position;
    map<int, int> remains;
};

class Order
{
public:
    int id;
    int totalQuantity;
    int t;
    Position position;
    map<int, int> demands;
};

const string FileName = "/Users/YQF/Documents/Google_HashCode/Distribution/Problem/mother_of_all_warehouses.in";
const string OutputName = "/Users/YQF/Documents/Google_HashCode/Distribution/Problem/mother_of_all_warehouses.txt";
fstream file, output;

vector<Drone> drones;
vector<Warehouse> warehouses;
vector<Order> orders;
vector<Product> products;

int row, col, totalDrone, totalProduct, totalWarehouse, totalOrder, T, maxLoad;


void LoadData()
{
    stringstream ss;
    string line;
    getline(file, line);
    ss << line;
    ss >> row >> col >> totalDrone >> T >> maxLoad;
    ss.clear();
    
    
    getline(file, line);
    ss << line;
    ss >> totalProduct;
    ss.clear();
    
    
    getline(file, line);
    ss << line;
    int weight;
    for (int i = 0; i < totalProduct; ++i)
    {
        ss >> weight;
        Product p (i, weight);
        products.push_back(p);
    }
    ss.clear();
    
    
    getline(file, line);
    ss << line;
    ss >> totalWarehouse;
    ss.clear();
    
    
    for (int i = 0;i < totalWarehouse; ++i)
    {
        Warehouse w;
        w.id = i;
        getline(file, line);
        ss << line;
        ss >> w.position.x >> w.position.y;
        ss.clear();
        
        getline(file, line);
        ss << line;
        for (int j = 0;j < totalProduct; ++j)
        {
            ss >> w.remains[j];
        }
        ss.clear();
        
        warehouses.push_back(w);
    }
    
    
    getline(file, line);
    ss << line;
    ss >> totalOrder;
    ss.clear();
    
    for (int i = 0; i < totalOrder; ++i)
    {
        Order o;
        o.id = i;
        getline(file, line);
        ss << line;
        ss >> o.position.x >> o.position.y;
        ss.clear();
        
        getline(file, line);
        ss << line;
        ss >> o.totalQuantity;
        ss.clear();
        
        getline(file, line);
        ss << line;
        int pId = 0;
        for (int j = 0; j < o.totalQuantity; ++j)
        {
            ss >> pId;
            if (o.demands.find(pId) == o.demands.end()) o.demands.insert({pId, 1});
            else o.demands[pId]++;
            
            ss.clear();
        }
        
        orders.push_back(o);
    }
    
}

int distance(Position p1, Position p2)
{
    return 0.5 + sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}

void Load(int drone, int warehouse, int product, int quantity)
{
    cout << drone << " L " << warehouse << " " << product << " " << quantity << endl;
    output << drone << " L " << warehouse << " " << product << " " << quantity << endl;
}

void Deliver(int drone, int order, int product, int quantity)
{
    cout << drone << " D " << order << " " << product << " " << quantity << endl;
    output << drone << " D " << order << " " << product << " " << quantity << endl;
}

bool compT(const Order & o1, const Order & o2)
{
    return o1.t < o2.t;
}

bool compQty(const Order & o1, const Order & o2)
{
    return o1.totalQuantity < o2.totalQuantity;
}

int main(int argc, const char * argv[]) {
    file.open(FileName);
    LoadData();
    
    output.open(OutputName);

    int totalTime = 0, time = 0;
    
    for (auto & order : orders)
    {
        time = 0;
        int drone = 0;
        for (auto it = order.demands.begin(); it != order.demands.end(); ++it)
        {
            time++; totalTime++; drone++;
        }
        
        drone = 0;
        for (auto it = order.demands.begin(); it != order.demands.end(); ++it)
        {
            int dis = distance(warehouses[0].position, order.position);
            time += dis; totalTime += dis; drone++;
        }
        order.t = time;
    }
    cout << totalTime << endl;
    
    sort(orders.begin(), orders.end(), compT);
    int lines = 0;
    totalTime = 0;
    for (auto & order : orders)
    {
        int drone = 0;
        for (auto it = order.demands.begin(); it != order.demands.end(); ++it)
        {
            if (totalTime >= T) break;
            Load(drone, 0, it->first, it->second);
            totalTime++; drone++; lines++;
        }
        
        drone = 0;
        for (auto it = order.demands.begin(); it != order.demands.end(); ++it)
        {
            if (totalTime >= T) break;
            Deliver(drone, order.id, it->first, it->second);
            int dis = distance(warehouses[0].position, order.position);
            totalTime += dis; drone++; lines++;
        }
    }

    
    cout << totalTime << " " << lines << endl;
    
    output.close();
    file.close();
    return 0;
}
