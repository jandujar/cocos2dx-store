//
// Created by Shubin Fedor on 20/08/14.
// Copyright (c) 2014 SOOMLA. All rights reserved.
//

#include "CCSimpleStoreEventHandler.h"

void soomla::CCSimpleStoreEventHandler::onBillingNotSupported() {

}

void soomla::CCSimpleStoreEventHandler::onBillingSupported() {

}

void soomla::CCSimpleStoreEventHandler::onCurrencyBalanceChanged(soomla::CCVirtualCurrency *virtualCurrency, int balance, int amountAdded) {

}

void soomla::CCSimpleStoreEventHandler::onGoodBalanceChanged(soomla::CCVirtualGood *virtualGood, int balance, int amountAdded) {

}

void soomla::CCSimpleStoreEventHandler::onGoodEquipped(soomla::CCEquippableVG *equippableVG) {

}

void soomla::CCSimpleStoreEventHandler::onGoodUnEquipped(soomla::CCEquippableVG *equippableVG) {

}

void soomla::CCSimpleStoreEventHandler::onGoodUpgrade(soomla::CCVirtualGood *virtualGood, soomla::CCUpgradeVG *upgradeVG) {

}

void soomla::CCSimpleStoreEventHandler::onItemPurchased(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {

}

void soomla::CCSimpleStoreEventHandler::onItemPurchaseStarted(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {

}

void soomla::CCSimpleStoreEventHandler::onMarketPurchaseCancelled(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {

}

void soomla::CCSimpleStoreEventHandler::onMarketPurchase(soomla::CCPurchasableVirtualItem *purchasableVirtualItem, cocos2d::CCString *token, cocos2d::CCString *payload) {

}

void soomla::CCSimpleStoreEventHandler::onMarketPurchaseStarted(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {

}

void soomla::CCSimpleStoreEventHandler::onMarketPurchaseVerification(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {

}

void soomla::CCSimpleStoreEventHandler::onRestoreTransactionsFinished(bool success) {

}

void soomla::CCSimpleStoreEventHandler::onRestoreTransactionsStarted() {

}

void soomla::CCSimpleStoreEventHandler::onUnexpectedErrorInStore() {

}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void soomla::CCSimpleStoreEventHandler::onUnexpectedErrorInStoreWithMessage(int ecode, cocos2d::CCString *message){
    
}
#endif

void soomla::CCSimpleStoreEventHandler::onStoreControllerInitialized() {

}

void soomla::CCSimpleStoreEventHandler::onMarketItemsRefreshed(cocos2d::CCArray *virtualItems) {

}

void soomla::CCSimpleStoreEventHandler::onMarketItemsRefreshStarted() {

}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

void soomla::CCSimpleStoreEventHandler::onMarketRefund(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {

}

void soomla::CCSimpleStoreEventHandler::onIabServiceStarted() {

}

void soomla::CCSimpleStoreEventHandler::onIabServiceStopped() {

}

#endif