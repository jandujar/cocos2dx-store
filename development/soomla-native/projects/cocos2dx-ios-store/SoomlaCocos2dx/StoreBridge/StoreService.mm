//
// Created by Fedor Shubin on 6/12/14.
//

#import "StoreTypeConsts.h"
#import "StoreService.h"
#import "NdkGlue.h"
#import "DomainHelper.h"
#import "StoreEventHandling.h"
#import "StoreAssetsBridge.h"
#import "SoomlaStore.h"
#import "InsufficientFundsException.h"
#import "NotEnoughGoodsException.h"
#import "VirtualCurrency.h"
#import "VirtualGood.h"
#import "EquippableVG.h"
#import "UpgradeVG.h"
#import "MarketItem.h"
#import "VirtualCategory.h"
#import "VirtualCurrencyPack.h"
#import "SingleUsePackVG.h"
#import "SingleUseVG.h"
#import "StoreInfo.h"
#import "PurchaseWithMarket.h"
#import "StoreConfig.h"
#import "SoomlaUtils.h"
#import "StoreInventory.h"
#import "VirtualItemNotFoundException.h"
#import "ParamsProvider.h"
#import "Soomla.h"
#import "DomainFactory.h"
#import "VirtualItemReward.h"

@interface StoreService ()
@end

@implementation StoreService {

}

+ (id)sharedStoreService {
    static StoreService *sharedProfileService = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedProfileService = [self alloc];
    });
    return sharedProfileService;
}

+ (void)initialize {
    [super initialize];
    [self initGlue];
    [self initDomainHelper];
}

+ (void)initDomainHelper {

    [[DomainHelper sharedDomainHelper] registerType:(NSString *)JSON_JSON_TYPE_EQUIPPABLE_VG
                                      withClassName:NSStringFromClass([EquippableVG class])
                                           andBlock:^id(NSDictionary *dict) {
                                               return [[EquippableVG alloc] initWithDictionary:dict] ;
                                           }];
    [[DomainHelper sharedDomainHelper] registerType:(NSString *)JSON_JSON_TYPE_LIFETIME_VG
                                      withClassName:NSStringFromClass([LifetimeVG class])
                                           andBlock:^id(NSDictionary *dict) {
                                               return [[LifetimeVG alloc] initWithDictionary:dict] ;
                                           }];
    [[DomainHelper sharedDomainHelper] registerType:(NSString *)JSON_JSON_TYPE_SINGLE_USE_PACK_VG
                                      withClassName:NSStringFromClass([SingleUsePackVG class])
                                           andBlock:^id(NSDictionary *dict) {
                                               return [[SingleUsePackVG alloc] initWithDictionary:dict] ;
                                           }];
    [[DomainHelper sharedDomainHelper] registerType:(NSString *)JSON_JSON_TYPE_SINGLE_USE_VG
                                      withClassName:NSStringFromClass([SingleUseVG class])
                                           andBlock:^id(NSDictionary *dict) {
                                               return [[SingleUseVG alloc] initWithDictionary:dict] ;
                                           }];
    [[DomainHelper sharedDomainHelper] registerType:(NSString *)JSON_JSON_TYPE_UPGRADE_VG
                                      withClassName:NSStringFromClass([UpgradeVG class])
                                           andBlock:^id(NSDictionary *dict) {
                                               return [[UpgradeVG alloc] initWithDictionary:dict] ;
                                           }];
    [[DomainHelper sharedDomainHelper] registerType:(NSString *)JSON_JSON_TYPE_VIRTUAL_CURRENCY
                                      withClassName:NSStringFromClass([VirtualCurrency class])
                                           andBlock:^id(NSDictionary *dict) {
                                               return [[VirtualCurrency alloc] initWithDictionary:dict] ;
                                           }];
    [[DomainHelper sharedDomainHelper] registerType:(NSString *)JSON_JSON_TYPE_VIRTUAL_CURRENCY_PACK
                                      withClassName:NSStringFromClass([VirtualCurrencyPack class])
                                           andBlock:^id(NSDictionary *dict) {
                                               return [[VirtualCurrencyPack alloc] initWithDictionary:dict] ;
                                           }];
    [[DomainHelper sharedDomainHelper] registerType:(NSString *)JSON_JSON_TYPE_MARKET_ITEM
                                      withClassName:NSStringFromClass([MarketItem class])
                                           andBlock:^id(NSDictionary *dict) {
                                               return [[MarketItem alloc] initWithDictionary:dict] ;
                                           }];
    [[DomainHelper sharedDomainHelper] registerType:(NSString *)JSON_JSON_TYPE_VIRTUAL_CATEGORY
                                      withClassName:NSStringFromClass([VirtualCategory class])
                                           andBlock:^id(NSDictionary *dict) {
                                               return [[VirtualCategory alloc] initWithDictionary:dict] ;
                                           }];
    [[DomainHelper sharedDomainHelper] registerType:(NSString *)JSON_JSON_TYPE_ITEM
                                      withClassName:NSStringFromClass([VirtualItemReward class])
                                           andBlock:^id(NSDictionary *dict) {
                                               return [[VirtualItemReward alloc] initWithDictionary:dict] ;
                                           }];
}

