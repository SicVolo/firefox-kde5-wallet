/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is KDE5 Wallet password manager storage.
 *
 * The Initial Developer of the Original Code is
 * Guillermo Molina <guillermoadrianmolina@hotmail.com>
 * Portions created by the Initial Developer are Copyright (C) 2009
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *  See CONTRIBUTORS file
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#include <QGuiApplication>
#include <QDebug>

#include <KWallet>

#include "KDE5Wallet.h"

typedef QMap< QString, QString > WalletEntry;
typedef QMap< QString, WalletEntry > WalletEntryMap;
KWallet::Wallet *wallet = NULL; // A pointer to KWallet - it doesn't like be declared over and over inside a function.
QGuiApplication *app; // A pointer to the QGuiApplication app - can only declare it once or we'll core dump!

const QString kSaveDisabledHostsMapName = QStringLiteral("SaveDisabledHosts");
const QString kStorageVersionMapName = QStringLiteral("StorageVersion");
const QString kStorageVersion = QStringLiteral("version");

const QString kHostnameAttr = QStringLiteral("hostname");
const QString kFormSubmitURLAttr = QStringLiteral("formSubmitURL");
const QString kHttpRealmAttr = QStringLiteral("httpRealm");
const QString kUsernameFieldAttr = QStringLiteral("usernameField");
const QString kPasswordFieldAttr = QStringLiteral("passwordField");
const QString kUsernameAttr = QStringLiteral("username");
const QString kPasswordAttr = QStringLiteral("password");
const QString kGuidAttr = QStringLiteral("guid");
const QString kTimeCreated = QStringLiteral("timeCreated");
const QString kTimeLastUsed = QStringLiteral("timeLastUsed");
const QString kTimePasswordChanged = QStringLiteral("timePasswordChanged");
const QString kTimesUsed = QStringLiteral("timesUsed");
QString walletType = QStringLiteral("LocalWallet");
QString walletFolder = QStringLiteral("Firefox");

uint32_t loginInfoBufferCount = 0;
LoginInfo* loginInfoBuffer = 0;

uint32_t disabledHostsBufferCount = 0;
char** disabledHostsBuffer = 0;
int fakeargc = 0;
char *fakeargv[] = { NULL };

void KDE5Wallet_SetWalletType( const char* _walletType  ) {
	qDebug() << "Start";
	walletType = QString::fromUtf8(_walletType);
	qDebug() << "walletType = " << walletType;
}

void KDE5Wallet_SetWalletFolder( const char* _walletFolder  ) {
	qDebug() << "Start";
	walletFolder = QString::fromUtf8(_walletFolder);
	qDebug() << "walletFolder = " << walletFolder;
}

bool checkWallet() {
	qDebug() << "Start";
        if (KWallet::Wallet::isEnabled()) {
            if( !wallet || !wallet->isOpen() ) {
                    if( walletType == QStringLiteral("LocalWallet") )
                            wallet =  KWallet::Wallet::openWallet(KWallet::Wallet::LocalWallet(), 0, KWallet::Wallet::Synchronous );
                    else if( walletType == QStringLiteral("NetworkWallet") )
                            wallet =  KWallet::Wallet::openWallet(KWallet::Wallet::NetworkWallet(), 0, KWallet::Wallet::Synchronous );
                    if( !wallet ) {
                            qCritical() << "Could not open wallet type" << walletType;
                            return false;
                    }
                    qDebug() << "Opened wallet type " << walletType;
            }
            if( !wallet->hasFolder( walletFolder ) ) {
                    if( !wallet->createFolder( walletFolder ) ) {
                            qCritical() << "Could not create wallet folder" << walletFolder;
                            return false;
                    }
                    qDebug() << "Created wallet folder " << walletFolder;
            }
            if( !wallet->setFolder( walletFolder ) ) {
                    qCritical() << "Could not select wallet folder" << walletFolder;
                    return false;
            }
            qDebug() << "Selected wallet folder " << walletFolder;
            return true;
        } else {
            qDebug() << "KWallet is not enabled.";
            return false;
        }
}

