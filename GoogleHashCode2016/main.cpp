// basic file operations
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>

using namespace std;

struct warehouse_T
{
    int row ;
    int col;
    vector<int> vNbProducts;
    vector<int> vMaxProducts;
};

struct order_T
{
    int row;
    int col;
    int nbItems;
    vector<int> nbItemsPerType;


};

struct action_T
{
    int idDrone;
    char actionType;
    int idTarget;
    int idProductType;
    int nbProduct;
    int time;
};

static int nbProductType,nbWarehouse,nbOrder,nbDrone,nbCol,nbRow,maxPayload, nbTurn;

int main()
{
    ifstream myFile;
    string firstLine;
    string line;



    myFile.open ("mother_of_all_warehouses.in");
    myFile >> nbRow >> nbCol >> nbDrone >> nbTurn >> maxPayload >> nbProductType;

    vector<int> weightOfProduct(nbProductType);
    for (int k=0; k<nbProductType; k++)
    {
        myFile >> weightOfProduct[k];
    }

    myFile >> nbWarehouse;


    vector<warehouse_T> vWarehouse(nbWarehouse);
    for (int k=0; k<nbWarehouse; k++)
    {
        myFile >> vWarehouse[k].row >>vWarehouse[k].col;
        vWarehouse[k].vNbProducts.resize(nbProductType);
        vWarehouse[k].vMaxProducts.resize(nbProductType);

        for (int product=0; product<nbProductType; product++)
        {
            myFile >> vWarehouse[k].vNbProducts[product];
            vWarehouse[k].vMaxProducts[product]=
                vWarehouse[k].vNbProducts[product];

        }
    }

    myFile >> nbOrder;
    vector<order_T> vOrders(nbOrder);
    for (int order=0; order<nbOrder; order++)
    {

        myFile >> vOrders[order].row >> vOrders[order].col >>vOrders[order].nbItems;
        vOrders[order].nbItemsPerType.resize(nbProductType);

        for (int itemNb=0; itemNb<vOrders[order].nbItems; itemNb++)
        {
            int productType(0);
            myFile >> productType;
            vOrders[order].nbItemsPerType[productType]++;
        }

    }


    myFile.close();

    return 0;
}


void actionToFile(vector<action_T> vActions)
{
    ofstream outputFile;
    outputFile.open("mother_of_all_warehouses.out");
    for (int numAction=0; numAction<vActions.size(); numAction++)
    {
        outputFile << vActions[numAction].idDrone << " ";
        outputFile << vActions[numAction].actionType << " ";
        if (vActions[numAction].actionType=='W')
            outputFile << vActions[numAction].time;
        else
        {
            outputFile << vActions[numAction].idTarget << " ";
            outputFile << vActions[numAction].idProductType << " ";
            outputFile << vActions[numAction].nbProduct;
        }


        outputFile << "\n";
    }
    outputFile.close();
}
