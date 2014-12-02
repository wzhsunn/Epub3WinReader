#include "stdafx.h"
#include "CPP2JS.h"

PackagePtr pkg;	// TODO: move it into ReadiumJSApi class!!!
//CCriticalSection g_cs;
void ReadiumJSApi::digInto(const NavigationList& list, TOCEntry& rOut)
{
	std::string sBaseUrl = getBasePath();	// fast fix
	int iTrimLen = sBaseUrl.length();

	for (NavigationList::const_iterator v = list.begin(); v != list.end(); ++v)
	{
		NavigationPoint* pt = dynamic_cast<NavigationPoint*>(v->get());
		std::string sTitle = pt->Title().c_str();
		std::string sPath = pt->AbsolutePath(pkg).c_str();
		if (0 == sPath.find(sBaseUrl))
		{
			sPath = sPath.substr(iTrimLen, sPath.length());
		}
		rOut.arrChildren.push_back(TOCEntry(sTitle, sPath));
		
		const NavigationList& list = pt->Children();
		digInto(list, (rOut.arrChildren.back()) );
	}
}
void ReadiumJSApi::getTOCList(TOCEntry &rOut)
{
	std::list<std::pair<std::string, std::string> > lst;
	if (!pkg)
		return;

	rOut = TOCEntry("TOC", "");

	shared_ptr<NavigationTable> toc = pkg->TableOfContents();
	const NavigationList& list = toc->Children();

	digInto(list, rOut);
}
std::list<std::pair<std::string, std::string> > ReadiumJSApi::getSpineList()
{
//	CSingleLock lock(&g_cs, TRUE);

	std::list<std::pair<std::string, std::string> > ret;
	if (!pkg)
		return ret;
	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	size_t idx = 0;
	shared_ptr<SpineItem>   pSpineItem = pkg->SpineItemAt(idx++);
	//listTOC->clear();
	//listTOC->addItem(new QListWidgetItem());
	while (pSpineItem != 0)
	{
		QJsonObject curItem;
		shared_ptr<ManifestItem>    manifestItem = pSpineItem->ManifestItem();
		//qDebug() << pSpineItem->Title().c_str();
		//pSpineItem->Title();	// TOC title
		if (manifestItem)
		{
			//qDebug() << QString(manifestItem->BaseHref().c_str());
			ret.push_back(std::make_pair(manifestItem->BaseHref().c_str(), manifestItem->BaseHref().c_str()));
			//listTOC->addItem(QString(manifestItem->BaseHref().c_str()));
			//curItem["href"] = QString(manifestItem->BaseHref().c_str());	//[dict setObject : _href forKey : @"href"];
		}
		//else
		//curItem["href"] = QString("");


		//curItem["idref"] = QString(pSpineItem->Idref().c_str());	//[dict setObject : _idref forKey : @"idref"];
		pSpineItem = pkg->SpineItemAt(idx++);
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return ret;
	// TOC TOC TOC
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//shared_ptr<NavigationTable> toc = pkg->TableOfContents();
	//const NavigationList& list = toc->Children();
	////qDebug() << toc->SourceHref().c_str();
	//for (NavigationList::const_iterator v = list.begin(); v != list.end(); ++v)
	//{
	//	//qDebug() << (*v)->Title().c_str();
	//}
}
std::string ReadiumJSApi::getBasePath()
{
	if (pkg)
	{
		return pkg->BasePath().c_str();
	}
	return "";
}
std::string ReadiumJSApi::getBookTitle()
{
	if (pkg)
	{
		pkg->BasePath();
		return pkg->Title().c_str();
	}
	return "";
}

void ReadiumJSApi::on_actionOpen_ePub3(std::string path)	//QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Files (*.epub)"));
{
//	CSingleLock lock(&g_cs, TRUE);
	
	try{
		////qDebug() << fileName;
		DWORD beginTimeMs = GetTickCount();
		ContainerPtr container = Container::OpenContainer(path);
		DWORD containerOpened = GetTickCount();
		if (container)
		{
			pkg = container->DefaultPackage();
			if (pkg == 0)
			{
				auto pkgs = container->Packages();
				if (pkgs.size() <= 0)
				{
					//QMessageBox messageBox;
					//messageBox.critical(0, "Error", "ReadiumSDK: No packages found !");
					//messageBox.setFixedSize(500, 200);
					//messageBox.exec();
					return;
				}
				pkg = pkgs[0];
			}
			//int contentLength = container->getArchiveInfoSize("titlepage.xhtml");

			OpenPageRequest req = OpenPageRequest::fromContentUrl("", "chapter_001.xhtml");//TODO:
			ViewerSettings set(true, 100, 20);
			
			DWORD openBookStartedMs = GetTickCount();
			openBook(pkg, set, req);
			DWORD EndTimeTimeMs = GetTickCount();
			
			//char buf[1024];wsprintf((LPWSTR)buf, L"TotalTime:%d ms\n Container::OpenContainer:%d  ms\nopenBook:%d  ms\n", (EndTimeTimeMs - beginTimeMs), (openBookStartedMs - beginTimeMs), (EndTimeTimeMs - containerOpened));
			//AfxMessageBox((LPCTSTR)buf);
			
			//BYTE* bytes = 0;
			//ULONGLONG pSize = 0;
			//getByteResp("/chapter_001.xhtml", &bytes, &pSize);
			//delete[]bytes;
			//getByteResp("/chapter_002.xhtml", &bytes, &pSize);
			//delete[]bytes;
			//getByteResp("/chapter_003.xhtml", &bytes, &pSize);

			//openContentUrl("chapter_001.xhtml", "");
			
			//ReadiumJsApi.openContentUrl((*list.begin())->Title().c_str(), "");
			//unique_ptr<ePub3::ByteStream> byteStream = pkg->ReadStreamForItemAtPath("titlepage.xhtml");

			//if (pkg)
			//{
				//CollectionPtr previewCollection = pkg->PreviewCollection();
				//unique_ptr<ePub3::ByteStream> byteStream1 = previewCollection->ReadStreamForItemAtPath("titlepage.xhtml");
			//}
		}


	}
	//catch (ePub3::epub_spec_error err)
	//{

	//}
	catch (...)
	{
		//qDebug() << "Exception!!!";
		throw;
	}

	//openContentUrl("chapter_001.xhtml", "");
	
}

bool ReadiumJSApi::getByteResp(std::string sURI, BYTE** bytes, ULONGLONG* pSize)
{
//	CSingleLock lock(&g_cs, TRUE);
	//QByteArray byteArr;
	//sURI = sURI.mid(1);
	//qDebug() << "Getting URI:" << sURI;
		
	unique_ptr<ByteStream> stream = pkg->ReadStreamForRelativePath(sURI.substr(1));
	if (stream)
	{
		ByteStream::size_type bytesAvailable = stream->BytesAvailable();
		if (bytesAvailable > 0)
		{
			char* buf = new char[bytesAvailable * 2];
			//memset(buf, 0, bytesAvailable);

			stream->ReadBytes(buf, bytesAvailable);
			*pSize = bytesAvailable;
			*bytes = (BYTE*)buf;
			return true;

		}
	}
	return false;
}

QJsonArray ReadiumJSApi::getJSON(shared_ptr<const ePub3::SMILData::Sequence> seqBody)
{
	QJsonArray ret;
	//TODO: populateChildren
	const shared_vector<const ePub3::SMILData::TimeContainer>::size_type k = seqBody->GetChildrenCount();
	shared_vector<const SMILData::TimeContainer>::size_type i = 0;
	for (; i < k; ++i)
	{
		shared_ptr<const SMILData::TimeContainer> timeContainer = seqBody->GetChild(i);
		QJsonObject obj;

		if (timeContainer->IsParallel())
		{
			auto para = std::dynamic_pointer_cast<const ePub3::SMILData::Parallel>(timeContainer);
			auto audio = para->Audio();
			auto text = para->Text();
			QJsonArray ret1;
			if (audio)
			{
				QJsonObject obj1;

				{
					std::ostringstream stringStream;
					stringStream << audio->ClipEndMilliseconds() / 1000.;
					std::string copyOfStr = stringStream.str();

					obj1["clipEnd"] = copyOfStr;
				}

				obj1["nodeType"] = QString("audio");
				obj1["src"] = QString(audio->SrcFile().c_str());	//"audio\/mobydick_001_002_melville.mp4",

				{
					std::ostringstream stringStream;
					stringStream << audio->ClipBeginMilliseconds() / 1000.;
					std::string copyOfStr = stringStream.str();

					obj1["clipBegin"] = copyOfStr;
				}
				ret1.push_back(obj1);
			}
			if (text)
			{
				QJsonObject obj2;

				obj2["srcFile"] = QString(text->SrcFile().c_str());				//"chapter_002.xhtml",
				obj2["srcFragmentId"] = QString(text->SrcFragmentId().c_str());	//"c02h01",
				obj2["nodeType"] = QString("text");
				obj2["src"] = QString(text->SrcFile().c_str()) + "#" + QString(text->SrcFragmentId().c_str());// "chapter_002.xhtml#c02h01"

				ret1.push_back(obj2);
			}
			obj["children"] = ret1;
			obj["nodeType"] = QString("par");
		}
		else if (timeContainer->IsSequence())
		{
			auto sequence = std::dynamic_pointer_cast<const ePub3::SMILData::Sequence>(timeContainer);
			obj["textref"] = QString(sequence->TextRefFile().c_str());
			//obj["textref"] =
			QJsonArray children = getJSON(sequence);
			obj["children"] = children;
			obj["nodeType"] = QString("seq");
		}

		//obj["nodeType"] = QString(timeContainer->Type().c_str());
		obj["epubtype"] = QString("");

		//"textref" : "",
		//Sequence
		//timeContainer->Name().c_str();
		//"textref" : "",
		//"epubtype" : ""
		//"children" : []
		//qDebug() << obj;
		ret.push_back(obj);
	}
	return ret;
}
QJsonObject ReadiumJSApi::getPackageJSON(PackagePtr pckg)
{
	QJsonObject obj;
#if _USE_READIUM

	// Level 0
	{
		obj["rootUrl"] = QString("/");			//[dict setObject : @"/" forKey:@"rootUrl"];
		obj["rendition_layout"] = QString("");	//[dict setObject : _rendition_layout forKey : @"rendition_layout"];
		obj["rendition_flow"] = QString("");	//[dict setObject : _rendition_flow forKey : @"rendition_flow"];

		QJsonObject spine;

		QJsonArray spineItems;

		size_t idx = 0;
		shared_ptr<SpineItem>   pSpineItem = pckg->SpineItemAt(idx++);
		while (pSpineItem != 0)
		{
			QJsonObject curItem;
			shared_ptr<ManifestItem>    manifestItem = pSpineItem->ManifestItem();
			if (manifestItem)
			{
				//qDebug() << QString(manifestItem->BaseHref().c_str());
				curItem["href"] = QString(manifestItem->BaseHref().c_str());	//[dict setObject : _href forKey : @"href"];
			}
			else
				curItem["href"] = QString("");


			curItem["idref"] = QString(pSpineItem->Idref().c_str());	//[dict setObject : _idref forKey : @"idref"];

			//pSpineItem->Spread()
			//enum class PageSpread
			//{
			//	Automatic,              ///< No value specified by the author.
			//	Left,                   ///< This is the left page of a spread.
			//	Right,                  ///< This is the right page of a spread.
			//	Center,                 ///< This is a double-width page, spread across both left & right.
			//};


			curItem["page_spread"] = QString("");//[dict setObject : _page_spread forKey : @"page_spread"];
			curItem["rendition_layout"] = QString("");;//[dict setObject : _rendition_layout forKey : @"rendition_layout"];
			curItem["rendition_spread"] = QString("");;//[dict setObject : _rendition_spread forKey : @"rendition_spread"];
			curItem["rendition_flow"] = QString("");;//[dict setObject : _rendition_flow forKey : @"rendition_flow"];
			curItem["media_overlay_id"] = QString("");;//[dict setObject : _media_overlay_id forKey : @"media_overlay_id"];
			spineItems.push_back(curItem);

			pSpineItem = pckg->SpineItemAt(idx++);
		}

		spine["items"] = spineItems;
		spine["direction"] = QString("default"); //[dict setObject : _direction forKey : @"direction"];
		obj["spine"] = spine;//[dict setObject : [_spine toDictionary] forKey : @"spine"];



		QJsonObject media_overlay;

		{
			std::shared_ptr<MediaOverlaysSmilModel>   smilModel = pckg->MediaOverlaysSmilModel();
			std::vector<std::shared_ptr<SMILData>>::size_type n = smilModel->GetSmilCount();
			QJsonArray smil_models;
			std::vector<std::shared_ptr<SMILData>>::size_type i = 0;
			for (i = 0; i < n; ++i)
			{
				std::shared_ptr<SMILData> curSmil = smilModel->GetSmil(i);
				QJsonObject smilModel;

				if (curSmil->XhtmlSpineItem())
					smilModel["spineItemId"] = QString(curSmil->XhtmlSpineItem()->Idref().c_str());
				else
					smilModel["spineItemId"] = QString("");

				//smilModel["id"]

				std::ostringstream stringStream;
				stringStream << curSmil->DurationMilliseconds_Calculated() / 1000.;
				std::string copyOfStr = stringStream.str();

				smilModel["duration"] = copyOfStr;

				if (curSmil->SmilManifestItem())
				{
					smilModel["id"] = QString(curSmil->SmilManifestItem()->Identifier().c_str());
					smilModel["href"] = QString(curSmil->SmilManifestItem()->Href().c_str());
				}
				else
				{
					smilModel["id"] = QString("");
					smilModel["href"] = QString("fake.smil");
				}

				smilModel["smilVersion"] = QString("3.0");
				//[dict setObject : self.children forKey : @"children"];


				shared_ptr<const ePub3::SMILData::Sequence> seqBody = curSmil->Body();
				QJsonArray arrChildren = getJSON(seqBody);
				smilModel["children"] = arrChildren;
				//qDebug() << smilModel;

				smil_models.push_back(smilModel);
			}

			/*
			[dict setObject : smilDictionaries forKey : @"smil_models"];
			{
			for (DNSmilModel *mo in _smilModels) {
			[smilDictionaries addObject : [mo toDictionary]];
			}
			[
			[dict setObject : self.id forKey : @"id"];
			[dict setObject : self.spineItemId forKey : @"spineItemId"];
			[dict setObject : self.href forKey : @"href"];
			[dict setObject : self.smilVersion forKey : @"smilVersion"];

			[dict setObject : self.duration forKey : @"duration"];

			[dict setObject : self.children forKey : @"children"];
			]
			}*/
			media_overlay["smil_models"] = smil_models;

			QJsonArray skippables;
			QJsonArray escapables;

			smilModel = pckg->MediaOverlaysSmilModel();
			
			if (smilModel)
			{
				std::vector<string>::size_type nCount = smilModel->GetSkippablesCount();
				for (std::vector<string>::size_type i = 0; i < nCount; ++i)
				{
					string sSkippable = smilModel->GetSkippable(i);
					skippables.push_back(sSkippable.c_str());
				}

				nCount = smilModel->GetEscapablesCount();
				for (std::vector<string>::size_type i = 0; i < nCount; ++i)
				{
					string sEsc = smilModel->GetEscapable(i);
					escapables.push_back(sEsc.c_str());
				}

			}
			media_overlay["skippables"] = skippables;//[dict setObject : self.skippables forKey : @"skippables"];
			media_overlay["escapables"] = escapables;//[dict setObject : self.escapables forKey : @"escapables"];
			media_overlay["duration"] = QString(pckg->MediaOverlays_DurationTotal().c_str()); //[dict setObject : self.duration forKey : @"duration"]; = 1403.5
			media_overlay["narrator"] = QString(pckg->MediaOverlays_Narrator().c_str());//[dict setObject : self.narrator forKey : @"narrator"];
			media_overlay["activeClass"] = QString(pckg->MediaOverlays_ActiveClass().c_str());//[dict setObject : self.activeClass forKey : @"activeClass"];, "activeClass" : "-epub-media-overlay-active",
			media_overlay["playbackActiveClass"] = QString(pckg->MediaOverlays_PlaybackActiveClass().c_str());//[dict setObject : self.playbackActiveClass forKey : @"playbackActiveClass"];
		}
		obj["media_overlay"] = media_overlay; //[dict setObject : [_mediaOverlay toDictionary] forKey : @"media_overlay"];

		return obj;
	}

#endif		
	return obj;
}
void ReadiumJSApi::openBook(PackagePtr pckg, ViewerSettings viewerSettings, OpenPageRequest openPageRequestData)
{
	QJsonObject openBookData;
	try {
		openBookData["package"] = getPackageJSON(pckg);
		openBookData["settings"] = viewerSettings.toJSON();
		openBookData["openPageRequest"] = openPageRequestData.toJSON();
	}
	catch (...)
	{
		//	Log.e(TAG, "" + e.getMessage(), e);
	}
	//catch (JSONException e) {
	//	Log.e(TAG, "" + e.getMessage(), e);
	//}
	std::string str = json::Serialize(openBookData);
	loadJSOnReady("ReadiumSDK.reader.openBook(" + str + ");");
	//loadJSOnReady("alert('Hi!');");
}
void ReadiumJSApi::updateSettings(ViewerSettings viewerSettings)
{
	try {
		std::string str = json::Serialize(viewerSettings.toJSON());
		loadJSOnReady(QString("ReadiumSDK.reader.updateSettings(") + str + ");");
	}
	//catch (JSONException e) {
	//	Log.e(TAG, "" + e.getMessage(), e);
	//}
	catch (...)
	{
		//	Log.e(TAG, "" + e.getMessage(), e);
	}
}
void ReadiumJSApi::openContentUrl(QString href, QString baseUrl)
{
	//baseUrl = getBasePath();	// fast fix
	loadJSOnReady("ReadiumSDK.reader.openContentUrl(\"" + href + "\", \"" + baseUrl + "\");");
}
void ReadiumJSApi::openSpineItemPage(QString idRef, int page)
{
	loadJSOnReady("ReadiumSDK.reader.openSpineItemPage(\"" + idRef + "\", " + std::to_string(page) + ");");
}
void ReadiumJSApi::openSpineItemElementCfi(QString idRef, QString elementCfi)
{
	loadJSOnReady("ReadiumSDK.reader.openSpineItemElementCfi(\"" + idRef + "\",\"" + elementCfi + "\");");
}
void ReadiumJSApi::nextMediaOverlay()
{
	loadJSOnReady("ReadiumSDK.reader.nextMediaOverlay();");
}
void ReadiumJSApi::previousMediaOverlay()
{
	loadJSOnReady("ReadiumSDK.reader.previousMediaOverlay();");
}
void ReadiumJSApi::toggleMediaOverlay()
{
	bMediaOverlayToggled = (bMediaOverlayToggled) ? false : true;
	loadJSOnReady("ReadiumSDK.reader.toggleMediaOverlay();");
}
void ReadiumJSApi::turnMediaOverlayOff()
{
	if (bMediaOverlayToggled)
		toggleMediaOverlay();

}
void ReadiumJSApi::bookmarkCurrentPage() 
{
	loadJS("window.LauncherUI.getBookmarkData(ReadiumSDK.reader.bookmarkCurrentPage());");
}
void ReadiumJSApi::openPageLeft() 
{
	loadJS("ReadiumSDK.reader.openPageLeft();");
}
void ReadiumJSApi::openPageRight()
{
	loadJS("ReadiumSDK.reader.openPageRight();");
}


void ReadiumJSApi::loadJS(QString jScript)
{

#if 0	
	QAxObject* doc = WebBrowser->querySubObject("Document()");
	//IDispatch* Disp;
	IDispatch* winDoc = NULL;
	IHTMLDocument2* document = NULL;

	//332C4425-26CB-11D0-B483-00C04FD90119 IHTMLDocument2
	//25336920-03F9-11CF-8FD0-00AA00686F13 HTMLDocument
	doc->queryInterface(QUuid("{332C4425-26CB-11D0-B483-00C04FD90119}"), (void**)(&winDoc));
	if (winDoc) {
		document = NULL;
		winDoc->QueryInterface(IID_IHTMLDocument2, (void**)&document);
		IHTMLWindow2 *window = NULL;
		document->get_parentWindow(&window);
		QAxObject* documentWin = new QAxObject(document, WebBrowser);
		QAxObject* jScriptWin = new QAxObject(window, WebBrowser);
		//connect(jScriptWin, SIGNAL(exception(int, QString, QString, QString)), this, SLOT(printWinException(int, QString, QString, QString)));
		//jScriptInitialized = true;

		QVariantList params;
		//params.append(javaScript);
		//params.append("alert('Hi')");
		params.append("JScript");
		//QVariant result = jScriptWin->dynamicCall("execScript(QString, QString)", params);
		VARIANT var;
		QString strCode = jScript;
		QString stringLang("JScript");
		BSTR bstrCode = SysAllocString(strCode.toStdWString().c_str());
		BSTR bstrLang = SysAllocString(stringLang.toStdWString().c_str());
		window->execScript(bstrCode, bstrLang, &var);
		SysFreeString(bstrCode);
		SysFreeString(bstrLang);
		document->Release();
		winDoc->Release();
	}
	else {
		//qDebug() << "COULD NOT GET DOCUMENT OBJECT! Aborting";
	}
#else
	if (WebBrowser)
	{
		CComPtr<IDispatch> pDispDoc = WebBrowser->get_Document();
		CComQIPtr<IHTMLDocument2> pHtmlDoc(pDispDoc);
		if (pHtmlDoc)
		{
			CComPtr<IHTMLWindow2>    pMainWin2;
			pHtmlDoc->get_parentWindow(&pMainWin2);

			if (pMainWin2)
			{
				CComVariant    vtRv(0);
				CComBSTR bsCode = jScript.c_str() /*L"alert (\" Hi !\");"*/, bsLang = L"JavaScript";
				HRESULT hr = pMainWin2->execScript(bsCode, bsLang, &vtRv);

				if (!SUCCEEDED(hr))
				{
					//AfxMessageBox(L"Error executing script");
				}
			}
		}
	}


	//QAxObject* qv = WebBrowser->querySubObject("Document()");
	//QAxObject* qv1 = qv->querySubObject("parentWindow");
	//QString script = "(function(){" + jScript + "})()";
	//qDebug() << script;

	/*{
	QString filename = "Data.txt";
	QFile file(filename);
	if (file.open(QIODevice::ReadWrite)) {
	QTextStream stream(&file);
	stream << script << endl;
	}
	file.close();
	}*/

	//QVariantList params;
	//params.append("alert('hello')");
	//params.append("JScript");
	//QVariant result = qv1->dynamicCall("execScript(QString, QString)", params);
	//qv1->dynamicCall("execScript(Qstring)", script, "javascript");
	//qv1->dynamicCall("execScript(Qstring, QString)", script, "JScript");	//, "javascript"
	//qv1->deleteLater();
	//qv->deleteLater();

	//Log.i(TAG, "loadJS: "+jScript);
	//mJSLoader.loadJS("javascript:(function(){" + jScript + "})()");
	//pWebBrowser->dynamicCall(script);

#endif
}
void ReadiumJSApi::loadJSOnReady(QString jScript)
{
	loadJS("$(document).ready(function () {" + jScript + "});");
}
void ReadiumJSApi::initReadiumSDK()
{
	using namespace	ePub3;
	InitializeSdk();
	PopulateFilterManager();
}
ReadiumJSApi::ReadiumJSApi(CExplorer*pWebBrowser_):WebBrowser(pWebBrowser_), bMediaOverlayToggled(false)
{
}
ReadiumJSApi::ReadiumJSApi():WebBrowser(0), bMediaOverlayToggled(false)
{
}
