#include <vector>
#include "node.h"
#include <iostream>
#include <string>

using namespace std;

void routingAlgo(vector<RoutingNode*> nd)
{
  int count=nd.size();
  while(count--)
  {
    for(int i=0;i<nd.size();i++)
      nd[i]->sendMsg();
  }
  for(int i = 0; i<nd.size(); i++){
    nd[i]->printTable();
  }
  for(int ii = 0; ii<nd.size(); ii++){
    if(nd[ii]->getName()=="B"){
      nd[ii]->updateTblEntry("10.0.1.3");
          // nd[ii]->printTable();
    }
    if(nd[ii]->getName()=="C"){
      nd[ii]->updateTblEntry("10.0.1.23");
      nd[ii]->updateTblEntry("10.0.0.21");
      // nd[ii]->printTable();
    }
  }

  cout << "\nCost changed\n";
  for(int i = 0; i<nd.size(); i++){
    nd[i]->printTable();
  }

  cout << "\nDisplaying the iterations\n";

  bool toBreak = false;
  while(1)
  {
    for(int i=0;i<nd.size();i++){
      // nd[i]->printTable();
      nd[i]->sendMsg();

      if(nd[i]->getName()=="B"){
        for(int j = 0; j<nd[i]->getTable().tbl.size(); j++){
          if(nd[i]->getTable().tbl[j].dstip=="10.0.1.3" && nd[i]->getTable().tbl[j].cost==16){
            toBreak = true;
            break;
          }

        }
      }
      if(nd[i]->getName()=="A"){
        for(int j = 0; j<nd[i]->getTable().tbl.size(); j++){
          if(nd[i]->getTable().tbl[j].dstip=="10.0.1.3" && nd[i]->getTable().tbl[j].cost==16){

            toBreak = true;
            break;
          }
        }
      }

      if(nd[i]->getName()=="A"){
        for(int i = 0; i<nd.size(); i++){
          nd[i]->printTable();
        }
        cout<<"\n";
      }

      if(nd[i]->getName()=="B"){
        for(int i = 0; i<nd.size(); i++){
          nd[i]->printTable();
        }
        cout<<"\n";
      }
    }
    if (toBreak)
      break;
    cout<<"\n";
  }
// (c) the problem here is the count to infinity problem.ip_interface.
// Since the link from B to C has broken whenever A sends msg to B.
// B perceives that A can take it to C in cost 2.
// However, it is not known that A goes to C via B only.
// Due to this the cost of A to C and B to C increases till it reaches hop count 16.

  cout << "\nThe problem here is the count to infinity problem.\n";
  cout << "Since the link from B to C has broken whenever A sends msg to B.\n";
  cout << "B perceives that A can take it to C in cost 2.\n";
  cout << "However, it is not known that A goes to C via B only.\n";
  cout << "Due to this the cost of A to C and B to C increases till it reaches hop count 16.\n";

}


void RoutingNode::recvMsg(RouteMsg *msg) 
{

  int s = msg->mytbl->tbl.size();
  int alphaa = 0;

  for(int i=0;i<s;i++)
  {
    bool hello = false;
    int alpha;
    for(int j=0;j<mytbl.tbl.size();j++)
    {
      if(msg->mytbl->tbl[i].dstip==mytbl.tbl[j].dstip)
      {
        hello = true;
        alpha=j;
        break;
      }
    }
    if(hello){
      if(mytbl.tbl[alpha].cost > msg->mytbl->tbl[i].cost+1 || mytbl.tbl[alpha].nexthop==msg->from){
        mytbl.tbl[alpha].cost=msg->mytbl->tbl[i].cost+1;
        mytbl.tbl[alpha].nexthop = msg->from;
      }
    }
    else
    {
      RoutingEntry new_route;
      new_route.ip_interface= msg->recvip;
      new_route.nexthop=msg->from;
      new_route.dstip=msg->mytbl->tbl[i].dstip;
      if(msg->from!=new_route.dstip)
      {
        new_route.cost=1+msg->mytbl->tbl[i].cost;
      }
      else{
        new_route.cost=1;
      }
      mytbl.tbl.push_back(new_route);
    } 
  }
}

// (c) the problem here is the count to infinity problem.ip_interface.
// Since the link from B to C has broken whenever A sends msg to B.
// B perceives that A can take it to C in cost 2.
// However, it is not known that A goes to C via B only.
// Due to this the cost of A to C and B to C increases till it reaches hop count 16.