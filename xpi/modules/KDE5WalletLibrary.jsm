var EXPORTED_SYMBOLS = [ "KDE5WalletLibrary" ];

Components.utils.import("resource://gre/modules/XPCOMUtils.jsm");
Components.utils.import("resource://gre/modules/Services.jsm");
Components.utils.import("resource://gre/modules/ctypes.jsm");

function KDE5WalletLibrary() {
    this.log("KDE5WalletLibrary() Start");
    this._prefBranch = Services.prefs.getBranch("extensions.kde5wallet.");
    let kwl = this;
    const ioService = Components.classes["@mozilla.org/network/io-service;1"].getService(Components.interfaces.nsIIOService);
    var uri = ioService.newURI( "resource://libkde5wallet", null, null);
    if( uri instanceof Components.interfaces.nsIFileURL ) {
		kwl.kde5wallet = ctypes.open(uri.file.path);
// int32_t KDE5Wallet_Init();
		kwl._Init = this.kde5wallet.declare("KDE5Wallet_Init", ctypes.default_abi, ctypes.int32_t);
// int32_t KDE5Wallet_Shutdown();
		kwl._Shutdown = this.kde5wallet.declare("KDE5Wallet_Shutdown", ctypes.default_abi, ctypes.int32_t);
// void KDE5Wallet_SetWalletType( const char* _walletType );
		kwl._SetWalletType = this.kde5wallet.declare("KDE5Wallet_SetWalletType", ctypes.default_abi, ctypes.void_t, ctypes.char.ptr);
// void KDE5Wallet_SetWalletFolder( const char* _walletFolder );
		kwl._SetWalletFolder = this.kde5wallet.declare("KDE5Wallet_SetWalletFolder", ctypes.default_abi, ctypes.void_t, ctypes.char.ptr);
/*typedef struct {
	char* hostname;
	char* username;
	char* usernameField;
	char* password;
	char* passwordField;
	char* formSubmitURL;
	char* httpRealm;
	char* guid;

	char* guid;
	uint64_t timeCreated;
	uint64_t timeLastUsed;
	uint64_t timePasswordChanged;
	uint32_t timesUsed;
} LoginInfo;*/
		kwl._LoginInfo = ctypes.StructType( "LoginInfo", [
			{ hostname			: ctypes.char.ptr	},
			{ username			: ctypes.char.ptr	},
			{ usernameField		: ctypes.char.ptr	},
			{ password			: ctypes.char.ptr	},
			{ passwordField		: ctypes.char.ptr	},
			{ formSubmitURL		: ctypes.char.ptr	},
			{ httpRealm			: ctypes.char.ptr	},
			{ guid				: ctypes.char.ptr	},
			{ timeCreated			: ctypes.uint64_t	},
			{ timeLastUsed			: ctypes.uint64_t	},
			{ timePasswordChanged		: ctypes.uint64_t	},
			{ timesUsed			: ctypes.uint32_t	}
		]);
//int32_t KDE5Wallet_GetAllLogins( uint32_t *aCount, LoginInfo **aLogins )
		kwl._GetAllLogins = this.kde5wallet.declare("KDE5Wallet_GetAllLogins", ctypes.default_abi, ctypes.int32_t, ctypes.uint32_t.ptr, kwl._LoginInfo.ptr.ptr );
// int32_t KDE5Wallet_FindLogins( uint32_t *count, const char* aHostname, const char* aActionURL, const char* aHttpRealm, LoginInfo **logins )
		kwl._FindLogins = this.kde5wallet.declare("KDE5Wallet_FindLogins", ctypes.default_abi, ctypes.int32_t, ctypes.uint32_t.ptr, ctypes.char.ptr, ctypes.char.ptr, ctypes.char.ptr, kwl._LoginInfo.ptr.ptr );
// int32_t KDE5Wallet_FindLoginsWithGUID( uint32_t *count, const char* guid, LoginInfo **logins )
		kwl._FindLoginsWithGUID = this.kde5wallet.declare("KDE5Wallet_FindLoginsWithGUID", ctypes.default_abi, ctypes.int32_t, ctypes.uint32_t.ptr, ctypes.char.ptr, kwl._LoginInfo.ptr.ptr );
// int32_t KDE5Wallet_CountLogins( const char* aHostname, const char* aActionURL, const char* aHttpRealm, uint32_t* _retval );
		kwl._CountLogins = this.kde5wallet.declare("KDE5Wallet_CountLogins", ctypes.default_abi, ctypes.int32_t, ctypes.char.ptr, ctypes.char.ptr, ctypes.char.ptr, ctypes.uint32_t.ptr );
//int32_t KDE5Wallet_AddLogin( const char* aUsername, const char* aUsernameField,
//							const char* aPassword, const char* aPasswordField,
//							const char* aActionURL, const char* aHttpRealm,
//							const char* aHostname, const char* aGUID,
//							uint64_t aTimeCreated, uint64_t aTimeLastUsed, 
//							uint64_t aTimePasswordChanged, uint32_t aTimesUsed );
		kwl._AddLogin = this.kde5wallet.declare("KDE5Wallet_AddLogin", ctypes.default_abi, ctypes.int32_t, ctypes.char.ptr, ctypes.char.ptr, ctypes.char.ptr,ctypes.char.ptr, ctypes.char.ptr, ctypes.char.ptr, ctypes.char.ptr, ctypes.char.ptr, ctypes.uint64_t, ctypes.uint64_t,
		ctypes.uint64_t, ctypes.uint32_t);
//int32_t KDE5Wallet_ModifyLogin( const char* aUsername, const char* aUsernameField,
//							const char* aPassword, const char* aPasswordField,
//							const char* aActionURL, const char* aHttpRealm,
//							const char* aHostname, const char* aGUID,
//							uint64_t aTimeCreated, uint64_t aTimeLastUsed, 
//							uint64_t aTimePasswordChanged, uint32_t aTimesUsed );
		kwl._ModifyLogin = this.kde5wallet.declare("KDE5Wallet_ModifyLogin", ctypes.default_abi, ctypes.int32_t, ctypes.char.ptr, ctypes.char.ptr, ctypes.char.ptr,ctypes.char.ptr, ctypes.char.ptr, ctypes.char.ptr, ctypes.char.ptr, ctypes.char.ptr, ctypes.uint64_t, ctypes.uint64_t,
		ctypes.uint64_t, ctypes.uint32_t);
//int32_t KDE5Wallet_RemoveLogin( const char* aUsername, const char* aActionURL, 
//							   const char* aHttpRealm, const char* aHostname )
		kwl._RemoveLogin = this.kde5wallet.declare("KDE5Wallet_RemoveLogin", ctypes.default_abi, ctypes.int32_t, ctypes.char.ptr, ctypes.char.ptr, ctypes.char.ptr, ctypes.char.ptr );
//int32_t KDE5Wallet_RemoveAllLogins();
		kwl._RemoveAllLogins = this.kde5wallet.declare("KDE5Wallet_RemoveAllLogins", ctypes.default_abi, ctypes.int32_t );
//int32_t KDE5Wallet_GetLoginSavingEnabled( const char* aHostname, int32_t* _isEnabled )
		kwl._GetLoginSavingEnabled = this.kde5wallet.declare("KDE5Wallet_GetLoginSavingEnabled", ctypes.default_abi, ctypes.int32_t, ctypes.char.ptr, ctypes.uint32_t.ptr );
//int32_t KDE5Wallet_SetLoginSavingEnabled( const char* aHostname, const int32_t isEnabled )
		kwl._SetLoginSavingEnabled = this.kde5wallet.declare("KDE5Wallet_SetLoginSavingEnabled", ctypes.default_abi, ctypes.int32_t, ctypes.char.ptr, ctypes.uint32_t );
//int32_t KDE5Wallet_GetAllDisabledHosts( uint32_t *aCount, char*** aHostnames )
		kwl._GetAllDisabledHosts = this.kde5wallet.declare("KDE5Wallet_GetAllDisabledHosts", ctypes.default_abi, ctypes.int32_t, ctypes.uint32_t.ptr, ctypes.char.ptr.ptr.ptr );
//int32_t KDE5Wallet_GetStorageVersion( uint32_t* version )
		kwl._GetStorageVersion = this.kde5wallet.declare("KDE5Wallet_GetStorageVersion", ctypes.default_abi, ctypes.int32_t, ctypes.uint32_t.ptr );
//int32_t KDE5Wallet_SetStorageVersion( const uint32_t version );
		kwl._SetStorageVersion = this.kde5wallet.declare("KDE5Wallet_SetStorageVersion", ctypes.default_abi, ctypes.int32_t, ctypes.uint32_t );
    }
    this._signonPrefBranch = Services.prefs.getBranch("signon.");
    this._debugObserver = {
        QueryInterface: XPCOMUtils.generateQI([Components.interfaces.nsIObserver,
                                               Components.interfaces.nsISupportsWeakReference]),
        observe: function(subject, topic, data) {
            kwl._debug = kwl._signonPrefBranch.getBoolPref("debug");
            kwl.log("_debugObserver debug preference updated: " + kwl._debug);
        }
    };
    this._signonPrefBranch.QueryInterface(Components.interfaces.nsIPrefBranch);
    this._signonPrefBranch.addObserver("debug", this._debugObserver, false);
    this._debug = this._signonPrefBranch.getBoolPref("debug");
        
    this._walletTypeObserver = {
        QueryInterface: XPCOMUtils.generateQI([Components.interfaces.nsIObserver,
                                               Components.interfaces.nsISupportsWeakReference]),
        observe: function(subject, topic, data) {
            kwl._walletType = kwl._prefBranch.getCharPref("wallet");
            kwl.log("_walletTypeObserver Wallet preference updated: " + kwl._walletType);
	    kwl._setWalletType();
        }
    };
    this._prefBranch.QueryInterface(Components.interfaces.nsIPrefBranch);
    this._prefBranch.addObserver("wallet", this._walletTypeObserver, false);
    this._walletType = this._prefBranch.getCharPref("wallet");
    this.log("KDE5WalletLibrary() wallet = " + this._walletType );
    
    this._walletFolderObserver = {
        QueryInterface: XPCOMUtils.generateQI([Components.interfaces.nsIObserver,
                                               Components.interfaces.nsISupportsWeakReference]),
        observe: function(subject, topic, data) {
            kwl._walletFolder = kwl._prefBranch.getCharPref("folder");
            kwl.log("_walletFolderObserver Folder preference updated: " + kwl._walletFolder);
	    kwl._setWalletFolder();
        }
    };
    this._prefBranch.QueryInterface(Components.interfaces.nsIPrefBranch);
    this._prefBranch.addObserver("folder", this._walletFolderObserver, false);
    this._walletFolder = this._prefBranch.getCharPref("folder");
    this.log("KDE5WalletLibrary() folder = " + this._walletFolder );
}

