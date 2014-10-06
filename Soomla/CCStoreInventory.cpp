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

#include "CCStoreInventory.h"
#include "CCSoomlaUtils.h"
#include "CCNdkBridge.h"

namespace soomla {
#define TAG "SOOMLA StoreInventory"

    USING_NS_CC;

    static CCStoreInventory *s_SharedStoreInventory = NULL;

    CCStoreInventory *CCStoreInventory::sharedStoreInventory() {
        if (!s_SharedStoreInventory) {
          s_SharedStoreInventory = new CCStoreInventory();
          s_SharedStoreInventory->init();
        }
        
        return s_SharedStoreInventory;
    }

    CCStoreInventory::CCStoreInventory() {

    }

    CCStoreInventory::~CCStoreInventory() {

    }

    bool CCStoreInventory::init() {
        return true;
    }

    void CCStoreInventory::buyItem(char const *itemId, CCError **error) {
        buyItem(itemId, NULL, error);
    }

    void CCStoreInventory::buyItem(char const *itemId, const char *payload, CCError **error) {
        CCSoomlaUtils::logDebug(TAG,
                CCString::createWithFormat("SOOMLA/COCOS2DX Calling buyItem with: %s", itemId)->getCString());

        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCStoreInventory::buyItem"), "method");
        params->setObject(CCString::create(itemId), "itemId");
        params->setObject(CCString::create(payload != NULL ? payload : ""), "payload");
        CCNdkBridge::callNative (params, error);
    }

    int CCStoreInventory::getItemBalance(char const *itemId, CCError **error) {
        CCSoomlaUtils::logDebug(TAG,
                CCString::createWithFormat("SOOMLA/COCOS2DX Calling getItemBalance with: %s", itemId)->getCString());
        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCStoreInventory::getItemBalance"), "method");
        params->setObject(CCString::create(itemId), "itemId");
        CCDictionary *retParams = (CCDictionary *) CCNdkBridge::callNative (params, error);

        if (retParams == NULL) {
        	return 0;
        }

		CCInteger *retValue = (CCInteger *) retParams->objectForKey("return");
		if (retValue) {
			return retValue->getValue();
		} else {
			return 0;
		}
    }

    void CCStoreInventory::giveItem(char const *itemId, int amount, CCError **error) {
        CCSoomlaUtils::logDebug(TAG, CCString::createWithFormat(
                "SOOMLA/COCOS2DX Calling giveItem with itemId: %s and amount %d", itemId, amount)->getCString());

        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCStoreInventory::giveItem"), "method");
        params->setObject(CCString::create(itemId), "itemId");
        params->setObject(CCInteger::create(amount), "amount");
        CCNdkBridge::callNative (params, error);
    }

    void CCStoreInventory::takeItem(char const *itemId, int amount, CCError **error) {
        CCSoomlaUtils::logDebug(TAG, CCString::createWithFormat(
                "SOOMLA/COCOS2DX Calling takeItem with itemId: %s and amount %d", itemId, amount)->getCString());

        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCStoreInventory::takeItem"), "method");
        params->setObject(CCString::create(itemId), "itemId");
        params->setObject(CCInteger::create(amount), "amount");
        CCNdkBridge::callNative (params, error);
    }

    void CCStoreInventory::equipVirtualGood(char const *itemId, CCError **error) {
        CCSoomlaUtils::logDebug(TAG,
                CCString::createWithFormat("SOOMLA/COCOS2DX Calling equipVirtualGood with: %s", itemId)->getCString());

        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCStoreInventory::equipVirtualGood"), "method");
        params->setObject(CCString::create(itemId), "itemId");
        CCNdkBridge::callNative (params, error);
   }

    void CCStoreInventory::unEquipVirtualGood(char const *itemId, CCError **error) {
        CCSoomlaUtils::logDebug(TAG,
                CCString::createWithFormat("SOOMLA/COCOS2DX Calling unEquipVirtualGood with: %s", itemId)->getCString());

        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCStoreInventory::unEquipVirtualGood"), "method");
        params->setObject(CCString::create(itemId), "itemId");
        CCNdkBridge::callNative (params, error);
    }

    bool CCStoreInventory::isVirtualGoodEquipped(char const *itemId, CCError **error) {
        CCSoomlaUtils::logDebug(TAG,
                CCString::createWithFormat("SOOMLA/COCOS2DX Calling isVirtualGoodEquipped with: %s", itemId)->getCString());

        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCStoreInventory::isVirtualGoodEquipped"), "method");
        params->setObject(CCString::create(itemId), "itemId");
        CCDictionary *retParams = (CCDictionary *) CCNdkBridge::callNative (params, error);

        if (retParams == NULL) {
        	return false;
        }

		CCBool *retValue = (CCBool *) retParams->objectForKey("return");
		if (retValue) {
			return retValue->getValue();
		} else {
			return false;
		}
    }

    int CCStoreInventory::getGoodUpgradeLevel(char const *goodItemId, CCError **error) {
        CCSoomlaUtils::logDebug(TAG,
                CCString::createWithFormat("SOOMLA/COCOS2DX Calling getGoodUpgradeLevel with: %s", goodItemId)->getCString());
        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCStoreInventory::getGoodUpgradeLevel"), "method");
        params->setObject(CCString::create(goodItemId), "goodItemId");
        CCDictionary *retParams = (CCDictionary *) CCNdkBridge::callNative (params, error);

        if (retParams == NULL) {
        	return 0;
        }

		CCInteger *retValue = (CCInteger *) retParams->objectForKey("return");
		if (retValue) {
			return retValue->getValue();
		} else {
			return 0;
		}
    }

    std::string CCStoreInventory::getGoodCurrentUpgrade(char const *goodItemId, CCError **error) {
        CCSoomlaUtils::logDebug(TAG,
                CCString::createWithFormat("SOOMLA/COCOS2DX Calling getGoodCurrentUpgrade with: %s", goodItemId)->getCString());
        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCStoreInventory::getGoodCurrentUpgrade"), "method");
        params->setObject(CCString::create(goodItemId), "goodItemId");
        CCDictionary *retParams = (CCDictionary *) CCNdkBridge::callNative (params, error);

        if (retParams == NULL) {
        	return "";
        }

		CCString *retValue = (CCString *) retParams->objectForKey("return");
		if (retValue) {
			return retValue->getCString();
		} else {
			return "";
		}
    }

    void CCStoreInventory::upgradeGood(char const *goodItemId, CCError **error) {
        CCSoomlaUtils::logDebug(TAG,
                CCString::createWithFormat("SOOMLA/COCOS2DX Calling upgradeGood with: %s", goodItemId)->getCString());

        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCStoreInventory::upgradeGood"), "method");
        params->setObject(CCString::create(goodItemId), "goodItemId");
        CCNdkBridge::callNative (params, error);
    }

    void CCStoreInventory::removeGoodUpgrades(char const *goodItemId, CCError **error) {
        CCSoomlaUtils::logDebug(TAG,
                CCString::createWithFormat("SOOMLA/COCOS2DX Calling removeGoodUpgrades with: %s", goodItemId)->getCString());

        CCDictionary *params = CCDictionary::create();
        params->setObject(CCString::create("CCStoreInventory::removeGoodUpgrades"), "method");
        params->setObject(CCString::create(goodItemId), "goodItemId");
        CCNdkBridge::callNative (params, error);
    }
}
