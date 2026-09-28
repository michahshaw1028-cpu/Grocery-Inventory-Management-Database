#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <variant>
#include <format>
using namespace std;

class Item
{ // CREATE ITEM CLASS
public:
    Item(std::string n, std::string d, std::string e, string t, std::string u, string i, int q, int s, int c, int z, double p)
    {
        name = n;
        department = d;
        img_url = e;
        tcin = t;
        upc = u;
        dpci = i;
        quantity = q;
        salesfloor_quantity = s;
        capacity = c;
        case_size = z;
        price = p;

    } // constructor
    std::string getName()
    {
        return name;
    }

    std::string getDepartment()
    {
        return department;
    }

    std::string getImg()
    {
        return img_url;
    }
    std::string getUPC()
    {
        return upc;
    }
    string getTCIN()
    {
        return tcin;
    }

    std::string getDPCI()
    {
        return dpci;
    }

    int getCaseSize()
    {
        return case_size;
    }
    int getQuantity()
    {
        return quantity;
    }
    int getSalesFloorQuantity()
    {
        return salesfloor_quantity;
    }
    int getCapacity()
    {
        return capacity;
    }

    double getPrice()
    {
        return price;
    }
    void setCapacity(int c)
    {
        capacity = c;
    }
    void setName(std::string n)
    {
        name = n;
    }

    void setDepartment(std::string d)
    {
        department = d;
    }

    void setTCIN(string t)
    {
        tcin = t;
    }
    void setUpC(string u)
    {
        upc = u;
    }

    void setDPCI(string i)
    {
        dpci = i;
    }

    void setQuantity(int q)
    {
        quantity = q;
    }

    void setSalesFloorQuantity(int s)
    {
        salesfloor_quantity = s;
    }

    void setCaseSize(int z)
    {
        case_size = z;
    }

    void setPrice(int p)
    {
        price = p;
    }

    void printItem()
    {
        std::cout << "Name: " << this->getName() << std::endl;
        std::cout << "Department: " << this->getDepartment() << std::endl;
        std::cout << "DPCI: " << this->getDPCI() << std::endl;
        std::cout << "Quantity: " << this->getQuantity() << std::endl;
        std::cout << "Price: $" << this->getPrice() << std::endl;
    }

private:
    std::string name, department, img_url, upc, dpci, tcin; // VARIABLES FOR IDENTIFACTION PURPOSE
    int quantity, salesfloor_quantity, capacity, case_size;
    double price;
};

std::vector<Item *> priorities;
std::vector<Item> storeCatalogue;
void addItem(Item item)
{
    storeCatalogue.push_back(item);
}

void prioritizeItem(Item &item)
{
    double sf_quantity = static_cast<double>(item.getSalesFloorQuantity());
    double it_capacity = static_cast<double>(item.getCapacity());
    if (sf_quantity / it_capacity < 0.7)
    {
        priorities.push_back(&item);
    }
}

// EMPTY RESTOCK LIST
void clearRestock()
{
    priorities.clear();
}

void restockItem(Item &item) {
    // RESTOCK UNDERSTOCKED ITEMS (PRIORITIES)
    int salesfloorQuantity = item.getSalesFloorQuantity();
    int quantity = item.getQuantity();
    bool inBackroom = salesfloorQuantity < quantity; // CHECK INVENTORY FOR ITEM
    bool outOfStock = salesfloorQuantity == 0;
    int unitsRestocked = 0;
    // PRINT ITEM'S STATUS
    std::string status;
    if(outOfStock) {
        status = "***OUT OF STOCK*** ";
    }


    if (inBackroom) {
        int backroomQuantity = item.getQuantity() - item.getSalesFloorQuantity();
        int amountNeeded = item.getCapacity() - item.getSalesFloorQuantity();
        double percentFull = static_cast<double>(item.getCapacity()) / static_cast<double>(item.getSalesFloorQuantity());
        int caseSize = item.getCaseSize();
        int cases = (amountNeeded >= caseSize) ? amountNeeded / caseSize : 0;
        int units = ((amountNeeded % (cases * caseSize)) == 0) ?  0 : amountNeeded - (cases * caseSize);
        unitsRestocked = (cases * caseSize) + units;

        if (cases > 1 && units > 1)
        {
            status += "PULL " + to_string(cases) + " CASES, " + to_string(units) + " UNITS";
        }
        else if (cases > 0 || units > 0)
        {
            if (cases == 1 && units == 1)
            {
                status += "PULL " + to_string(cases) + " CASE, " + to_string(units) + " UNIT";
            }
            else if (units == 0)
            {
                if (cases > 1)
                {
                    status += "PULL " + to_string(cases) + " CASES";
                }
                else
                {
                    status += "PULL " + to_string(cases) + " CASE";
                }
            }
            else if (cases == 0)
            {
                if (units > 1)
                {
                    status += "PULL " + to_string(units) + " UNITS";
                }
                else
                {
                    status += "PULL " + to_string(units) + " UNIT";
                }
            }
            else if (units == 1)
            {
                status += "PULL " + to_string(cases) + " CASES, " + to_string(units) + " UNIT";
            }
            else if (cases == 1)
            {
                status += "PULL " + to_string(cases) + " CASE, " + to_string(units) + " UNITS";
            }
        }
        else
        {
            if (units == 0 && !(cases == 0))
            {
                status += "PULL " + to_string(cases) + " CASES";
            }
            else if (cases == 0 && !(units == 0))
            {
                status += "PULL " + to_string(units) + " UNITS";
            }
            else
            {
                status += "INVENTORY EMPTY";
            }
        }
    }
    else
    {
        status += "INVENTORY EMPTY";
    }

    // FILL ITEM TO 100%
    item.setSalesFloorQuantity(salesfloorQuantity + unitsRestocked);
    // PRINT ITEM STATUS
    std::cout << item.getName() <<  " | PRICE: $" << item.getPrice() << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << status << std::endl;
    std::cout << std::endl;
}