QString generateWalletKey( const char* aHostname,
			const char* aActionURL,
			const char* aHttpRealm,
			const char* aUsername ) {
	return QStringLiteral( "%1,%2,%3,%4" )
		.arg( QString::fromUtf8(aUsername) )
		.arg( QString::fromUtf8(aActionURL) )
		.arg( QString::fromUtf8(aHttpRealm) )
		.arg( QString::fromUtf8(aHostname) );
}

QString generateQueryWalletKey( const char* aHostname,
			const char* aActionURL,
			const char* aHttpRealm,
			const char* aUsername ) {
  
	QString hostname = QString::fromUtf8(aHostname);
	if( hostname.isEmpty() )
	  hostname = QStringLiteral("*");
	QString actionURL = QString::fromUtf8(aActionURL);
	if( actionURL.isEmpty() )
	  actionURL = QStringLiteral("*");
	QString httpRealm = QString::fromUtf8( aHttpRealm );
	if( httpRealm.isEmpty() )
	  httpRealm = QStringLiteral("*");
	QString username = QString::fromUtf8( aUsername );
	if( username.isEmpty() )
	  username = QStringLiteral("*");
	const QString key = QStringLiteral( "%1,%2,%3,%4" ).arg( username ).arg( actionURL ).arg( httpRealm ).arg( hostname );
	return key;
}

int32_t KDE5Wallet_Init() {
	qDebug() << "Start";

        // KWindowSystem requires a functioning QGuiApplication or it will segfault
        if (!app)
            app = new QGuiApplication(fakeargc, fakeargv);

        if (!app) {
            qCritical() << "Could not create KApplication";
            return false;
        }

	return true;
}

void freeLoginInfoBuffer() {
	qDebug() << "Start";
	if( !loginInfoBuffer && !loginInfoBufferCount )
		return;
	if( loginInfoBuffer && loginInfoBufferCount ) {
		for( int i = 0; (unsigned) i < loginInfoBufferCount; i++ ) {
			delete [] loginInfoBuffer[ i ].hostname;
			delete [] loginInfoBuffer[ i ].username;
			delete [] loginInfoBuffer[ i ].usernameField;
			delete [] loginInfoBuffer[ i ].password;
			delete [] loginInfoBuffer[ i ].passwordField;
			delete [] loginInfoBuffer[ i ].formSubmitURL;
			delete [] loginInfoBuffer[ i ].httpRealm;
			delete [] loginInfoBuffer[ i ].guid;
		}
		delete [] loginInfoBuffer;
		loginInfoBufferCount = 0;
	}
}

bool allocLoginInfoBuffer( uint32_t count ) {
	freeLoginInfoBuffer();
	loginInfoBufferCount = count;
	loginInfoBuffer = new LoginInfo[ count ];
	if( !loginInfoBuffer ) {
		qCritical() << "Could not alloc memory buffer";
		return false;
	}
	return true;
}

void freeDisabledHostsBuffer() {
	qDebug() << "Start";
	if( !disabledHostsBuffer && !disabledHostsBufferCount )
		return;
	if( disabledHostsBuffer && disabledHostsBufferCount ) {
		for( int i = 0; (unsigned) i < disabledHostsBufferCount; i++ )
			delete [] disabledHostsBuffer[ i ];
		delete [] disabledHostsBuffer;
		disabledHostsBufferCount = 0;
	}
}

bool allocDisabledHostsBuffer( uint32_t count ) {
	qDebug() << "Start";
	freeDisabledHostsBuffer();
	disabledHostsBufferCount = count;
	disabledHostsBuffer = new char*[ count ];
	if( !disabledHostsBuffer ) {
		qCritical() << "Could not alloc memory buffer";
		return false;
	}
	return true;
}

