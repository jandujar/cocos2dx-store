package com.soomla.cocos2dx.store;

import android.opengl.GLSurfaceView;
import com.soomla.Soomla;
import com.soomla.SoomlaUtils;
import com.soomla.cocos2dx.common.*;
import com.soomla.rewards.VirtualItemReward;
import com.soomla.store.IStoreAssets;
import com.soomla.store.SoomlaStore;
import com.soomla.store.StoreInventory;
import com.soomla.store.data.StoreInfo;
import com.soomla.store.domain.*;
import com.soomla.store.domain.virtualCurrencies.VirtualCurrency;
import com.soomla.store.domain.virtualCurrencies.VirtualCurrencyPack;
import com.soomla.store.domain.virtualGoods.*;
import com.soomla.store.exceptions.InsufficientFundsException;
import com.soomla.store.exceptions.NotEnoughGoodsException;
import com.soomla.store.exceptions.VirtualItemNotFoundException;
import com.soomla.store.purchaseTypes.PurchaseWithMarket;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.lang.ref.WeakReference;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;

/**
 * @author vedi
 *         date 6/10/14
 *         time 11:08 AM
 */
public class StoreService extends AbstractSoomlaService {

    private static StoreService INSTANCE = null;

    private static IStoreAssets mStoreAssets   = null;
    private boolean inited = false;

    public static StoreService getInstance() {
        if (INSTANCE == null) {
            synchronized (StoreService.class) {
                if (INSTANCE == null) {
                    INSTANCE = new StoreService();
                }
            }
        }
        return INSTANCE;
    }

    @SuppressWarnings("FieldCanBeLocal")
    private StoreEventHandlerBridge storeEventHandlerBridge;

