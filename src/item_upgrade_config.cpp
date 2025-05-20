/*
 * Credits: silviu20092
 */

#include "ItemTemplate.h"
#include "item_upgrade_config.h"
#include "Config.h"
#include "Player.h"

ItemUpgradeConfig::ItemUpgradeConfig()
{
    memset(boolConfigs, 0, sizeof(boolConfigs));
    memset(floatConfigs, 0, sizeof(floatConfigs));
    memset(intConfigs, 0, sizeof(intConfigs));
}

void ItemUpgradeConfig::Initialize()
{
    boolConfigs[CONFIG_ITEM_UPGRADE_ENABLED] = sConfigMgr->GetOption<bool>("ItemUpgrade.Enable", true);
    boolConfigs[CONFIG_ITEM_UPGRADE_SEND_PACKETS] = sConfigMgr->GetOption<bool>("ItemUpgrade.SendUpgradedItemsPackets", false);
    boolConfigs[CONFIG_ITEM_UPGRADE_ALLOW_PURGE] = sConfigMgr->GetOption<bool>("ItemUpgrade.AllowUpgradesPurge", false);
    boolConfigs[CONFIG_ITEM_UPGRADE_REFUND_ALL_ON_PURGE] = sConfigMgr->GetOption<bool>("ItemUpgrade.RefundAllOnPurge", true);
    boolConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES] = sConfigMgr->GetOption<bool>("ItemUpgrade.RandomUpgradesOnLoot", false);
    boolConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_BUY] = sConfigMgr->GetOption<bool>("ItemUpgrade.RandomUpgradeWhenBuying", false);
    boolConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_LOOT] = sConfigMgr->GetOption<bool>("ItemUpgrade.RandomUpgradeWhenLooting", true);
    boolConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_WIN] = sConfigMgr->GetOption<bool>("ItemUpgrade.RandomUpgradeWhenWinning", true);
    boolConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_QUEST_REWARD] = sConfigMgr->GetOption<bool>("ItemUpgrade.RandomUpgradeOnQuestReward", true);
    boolConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_CRAFTING] = sConfigMgr->GetOption<bool>("ItemUpgrade.RandomUpgradeWhenCrafting", true);
    boolConfigs[CONFIG_ITEM_UPGRADE_WEAPON_DAMAGE] = sConfigMgr->GetOption<bool>("ItemUpgrade.UpgradeWeaponDamage", true);
    boolConfigs[CONFIG_ITEM_UPGRADE_WEAPON_SPEED] = sConfigMgr->GetOption<bool>("ItemUpgrade.UpgradeWeaponSpeed", true);

    stringConfigs[CONFIG_ITEM_UPGRADE_ALLOWED_STATS] = sConfigMgr->GetOption<std::string>("ItemUpgrade.AllowedStats", "0,3,4,5,6,7,32,36,45");
    stringConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_LOGIN_MSG] = sConfigMgr->GetOption<std::string>("ItemUpgrade.RandomUpgradesBroadcastLoginMsg", "");
    stringConfigs[CONFIG_ITEM_UPGRADE_WEAPON_DAMAGE_PERCENTS] = sConfigMgr->GetOption<std::string>("ItemUpgrade.UpgradeWeaponDamagePercents", "5,10,15");
    stringConfigs[CONFIG_ITEM_UPGRADE_WEAPON_SPEED_PERCENTS] = sConfigMgr->GetOption<std::string>("ItemUpgrade.UpgradeWeaponSpeedPercents", "10,20,30");

    floatConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_CHANCE] = sConfigMgr->GetOption<float>("ItemUpgrade.RandomUpgradeChance", 2.0f);
    if (floatConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_CHANCE] <= 0.0f)
        floatConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_CHANCE] = 2.0f;
    else if (floatConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_CHANCE] > 100.0f)
        floatConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_CHANCE] = 100.0f;

    intConfigs[CONFIG_ITEM_UPGRADE_SEND_PACKETS_PRIORITY] = sConfigMgr->GetOption<int32>("ItemUpgrade.SendUpgradedItemsPacketsPrioritization", 0);
    intConfigs[CONFIG_ITEM_UPGRADE_PURGE_TOKEN] = sConfigMgr->GetOption<int32>("ItemUpgrade.UpgradePurgeToken", 0);
    if (intConfigs[CONFIG_ITEM_UPGRADE_PURGE_TOKEN] < 0)
        intConfigs[CONFIG_ITEM_UPGRADE_PURGE_TOKEN] = 0;
    intConfigs[CONFIG_ITEM_UPGRADE_PURGE_TOKEN_COUNT] = sConfigMgr->GetOption<int32>("ItemUpgrade.UpgradePurgeTokenCount", 1);
    if (intConfigs[CONFIG_ITEM_UPGRADE_PURGE_TOKEN_COUNT] < 1)
        intConfigs[CONFIG_ITEM_UPGRADE_PURGE_TOKEN_COUNT] = 1;
    intConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_MAX_STATS] = sConfigMgr->GetOption<int32>("ItemUpgrade.RandomUpgradeMaxStatCount", 2);
    if (intConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_MAX_STATS] <= 0)
        intConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_MAX_STATS] = 2;
    else if (intConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_MAX_STATS] > MAX_ITEM_PROTO_STATS)
        intConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_MAX_STATS] = MAX_ITEM_PROTO_STATS;
    intConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_MAX_RANK] = sConfigMgr->GetOption<int32>("ItemUpgrade.RandomUpgradeMaxRank", 3);
    if (intConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_MAX_RANK] <= 0)
        intConfigs[CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_MAX_RANK] = 3;
    intConfigs[CONFIG_ITEM_UPGRADE_WEAPON_DAMAGE_TOKEN] = sConfigMgr->GetOption<int32>("ItemUpgrade.UpgradeWeaponDamageToken", 0);
    if (intConfigs[CONFIG_ITEM_UPGRADE_WEAPON_DAMAGE_TOKEN] < 0)
        intConfigs[CONFIG_ITEM_UPGRADE_WEAPON_DAMAGE_TOKEN] = 0;
    intConfigs[CONFIG_ITEM_UPGRADE_WEAPON_DAMAGE_TOKEN_COUNT] = sConfigMgr->GetOption<int32>("ItemUpgrade.UpgradeWeaponDamageTokenCount", 1);
    if (intConfigs[CONFIG_ITEM_UPGRADE_WEAPON_DAMAGE_TOKEN_COUNT] < 1)
        intConfigs[CONFIG_ITEM_UPGRADE_WEAPON_DAMAGE_TOKEN_COUNT] = 1;
    intConfigs[CONFIG_ITEM_UPGRADE_WEAPON_DAMAGE_MONEY] = sConfigMgr->GetOption<int32>("ItemUpgrade.UpgradeWeaponDamageMoney", 0);
    if (intConfigs[CONFIG_ITEM_UPGRADE_WEAPON_DAMAGE_MONEY] < 0 || intConfigs[CONFIG_ITEM_UPGRADE_WEAPON_DAMAGE_MONEY] > MAX_MONEY_AMOUNT)
        intConfigs[CONFIG_ITEM_UPGRADE_WEAPON_DAMAGE_MONEY] = 0;
    intConfigs[CONFIG_ITEM_UPGRADE_WEAPON_SPEED_TOKEN] = sConfigMgr->GetOption<int32>("ItemUpgrade.UpgradeWeaponSpeedToken", 0);
    if (intConfigs[CONFIG_ITEM_UPGRADE_WEAPON_SPEED_TOKEN] < 0)
        intConfigs[CONFIG_ITEM_UPGRADE_WEAPON_SPEED_TOKEN] = 0;
    intConfigs[CONFIG_ITEM_UPGRADE_WEAPON_SPEED_TOKEN_COUNT] = sConfigMgr->GetOption<int32>("ItemUpgrade.UpgradeWeaponSpeedTokenCount", 1);
    if (intConfigs[CONFIG_ITEM_UPGRADE_WEAPON_SPEED_TOKEN_COUNT] < 1)
        intConfigs[CONFIG_ITEM_UPGRADE_WEAPON_SPEED_TOKEN_COUNT] = 1;
    intConfigs[CONFIG_ITEM_UPGRADE_WEAPON_SPEED_MONEY] = sConfigMgr->GetOption<int32>("ItemUpgrade.UpgradeWeaponSpeedMoney", 0);
    if (intConfigs[CONFIG_ITEM_UPGRADE_WEAPON_SPEED_MONEY] < 0 || intConfigs[CONFIG_ITEM_UPGRADE_WEAPON_SPEED_MONEY] > MAX_MONEY_AMOUNT)
        intConfigs[CONFIG_ITEM_UPGRADE_WEAPON_SPEED_MONEY] = 0;
}

bool ItemUpgradeConfig::GetBoolConfig(ItemUpgradeBoolConfigs index) const
{
    return boolConfigs[index];
}

std::string ItemUpgradeConfig::GetStringConfig(ItemUpgradeStringConfigs index) const
{
    return stringConfigs[index];
}

float ItemUpgradeConfig::GetFloatConfig(ItemUpgradeFloatConfigs index) const
{
    return floatConfigs[index];
}

int32 ItemUpgradeConfig::GetIntConfig(ItemUpgradeIntConfigs index) const
{
    return intConfigs[index];
}