int32_t KDE5Wallet_Shutdown() {
	qDebug() << "Shutdown";
	freeLoginInfoBuffer();
	if (!app) {
		qCritical() << "QGuiApplication not created";
		return false;
	}
	delete app;
	qDebug() << "QGuiApplication destroyed";
	return true;
}

int32_t KDE5Wallet_AddLogin( const char* aUsername, const char* aUsernameField,
		const char* aPassword, const char* aPasswordField,
		const char* aActionURL, const char* aHttpRealm,
		const char* aHostname, const char* aGUID,
		uint64_t aTimeCreated, uint64_t aTimeLastUsed, 
		uint64_t aTimePasswordChanged, uint32_t aTimesUsed ) {
	qDebug() << "Start";
  
	if( !checkWallet() )
		return false;

	WalletEntry entry;
	entry[ kUsernameAttr ] = QString::fromUtf8(aUsername);
	entry[ kPasswordAttr ] = QString::fromUtf8(aPassword);
	entry[ kUsernameFieldAttr ] = QString::fromUtf8(aUsernameField);
	entry[ kPasswordFieldAttr ] = QString::fromUtf8(aPasswordField);
	entry[ kFormSubmitURLAttr ] = QString::fromUtf8(aActionURL);
	entry[ kHttpRealmAttr ] = QString::fromUtf8(aHttpRealm);
	entry[ kHostnameAttr ] = QString::fromUtf8(aHostname);
	entry[ kGuidAttr ] = QString::fromUtf8(aGUID);
	entry[ kTimeCreated ] = QString::number(aTimeCreated);
	entry[ kTimeLastUsed ] = QString::number(aTimeLastUsed);
	entry[ kTimePasswordChanged ] = QString::number(aTimePasswordChanged);
	entry[ kTimesUsed ] = QString::number(aTimesUsed);

	qDebug() << "Add username = " << entry[ kUsernameAttr ];
	
	//TODO: Verify the guid is not already inside de DB !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	const QString key = generateWalletKey( aHostname, aActionURL, aHttpRealm, aUsername );
	if( wallet->writeMap( key, entry ) ) {
		qCritical() << "Can not save map information";
		return false;
	}
	return true;
}

int32_t KDE5Wallet_ModifyLogin( const char* aUsername, const char* aUsernameField,
		const char* aPassword, const char* aPasswordField,
		const char* aActionURL, const char* aHttpRealm,
		const char* aHostname, const char* aGUID,
		uint64_t aTimeCreated, uint64_t aTimeLastUsed, 
		uint64_t aTimePasswordChanged, uint32_t aTimesUsed ) {
	qDebug() << "Start";
  
	if( !checkWallet() )
		return false;

	qDebug() << "Hostname: " << aHostname;
	const QString key = generateWalletKey( aHostname, aActionURL, aHttpRealm, aUsername );
	WalletEntryMap entryMap;
	if( wallet->readMapList( key, entryMap ) != 0 ) {
		qCritical() << "Error reading entries";
		return false;
	}
	if( entryMap.count() != 1 ) {
		qDebug() << "Could not find entry " << key;
		return false;
	}
	
	const WalletEntryMap::iterator iterator = entryMap.begin();
	WalletEntry entry = iterator.value();
	
	QString buffer = QString::fromUtf8(aGUID);
	if(entry[ kGuidAttr ] != buffer ) {
		qDebug() << "Can not modify GUID " << entry[ kGuidAttr ] << " with " << buffer;
		return false;
	}

	buffer = QString::fromUtf8(aUsername);
	if(!buffer.isEmpty())
		entry[ kUsernameAttr ] = buffer;
	
	buffer = QString::fromUtf8(aPassword);
	if(!buffer.isEmpty())
		entry[ kPasswordAttr ] = buffer;
	
	buffer = QString::fromUtf8(aUsernameField);
	if(!buffer.isEmpty())
		entry[ kUsernameFieldAttr ] = buffer;
	
	buffer = QString::fromUtf8(aPasswordField);
	if(!buffer.isEmpty())
		entry[ kPasswordFieldAttr ] = buffer;
	
	buffer = QString::fromUtf8(aActionURL);
	if(!buffer.isEmpty())
		entry[ kFormSubmitURLAttr ] = buffer;
	
	buffer = QString::fromUtf8(aHttpRealm);
	if(!buffer.isEmpty())
		entry[ kHttpRealmAttr ] = buffer;
	
	buffer = QString::fromUtf8(aHostname);
	if(!buffer.isEmpty())
		entry[ kHostnameAttr ] = buffer;

	if(aTimeCreated != 0)
		entry[ kTimeCreated ] = QString::number(aTimeCreated);
	
	if(aTimeLastUsed != 0)
		entry[ kTimeLastUsed ] = QString::number(aTimeLastUsed);
	
	if(aTimePasswordChanged != 0)
		entry[ kTimePasswordChanged ] = QString::number(aTimePasswordChanged);
	
	if(aTimesUsed != 0)
		entry[ kTimesUsed ] = QString::number(aTimesUsed);
		
	if( wallet->writeMap( key, entry ) ) {
		qCritical() << "Can not save map information";
		return false;
	}
	return true;
}