- (id)init {
    self = [super init];
    if (self) {
        [StoreEventHandling observeAllEventsWithObserver:[NdkGlue sharedInstance]
                                       withSelector:@selector(dispatchNdkCallback:)];
    }

    return self;
}

+ (void)initGlue {
    NdkGlue *ndkGlue = [NdkGlue sharedInstance];

    /* -= Call handlers =- */
    [ndkGlue registerCallHandlerForKey:@"CCStoreAssets::init" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        NSNumber *version = (NSNumber *) [parameters objectForKey:@"version"];
        NSDictionary *storeAssetsDict = (NSDictionary *) [parameters objectForKey:@"storeAssets"];
        [[StoreAssetsBridge sharedInstance] initializeWithStoreAssetsDict:storeAssetsDict andVersion:version.intValue];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCStoreService::init" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        [[StoreService sharedStoreService] init];
        NSDictionary *commonParams = [[ParamsProvider sharedParamsProvider] getParamsForKey:@"common"];
        NSString *customSecret = commonParams[@"customSecret"];
        [Soomla initializeWithSecret:customSecret];
        [[SoomlaStore getInstance] initializeWithStoreAssets:[StoreAssetsBridge sharedInstance]];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCSoomlaStore::buyMarketItem" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        NSString *productId = (NSString *) [parameters objectForKey:@"productId"];
        PurchasableVirtualItem *pvi = [[StoreInfo getInstance] purchasableItemWithProductId:productId];
        NSString *payload = [parameters objectForKey:@"payload"];
        [[SoomlaStore getInstance] buyInMarketWithMarketItem:((PurchaseWithMarket *) pvi.purchaseType).marketItem andPayload:payload];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCSoomlaStore::restoreTransactions" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        [[SoomlaStore getInstance] restoreTransactions];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCSoomlaStore::transactionsAlreadyRestored" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        bool res = [[SoomlaStore getInstance] transactionsAlreadyRestored];
        [retParameters setObject:[NSNumber numberWithBool:res] forKey:@"return"];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCSoomlaStore::refreshInventory" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        [[SoomlaStore getInstance] refreshInventory];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCSoomlaStore::setSSV" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        bool ssv = [(NSNumber*)[parameters objectForKey:@"ssv"] boolValue];
        LogDebug(@"SOOMLA SoomlaStoreBridge", ([NSString stringWithFormat:@"Setting iOS SSV to: %@", ssv ?@"true":@"false"]));
        VERIFY_PURCHASES = ssv;
    }];

    [ndkGlue registerCallHandlerForKey:@"CCSoomlaStore::refreshMarketItemsDetails" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        [[SoomlaStore getInstance] refreshMarketItemsDetails];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCStoreInventory::buyItem" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        NSString *itemId = [parameters objectForKey:@"itemId"];
        NSString *payload = [parameters objectForKey:@"payload"];
        [StoreInventory buyItemWithItemId:itemId andPayload:payload];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCStoreInventory::getItemBalance" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        NSString *itemId = (NSString *) [parameters objectForKey:@"itemId"];
        int res = [StoreInventory getItemBalance:itemId];
        [retParameters setObject:[NSNumber numberWithInt:res] forKey:@"return"];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCStoreInventory::giveItem" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        NSString *itemId = (NSString *) [parameters objectForKey:@"itemId"];
        NSNumber *amount = (NSNumber *) [parameters objectForKey:@"amount"];
        [StoreInventory giveAmount:[amount intValue] ofItem:itemId];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCStoreInventory::takeItem" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        NSString *itemId = (NSString *) [parameters objectForKey:@"itemId"];
        NSNumber *amount = (NSNumber *) [parameters objectForKey:@"amount"];
        [StoreInventory takeAmount:[amount intValue] ofItem:itemId];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCStoreInventory::equipVirtualGood" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        NSString *itemId = (NSString *) [parameters objectForKey:@"itemId"];
        [StoreInventory equipVirtualGoodWithItemId:itemId];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCStoreInventory::unEquipVirtualGood" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        NSString *itemId = (NSString *) [parameters objectForKey:@"itemId"];
        [StoreInventory unEquipVirtualGoodWithItemId:itemId];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCStoreInventory::isVirtualGoodEquipped" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        NSString *itemId = (NSString *) [parameters objectForKey:@"itemId"];
        bool res = [StoreInventory isVirtualGoodWithItemIdEquipped:itemId];
        [retParameters setObject:[NSNumber numberWithBool:res] forKey:@"return"];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCStoreInventory::getGoodUpgradeLevel" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        NSString *goodItemId = (NSString *) [parameters objectForKey:@"goodItemId"];
        int res = [StoreInventory goodUpgradeLevel:goodItemId];
        [retParameters setObject:[NSNumber numberWithInt:res] forKey:@"return"];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCStoreInventory::getGoodCurrentUpgrade" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        NSString *goodItemId = (NSString *) [parameters objectForKey:@"goodItemId"];
        NSString *res = [StoreInventory goodCurrentUpgrade:goodItemId];
        [retParameters setObject:res forKey:@"return"];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCStoreInventory::upgradeGood" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        NSString *goodItemId = (NSString *) [parameters objectForKey:@"goodItemId"];
        [StoreInventory upgradeVirtualGood:goodItemId];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCStoreInventory::removeGoodUpgrades" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        NSString *goodItemId = (NSString *) [parameters objectForKey:@"goodItemId"];
        [StoreInventory removeUpgrades:goodItemId];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCStoreInfo::getItemByItemId" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        NSString *itemId = (NSString *) [parameters objectForKey:@"itemId"];
        VirtualItem* vi = [[StoreInfo getInstance] virtualItemWithId:itemId];
        NSDictionary *retObj = [[DomainHelper sharedDomainHelper] domainToDict:vi];
        [retParameters setObject: retObj forKey: @"return"];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCStoreInfo::getPurchasableItemWithProductId" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        NSString *productId = (NSString *) [parameters objectForKey:@"productId"];
        PurchasableVirtualItem*pvi = [[StoreInfo getInstance] purchasableItemWithProductId:productId];
        NSDictionary *retObj = [[DomainHelper sharedDomainHelper] domainToDict:pvi];
        [retParameters setObject: retObj forKey: @"return"];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCStoreInfo::getCategoryForVirtualGood" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        NSString *goodItemId = (NSString *) [parameters objectForKey:@"goodItemId"];
        NSDictionary *retObj = [[DomainHelper sharedDomainHelper] domainToDict:[[StoreInfo getInstance] categoryForGoodWithItemId:goodItemId]];
        [retParameters setObject: retObj forKey: @"return"];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCStoreInfo::getFirstUpgradeForVirtualGood" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        NSString *goodItemId = (NSString *) [parameters objectForKey:@"goodItemId"];
        UpgradeVG *upgradeVG = [[StoreInfo getInstance] firstUpgradeForGoodWithItemId:goodItemId];
        NSDictionary *retObj = [[DomainHelper sharedDomainHelper] domainToDict:upgradeVG];
        [retParameters setObject: retObj forKey: @"return"];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCStoreInfo::getLastUpgradeForVirtualGood" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        NSString *goodItemId = (NSString *) [parameters objectForKey:@"goodItemId"];
        UpgradeVG *upgradeVG = [[StoreInfo getInstance] lastUpgradeForGoodWithItemId:goodItemId];
        NSDictionary *retObj = [[DomainHelper sharedDomainHelper] domainToDict:upgradeVG];
        [retParameters setObject: retObj forKey: @"return"];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCStoreInfo::getUpgradesForVirtualGood" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        NSString *goodItemId = (NSString *) [parameters objectForKey:@"goodItemId"];
        NSArray *upgrades = [[StoreInfo getInstance] upgradesForGoodWithItemId:goodItemId];
        NSArray *retObj = [[DomainHelper sharedDomainHelper] getDictListFromDomains:upgrades];
        [retParameters setObject: retObj forKey: @"return"];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCStoreInfo::getVirtualCurrencies" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        NSArray *virtualCurrencies = [[StoreInfo getInstance] virtualCurrencies];
        NSArray *retObj = [[DomainHelper sharedDomainHelper] getDictListFromDomains:virtualCurrencies];
        [retParameters setObject: retObj forKey: @"return"];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCStoreInfo::getVirtualGoods" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        NSArray *virtualGoods = [[StoreInfo getInstance] virtualGoods];
        NSArray *retObj = [[DomainHelper sharedDomainHelper] getDictListFromDomains:virtualGoods];
        [retParameters setObject: retObj forKey: @"return"];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCStoreInfo::getVirtualCurrencyPacks" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        NSArray *virtualCurrencyPacks = [[StoreInfo getInstance] virtualCurrencyPacks];
        NSArray *retObj = [[DomainHelper sharedDomainHelper] getDictListFromDomains:virtualCurrencyPacks];
        [retParameters setObject: retObj forKey: @"return"];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCStoreInfo::getVirtualCategories" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        NSArray *virtualCategories = [[StoreInfo getInstance] virtualCategories];
        NSArray *retObj = [[DomainHelper sharedDomainHelper] getDictListFromDomains:virtualCategories];
        [retParameters setObject: retObj forKey: @"return"];
    }];

    [ndkGlue registerCallHandlerForKey:@"CCStoreInfo::saveItem" withBlock:^(NSDictionary *parameters, NSMutableDictionary *retParameters) {
        NSDictionary *viDict = parameters[@"virtualItem"];
        [[StoreInfo getInstance] save:[[DomainFactory sharedDomainFactory] createWithDict:viDict]];
    }];

    /* -= Exception handlers =- */
    void (^exceptionHandler)(NSException *, NSDictionary *, NSMutableDictionary *) = ^(NSException *exception, NSDictionary *parameters, NSMutableDictionary *retParameters) {
        [retParameters setObject: NSStringFromClass([exception class]) forKey: @"errorInfo"];
    };
    [ndkGlue registerExceptionHandlerForKey:NSStringFromClass([VirtualItemNotFoundException class]) withBlock:exceptionHandler];
    [ndkGlue registerExceptionHandlerForKey:NSStringFromClass([InsufficientFundsException class]) withBlock:exceptionHandler];
    [ndkGlue registerExceptionHandlerForKey:NSStringFromClass([NotEnoughGoodsException class]) withBlock:exceptionHandler];

    /* -= Callback handlers =- */
    [ndkGlue registerCallbackHandlerForKey:EVENT_BILLING_NOT_SUPPORTED withBlock:^(NSNotification *notification, NSMutableDictionary *parameters) {
        [parameters setObject:@"CCStoreEventHandler::onBillingNotSupported" forKey:@"method"];
    }];

    [ndkGlue registerCallbackHandlerForKey:EVENT_BILLING_SUPPORTED withBlock:^(NSNotification *notification, NSMutableDictionary *parameters) {
        [parameters setObject:@"CCStoreEventHandler::onBillingSupported" forKey:@"method"];
    }];

    [ndkGlue registerCallbackHandlerForKey:EVENT_CURRENCY_BALANCE_CHANGED withBlock:^(NSNotification *notification, NSMutableDictionary *parameters) {
        [parameters setObject:@"CCStoreEventHandler::onCurrencyBalanceChanged" forKey:@"method"];
        [parameters setObject:[(VirtualCurrency*)[notification.userInfo objectForKey:DICT_ELEMENT_CURRENCY] itemId] forKey:@"itemId"];
        [parameters setObject:(NSNumber*)[notification.userInfo objectForKey:DICT_ELEMENT_BALANCE] forKey:@"balance"];
        [parameters setObject:(NSNumber*)[notification.userInfo objectForKey:DICT_ELEMENT_AMOUNT_ADDED] forKey:@"amountAdded"];
    }];

    [ndkGlue registerCallbackHandlerForKey:EVENT_GOOD_BALANCE_CHANGED withBlock:^(NSNotification *notification, NSMutableDictionary *parameters) {
        [parameters setObject:@"CCStoreEventHandler::onGoodBalanceChanged" forKey:@"method"];
        [parameters setObject:[(VirtualGood*)[notification.userInfo objectForKey:DICT_ELEMENT_GOOD] itemId] forKey:@"itemId"];
        [parameters setObject:(NSNumber*)[notification.userInfo objectForKey:DICT_ELEMENT_BALANCE] forKey:@"balance"];
        [parameters setObject:(NSNumber*)[notification.userInfo objectForKey:DICT_ELEMENT_AMOUNT_ADDED] forKey:@"amountAdded"];
    }];

    [ndkGlue registerCallbackHandlerForKey:EVENT_GOOD_EQUIPPED withBlock:^(NSNotification *notification, NSMutableDictionary *parameters) {
        EquippableVG* good = (EquippableVG*)[notification.userInfo objectForKey:DICT_ELEMENT_EquippableVG];
        [parameters setObject:@"CCStoreEventHandler::onGoodEquipped" forKey:@"method"];
        [parameters setObject:[good itemId] forKey:@"itemId"];
    }];

    [ndkGlue registerCallbackHandlerForKey:EVENT_GOOD_UNEQUIPPED withBlock:^(NSNotification *notification, NSMutableDictionary *parameters) {
        EquippableVG* good = (EquippableVG*)[notification.userInfo objectForKey:DICT_ELEMENT_EquippableVG];
        [parameters setObject:@"CCStoreEventHandler::onGoodUnEquipped" forKey:@"method"];
        [parameters setObject:[good itemId] forKey:@"itemId"];
    }];

    [ndkGlue registerCallbackHandlerForKey:EVENT_GOOD_UPGRADE withBlock:^(NSNotification *notification, NSMutableDictionary *parameters) {
        VirtualGood* good = (VirtualGood*)[notification.userInfo objectForKey:DICT_ELEMENT_GOOD];
        UpgradeVG* vgu = (UpgradeVG*)[notification.userInfo objectForKey:DICT_ELEMENT_UpgradeVG];
        [parameters setObject:@"CCStoreEventHandler::onGoodUpgrade" forKey:@"method"];
        [parameters setObject:[good itemId] forKey:@"itemId"];
        [parameters setObject:[vgu itemId] forKey:@"vguItemId"];
    }];

    [ndkGlue registerCallbackHandlerForKey:EVENT_ITEM_PURCHASED withBlock:^(NSNotification *notification, NSMutableDictionary *parameters) {
        PurchasableVirtualItem* pvi = (PurchasableVirtualItem*)[notification.userInfo objectForKey:DICT_ELEMENT_PURCHASABLE];
        NSString* payload = [notification.userInfo objectForKey:DICT_ELEMENT_DEVELOPERPAYLOAD];
        [parameters setObject:@"CCStoreEventHandler::onItemPurchased" forKey:@"method"];
        [parameters setObject:[pvi itemId] forKey:@"itemId"];
        [parameters setObject:payload forKey:@"payload"];
    }];

    [ndkGlue registerCallbackHandlerForKey:EVENT_ITEM_PURCHASE_STARTED withBlock:^(NSNotification *notification, NSMutableDictionary *parameters) {
        PurchasableVirtualItem* pvi = (PurchasableVirtualItem*)[notification.userInfo objectForKey:DICT_ELEMENT_PURCHASABLE];
        [parameters setObject:@"CCStoreEventHandler::onItemPurchaseStarted" forKey:@"method"];
        [parameters setObject:[pvi itemId] forKey:@"itemId"];
    }];

    [ndkGlue registerCallbackHandlerForKey:EVENT_MARKET_PURCHASE_CANCELLED withBlock:^(NSNotification *notification, NSMutableDictionary *parameters) {
        PurchasableVirtualItem* pvi = (PurchasableVirtualItem*)[notification.userInfo objectForKey:DICT_ELEMENT_PURCHASABLE];
        [parameters setObject:@"CCStoreEventHandler::onMarketPurchaseCancelled" forKey:@"method"];
        [parameters setObject:[pvi itemId] forKey:@"itemId"];
    }];

    [ndkGlue registerCallbackHandlerForKey:EVENT_MARKET_PURCHASED withBlock:^(NSNotification *notification, NSMutableDictionary *parameters) {
        PurchasableVirtualItem* pvi = (PurchasableVirtualItem*)[notification.userInfo objectForKey:DICT_ELEMENT_PURCHASABLE];
        NSString* purchaseToken = [notification.userInfo objectForKey:DICT_ELEMENT_TOKEN];
        [parameters setObject:@"CCStoreEventHandler::onMarketPurchase" forKey:@"method"];
        [parameters setObject:[pvi itemId] forKey:@"itemId"];
        [parameters setObject:@"[iOS Purchase no payload]" forKey:@"payload"];
        [parameters setObject:purchaseToken forKey:@"token"];
    }];

    [ndkGlue registerCallbackHandlerForKey:EVENT_MARKET_PURCHASE_STARTED withBlock:^(NSNotification *notification, NSMutableDictionary *parameters) {
        PurchasableVirtualItem* pvi = (PurchasableVirtualItem*)[notification.userInfo objectForKey:DICT_ELEMENT_PURCHASABLE];
        [parameters setObject:@"CCStoreEventHandler::onMarketPurchaseStarted" forKey:@"method"];
        [parameters setObject:[pvi itemId] forKey:@"itemId"];
    }];

    [ndkGlue registerCallbackHandlerForKey:EVENT_MARKET_ITEMS_REFRESH_FINISHED withBlock:^(NSNotification *notification, NSMutableDictionary *parameters) {
        NSArray* marketItems = (NSArray*)[notification.userInfo objectForKey:DICT_ELEMENT_MARKET_ITEMS];
        NSMutableArray* jsonArr = [NSMutableArray array];
        NSMutableDictionary *miDict;
        for (MarketItem* mi in marketItems) {
            miDict = [NSMutableDictionary dictionary];
            [miDict setObject:mi.productId forKey:@"productId"];
            [miDict setObject:mi.marketPriceAndCurrency forKey:@"marketPrice"];
            [miDict setObject:mi.marketTitle forKey:@"marketTitle"];
            [miDict setObject:mi.marketDescription forKey:@"marketDesc"];
            [miDict setObject:mi.marketCurrencyCode forKey:@"marketCurrencyCode"];
            [miDict setObject:@(mi.marketPriceMicros) forKey:@"marketPriceMicros"];
            [jsonArr addObject:miDict];
        }
        [parameters setObject:@"CCStoreEventHandler::onMarketItemsRefreshed" forKey:@"method"];
        [parameters setObject: jsonArr forKey:@"marketItems"];
    }];

    [ndkGlue registerCallbackHandlerForKey:EVENT_MARKET_ITEMS_REFRESH_STARTED withBlock:^(NSNotification *notification, NSMutableDictionary *parameters) {
        [parameters setObject:@"CCStoreEventHandler::onMarketItemsRefreshStarted" forKey:@"method"];
    }];

    [ndkGlue registerCallbackHandlerForKey:EVENT_MARKET_PURCHASE_VERIF withBlock:^(NSNotification *notification, NSMutableDictionary *parameters) {
        PurchasableVirtualItem* pvi = (PurchasableVirtualItem*)[notification.userInfo objectForKey:DICT_ELEMENT_PURCHASABLE];
        [parameters setObject:@"CCStoreEventHandler::onMarketPurchaseVerification" forKey:@"method"];
        [parameters setObject:[pvi itemId] forKey:@"itemId"];
    }];

    [ndkGlue registerCallbackHandlerForKey:EVENT_RESTORE_TRANSACTIONS_FINISHED withBlock:^(NSNotification *notification, NSMutableDictionary *parameters) {
        BOOL success = [(NSNumber*)[notification.userInfo objectForKey:DICT_ELEMENT_SUCCESS] boolValue];
        [parameters setObject:@"CCStoreEventHandler::onRestoreTransactionsFinished" forKey:@"method"];
        [parameters setObject: [NSNumber numberWithBool: success] forKey:@"success"];
    }];

    [ndkGlue registerCallbackHandlerForKey:EVENT_RESTORE_TRANSACTIONS_STARTED withBlock:^(NSNotification *notification, NSMutableDictionary *parameters) {
        [parameters setObject:@"CCStoreEventHandler::onRestoreTransactionsStarted" forKey:@"method"];
    }];

    [ndkGlue registerCallbackHandlerForKey:EVENT_UNEXPECTED_ERROR_IN_STORE withBlock:^(NSNotification *notification, NSMutableDictionary *parameters) {
        [parameters setObject:@"CCStoreEventHandler::onUnexpectedErrorInStore" forKey:@"method"];
    }];

    [ndkGlue registerCallbackHandlerForKey:EVENT_SOOMLASTORE_INIT withBlock:^(NSNotification *notification, NSMutableDictionary *parameters) {
        [parameters setObject:@"CCStoreEventHandler::onStoreControllerInitialized" forKey:@"method"];
    }];
}

@end