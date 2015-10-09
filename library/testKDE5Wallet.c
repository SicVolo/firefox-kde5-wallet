#include <dlfcn.h>
#include <stdio.h>
#include "KDE5Wallet.h"

int main() {
    printf( "Test KDE5Wallet\n" );
	
	printf( "*****************WARNING*******************\n" );
	printf( "This test is destructive!!!!!!!!!!!!!!!!!!!\n" );
	printf( "Backup your wallet first\n" );
	printf( "Continue (y/n) [n]? " );
	if( getchar() != 'y' ) {
		printf( "...bye\n" );
		return 1;
	}
	

    printf( "Opening libkde5wallet.so...\n" );
    void* handle = dlopen("./libkde5wallet.so", RTLD_LAZY);
    
    if (!handle) {
        printf( "Cannot open library: %s\n", dlerror() );
        return 1;
    }
/***************************************************************************************************/        
    printf( "Loading symbol KDE5Wallet_Init...\n" );
    typedef int (*libfunction_t)();

    dlerror();
    libfunction_t KDE5Wallet_Init = (libfunction_t) dlsym(handle, "KDE5Wallet_Init");
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        printf( "Cannot load symbol 'KDE5Wallet_Init': %s\n", dlsym_error );
        dlclose(handle);
        return 1;
    }
    
    printf( "Calling KDE5Wallet_Init...\n" );
    KDE5Wallet_Init();
/***************************************************************************************************/        
    printf( "Loading symbol KDE5Wallet_SetWalletType...\n" );
    typedef int (*libfunction_t)();

    dlerror();
    libfunction_t KDE5Wallet_SetWalletType = (libfunction_t) dlsym(handle, "KDE5Wallet_SetWalletType");
    dlsym_error = dlerror();
    if (dlsym_error) {
        printf( "Cannot load symbol 'KDE5Wallet_SetWalletType': %s\n", dlsym_error );
        dlclose(handle);
        return 1;
    }
    
    printf( "Calling KDE5Wallet_SetWalletType...\n" );
    KDE5Wallet_SetWalletType( "LocalWallet" );
/***************************************************************************************************/        
    printf( "Loading symbol KDE5Wallet_SetWalletFolder...\n" );
    typedef int (*libfunction_t)();

    dlerror();
    libfunction_t KDE5Wallet_SetWalletFolder = (libfunction_t) dlsym(handle, "KDE5Wallet_SetWalletFolder");
    dlsym_error = dlerror();
    if (dlsym_error) {
        printf( "Cannot load symbol 'KDE5Wallet_SetWalletFolder': %s\n", dlsym_error );
        dlclose(handle);
        return 1;
    }
    
    printf( "Calling KDE5Wallet_SetWalletFolder...\n" );
    KDE5Wallet_SetWalletFolder( "Firefox" );
/***************************************************************************************************/        
    printf( "Loading symbol KDE5Wallet_CountLogins...\n" );
    typedef int (*libfunction_t)();

    dlerror();
    libfunction_t KDE5Wallet_CountLogins = (libfunction_t) dlsym(handle, "KDE5Wallet_CountLogins");
    dlsym_error = dlerror();
    if (dlsym_error) {
        printf( "Cannot load symbol 'KDE5Wallet_CountLogins': %s\n", dlsym_error );
        dlclose(handle);
        return 1;
    }
    
    printf( "Calling KDE5Wallet_CountLogins...\n" );
	int count;
    KDE5Wallet_CountLogins( "", "", "", &count );
