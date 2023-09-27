#ifndef _UNIT_HPP_INC_
#define _UNIT_HPP_INC_
#include <string>
#include <cstdlib>

class Unit
{
public:
    enum UnitType
    {
        UNIT_TYPE_INVALID = -1,
        UNIT_TYPE_HERO = 0,
        UNIT_TYPE_MINION,
        UNIT_TYPE_SHAMAN,
        UNIT_TYPE_SPELL
    };
    struct UnitInfoType
    {
        std::string name;
        uint16_t health;
        uint16_t damage;
        UnitType type;
    };
    virtual std::string getName(void) =0;
    virtual uint16_t getHealth(void)  =0;
    virtual uint16_t getDamage(void)  =0;
    virtual UnitType getType(void)     =0;
};

#endif