    public StoreService() {
        storeEventHandlerBridge = new StoreEventHandlerBridge();

        DomainHelper.getInstance().registerTypeWithClassName(StoreConsts.JSON_JSON_TYPE_MARKET_ITEM, MarketItem.class);
        DomainHelper.getInstance().registerTypeWithClassName(StoreConsts.JSON_JSON_TYPE_NON_CONSUMABLE_ITEM, NonConsumableItem.class);
        DomainHelper.getInstance().registerTypeWithClassName(StoreConsts.JSON_JSON_TYPE_VIRTUAL_CATEGORY, VirtualCategory.class);
        DomainHelper.getInstance().registerTypeWithClassName(StoreConsts.JSON_JSON_TYPE_VIRTUAL_CURRENCY, VirtualCurrency.class);
        DomainHelper.getInstance().registerTypeWithClassName(StoreConsts.JSON_JSON_TYPE_VIRTUAL_CURRENCY_PACK, VirtualCurrencyPack.class);
        DomainHelper.getInstance().registerTypeWithClassName(StoreConsts.JSON_JSON_TYPE_EQUIPPABLE_VG, EquippableVG.class);
        DomainHelper.getInstance().registerTypeWithClassName(StoreConsts.JSON_JSON_TYPE_LIFETIME_VG, LifetimeVG.class);
        DomainHelper.getInstance().registerTypeWithClassName(StoreConsts.JSON_JSON_TYPE_SINGLE_USE_PACK_VG, SingleUsePackVG.class);
        DomainHelper.getInstance().registerTypeWithClassName(StoreConsts.JSON_JSON_TYPE_SINGLE_USE_VG, SingleUseVG.class);
        DomainHelper.getInstance().registerTypeWithClassName(StoreConsts.JSON_JSON_TYPE_UPGRADE_VG, UpgradeVG.class);

        DomainHelper.getInstance().registerTypeWithClassName(StoreConsts.JSON_JSON_TYPE_ITEM, VirtualItemReward.class);

        final NdkGlue ndkGlue = NdkGlue.getInstance();

        ndkGlue.registerCallHandler("CCStoreAssets::init", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                init();
                int version = params.getInt("version");
                JSONObject storeAssetsJson = params.getJSONObject("storeAssets");
                mStoreAssets = new StoreAssetsBridge(version, storeAssetsJson);
            }
        });

        ndkGlue.registerCallHandler("CCStoreService::init", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String customSecret = ParamsProvider.getInstance().getParams("common").optString("customSecret");
                SoomlaUtils.LogDebug("SOOMLA", "initialize is called from java!");
                Soomla.initialize(customSecret);
                SoomlaStore.getInstance().initialize(mStoreAssets);
            }
        });

        ndkGlue.registerCallHandler("CCSoomlaStore::buyMarketItem", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String productId = params.getString("productId");
                String payload = params.getString("payload");
                SoomlaUtils.LogDebug("SOOMLA", "buyWithMarket is called from java with productId: " + productId + "!");
                PurchasableVirtualItem pvi = StoreInfo.getPurchasableItem(productId);
                if(pvi.getPurchaseType() instanceof PurchaseWithMarket) {
                    SoomlaStore.getInstance().buyWithMarket(((PurchaseWithMarket)pvi.getPurchaseType()).getMarketItem(), payload);
                } else {
                    throw new VirtualItemNotFoundException("productId", productId);
                }
            }
        });

        ndkGlue.registerCallHandler("CCSoomlaStore::startIabServiceInBg", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                SoomlaUtils.LogDebug("SOOMLA", "startIabServiceInBg is called from java!");
                SoomlaStore.getInstance().startIabServiceInBg();
            }
        });

        ndkGlue.registerCallHandler("CCSoomlaStore::stopIabServiceInBg", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                SoomlaUtils.LogDebug("SOOMLA", "stopIabServiceInBg is called from java!");
                SoomlaStore.getInstance().stopIabServiceInBg();
            }
        });

        ndkGlue.registerCallHandler("CCSoomlaStore::restoreTransactions", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                SoomlaUtils.LogDebug("SOOMLA", "restoreTransactions is called from java!");
                SoomlaStore.getInstance().restoreTransactions();
            }
        });

        ndkGlue.registerCallHandler("CCSoomlaStore::transactionsAlreadyRestored", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                throw new UnsupportedOperationException("transactionsAlreadyRestored has no use in Android");
            }
        });

        ndkGlue.registerCallHandler("CCSoomlaStore::refreshInventory", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                SoomlaUtils.LogDebug("SOOMLA", "refreshInventory is called from java!");
                SoomlaStore.getInstance().refreshInventory();
            }
        });

        ndkGlue.registerCallHandler("CCSoomlaStore::setAndroidPublicKey", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                try {
                    String publicKey = params.getString("androidPublicKey");

                    Class googlePlayClass = Class.forName("com.soomla.store.billing.google.GooglePlayIabService");
                    Method factoryMethod = googlePlayClass.getDeclaredMethod("getInstance");
                    Object singleton = factoryMethod.invoke(null, null);
                    Method setPKMethod = googlePlayClass.getDeclaredMethod("setPublicKey", String.class);
                    setPKMethod.invoke(singleton, publicKey);
                } catch (Exception e) {
                    SoomlaUtils.LogError("StoreService JNI", "Something happened while we were trying to run CCSoomlaStore::setAndroidPublicKey. error: " + e.getLocalizedMessage());
                    e.printStackTrace();
                }

            }
        });

        ndkGlue.registerCallHandler("CCSoomlaStore::setTestPurchases", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                try {
                    boolean testPurchases = params.getBoolean("testPurchases");
                    Class googlePlayClass = Class.forName("com.soomla.store.billing.google.GooglePlayIabService");
                    Field testPurchasesField = googlePlayClass.getDeclaredField("AllowAndroidTestPurchases");
                    testPurchasesField.set(null, testPurchases);
                } catch (Exception e) {
                    SoomlaUtils.LogError("StoreService JNI", "Something happened while we were trying to run CCSoomlaStore::setTestPurchases. error: " + e.getLocalizedMessage());
                    e.printStackTrace();
                }
            }
        });

        ndkGlue.registerCallHandler("CCStoreInventory::buyItem", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String itemId = params.getString("itemId");
                String payload = params.optString("payload");
                SoomlaUtils.LogDebug("SOOMLA", "buy is called from java!");
                StoreInventory.buy(itemId, payload);
            }
        });

        ndkGlue.registerCallHandler("CCStoreInventory::getItemBalance", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String itemId = params.getString("itemId");
                SoomlaUtils.LogDebug("SOOMLA", "getItemBalance is called from java!");
                int retValue = StoreInventory.getVirtualItemBalance(itemId);
                retParams.put("return", retValue);
            }
        });

        ndkGlue.registerCallHandler("CCStoreInventory::giveItem", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String itemId = params.getString("itemId");
                Integer amount = params.getInt("amount");
                SoomlaUtils.LogDebug("SOOMLA", "addCurrencyAmount is called from java!");
                StoreInventory.giveVirtualItem(itemId, amount);
            }
        });

        ndkGlue.registerCallHandler("CCStoreInventory::takeItem", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String itemId = params.getString("itemId");
                Integer amount = params.getInt("amount");
                SoomlaUtils.LogDebug("SOOMLA", "removeCurrencyAmount is called from java!");
                StoreInventory.takeVirtualItem(itemId, amount);
            }
        });

        ndkGlue.registerCallHandler("CCStoreInventory::equipVirtualGood", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String itemId = params.getString("itemId");
                SoomlaUtils.LogDebug("SOOMLA", "equipVirtualGood is called from java!");
                StoreInventory.equipVirtualGood(itemId);
            }
        });

        ndkGlue.registerCallHandler("CCStoreInventory::unEquipVirtualGood", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String itemId = params.getString("itemId");
                SoomlaUtils.LogDebug("SOOMLA", "unEquipVirtualGood is called from java!");
                StoreInventory.unEquipVirtualGood(itemId);
            }
        });

        ndkGlue.registerCallHandler("CCStoreInventory::isVirtualGoodEquipped", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String itemId = params.getString("itemId");
                SoomlaUtils.LogDebug("SOOMLA", "isVirtualGoodEquipped is called from java!");
                boolean retValue = StoreInventory.isVirtualGoodEquipped(itemId);
                retParams.put("return", retValue);
            }
        });

        ndkGlue.registerCallHandler("CCStoreInventory::getGoodUpgradeLevel", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String goodItemId = params.getString("goodItemId");
                SoomlaUtils.LogDebug("SOOMLA", "getGoodUpgradeLevel is called from java!");
                Integer retValue = StoreInventory.getGoodUpgradeLevel(goodItemId);
                retParams.put("return", retValue);
            }
        });

        ndkGlue.registerCallHandler("CCStoreInventory::getGoodCurrentUpgrade", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String goodItemId = params.getString("goodItemId");
                SoomlaUtils.LogDebug("SOOMLA", "removeGoodAmount is called from java!");
                String retValue = StoreInventory.getGoodCurrentUpgrade(goodItemId);
                retParams.put("return", retValue);
            }
        });

        ndkGlue.registerCallHandler("CCStoreInventory::upgradeGood", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String goodItemId = params.getString("goodItemId");
                SoomlaUtils.LogDebug("SOOMLA", "upgradeVirtualGood is called from java!");
                StoreInventory.upgradeVirtualGood(goodItemId);
            }
        });

        ndkGlue.registerCallHandler("CCStoreInventory::removeGoodUpgrades", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String goodItemId = params.getString("goodItemId");
                SoomlaUtils.LogDebug("SOOMLA", "removeUpgrades is called from java!");
                StoreInventory.removeUpgrades(goodItemId);
            }
        });

        ndkGlue.registerCallHandler("CCStoreInventory::nonConsumableItemExists", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String nonConsItemId = params.getString("nonConsItemId");
                SoomlaUtils.LogDebug("SOOMLA", "nonConsumableItemExists is called from java!");
                boolean retValue = StoreInventory.nonConsumableItemExists(nonConsItemId);
                retParams.put("return", retValue);
            }
        });

        ndkGlue.registerCallHandler("CCStoreInventory::addNonConsumableItem", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String nonConsItemId = params.getString("nonConsItemId");
                SoomlaUtils.LogDebug("SOOMLA", "addNonConsumableItem is called from java!");
                StoreInventory.addNonConsumableItem(nonConsItemId);
            }
        });

        ndkGlue.registerCallHandler("CCStoreInventory::removeNonConsumableItem", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String nonConsItemId = params.getString("nonConsItemId");
                SoomlaUtils.LogDebug("SOOMLA", "removeNonConsumableItem is called from java!");
                StoreInventory.removeNonConsumableItem(nonConsItemId);
            }
        });

        ndkGlue.registerCallHandler("CCStoreInfo::getItemByItemId", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String itemId = params.getString("itemId");
                VirtualItem virtualItem = StoreInfo.getVirtualItem(itemId);
                retParams.put("return", DomainHelper.getInstance().domainToJsonObject(virtualItem));
            }
        });

        ndkGlue.registerCallHandler("CCStoreInfo::getPurchasableItemWithProductId", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String productId = params.getString("productId");
                PurchasableVirtualItem purchasableVirtualItem = StoreInfo.getPurchasableItem(productId);
                retParams.put("return", DomainHelper.getInstance().domainToJsonObject(purchasableVirtualItem));
            }
        });

        ndkGlue.registerCallHandler("CCStoreInfo::getCategoryForVirtualGood", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String goodItemId = params.getString("goodItemId");
                JSONObject retValue = StoreInfo.getCategory(goodItemId).toJSONObject();
                retParams.put("return", retValue);
            }
        });

        ndkGlue.registerCallHandler("CCStoreInfo::getFirstUpgradeForVirtualGood", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String goodItemId = params.getString("goodItemId");
                UpgradeVG upgradeVG = StoreInfo.getGoodFirstUpgrade(goodItemId);
                retParams.put("return", DomainHelper.getInstance().domainToJsonObject(upgradeVG));
            }
        });

        ndkGlue.registerCallHandler("CCStoreInfo::getLastUpgradeForVirtualGood", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String goodItemId = params.getString("goodItemId");
                UpgradeVG upgradeVG = StoreInfo.getGoodLastUpgrade(goodItemId);
                retParams.put("return", DomainHelper.getInstance().domainToJsonObject(upgradeVG));
            }
        });

        ndkGlue.registerCallHandler("CCStoreInfo::getUpgradesForVirtualGood", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String goodItemId = params.getString("goodItemId");
                List<JSONObject> ret = new ArrayList<JSONObject>();
                List<UpgradeVG> upgradeVGs = StoreInfo.getGoodUpgrades(goodItemId);
                for (UpgradeVG upgradeVG : upgradeVGs) {
                    ret.add(DomainHelper.getInstance().domainToJsonObject(upgradeVG));
                }
                JSONArray retValue = new JSONArray(ret);
                retParams.put("return", retValue);
            }
        });

        ndkGlue.registerCallHandler("CCStoreInfo::getVirtualCurrencies", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                List<JSONObject> ret = new ArrayList<JSONObject>();
                List<VirtualCurrency> virtualCurrencies = StoreInfo.getCurrencies();
                for (VirtualCurrency virtualCurrency : virtualCurrencies) {
                    ret.add(DomainHelper.getInstance().domainToJsonObject(virtualCurrency));
                }
                JSONArray retValue = new JSONArray(ret);
                retParams.put("return", retValue);
            }
        });

        ndkGlue.registerCallHandler("CCStoreInfo::getVirtualGoods", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                List<JSONObject> ret = new ArrayList<JSONObject>();
                List<VirtualGood> virtualGoods = StoreInfo.getGoods();
                for (VirtualGood virtualGood : virtualGoods) {
                    ret.add(DomainHelper.getInstance().domainToJsonObject(virtualGood));
                }
                JSONArray retValue = new JSONArray(ret);
                retParams.put("return", retValue);
            }
        });

        ndkGlue.registerCallHandler("CCStoreInfo::getVirtualCurrencyPacks", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                List<JSONObject> ret = new ArrayList<JSONObject>();
                List<VirtualCurrencyPack> virtualCurrencyPacks = StoreInfo.getCurrencyPacks();
                for (VirtualCurrencyPack virtualCurrencyPack : virtualCurrencyPacks) {
                    ret.add(DomainHelper.getInstance().domainToJsonObject(virtualCurrencyPack));
                }
                JSONArray retValue = new JSONArray(ret);
                retParams.put("return", retValue);
            }
        });

        ndkGlue.registerCallHandler("CCStoreInfo::getNonConsumableItems", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                List<JSONObject> ret = new ArrayList<JSONObject>();
                List<NonConsumableItem> nonConsumableItems = StoreInfo.getNonConsumableItems();
                for (NonConsumableItem nonConsumableItem : nonConsumableItems) {
                    ret.add(DomainHelper.getInstance().domainToJsonObject(nonConsumableItem));
                }
                JSONArray retValue = new JSONArray(ret);
                retParams.put("return", retValue);
            }
        });

        ndkGlue.registerCallHandler("CCStoreInfo::getVirtualCategories", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                List<JSONObject> ret = new ArrayList<JSONObject>();
                List<VirtualCategory> virtualCategories = StoreInfo.getCategories();
                for (VirtualCategory virtualCategory : virtualCategories) {
                    ret.add(DomainHelper.getInstance().domainToJsonObject(virtualCategory));
                }
                JSONArray retValue = new JSONArray(ret);
                retParams.put("return", retValue);
            }
        });

        ndkGlue.registerCallHandler("CCStoreInfo::saveItem", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {

                JSONObject viJsonObject = params.getJSONObject("virtualItem");
                StoreInfo.save(DomainFactory.getInstance().<VirtualItem>createWithJsonObject(viJsonObject));
            }
        });

        final NdkGlue.ExceptionHandler exceptionHandler = new NdkGlue.ExceptionHandler() {
            @Override
            public void handle(Exception exception, JSONObject params, JSONObject retParams) throws Exception {
                retParams.put("errorInfo", exception.getClass().getName());
            }
        };

        ndkGlue.registerExceptionHandler(VirtualItemNotFoundException.class.getName(), exceptionHandler);
        ndkGlue.registerExceptionHandler(InsufficientFundsException.class.getName(), exceptionHandler);
        ndkGlue.registerExceptionHandler(NotEnoughGoodsException.class.getName(), exceptionHandler);
    }

    public void init() {
        final GLSurfaceView glSurfaceView = glSurfaceViewRef.get();
        if (glSurfaceView != null) {
            storeEventHandlerBridge.setGlSurfaceView(glSurfaceView);
        }

        inited = true;
    }

    public void setGlSurfaceView(GLSurfaceView glSurfaceView) {
        if (inited) {
            storeEventHandlerBridge.setGlSurfaceView(glSurfaceView);
        } else {
            glSurfaceViewRef = new WeakReference<GLSurfaceView>(glSurfaceView);
        }
    }
}
