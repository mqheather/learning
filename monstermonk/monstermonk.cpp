#include <iostream>
#include "monstermonk.h"

static std::string s_boatLocation[BOAT_MAX_LOCATION] = {"Local", "Remote"}; 
//------------------------------------------------------------------------------------------------------------
MonsterMonkBoatState::MonsterMonkBoatState(int localMonkCount, 
			                               int localMonsterCount,
										   int remoteMonkCount,
										   int remoteMonsterCount,
										   BoatLocation boatLoc)
:m_localMonkCount(localMonkCount),
 m_localMonsterCount(localMonsterCount),
 m_remoteMonkCount(remoteMonkCount),
 m_remoteMonsterCount(remoteMonsterCount),
 m_boatLoc(boatLoc)
{
}

MonsterMonkBoatState::MonsterMonkBoatState(const MonsterMonkBoatState &copyState)
{
	if (this == &copyState)
	{
		return;
	}

	m_localMonkCount = copyState.m_localMonkCount;
	m_localMonsterCount = copyState.m_localMonsterCount;
	m_remoteMonkCount = copyState.m_remoteMonkCount;
	m_remoteMonsterCount = copyState.m_remoteMonsterCount;
	m_boatLoc = copyState.m_boatLoc;
}

MonsterMonkBoatState& MonsterMonkBoatState::operator=(const MonsterMonkBoatState &rightState)
{
	if (this != &rightState)
	{
		m_localMonkCount = rightState.m_localMonkCount;
        m_localMonsterCount = rightState.m_localMonsterCount;
        m_remoteMonkCount = rightState.m_remoteMonkCount;
        m_remoteMonsterCount = rightState.m_remoteMonsterCount;
        m_boatLoc = rightState.m_boatLoc;
	}

	return *this;
}

bool MonsterMonkBoatState::operator==(const MonsterMonkBoatState& rightState) const
{
	bool isEqual = false;

	isEqual = (m_localMonkCount == rightState.m_localMonkCount)
		    &&(m_localMonsterCount == rightState.m_localMonsterCount)
			&&(m_remoteMonkCount == rightState.m_remoteMonkCount)
			&&(m_remoteMonsterCount == rightState.m_remoteMonsterCount)
			&&(m_boatLoc == rightState.m_boatLoc);

	return isEqual;
}

bool MonsterMonkBoatState::isValidState() const
{
	bool isValid = false;
    
    if ( (m_localMonkCount < 0)
      || (m_localMonsterCount < 0)
      || (m_remoteMonkCount < 0)
      || (m_remoteMonsterCount < 0))
    {
       return isValid;
    }
    
    isValid = ((0 == m_localMonkCount) || (m_localMonkCount >= m_localMonsterCount))
            &&((0 == m_remoteMonkCount) || (m_remoteMonkCount >= m_remoteMonsterCount))
            &&(m_boatLoc < BOAT_MAX_LOCATION);
    
	return isValid;
}

int MonsterMonkBoatState::getNextState(const CrossRiverAction& action,
			                           MonsterMonkBoatState& nextState) const
{
	int ret = RET_FAILURE;
	int localMonk = 0;
	int localMonster = 0;
	int remoteMonk = 0; 
	int remoteMonster = 0;

	if (!(action.isValidAction()))
	{
		return ret;
	}

    if (m_boatLoc == action.m_to)
	{
		return ret;
	}
    
	if (BOAT_REMOTE == action.m_to)
	{
		localMonk = m_localMonkCount - action.m_monkCount;
        localMonster = m_localMonsterCount - action.m_monsterCount;
        remoteMonk = m_remoteMonkCount + action.m_monkCount;
        remoteMonster = m_remoteMonsterCount + action.m_monsterCount;
	}
    else
    {
        localMonk = m_localMonkCount + action.m_monkCount;
        localMonster = m_localMonsterCount + action.m_monsterCount;
        remoteMonk = m_remoteMonkCount - action.m_monkCount;
        remoteMonster = m_remoteMonsterCount - action.m_monsterCount;
    }
    
    MonsterMonkBoatState checkState(localMonk, localMonster, remoteMonk, remoteMonster, action.m_to);
    
    if (checkState.isValidState())
    {
        ret = RET_SUCCESS;
        nextState = checkState;
    }
    
    return ret;
}
void MonsterMonkBoatState::printSelf() const
{
    std::cout << "==============================" << std::endl;
    if(isValidState())
    {
        //std::cout << "\tLocal Monk:" << m_localMonkCount << std::endl;
        //std::cout << "\tLocal Monster:" << m_localMonsterCount << std::endl;
        //std::cout << "\tRemote Monk:" << m_remoteMonkCount << std::endl;
        //std::cout << "\tRemote Monster:" << m_remoteMonsterCount << std::endl;
        //std::cout << "\tBoat Location:" << s_boatLocation[m_boatLoc] << std::endl;
        std::cout << "(" << m_localMonkCount << ", " <<  m_localMonsterCount << ", ";
		std::cout << m_remoteMonkCount << ", " << m_remoteMonsterCount << ", ";
		std::cout << s_boatLocation[m_boatLoc] << ")" << std::endl;
    }
    std::cout << "==============================" << std::endl;
}

//----------------------------------------------------------------------------------------------------------------------
CrossRiverAction::CrossRiverAction(int monkCount, int monsterCount, BoatLocation to)
:m_monkCount(monkCount),
 m_monsterCount(monsterCount),
 m_to(to)
{
}

CrossRiverAction::CrossRiverAction(const CrossRiverAction& copyAction)
{
    if (this == &copyAction)
	{
		return;
	}
    
    m_monkCount = copyAction.m_monkCount;
    m_monsterCount = copyAction.m_monsterCount;
    m_to = copyAction.m_to;
}

bool CrossRiverAction::isValidAction() const
{
    return ((m_monkCount == 0) || (m_monkCount >= m_monsterCount)) 
        && ((m_monkCount + m_monsterCount) <= CrossRiverAction::MAX_BOAT_COUNT) 
        && ((m_monkCount + m_monsterCount) > 0);
}

void CrossRiverAction::printSelf() const
{
   
    if(isValidAction())
    {

        std::cout << "\t||" << std::endl;
        std::cout << "(" << m_monkCount << ", ";
        std::cout << m_monsterCount << ", ";
        std::cout <<  s_boatLocation[m_to] << ")" << std::endl;
        std::cout << "\t||" << std::endl;
        std::cout << "\t\\/" << std::endl;
    }
    
}

//-----------------------------------------------------------------------------------------------------------------------

SearchTreeNode::SearchTreeNode(const MonsterMonkBoatState& state, const CrossRiverAction& fromAction)
:m_curState(state),
 m_fromAction(fromAction)
{
}