int32_t KDE5Wallet_RemoveLogin( const char* aUsername, const char* aActionURL, 
							   const char* aHttpRealm, const char* aHostname ) {
	qDebug() << "Start";
	
	if( !checkWallet() )
		return false;

	qDebug() << "Remove username = " << QString::fromUtf8(aUsername);
	
	const QString key = generateWalletKey( aHostname, aActionURL, aHttpRealm, aUsername );
	if( wallet->removeEntry( key ) ) {
		qCritical() << "Can not remove map information";
		return false;
	}
	return true;
}

int32_t KDE5Wallet_RemoveAllLogins() {
	qDebug() << "Start";
	
	if( !checkWallet() )
		return false;
	
	const QString key = generateQueryWalletKey( "*","*","*","*" );

	WalletEntryMap entryMap;
	if( wallet->readMapList( key, entryMap ) != 0 ) {
		qCritical() << "Error reading passwords";
		return false;
	}
	qDebug() <<  "RemoveAllLogins() Found " << entryMap.count() << " maps";
	if( entryMap.count() == 0 ) 
		return true;

	WalletEntryMap::iterator iterator = entryMap.begin();
	while( iterator != entryMap.end() ) {
		if( wallet->removeEntry( iterator.key() ) ) {
			qCritical() << "Can not remove map information";
			return false;
		}		
		++iterator;
	}	
	return true;
}

bool findLogins( const char* aHostname,
				const char* aActionURL,
				const char* aHttpRealm,
				WalletEntryMap& entryMap) {
	qDebug() << "Start";

	if( !checkWallet() )
		return false;
	
	const QString key = generateQueryWalletKey( aHostname, aActionURL, aHttpRealm, "*" );

	if( wallet->readMapList( key, entryMap ) != 0 ) 
		return false;
	qDebug() <<  "Found " << entryMap.count() << " maps";
	return true;
}

