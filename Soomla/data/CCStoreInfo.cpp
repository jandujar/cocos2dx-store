/*
 Copyright (C) 2012-2014 Soomla Inc.
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

// Created by Fedor Shubin on 5/21/13.

#include "CCStoreInfo.h"
#include "../domain/virtualGoods/CCSingleUseVG.h"
#include "../domain/virtualGoods/CCEquippableVG.h"
#include "../domain/virtualGoods/CCSingleUsePackVG.h"
#include "../domain/virtualCurrencies/CCVirtualCurrency.h"
#include "../domain/virtualCurrencies/CCVirtualCurrencyPack.h"
#include "../domain/CCMarketItem.h"
#include "CCNdkBridge.h"
#include "CCDomainFactory.h"
#include "CCDomainHelper.h"
#include "CCSoomlaUtils.h"

namespace soomla {

#define TAG "SOOMLA StoreInfo"

    USING_NS_CC;

    static CCStoreInfo *s_SharedStoreInfo = NULL;

    CCStoreInfo *CCStoreInfo::sharedStoreInfo() {
        return s_SharedStoreInfo;
    }

    void CCStoreInfo::createShared(CCStoreAssets *storeAssets) {
        CCStoreInfo *ret = new CCStoreInfo();
        if (ret->init(storeAssets)) {
            s_SharedStoreInfo = ret;
        } else {
            delete ret;
        }
    }

    bool CCStoreInfo::init(CCStoreAssets *storeAssets) {
        CCArray *currenciesJSON = CCArray::create();
        {
            CCArray *currencies = storeAssets->getCurrencies();
            CCObject *obj;
            CCARRAY_FOREACH(currencies, obj) {
				currenciesJSON->addObject(((CCVirtualCurrency *)obj)->toDictionary());
			}
        }

        CCArray *packsJSON = CCArray::create();
        {
            CCArray *packs = storeAssets->getCurrencyPacks();
            CCObject *obj;
            CCARRAY_FOREACH(packs, obj) {
				packsJSON->addObject(((CCVirtualCurrencyPack *)obj)->toDictionary());
			}
        }

        CCArray *suGoods = CCArray::create();
        CCArray *ltGoods = CCArray::create();
        CCArray *eqGoods = CCArray::create();
        CCArray *upGoods = CCArray::create();
        CCArray *paGoods = CCArray::create();

        CCObject *obj;
        CCARRAY_FOREACH(storeAssets->getGoods(), obj) {
			if (dynamic_cast<CCSingleUseVG *>(obj)) {
				suGoods->addObject(((CCSingleUseVG *)obj)->toDictionary());
			} else if (dynamic_cast<CCEquippableVG *>(obj)) {
				eqGoods->addObject(((CCEquippableVG *)obj)->toDictionary());
			} else if (dynamic_cast<CCLifetimeVG *>(obj)) {
				ltGoods->addObject(((CCLifetimeVG *)obj)->toDictionary());
			} else if (dynamic_cast<CCSingleUsePackVG *>(obj)) {
				paGoods->addObject(((CCSingleUsePackVG *)obj)->toDictionary());
			} else if (dynamic_cast<CCUpgradeVG *>(obj)) {
				upGoods->addObject(((CCUpgradeVG *)obj)->toDictionary());
			}
		}

        CCDictionary *goodsJSON = CCDictionary::create();
        goodsJSON->setObject(suGoods, CCStoreConsts::JSON_STORE_GOODS_SU);
        goodsJSON->setObject(ltGoods, CCStoreConsts::JSON_STORE_GOODS_LT);
        goodsJSON->setObject(eqGoods, CCStoreConsts::JSON_STORE_GOODS_EQ);
        goodsJSON->setObject(upGoods, CCStoreConsts::JSON_STORE_GOODS_UP);
        goodsJSON->setObject(paGoods, CCStoreConsts::JSON_STORE_GOODS_PA);

        CCArray *categoriesJSON = CCArray::create();
        {
            CCArray *categories = storeAssets->getCategories();
            CCObject *obj;
            CCARRAY_FOREACH(categories, obj) {
				categoriesJSON->addObject(((CCVirtualCategory *)obj)->toDictionary());
			}
        }

        CCDictionary *storeAssetsObj = CCDictionary::create();
        storeAssetsObj->setObject(categoriesJSON, CCStoreConsts::JSON_STORE_CATEGORIES);
        storeAssetsObj->setObject(currenciesJSON, CCStoreConsts::JSON_STORE_CURRENCIES);
        storeAssetsObj->setObject(packsJSON, CCStoreConsts::JSON_STORE_CURRENCY_PACKS);
        storeAssetsObj->setObject(goodsJSON, CCStoreConsts::JSON_STORE_GOODS);

        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCStoreAssets::init"), "method");
        params->setObject(CCInteger::create(storeAssets->getVersion()), "version");
        params->setObject(storeAssetsObj, "storeAssets");
        CCNdkBridge::callNative (params, NULL);


        return true;
    }

    CCVirtualItem *CCStoreInfo::getItemByItemId(char const *itemId, CCError **error) {
        CCSoomlaUtils::logDebug(TAG,
							   CCString::createWithFormat("Trying to fetch an item with itemId: %s", itemId)->getCString());

        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCStoreInfo::getItemByItemId"), "method");
        params->setObject(CCString::create(itemId), "itemId");
        CCDictionary *retParams = (CCDictionary *) CCNdkBridge::callNative (params, error);
        if (!*error) {
            SL_SAFE_CREATE_FROM_RETURN(CCVirtualItem *, ret, retParams);
            return ret;
        } else {
            CCSoomlaUtils::logDebug(TAG, CCString::createWithFormat(
                    "getItemByItemId: Cannot get item: %s", itemId)->getCString());
            return NULL;
        }
    }

    CCPurchasableVirtualItem *CCStoreInfo::getPurchasableItemWithProductId(char const *productId, CCError **error) {
        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCStoreInfo::getPurchasableItemWithProductId"), "method");
        params->setObject(CCString::create(productId), "productId");
        CCDictionary *retParams = (CCDictionary *) CCNdkBridge::callNative (params, error);
        if (!*error) {
            SL_SAFE_CREATE_FROM_RETURN(CCPurchasableVirtualItem *, ret, retParams);
            return ret;
        } else {
            CCSoomlaUtils::logDebug(TAG, CCString::createWithFormat(
                    "getPurchasableItemWithProductId: Cannot get item: %s", productId)->getCString());
            return NULL;
        }
    }

    CCVirtualCategory *CCStoreInfo::getCategoryForVirtualGood(char const *goodItemId, CCError **error) {
        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCStoreInfo::getCategoryForVirtualGood"), "method");
        params->setObject(CCString::create(goodItemId), "goodItemId");
        CCDictionary *retParams = (CCDictionary *) CCNdkBridge::callNative (params, error);
        if (!*error) {
            SL_SAFE_CREATE_FROM_RETURN(CCVirtualCategory *, ret, retParams);
            return ret;
        } else {
            CCSoomlaUtils::logDebug(TAG, CCString::createWithFormat(
                    "getCategoryForVirtualGood: Cannot get item: %s", goodItemId)->getCString());
            return NULL;
        }
    }

    CCUpgradeVG *CCStoreInfo::getFirstUpgradeForVirtualGood(char const *goodItemId) {
        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCStoreInfo::getFirstUpgradeForVirtualGood"), "method");
        params->setObject(CCString::create(goodItemId), "goodItemId");
        CCDictionary *retParams = (CCDictionary *) CCNdkBridge::callNative (params, NULL);
        SL_SAFE_CREATE_FROM_RETURN(CCUpgradeVG *, ret, retParams);
        return ret;
    }

    CCUpgradeVG *CCStoreInfo::getLastUpgradeForVirtualGood(char const *goodItemId) {
        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCStoreInfo::getLastUpgradeForVirtualGood"), "method");
        params->setObject(CCString::create(goodItemId), "goodItemId");
        CCDictionary *retParams = (CCDictionary *) CCNdkBridge::callNative (params, NULL);
        SL_SAFE_CREATE_FROM_RETURN(CCUpgradeVG *, ret, retParams);
        return ret;
    }

    CCArray *CCStoreInfo::getUpgradesForVirtualGood(char const *goodItemId) {
        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCStoreInfo::getUpgradesForVirtualGood"), "method");
        params->setObject(CCString::create(goodItemId), "goodItemId");
		CCDictionary *retParams = (CCDictionary *) CCNdkBridge::callNative (params, NULL);
		CCArray *retArray = (CCArray *)retParams->objectForKey("return");

        return CCDomainHelper::getInstance()->getDomainsFromDictArray(
                retArray, CCStoreConsts::JSON_JSON_TYPE_UPGRADE_VG);
    }

    CCArray *CCStoreInfo::getVirtualCurrencies() {
        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCStoreInfo::getVirtualCurrencies"), "method");
		CCDictionary *retParams = (CCDictionary *) CCNdkBridge::callNative (params, NULL);
		CCArray *retArray = (CCArray *)retParams->objectForKey("return");

        return CCDomainHelper::getInstance()->getDomainsFromDictArray(
                retArray, CCStoreConsts::JSON_JSON_TYPE_VIRTUAL_CURRENCY);
    }

    CCArray *CCStoreInfo::getVirtualGoods() {
        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCStoreInfo::getVirtualGoods"), "method");
		CCDictionary *retParams = (CCDictionary *) CCNdkBridge::callNative (params, NULL);
		CCArray *retArray = (CCArray *)retParams->objectForKey("return");

        return CCDomainHelper::getInstance()->getDomainsFromDictArray(retArray);
    }

    CCArray *CCStoreInfo::getVirtualCurrencyPacks() {
        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCStoreInfo::getVirtualCurrencyPacks"), "method");
        CCDictionary *retParams = (CCDictionary *) CCNdkBridge::callNative (params, NULL);
		CCArray *retArray = (CCArray *)retParams->objectForKey("return");

        return CCDomainHelper::getInstance()->getDomainsFromDictArray(
                retArray, CCStoreConsts::JSON_JSON_TYPE_VIRTUAL_CURRENCY_PACK);
    }

    CCArray *CCStoreInfo::getVirtualCategories() {
        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCStoreInfo::getVirtualCategories"), "method");
		CCDictionary *retParams = (CCDictionary *) CCNdkBridge::callNative (params, NULL);
		CCArray *retArray = (CCArray *)retParams->objectForKey("return");

        return CCDomainHelper::getInstance()->getDomainsFromDictArray(
                retArray, CCStoreConsts::JSON_JSON_TYPE_VIRTUAL_CATEGORY);
    }

    void CCStoreInfo::saveItem(CCVirtualItem *virtualItem) {
        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCStoreInfo::saveItem"), "method");
        params->setObject(virtualItem->toDictionary(), "virtualItem");
        CCNdkBridge::callNative (params, NULL);
    }

}

