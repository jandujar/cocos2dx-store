//
// Created by Fedor Shubin on 6/19/14.
//

#include "CCStoreEventDispatcher.h"
#include "CCSoomlaEventDispatcher.h"
#include "CCError.h"
#include "CCStoreInfo.h"
#include "CCSoomlaUtils.h"
#include "CCPurchaseWithMarket.h"

namespace soomla {

    USING_NS_CC;

    static CCStoreEventDispatcher *s_SharedInstance = NULL;

    CCStoreEventDispatcher *CCStoreEventDispatcher::getInstance() {
        if (!s_SharedInstance) {
            s_SharedInstance = new CCStoreEventDispatcher();
            s_SharedInstance->init();
        }

        return s_SharedInstance;
    }


    bool CCStoreEventDispatcher::init() {

        if (!CCAbstractAggregatedEventHandler::init()) {
            return false;
        }

        CCSoomlaEventDispatcher *eventDispatcher = CCSoomlaEventDispatcher::getInstance();

        eventDispatcher->registerEventHandler(CCStoreConsts::EVENT_BILLING_NOT_SUPPORTED,
                this, (SEL_EventHandler) (&CCStoreEventDispatcher::handle__EVENT_BILLING_NOT_SUPPORTED));

        eventDispatcher->registerEventHandler(CCStoreConsts::EVENT_BILLING_SUPPORTED,
                this, (SEL_EventHandler) (&CCStoreEventDispatcher::handle__EVENT_BILLING_SUPPORTED));

        eventDispatcher->registerEventHandler(CCStoreConsts::EVENT_CURRENCY_BALANCE_CHANGED,
                this, (SEL_EventHandler) (&CCStoreEventDispatcher::handle__EVENT_CURRENCY_BALANCE_CHANGED));

        eventDispatcher->registerEventHandler(CCStoreConsts::EVENT_GOOD_BALANCE_CHANGED,
                this, (SEL_EventHandler) (&CCStoreEventDispatcher::handle__EVENT_GOOD_BALANCE_CHANGED));

        eventDispatcher->registerEventHandler(CCStoreConsts::EVENT_GOOD_EQUIPPED,
                this, (SEL_EventHandler) (&CCStoreEventDispatcher::handle__EVENT_GOOD_EQUIPPED));

        eventDispatcher->registerEventHandler(CCStoreConsts::EVENT_GOOD_UNEQUIPPED,
                this, (SEL_EventHandler) (&CCStoreEventDispatcher::handle__EVENT_GOOD_UNEQUIPPED));

        eventDispatcher->registerEventHandler(CCStoreConsts::EVENT_GOOD_UPGRADE,
                this, (SEL_EventHandler) (&CCStoreEventDispatcher::handle__EVENT_GOOD_UPGRADE));

        eventDispatcher->registerEventHandler(CCStoreConsts::EVENT_ITEM_PURCHASED,
                this, (SEL_EventHandler) (&CCStoreEventDispatcher::handle__EVENT_ITEM_PURCHASED));

        eventDispatcher->registerEventHandler(CCStoreConsts::EVENT_ITEM_PURCHASE_STARTED,
                this, (SEL_EventHandler) (&CCStoreEventDispatcher::handle__EVENT_ITEM_PURCHASE_STARTED));

        eventDispatcher->registerEventHandler(CCStoreConsts::EVENT_MARKET_PURCHASE_CANCELED,
                this, (SEL_EventHandler) (&CCStoreEventDispatcher::handle__EVENT_MARKET_PURCHASE_CANCELED));

        eventDispatcher->registerEventHandler(CCStoreConsts::EVENT_MARKET_PURCHASE,
                this, (SEL_EventHandler) (&CCStoreEventDispatcher::handle__EVENT_MARKET_PURCHASE));

        eventDispatcher->registerEventHandler(CCStoreConsts::EVENT_MARKET_PURCHASE_STARTED,
                this, (SEL_EventHandler) (&CCStoreEventDispatcher::handle__EVENT_MARKET_PURCHASE_STARTED));

        eventDispatcher->registerEventHandler(CCStoreConsts::EVENT_MARKET_ITEMS_REFRESHED,
                this, (SEL_EventHandler) (&CCStoreEventDispatcher::handle__EVENT_MARKET_ITEMS_REFRESHED));

        eventDispatcher->registerEventHandler(CCStoreConsts::EVENT_MARKET_ITEMS_REFRESH_STARTED,
                this, (SEL_EventHandler) (&CCStoreEventDispatcher::handle__EVENT_MARKET_ITEMS_REFRESH_STARTED));

        eventDispatcher->registerEventHandler(CCStoreConsts::EVENT_MARKET_PURCHASE_VERIFICATION,
                this, (SEL_EventHandler) (&CCStoreEventDispatcher::handle__EVENT_MARKET_PURCHASE_VERIFICATION));

        eventDispatcher->registerEventHandler(CCStoreConsts::EVENT_RESTORE_TRANSACTION_FINISHED,
                this, (SEL_EventHandler) (&CCStoreEventDispatcher::handle__EVENT_RESTORE_TRANSACTION_FINISHED));

        eventDispatcher->registerEventHandler(CCStoreConsts::EVENT_RESTORE_TRANSACTION_STARTED,
                this, (SEL_EventHandler) (&CCStoreEventDispatcher::handle__EVENT_RESTORE_TRANSACTION_STARTED));

        eventDispatcher->registerEventHandler(CCStoreConsts::EVENT_UNEXPECTED_ERROR_IN_STORE,
                this, (SEL_EventHandler) (&CCStoreEventDispatcher::handle__EVENT_UNEXPECTED_ERROR_IN_STORE));

        eventDispatcher->registerEventHandler(CCStoreConsts::EVENT_STORE_CONTROLLER_INITIALIZED,
                this, (SEL_EventHandler) (&CCStoreEventDispatcher::handle__EVENT_STORE_CONTROLLER_INITIALIZED));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        eventDispatcher->registerEventHandler(CCStoreConsts::EVENT_MARKET_REFUND,
                this, (SEL_EventHandler) (&CCStoreEventDispatcher::handle__EVENT_MARKET_REFUND));

        eventDispatcher->registerEventHandler(CCStoreConsts::EVENT_IAB_SERVICE_STARTED,
                this, (SEL_EventHandler) (&CCStoreEventDispatcher::handle__EVENT_IAB_SERVICE_STARTED));

        eventDispatcher->registerEventHandler(CCStoreConsts::EVENT_IAB_SERVICE_STOPPED,
                this, (SEL_EventHandler) (&CCStoreEventDispatcher::handle__EVENT_IAB_SERVICE_STOPPED));
#endif
        return true;
    }

