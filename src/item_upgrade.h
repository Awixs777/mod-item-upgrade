/*
 * Credits: silviu20092
 */

#ifndef _ITEM_UPGRADE_H_
#define _ITEM_UPGRADE_H_

#include <vector>
#include "GossipDef.h"
#include "Player.h"
#include "item_upgrade_config.h"

class ItemUpgrade
{
private:
    ItemUpgrade();
    ~ItemUpgrade();
public:
    enum PagedDataType
    {
        PAGED_DATA_TYPE_ITEMS,
        PAGED_DATA_TYPE_STATS,
        PAGED_DATA_TYPE_REQS,
        PAGED_DATA_TYPE_UPGRADED_ITEMS,
        PAGED_DATA_TYPE_UPGRADED_ITEMS_STATS,
        PAGED_DATA_TYPE_ITEMS_FOR_PURGE,
        PAGED_DATA_TYPE_ITEMS_BULK,
        PAGED_DATA_TYPE_STATS_BULK,
        PAGED_DATA_TYPE_STAT_UPGRADE_BULK,
        PAGED_DATA_TYPE_REQS_BULK,
        PAGED_DATA_TYPE_WEAPON_UPGRADE_ITEMS,
        PAGED_DATA_TYPE_WEAPON_UPGRADE_PERCS,
        PAGED_DATA_TYPE_WEAPON_UPGRADE_PERC_INFO,
        PAGED_DATA_TYPE_WEAPON_UPGRADE_ITEMS_CHECK,
        PAGED_DATA_TYPE_WEAPON_UPGRADE_ITEMS_CHECK_INFO,
        PAGED_DATA_TYPE_WEAPON_SPEED_ITEMS,
        PAGED_DATA_TYPE_WEAPON_SPEED_UPGRADE_PERCS,
        PAGED_DATA_TYPE_WEAPON_SPEED_UPGRADE_PERC_INFO,
        PAGED_DATA_TYPE_WEAPON_SPEED_UPGRADE_ITEMS_CHECK,
        PAGED_DATA_TYPE_WEAPON_SPEED_UPGRADE_ITEMS_CHECK_INFO,
        MAX_PAGED_DATA_TYPE
    };

    enum IdentifierType
    {
        BASE_IDENTIFIER,
        ITEM_IDENTIFIER,
        FLOAT_IDENTIFIER
    };

    enum ItemVisualsPriority
    {
        PRIORITIZE_STATS,
        PRIORITIZE_WEAPON_DAMAGE
    };

    struct Identifier
    {
        uint32 id;
        std::string name;
        std::string uiName;
        GossipOptionIcon optionIcon;

        Identifier() : id(0), optionIcon(GOSSIP_ICON_INTERACT_1) {}

        virtual IdentifierType GetType() const
        {
            return BASE_IDENTIFIER;
        }
    };

    struct ItemIdentifier : public Identifier
    {
        ObjectGuid guid;

        IdentifierType GetType() const override
        {
            return ITEM_IDENTIFIER;
        }
    };

    struct FloatIdentifier : public Identifier
    {
        float modPct;

        IdentifierType GetType() const override
        {
            return FLOAT_IDENTIFIER;
        }
    };

    struct UpgradeStat;
    struct PagedData
    {
        static constexpr int PAGE_SIZE = 12;

        uint32 totalPages;
        uint32 currentPage;
        bool reloaded;
        PagedDataType type;
        const UpgradeStat* upgradeStat;
        ItemIdentifier item;
        std::vector<Identifier *> data;
        float pct;

        PagedData() : totalPages(0), currentPage(0), reloaded(false), type(MAX_PAGED_DATA_TYPE), upgradeStat(nullptr), pct(0.0f) {}

        void Reset();
        void CalculateTotals();
        void SortAndCalculateTotals();
        bool IsEmpty() const;
        const Identifier* FindIdentifierById(uint32 id) const;
    };
    typedef std::unordered_map<uint32, PagedData> PagedDataMap;

    enum UpgradeStatReqType
    {
        REQ_TYPE_COPPER = 1,
        REQ_TYPE_HONOR,
        REQ_TYPE_ARENA,
        REQ_TYPE_ITEM,
        REQ_TYPE_NONE,
        MAX_REQ_TYPE
    };

    struct UpgradeStatReq
    {
        /* Associated stat ID from UpgradeStat */
        uint32 statId;