bool walletEntryToLoginInfo( WalletEntryMap& entryMap,
							 uint32_t *count,
							 LoginInfo **logins ) {
	qDebug() << "Start";
	*count = 0;
	if( !allocLoginInfoBuffer( entryMap.count() ) ) {
		qCritical() << "Could not alloc buffer for logins";
		return false;
	}

	int i = 0;
	WalletEntryMap::iterator iterator = entryMap.begin();
	while (iterator != entryMap.end()) {
 		WalletEntry entry = iterator.value();
		if( entry.contains( kHostnameAttr ) ) {
			QByteArray hostname = entry.value( kHostnameAttr ).toUtf8();
			loginInfoBuffer[ i ].hostname = new char[ hostname.size() + 1 ];
			strcpy( loginInfoBuffer[ i ].hostname, hostname.data() );
		}
		else {
			loginInfoBuffer[ i ].hostname = new char[ 1 ];
			loginInfoBuffer[ i ].hostname[ 0 ] = 0;
		}
		if( entry.contains( kUsernameAttr ) ) {
			QByteArray username = entry.value( kUsernameAttr ).toUtf8();
			loginInfoBuffer[ i ].username = new char[ username.size() + 1 ];
			strcpy( loginInfoBuffer[ i ].username, username.data() );
		}
		else {
			loginInfoBuffer[ i ].username = new char[ 1 ];
			loginInfoBuffer[ i ].username[ 0 ] = 0;
		}
		if( entry.contains( kUsernameFieldAttr ) ) {
			QByteArray usernameField = entry.value( kUsernameFieldAttr ).toUtf8();
			loginInfoBuffer[ i ].usernameField = new char[ usernameField.size() + 1 ];
			strcpy( loginInfoBuffer[ i ].usernameField, usernameField.data() );
		}
		else {
			loginInfoBuffer[ i ].usernameField = new char[ 1 ];
			loginInfoBuffer[ i ].usernameField[ 0 ] = 0;
		}
		if( entry.contains( kPasswordAttr ) ) {
			QByteArray password = entry.value( kPasswordAttr ).toUtf8();
			loginInfoBuffer[ i ].password = new char[ password.size() + 1 ];
			strcpy( loginInfoBuffer[ i ].password, password.data() );
		}
		else {
			loginInfoBuffer[ i ].password = new char[ 1 ];
			loginInfoBuffer[ i ].password[ 0 ] = 0;
		}
		if( entry.contains( kPasswordFieldAttr ) ) {
			QByteArray passwordField = entry.value( kPasswordFieldAttr ).toUtf8();
			loginInfoBuffer[ i ].passwordField = new char[ passwordField.size() + 1 ];
			strcpy( loginInfoBuffer[ i ].passwordField, passwordField.data() );
		}
		else {
			loginInfoBuffer[ i ].passwordField = new char[ 1 ];
			loginInfoBuffer[ i ].passwordField[ 0 ] = 0;
		}
		if( entry.contains( kFormSubmitURLAttr ) ) {
			QByteArray formSubmitURL = entry.value( kFormSubmitURLAttr ).toUtf8();
			loginInfoBuffer[ i ].formSubmitURL = new char[ formSubmitURL.size() + 1 ];
			strcpy( loginInfoBuffer[ i ].formSubmitURL, formSubmitURL.data() );
		}
		else {
			loginInfoBuffer[ i ].formSubmitURL = new char[ 1 ];
			loginInfoBuffer[ i ].formSubmitURL[ 0 ] = 0;
		}
		if( entry.contains( kHttpRealmAttr ) ) {
			QByteArray httpRealm = entry.value( kHttpRealmAttr ).toUtf8();
			loginInfoBuffer[ i ].httpRealm = new char[ httpRealm.size() + 1 ];
			strcpy( loginInfoBuffer[ i ].httpRealm, httpRealm.data() );
		}
		else {
			loginInfoBuffer[ i ].httpRealm = new char[ 1 ];
			loginInfoBuffer[ i ].httpRealm[ 0 ] = 0;
		}
		if( entry.contains( kGuidAttr ) ) {
			QByteArray guid = entry.value( kGuidAttr ).toUtf8();
			loginInfoBuffer[ i ].guid = new char[ guid.size() + 1 ];
			strcpy( loginInfoBuffer[ i ].guid, guid.data() );
		}
		else {
			loginInfoBuffer[ i ].guid = new char[ 1 ];
			loginInfoBuffer[ i ].guid[ 0 ] = 0;
		}
		if( entry.contains( kTimeCreated ) ) {
			uint64_t timeCreated = entry.value( kTimeCreated ).toULongLong();
			loginInfoBuffer[ i ].timeCreated = timeCreated;
		}
		else
			loginInfoBuffer[ i ].timeCreated = 0;
		if( entry.contains( kTimeLastUsed ) ) {
			uint64_t timeLastUsed = entry.value( kTimeLastUsed ).toULongLong();
			loginInfoBuffer[ i ].timeLastUsed = timeLastUsed;
		}
		else
			loginInfoBuffer[ i ].timeLastUsed = 0;
		if( entry.contains( kTimePasswordChanged ) ) {
			uint64_t timePasswordChanged = entry.value( kTimePasswordChanged ).toULongLong();
			loginInfoBuffer[ i ].timePasswordChanged = timePasswordChanged;
		}
		else
			loginInfoBuffer[ i ].timePasswordChanged = 0;
		if( entry.contains( kTimesUsed ) ) {
			uint64_t timesUsed = entry.value( kTimesUsed ).toULong();
			loginInfoBuffer[ i ].timesUsed = timesUsed;
		}
		else
			loginInfoBuffer[ i ].timesUsed = 0;
		qDebug() <<  "Converted key: " << loginInfoBuffer[ i ].hostname << ", username: " << loginInfoBuffer[ i ].username;
		i++;
		iterator++;
	}	
	*count = loginInfoBufferCount;
	*logins = loginInfoBuffer;
	return true;
}