    void CCStoreEventDispatcher::onBillingNotSupported() {
        FOR_EACH_EVENT_HANDLER(CCStoreEventHandler)
            eventHandler->onBillingNotSupported();
        }
    }

    void CCStoreEventDispatcher::onBillingSupported() {
        FOR_EACH_EVENT_HANDLER(CCStoreEventHandler)
            eventHandler->onBillingSupported();
        }
    }

    void CCStoreEventDispatcher::onCurrencyBalanceChanged(CCVirtualCurrency *virtualCurrency, int balance, int amountAdded) {
        FOR_EACH_EVENT_HANDLER(CCStoreEventHandler)
            eventHandler->onCurrencyBalanceChanged(virtualCurrency, balance, amountAdded);
        }
    }

    void CCStoreEventDispatcher::onGoodBalanceChanged(CCVirtualGood *virtualGood, int balance, int amountAdded) {
        FOR_EACH_EVENT_HANDLER(CCStoreEventHandler)
            eventHandler->onGoodBalanceChanged(virtualGood, balance, amountAdded);
        }
    }

    void CCStoreEventDispatcher::onGoodEquipped(CCEquippableVG *equippableVG) {
        FOR_EACH_EVENT_HANDLER(CCStoreEventHandler)
            eventHandler->onGoodEquipped(equippableVG);
        }
    }

    void CCStoreEventDispatcher::onGoodUnEquipped(CCEquippableVG *equippableVG) {
        FOR_EACH_EVENT_HANDLER(CCStoreEventHandler)
            eventHandler->onGoodUnEquipped(equippableVG);
        }
    }

    void CCStoreEventDispatcher::onGoodUpgrade(CCVirtualGood *virtualGood, CCUpgradeVG *upgradeVG) {
        FOR_EACH_EVENT_HANDLER(CCStoreEventHandler)
            eventHandler->onGoodUpgrade(virtualGood, upgradeVG);
        }
    }

    void CCStoreEventDispatcher::onItemPurchased(CCPurchasableVirtualItem *purchasableVirtualItem) {
        FOR_EACH_EVENT_HANDLER(CCStoreEventHandler)
            eventHandler->onItemPurchased(purchasableVirtualItem);
        }
    }

    void CCStoreEventDispatcher::onItemPurchaseStarted(CCPurchasableVirtualItem *purchasableVirtualItem) {
        FOR_EACH_EVENT_HANDLER(CCStoreEventHandler)
            eventHandler->onItemPurchaseStarted(purchasableVirtualItem);
        }
    }

    void CCStoreEventDispatcher::onMarketPurchaseCancelled(CCPurchasableVirtualItem *purchasableVirtualItem) {
        FOR_EACH_EVENT_HANDLER(CCStoreEventHandler)
            eventHandler->onMarketPurchaseCancelled(purchasableVirtualItem);
        }
    }

    void CCStoreEventDispatcher::onMarketPurchase(CCPurchasableVirtualItem *purchasableVirtualItem, cocos2d::CCString *token, cocos2d::CCString *payload) {
        FOR_EACH_EVENT_HANDLER(CCStoreEventHandler)
            eventHandler->onMarketPurchase(purchasableVirtualItem, token, payload);
        }
    }

    void CCStoreEventDispatcher::onMarketPurchaseStarted(CCPurchasableVirtualItem *purchasableVirtualItem) {
        FOR_EACH_EVENT_HANDLER(CCStoreEventHandler)
            eventHandler->onMarketPurchaseStarted(purchasableVirtualItem);
        }
    }

    void CCStoreEventDispatcher::onMarketPurchaseVerification(CCPurchasableVirtualItem *purchasableVirtualItem) {
        FOR_EACH_EVENT_HANDLER(CCStoreEventHandler)
            eventHandler->onMarketPurchaseVerification(purchasableVirtualItem);
        }
    }

    void CCStoreEventDispatcher::onRestoreTransactionsFinished(bool success) {
        FOR_EACH_EVENT_HANDLER(CCStoreEventHandler)
            eventHandler->onRestoreTransactionsFinished(success);
        }
    }

    void CCStoreEventDispatcher::onRestoreTransactionsStarted() {
        FOR_EACH_EVENT_HANDLER(CCStoreEventHandler)
            eventHandler->onRestoreTransactionsStarted();
        }
    }

    void CCStoreEventDispatcher::onUnexpectedErrorInStore() {
        FOR_EACH_EVENT_HANDLER(CCStoreEventHandler)
            eventHandler->onUnexpectedErrorInStore();
        }
    }

    void CCStoreEventDispatcher::onStoreControllerInitialized() {
        FOR_EACH_EVENT_HANDLER(CCStoreEventHandler)
            eventHandler->onStoreControllerInitialized();
        }
    }

    void CCStoreEventDispatcher::onMarketItemsRefreshed(cocos2d::CCArray *virtualItems) {
        FOR_EACH_EVENT_HANDLER(CCStoreEventHandler)
            eventHandler->onMarketItemsRefreshed(virtualItems);
        }
    }

    void CCStoreEventDispatcher::onMarketItemsRefreshStarted() {
        FOR_EACH_EVENT_HANDLER(CCStoreEventHandler)
            eventHandler->onMarketItemsRefreshStarted();
        }
    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    void CCStoreEventDispatcher::onMarketRefund(CCPurchasableVirtualItem *purchasableVirtualItem) {
        FOR_EACH_EVENT_HANDLER(CCStoreEventHandler)
            eventHandler->onMarketRefund(purchasableVirtualItem);
        }
    }

    void CCStoreEventDispatcher::onIabServiceStarted() {
        FOR_EACH_EVENT_HANDLER(CCStoreEventHandler)
            eventHandler->onIabServiceStarted();
        }
    }

    void CCStoreEventDispatcher::onIabServiceStopped() {
        FOR_EACH_EVENT_HANDLER(CCStoreEventHandler)
            eventHandler->onIabServiceStopped();
        }
    }
