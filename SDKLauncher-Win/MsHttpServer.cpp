// 
//  SDKLauncherWin.h : main header file for the PROJECT_NAME application
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
//  THE POSSIBILITY OF SUCH DAMAGE.

#include "stdafx.h"
#include "MsHttpServer.h"

void MSHttpAPIWrapper::Cleanup()
{
	if (hReqQueue)
	{
		for (int i = 0; i < (int)arrOfURLs.size(); i++)	// Call HttpRemoveUrl for all added URLs.
		{
			HttpRemoveUrl(hReqQueue, arrOfURLs[i].c_str());
		}
		CloseHandle(hReqQueue);		// Close the Request Queue handle.
		hReqQueue = 0;
	}
	if (bHTTPInitialized)
	{
		HttpTerminate(HTTP_INITIALIZE_SERVER, NULL);// Call HttpTerminate.
		bHTTPInitialized = false;
	}
	freeBufferMem();
}
ULONG MSHttpAPIWrapper::DoReceiveRequests()
{
	ULONG              result;
	HTTP_REQUEST_ID    requestId;
	DWORD              bytesRead;
	PHTTP_REQUEST      pRequest;

	// Wait for a new request. This is indicated by a NULL 
	// request ID.
	pRequest = (PHTTP_REQUEST)pRequestBuffer;
	HTTP_SET_NULL_ID(&requestId);

	for (; !bStop;)
	{
		RtlZeroMemory(pRequest, RequestBufferLength);

		result = HttpReceiveHttpRequest(
			hReqQueue,          // Req Queue
			requestId,          // Req ID
			0,                  // Flags
			pRequest,           // HTTP request buffer
			RequestBufferLength,// req buffer length
			&bytesRead,         // bytes received
			NULL                // LPOVERLAPPED
			);

		//Handle the HTTP Request
		if (NO_ERROR == result)
		{
			//
			// Worked! 
			// 
			switch (pRequest->Verb)
			{
			case HttpVerbGET:
				wprintf(L"Got a GET request for %ws \n", pRequest->CookedUrl.pFullUrl);
				result = SendHttpResponse(pRequest, 200, "OK", "Hey! You hit the server \r\n");
				break;

			case HttpVerbPOST:
				wprintf(L"Got a POST request for %ws \n", pRequest->CookedUrl.pFullUrl);
				result = SendHttpResponse(pRequest, 503, "Not Implemented", NULL);
				break;

			default:
				wprintf(L"Got a unknown request for %ws \n", pRequest->CookedUrl.pFullUrl);
				result = SendHttpResponse(pRequest, 503, "Not Implemented", NULL);
				break;
			}

			if (result != NO_ERROR)
			{
				break;
			}
			HTTP_SET_NULL_ID(&requestId);// Reset the Request ID to handle the next request.
		}
		else if (result == ERROR_MORE_DATA)
		{
			//
			// The input buffer was too small to hold the request
			// headers. Increase the buffer size and call the 
			// API again. 
			//
			// When calling the API again, handle the request
			// that failed by passing a RequestID.
			//
			// This RequestID is read from the old buffer.
			//
			requestId = pRequest->RequestId;

			//
			// Free the old buffer and allocate a new buffer.
			//
			RequestBufferLength = bytesRead;
			allocBufferMem(RequestBufferLength);
			if (pRequestBuffer == NULL)
			{
				result = ERROR_NOT_ENOUGH_MEMORY;
				break;
			}

			pRequest = (PHTTP_REQUEST)pRequestBuffer;
		}
		else if (ERROR_CONNECTION_INVALID == result && !HTTP_IS_NULL_ID(&requestId))
		{
			// The TCP connection was corrupted by the peer when
			// attempting to handle a request with more buffer. 
			// Continue to the next request.
			HTTP_SET_NULL_ID(&requestId);
		}
		else
		{
			break;
		}

	}
	return result;
}
DWORD MSHttpAPIWrapper::SendHttpResponse(IN PHTTP_REQUEST pRequest, IN USHORT StatusCode, IN PSTR pReason, IN PSTR pEntityString)
{
	HTTP_RESPONSE   response;
	HTTP_DATA_CHUNK dataChunk;
	DWORD           result;
	DWORD           bytesSent;

	BYTE* bytes = 0;	// TODO:!!!
	ULONGLONG size = 0;
	std::string mimeTxt;
	bool bRes = getResponseStringAndMime(pRequest->pRawUrl, &bytes, &size, mimeTxt);


	if (!bRes)	// if failed
	{
		INITIALIZE_HTTP_RESPONSE(&response, StatusCode, pReason);
		//
		// Add a known header.
		//

		//[1]
		//CStringA strA(mimeTxt); // a helper string
		//LPCSTR ptr = strA;
		ADD_KNOWN_HEADER(response, HttpHeaderContentType, "text/html");

		if (pEntityString)
		{
			// Add an entity chunk.
			dataChunk.DataChunkType = HttpDataChunkFromMemory;
			dataChunk.FromMemory.pBuffer = pEntityString;
			dataChunk.FromMemory.BufferLength = 4;
			response.EntityChunkCount = 1;
			response.pEntityChunks = &dataChunk;
		}

		// Because the entity body is sent in one call, it is not
		// required to specify the Content-Length.
		result = HttpSendHttpResponse(
			hReqQueue,           // ReqQueueHandle
			pRequest->RequestId, // Request ID
			0,                   // Flags
			&response,           // HTTP response
			NULL,                // pReserved1
			&bytesSent,          // bytes sent  (OPTIONAL)
			NULL,                // pReserved2  (must be NULL)
			0,                   // Reserved3   (must be 0)
			NULL,                // LPOVERLAPPED(OPTIONAL)
			NULL                 // pReserved4  (must be NULL)
			);

		if (result != NO_ERROR)
		{
			wprintf(L"HttpSendHttpResponse failed with %lu \n", result);
		}
		delete[] bytes;

		return result;
	}
	//
	// Initialize the HTTP response structure.
	//
	INITIALIZE_HTTP_RESPONSE(&response, StatusCode, pReason);

	//
	// Add a known header.
	//

	//[1]
	//CStringA strA(mimeTxt); // a helper string
	//LPCSTR ptr = strA;
	ADD_KNOWN_HEADER(response, HttpHeaderContentType, mimeTxt.c_str()/*ptr"text/html"*/);

	if (pEntityString)
	{
		// 
		// Add an entity chunk.
		//
		dataChunk.DataChunkType = HttpDataChunkFromMemory;
		dataChunk.FromMemory.pBuffer = bytes;// pEntityString;
		dataChunk.FromMemory.BufferLength = (ULONG)size;
		//(ULONG)strlen(pEntityString);

		response.EntityChunkCount = 1;
		response.pEntityChunks = &dataChunk;
	}


	// Because the entity body is sent in one call, it is not
	// required to specify the Content-Length.

	result = HttpSendHttpResponse(
		hReqQueue,           // ReqQueueHandle
		pRequest->RequestId, // Request ID
		0,                   // Flags
		&response,           // HTTP response
		NULL,                // pReserved1
		&bytesSent,          // bytes sent  (OPTIONAL)
		NULL,                // pReserved2  (must be NULL)
		0,                   // Reserved3   (must be 0)
		NULL,                // LPOVERLAPPED(OPTIONAL)
		NULL                 // pReserved4  (must be NULL)
		);

	if (result != NO_ERROR)
	{
		wprintf(L"HttpSendHttpResponse failed with %lu \n", result);
	}
	
	delete [] bytes;

	return result;
}
UINT MSHttpAPIWrapper::MSHTTPServerThread(void* pThis_)
{
	MSHttpAPIWrapper *pThis = (MSHttpAPIWrapper*)(pThis_);
	std::vector<std::wstring> arrUrls;	arrUrls.push_back(L"http://localhost:5000/");
	pThis->Init(arrUrls);

	pThis->DoReceiveRequests();	// server main loop

	return pThis->retCode;
}
void MSHttpAPIWrapper::stopMsHTTPServerThread()
{
	if (pHTTPServerThread)
	{
		bStop = true;
		Cleanup();
		//::WaitForSingleObject(pHTTPServerThread->m_hThread, INFINITE);
		//delete pHTTPServerThread;
	}
}

