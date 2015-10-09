function initializeOverlay() {
	var pref = Components.classes["@mozilla.org/preferences-service;1"].
				getService(Components.interfaces.nsIPrefBranch);

	try {
		folder = pref.getCharPref("extensions.kde5wallet.folder");
	} catch(e) {}

	if (folder == "Unknown" ) {
		var appInfo = Components.classes["@mozilla.org/xre/app-info;1"]
                        .getService(Components.interfaces.nsIXULAppInfo);
		pref.setCharPref("extensions.kde5wallet.folder", appInfo.name );
	}
}

window.addEventListener("load", initializeOverlay, false);