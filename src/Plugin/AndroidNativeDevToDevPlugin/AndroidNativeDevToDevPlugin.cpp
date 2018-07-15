#include "AndroidNativeDevToDevPlugin.h"

#include "Kernel/Callback.h"

#include "Android/AndroidUtils.h"

#include "pybind/pybind.hpp"

#define DEVTODEV_JAVA_PREFIX org_Mengine_Build_DevToDev
#define DEVTODEV_JAVA_INTERFACE(function) MENGINE_JAVA_FUNCTION_INTERFACE(DEVTODEV_JAVA_PREFIX, DevToDevInteractionLayer, function)

static jclass mActivityClass;
static jmethodID jmethodID_onTutorialEvent;
static jmethodID jmethodID_setCurrentLevel;
static jmethodID jmethodID_onLevelUp;
static jmethodID jmethodID_onCurrencyAccrual;
static jmethodID jmethodID_onRealPayment;
static jmethodID jmethodID_onInAppPurchase;
static jmethodID jmethodID_onSimpleCustomEvent;

extern "C"
{
    //////////////////////////////////////////////////////////////////////////
    JNIEXPORT void JNICALL
        MENGINE_ACTIVITY_JAVA_INTERFACE( AndroidNativeDevToDev_1setupDevToDevJNI )(JNIEnv *mEnv, jclass cls)
    {
        mActivityClass = (jclass)(mEnv->NewGlobalRef( cls ));

        jmethodID_onTutorialEvent = mEnv->GetStaticMethodID( mActivityClass, "devtodevOnTutorialEvent", "(I)V" );
        jmethodID_setCurrentLevel = mEnv->GetStaticMethodID( mActivityClass, "devtodevSetCurrentLevel", "(I)V" );
        jmethodID_onLevelUp = mEnv->GetStaticMethodID( mActivityClass, "devtodevOnLevelUp", "(I)V" );
        jmethodID_onCurrencyAccrual = mEnv->GetStaticMethodID( mActivityClass, "devtodevOnCurrencyAccrual", "(Ljava/lang/String;II)V" );
        jmethodID_onRealPayment = mEnv->GetStaticMethodID( mActivityClass, "devtodevOnRealPayment", "(Ljava/lang/String;FLjava/lang/String;Ljava/lang/String;)V" );
        jmethodID_onInAppPurchase = mEnv->GetStaticMethodID( mActivityClass, "devtodevOnInAppPurchase", "(Ljava/lang/String;Ljava/lang/String;IILjava/lang/String;)V" );
        jmethodID_onSimpleCustomEvent = mEnv->GetStaticMethodID( mActivityClass, "devtodevOnSimpleCustomEvent", "(Ljava/lang/String;)V" );
    }

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PLUGIN_FACTORY( AndroidNativeDevToDev, Mengine::AndroidNativeDevToDevPlugin )
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    AndroidNativeDevToDevPlugin::AndroidNativeDevToDevPlugin()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    AndroidNativeDevToDevPlugin::~AndroidNativeDevToDevPlugin()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool AndroidNativeDevToDevPlugin::_avaliable()
    {
        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool AndroidNativeDevToDevPlugin::_initialize()
    {
        pybind::kernel_interface * kernel = pybind::get_kernel();

        pybind::def_functor( kernel, "androidDevToDevSetTutorialEvent", this, &AndroidNativeDevToDevPlugin::onTutorialEvent );
        pybind::def_functor( kernel, "androidDevToDevSetCurrentLevel", this, &AndroidNativeDevToDevPlugin::setCurrentLevel );
        pybind::def_functor( kernel, "androidDevToDevOnLevelUp", this, &AndroidNativeDevToDevPlugin::onLevelUp );
        pybind::def_functor( kernel, "androidDevToDevOnCurrencyAccrual", this, &AndroidNativeDevToDevPlugin::onCurrencyAccrual );
        pybind::def_functor( kernel, "androidDevToDevOnRealPayment", this, &AndroidNativeDevToDevPlugin::onRealPayment );
        pybind::def_functor( kernel, "androidDevToDevOnInAppPurchase", this, &AndroidNativeDevToDevPlugin::onInAppPurchase );
        pybind::def_functor( kernel, "androidDevToDevOnSimpleCustomEvent", this, &AndroidNativeDevToDevPlugin::onSimpleCustomEvent );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void AndroidNativeDevToDevPlugin::_finalize()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool AndroidNativeDevToDevPlugin::onTutorialEvent( int _stateOrStep )
    {
        JNIEnv * env = Mengine_JNI_GetEnv();
        
        jint jstateOrStep = static_cast<jint>(_stateOrStep);

        env->CallStaticVoidMethod( mActivityClass, jmethodID_onTutorialEvent, jstateOrStep );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool AndroidNativeDevToDevPlugin::setCurrentLevel( int _level )
    {
        JNIEnv *env = Mengine_JNI_GetEnv();
        
        jint jlevel = static_cast<jint>(_level);

        env->CallStaticVoidMethod( mActivityClass, jmethodID_setCurrentLevel, jlevel );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool AndroidNativeDevToDevPlugin::onLevelUp( int _level )
    {
        JNIEnv *env = Mengine_JNI_GetEnv();
        
        jint jlevel = static_cast<jint>(_level);
        
        env->CallStaticVoidMethod( mActivityClass, jmethodID_onLevelUp, jlevel );
        
        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool AndroidNativeDevToDevPlugin::onCurrencyAccrual( const String & _currencyName, int _currencyAmount, int _accrualType )
    {
        JNIEnv * env = Mengine_JNI_GetEnv();
        
        const Char * currencyName_str = _currencyName.c_str();
        jstring jcurrencyName = env->NewStringUTF( currencyName_str );
        jint jcurrencyAmount = static_cast<jint>(_currencyAmount);
        jint jaccrualType = static_cast<jint>(_accrualType);

        env->CallStaticVoidMethod( mActivityClass, jmethodID_onCurrencyAccrual, jcurrencyName, jcurrencyAmount, jaccrualType );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool AndroidNativeDevToDevPlugin::onRealPayment( const String & _paymentId, float _inAppPrice, const String & _inAppName, const String & _inAppCurrencyISOCode )
    {
        JNIEnv * env = Mengine_JNI_GetEnv();

        const Char * paymentId_str = _paymentId.c_str();
        jstring jpaymentId = env->NewStringUTF( paymentId_str );
        jfloat jinAppPrice = static_cast<jfloat>(_inAppPrice);
        const Char * inAppName_str = _inAppName.c_str();
        jstring jinAppName = env->NewStringUTF( inAppName_str );
        const Char * inAppCurrencyISOCode_str = _inAppCurrencyISOCode.c_str();
        jstring jinAppCurrencyISOCode = env->NewStringUTF( inAppCurrencyISOCode_str );

        env->CallStaticVoidMethod( mActivityClass, jmethodID_onRealPayment, jpaymentId, jinAppPrice, jinAppName, jinAppCurrencyISOCode );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool AndroidNativeDevToDevPlugin::onInAppPurchase( const String & _purchaseId, const String & _purchaseType, int _purchaseAmount, int _purchasePrice, const String & _purchaseCurrency )
    {
        JNIEnv * env = Mengine_JNI_GetEnv();

        const Char * purchaseId_str = _purchaseId.c_str();
        jstring jpurchaseId = env->NewStringUTF( purchaseId_str );
        const Char * purchaseType_str = _purchaseType.c_str();
        jstring jpurchaseType = env->NewStringUTF( purchaseType_str );
        jint jpurchaseAmount = static_cast<jint>(_purchaseAmount);
        jint jpurchasePrice = static_cast<jint>(_purchasePrice);
        const Char * purchaseCurrency_str = _purchaseCurrency.c_str();
        jstring jpurchaseCurrency = env->NewStringUTF( purchaseCurrency_str );

        env->CallStaticVoidMethod( mActivityClass, jmethodID_onInAppPurchase, jpurchaseId, jpurchaseType, jpurchaseAmount, jpurchasePrice, jpurchaseCurrency );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool AndroidNativeDevToDevPlugin::onSimpleCustomEvent( const String & _eventName )
    {
        JNIEnv * env = Mengine_JNI_GetEnv();
        
        const Char * eventName_str = _eventName.c_str();
        jstring jeventName = env->NewStringUTF( eventName_str );
        
        env->CallStaticVoidMethod( mActivityClass, jmethodID_onSimpleCustomEvent, jeventName );
        
        return true;
    }
}  