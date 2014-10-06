//
// Created by Fedor Shubin on 6/19/14.
//

#ifndef __CCStoreEventDispatcher_H_
#define __CCStoreEventDispatcher_H_

#include "CCStoreEventHandler.h"
#include "CCAbstractAggregatedEventHandler.h"

namespace soomla {
    class CCStoreEventDispatcher: public CCAbstractAggregatedEventHandler<CCStoreEventHandler>, public CCStoreEventHandler {
    public:
        /**
        This class is singleton, access it with this function.
        */
        static CCStoreEventDispatcher *getInstance();

        bool init();

        virtual void onBillingNotSupported();

        virtual void onBillingSupported();

        virtual void onCurrencyBalanceChanged(CCVirtualCurrency *virtualCurrency, int balance, int amountAdded);

        virtual void onGoodBalanceChanged(CCVirtualGood *virtualGood, int balance, int amountAdded);

        virtual void onGoodEquipped(CCEquippableVG *equippableVG);

        virtual void onGoodUnEquipped(CCEquippableVG *equippableVG);

        virtual void onGoodUpgrade(CCVirtualGood *virtualGood, CCUpgradeVG *upgradeVG);

        virtual void onItemPurchased(CCPurchasableVirtualItem *purchasableVirtualItem);

        virtual void onItemPurchaseStarted(CCPurchasableVirtualItem *purchasableVirtualItem);

        virtual void onMarketPurchaseCancelled(CCPurchasableVirtualItem *purchasableVirtualItem);

        virtual void onMarketPurchase(CCPurchasableVirtualItem *purchasableVirtualItem, cocos2d::CCString *token, cocos2d::CCString *payload);

        virtual void onMarketPurchaseStarted(CCPurchasableVirtualItem *purchasableVirtualItem);

        virtual void onMarketPurchaseVerification(CCPurchasableVirtualItem *purchasableVirtualItem);

        virtual void onRestoreTransactionsFinished(bool success);

        virtual void onRestoreTransactionsStarted();

        virtual void onUnexpectedErrorInStore();

        virtual void onStoreControllerInitialized();

        virtual void onMarketItemsRefreshed(cocos2d::CCArray *virtualItems);

/** Android only events*/
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

        virtual void onMarketRefund(CCPurchasableVirtualItem *purchasableVirtualItem);

        virtual void onIabServiceStarted();

        virtual void onIabServiceStopped();
#endif

        virtual void onMarketItemsRefreshStarted();

        void handle__EVENT_BILLING_NOT_SUPPORTED(cocos2d::CCDictionary *paramaters);

        void handle__EVENT_BILLING_SUPPORTED(cocos2d::CCDictionary *paramaters);

        void handle__EVENT_CURRENCY_BALANCE_CHANGED(cocos2d::CCDictionary *paramaters);

        void handle__EVENT_GOOD_BALANCE_CHANGED(cocos2d::CCDictionary *parameters);

        void handle__EVENT_GOOD_EQUIPPED(cocos2d::CCDictionary *parameters);

        void handle__EVENT_GOOD_UNEQUIPPED(cocos2d::CCDictionary *parameters);

        void handle__EVENT_GOOD_UPGRADE(cocos2d::CCDictionary *parameters);

        void handle__EVENT_ITEM_PURCHASED(cocos2d::CCDictionary *parameters);

        void handle__EVENT_ITEM_PURCHASE_STARTED(cocos2d::CCDictionary *parameters);

        void handle__EVENT_MARKET_PURCHASE_CANCELED(cocos2d::CCDictionary *parameters);

        void handle__EVENT_MARKET_PURCHASE(cocos2d::CCDictionary *parameters);

        void handle__EVENT_MARKET_PURCHASE_STARTED(cocos2d::CCDictionary *parameters);

        void handle__EVENT_MARKET_ITEMS_REFRESHED(cocos2d::CCDictionary *parameters);

        void handle__EVENT_MARKET_ITEMS_REFRESH_STARTED(cocos2d::CCDictionary *parameters);

        void handle__EVENT_MARKET_PURCHASE_VERIFICATION(cocos2d::CCDictionary *parameters);

        void handle__EVENT_RESTORE_TRANSACTION_FINISHED(cocos2d::CCDictionary *parameters);

        void handle__EVENT_RESTORE_TRANSACTION_STARTED(cocos2d::CCDictionary *parameters);

        void handle__EVENT_UNEXPECTED_ERROR_IN_STORE(cocos2d::CCDictionary *parameters);

        void handle__EVENT_STORE_CONTROLLER_INITIALIZED(cocos2d::CCDictionary *parameters);

        void handle__EVENT_MARKET_REFUND(cocos2d::CCDictionary *parameters);

        void handle__EVENT_IAB_SERVICE_STARTED(cocos2d::CCDictionary *parameters);

        void handle__EVENT_IAB_SERVICE_STOPPED(cocos2d::CCDictionary *parameters);

    };
}


#endif //__CCStoreEventDispatcher_H_