/***************************************************************************************************/        
    printf( "Loading symbol KDE5Wallet_GetAllLogins...\n" );
    typedef int (*libfunction_t)();

    dlerror();
    libfunction_t KDE5Wallet_GetAllLogins = (libfunction_t) dlsym(handle, "KDE5Wallet_GetAllLogins");
    dlsym_error = dlerror();
    if (dlsym_error) {
        printf( "Cannot load symbol 'KDE5Wallet_GetAllLogins': %s\n", dlsym_error );
        dlclose(handle);
        return 1;
    }
    
    printf( "Calling KDE5Wallet_GetAllLogins...\n" );
	LoginInfo *loginInfo;
    KDE5Wallet_GetAllLogins( &count, &loginInfo );
	int i;
	for( i = 0; i < count; i++ ) {
		printf( "Login[ %i ].hostname = %s\n", i, loginInfo[ i ].hostname );
		printf( "Login[ %i ].username = %s\n", i, loginInfo[ i ].username );
		printf( "Login[ %i ].usernameField = %s\n", i, loginInfo[ i ].usernameField );
		printf( "Login[ %i ].password = %s\n", i, loginInfo[ i ].password );
		printf( "Login[ %i ].passwordField = %s\n", i, loginInfo[ i ].passwordField );
		printf( "Login[ %i ].formSubmitURL = %s\n", i, loginInfo[ i ].formSubmitURL );
		printf( "Login[ %i ].httpRealm = %s\n", i, loginInfo[ i ].httpRealm );
		printf( "Login[ %i ].guid = %s\n", i, loginInfo[ i ].guid );
	}
/***************************************************************************************************/        
 /*   printf( "Loading symbol KDE5Wallet_RemoveAllLogins...\n" );
    typedef int (*libfunction_t)();

    dlerror();
    libfunction_t KDE5Wallet_RemoveAllLogins = (libfunction_t) dlsym(handle, "KDE5Wallet_RemoveAllLogins");
    dlsym_error = dlerror();
    if (dlsym_error) {
        printf( "Cannot load symbol 'KDE5Wallet_RemoveAllLogins': %s\n", dlsym_error );
        dlclose(handle);
        return 1;
    }
    
    printf( "Calling KDE5Wallet_RemoveAllLogins...\n" );
    KDE5Wallet_RemoveAllLogins();*/
/***************************************************************************************************/        
    printf( "Loading symbol KDE5Wallet_AddLogin...\n" );
    typedef int (*libfunction_t)();

    dlerror();
    libfunction_t KDE5Wallet_AddLogin = (libfunction_t) dlsym(handle, "KDE5Wallet_AddLogin");
    dlsym_error = dlerror();
    if (dlsym_error) {
        printf( "Cannot load symbol 'KDE5Wallet_AddLogin': %s\n", dlsym_error );
        dlclose(handle);
        return 1;
    }
    
    printf( "Calling KDE5Wallet_AddLogin...\n" );
    KDE5Wallet_AddLogin( "guillermo adrián", "user", "ñKdsfkijr", "password", "", 
						"http://www.guillermomolina.com.ar/", "http://www.guillermomolina.com.ar/",
						"{900f8667-269a-4425-b1c0-2f804619f316}" );
/***************************************************************************************************/        
    printf( "Loading symbol KDE5Wallet_RemoveLogin...\n" );
    typedef int (*libfunction_t)();

    dlerror();
    libfunction_t KDE5Wallet_RemoveLogin = (libfunction_t) dlsym(handle, "KDE5Wallet_RemoveLogin");
    dlsym_error = dlerror();
    if (dlsym_error) {
        printf( "Cannot load symbol 'KDE5Wallet_RemoveLogin': %s\n", dlsym_error );
        dlclose(handle);
        return 1;
    }
    
    printf( "Calling KDE5Wallet_RemoveLogin...\n" );
    KDE5Wallet_RemoveLogin( "guillermo adrián", "", "http://www.guillermomolina.com.ar/",
						"http://www.guillermomolina.com.ar/" );
/***************************************************************************************************/        
    printf( "Loading symbol KDE5Wallet_FindLoginsWithGUID...\n" );
    typedef int (*libfunction_t)();

    dlerror();
    libfunction_t KDE5Wallet_FindLoginsWithGUID = (libfunction_t) dlsym(handle, "KDE5Wallet_FindLoginsWithGUID");
    dlsym_error = dlerror();
    if (dlsym_error) {
        printf( "Cannot load symbol 'KDE5Wallet_FindLoginsWithGUID': %s\n", dlsym_error );
        dlclose(handle);
        return 1;
    }
    
    printf( "Calling KDE5Wallet_FindLoginsWithGUID...\n" );
    KDE5Wallet_FindLoginsWithGUID( &count, "{900f8667-269a-4425-b1c0-2f804619f316}", &loginInfo );
	for( i = 0; i < count; i++ ) {
		printf( "Login[ %i ].hostname = %s\n", i, loginInfo[ i ].hostname );
		printf( "Login[ %i ].username = %s\n", i, loginInfo[ i ].username );
		printf( "Login[ %i ].usernameField = %s\n", i, loginInfo[ i ].usernameField );
		printf( "Login[ %i ].password = %s\n", i, loginInfo[ i ].password );
		printf( "Login[ %i ].passwordField = %s\n", i, loginInfo[ i ].passwordField );
		printf( "Login[ %i ].formSubmitURL = %s\n", i, loginInfo[ i ].formSubmitURL );
		printf( "Login[ %i ].httpRealm = %s\n", i, loginInfo[ i ].httpRealm );
		printf( "Login[ %i ].guid = %s\n", i, loginInfo[ i ].guid );
	}