        /*
            Possible values:
                1 = this rank requires money (copper, gold) to be bought
                2 = this rank requires honor points to be bought
                3 = this rank requires arena points to be bought
                4 = this rank requires certain item(s) to be bought
         */
        UpgradeStatReqType reqType;

        /*
         *   If reqType = 1 THEN required copper to purchase rank
         *   If reqType = 2 THEN required honor points to purchase rank
         *   If reqType = 3 THEN required arena points to purchase rank
         *   If reqType = 4 THEN item ENTRY (from item_template.entry) required to purchase rank
         */
        float reqVal1;

        /*
         *  If reqType = 4 THEN item count required to purchase rank
         *  NOT USED otherwise
         */ 
        float reqVal2;

        UpgradeStatReq()
        {
            statId = 0;
            reqType = MAX_REQ_TYPE;
            reqVal1 = 0.0f;
            reqVal2 = 0.0f;
        }

        UpgradeStatReq(uint32 statId, UpgradeStatReqType reqType, float reqVal1, float reqVal2)
            : statId(statId), reqType(reqType), reqVal1(reqVal1), reqVal2(reqVal2) {}

        UpgradeStatReq(uint32 statId, UpgradeStatReqType reqType, float reqVal1)
            : statId(statId), reqType(reqType), reqVal1(reqVal1), reqVal2(0.0f) {}

        UpgradeStatReq(uint32 statId, UpgradeStatReqType reqType)
            : statId(statId), reqType(reqType), reqVal1(0.0f), reqVal2(0.0f) {}
    };
    typedef std::vector<UpgradeStatReq> StatRequirementContainer;

    struct UpgradeStat
    {
        uint32 statId;
        uint32 statType;
        float statModPct;
        uint16 statRank;
    };
    typedef std::vector<UpgradeStat> UpgradeStatContainer;

    struct CharacterUpgrade
    {
        uint32 guid;
        ObjectGuid itemGuid;
        const UpgradeStat* upgradeStat;
        float upgradeStatModPct;
    };
    typedef std::unordered_map<uint32, std::vector<CharacterUpgrade>> CharacterUpgradeContainer;

    struct ItemUpgradeInfo
    {
        ObjectGuid itemGuid;
        std::vector<const UpgradeStat*> upgrades;
        const UpgradeStat* weaponUpgrade;
    };

    typedef std::set<uint32> ItemEntryContainer;
    typedef std::unordered_map<uint32, std::set<uint32>> StatWithItemContainer;
public:
    static ItemUpgrade* instance();

    template <class Container, typename T>
    static T* FindInContainer(const Container& c, const T& val)
    {
        typename Container::const_iterator citr = std::find_if(c.begin(), c.end(), [&](const T& value) { return value == val; });
        return citr != c.end() ? (T*)(&*citr) : nullptr;
    }

    bool GetBoolConfig(ItemUpgradeBoolConfigs index) const;
    std::string GetStringConfig(ItemUpgradeStringConfigs index) const;
    float GetFloatConfig(ItemUpgradeFloatConfigs index) const;
    int32 GetIntConfig(ItemUpgradeIntConfigs index) const;

    void LoadConfig(bool reload);
    void LoadFromDB(bool reload = false);

    void BuildUpgradableItemCatalogue(const Player* player, PagedDataType type);
    void BuildUpgradableWeaponSpeedItemCatalogue(const Player* player);
    void BuildStatsUpgradeCatalogue(const Player* player, const Item* item);
    void BuildStatsUpgradeCatalogueBulk(const Player* player, const Item* item);
    void BuildStatsUpgradeByPctCatalogueBulk(const Player* player, const Item* item, float pct);
    void BuildStatsRequirementsCatalogueBulk(const Player* player, const Item* item, float pct);
    void BuildStatsRequirementsCatalogue(const Player* player, const UpgradeStat* upgradeStat, const Item* item);
    void BuildAlreadyUpgradedItemsCatalogue(const Player* player, PagedDataType type);
    void BuildItemUpgradeStatsCatalogue(const Player* player, const Item* item);
    void BuildWeaponPercentUpgradesCatalogue(const Player* player, const Item* item);
    void BuildWeaponSpeedPercentUpgradesCatalogue(const Player* player, const Item* item);
    void BuildWeaponUpgradesPercentInfoCatalogue(const Player* player, const Item* item, float pct);
    void BuildWeaponSpeedUpgradesPercentInfoCatalogue(const Player* player, const Item* item, float pct);
    void BuildWeaponUpgradeInfoCatalogue(const Player* player, const Item* item);
    void BuildWeaponSpeedUpgradeInfoCatalogue(const Player* player, const Item* item);

