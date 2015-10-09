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
 * The Original Code is KDE Wallet password manager storage.
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

#pragma once

#include <stdint.h>

#include <QtGlobal>

#ifndef KDEWALLET_EXPORT
# if defined(MAKE_KDEWALLET_LIB)
/* We are building this library */
#  define KDEWALLET_EXPORT Q_DECL_EXPORT
# else
/* We are using this library */
#  define KDEWALLET_EXPORT Q_DECL_IMPORT
# endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
/*LoginInfo*/
	char* hostname;
	char* username;
	char* usernameField;
	char* password;
	char* passwordField;
	char* formSubmitURL;
	char* httpRealm;
/*LoginMetaInfo*/
	char* guid;
	uint64_t timeCreated;
	uint64_t timeLastUsed;
	uint64_t timePasswordChanged;
	uint32_t timesUsed;
} LoginInfo;
	
KDEWALLET_EXPORT int32_t KDE5Wallet_Init();
KDEWALLET_EXPORT int32_t KDE5Wallet_Shutdown();
KDEWALLET_EXPORT void KDE5Wallet_SetWalletType( const char* _walletType );
KDEWALLET_EXPORT void KDE5Wallet_SetWalletFolder( const char* _walletFolder );
KDEWALLET_EXPORT int32_t KDE5Wallet_CountLogins( const char* aHostname, const char* aActionURL, const char* aHttpRealm, uint32_t* _retval );
KDEWALLET_EXPORT int32_t KDE5Wallet_GetAllLogins( uint32_t *aCount, LoginInfo **aLogins );
KDEWALLET_EXPORT int32_t KDE5Wallet_FindLogins( uint32_t *count, const char* aHostname, const char* aActionURL, const char* aHttpRealm, LoginInfo **logins );
KDEWALLET_EXPORT int32_t KDE5Wallet_FindLoginsWithGUID( uint32_t *count, const char* aGUID, LoginInfo **logins );
KDEWALLET_EXPORT int32_t KDE5Wallet_AddLogin( const char* aUsername, const char* aUsernameField,
		const char* aPassword, const char* aPasswordField,
		const char* aActionURL, const char* aHttpRealm,
		const char* aHostname, const char* aGUID,
		uint64_t aTimeCreated, uint64_t aTimeLastUsed, 
		uint64_t aTimePasswordChanged, uint32_t aTimesUsed );
KDEWALLET_EXPORT int32_t KDE5Wallet_ModifyLogin( const char* aUsername, const char* aUsernameField,
		const char* aPassword, const char* aPasswordField,
		const char* aActionURL, const char* aHttpRealm,
		const char* aHostname, const char* aGUID,
		uint64_t aTimeCreated, uint64_t aTimeLastUsed, 
		uint64_t aTimePasswordChanged, uint32_t aTimesUsed );
KDEWALLET_EXPORT int32_t KDE5Wallet_RemoveLogin( const char* aUsername, const char* aActionURL,
		const char* aHttpRealm, const char* aHostname );
KDEWALLET_EXPORT int32_t KDE5Wallet_RemoveAllLogins();
KDEWALLET_EXPORT int32_t KDE5Wallet_GetAllDisabledHosts( uint32_t *aCount, char*** aHostnames );
KDEWALLET_EXPORT int32_t KDE5Wallet_SetLoginSavingEnabled( const char* aHostname, const int32_t isEnabled );
KDEWALLET_EXPORT int32_t KDE5Wallet_GetLoginSavingEnabled( const char* aHostname, int32_t* _isEnabled );
KDEWALLET_EXPORT int32_t KDE5Wallet_SetStorageVersion( const uint32_t version );
KDEWALLET_EXPORT int32_t KDE5Wallet_GetStorageVersion( uint32_t* version );


#ifdef __cplusplus
}
#endif
