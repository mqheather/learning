#ifndef MONSTERMONK_H
#define MONSTERMONK_H

enum ReturnValue
{
	RET_SUCCESS = 0,
	RET_FAILURE
};
enum BoatLocation
{
	BOAT_LOCAL = 0,
	BOAT_REMOTE,
    BOAT_MAX_LOCATION
};

struct CrossRiverAction
{
	public:
    CrossRiverAction(int monkCount, int monsterCount, BoatLocation to);
    CrossRiverAction(const CrossRiverAction& copyAction);
    bool isValidAction() const;
    void printSelf() const;
	
    
    int m_monkCount;
    int m_monsterCount;
    BoatLocation m_to;
    
    static int MAX_BOAT_COUNT;
};

class MonsterMonkBoatState
{
	public:
		MonsterMonkBoatState(int localMonkCount, int localMonsterCount, int remoteMonkCount, int remoteMonsterCount, BoatLocation boatLoc);
        MonsterMonkBoatState(const MonsterMonkBoatState& copyState);
        
        MonsterMonkBoatState& operator=(const MonsterMonkBoatState &rightState);
		bool operator==(const MonsterMonkBoatState& rightState) const;
        
        bool isValidState() const;
        int getNextState(const CrossRiverAction& action,
                         MonsterMonkBoatState& nextState) const;
        
        void printSelf() const;
	private:
		int m_localMonkCount;
		int m_localMonsterCount;
		int m_remoteMonkCount;
		int m_remoteMonsterCount;
		BoatLocation m_boatLoc;
};

struct SearchTreeNode
{
    SearchTreeNode(const MonsterMonkBoatState& state, const CrossRiverAction& fromAction);
    MonsterMonkBoatState m_curState;
    CrossRiverAction     m_fromAction;
};
#endif //MONSTERMONK_H