int32_t KDE5Wallet_FindLogins( uint32_t *count,
			const char* aHostname,
			const char* aActionURL,
			const char* aHttpRealm,
			LoginInfo **logins ) {
	qDebug() << "Start";
	*count = 0;

	WalletEntryMap entryMap;
	if( !findLogins( aHostname, aActionURL, aHttpRealm, entryMap ) )
		return false;
	if( entryMap.count() == 0 ) 
		return true;

	return walletEntryToLoginInfo( entryMap, count, logins );
}

int32_t KDE5Wallet_GetAllLogins( uint32_t *aCount, LoginInfo **aLogins ) {
	qDebug() << "Start";
	return KDE5Wallet_FindLogins( aCount, "*", "*", "*", aLogins);
}

int32_t KDE5Wallet_FindLoginsWithGUID( uint32_t *count, 
								const char* aGUID,
								LoginInfo **logins ) {
	qDebug() << "Start";
	*count = 0;

	WalletEntryMap entryMap;
	if( !findLogins( "*", "*", "*", entryMap ) )
		return false;
	
	qDebug() << "Found " << entryMap.count() << " entries";
	
	if( entryMap.count() == 0 ) 
		return true;

	WalletEntryMap::iterator iterator = entryMap.begin();
	while (iterator != entryMap.end()) {
 		WalletEntry entry = iterator.value();
		if( !entry.contains( kGuidAttr ) || entry.value( kGuidAttr ) != QString::fromUtf8(aGUID) )
			iterator = entryMap.erase(iterator);
		else
			++iterator;
	}
	return walletEntryToLoginInfo( entryMap, count, logins );
}

int32_t KDE5Wallet_GetAllDisabledHosts( uint32_t *aCount, char*** aHostnames ) {
	qDebug() << "Start";
	*aCount = 0;

	if( !checkWallet() )
		return false;

	WalletEntry saveDisabledHostMap;
	wallet->readMap( kSaveDisabledHostsMapName, saveDisabledHostMap );
	
	qDebug() << "Found " << saveDisabledHostMap.count() << " entries";

	if( saveDisabledHostMap.count() == 0 )		
		return true;

	if( !allocDisabledHostsBuffer( saveDisabledHostMap.count() ) ) {
		qCritical() << "Could not alloc buffer for logins";
		return false;
	}

	WalletEntry::iterator iterator = saveDisabledHostMap.begin();
	int32_t i = 0;
	while (iterator != saveDisabledHostMap.end()) {
		disabledHostsBuffer[ i ] = new char[ iterator.key().size() + 1 ];
		strcpy( disabledHostsBuffer[ i ], iterator.key().toUtf8().data() );
		qDebug() <<  "Added host: " << disabledHostsBuffer[ i ];
		i++;
		++iterator;
	}
	*aCount = i;
	*aHostnames = disabledHostsBuffer;
	return true;
}

