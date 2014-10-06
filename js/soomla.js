/**
 * Created by vedi on 1/21/14.
 */

var PrevSoomla = Soomla;
Soomla = new function () {

  var Soomla = _.extend(PrevSoomla, {Models: {}}); // merge with binding instance

  Soomla.DEBUG = false;

  var declareClass = Soomla.declareClass = function (ClassName, fields, parentClass) {
    var Clazz = function () {
      return _.extend(parentClass ? parentClass() : {}, fields ? fields : {}, {
        jsonType: ClassName
      });
    };
    Clazz.create = function (values) {
      return _.defaults(values ? _.omit(values, "jsonType") : {}, Clazz());
    };

    return Clazz;
  };

  /**
   * Domain
   */
  var Domain = Soomla.Models.SoomlaEntity = declareClass("Domain", {
  });

  /**
   * SoomlaEntity
   */
  var SoomlaEntity = Soomla.Models.SoomlaEntity = declareClass("SoomlaEntity", {
    name: "",
    description: "",
    itemId: null
  }, Domain);

  /**
   * VirtualItem
   */
  var VirtualItem = Soomla.Models.VirtualItem = declareClass("VirtualItem", {
  }, SoomlaEntity);

  /**
   * VirtualCategory
   */
  var VirtualCategory = Soomla.Models.VirtualCategory = declareClass("VirtualCategory", {
    name: "",
    goods_itemIds: null
  }, Domain);

  /**
   * MarketItem
   */
  var MarketItem = Soomla.Models.MarketItem = declareClass("MarketItem", {
    productId: null,
    consumable: null,
    price: null,
    market_price: null,
    market_title: null,
    market_desc: null
  }, Domain);
  MarketItem.Consumable = {
    NONCONSUMABLE: 0,
    CONSUMABLE: 1,
    SUBSCRIPTION: 2
  };

  var PURCHASE_TYPE = {
    MARKET: "market",
    VI: "virtualItem"
  };

  /**
   * PurchasableVirtualItem
   */
  var PurchasableVirtualItem = Soomla.Models.PurchasableVirtualItem = declareClass("PurchasableVirtualItem", {
    purchasableItem: null
  }, VirtualItem);

  /**
   * NonConsumableItem
   */
  var NonConsumableItem = Soomla.Models.NonConsumableItem = declareClass("NonConsumableItem", {

  }, PurchasableVirtualItem);

  /**
   * VirtualCurrency
   */
  var VirtualCurrency = Soomla.Models.VirtualCurrency = declareClass("VirtualCurrency", {
  }, VirtualItem);

  /**
   * VirtualCurrencyPack
   */
  var VirtualCurrencyPack = Soomla.Models.VirtualCurrencyPack = declareClass("v", {
    currency_amount: 0,
    currency_itemId: null
  }, PurchasableVirtualItem);

  /**
   * VirtualGood
   */
  var VirtualGood = Soomla.Models.VirtualGood = declareClass("VirtualGood", {
  }, PurchasableVirtualItem);

  /**
   * LifetimeVG
   */
  var LifetimeVG = Soomla.Models.LifetimeVG = declareClass("LifetimeVG", {
  }, VirtualGood);

  /**
   * EquippableVG
   */
  var EquippableVG = Soomla.Models.EquippableVG = declareClass("EquippableVG", {
    equipping: null
  }, LifetimeVG);
  EquippableVG.EquippingModel = {
    LOCAL: "local",
    CATEGORY: "category",
    GLOBAL: "global"
  };

  /**
   * SingleUseVG
   */
  var SingleUseVG = Soomla.Models.SingleUseVG = declareClass("SingleUseVG", {
  }, VirtualGood);

  /**
   * SingleUsePackVG
   */
  var SingleUsePackVG = Soomla.Models.SingleUsePackVG = declareClass("SingleUsePackVG", {
    good_itemId: null,
    good_amount: null
  }, VirtualGood);

  /**
   * UpgradeVG
   */
  var UpgradeVG = Soomla.Models.UpgradeVG = declareClass("UpgradeVG", {
    good_itemId: null,
    prev_itemId: null,
    next_itemId: null
  }, VirtualGood);

  /**
   * PurchaseType
   */
  var PurchaseType = Soomla.Models.PurchaseType = declareClass("PurchaseType", {
    purchaseType: null
  });

  /**
   * PurchaseWithMarket
   */
  var PurchaseWithMarket = Soomla.Models.PurchaseWithMarket = declareClass("PurchaseWithMarket", {
    purchaseType: PURCHASE_TYPE.MARKET,
    marketItem: null
  }, PurchaseType);

  PurchaseWithMarket.createWithMarketItem = function(productId, price) {
    var marketItem = MarketItem.create({
      productId: productId,
      consumable: MarketItem.Consumable.CONSUMABLE,
      price: price
    });
    return PurchaseWithMarket.create({marketItem: marketItem});
  };

  /**
   * PurchaseWithVirtualItem
   */
  var PurchaseWithVirtualItem = Soomla.Models.PurchaseWithVirtualItem = declareClass("PurchaseWithVirtualItem", {
    purchaseType: PURCHASE_TYPE.VI,
    pvi_itemId: null,
    pvi_amount: null
  }, PurchaseType);

  function extractModel(retParams) {
    return retParams.return;
  }

  function extractCollection(retParams) {
    var retArray = retParams.return;

    var result = [];
    for (var i = 0; i < retArray.length; i++) {
      result.push(retArray[i]);
    }
    return result;
  }

  /**
   * StoreInfo
   */
  var StoreInfo = Soomla.StoreInfo = declareClass("StoreInfo", {
    init: function(storeAssets) {
      callNative({
        method: "CCStoreAssets::init",
        version: storeAssets.version,
        storeAssets: storeAssets
      });
      return true;
    },
    getItemByItemId: function(itemId) {
      var retParams = callNative({
        method: "CCStoreInfo::getItemByItemId",
        itemId: itemId
      });
      return extractModel(retParams);
    },
    getPurchasableItemWithProductId: function(productId) {
      var retParams = callNative({
        method: "CCStoreInfo::getPurchasableItemWithProductId",
        productId: productId
      });
      return extractModel(retParams);
    },
    getCategoryForVirtualGood: function(goodItemId) {
      var retParams = callNative({
        method: "CCStoreInfo::getCategoryForVirtualGood",
        goodItemId: goodItemId
      });
      return extractModel(retParams);
    },
    getFirstUpgradeForVirtualGood: function(goodItemId) {
      var retParams = callNative({
        method: "CCStoreInfo::getFirstUpgradeForVirtualGood",
        goodItemId: goodItemId
      });
      return extractModel(retParams);
    },
    getLastUpgradeForVirtualGood: function(goodItemId) {
      var retParams = callNative({
        method: "CCStoreInfo::getLastUpgradeForVirtualGood",
        goodItemId: goodItemId
      });
      return extractModel(retParams);
    },
    getUpgradesForVirtualGood: function(goodItemId) {
      var retParams = callNative({
        method: "CCStoreInfo::getUpgradesForVirtualGood",
        goodItemId: goodItemId
      });

      return extractCollection(retParams);
    },
    getVirtualCurrencies: function() {
      var retParams = callNative({
        method: "CCStoreInfo::getVirtualCurrencies"
      });
      return extractCollection(retParams);
    },
    getVirtualGoods: function() {
      var retParams = callNative({
        method: "CCStoreInfo::getVirtualGoods"
      });
      return extractCollection(retParams);
    },
    getVirtualCurrencyPacks: function() {
      var retParams = callNative({
        method: "CCStoreInfo::getVirtualCurrencyPacks"
      });
      return extractCollection(retParams);
    },
    getNonConsumableItems: function() {
      var retParams = callNative({
        method: "CCStoreInfo::getNonConsumableItems"
      });
      return extractCollection(retParams);
    },
    getVirtualCategories: function() {
      var retParams = callNative({
        method: "CCStoreInfo::getVirtualCategories"
      });
      return extractCollection(retParams);
    }
  });

  StoreInfo.createShared = function(storeAssets) {
    var ret = new StoreInfo();
    if (ret.init(storeAssets)) {
      Soomla.storeInfo = ret;
    } else {
      Soomla.storeInfo = null;
    }
  };

  var IStoreAssets = Soomla.IStoreAssets = declareClass("IStoreAssets", {
    categories: [],
    currencies: [],
    currencyPacks: [],
    goods: {
      singleUse: [],
      lifetime: [],
      equippable: [],
      goodUpgrades: [],
      goodPacks: []
    },
    nonConsumables: [],
    version: 1
  });

  /**
   * EventHandler
   */
  var EventHandler = Soomla.EventHandler = declareClass("EventHandler", {
    onBillingNotSupported: function() {},
    onBillingSupported: function() {},
    onCurrencyBalanceChanged: function(virtualCurrency, balance, amountAdded) {},
    onGoodBalanceChanged: function(virtualGood, balance, amountAdded) {},
    onGoodEquipped: function(equippableVG) {},
    onGoodUnEquipped: function(equippableVG) {},
    onGoodUpgrade: function(virtualGood, upgradeVG) {},
    onItemPurchased: function(purchasableVirtualItem) {},
    onItemPurchaseStarted: function(purchasableVirtualItem) {},
    onMarketPurchaseCancelled: function(purchasableVirtualItem) {},
    onMarketPurchase: function(purchasableVirtualItem, token, payload) {},
    onMarketPurchaseStarted: function(purchasableVirtualItem) {},
    onMarketItemsRefreshStarted: function() {},
    onMarketItemsRefreshed: function(marketItems) {},
    onMarketPurchaseVerification: function(purchasableVirtualItem) {},
    onRestoreTransactionsStarted: function() {},
    onRestoreTransactionsFinished: function(success) {},
    onUnexpectedErrorInStore: function() {},
    onStoreControllerInitialized: function() {},
    // For Android only
    onMarketRefund: function(purchasableVirtualItem) {},
    onIabServiceStarted: function() {},
    onIabServiceStopped: function() {}
  });

  /**
   * Root definitions
   */
  Soomla.eventHandlers = [];
  Soomla.addEventHandler = Soomla.on = function (eventHandler) {
    Soomla.eventHandlers.push(eventHandler);
  };
  Soomla.removeEventHandler = Soomla.off = function (eventHandler) {
    var idx = Soomla.eventHandlers.indexOf(eventHandler);
    Soomla.eventHandlers.splice(idx, 1);
  };
  Soomla.easyNDKCallBack = function (parameters) {
    parameters = JSON.parse(parameters);
    try {
      var methodName = parameters.method || "";
      if (methodName == "CCStoreEventHandler::onBillingNotSupported") {
        _.forEach(Soomla.eventHandlers, function (eventHandler) {
          if (eventHandler.onBillingNotSupported) {
            eventHandler.onBillingNotSupported();
          }
        });
      }
      else if (methodName == "CCStoreEventHandler::onBillingSupported") {
        _.forEach(Soomla.eventHandlers, function (eventHandler) {
          if (eventHandler.onBillingSupported) {
            eventHandler.onBillingSupported();
          }
        });
      }
      else if (methodName == "CCStoreEventHandler::onCurrencyBalanceChanged") {
        var virtualCurrency = Soomla.storeInfo.getItemByItemId(parameters.itemId);
        _.forEach(Soomla.eventHandlers, function (eventHandler) {
          if (eventHandler.onCurrencyBalanceChanged) {
            eventHandler.onCurrencyBalanceChanged(virtualCurrency, parameters.balance, parameters.amountAdded);
          }
        });
      }
      else if (methodName == "CCStoreEventHandler::onGoodBalanceChanged") {
        var virtualGood = Soomla.storeInfo.getItemByItemId(parameters.itemId);
        _.forEach(Soomla.eventHandlers, function (eventHandler) {
          if (eventHandler.onGoodBalanceChanged) {
            eventHandler.onGoodBalanceChanged(virtualGood, parameters.balance, parameters.amountAdded);
          }
        });
      }
      else if (methodName == "CCStoreEventHandler::onGoodEquipped") {
        var equippableVG = Soomla.storeInfo.getItemByItemId(parameters.itemId);
        _.forEach(Soomla.eventHandlers, function (eventHandler) {
          if (eventHandler.onGoodEquipped) {
            eventHandler.onGoodEquipped(equippableVG);
          }
        });
      }
      else if (methodName == "CCStoreEventHandler::onGoodUnEquipped") {
        var equippableVG = Soomla.storeInfo.getItemByItemId(parameters.itemId);
        _.forEach(Soomla.eventHandlers, function (eventHandler) {
          if (eventHandler.onGoodUnEquipped) {
            eventHandler.onGoodUnEquipped(equippableVG);
          }
        });
      }
      else if (methodName == "CCStoreEventHandler::onGoodUpgrade") {
        var virtualGood = Soomla.storeInfo.getItemByItemId(parameters.itemId);
        var upgradeVG = Soomla.storeInfo.getItemByItemId(parameters.vguItemId);
        _.forEach(Soomla.eventHandlers, function (eventHandler) {
          if (eventHandler.onGoodUpgrade) {
            eventHandler.onGoodUpgrade(virtualGood, upgradeVG);
          }
        });
      }
      else if (methodName == "CCStoreEventHandler::onItemPurchased") {
        var purchasableVirtualItem = Soomla.storeInfo.getItemByItemId(parameters.itemId);
        _.forEach(Soomla.eventHandlers, function (eventHandler) {
          if (eventHandler.onItemPurchased) {
            eventHandler.onItemPurchased(purchasableVirtualItem);
          }
        });
      }
      else if (methodName == "CCStoreEventHandler::onItemPurchaseStarted") {
        var purchasableVirtualItem = Soomla.storeInfo.getItemByItemId(parameters.itemId);
        _.forEach(Soomla.eventHandlers, function (eventHandler) {
          if (eventHandler.onItemPurchaseStarted) {
            eventHandler.onItemPurchaseStarted(purchasableVirtualItem);
          }
        });
      }
      else if (methodName == "CCStoreEventHandler::onMarketPurchaseCancelled") {
        var purchasableVirtualItem = Soomla.storeInfo.getItemByItemId(parameters.itemId);
        _.forEach(Soomla.eventHandlers, function (eventHandler) {
          if (eventHandler.onMarketPurchaseCancelled) {
            eventHandler.onMarketPurchaseCancelled(purchasableVirtualItem);
          }
        });
      }
      else if (methodName == "CCStoreEventHandler::onMarketPurchase") {
        var purchasableVirtualItem = Soomla.storeInfo.getItemByItemId(parameters.itemId);
        var token = parameters.token;
        var payload = parameters.payload;
        _.forEach(Soomla.eventHandlers, function (eventHandler) {
          if (eventHandler.onMarketPurchase) {
            eventHandler.onMarketPurchase(purchasableVirtualItem, token, payload);
          }
        });
      }
      else if (methodName == "CCStoreEventHandler::onMarketPurchaseStarted") {
        var purchasableVirtualItem = Soomla.storeInfo.getItemByItemId(parameters.itemId);
        _.forEach(Soomla.eventHandlers, function (eventHandler) {
          if (eventHandler.onMarketPurchaseStarted) {
            eventHandler.onMarketPurchaseStarted(purchasableVirtualItem);
          }
        });
      }
      else if (methodName == "CCStoreEventHandler::onMarketItemsRefreshed") {
        var marketItemsDict = parameters.marketItems;
        var marketItems = [];
        _.forEach(marketItemsDict, function(marketItem) {

          // be careful confusing naming: snake_case VS camelCase
          var productId = marketItem.productId;
          var marketPrice = marketItem.marketPrice;
          var marketTitle = marketItem.marketTitle;
          var marketDescription = marketItem.marketDesc;

          var pvi = Soomla.storeInfo.getPurchasableItemWithProductId(productId);

          var purchaseWithMarket = pvi.purchaseType;
          var mi = purchaseWithMarket.marketItem;

          mi.marketPrice        = marketPrice;
          mi.marketTitle        = marketTitle;
          mi.marketDescription  = marketDescription;

          pvi.save();

          marketItems.push(pvi);
        });

        _.forEach(Soomla.eventHandlers, function (eventHandler) {
          if (eventHandler.onMarketItemsRefreshed) {
            eventHandler.onMarketItemsRefreshed(marketItems);
          }
        });
      }
      else if (methodName == "CCStoreEventHandler::onMarketItemsRefreshStarted") {
        _.forEach(Soomla.eventHandlers, function (eventHandler) {
          if (eventHandler.onMarketItemsRefreshStarted) {
            eventHandler.onMarketItemsRefreshStarted();
          }
        });
      }
      else if (methodName == "CCStoreEventHandler::onMarketPurchaseVerification") {
        var purchasableVirtualItem = Soomla.storeInfo.getItemByItemId(parameters.itemId);
        _.forEach(Soomla.eventHandlers, function (eventHandler) {
          if (eventHandler.onMarketPurchaseVerification) {
            eventHandler.onMarketPurchaseVerification(purchasableVirtualItem);
          }
        });
      }
      else if (methodName == "CCStoreEventHandler::onRestoreTransactionsFinished") {
        _.forEach(Soomla.eventHandlers, function (eventHandler) {
          if (eventHandler.onRestoreTransactionsFinished) {
            eventHandler.onRestoreTransactionsFinished(parameters.success);
          }
        });
      }
      else if (methodName == "CCStoreEventHandler::onRestoreTransactionsStarted") {
        _.forEach(Soomla.eventHandlers, function (eventHandler) {
          if (eventHandler.onRestoreTransactionsStarted) {
            eventHandler.onRestoreTransactionsStarted();
          }
        });
      }
      else if (methodName == "CCStoreEventHandler::onUnexpectedErrorInStore") {
        _.forEach(Soomla.eventHandlers, function (eventHandler) {
          if (eventHandler.onUnexpectedErrorInStore) {
            eventHandler.onUnexpectedErrorInStore();
          }
        });
      }
      else if (methodName == "CCStoreEventHandler::onStoreControllerInitialized") {
        _.forEach(Soomla.eventHandlers, function (eventHandler) {
          if (eventHandler.onStoreControllerInitialized) {
            eventHandler.onStoreControllerInitialized();
          }
        });
      }
      //  Android specific
      else if (methodName == "CCStoreEventHandler::onMarketRefund") {
        var purchasableVirtualItem = Soomla.storeInfo.getItemByItemId(parameters.itemId);
        _.forEach(Soomla.eventHandlers, function (eventHandler) {
          if (eventHandler.onMarketRefund) {
            eventHandler.onMarketRefund(purchasableVirtualItem);
          }
        });
      }
      else if (methodName == "CCStoreEventHandler::onIabServiceStarted") {
        _.forEach(Soomla.eventHandlers, function (eventHandler) {
          if (eventHandler.onIabServiceStarted) {
            eventHandler.onIabServiceStarted();
          }
        });
      }
      else if (methodName == "CCStoreEventHandler::onIabServiceStopped") {
        _.forEach(Soomla.eventHandlers, function (eventHandler) {
          if (eventHandler.onIabServiceStopped) {
            eventHandler.onIabServiceStopped();
          }
        });
      }
    } catch (e) {
      logError("easyNDKCallBack: " + e.message);
    }
  };
  // put it into global context
  easyNDKCallBack = function(params) {
    Soomla.easyNDKCallBack.call(Soomla, params);
  };

  /**
   * SoomlaStore
   */
  var SoomlaStore = Soomla.SoomlaStore = declareClass("SoomlaStore", {
    SOOMLA_AND_PUB_KEY_DEFAULT: "YOUR GOOGLE PLAY PUBLIC KEY",
    SOOMLA_ONLY_ONCE_DEFAULT: "SET ONLY ONCE",
    init: function(storeAssets, storeParams) {

      // Redundancy checking. Most JS libraries don't do this. I hate it when they don't do this. Do this.
      var fields = ["customSecret", "androidPublicKey", "SSV", "testPurchases"];
      var wrongParams = _.omit(storeParams, fields);
      if (wrongParams.length > 0) {
        logDebug("WARNING!! Possible typo in member of storeParams: " + wrongParams);
      }

      storeParams = _.pick(storeParams, fields);
      storeParams.customSecret      = storeParams.customSecret || "";
      storeParams.androidPublicKey  = storeParams.androidPublicKey || "";
      storeParams.SSV  = storeParams.SSV === true || false;
      storeParams.testPurchases  = storeParams.testPurchases === true || false;

      if (storeParams.customSecret.length == 0) {
        logError("SOOMLA/COCOS2DX MISSING customSecret!!! Stopping here !!");
        return false;
      }

      if (storeParams.customSecret == this.SOOMLA_ONLY_ONCE_DEFAULT) {
        logError("SOOMLA/COCOS2DX You have to change customSecret!!! Stopping here !!");
        return false;
      }

      if (storeParams.androidPublicKey.length == 0) {
        logError("SOOMLA/COCOS2DX MISSING publickKey !!! Stopping here !!");
        return false;
      }

      if (storeParams.androidPublicKey == this.SOOMLA_AND_PUB_KEY_DEFAULT) {
        logError("SOOMLA/COCOS2DX You have to change android publicKey !!! Stopping here !!");
        return false;
      }

      callNative({
        method: "CCServiceManager::setCommonParams",
        params: {customSecret: storeParams.customSecret}
      });

      if (sys.os == "ios") {
        callNative({
          method: "CCSoomlaStore::setSSV",
          ssv: storeParams.SSV
        });
      }

      StoreInfo.createShared(storeAssets);

      callNative({
        method: "CCStoreService::init"
      });

      if (sys.os == "android") {
        callNative({
          method: "CCSoomlaStore::setAndroidPublicKey",
          androidPublicKey: storeParams.androidPublicKey
        });
        callNative({
          method: "CCSoomlaStore::setTestPurchases",
          testPurchases: storeParams.testPurchases
        });
      }

      return true;
    },
    buyMarketItem: function(productId, payload) {
      callNative({
        method: "CCSoomlaStore::buyMarketItem",
        productId: productId,
        payload: payload
      });
    },
    restoreTransactions: function() {
      callNative({
        method: "CCSoomlaStore::restoreTransactions"
      });
    },
    refreshInventory: function() {
      callNative({
        method: "CCSoomlaStore::refreshInventory"
      });
    },
    // TODO: For iOS only
    transactionsAlreadyRestored: function() {
      var retParams = callNative({
        method: "CCSoomlaStore::transactionsAlreadyRestored"
      });
      return retParams.return;
    },
    refreshMarketItemsDetails: function() {
      callNative({
        method: "CCSoomlaStore::refreshMarketItemsDetails"
      });
    },
    // TODO: For Android only
    startIabServiceInBg: function() {
      callNative({
        method: "CCSoomlaStore::startIabServiceInBg"
      });
    },
    // TODO: For Android only
    stopIabServiceInBg: function() {
      callNative({
        method: "CCSoomlaStore::stopIabServiceInBg"
      });
    }
  });

  SoomlaStore.createShared = function(storeAssets, storeParams) {
    var ret = new SoomlaStore();
    if (ret.init(storeAssets, storeParams)) {
      Soomla.soomlaStore = ret;
    } else {
      Soomla.soomlaStore = null;
    }
  };

  var StoreInventory = Soomla.StoreInventory = declareClass("StoreInventory", {
    buyItem: function(itemId, payload) {
      callNative({
        method: "CCStoreInventory::buyItem",
        payload: payload,
        itemId: itemId
      });
    },
    getItemBalance: function(itemId) {
      var retParams = callNative({
        method: "CCStoreInventory::getItemBalance",
        itemId: itemId
      });
      return retParams.return;
    },
    giveItem: function(itemId, amount) {
      callNative({
        method: "CCStoreInventory::giveItem",
        itemId: itemId,
        amount: amount
      });
    },
    takeItem: function(itemId, amount) {
      callNative({
        method: "CCStoreInventory::takeItem",
        itemId: itemId,
        amount: amount
      });
    },
    equipVirtualGood: function(itemId) {
      callNative({
        method: "CCStoreInventory::equipVirtualGood",
        itemId: itemId
      });
    },
    unEquipVirtualGood: function(itemId) {
      callNative({
        method: "CCStoreInventory::unEquipVirtualGood",
        itemId: itemId
      });
    },
    isVirtualGoodEquipped: function(itemId) {
      var retParams = callNative({
        method: "CCStoreInventory::isVirtualGoodEquipped",
        itemId: itemId
      });
      return retParams.return;
    },
    getGoodUpgradeLevel: function(goodItemId) {
      var retParams = callNative({
        method: "CCStoreInventory::getGoodUpgradeLevel",
        goodItemId: goodItemId
      });
      return retParams.return;
    },
    getGoodCurrentUpgrade: function(goodItemId) {
      var retParams = callNative({
        method: "CCStoreInventory::getGoodCurrentUpgrade",
        goodItemId: goodItemId
      });
      return retParams.return;
    },
    upgradeGood: function(goodItemId) {
      callNative({
        method: "CCStoreInventory::upgradeGood",
        goodItemId: goodItemId
      });
    },
    removeGoodUpgrades: function(goodItemId) {
      callNative({
        method: "CCStoreInventory::removeGoodUpgrades",
        goodItemId: goodItemId
      });
    },
    nonConsumableItemExists: function(nonConsItemId) {
      var retParams = callNative({
        method: "CCStoreInventory::nonConsumableItemExists",
        nonConsItemId: nonConsItemId
      });
      return retParams.return;
    },
    addNonConsumableItem: function(nonConsItemId) {
      callNative({
        method: "CCStoreInventory::addNonConsumableItem",
        nonConsItemId: nonConsItemId
      });
    },
    removeNonConsumableItem: function(nonConsItemId) {
      callNative({
        method: "CCStoreInventory::removeNonConsumableItem",
        nonConsItemId: nonConsItemId
      });
    }
  });

  Soomla.storeInventory = StoreInventory.create();

  function SoomlaException(code, message) {
    this.name = "SoomlaException";
    this.code = code || 0;
    this.message = (message || "");
  }
  SoomlaException.prototype = Error.prototype;
  SoomlaException.CODE = {
    ITEM_NOT_FOUND: -1,
    INSUFFICIENT_FUNDS: -2,
    NOT_ENOUGH_GOODS: -3,
    OTHER: -4
  };

  var callNative = function (params) {
    var jsonString = Soomla.CCSoomlaNdkBridge.callNative(JSON.stringify(params));
    var result = JSON.parse(jsonString);

    if (!result.success) {
      throw new SoomlaException(result.code, result.info);
    }
    return result.result;
  };

  var logDebug = Soomla.logDebug = function (output) {
    if (Soomla.DEBUG) {
      cc.log("DEBUG: " + output);
    }
  };

  var logError = Soomla.logError = function (output) {
    cc.log("ERROR: " + output);
  };

  var dumpError = Soomla.dumpError = function (e) {
    return e + " : " + JSON.stringify(e);
  };

  return Soomla
};
