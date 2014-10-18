//
// Created by Shubin Fedor on 20/08/14.
// Copyright (c) 2014 SOOMLA. All rights reserved.
//


#ifndef __CCSimpleStoreEventHandler_H_
#define __CCSimpleStoreEventHandler_H_

#include "CCStoreEventHandler.h"

namespace soomla {
    class CCSimpleStoreEventHandler: public CCStoreEventHandler {
    public:

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

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        virtual void onUnexpectedErrorInStoreWithMessage(int ecode, cocos2d::CCString *message);
#endif

        virtual void onStoreControllerInitialized();

        virtual void onMarketItemsRefreshed(cocos2d::CCArray *virtualItems);

        virtual void onMarketItemsRefreshStarted();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

        virtual void onMarketRefund(CCPurchasableVirtualItem *purchasableVirtualItem);

        virtual void onIabServiceStarted();

        virtual void onIabServiceStopped();
#endif
    };
}

#endif //__CCSimpleStoreEventHandler_H_