KDE5WalletLibrary.reload = function() {
	Services.console.logStringMessage("KDE5WalletLibrary: Reloading KDE5WalletLibrary module");
	let l = Components.classes['@mozilla.org/moz/jssubscript-loader;1'].getService(Components.interfaces.mozIJSSubScriptLoader);
	l.loadSubScript("resource://kde5wallet/modules/KDE5WalletLibrary.jsm");
};

KDE5WalletLibrary.prototype = {
		log: function(m) {
		if (this._debug)
			Services.console.logStringMessage("KDE5WalletLibrary: " + m);
    },

    init: function() {
		this.log( "init() Start" );
		this._setWalletType();
		this._setWalletFolder();
		this._Init();
    },

    _setWalletType: function() {
		this.log( "_setWalletType() Start" );
		this._SetWalletType( this._walletType );
    },

    _setWalletFolder: function() {
		this.log( "_setWalletFolder() Start" );
		this._SetWalletFolder( this._walletFolder );
    },
    
    shutdown: function() {
		this.log( "shutdown() Start" );
		this._Shutdown();
    },
    
    countLogins: function(aHostname, aActionURL, aHttpRealm) {
		this.log( "countLogins( " + aHostname + ", " + aActionURL + ", " + aHttpRealm + " ) Start" );
		let count = new ctypes.uint32_t; 
		this._CountLogins( aHostname, aActionURL, aHttpRealm, count.address() );
		this.log( "countLogins() logins counted = " + count.value );
		return count.value;
    },
    
	_loginInfo2Array: function( count, loginInfo ) {
		this.log( "_loginInfo2Array() Start loginInfo: " + loginInfo.toString() );
		var loginArray = [];
		for(var i = 0; i < count; i++ ) {
			var r = {
				hostname			: loginInfo.contents[i].hostname.readString(),
				username			: loginInfo.contents[i].username.readString(),
				usernameField		: loginInfo.contents[i].usernameField.readString(),
				password			: loginInfo.contents[i].password.readString(),
				passwordField		: loginInfo.contents[i].passwordField.readString(),
				formSubmitURL		: loginInfo.contents[i].formSubmitURL.readString(),
				httpRealm			: loginInfo.contents[i].httpRealm.readString(),
				guid				: loginInfo.contents[i].guid.readString(),
				timeCreated		: loginInfo.contents[i].timeCreated, 
				timeLastUsed		: loginInfo.contents[i].timeLastUsed, 
				timePasswordChanged		: loginInfo.contents[i].timePasswordChanged, 
				timesUsed		: loginInfo.contents[i].timesUsed 
			};
			loginArray.push( r );
			this.log( "_loginInfo2Array() Adding login for: " + r.hostname + ", username: " + r.username );
		}
		this.log( "_loginInfo2Array() Count: " + loginArray.length );
		return loginArray;
	},
    
    getAllLogins: function() {
		this.log( "getAllLogins() Start" );
		var count = new ctypes.uint32_t; 
		var loginInfo = new this._LoginInfo.ptr;
		this._GetAllLogins( count.address(), loginInfo.address() );
		loginInfoAsArray = ctypes.cast( loginInfo, this._LoginInfo.array( count.value ).ptr );
		return this._loginInfo2Array( count.value, loginInfoAsArray );
    },
    
    findLogins: function( aHostname, aActionURL, aHttpRealm) {
		this.log( "findLogins( " + aHostname + ", " + aActionURL + ", " + aHttpRealm + " ) Start" );
		let count = new ctypes.uint32_t; 
		let loginInfo = new this._LoginInfo.ptr;
		this._FindLogins( count.address(), aHostname, aActionURL, aHttpRealm, loginInfo.address() );
		loginInfoAsArray = ctypes.cast( loginInfo, this._LoginInfo.array( count.value ).ptr );
		return this._loginInfo2Array( count.value, loginInfoAsArray );
    },
    
    findLoginsWithGuid: function( aGuid ) {
		this.log( "findLoginsWithGuid( " + aGuid + " ) Start" );
		let count = new ctypes.uint32_t; 
		let loginInfo = new this._LoginInfo.ptr;
		this._FindLoginsWithGUID( count.address(), aGuid, loginInfo.address() );
		loginInfoAsArray = ctypes.cast( loginInfo, this._LoginInfo.array( count.value ).ptr );
		return this._loginInfo2Array( count.value, loginInfoAsArray );
    },
    
    removeLogin: function(aUsername, aActionURL, aHttpRealm, aHostname) {
		this.log( "removeLogin( " + aUsername + ", " + aHostname + ", " + aActionURL + ", " + aHttpRealm + " ) Start" );
		this._RemoveLogin(aUsername, aActionURL, aHttpRealm, aHostname);
    },
    
    removeAllLogins: function() {
		this.log( "removeAllLogins() Start" );
		return this._RemoveAllLogins();
    },
    
    addLogin: function( aUsername, aUsernameField, aPassword, aPasswordField,
						aActionURL, aHttpRealm, aHostname, aGUID,
		 aTimeCreated,  aTimeLastUsed, aTimePasswordChanged, aTimesUsed ) {
		this.log( "addLogin( " + aUsername + ", " + aHostname + ", " + aGUID + " ) Start" );
		return this._AddLogin( aUsername, aUsernameField, aPassword, aPasswordField,
						aActionURL, aHttpRealm, aHostname, aGUID, aTimeCreated,  aTimeLastUsed, aTimePasswordChanged, aTimesUsed );
    },
    
    modifyLogin: function( aUsername, aUsernameField, aPassword, aPasswordField,
						aActionURL, aHttpRealm, aHostname, aGUID,
		 aTimeCreated,  aTimeLastUsed, aTimePasswordChanged, aTimesUsed ) {
		this.log( "modifyLogin( " + aUsername + ", " + aHostname + ", " + aGUID + " ) Start" );
		return this._ModifyLogin( aUsername, aUsernameField, aPassword, aPasswordField,
						aActionURL, aHttpRealm, aHostname, aGUID, aTimeCreated,  aTimeLastUsed, aTimePasswordChanged, aTimesUsed );
    },
    
    getLoginSavingEnabled: function(aHostname) {
		this.log( "getLoginSavingEnabled( " + aHostname + " ) Start" );
		let isEnabled = new ctypes.uint32_t; 
		this._GetLoginSavingEnabled( aHostname, isEnabled.address() );
		this.log( "countLogins() logins counted = " + isEnabled.value != 0 );
		return isEnabled.value != 0;
    },
    
    setLoginSavingEnabled: function(aHostname, isEnabled) {
		this.log( "setLoginSavingEnabled( " + aHostname + "," + isEnabled + " ) Start" );
		this._SetLoginSavingEnabled( aHostname, isEnabled );
    },
    
    getAllDisabledHosts: function() {
		this.log( "getAllDisabledHosts() Start" );
		let count = new ctypes.uint32_t; 
		let disabledHosts = new ctypes.char.ptr.ptr;
		this._GetAllDisabledHosts( count.address(), disabledHosts.address() );
		disabledHostsAsArray = ctypes.cast( disabledHosts, ctypes.char.ptr.array( count.value ).ptr );
		var disabledHostsArray = [];
		for(var i = 0; i < count.value; i++ ) {
			disabledHostsArray.push( disabledHostsAsArray.contents[i].readString() );
			this.log( "getAllDisabledHosts() Adding host: " + disabledHostsAsArray.contents[i].readString() );
		}
		this.log( "getAllDisabledHosts() Count: " + disabledHostsArray.length );
		return disabledHostsArray;
    },
    
    getStorageVersion: function() {
		this.log( "getStorageVersion() Start" );
		let version = new ctypes.uint32_t; 
		this._GetStorageVersion( version.address() );
		this.log( "getStorageVersion() version = " + version.value );
		return version.value;
    },
    
    setStorageVersion: function(version) {
		this.log( "setStorageVersion( " + version + " ) Start" );
		this._SetStorageVersion( version );
    }
};