Item createItem(std::string &line)
{
    // CREATE ITEMS

    string n, dep, url, u, t, d; // NAME, DEPARTMENT, IMAGE URL, UPC, TCIN, DPCI
    int location, q, s, c, cs;   // QUANTITY, SALES FLOOR QUANTITY, CAPACITY, CASE SIZE
    double p;                    // PRICE

    // SET NAME
    location = line.find(',');
    n = line.substr(0, location);
    line = line.substr(++location, line.length());

    // SET DEPARTMENT
    location = line.find(',');
    dep = line.substr(0, location);
    line = line.substr(++location, line.length());

    // SET URL
    location = line.find(',');
    url = line.substr(0, location);
    line = line.substr(++location, line.length());

    // SET TCIN
    location = line.find(',');
    t = line.substr(0, location);
    line = line.substr(++location, line.length());

    // SET UPC
    location = line.find(',');
    u = line.substr(0, location);
    line = line.substr(++location, line.length());

    // SET DPCI
    location = line.find(',');
    d = line.substr(0, location);
    line = line.substr(++location, line.length());

    // SET STORE QUANTITY
    location = line.find(',');
    q = stoi(line.substr(0, location));
    line = line.substr(++location, line.length());

    // SET SALES FLOOR QUANTITY
    location = line.find(',');
    s = stoi(line.substr(0, location));
    line = line.substr(++location, line.length());

    // SET CAPACITY
    location = line.find(',');
    c = stoi(line.substr(0, location));
    line = line.substr(++location, line.length());

    // SET CASE SIZE
    location = line.find(',');
    cs = stoi(line.substr(0, location));
    line = line.substr(++location, line.length());

    // SET PRICE
    location = line.find(',');
    p = stod(line.substr(0, location));
    line = line.substr(++location, line.length());

    return Item(n, dep, url, t, u, d, q, s, c, cs, p);
}

int main()
{
    // OPEN CSV DATA
    std::ifstream file("grocery_data.csv");
    std::string line;
    std::vector<std::string> lines;

    if (!file.is_open())
    {
        std::cerr << "ERROR: FILE NOT OPENED" << std::endl;
        return 1;
    }

    std::getline(file, line); // SKIP HEADER LINE

    // READ CSV FILE, CREATE OBJECTS
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }

    for (auto &line : lines)
    {
        Item item = createItem(line);
        addItem(item);
    }

    // CREATE RESTOCK LIST
    for (auto &item : storeCatalogue)
    {
        prioritizeItem(item);
    }
 
    // RESTOCK INVENTORY
    for (auto item : priorities) {
        restockItem(*item);
    }

    // CLEAR RESTOCK LIST
    clearRestock();

    // CHECK RESTOCK LIST
    for (auto &item : storeCatalogue) {
        prioritizeItem(item);
    }

    std::cout << "# OF UNDERSTOCKED INVENTORY AFTER RESTOCK: " << priorities.size() << std::endl;
    std::cout << std::endl;
    std::cout << "ITEMS STILL UNDERSTOCKED: " << std::endl;
    std::cout << std::endl;
    for (auto item : priorities) {
        restockItem(*item);
    }

    return 0;
};