#include <iostream>
#include <deque>
#include <list>
#include <algorithm>
#include "monstermonk.h"

void generateCrossRiverAction(int maxBoatCount, int maxMonkCount, int maxMonsterCount, std::list<CrossRiverAction>& actionList)
{    
    for(int monkCount = 0; monkCount <= std::min(maxMonkCount, maxBoatCount); ++monkCount)
    {
        for(int monsterCount = 0; monsterCount <= std::min(maxMonsterCount, maxBoatCount); ++monsterCount)
        {
            if(((monkCount == 0) || (monkCount >= monsterCount))
            && ((monkCount + monsterCount) <= maxBoatCount)
            && ((monsterCount + monkCount) > 0))
            {
                actionList.push_back(CrossRiverAction(monkCount, monsterCount, BOAT_LOCAL));
                actionList.push_back(CrossRiverAction(monkCount, monsterCount, BOAT_REMOTE));
            }
        }          
    } 

    for(std::list<CrossRiverAction>::const_iterator iter = actionList.begin(); iter != actionList.end(); ++iter)
    {
       iter->printSelf();
    }    
}

bool isExistState(const std::deque<SearchTreeNode>& searchTree, const MonsterMonkBoatState& curState)
{
    bool isExist = false;
    for (std::deque<SearchTreeNode>::const_iterator iter = searchTree.begin(); iter != searchTree.end(); ++iter)
    {
        if(curState == iter->m_curState)
        {
            isExist = true;
        }
    }
    return isExist;
}

void printSearchTree(const std::deque<SearchTreeNode>& searchTree)
{
    static int count = 0;
    std::cout << "//~~~~~~~~~~~~~~~~~~~"<< ++count <<"~~~~~~~~~~~~~~~~~~~ "<< std::endl;
    for (std::deque<SearchTreeNode>::const_iterator iter = searchTree.begin(); iter != searchTree.end(); ++iter)
    {
        iter->m_fromAction.printSelf();
        iter->m_curState.printSelf();
    }
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//" << std::endl;
}

void searchFinalState(std::deque<SearchTreeNode>& searchTree, const MonsterMonkBoatState& finalState, const std::list<CrossRiverAction>& actionList)
{
    const SearchTreeNode& curNode = searchTree.back();
    
    if (curNode.m_curState == finalState)
    {
        printSearchTree(searchTree);
        return;
    }
    
    if (!(curNode.m_curState.isValidState()))
    {
        std::cout << "Invalid State." << std::endl;
        return;
    }
    
    for(std::list<CrossRiverAction>::const_iterator iter = actionList.begin(); iter != actionList.end(); ++iter)
    {
        MonsterMonkBoatState nextState(curNode.m_curState);
        if(RET_SUCCESS == curNode.m_curState.getNextState(*iter, nextState))
        {
            if(!isExistState(searchTree, nextState))
            {
                searchTree.push_back(SearchTreeNode(nextState, *iter));
                searchFinalState(searchTree, finalState, actionList);
                searchTree.pop_back();    
            }            
        }
    }
}

int CrossRiverAction::MAX_BOAT_COUNT = 0;

int main()
{
    int monkCount = 0;
    int monsterCount = 0;
    int boatCount = 0;
    
    while(true)
    {
    
        std::cout << "Please input count held by boat:" << std::endl;
        std::cin >> boatCount;
        
        if (boatCount <= 0)
        {
            std::cout << "Game is over." << std::endl;
            break;
        }
        
        CrossRiverAction::MAX_BOAT_COUNT = boatCount;
        
        std::cout << "Please input count of monk:" << std::endl;
        std::cin >> monkCount;
        
        std::cout << "Please input count of monster:" << std::endl;
        std::cin >> monsterCount;
        
        if((monkCount <= 0) && (monsterCount <= 0))
        {
            std::cout << "Bad input." << std::endl;
            continue;
        }
        
        std::list<CrossRiverAction> actionList;
        generateCrossRiverAction(boatCount, monkCount, monsterCount, actionList);
        
        MonsterMonkBoatState initState(monkCount, monsterCount, 0, 0, BOAT_LOCAL);
        CrossRiverAction firstAction(0, 0, BOAT_MAX_LOCATION);
        MonsterMonkBoatState finalState(0, 0, monkCount, monsterCount, BOAT_REMOTE);
        
        std::deque<SearchTreeNode> searchTree;
        searchTree.push_back(SearchTreeNode(initState, firstAction));
        
        searchFinalState(searchTree, finalState, actionList);
    }
    
    
}