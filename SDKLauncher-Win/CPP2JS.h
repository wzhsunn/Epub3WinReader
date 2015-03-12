//
//  Copyright (c) 2014 Readium Foundation and/or its licensees. All rights reserved.
//  
//  Redistribution and use in source and binary forms, with or without modification,
//  are permitted provided that the following conditions are met :
//  1. Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation and / or
//  other materials provided with the distribution.
//  3. Neither the name of the organization nor the names of its contributors may
//  be used to endorse or promote products derived from this software without
//  specific prior written permission.
//  
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
//  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE)
//  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
//  THE POSSIBILITY OF SUCH DAMAGE.#include "stdafx.h"

#pragma once

#define _USE_READIUM 1
#include <atlbase.h>
#include "explorer.h"
#include <MsHTML.h>
#include <afxwin.h>
#include <string>

#if _USE_READIUM
#include <ePub3\initialization.h>
#include <ePub3\archive.h>
#include <ePub3\xml\io.h>
#include <ePub3\filter_manager_impl.h>
#include <ePub3\media-overlays_smil_model.h>
using namespace ePub3;
//#define EPUB_PATH "F:/Work/GitHub/readium-sdk/TestData/alice3.epub"
//#define EPUB_PATH "F:/Work/GitHub/readium-sdk/TestData/dante-hell.epub"
//#define EPUB_PATH "F:/Work/GitHub/readium-sdk/TestData/moby-dick-preview-collection.epub"
#include <ePub3\utilities\byte_stream.h>
#include <ePub3\container.h>
#include <ePub3\nav_table.h>

#include "json.h"

#else
struct PackagePtr{};
#endif

typedef std::string QString;
typedef json::Object QJsonObject;
typedef json::Array QJsonArray;

//std::string toStdString(const QString& l)
//{
//	return ePub3::string((LPCTSTR)l).stl_str();
//}
//inline json::Value  xxx::operator= (json::Value& r, const QString& l)
//{
//	r = (LPCTSTR)l;
//	return r;
///}


#if _USE_READIUM
/*
void serveFile(QString uri, QMap<QString, QString> header, PackagePtr pckg)
{
    //InputStream is = pckg.getInputStream(uri);
    
    //Response res = null;

    //int contentLength = pckg->getArchiveInfoSize(uri);

    if (contentLength == 0) {
    res = new Response(Response.Status.NOT_FOUND,
    NanoHTTPD.MIME_PLAINTEXT, "Error 404, file not found.");
    }

    if (res == null) {
    // Get MIME type from file name extension, if possible
    String mime = null;
    int dot = uri.lastIndexOf('.');
    if (dot >= 0) {
    mime = MIME_TYPES.get(uri.substring(dot + 1).toLowerCase());
    }
    if (mime == null) {
    mime = NanoHTTPD.MIME_DEFAULT_BINARY;
    }

    // Calculate etag
    //String etag = Integer.toHexString((pckg.getUniqueID()
    //	+ pckg.getModificationDate() + "" + pckg.getBasePath())
    //	.hashCode());

    long startFrom = 0;
    long endAt = -1;
    String range = header.get("range");
    //			Log.d(TAG, "range: "+range);
    if (range != null) {
    if (range.startsWith("bytes=")) {
    range = range.substring("bytes=".length());
    int minus = range.indexOf('-');
    try {
    if (minus > 0) {
    startFrom = Long.parseLong(range.substring(0, minus));
    endAt = Long.parseLong(range.substring(minus + 1));
    }
    }
    catch (NumberFormatException ignored) {
    Log.e(TAG, "NumberFormatException: " + ignored.getMessage());
    }
    }
    }

    // Change return code and add Content-Range header when skipping is requested
    if (range != null && startFrom >= 0) {
    if (startFrom >= contentLength) {
    res = new Response(Response.Status.RANGE_NOT_SATISFIABLE,
    NanoHTTPD.MIME_PLAINTEXT, "");
    res.addHeader("Content-Range", "bytes 0-0/" + contentLength);
    res.addHeader("ETag", etag);
    }
    else {
    if (endAt < 0) {
    endAt = contentLength - 1;
    }
    long newLen = endAt - startFrom + 1;
    if (newLen < 0) {
    newLen = 0;
    }

    InputStream is = pckg.getInputStream(uri);
    try {
    is.skip(startFrom);
    }
    catch (IOException e) {
    Log.e(TAG, "InputStream.skip(" + startFrom + ") failed: " + e.getMessage(), e);
    }

    res = new Response(Response.Status.PARTIAL_CONTENT, mime, is);
    res.addHeader("Content-Range", "bytes " + startFrom + "-"
    + endAt + "/" + contentLength);
    }
    }
    else {
    if (etag.equals(header.get("if-none-match"))) {
    res = new Response(Response.Status.NOT_MODIFIED, mime, "");
    }
    else {
    InputStream is = pckg.getInputStream(uri);
    res = new Response(Response.Status.OK, mime, is);
    }
    }
    res.addHeader("ETag", etag);
    }
    // Announce that the file server accepts partial content requests
    res.addHeader("Accept-Ranges", "bytes");

    return res;
    
}
*/