/***************************************************************************************************/        
    printf( "Loading symbol KDE5Wallet_SetLoginSavingEnabled...\n" );
    typedef int (*libfunction_t)();

    dlerror();
    libfunction_t KDE5Wallet_SetLoginSavingEnabled = (libfunction_t) dlsym(handle, "KDE5Wallet_SetLoginSavingEnabled");
    dlsym_error = dlerror();
    if (dlsym_error) {
        printf( "Cannot load symbol 'KDE5Wallet_SetLoginSavingEnabled': %s\n", dlsym_error );
        dlclose(handle);
        return 1;
    }
    
    printf( "Calling KDE5Wallet_SetLoginSavingEnabled...\n" );
    KDE5Wallet_SetLoginSavingEnabled( "http://www.guillermomolina.com.ar/", 0  );
/***************************************************************************************************/        
    printf( "Loading symbol KDE5Wallet_GetLoginSavingEnabled...\n" );
    typedef int (*libfunction_t)();

    dlerror();
    libfunction_t KDE5Wallet_GetLoginSavingEnabled = (libfunction_t) dlsym(handle, "KDE5Wallet_GetLoginSavingEnabled");
    dlsym_error = dlerror();
    if (dlsym_error) {
        printf( "Cannot load symbol 'KDE5Wallet_GetLoginSavingEnabled': %s\n", dlsym_error );
        dlclose(handle);
        return 1;
    }
    
    printf( "Calling KDE5Wallet_GetLoginSavingEnabled...\n" );
	int32_t isEnabled;
    KDE5Wallet_GetLoginSavingEnabled( "http://www.guillermomolina.com.ar/", &isEnabled );
	printf( "Savings for http://www.guillermomolina.com.ar/ is " );
	if( isEnabled )
		printf( "enabled\n" );
	else
		printf( "disabled\n" );
/***************************************************************************************************/        
    printf( "Loading symbol KDE5Wallet_GetAllDisabledHosts...\n" );
    typedef int (*libfunction_t)();

    dlerror();
    libfunction_t KDE5Wallet_GetAllDisabledHosts = (libfunction_t) dlsym(handle, "KDE5Wallet_GetAllDisabledHosts");
    dlsym_error = dlerror();
    if (dlsym_error) {
        printf( "Cannot load symbol 'KDE5Wallet_GetAllDisabledHosts': %s\n", dlsym_error );
        dlclose(handle);
        return 1;
    }
    
    printf( "Calling KDE5Wallet_GetAllDisabledHosts...\n" );
	char **hostnames;
    KDE5Wallet_GetAllDisabledHosts( &count, &hostnames );
	for( i = 0; i < count; i++ ) 
		printf( "DisabledHost[ %i ] = %s\n", i, hostnames[ i ] );
/***************************************************************************************************/        
    printf( "Loading symbol KDE5Wallet_Shutdown...\n" );
    typedef int (*libfunction_t)();

    dlerror();
    libfunction_t KDE5Wallet_Shutdown = (libfunction_t) dlsym(handle, "KDE5Wallet_Shutdown");
    dlsym_error = dlerror();
    if (dlsym_error) {
        printf( "Cannot load symbol 'KDE5Wallet_Shutdown': %s\n", dlsym_error );
        dlclose(handle);
        return 1;
    }
    
    printf( "Calling KDE5Wallet_Shutdown...\n" );
    KDE5Wallet_Shutdown();
/***************************************************************************************************/        
    
    printf( "Closing library...\n" );
    dlclose(handle);
	return 0;
}