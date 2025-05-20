/*
 * Credits: silviu20092
 */

#include "ScriptMgr.h"
#include "DatabaseEnv.h"
#include "Player.h"
#include "item_upgrade.h"

class item_upgrade_playerscript : public PlayerScript
{
private:
    class SendUpgradePackets : public BasicEvent
    {
    public:
        SendUpgradePackets(Player* player) : player(player)
        {
            player->m_Events.AddEvent(this, player->m_Events.CalculateTime(DELAY_MS));
        }

        bool Execute(uint64 /*e_time*/, uint32 /*p_time*/)
        {
            sItemUpgrade->RefreshWeaponSpeed(player);
            sItemUpgrade->UpdateVisualCache(player);
            return true;
        }
    private:
        static constexpr uint64 DELAY_MS = 3000;

        Player* player;
    };
public:
    item_upgrade_playerscript() : PlayerScript("item_upgrade_playerscript",
        {
            PLAYERHOOK_ON_APPLY_ITEM_MODS_BEFORE,
            PLAYERHOOK_ON_APPLY_ENCHANTMENT_ITEM_MODS_BEFORE,
            PLAYERHOOK_ON_AFTER_MOVE_ITEM_FROM_INVENTORY,
            PLAYERHOOK_ON_DELETE_FROM_DB,
            PLAYERHOOK_ON_LOGIN,
            PLAYERHOOK_ON_LOOT_ITEM,
            PLAYERHOOK_ON_GROUP_ROLL_REWARD_ITEM,
            PLAYERHOOK_ON_QUEST_REWARD_ITEM,
            PLAYERHOOK_ON_CREATE_ITEM,
            PLAYERHOOK_ON_AFTER_STORE_OR_EQUIP_NEW_ITEM,
            PLAYERHOOK_ON_APPLY_WEAPON_DAMAGE,
            PLAYERHOOK_ON_EQUIP
        }) {}

    void OnPlayerApplyItemModsBefore(Player* player, uint8 slot, bool /*apply*/, uint8 /*itemProtoStatNumber*/, uint32 statType, int32& val) override
    {
        val = sItemUpgrade->HandleStatModifier(player, slot, statType, val);
    }

    void OnPlayerApplyEnchantmentItemModsBefore(Player* player, Item* item, EnchantmentSlot slot, bool /*apply*/, uint32 enchant_spell_id, uint32& enchant_amount) override
    {
        enchant_amount = sItemUpgrade->HandleStatModifier(player, item, enchant_spell_id, enchant_amount, slot);
    }

    void OnPlayerAfterMoveItemFromInventory(Player* player, Item* it, uint8 /*bag*/, uint8 /*slot*/, bool /*update*/) override
    {
        sItemUpgrade->HandleItemRemove(player, it);
    }

    void OnPlayerDeleteFromDB(CharacterDatabaseTransaction trans, uint32 guid) override
    {
        trans->Append("DELETE FROM character_item_upgrade WHERE guid = {}", guid);
        trans->Append("DELETE FROM character_weapon_upgrade WHERE guid = {}", guid);
        trans->Append("DELETE FROM character_weapon_speed_upgrade WHERE guid = {}", guid);
        sItemUpgrade->HandleCharacterRemove(guid);
    }

    void OnPlayerLogin(Player* player) override
    {
        new SendUpgradePackets(player);

        if (sItemUpgrade->GetBoolConfig(CONFIG_ITEM_UPGRADE_ENABLED) && sItemUpgrade->GetBoolConfig(CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES))
        {
            const std::string& loginMsg = sItemUpgrade->GetStringConfig(CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_LOGIN_MSG);
            if (!loginMsg.empty())
                ItemUpgrade::SendMessage(player, loginMsg);
        }
    }

    void OnPlayerLootItem(Player* player, Item* item, uint32 /*count*/, ObjectGuid /*lootguid*/) override
    {
        if (sItemUpgrade->GetBoolConfig(CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_LOOT))
            sItemUpgrade->ChooseRandomUpgrade(player, item);
    }

    void OnPlayerGroupRollRewardItem(Player* player, Item* item, uint32 /*count*/, RollVote /*voteType*/, Roll* /*roll*/) override
    {
        if (sItemUpgrade->GetBoolConfig(CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_WIN))
            sItemUpgrade->ChooseRandomUpgrade(player, item);
    }

    void OnPlayerQuestRewardItem(Player* player, Item* item, uint32 /*count*/) override
    {
        if (sItemUpgrade->GetBoolConfig(CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_QUEST_REWARD))
            sItemUpgrade->ChooseRandomUpgrade(player, item);
    }

    void OnPlayerCreateItem(Player* player, Item* item, uint32 /*count*/) override
    {
        if (sItemUpgrade->GetBoolConfig(CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_CRAFTING))
            sItemUpgrade->ChooseRandomUpgrade(player, item);
    }

    void OnPlayerAfterStoreOrEquipNewItem(Player* player, uint32 /*vendorslot*/, Item* item, uint8 /*count*/, uint8 /*bag*/, uint8 /*slot*/, ItemTemplate const* /*pProto*/, Creature* /*pVendor*/, VendorItem const* /*crItem*/, bool /*bStore*/) override
    {
        if (sItemUpgrade->GetBoolConfig(CONFIG_ITEM_UPGRADE_RANDOM_UPGRADES_BUY))
            sItemUpgrade->ChooseRandomUpgrade(player, item);
    }

    void OnPlayerApplyWeaponDamage(Player* player, uint8 slot, ItemTemplate const* /*proto*/, float& minDamage, float& maxDamage, uint8 damageIndex) override
    {
        if (damageIndex == 0)
        {
            std::pair<float, float> upgradedDmgInfo = sItemUpgrade->HandleWeaponModifier(player, slot, minDamage, maxDamage);
            minDamage = upgradedDmgInfo.first;
            maxDamage = upgradedDmgInfo.second;
        }
    }

    void OnPlayerEquip(Player* player, Item* it, uint8 bag, uint8 slot, bool update) override
    {
        uint8 attType = Player::GetAttackBySlot(slot);
        if (attType != MAX_ATTACK)
        {
            const ItemTemplate* proto = it->GetTemplate();
            if (proto->Delay && !player->IsInFeralForm())
            {
                uint32 delay = sItemUpgrade->HandleWeaponSpeedModifier(player, it);
                if (slot == EQUIPMENT_SLOT_RANGED)
                    player->SetAttackTime(RANGED_ATTACK, delay);
                else if (slot == EQUIPMENT_SLOT_MAINHAND)
                    player->SetAttackTime(BASE_ATTACK, delay);
                else if (slot == EQUIPMENT_SLOT_OFFHAND)
                    player->SetAttackTime(OFF_ATTACK, delay);

                if (player->CanModifyStats() && player->GetWeaponDamageRange(WeaponAttackType(attType), MAXDAMAGE))
                    player->UpdateDamagePhysical(WeaponAttackType(attType));
            }
        }
    }
};

void AddSC_item_upgrade_playerscript()
{
    new item_upgrade_playerscript();
}