class DNToc;
class DNSpineItem;
class DNSpine;
class DNPackage;

struct DNePubSdkApi
{
    ePub3::ContainerPtr			  _container;
    ePub3::Container::PackageList _packages;
    ePub3::PackagePtr			_currentPackage;

    ~DNePubSdkApi(){
        dealloc();
    }

    void initialize()
    {
        ePub3::InitializeSdk();
        ePub3::PopulateFilterManager();
    }

    void openFile(QString file)
    {
        dealloc();

        _container = ePub3::Container::OpenContainer( ePub3::string((LPCSTR)file.c_str()) );

        if (!_container)
            return;

        _packages = _container->Packages();

        if (_packages.empty())
            return;

        if (_packages.size()) // there is only one package, but may be in the future?
        {
            _currentPackage = _packages[0];
        }

    }

    ePub3::Container::PackageList getPackages()
    {
        return _packages;
    }

    void dealloc()
    {
        _packages.clear();
        _currentPackage.reset();;
        _container.reset();
    }
};
#endif

class ViewerSettings
{
public:
    bool mIsSyntheticSpread;
    int mFontSize;
    int mColumnGap;

    ViewerSettings(bool isSyntheticSpread, int fontSize, int columnGap)
    {
        mIsSyntheticSpread = isSyntheticSpread;
        mFontSize = fontSize;
        mColumnGap = columnGap;
    }

    bool isSyntheticSpread()
    {
        return mIsSyntheticSpread;
    }

    int getFontSize()
    {
        return mFontSize;
    }

    int getColumnGap()
    {
        return mColumnGap;
    }

    QJsonObject toJSON() {
        QJsonObject json;
        json["isSyntheticSpread"] = mIsSyntheticSpread;
        json["fontSize"] = mFontSize;
        json["columnGap"] = mColumnGap;
        return json;
    }

    QString toString() {
        return "";
        //return QString(QString("ViewerSettings [isSyntheticSpread=") + mIsSyntheticSpread + QString(", fontSize=") + mFontSize + ", columnGap=" + mColumnGap + "]";
    }

};
struct OpenPageRequest
{
    QString idref;
    int spineItemPageIndex;
    QString elementCfi;
    QString contentRefUrl;
    QString sourceFileHref;
    QString elementId;

public:
    OpenPageRequest(QString idref_, int spineItemPageIndex_, QString elementCfi_, QString contentRefUrl_, QString sourceFileHref_, QString elementId_) :
        idref(idref_), spineItemPageIndex(spineItemPageIndex_), elementCfi(elementCfi_), contentRefUrl(contentRefUrl_), sourceFileHref(sourceFileHref_), elementId(elementId_){}

    static OpenPageRequest fromIdref(QString idref) {
        return OpenPageRequest(idref, 0, "", "", "", "");
    }

    static OpenPageRequest fromIdrefAndIndex(QString idref, int spineItemPageIndex) {
        return OpenPageRequest(idref, spineItemPageIndex, "", "", "", "");
    }

    static OpenPageRequest fromIdrefAndCfi(QString idref, QString elementCfi) {
        return OpenPageRequest(idref, 0, elementCfi, "", "", "");
    }