int32_t KDE5Wallet_GetLoginSavingEnabled( const char* aHostname, int32_t* _isEnabled ) {
	qDebug() << "Start";

	if( !checkWallet() )
	  return false;

	QString hostname = QString::fromUtf8( aHostname );
	
	WalletEntry saveDisabledHostMap;

	wallet->readMap( kSaveDisabledHostsMapName, saveDisabledHostMap );

	*_isEnabled = true;
	if( saveDisabledHostMap.contains(  hostname ) )
		*_isEnabled = false;
	
	if( *_isEnabled )
		qDebug() << "saving for " << hostname << " is enabled";
	else
		qDebug() << "saving for " << hostname << " is disabled";

	return true;
}

int32_t KDE5Wallet_SetLoginSavingEnabled( const char* aHostname, const int32_t isEnabled ) {
	qDebug() << "Start";

	if( !checkWallet() )
	  return false;

	WalletEntry saveDisabledHostMap;
	const QString hostname = QString::fromUtf8( aHostname );

	wallet->readMap( kSaveDisabledHostsMapName, saveDisabledHostMap );
	if( isEnabled ) { //Remove form disabled list, if it is there
		if( saveDisabledHostMap.contains( hostname ) )
			if( saveDisabledHostMap.remove( hostname ) != 1 ) {
				qCritical() << "Can not remove save map information";
				return false;
			}
	}
	else 
		saveDisabledHostMap[ hostname ] = QStringLiteral("TRUE");
	if( wallet->writeMap( kSaveDisabledHostsMapName, saveDisabledHostMap ) ) {
		qCritical() << "Can not remove save map information";
		return false;
	}
	return true;
}

int32_t KDE5Wallet_CountLogins( const char* aHostname, const char* aActionURL, const char* aHttpRealm, uint32_t* _retval ) {
	qDebug() << "Start ( " << QString::fromUtf8(aHostname) << ", " << QString::fromUtf8(aActionURL)
	<< ", " << QString::fromUtf8(aHttpRealm) << " )";
	*_retval = 0;
	
	if( !checkWallet() )
	  return false;
	
	const QString key = generateQueryWalletKey( aHostname, aActionURL, aHttpRealm, "*" );
	qDebug() << " key = " << key;

	QMap< QString, WalletEntry > entryMap;
	if( wallet->readMapList( key, entryMap ) != 0 ) 
		return true;

	*_retval = entryMap.count();
	qDebug() << " Found " << *_retval << " logins";
	return true;
}

int32_t KDE5Wallet_GetStorageVersion( uint32_t* version ) {
	qDebug() << "Start";

	if( !checkWallet() )
	  return false;

	WalletEntry storageVersionMap;
	
	wallet->readMap( kStorageVersionMapName, storageVersionMap );

	*version = 1;
	if( storageVersionMap.contains(  kStorageVersion ) )
		*version = storageVersionMap.value( kStorageVersion ).toULong();
	
	qDebug() << "Storage is version " << *version;

	return true;
}

int32_t KDE5Wallet_SetStorageVersion( const uint32_t version ) {
	qDebug() << "Start";

	if( !checkWallet() )
	  return false;

	WalletEntry entry;
	entry[ kStorageVersion ] = QString::number(version);

	qDebug() << "Set storage version " << entry[ kStorageVersion ];
	
	if( wallet->writeMap( kStorageVersionMapName, entry ) ) {
		qCritical() << "Can not save map information";
		return false;
	}

	return true;
}