    PagedData& GetPagedData(const Player* player);
    PagedDataMap& GetPagedDataMap();
    bool AddPagedData(Player* player, Creature* creature, uint32 page);
    bool TakePagedDataAction(Player* player, Creature* creature, uint32 action);

    bool IsValidItemForUpgrade(const Item* item, const Player* player) const;
    bool IsValidWeaponForUpgrade(const Item* item, const Player* player) const;
    bool IsValidWeaponForSpeedUpgrade(const Item* item, const Player* player) const;

    int32 HandleStatModifier(const Player* player, uint8 slot, uint32 statType, int32 amount) const;
    int32 HandleStatModifier(const Player* player, Item* item, uint32 statType, int32 amount, EnchantmentSlot slot) const;
    std::pair<float, float> HandleWeaponModifier(const Player* player, uint8 slot, float minDamage, float maxDamage) const;
    std::pair<float, float> HandleWeaponModifier(const Player* player, const Item* item, float minDamage, float maxDamag) const;
    uint32 HandleWeaponSpeedModifier(const Player* player, const Item* item) const;
    void HandleItemRemove(Player* player, Item* item);
    void HandleCharacterRemove(uint32 guid);

    void SetReloading(bool value);
    bool GetReloading() const;

    void HandleDataReload(bool apply);

    void UpdateVisualCache(Player* player);
    void VisualFeedback(Player* player);

    bool ChooseRandomUpgrade(Player* player, Item* item);

    void BuildWeaponUpgradeReqs();
    void BuildWeaponSpeedUpgradeReqs();

    static std::string StatTypeToString(uint32 statType);
    static std::string EquipmentSlotToString(EquipmentSlots slot);
    static std::vector<_ItemStat> LoadItemStatInfo(const Item* item);
    static const _ItemStat* GetStatByType(const std::vector<_ItemStat>& statInfo, uint32 statType);
    static std::pair<float, float> GetItemProtoDamage(const ItemTemplate* proto);
    static std::pair<float, float> GetItemProtoDamage(const Item* item);
    static uint32 GetItemProtoDelay(const ItemTemplate* proto);
    static uint32 GetItemProtoDelay(const Item* item);

    static std::string FormatFloat(float val, uint32 decimals = 2);
    static std::string FormatIncrease(float prev, float next);
    static std::string FormatDelay(uint32 val);

    static int32 CalculateModPct(int32 value, const UpgradeStat* upgradeStat);
    static float CalculateModPctF(float value, const UpgradeStat* upgradeStat);
    static uint32 CalculatePctDecrease(uint32 value, float pct);

    std::vector<const UpgradeStat*> FindUpgradesForItem(const Player* player, const Item* item) const;
    const UpgradeStat* FindUpgradeForWeapon(const CharacterUpgradeContainer& characterUpgradeContainer, const Player* player, const Item* item) const;
    const UpgradeStat* FindUpgradeForWeaponDamage(const Player* player, const Item* item) const;
    const UpgradeStat* FindUpgradeForWeaponSpeed(const Player* player, const Item* item) const;

    bool IsInactiveStatUpgrade(const Item* item, const UpgradeStat* upgradeStat) const;
    bool IsInactiveWeaponUpgrade() const;
    bool IsInactiveWeaponSpeedUpgrade() const;

    void RefreshWeaponSpeed(Player* player);
public:
    static std::string ItemIcon(const ItemTemplate* proto, uint32 width, uint32 height, int x, int y);
    static std::string ItemIcon(const ItemTemplate* proto);
    static std::string ItemNameWithLocale(const Player* player, const ItemTemplate* itemTemplate, int32 randomPropertyId);
    static std::string ItemLink(const Player* player, const ItemTemplate* itemTemplate, int32 randomPropertyId);
    static std::string ItemLink(const Player* player, const Item* item);
    static void SendMessage(const Player* player, const std::string& message);
private:
    static constexpr int VISUAL_FEEDBACK_SPELL_ID = 46331;