#endif


    void CCStoreEventDispatcher::handle__EVENT_BILLING_NOT_SUPPORTED(cocos2d::CCDictionary *parameters) {
        this->onBillingNotSupported();
    }

    void CCStoreEventDispatcher::handle__EVENT_BILLING_SUPPORTED(cocos2d::CCDictionary *parameters) {
        this->onBillingSupported();
    }

    void CCStoreEventDispatcher::handle__EVENT_CURRENCY_BALANCE_CHANGED(cocos2d::CCDictionary *parameters) {
        CCString *itemId = (CCString *)(parameters->objectForKey("itemId"));
        CCInteger *balance = (CCInteger *)(parameters->objectForKey("balance"));
        CCInteger *amountAdded = (CCInteger *)(parameters->objectForKey("amountAdded"));
        CCError *error = NULL;
        CCVirtualCurrency *virtualCurrency =
                dynamic_cast<CCVirtualCurrency *>(CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId->getCString(), &error));
        if (error) {
            CCSoomlaUtils::logException(CCStoreConsts::EVENT_CURRENCY_BALANCE_CHANGED, error);
            return;
        }
        CC_ASSERT(virtualCurrency);
        this->onCurrencyBalanceChanged(virtualCurrency, balance->getValue(), amountAdded->getValue());
    }

    void CCStoreEventDispatcher::handle__EVENT_GOOD_BALANCE_CHANGED(cocos2d::CCDictionary *parameters) {
        CCString *itemId = (CCString *)(parameters->objectForKey("itemId"));
        CCInteger *balance = (CCInteger *)(parameters->objectForKey("balance"));
        CCInteger *amountAdded = (CCInteger *)(parameters->objectForKey("amountAdded"));
        CCError *error = NULL;
        CCVirtualGood *virtualGood =
                dynamic_cast<CCVirtualGood *>(CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId->getCString(), &error));
        if (error) {
            CCSoomlaUtils::logException(CCStoreConsts::EVENT_GOOD_BALANCE_CHANGED, error);
            return;
        }
        CC_ASSERT(virtualGood);
        this->onGoodBalanceChanged(virtualGood, balance->getValue(), amountAdded->getValue());
    }

    void CCStoreEventDispatcher::handle__EVENT_GOOD_EQUIPPED(cocos2d::CCDictionary *parameters) {
        CCString *itemId = (CCString *)(parameters->objectForKey("itemId"));
        CCError *error = NULL;
        CCEquippableVG *equippableVG =
                dynamic_cast<CCEquippableVG *>(CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId->getCString(), &error));
        if (error) {
            CCSoomlaUtils::logException(CCStoreConsts::EVENT_GOOD_EQUIPPED, error);
            return;
        }
        CC_ASSERT(equippableVG);
        this->onGoodEquipped(equippableVG);
    }

    void CCStoreEventDispatcher::handle__EVENT_GOOD_UNEQUIPPED(cocos2d::CCDictionary *parameters) {
        CCString *itemId = (CCString *)(parameters->objectForKey("itemId"));
        CCError *error = NULL;
        CCEquippableVG *equippableVG =
                dynamic_cast<CCEquippableVG *>(CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId->getCString(), &error));
        if (error) {
            CCSoomlaUtils::logException(CCStoreConsts::EVENT_GOOD_UNEQUIPPED, error);
            return;
        }
        CC_ASSERT(equippableVG);
        this->onGoodUnEquipped(equippableVG);
    }

    void CCStoreEventDispatcher::handle__EVENT_GOOD_UPGRADE(cocos2d::CCDictionary *parameters) {
        CCString *itemId = (CCString *) (parameters->objectForKey("itemId"));
        CCString *vguItemId = (CCString *) (parameters->objectForKey("vguItemId"));

        CCError *error;

        error = NULL;
        CCVirtualGood *virtualGood =
                dynamic_cast<CCVirtualGood *>(CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId->getCString(), &error));
        if (error) {
            CCSoomlaUtils::logException(CCStoreConsts::EVENT_GOOD_UPGRADE, error);
            return;
        }
        CC_ASSERT(virtualGood);

        error = NULL;
        CCUpgradeVG *upgradeVG =
                dynamic_cast<CCUpgradeVG *>(CCStoreInfo::sharedStoreInfo()->getItemByItemId(vguItemId->getCString(), &error));
        if (error) {
            CCSoomlaUtils::logException(CCStoreConsts::EVENT_GOOD_UPGRADE, error);
            return;
        }
        CC_ASSERT(upgradeVG);
        this->onGoodUpgrade(virtualGood, upgradeVG);
    }

    void CCStoreEventDispatcher::handle__EVENT_ITEM_PURCHASED(cocos2d::CCDictionary *parameters) {
        CCString *itemId = (CCString *)(parameters->objectForKey("itemId"));
        CCError *error = NULL;
        CCPurchasableVirtualItem *purchasableVirtualItem =
                dynamic_cast<CCPurchasableVirtualItem *>(CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId->getCString(), &error));
        if (error) {
            CCSoomlaUtils::logException(CCStoreConsts::EVENT_ITEM_PURCHASED, error);
            return;
        }
        CC_ASSERT(purchasableVirtualItem);
        this->onItemPurchased(purchasableVirtualItem);
    }

    void CCStoreEventDispatcher::handle__EVENT_ITEM_PURCHASE_STARTED(cocos2d::CCDictionary *parameters) {
        CCString *itemId = (CCString *)(parameters->objectForKey("itemId"));
        CCError *error = NULL;
        CCPurchasableVirtualItem *purchasableVirtualItem = dynamic_cast<CCPurchasableVirtualItem *>(
                CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId->getCString(), &error));
        if (error) {
            CCSoomlaUtils::logException(CCStoreConsts::EVENT_ITEM_PURCHASE_STARTED, error);
            return;
        }
        CC_ASSERT(purchasableVirtualItem);
        this->onItemPurchaseStarted(purchasableVirtualItem);
    }

    void CCStoreEventDispatcher::handle__EVENT_MARKET_PURCHASE_CANCELED(cocos2d::CCDictionary *parameters) {
        CCString *itemId = (CCString *)(parameters->objectForKey("itemId"));
        CCError *error = NULL;
        CCPurchasableVirtualItem *purchasableVirtualItem =
                dynamic_cast<CCPurchasableVirtualItem *>(CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId->getCString(), &error));
        if (error) {
            CCSoomlaUtils::logException(CCStoreConsts::EVENT_MARKET_PURCHASE_CANCELED, error);
            return;
        }
        CC_ASSERT(purchasableVirtualItem);
        this->onMarketPurchaseCancelled(purchasableVirtualItem);
    }

    void CCStoreEventDispatcher::handle__EVENT_MARKET_PURCHASE(cocos2d::CCDictionary *parameters) {
        CCString *itemId = (CCString *)(parameters->objectForKey("itemId"));
        CCError *error = NULL;
        CCPurchasableVirtualItem *purchasableVirtualItem =
                dynamic_cast<CCPurchasableVirtualItem *>(CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId->getCString(), &error));
        if (error) {
            CCSoomlaUtils::logException(CCStoreConsts::EVENT_MARKET_PURCHASE, error);
            return;
        }
        CC_ASSERT(purchasableVirtualItem);
        CCString *token = (CCString *)(parameters->objectForKey("token"));
        CCString *payload = (CCString *)(parameters->objectForKey("payload"));
        this->onMarketPurchase(purchasableVirtualItem, token, payload);
    }

    void CCStoreEventDispatcher::handle__EVENT_MARKET_PURCHASE_STARTED(cocos2d::CCDictionary *parameters) {
        CCString *itemId = (CCString *) (parameters->objectForKey("itemId"));
        CCError *error = NULL;
        CCPurchasableVirtualItem *purchasableVirtualItem = dynamic_cast<CCPurchasableVirtualItem *>(
                CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId->getCString(), &error));
        if (error) {
            CCSoomlaUtils::logException(CCStoreConsts::EVENT_MARKET_PURCHASE_STARTED, error);
            return;
        }
        CC_ASSERT(purchasableVirtualItem)
                ;
        this->onMarketPurchaseStarted(purchasableVirtualItem);
    }

    void CCStoreEventDispatcher::handle__EVENT_MARKET_ITEMS_REFRESHED(cocos2d::CCDictionary *parameters) {
        CCArray *marketItemDicts = (CCArray *)(parameters->objectForKey("marketItems"));
        CCArray *marketItems = CCArray::create();

        CCError *error = NULL;
        CCDictionary *marketItem = NULL;
        for (unsigned int i = 0; i < marketItemDicts->count(); i++) {
            marketItem = dynamic_cast<CCDictionary *>(marketItemDicts->objectAtIndex(i));
            CC_ASSERT(marketItem);
            CCString *productId = dynamic_cast<CCString *>(marketItem->objectForKey("productId"));
            CCString *marketPrice = dynamic_cast<CCString *>(marketItem->objectForKey("marketPrice"));
            CCString *marketTitle = dynamic_cast<CCString *>(marketItem->objectForKey("marketTitle"));
            CCString *marketDescription = dynamic_cast<CCString *>(marketItem->objectForKey("marketDesc"));
            CCString *marketCurrencyCode = dynamic_cast<CCString *>(marketItem->objectForKey("marketCurrencyCode"));
            CCInteger *marketPriceMicros = dynamic_cast<CCInteger *>(marketItem->objectForKey("marketPriceMicros"));

            CCPurchasableVirtualItem *pvi = CCStoreInfo::sharedStoreInfo()->getPurchasableItemWithProductId(
                    productId->getCString(), &error);
            if (error) {
                CCSoomlaUtils::logException(CCStoreConsts::EVENT_MARKET_ITEMS_REFRESHED, error);
                return;
            }
            CC_ASSERT(pvi);

            CCPurchaseWithMarket *purchaseWithMarket = dynamic_cast<CCPurchaseWithMarket *>(pvi->getPurchaseType());
            CC_ASSERT(purchaseWithMarket);
            CCMarketItem *mi = purchaseWithMarket->getMarketItem();
                        mi->setMarketPriceAndCurrency(marketPrice);
            mi->setMarketTitle(marketTitle);
            mi->setMarketDescription(marketDescription);
                        mi->setMarketCurrencyCode(marketCurrencyCode);
                        mi->setMarketPriceMicros(marketPriceMicros);
            pvi->save();

            marketItems->addObject(purchaseWithMarket);
        }

        this->onMarketItemsRefreshed(marketItems);
    }

    void CCStoreEventDispatcher::handle__EVENT_MARKET_ITEMS_REFRESH_STARTED(cocos2d::CCDictionary *parameters) {
        this->onMarketItemsRefreshStarted();
    }

    void CCStoreEventDispatcher::handle__EVENT_MARKET_PURCHASE_VERIFICATION(cocos2d::CCDictionary *parameters) {
        CCString *itemId = (CCString *)(parameters->objectForKey("itemId"));
        CCError *error = NULL;
        CCPurchasableVirtualItem *purchasableVirtualItem =
                dynamic_cast<CCPurchasableVirtualItem *>(CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId->getCString(), &error));
        if (error) {
            CCSoomlaUtils::logException(CCStoreConsts::EVENT_MARKET_PURCHASE_VERIFICATION, error);
            return;
        }
        CC_ASSERT(purchasableVirtualItem);
        this->onMarketPurchaseVerification(purchasableVirtualItem);
    }

    void CCStoreEventDispatcher::handle__EVENT_RESTORE_TRANSACTION_FINISHED(cocos2d::CCDictionary *parameters) {
        CCBool *success = (CCBool *) (parameters->objectForKey("success"));
        this->onRestoreTransactionsFinished(success->getValue());
    }

    void CCStoreEventDispatcher::handle__EVENT_RESTORE_TRANSACTION_STARTED(cocos2d::CCDictionary *parameters) {
        this->onRestoreTransactionsStarted();
    }

    void CCStoreEventDispatcher::handle__EVENT_UNEXPECTED_ERROR_IN_STORE(cocos2d::CCDictionary *parameters) {
        this->onUnexpectedErrorInStore();
    }

    void CCStoreEventDispatcher::handle__EVENT_STORE_CONTROLLER_INITIALIZED(cocos2d::CCDictionary *parameters) {
        this->onStoreControllerInitialized();
    }

    void CCStoreEventDispatcher::handle__EVENT_MARKET_REFUND(cocos2d::CCDictionary *parameters) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        CCString *itemId = (CCString *)(parameters->objectForKey("itemId"));
        CCError *error = NULL;
        CCPurchasableVirtualItem *purchasableVirtualItem =
                dynamic_cast<CCPurchasableVirtualItem *>(CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId->getCString(), &error));
        if (error) {
            CCSoomlaUtils::logException(CCStoreConsts::EVENT_MARKET_REFUND, error);
            return;
        }
        CC_ASSERT(purchasableVirtualItem);
        this->onMarketRefund(purchasableVirtualItem);
#endif
    }

    void CCStoreEventDispatcher::handle__EVENT_IAB_SERVICE_STARTED(cocos2d::CCDictionary *parameters) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        this->onIabServiceStarted();
#endif
    }

    void CCStoreEventDispatcher::handle__EVENT_IAB_SERVICE_STOPPED(cocos2d::CCDictionary *parameters) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        this->onIabServiceStopped();
#endif
    }

}