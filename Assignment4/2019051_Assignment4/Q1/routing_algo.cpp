#include <vector>
#include "node.h"
#include <iostream>
#include <string>

using namespace std;

void routingAlgo(vector<RoutingNode*> nd)
{
  for(int j = 0; j<nd.size(); j++)
  {
    for(int i=0;i<nd.size();i++)
      nd[i]->sendMsg();
  }
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
      if(mytbl.tbl[alpha].cost > msg->mytbl->tbl[i].cost+1)
        mytbl.tbl[alpha].cost=msg->mytbl->tbl[i].cost+1;
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