    ItemUpgradeConfig cfg;

    bool reloading;
    std::vector<uint32> allowedStats;
    UpgradeStatContainer upgradeStatList;
    PagedDataMap playerPagedData;
    CharacterUpgradeContainer characterUpgradeData;
    ItemEntryContainer allowedItems;
    ItemEntryContainer blacklistedItems;
    StatWithItemContainer allowedStatItems;
    StatWithItemContainer blacklistedStatItems;

    std::map<float, std::vector<const ItemUpgrade::UpgradeStat*>> upgradesPctMap;

    std::unordered_map<uint32, StatRequirementContainer> baseStatRequirements;
    std::unordered_map<uint32, std::unordered_map<uint32, StatRequirementContainer>> overrideStatRequirements;

    UpgradeStatContainer weaponUpgradeStats;
    CharacterUpgradeContainer characterWeaponUpgradeData;
    StatRequirementContainer weaponUpgradeReqs;

    UpgradeStatContainer weaponSpeedUpgradeStats;
    CharacterUpgradeContainer characterWeaponSpeedUpgradeData;
    StatRequirementContainer weaponSpeedUpgradeReqs;

    static bool CompareIdentifier(const Identifier* a, const Identifier* b);
    static std::string CopperToMoneyStr(uint32 money, bool colored);
    static std::string FormatItemLocation(const Player* player, const Item* item);

    void CleanupDB(bool reload);
    void LoadStatRequirements();
    void LoadStatRequirementsOverrides();
    void LoadUpgradeStats();
    void LoadCharacterUpgradeData();
    void LoadCharacterWeaponUpgradeData();
    void LoadCharacterWeaponSpeedUpgradeData();
    void LoadAllowedItems();
    void LoadAllowedStatsItems();
    void LoadBlacklistedItems();
    void LoadBlacklistedStatsItems();
    bool IsValidReqType(uint8 reqType) const;
    bool ValidateReq(uint32 id, UpgradeStatReqType reqType, float val1, float val2, const std::string& table) const;
    void AddItemToPagedData(const Item* item, const Player* player, PagedData& pagedData);
    bool _AddPagedData(Player* player, const PagedData& pagedData, uint32 page) const;
    void NoPagedData(Player* player, const PagedData& pagedData) const;
    std::string ItemLinkForUI(const Item* item, const Player* player) const;
    void MergeStatRequirements(std::unordered_map<uint32, StatRequirementContainer>& statRequirementMap, bool validate = true);

    template <typename Func>
    const UpgradeStat* _FindUpgradeStat(const UpgradeStatContainer& upgradeStatContainer, Func f) const
    {
        UpgradeStatContainer::const_iterator citer = std::find_if(upgradeStatContainer.begin(), upgradeStatContainer.end(), f);
        if (citer != upgradeStatContainer.end())
            return &*citer;
        return nullptr;
    }