MSHttpAPIWrapper::MSHttpAPIWrapper() :bHTTPInitialized(false), hReqQueue(nullptr), HttpApiVersion(HTTPAPI_VERSION_1), pRequestBuffer(0), pHTTPServerThread(0), bStop(false)
{
}
MSHttpAPIWrapper::~MSHttpAPIWrapper()
{
	stopMsHTTPServerThread();
	Cleanup();
}
void MSHttpAPIWrapper::Init(std::vector<std::wstring>& rArrOfURLs)
{
	arrOfURLs = rArrOfURLs;

	retCode = HttpInitialize(
		HttpApiVersion,
		HTTP_INITIALIZE_SERVER,    // Flags
		NULL                       // Reserved
		);

	if (retCode != NO_ERROR)
	{
		wprintf(L"HttpInitialize failed with %lu \n", retCode);
		return;
	}
	bHTTPInitialized = true;

	// Create a Request Queue Handle
	retCode = HttpCreateHttpHandle(&hReqQueue, 0                  // Reserved
		);

	if (retCode != NO_ERROR)
	{
		wprintf(L"HttpCreateHttpHandle failed with %lu \n", retCode);
		Cleanup();
		return;
	}

	// Register the URLs to Listen On
	//
	// The command line arguments represent URIs that to 
	// listen on. Call HttpAddUrl for each URI.
	//
	// The URI is a fully qualified URI and must include the
	// terminating (/) character.
	//
	for (int i = 0; i < (int)arrOfURLs.size(); i++)
	{
		wprintf(L"listening for requests on the following url: %s\n", arrOfURLs[i]);

		retCode = HttpAddUrl(hReqQueue, arrOfURLs[i].c_str(), NULL);

		if (retCode != NO_ERROR)
		{
			wprintf(L"HttpAddUrl failed with %lu \n", retCode);
			Cleanup();
			return;
		}
	}
	//
	// Allocate a 2 KB buffer. This size should work for most 
	// requests. The buffer size can be increased if required. Space
	// is also required for an HTTP_REQUEST structure.
	//
	
	RequestBufferLength = sizeof(HTTP_REQUEST) + 2048;
	allocBufferMem(RequestBufferLength);

	if (pRequestBuffer == NULL)
	{
		Cleanup();
		return;

	}
}
void MSHttpAPIWrapper::allocBufferMem(int iMemSize)
{
	freeBufferMem();
	pRequestBuffer = (PCHAR)ALLOC_MEM(iMemSize);
}
void MSHttpAPIWrapper::freeBufferMem()
{
	if (pRequestBuffer)
	{
		FREE_MEM(pRequestBuffer);
		pRequestBuffer = 0;
	}
}

void MSHttpAPIWrapper::startMsHTTPServerThread()
{
	pHTTPServerThread = AfxBeginThread(MSHttpAPIWrapper::MSHTTPServerThread, this);
	pHTTPServerThread->m_bAutoDelete = true;
}
