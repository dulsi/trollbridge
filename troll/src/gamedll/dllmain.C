/*-------------------------------------------------------------------------*\
  <dllmain.C> -- Troll Bridge dynamically linked library

  Date      Programmer  Description
  04/17/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"
#include "items.h"
#include "people.h"
#include "door.h"
#include "scorpion.h"
#include "bug.h"
#include "block.h"
#include "undead.h"
#include "closedoor.h"
#include "cobra.h"
#include "healer.h"
#include "jumper.h"
#include "octoid.h"

// This function must be extern "C" so that C++ doesn't rename the function.
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollDllInit - Sends information about monsters and items to the main
    program.

    Parameters:
      game         (In)  Game being played

    Returns: 0 on success, any other value indicates failure
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
extern "C" int TrollDllInit(TrollGame *game)
{
 // add monster types
 game->addMonsterType(0, TrollPerson::createGrayTroll);
 game->addMonsterType(1, TrollDoor::createDoor);
 game->addMonsterType(2, TrollDoor::createSouthDoor);
 game->addMonsterType(3, TrollDoor::createWestDoor);
 game->addMonsterType(4, TrollDoor::createEastDoor);
 game->addMonsterType(5, TrollScorpion::createBlueScorpion);
 game->addMonsterType(6, TrollScorpion::createRedScorpion);
 game->addMonsterType(7, TrollBug::createBlueBug);
 game->addMonsterType(8, TrollBug::createRedBug);
 game->addMonsterType(9, TrollBlock::createBlock);
 game->addMonsterType(10, TrollSkeletalTroll::createSkeletalTroll);
 game->addMonsterType(11,
   TrollSkeletalTrollWithItem::createSkeletalTrollWithKey);
 game->addMonsterType(12, TrollClosingDoor::createNorthDoor);
 game->addMonsterType(13, TrollClosingDoor::createSouthDoor);
 game->addMonsterType(14, TrollClosingDoor::createWestDoor);
 game->addMonsterType(15, TrollClosingDoor::createEastDoor);
 game->addMonsterType(16, TrollMerchant::createMerchant);
 game->addMonsterType(17, TrollCobra::createGreenCobra);
 game->addMonsterType(18, TrollHealer::createHealer);
 game->addMonsterType(19, TrollJumper::createGreenJumper);
 game->addMonsterType(20, TrollJumper::createRedJumper);
 game->addMonsterType(21, TrollOctoid::createBlueOctoid);
 // add items
 game->addItemType(0, TrollActivatibleItem::createSword);
 game->addItemType(1, TrollLimitedActivatibleItem::createKey);
 game->addItemType(2, TrollGold::createGoldCoin);
 game->addItemType(3, TrollGold::create3GoldCoins);
 game->addItemType(4, TrollHitPoint::createHitPoint);
 game->addItemType(5, TrollActivatibleItem::createEnchantedSword);
 game->addItemType(6, TrollLimitedActivatibleItem::createSoldKey);
 game->addItemType(7, TrollHitPoint::createSoldHitPoint);
 game->addItemType(8, TrollLevelMap::createLevelMap);
 game->addItemType(9, TrollActivatibleItem::createAnhk);
 game->addItemType(10, TrollActivatibleItem::createShield);
 game->addItemType(11, TrollActivatibleItem::createSoldShield);
 game->addItemType(12, TrollActivatibleItem::createEgg);
 // add carried items
 game->addCarriedItem(&TrollNormalSword);
 game->addCarriedItem(&TrollKey);
 game->addCarriedItem(&TrollEnchantedSword);
 game->addCarriedItem(&TrollAnhk);
 game->addCarriedItem(&TrollShield);
 game->addCarriedItem(&TrollEgg);
 return 0;
}

