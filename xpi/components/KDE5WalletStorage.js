const Ci = Components.interfaces;
const Cu = Components.utils;
const Cc = Components.classes;

Cu.import("resource://gre/modules/XPCOMUtils.jsm");
Cu.import("resource://gre/modules/Services.jsm");
Cu.import("resource://kde5wallet/modules/KDE5WalletLibrary.jsm");

function KDE5WalletStorage() {
    XPCOMUtils.defineLazyGetter(this, "_lib", function() new KDE5WalletLibrary());
}

KDE5WalletStorage.prototype = {
    classDescription: "KDE5 Wallet Login Manager Storage",
    contractID:       "@guillermo.molina/kde5wallet;1",
    classID:          Components.ID("{734ef55a-6ca9-11e5-abe4-cbcca39e461b}"),
    QueryInterface:   XPCOMUtils.generateQI([Ci.nsISupports, Ci.nsILoginManagerStorage]),

    /*
     * log
     *
     * Internal function for logging debug messages to the Error Console.
     */
    log: function(m) {
		if (!this._lib._debug)
			return;
        Services.console.logStringMessage("KDE5WalletStorage: " + m);
    },

    init: function () {
      this.initialize();
    },

		/*
     * initialize
     *
     * Initialize this storage component.
     */
    initialize: function () {
		this.log( "init() Start" );
		this._lib.init();
		this.checkStorageVersion()
    }, 

	checkStorageVersion: function() {
		this.log( "verifyStorageVersion() Start" );
		if( this._lib.getStorageVersion() != 2 )
			this.migrateFromVersion1ToVersion2();
	},

	migrateFromVersion1ToVersion2: function() {
		this.log( "migrateFromVersion1ToVersion2() Start" );
		let count = 0;
		let entries = this.getAllLogins(count);
        for (let i = 0; i < entries.length; i++) {
			this.removeLogin( entries[i] );
			this.addLogin( entries[i] );
		}
		this._lib.setStorageVersion(2);
	},

    initWithFile: function (inFile, outFile) {
		this.log( "initWithFile() Unimplemented function" );
    }, 

    addLogin: function (login) {
		this.log( "addLogin() Start" );
		let loginClone = login.clone();
		
		loginClone.QueryInterface(Ci.nsILoginMetaInfo);
		if( !loginClone.guid || login.guid == "" ) {
			let g = Cc['@mozilla.org/uuid-generator;1'].getService(Ci.nsIUUIDGenerator);
			loginClone.guid = g.generateUUID().toString();
			this.log( "addLogin() New guid:" + loginClone.guid );
		}
		
		let currentTime = Date.now();
		if(!loginClone.timeCreated)
		  loginClone.timeCreated = currentTime;
		if(!loginClone.timeLastUsed)
		  loginClone.timeLastUsed = currentTime;
		if(!loginClone.timePasswordChanged)
		  loginClone.timePasswordChanged = currentTime;
		if(!loginClone.timesUsed)
		  loginClone.timesUsed = 1;
		
		this._lib.addLogin( loginClone.username, loginClone.usernameField, loginClone.password, 
							loginClone.passwordField, loginClone.formSubmitURL, loginClone.httpRealm, 
							loginClone.hostname, loginClone.guid, loginClone.timeCreated, loginClone.timeLastUsed,
							loginClone.timePasswordChanged, loginClone.timesUsed );
   },
    
    removeLogin: function (login) {
		this.log( "removeLogin() Start" );
		this._lib.removeLogin( login.username, login.formSubmitURL, login.httpRealm, login.hostname );
    },

    modifyLogin: function (oldLogin, newLoginData) {
		this.log( "modifyLogin() Start" );
		let newLogin;
		let needsUpdate = false;
		newLogin = oldLogin.clone().QueryInterface(Ci.nsILoginMetaInfo);
		if (newLoginData instanceof Ci.nsILoginInfo) {
			newLogin.init(newLoginData.hostname,
				newLoginData.formSubmitURL, newLoginData.httpRealm,
				newLoginData.username, newLoginData.password,
				newLoginData.usernameField, newLoginData.passwordField);
				newLogin.QueryInterface(Ci.nsILoginMetaInfo);

			if (newLogin.username != oldLogin.username) {
				this.log("Updating username");
				needsUpdate = true;
			}
			if (newLogin.password != oldLogin.password) {
				this.log("Updating password");
				newLogin[timePasswordChanged] = Date.now();
				needsUpdate = true;
			}
		
		} else if (newLoginData instanceof Ci.nsIPropertyBag) {
			let propEnum = newLoginData.enumerator;
			while (propEnum.hasMoreElements()) {
				let prop = propEnum.getNext().QueryInterface(Ci.nsIProperty);
				switch (prop.name) {
					// nsILoginInfo properties...
					//
					case "hostname":
					case "username":
					case "password":
					case "formSubmitURL":
					case "usernameField":
					case "passwordField":
					case "httpRealm":
					// nsILoginMetaInfo properties...
					case "timeCreated":
					case "timeLastUsed":
					case "timePasswordChanged":
					case "timesUsed":
						needsUpdate = true;
						this.log("updating field: " + prop.name);
						newLogin[prop.name] = prop.value;
						break;

					case "timesUsedIncrement":
						needsUpdate = true;
						this.log("updating field: " + prop.name);
						newLogin.timesUsed += prop.value;
						break;

					case "guid":
						this.log("Guid is changing?!  Not supported");
						break;

					// Fail if caller requests setting an unknown property.
					default:
						throw "Unexpected propertybag item: " + prop.name;
				}
			}
	    } else {
			throw "newLoginData needs an expected interface!";
	    }
	    if (needsUpdate) {
//			this.removeLogin(oldLogin);
//			this.addLogin(newLogin);
		
				this._lib.modifyLogin( newLogin.username, newLogin.usernameField, newLogin.password, 
					newLogin.passwordField, newLogin.formSubmitURL, newLogin.httpRealm, 
					newLogin.hostname, newLogin.guid, newLogin.timeCreated, newLogin.timeLastUsed,
					newLogin.timePasswordChanged, newLogin.timesUsed );
	    }
	},

	_array2NsILoginInfo: function( outCount, entries ) {
 		this.log( "_array2NsILoginInfo() Start" );
		let logins = [];
        for (let i = 0; i < entries.length; i++) {
			let l = Cc['@mozilla.org/login-manager/loginInfo;1']
                    .createInstance(Ci.nsILoginInfo);
			if( entries[i].formSubmitURL != "" )
				l.formSubmitURL = entries[i].formSubmitURL;
			if( entries[i].httpRealm != "" )
				l.httpRealm = entries[i].httpRealm;
			if( entries[i].hostname != "" )
				l.hostname = entries[i].hostname;
			// Aliaksandr Stelmachonak:
			// assingning empty username as well, because
			// thunderbird expects empty string, not null
			l.username = entries[i].username;
			if( entries[i].password != "" )
				l.password = entries[i].password;
			if( entries[i].usernameField != "" )
				l.usernameField = entries[i].usernameField;
			if( entries[i].passwordField != "" )
				l.passwordField = entries[i].passwordField;
			l.QueryInterface(Ci.nsILoginMetaInfo);
			if( entries[i].guid != "" )
				l.guid = entries[i].guid;
			if( entries[i].timeCreated != 0 )
				l.timeCreated = entries[i].timeCreated;
			if( entries[i].timeLastUsed != 0 )
				l.timeLastUsed = entries[i].timeLastUsed;
			if( entries[i].timePasswordChanged != 0 )
				l.timePasswordChanged = entries[i].timePasswordChanged;
			if( entries[i].timesUsed != 0 )
				l.timesUsed = entries[i].timesUsed;
            logins.push(l);
        }
		this.log( "_array2NsILoginInfo() Count: " + logins.length );
		outCount.value = logins.length;
        return logins;
	},
	
    getAllLogins: function (outCount) {
		this.log( "getAllLogins() Start" );
        let entries = this._lib.getAllLogins();
		return this._array2NsILoginInfo( outCount, entries );
    },
    
    getAllEncryptedLogins: function (outCount) {
		this.log( "getAllEncryptedLogins() Start" );
        return this.getAllLogins(outCount);
    },
    
    searchLogins: function (outCount, matchData) {
		this.log( "searchLogins() Start" );
		let propEnum = matchData.enumerator;
		var guid;
		
        // The function _lib.findLogins() crashes if one parameter is "undefined" : previously, set the vars to null
        var hostname = null;
        var submitURL = null;
        var realm = null;

        while (propEnum.hasMoreElements()) {
			let prop = propEnum.getNext().QueryInterface(Ci.nsIProperty);
			switch (prop.name) {
				case "hostname":
                    hostname = prop.value;
                    break;
				case "formSubmitURL":
                    formSubmitURL = prop.value;
                    break;
				case "httpRealm":
                    realm = prop.value;
                    break;
                case "schemeUpgrades":
                    // Mozilla internal scheme upgrade, do nothing
                    break;
/*				case "username":
				case "password":
				case "usernameField":
				case "passwordField":*/
                    
				// nsILoginMetaInfo properties...
				case "guid":
					guid = prop.value;
					break;
/*				case "timeCreated":
				case "timeLastUsed":
				case "timePasswordChanged":
				case "timesUsed":
				case "timesUsedIncrement":*/
				// Fail if caller requests setting an unknown property.
				default:
					throw "Unexpected propertybag item: " + prop.name;
			}
		}
        let entries;
        if( guid )
            entries = this._lib.findLoginsWithGuid( guid );
        else
            entries = this._lib.findLogins( hostname, formSubmitURL, realm );
 		return this._array2NsILoginInfo( outCount, entries );
    },

    removeAllLogins: function() {
		this.log( "removeAllLogins() Start" ); 
		this._lib.removeAllLogins();
    }, 

    getAllDisabledHosts: function(count) {
		this.log( "getAllDisabledHosts() Start" );
        var result = this._lib.getAllDisabledHosts();
		count.value = result.length;
		return result;
    },
	
    getLoginSavingEnabled: function(hostname) {
		this.log( "getLoginSavingEnabled() Start" );
		return this._lib.getLoginSavingEnabled( hostname );
    },
    
    setLoginSavingEnabled: function(hostname, enabled) {
		this.log( "setLoginSavingEnabled() Start" );
		this._lib.setLoginSavingEnabled( hostname, enabled );
    },

    findLogins: function (outCount, hostname, submitURL, realm) {
		this.log( "findLogins() Start" );
        let entries = this._lib.findLogins( hostname, submitURL, realm );
		return this._array2NsILoginInfo( outCount, entries );
    },
	
    countLogins: function countLogins(hostname, submitURL, realm) {
		this.log( "countLogins() Start" );
		var count = this._lib.countLogins(hostname, submitURL, realm);
		this.log( "countLogins() counted = " + count );
		return count;
    },
    
    get uiBusy() {
	    return false;
    },
    
    get isLoggedIn() {
	    return true;
    },
};

const NSGetFactory = XPCOMUtils.generateNSGetFactory([KDE5WalletStorage]);

