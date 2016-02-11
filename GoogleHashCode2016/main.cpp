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
    int weight;
    map<int,int> nbByType;


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



void actionToFile(vector<vector<action_T> > vActions)
{
    ofstream outputFile;
    outputFile.open("mother_of_all_warehouses.out");
    int nbCommands;
    for (int numDrone=0; numDrone<vActions.size(); numDrone++)
    {
        nbCommands += vActions[numDrone].size();
    }
    outputFile << nbCommands << " " << nbDrone << " " << 142000 << "\n";
    for (int numDrone=0; numDrone<vActions.size(); numDrone++)
    {

        for (int numAction=0; numAction<vActions[numDrone].size(); numAction++)
        {
            outputFile << vActions[numDrone][numAction].idDrone << " ";
            outputFile << vActions[numDrone][numAction].actionType << " ";
            if (vActions[numDrone][numAction].actionType=='W')
                outputFile << vActions[numDrone][numAction].time;
            else
            {
                outputFile << vActions[numDrone][numAction].idTarget << " ";
                outputFile << vActions[numDrone][numAction].idProductType << " ";
                outputFile << vActions[numDrone][numAction].nbProduct;
            }


            outputFile << "\n";
        }

    }
    outputFile.close();
}

vector<action_T> orderToActions(int idOrder, vector<order_T> vOrders,vector<int> weightOfProduct,int idDrone)
{
    order_T order = vOrders[idOrder];
    vector<action_T> results;
    while (order.weight>0)
    {
        int accWeight(0);
        int nbItem(0);
        map<int,int>::iterator it(order.nbByType.begin());
        int idProductType(it->first);
        while (accWeight<=maxPayload-weightOfProduct[it->first])
        {
            accWeight += weightOfProduct[it->first];
            order.weight -= weightOfProduct[it->first];
            nbItem++;
            if (it->second==1)
            {
                order.nbByType.erase(it->first);
                break;
            }
            else
            {
                (it->second)--;
            }
        }
        action_T load;
        load.actionType ='L';
        load.idDrone = idDrone;
        load.idProductType =  idProductType;
        load.idTarget = 0;
        load.nbProduct = nbItem;
        load.time = 0;
        action_T deliver;
        deliver.actionType ='D';
        deliver.idDrone = idDrone;
        deliver.idProductType =  idProductType;
        deliver.idTarget = idOrder;
        deliver.nbProduct = nbItem;
        deliver.time = 0;
        results.push_back(load);
        results.push_back(deliver);

    }
    return results;
}

vector<vector<action_T> > initialSolution(vector<int> weightOfProduct,vector<order_T> vOrders,
        vector<warehouse_T> vWarehouse)
{
    vector<vector<action_T> > droneActions (nbDrone);
    int droneID=0;
    cout<< nbOrder << endl;
    for(int orderID = 0; orderID<nbOrder; orderID++)
    {

        vector<action_T> actions(orderToActions(orderID, vOrders,weightOfProduct,droneID));
        droneActions[droneID].insert(droneActions[droneID].end(), actions.begin(), actions.end());
        droneID = (droneID+1)%nbDrone;
    }

return droneActions;
}


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
    cout << nbOrder << endl;
    vector<order_T> vOrders(nbOrder);
    for (int order=0; order<nbOrder; order++)
    {

        myFile >> vOrders[order].row >> vOrders[order].col >>vOrders[order].nbItems;


        for (int itemNb=0; itemNb<vOrders[order].nbItems; itemNb++)
        {
            int productType(0);
            myFile >> productType;
            vOrders[order].nbByType[productType]++;
            vOrders[order].weight += weightOfProduct[productType];
        }

    }


    myFile.close();
    actionToFile(initialSolution(weightOfProduct,vOrders,vWarehouse));
//initialSolution(weightOfProduct,vOrders,vWarehouse);
    return 0;
}