    const UpgradeStat* FindUpgradeStat(uint32 statId) const;
    const UpgradeStat* FindUpgradeStat(uint32 statType, uint16 rank) const;
    const UpgradeStat* FindWeaponUpgradeStat(const UpgradeStatContainer& upgradeStatContainer, float pct) const;
    const UpgradeStat* FindNearestWeaponUpgradeStat(const UpgradeStatContainer& upgradeStatContainer, float pct) const;
    const UpgradeStat* FindNextWeaponUpgradeStat(const UpgradeStatContainer& upgradeStatContainer, float pct) const;
    std::vector<const UpgradeStat*> _FindUpgradesForItem(const CharacterUpgradeContainer& characterUpgradeDataContainer, const Player* player, const Item* item) const;
    const UpgradeStat* FindUpgradeForItem(const Player* player, const Item* item, uint32 statType) const;
    bool MeetsRequirement(const Player* player, const UpgradeStatReq& req) const;
    bool MeetsRequirement(const Player* player, const UpgradeStat* upgradeStat, const Item* item) const;
    bool MeetsRequirement(const Player* player, const StatRequirementContainer* reqs) const;
    void TakeRequirements(Player* player, const UpgradeStat* upgradeStat, const Item* item);
    void TakeRequirements(Player* player, const StatRequirementContainer* reqs);
    void TakeWeaponUpgradeRequirements(Player* player);
    void TakeWeaponSpeedUpgradeRequirements(Player* player);
    bool PurchaseUpgrade(Player* player);
    bool PurchaseWeaponUpgrade(Player* player);
    bool PurchaseWeaponSpeedUpgrade(Player* player);
    void AddUpgradedItemToPagedData(const Item* item, const Player* player, PagedData& pagedData, const std::string &from);
    void HandleDataReload(Player* player, bool apply);
    std::vector<Item*> GetPlayerItems(const Player* player, bool inBankAlso) const;
    bool IsAllowedItem(const Item* item) const;
    bool IsBlacklistedItem(const Item* item) const;
    void SendItemPacket(Player* player, Item* item) const;
    std::pair<uint32, uint32> CalculateItemLevel(const Player* player, Item* item, const UpgradeStat* upgrade = nullptr) const;
    std::pair<uint32, uint32> CalculateItemLevel(const Player* player, Item* item, std::unordered_map<uint32, const UpgradeStat*>) const;
    void RemoveItemUpgradeFromContainer(CharacterUpgradeContainer& upgradesContainer, Player* player, Item* item);
    void RemoveItemUpgrade(Player* player, Item* item);
    void RemoveWeaponUpgrade(Player* player, Item* item);
    void RemoveWeaponSpeedUpgrade(Player* player, Item* item);
    bool AddUpgradeForNewItem(Player* player, Item* item, const UpgradeStat* upgrade, const _ItemStat* stat);
    void AddItemUpgradeToDB(const Player* player, const Item* item, const UpgradeStat* upgrade) const;
    const UpgradeStat* FindNearestUpgradeStat(uint32 statType, uint16 rank, const Item* item) const;
    bool IsAllowedStatForItem(const Item* item, const UpgradeStat* upgrade) const;
    bool IsBlacklistedStatForItem(const Item* item, const UpgradeStat* upgrade) const;
    bool CanApplyUpgradeForItem(const Item* item, const UpgradeStat* upgrade) const;
    Item* FindItemIdentifierFromPage(const PagedData& pagedData, uint32 id, Player* player) const;
    void CreateUpgradesPctMap();
    std::unordered_map<uint32, const UpgradeStat*> FindAllUpgradeableRanks(const Player* player, const Item* item, float pct) const;
    StatRequirementContainer BuildBulkRequirements(const std::unordered_map<uint32, const UpgradeStat*>& upgrades, const Item* item) const;
    void BuildRequirementsPage(const Player* player, PagedData& pagedData, const StatRequirementContainer* reqs) const;
    bool PurchaseUpgradeBulk(Player* player);
    bool HandlePurchaseRank(Player* player, Item* item, const UpgradeStat* upgrade);
    bool HandlePurchaseWeaponUpgrade(Player* player, Item* item, const UpgradeStat* upgrade, bool speedUpgrade);
    bool CheckDataValidity() const;
    bool IsValidStatType(uint32 statType) const;
    const StatRequirementContainer* GetStatRequirements(const UpgradeStat* upgrade, const Item* item) const;
    bool EmptyRequirements(const StatRequirementContainer* reqs) const;
    void EquipItem(Player* player, Item* item);
    bool TryRefundRequirements(Player* player, const StatRequirementContainer& reqs);
    bool RefundEverything(Player* player, Item* item, const std::vector<const ItemUpgrade::UpgradeStat*>& upgrades);
    bool TryAddItem(Player* player, uint32 entry, uint32 count, bool add);
    bool IsAllowedStatType(uint32 statType) const;
    void LoadAllowedStats(const std::string& stats);

    void LoadWeaponUpgradePercents(UpgradeStatContainer& upgradeStats, CharacterUpgradeContainer& characterUpgradeContainer, const std::string& percents);
    bool MeetsWeaponUpgradeRequirement(const Player* player) const;
    bool MeetsWeaponSpeedUpgradeRequirement(const Player* player) const;

    bool PurgeUpgrade(Player* player, Item* item);
    bool PurgeWeaponUpgrade(Player* player, Item* item);
    bool PurgeWeaponSpeedUpgrade(Player* player, Item* item);

    ItemVisualsPriority GetItemVisualsPriority() const;

    void _BuildWeaponPercentUpgradesCatalogue(const Player* player, const Item* item, PagedDataType type);

    void RefreshWeaponSpeed(Player* player, EquipmentSlots slot);
};

#define sItemUpgrade ItemUpgrade::instance()

#endif