    static OpenPageRequest fromContentUrl(QString contentRefUrl, QString sourceFileHref) {
        return OpenPageRequest("", 0, "", contentRefUrl, sourceFileHref, "");
    }

    static OpenPageRequest fromElementId(QString idref, QString elementId){
        return OpenPageRequest(idref, 0, "", "", "", elementId);
    }

    /*OpenPageRequest(QString idref, int spineItemPageIndex,
    QString elementCfi, QString contentRefUrl, QString sourceFileHref, QString elementId) {
    this->idref = idref;
    this->spineItemPageIndex = spineItemPageIndex;
    this->elementCfi = elementCfi;
    this->contentRefUrl = contentRefUrl;
    this->sourceFileHref = sourceFileHref;
    this->elementId = elementId;
    }*/

    QJsonObject toJSON()
    {
        QJsonObject json;
        json["idref"] = idref;
        json["spineItemPageIndex"] = spineItemPageIndex;
        json["elementCfi"] = elementCfi;
        json["contentRefUrl"] = contentRefUrl;
        json["sourceFileHref"] = sourceFileHref;
        json["elementId"] = elementId;
        return json;
    }

#if 0
    static OpenPageRequest fromJSON(QString data)
    {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject json = jsonDoc.object();

        int spineItemPageIndex = (json.find("spineItemPageIndex") != json.end()) ? json["spineItemPageIndex"].toInt() : 0;

        QString sCFI = (json.find("elementCfi") != json.end()) ? json["elementCfi"].toString() : ((json.find("contentCFI") != json.end()) ? (json["contentCFI"].toString()) : "");

        QString scontentRefUrl = (json.find("contentRefUrl") != json.end()) ? json["contentRefUrl"].toString() : "";
        QString ssourceFileHref = (json.find("sourceFileHref") != json.end()) ? (json["sourceFileHref"].toString()) : ((json.find("elementId") != json.end()) ? json["elementId"].toString() : "");

        return OpenPageRequest(json["idref"].toString(), spineItemPageIndex,
            // get elementCfi and then contentCFI (from bookmarkData) if it was empty
            sCFI,
            scontentRefUrl,
            ssourceFileHref, "");
    }
#endif
};

struct TOCEntry
{
    TOCEntry(){}
    TOCEntry(std::string name_, std::string uri_) :sTOCName(name_), sURI(uri_){}

    std::string sTOCName;
    std::string sURI;
    std::vector<TOCEntry> arrChildren;
};

class ReadiumJSApi
{
    PackagePtr pkg;	
public:
    bool getByteResp(std::string sURI, BYTE** bytes, ULONGLONG* pSize);
    void initReadiumSDK();
    std::list<std::pair<std::string, std::string> > getSpineList();
    void digInto(const NavigationList& lst, TOCEntry& rOut);
    void ReadiumJSApi::getTOCList(TOCEntry &rOut);
    void on_actionOpen_ePub3(std::string path);
    std::string getBookTitle();
    std::string getBasePath();
    void loadJSOnReady(QString jScript);
    void loadJS(QString jScript);

public:
    CExplorer *WebBrowser;
    ReadiumJSApi(CExplorer*pWebBrowser_);
    ReadiumJSApi();
    ~ReadiumJSApi()	{}

    void bookmarkCurrentPage();
    void openPageLeft();
    void openPageRight();
    QJsonArray getJSON(shared_ptr<const ePub3::SMILData::Sequence> seqBody);
    QJsonObject getPackageJSON(PackagePtr pckg);
    void openBook(PackagePtr pckg, ViewerSettings viewerSettings, OpenPageRequest openPageRequestData);
    void updateSettings(ViewerSettings viewerSettings);
    void openContentUrl(QString href, QString baseUrl);
    void openSpineItemPage(QString idRef, int page);
    void openSpineItemElementCfi(QString idRef, QString elementCfi);
    void nextMediaOverlay();
    void previousMediaOverlay();
    bool bMediaOverlayToggled;	// TODO: this state should be in sync
    void toggleMediaOverlay();
    void turnMediaOverlayOff();
};
